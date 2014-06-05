/*
#define EL2014
#define TST_EL2014

#ifdef EL2014
extern char my_msg[];
extern void my_Trace(void);
#endif
*/
/**************************************************************************
<MODULE NAME>
<FILENAME> mod_vote.c </FILENAME>
<HISTORY>
  <MODIFICATION>
    <DATE> </DATE>
    <AUTHOR>  </AUTHOR>
    <DESCRIPTION>
    Clean_Memory
    Vote_Start (main loop... waiting for a card)
    Vote_Empty
    Vote_Write
    Vote_End
    Vote_Encode
    </DESCRIPTION>
  </MODIFICATION>
</HISTORY>
</MODULE NAME>
**************************************************************************/


/**************************************************************************
INCLUDE FILES
**************************************************************************/
#include <bios.h>
#include <io.h>
#include <fcntl.h>
#include <alloc.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <graphics.h>
#include "..\CrdReade.r\LibCrdRd.h"
#include "..\LibAlrBx\LIBALRBX.H"
#include "defs.h"
#include "cfgscrut.h"
#include "scrutin.h"

#ifdef EL2014
#include "util_mav.h"
#endif


/**************************************************************************
EXTERNAL VARIABLES
**************************************************************************/
#ifdef EL2014
extern char   gszArrond[4];
extern int    giNbrScrutin;
extern int    swVLR;
#endif
extern char   gcCodeLg;
extern char   gcCodeLgOri;
extern char   gcLg;
extern int    giCardNBytes;
extern int    giKlavier;
extern int    giSWCard;
extern s_CFGSCRUT gshCfgScrut[NMX_SCRUTIN_T+1];
extern s_Scrutin  gshScrutin[10];
extern char   gszCrcCalcul[5];
extern char   gszJeton[16];
extern char   gszJetonMini[6];
extern unsigned char gucMavType;
extern char **pszMsg;
extern char  *pszMsg1[N_SCR_MESSAGE];

/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/
#ifdef EL2014
extern int les_listes();
#endif
extern void  Calcul_Crc(char *, int);
extern void  Card_Crc_Read(char *, char *);
extern int   Card_Eject(void);
extern void  Card_RW(char *);
extern void  Card_Jeton(char *, char *);
extern void  Leds(char, int);
extern int   Lg_Change(void);
extern unsigned char Lg_Select(void);
extern int   Lg_Confirm(void);
extern int   List_Display(void);
extern int   Pen_Capture(int *, int *);
extern int (*ptfunct)(void);
extern void  Screen_Ask_Viewing(void);
extern int   Screen_Ask_Visu(void);
extern void  Screen_Bad_Card(void);
/* << 9.01b */
extern void  Screen_Bad_Card_Blocked(void);
/* 9.01b >> */
extern void  Screen_Eject_Card(void);
extern void  Screen_Incompatible_Card(void);
extern void  Screen_Interrupt(void);
extern void  Screen_LigthPen(unsigned char);
extern void  Screen_New_Vote(void);
extern void  Screen_Take_Back(void);
extern void  Screen_Technical_Error(void);
extern void  Screen_Vote_End(void);
extern void  Screen_Vote_Empty_Confirmation(void);
extern int   Scrut_Loop(void);
extern void  Set_Cursor(int);
extern int   View_Vote(void);
extern void  Wait_Loop(int);
extern void  Wait_Loop_Int(int);

/**************************************************************************
LOCAL FUNCTIONS
**************************************************************************/
int Vote_End(void);

/**************************************************************************
GLOBAL VARIABLES
**************************************************************************/
char arcMemoScrutin[NB_MAX_SCRUT];
char arcMemoList[NB_MAX_SCRUT][NB_MAX_LISTS];
char arcMemoCandid[NB_MAX_SCRUT][NB_MAX_LISTS][100];
char gcCardRW; /* CARD_WRITE, CARD_READ */
int  giCodeVoteEnd;
int  giCurrentScr = 0;
int  giCurrentList = 0;
int  giCurrentCandid = 0;
unsigned char guszCard[75];

