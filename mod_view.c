/*
#define EL2014
#define TST_EL2014
*/

/**************************************************************************
<MODULE NAME>
<FILENAME> mod_view.c </FILENAME>
<HISTORY>
  <MODIFICATION>
    <DATE> </DATE>
    <AUTHOR>  </AUTHOR>
    <DESCRIPTION>
    Voting Card already used -> can just read it
    </DESCRIPTION>
  </MODIFICATION>
</HISTORY>
</MODULE NAME>
**************************************************************************/

/**************************************************************************
INCLUDE FILES
**************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>
#include "defs.h"
#include "cfgscrut.h"
#include "scrutin.h"
#include "libcrdrd.h"
#include "..\LibAlrBx\LIBALRBX.H"

#ifdef EL2014
#include "util_mav.h"
#endif

/**************************************************************************
MACRO
**************************************************************************/
#define  _Blanc_Exp   0
#define  _Vote        1
#define  _Pas_De_Vote 2

/**************************************************************************
EXTERNAL VARIABLES
**************************************************************************/
#ifdef EL2014
extern char          gszArrond[4];
#endif
extern char          arcMemoCandid[NB_MAX_SCRUT][NB_MAX_LISTS][NB_MAX_CANDID];
extern char          aszTabEff[NB_MAX_SCRUT][NB_MAX_LISTS];
extern char          aszTabSuppl[NB_MAX_SCRUT][NB_MAX_LISTS];
extern int           gaiNbListScrut[NB_MAX_SCRUT];
extern char          gcCardRW;
extern int           giCodeVoteEnd;
extern int           giCurrentList;
extern int           giCurrentScr;
extern int           giKlavier;
extern s_Scrutin     gshScrutin[10];
extern char        **pszMsg;
extern int           giNbrScrutin;
extern int           giSWUseParamFile;
extern s_CFGSCRUT    gshCfgScrut[NMX_SCRUTIN_T+1];
extern s_List       *gshList;
extern unsigned char guszCard[75];

/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/
#ifdef EL2014
extern int affichc(void);
#endif

#ifdef TST_EL2014
extern void Card_UnWritten(char *);
#endif

extern int   Cand_Display(void);
extern int   Msg_Election(void);
extern int   Pen_Capture(int *, int *);
extern int (*ptfunct)(void);
extern void  Screen_View_Vote_Empty(void);
extern void  Screen_Viewing_Next(void);
extern void  Set_Cursor(int);
extern int   Vote_End(void);
extern void  Wait_Loop_Int(int);

/**************************************************************************
GLOBAL VARIABLES
**************************************************************************/
#ifdef EL2014
extern int swVLR;
#endif

/**************************************************************************
LOCAL FUNCTIONS
**************************************************************************/
static void View_Vote_Empty(void);

/**************************************************************************
<FUNCTION>
  <NAME> View_Vote </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Reads the magnetic card and display the votes
  </DESCRIPTION>
  <RETURNS>
    <TYPE> int </TYPE>
    <DESCRIPTION> cfr "int (*ptfunct)(void)" </DESCRIPTION>
  </RETURNS>
</FUNCTION>
**************************************************************************/
#ifdef TST_EL2014 
extern int Tst_View_Vote(void);    
int View_Vote(void)
{
  int ret;
  
  ret = Tst_View_Vote();
  
  /* Mark the card as unused */
  Card_UnWritten(guszCard);
  Write_Commande((const char*)guszCard);
  
  return (ret);
}