/**************************************************************************
<FUNCTION>
  <NAME> Clean_Memory </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  resets globals variables (starting a new vote)
  (arcMemoCandid, arcMemoList, arcMemoScrutin)
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
static void Clean_Memory(void)
{
  int x,y,z;

  for(x=0; x<NB_MAX_SCRUT; x++)
    for(y=0; y<NB_MAX_LISTS; y++)
      for(z=0; z<NB_MAX_CANDID; z++)
        arcMemoCandid[x][y][z] = 0;

  for(x=0; x<NB_MAX_SCRUT; x++)
    for(y=0; y<NB_MAX_LISTS; y++)
      arcMemoList[x][y] = 0;

  for(x=0; x<NB_MAX_SCRUT; x++)
    arcMemoScrutin[x] = 0;
}

/**************************************************************************
<FUNCTION>
  <NAME> Vote_Start </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
    Starting a new vote
    Waiting for a card
    -> Validation of the "jeton" (token)
    -> Test CRC
    -> the card contains a vote
      + allows the visualization
      - start a new vote process
  </DESCRIPTION>
  <RETURNS>
    <TYPE> int </TYPE>
    <DESCRIPTION> cfr "int (*ptfunct)(void)" </DESCRIPTION>
  </RETURNS>
</FUNCTION>
**************************************************************************/
int Vote_Start(void)
{
  int iSWWaitingCard, iStatus, iPenX, iPenY, iPenCapture, i, nb_read = 3;
  int iHwdDisconnected;
  char szJeton[6], szBuffRead[75], szCrc[5];
  unsigned char ucCardReadAtStart_JIT = 0;
  s_Parameters shParametersMagnCard;
  static int iAfter3TimesBlockTheCard = 0;

  giCurrentScr = giCurrentList = iStatus = giCodeVoteEnd = 0;
  gcCardRW = CARD_WRITE;
  giKlavier = 1;

  memset(szBuffRead, '\0', sizeof(szBuffRead));

  /* switch off the leds */
  Leds(OFF,0);

  if(gucMavType == MAV_JITES_1 || gucMavType == MAV_JITES_2)
  {
    shParametersMagnCard.iIoAddr = 0x300;
    shParametersMagnCard.iIrq = 5;
    shParametersMagnCard.iPort = 1;
    shParametersMagnCard.iSetting = _COM_CHR7 | _COM_STOP1 | _COM_EVENPARITY| _COM_9600;
    Lecteur_Init(&shParametersMagnCard);

    iStatus = Read_Commande(szBuffRead);
    if(iStatus == 0) ucCardReadAtStart_JIT = 1;
    else
    {
      if(iStatus == READING_ERROR)
      {
        giCodeVoteEnd = READING_ERROR;
        ptfunct = Vote_End;
        return(0);
      }
    }
  }

  /* Reset previous vote (Free memory) */
  Clean_Memory();

  /* Is the current language the original one ? */
  if(gcCodeLg != gcCodeLgOri)
    Lg_Change();

  /* Display Screen (Waiting for a card) */
  Screen_New_Vote();

  /* In expectation of a card */
  if(!ucCardReadAtStart_JIT)
    iSWWaitingCard = Wait_Card();
  else /* Under JITES, card already inserted (and read) */
    iSWWaitingCard = CARD_HOME;

  while(iSWWaitingCard != CARD_HOME)
  {
    if(iSWWaitingCard != CARD_GATE && iSWWaitingCard != CARD_NO)
    {
      giCodeVoteEnd = iSWWaitingCard;
      ptfunct = Vote_End;
      return(0);
    }

    /* While waiting we can
        - change the current language (in some cases)
        - display the "jeton"
        - take off the card */
    iHwdDisconnected = iPenCapture = Pen_Capture(&iPenX, &iPenY);
    if(iPenCapture == 1)
    {
      if(iPenY > 450)
      {
        if(iPenX > 110 && iPenX < 530) /* Change current language */
        {
          if(gcCodeLg == '1' || gcCodeLg == '2')
          {
            Lg_Change();
            Screen_New_Vote();
          }
        }
        if(iPenX > 535) /* Display jeton */
        {
          settextjustify(LEFT_TEXT, CENTER_TEXT);
          settextstyle( SMALL_FONT, HORIZ_DIR, 5);
          outtextxy (30, 40, MAV_VERSION);
          outtextxy (30, 60, gszJeton);
        }
      }
      else
      { /* To take back the card (button pressed + pen)*/
        if ((iPenY <= 35) && (iPenX >= 550))
        {
          iHwdDisconnected = iPenCapture = Pen_Capture(&iPenX, &iPenY);
          if(!giKlavier)
          {
            giCodeVoteEnd = 13;
            ptfunct = Vote_End;
            return(0);
          }
        }
      }
    }
    PEN_OR_ALARMBOX_DISCONNECTED

    iSWWaitingCard = Wait_Card();
  }

  /* Card inserted -> read */
  if(ucCardReadAtStart_JIT)
    if(strlen(szBuffRead) == giCardNBytes)
      nb_read = 0;

  for(i=0; i<nb_read; ++i)
  {
    iStatus = Read_Commande(szBuffRead);

    /* Test read status */
    if(iStatus != 0) break;

    /* Numb of characters */
    if(strlen(szBuffRead) == giCardNBytes) break;
#ifdef TST_EL2014
    else {
        strcpy(szBuffRead, gszJetonMini);
        strcat(szBuffRead, "05321N                                         ");
        break;
    }
#endif      
  }

  if(i && i == nb_read) iStatus = 12; /* F R A U D E */

  /* If two languages ... select one */
  if(gcCodeLg == '4' ||
     gcCodeLg == '5' ||
/* << 9.01c */
     /*gcCodeLg == '6' )*/
     gcCodeLg == '6' ||
     gcCodeLg == '8' )
/* 9.01c >> */
  {
    do
    {
      iHwdDisconnected = iPenCapture = Lg_Select();
      PEN_OR_ALARMBOX_DISCONNECTED

      iHwdDisconnected = iPenCapture =  Lg_Confirm();
      PEN_OR_ALARMBOX_DISCONNECTED
    } while(!iPenCapture);
  }
  /* Bad card ? */
/* << 9.01b
  /*if(iStatus != 0)
  {
    if(iAfter3TimesBlockTheCard < 2)
      giCodeVoteEnd = iStatus;
    else
      giCodeVoteEnd = 41; / Keep the card /
    ++iAfter3TimesBlockTheCard;

    ptfunct = Vote_End;
    return(0);
  } */
  
 if(iStatus != 0)
  {
    if(iStatus == 10 || iStatus == 12) /* F R A U D E */
    {
      iAfter3TimesBlockTheCard = 0;
      giCodeVoteEnd = iStatus;
      ptfunct = Vote_End;
      return(0);
    }

    if(iAfter3TimesBlockTheCard < 2)
    {
      ++iAfter3TimesBlockTheCard;
      giCodeVoteEnd = iStatus;
    }
    else
    {
      iAfter3TimesBlockTheCard = 0;
      giCodeVoteEnd = 41; /* Keep the card */
    }

    ptfunct = Vote_End;
    return(0);
  }
/* 9.01b >> */
  iAfter3TimesBlockTheCard = 0;

  strcpy(guszCard,szBuffRead);
  /* Validation of the "jeton" */
  Card_Jeton(guszCard, szJeton);
  iStatus = strcmp(gszJetonMini, szJeton);

  /* Test Crc */
  Calcul_Crc(guszCard,_C_TEST_PLACE);
  Card_Crc_Read(guszCard, szCrc);
  
#ifdef EL2014
  sprintf(my_msg, "Vote_Start - gszJetonMini [%s] - szJeton [%s]\n", gszJetonMini, szJeton); 
  my_Trace();
  sprintf(my_msg, "Vote_Start - gszCrcCalcul [%s] - szCrc [%s]\n", gszCrcCalcul, szCrc); 
  my_Trace();
#endif  
  
#ifdef TST_EL2014
  /* bypassing jeton and CRC checks */
  if (0)
#else 
  if((iStatus != 0) || (strcmp(gszCrcCalcul,szCrc) !=0))
#endif      
  {
    giCodeVoteEnd = 12;
    ptfunct = Vote_End;
    return(0);
  }

  /* Does the card already contain a vote ? */
  Card_RW(guszCard);
  
  if(giSWCard != _NOCARD)
  {
    if(gcCardRW == CARD_READ)
    { /* Vote_View (through Vote_End) */
      giCodeVoteEnd = 11;
      ptfunct = Vote_End;
      return(0);
    }
  }
    
#ifdef EL2014
  //071113 swVLR = 0;
#endif

  /* Go for a loop */
  ptfunct = Scrut_Loop;
  return(0);
}

/**************************************************************************
<FUNCTION>
  <NAME> Vote_Empty </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Displays the empty vote screen and asks for validation
  </DESCRIPTION>
  <RETURNS>
    <TYPE> int </TYPE>
    <DESCRIPTION> cfr "int (*ptfunct)(void)" </DESCRIPTION>
  </RETURNS>
</FUNCTION>
**************************************************************************/
int Vote_Empty(void)
{
  int iPenX, iPenY;

  /* Display screen */
  Screen_Vote_Empty_Confirmation();

  Set_Cursor(CURSOR_XY);

  while(giKlavier)
  {
    Set_Cursor(CURSOR_ON);
    if(Pen_Capture(&iPenX, &iPenY))
    {
      Set_Cursor(CURSOR_OFF);
      if((iPenY > 200) && (iPenY < 290))
      {
        if((iPenX > 40) && (iPenX < 300)) /* Validated */
        {
          ++giCurrentScr;
          ptfunct = Scrut_Loop;
          return(0);
        }
        else
        {
          if((iPenX > 340 ) && (iPenX < 600)) /* Canceled */
          {
#ifdef EL2014
            ptfunct = les_listes;
#else        
            ptfunct = List_Display;
#endif      
            return(0);
          }
        }
      }
    }
  }

  Set_Cursor(CURSOR_OFF);

  giCodeVoteEnd = 13;
  ptfunct = Vote_End;
  return(0);
}