int Tst_View_Vote(void)
#else
int View_Vote(void)
#endif  
{
  unsigned int Position;
  int  Param,VParam;
  char Nul_StrL[3],bufscr[2];
  int  Compt,tot,i,z,iMsgId;
#ifdef EL2014     
  int nbMaxVotes;
  int corrected;
#endif  

  /* Good Card ? */
  Nul_StrL[2] = 0x00;
  for(Compt=_C_VOTE_PLACE, tot = 0,i = 0; i < giNbrScrutin; i++)
  {
    tot +=2;
#ifdef EL2014
    /* tot += getVoteMaxBytes(i,true); */
#else
    tot += gshCfgScrut[gshScrutin[i].TYPE_SCRUTIN].nmxB;
#endif    	
  }
  tot += _C_VOTE_PLACE;
#ifdef EL2014
  tot = MAX_CARD_BYTES;
#endif    	
  --tot;

  if(((guszCard[tot]) == 0x00)  || ((guszCard[tot + 1]) != 0x00))
  { /* Bad Card */
    giCodeVoteEnd = 12;
    ptfunct = Vote_End;
    return(0);
  }

#ifdef EL2014
  swVLR = isVoteMark(guszCard);
#endif        

  /* Start the display */
  for(Compt = _C_VOTE_PLACE, giCurrentScr = 0; giCurrentScr < giNbrScrutin; ++giCurrentScr)
  {
    strncpy(Nul_StrL,&guszCard[Compt] ,2);
    giCurrentList = atoi(Nul_StrL);
    
#ifdef EL2014
    if(strcmp(gszArrond,"002") == 0 && gshScrutin[giCurrentScr].TYPE_SCRUTIN == IS_CONSEIL_FLAMANT && !isVoteMark(guszCard)) {
        /* no BV if BXL/F */
        continue;
    }
#endif    

    if((giCurrentList < 98) && (giCurrentList > (gaiNbListScrut[giCurrentScr] - 1)))
    {
      giCodeVoteEnd = 12;
      ptfunct = Vote_End;
      return(0);
    }

    if(giCurrentList == 98) /* 98 -> Stranger ... vote not allowed */
    {
      Compt += 2;
#ifdef EL2014
      Compt += getVoteMaxBytes(giCurrentScr,true);
#else
      Compt += gshCfgScrut[gshScrutin[giCurrentScr].TYPE_SCRUTIN].nmxB;
#endif        
      continue;
    }

    gshList = gshScrutin[giCurrentScr].ptlist;
    setfillstyle(SOLID_FILL, WHITE);
    bar(0,0,639,479);
    setcolor( BLACK );
#ifdef EL2014
    settextstyle( SMALL_FONT, HORIZ_DIR, 7);
#else
    settextstyle( SMALL_FONT, HORIZ_DIR, 9);
#endif    
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    /* Looking for the message id corresponding to the current election */
    iMsgId = Msg_Election();
    if(iMsgId == 9)
       {
            if(strcmp(gszArrond,"005") == 0 ) iMsgId+=1;
            if(strcmp(gszArrond,"002") == 0 ) iMsgId+=2;
       }  
    outtextxy( 320, 200, pszMsg[iMsgId]);
    outtextxy( 321, 200, pszMsg[iMsgId]);
    Wait_Loop_Int(3);

    if(giCurrentList == 0)
      Param = _Pas_De_Vote;
    else
    {
      if(giCurrentList == 99)
        Param = _Blanc_Exp;
      else
      {
        giCurrentList--;
        gshList = gshScrutin[giCurrentScr].ptlist;
        gshList += giCurrentList;
        gshList = gshScrutin[giCurrentScr].ptlist;
        Param = _Vote;
      }
    }
    gshList = gshScrutin[giCurrentScr].ptlist;
    Compt +=2;
    
    if(Param == _Vote)
    {
      gshList += giCurrentList;
#ifdef EL2014   
      /* corrected = true; */
      /* no BV if BXL/F */
      /* if (strcmp(gszArrond,"002") == 0 && gshScrutin[giCurrentScr].TYPE_SCRUTIN == IS_REGION && !isVoteMark(guszCard)) corrected = false; */
  
      nbMaxVotes = getVoteMaxVotes(giCurrentScr,true);
      for(z = 0,VParam = 0; (z <nbMaxVotes);Compt++)
#else
      for(z = 0,VParam = 0; (z < gshCfgScrut[gshScrutin[giCurrentScr].TYPE_SCRUTIN].nmxV);Compt++)
#endif    
      {
        tot = aszTabEff[giCurrentScr][giCurrentList] + aszTabSuppl[giCurrentScr][giCurrentList] + 1;
        switch(guszCard[Compt])
        {
          case '1' :
            if(z > tot) VParam+=1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            break;
          case '2' :
            if(z > tot) VParam+=1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            break;
          case '3' :
            if(z > tot) VParam+=2;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            break;
          case '4' :
            if(z > tot) VParam+=1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            break;
          case '5' :
            if(z > tot) VParam+=2;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            break;
          case '6' :
            if(z > tot) VParam+=2;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            break;
          case '7' :
            if(z > tot) VParam+=3;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            break;
          case '8' :
            if(z > tot)  VParam+=1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            break;
          case '9' :
            if(z > tot) VParam+=2;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            break;
          case 'A' :
            if(z > tot) VParam+=2;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            break;
          case 'B' :
            if(z > tot) VParam+=3;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            break;
          case 'C' :
            if(z > tot) VParam+=2;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            break;
          case 'D' :
            if(z > tot) VParam+=3;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            break;
          case 'E' :
            if(z > tot) VParam+=3;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            break;
          case 'F' :
            if(z > tot) VParam+=4;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 1;
            break;
          default  :
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
            arcMemoCandid[giCurrentScr][giCurrentList][z++] = 0;
        }
      }
      if(VParam > 0) /* Liste ne correspond pas */
      {
        giCodeVoteEnd = 12;
        ptfunct = Vote_End;
        return(0);
      }

#ifdef EL2014		
      affichc();
#else		
      Cand_Display();
#endif		
    }
    else
#ifdef EL2014		
      Compt += getVoteMaxBytes(giCurrentScr,true);
#else		
      Compt += gshCfgScrut[gshScrutin[giCurrentScr].TYPE_SCRUTIN].nmxB;
#endif	

    if (Param == _Blanc_Exp) View_Vote_Empty();

    if((!giKlavier) && (gcCardRW == CARD_READ))
    {
      giCodeVoteEnd = 13;
      ptfunct = Vote_End;
      return(0);
    }
  }

  giCodeVoteEnd = 16;
  ptfunct    = Vote_End;
  return(0);
}

/**************************************************************************
<FUNCTION>
  <NAME> View_Vote_Empty </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  Displays the screen managed to express an empty vote
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void View_Vote_Empty(void)
{
  int iPenX, iPenY, iPenCapture;

  /* Display screen */
  Screen_View_Vote_Empty();

  /* Display "Next" button */
  Screen_Viewing_Next();

  if(gcCardRW == CARD_READ)
  {
    Wait_Loop_Int(10);
  }
  else
  {
    Set_Cursor(CURSOR_XY);
    while((giKlavier) || (gcCardRW == CARD_WRITE))
    {
      Set_Cursor(CURSOR_ON);
      iPenCapture = Pen_Capture(&iPenX, &iPenY);
      if(iPenCapture == 1)
      {
        Set_Cursor(CURSOR_OFF);
        if((iPenY > 452) && (iPenY < 479)) return;
      }
      else
      {
        if(iPenCapture == -1)
        {
          /* Light Pen disconnected */
          giCodeVoteEnd = 5;
          ptfunct = Vote_End;
          return;
        }
      }
    }
  }
}