/**************************************************************************
<FUNCTION>
  <NAME> Vote_Write </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Write card
  </DESCRIPTION>
  <RETURNS>
    <TYPE> int </TYPE>
    <DESCRIPTION> cfr "int (*ptfunct)(void)" </DESCRIPTION>
  </RETURNS>
</FUNCTION>
**************************************************************************/
int Vote_Write(void)
{
  int iStatus;
  char szBuffRead[75];

#if (LOOPWRITE_CARTE >= 1)
#  define LCARTEBAK  75
   unsigned char carteBAK[LCARTEBAK];
   int iWriteCard = 0;
   int iWriteCardMax = 3;
#endif

  Screen_Vote_End();

  giCodeVoteEnd = 17;

#if (NOCARD_CHECK >= 1)
  if(giSWCard != _NOCARD) { /* BEGIN WITH Carte */
#endif

#if (LOOPWRITE_CARTE == 0)
    iStatus = Write_Commande((const char*)guszCard);
    if(iStatus != 0) giCodeVoteEnd = 3;
    else             giCodeVoteEnd = 17;
#endif

#if (LOOPWRITE_CARTE >= 1)
    memcpy(carteBAK, guszCard, (size_t)LCARTEBAK);
    for(iWriteCard = 1; iWriteCard <= iWriteCardMax; iWriteCard++)
    {
#ifdef EL2014
  sprintf(my_msg, "Vote_Write - [%s]\n", guszCard); 
  my_Trace();
#endif  
      iStatus = Write_Commande((const char*)guszCard);
      if(iStatus != 0)
      {
        giCodeVoteEnd = 3;
      }
      else
      {
        szBuffRead[0] = '\0';
        iStatus = Read_Commande(szBuffRead);
        if (iStatus == 0)
        {
          /* Verification (read the card and compare) */
          if(strcmp(carteBAK, szBuffRead) == 0)
          {
            giCodeVoteEnd = 17;
            break;
          }
        }
      }
    }
    if(iWriteCard > iWriteCardMax)
    {
      giCodeVoteEnd = 3;
      carteBAK[0] = ' ';
      carteBAK[1] = '\0';
      Write_Commande((const char*)carteBAK);
    }
#endif
#if (NOCARD_CHECK >= 1)
  } /* END WITH Carte */
#endif

  ptfunct = Vote_End;
  return(0);
}

/**************************************************************************
<FUNCTION>
  <NAME> Vote_End </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  The end of a vote
  -> interrupted
  -> "well done"
  -> error on reading, writing
  </DESCRIPTION>
  <RETURNS>
    <TYPE> int </TYPE>
    <DESCRIPTION> cfr "int (*ptfunct)(void)" </DESCRIPTION>
  </RETURNS>
</FUNCTION>
**************************************************************************/
int Vote_End(void)
{
  char szBuffRead[75];
  int iPenX, iPenY, iPenCapture, iHwdDisconnected, t;
  s_Parameters shParametersMagnCard;
  unsigned long far *timer;
  unsigned long temptimes;
  FILE *fu;

  switch(giCodeVoteEnd)
  {
    case 0:
      ptfunct = Vote_Write;
      return(0);
    case 3: /* Writing Error */
      Screen_Technical_Error();

      Read_Commande(szBuffRead);
      /* switch on the red and green leds + bips */
      Leds(RG,1);
      giKlavier = 1;
      while(giKlavier) Pen_Capture(&iPenX, &iPenY);
      if(!Card_Eject()) return(0);
      Wait_Loop(8);
      break;
    case 2:  /* Read / Write error */
    case 4:
      /* switch on the red leds */
      Leds(RR,0);
      Screen_Bad_Card();

      Wait_Loop(2);
      if(!Card_Eject()) return(0);
      Wait_Loop(2);
      break;

    case 41:
/* << 9.01b */
/*      Screen_Bad_Card();*/
      Screen_Bad_Card_Blocked();
/* 9.01b >> */

      /* switch on the green leds */
      Leds(GG,1);

      while(giKlavier) Pen_Capture(&iPenX, &iPenY);
      if(!Card_Eject()) return(0);
      break;

    case 5:
    case 6:         /* Probleme technique */
    case 9:
      shParametersMagnCard.iIoAddr = 0x300;
      shParametersMagnCard.iIrq = 5;
      shParametersMagnCard.iPort = 1;
      shParametersMagnCard.iSetting = _COM_CHR7 | _COM_STOP1 | _COM_EVENPARITY| _COM_9600;
      Lecteur_Init(&shParametersMagnCard);

      Screen_Technical_Error();
      /* switch on the green led */
      Leds(GG,0);
      Wait_Loop(2);
      if(!Card_Eject()) return(0);
      Wait_Loop(2);
      break;

    case 8:
      Wait_Loop(1);
      ptfunct = Vote_Start;
      return(0);

    case 11: /* already done */
      /* switch on the green led */
      Leds(GG,0);
      Screen_Ask_Viewing();

      while(1)
      {
        Set_Cursor(CURSOR_ON);
        iHwdDisconnected = iPenCapture = Pen_Capture(&iPenX, &iPenY);
        if(iPenCapture == 1)
        {
          Set_Cursor(CURSOR_OFF);
          if((iPenY > 375) && (iPenY < 445))
          {
            if((iPenX > 108) && (iPenX <  282))
            {
#ifdef EL2014
              gcCardRW = CARD_WRITE;
#else        
              gcCardRW = CARD_READ;
#endif      
              break;
            }
            else
            {
              if((iPenX >  358) && (iPenX < 532))
              {
#ifdef EL2014
                gcCardRW = CARD_READ;
#else        
                gcCardRW = CARD_WRITE;
#endif      
                break;
              }
            }
          }
        }
        PEN_OR_ALARMBOX_DISCONNECTED
      }
      /* Read card */
      if(gcCardRW == CARD_READ)
      {
        ptfunct = View_Vote;
        return(0);
      }
      giCodeVoteEnd = 16;
	  
      ptfunct    = Vote_End;
      return(0);
    case 10:
    case 12: /* F R A U D E */
      Screen_Incompatible_Card();
      /* switch on the red and green leds + bips */
      Leds(RG,1);
      while(giKlavier) Pen_Capture(&iPenX, &iPenY);
      if(!Card_Eject()) return(0);
      break;

    case 13: /* Eject card */
      /* switch on the red and green leds */
      Leds(RG,0);
      Screen_Eject_Card();
      Wait_Loop(2);
      if(!Card_Eject()) return(0);
      Wait_Loop(2);
      break;
    case 14:   /* Eject card MAV not ready */
      /* switch on the red and green leds */
      Leds(RG,0);
      if(!Card_Eject()) return(0);
      Wait_Loop(5);
      break;
    case 15:
      break;
    case 16: /* End visu */
      Screen_Take_Back();
      if(!Card_Eject()) return(0);
      Wait_Loop(2);
      break;
    case 17: /* Vote done -> take back the card */
      Screen_Interrupt();
      if(!Card_Eject()) return(0);
/* << 9.03 */
      /*Wait_Loop(6);*/
      if(gcLg != 'A')
        Wait_Loop(6);
      else /* Message en Allemand -> temps de lecture + long */
        Wait_Loop(9);
/* 9.03 >> */
      break;
    case CARD_GATE:
      break;

    case 28: /* Light pen reconnected */
      Screen_LigthPen(1);
      /* switch on the green led */
      Leds(GG,0);

      while(giKlavier) Pen_Capture(&iPenX, &iPenY);

      shParametersMagnCard.iIoAddr = 0x300;
      shParametersMagnCard.iIrq = 5;
      shParametersMagnCard.iPort = 1;
      shParametersMagnCard.iSetting = _COM_CHR7 | _COM_STOP1 | _COM_EVENPARITY| _COM_9600;
      Lecteur_Init(&shParametersMagnCard);
      if(!Card_Eject()) return(0);
      Wait_Loop(2);
      break;
  }

  ptfunct = Vote_Start;

  return(0);
}

/**************************************************************************
<FUNCTION>
  <NAME> Vote_Encode </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Vote_Encode(char *_szCarte, int _iScrutin, int _iList, int *_iDebut)
{
  char strtmp[10];
  char strurn[26],c;
  int i,j,z;
  char codage[] = "0123456789ABCDEF";
  struct
  {
    unsigned ch0 : 1;
  } ch[NB_MAX_CANDID];
#ifdef EL2014     
  int nbMaxBytes;
  int corrected;
#endif 
  
#ifdef EL2014     
  /* no BV if BXL/F */
  if (strcmp(gszArrond,"002") == 0 && gshScrutin[_iScrutin].TYPE_SCRUTIN == IS_CONSEIL_FLAMANT && swVLR == 0) return;
  /* if BV add MARK */
  if (gshScrutin[_iScrutin].TYPE_SCRUTIN == IS_CONSEIL_FLAMANT) addVoteMark(_szCarte, *_iDebut-1);
#endif 

  sprintf(strtmp,"%2d",_iList);
  
  for(i=0; i<_C_LISTE_MAX_BYTE; _szCarte[(*_iDebut)++] = strtmp[i], i++);

  for(z=0; z<NB_MAX_CANDID; z++)
    ch[z].ch0 = 0; /* Reset */

  for(z=0; z<NB_MAX_CANDID; z++)
    if(arcMemoCandid[_iScrutin][_iList - 1][z] == 1)
      ch[z].ch0 = 1;

  for(i = 0, j = 0; (i < gshCfgScrut[gshScrutin[_iScrutin].TYPE_SCRUTIN].nmxV);)
  {
    c = 0x00;
    c = (ch[i++].ch0) * 8;
    c+= (ch[i++].ch0) * 4;
    c+= (ch[i++].ch0) * 2;
    c+= (ch[i++].ch0) * 1;
    z = c;
    strurn[j++] = codage[z];
  }
  strurn[j] = 0x00;

#ifdef EL2014
  /* corrected = true; */
  /* no BV if BXL/F */
  /* if (strcmp(gszArrond,"002") == 0 && gshScrutin[_iScrutin].TYPE_SCRUTIN == IS_REGION && swVLR == 0) corrected = false; */
  
  nbMaxBytes = getVoteMaxBytes(_iScrutin,true);
  for(i=0;
      i<nbMaxBytes;
      _szCarte[(*_iDebut)++] = strurn[i], i++) ;
#else
  for(i=0;
      i<gshCfgScrut[gshScrutin[_iScrutin].TYPE_SCRUTIN].nmxB;
      _szCarte[(*_iDebut)++] = strurn[i], i++) ;
#endif    
  
  _szCarte[(*_iDebut)] = '\0';  
}
