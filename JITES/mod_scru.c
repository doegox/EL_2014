/*
#define EL2014
#ifdef EL2014
extern char my_msg[];
extern void my_Trace(void);
#endif
*/

/**************************************************************************
<MODULE NAME>
<FILENAME> mod_scru.c (scrutin) </FILENAME>
<HISTORY>
  <MODIFICATION>
    <DATE> </DATE>
    <AUTHOR> </AUTHOR>
    <DESCRIPTION>
    Jeton (token)
    machvots.tbl
    ballots' results
    ballots' sequence
    ballots' loop (access control (National/Stranger))
    </DESCRIPTION>
  </MODIFICATION>
</HISTORY>
<FUNCTIONS>

</FUNCTIONS>
</MODULE NAME>
**************************************************************************/


/**************************************************************************
INCLUDE FILES
**************************************************************************/
//#include <bios.h>
//#include <io.h>
#include <fcntl.h>
//#include <alloc.h>
//#include <dos.h>
//#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graphics.h"

#include "defs.h"
#include "cfgscrut.h"
#include "scrutin.h"
#ifdef EL2014
#include "variable.h"
#include "util_mav.h"
#endif

/**************************************************************************
EXTERNAL VARIABLES
**************************************************************************/
extern char    arcMemoCandid[NB_MAX_SCRUT][NB_MAX_LISTS][NB_MAX_CANDID];
extern char    arcMemoList[NB_MAX_SCRUT][NB_MAX_LISTS];
extern char    arcMemoScrutin[NB_MAX_SCRUT];
extern unsigned char guszCard[75];
extern int     giCurrentScr;
extern int     giSWCard;
extern char   *pszMsg1[N_SCR_MESSAGE];
extern char  **pszMsg;
#ifdef EL2014
extern char   gcLg;
extern s_List *gshList;
#endif

/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/
#ifdef EL2014
extern int les_listes();
#endif
extern void  Calcul_Crc(char *, int);
extern void  Card_Crc_Write(char *);
extern void  Card_Written(char *);
extern int   List_Display(void);
extern void  List_Fill(FILE *, int, int);
extern void  Msg_Error_Exit(char *);
extern int   Para_Read(void);
extern int (*ptfunct)(void);
extern void  Vote_Encode(char *, int, int, int *);
extern int   Vote_End(void);

/**************************************************************************
GLOBAL VARIABLES
**************************************************************************/
int  gaiNbListScrut[NB_MAX_SCRUT]; /* Amount of lists by ballot */
int  giNbrScrutin;
int  giCardNBytes;
int  giVoteMaxBytes;
char gszJeton[16];
char gszJetonMini[6];
char gszArrond[4];
s_CFGSCRUT gshCfgScrut[NMX_SCRUTIN_T+1] = { {0, 0, 0},
        {VM_EUROPE,   NMXV_EUROPE,   NMXB_EUROPE},
        {VM_CHAMBRE,  NMXV_CHAMBRE,  NMXB_CHAMBRE},
        {VM_SENAT,    NMXV_SENAT,    NMXB_SENAT},
        {VM_REGION,   NMXV_REGION,   NMXB_REGION},
        {VM_PROVINCE, NMXV_PROVINCE, NMXB_PROVINCE},
        {VM_COMMUNE,  NMXV_COMMUNE,  NMXB_COMMUNE},
        {VM_CONSEIL_FLAMANT,NMXV_CONSEIL_FLAMANT,NMXB_CONSEIL_FLAMANT}
#ifdef EL2014 
        ,
		{VM_DISTRICT,        NMXV_DISTRICT,        NMXB_DISTRICT},
		{VM_REGION_GERMANO,  NMXV_REGION_GERMANO,  NMXB_REGION_GERMANO}
#else
#endif                                
        };
s_Scrutin gshScrutin[10];

/**************************************************************************
<FUNCTION>
  <NAME> Build_Jeton </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  Creation of the "jeton" (token) from a given string
  </DESCRIPTION>
  <PARAMETERS>
    <PARAMETER>
      <NAME> _pzIn </NAME>
      <TYPE> char * </TYPE>
      <IO> IN </ IO >
    </PARAMETER>
    <PARAMETER>
      <NAME> _pzOut </NAME>
      <TYPE> char * </TYPE>
      <IO> OUT </ IO >
    </PARAMETER>
  </PARAMETERS>
</FUNCTION>
**************************************************************************/
static void Build_Jeton(char *_pzIn, char *_pzOut)
{
  int i;
  int jj, mm,aa;
  int cant,buv1,buv2,tmp;
  char Cjj[3],Cmm[3],Caa[3];
  char Ccant[4], Cbuv1[4],Cbuv2[4];
  char _Code[] = "0EC3ZN678LAB2DFRH1IJK9M5OPQGSTUVWXY4";

  strncpy(Ccant,_pzIn + 3,3);
  strncpy(Cbuv1,_pzIn + 6,2);
  strncpy(Cbuv2,_pzIn + 8,1);
  strncpy(Cjj,_pzIn   + 9,2);
  strncpy(Cmm,_pzIn   + 11,2);
  strncpy(Caa,_pzIn   + 13,2);

  Ccant[3] = '\0';
  Cbuv1[2] = '\0';
  Cbuv2[1] = '\0';
  Cjj[2]   = '\0';
  Cmm[2]   = '\0';
  Caa[2]   = '\0';

  cant = atoi(Ccant);
  buv1 = atoi(Cbuv1);
  buv2 = atoi(Cbuv2);
  jj   = atoi(Cjj);
  mm   = atoi(Cmm);
  aa   = atoi(Caa);

  for(i = 0; i < 5;i++)
  {
    _pzOut[1] = _Code[buv1 % 35];
    _pzOut[3] = _Code[buv2 % 35];
    _pzOut[2] = _Code[(jj + cant) % 35];
    _pzOut[0] = _Code[mm   % 35];
    _pzOut[4] = _Code[aa   % 35];
  }
  _pzOut[5] = '\0';
}

/**************************************************************************
<FUNCTION>
  <NAME> Scrut_Read </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  fill up scrutin variables (reading machvots.tbl)
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Scrut_Read(void)
{
  char bufs[512],c;
  int i,j;
  FILE *fpl, *fps;

#if (MEMORY_CHECK >= 1)
  if(Debut_Mcbc() != 7) Msg_Error_Exit("Incorrect Config");
#endif

  i = 0;
  giNbrScrutin = 0;

  fps = fopen("DATA/machvots.tbl","r");
/* << 9.01c */
  if(fps == NULL) Msg_Error_Exit("DATA/machvots.tbl FILE ERROR");
/* 9.01c >> */
  fpl = fopen("DATA/machvotl.tbl","r");
/* << 9.01c */
  if(fpl == NULL) Msg_Error_Exit("DATA/machvotl.tbl FILE ERROR");
/* 9.01c >> */

  /* Read the first 16 caracts */
  fread(bufs,16,1,fps);
  strncpy(gszJeton,bufs,15);
  gszJeton[15] = '\0';

  strncpy(gszArrond,bufs,3);
  gszArrond[3] = '\0';
//printf("SCRUTIN: Arrond %s\n", gszArrond);

  /* Jeton */
  Build_Jeton(gszJeton,gszJetonMini);
//printf("SCRUTIN: Jeton %s\n", gszJetonMini);
  fread(bufs,8,1,fps);

  giVoteMaxBytes = 0;
  while(!feof(fps))
  {
//printf("SCRUTIN: %s\n", bufs);
    /* Code */
    gshScrutin[i].code_s = bufs[0];
//printf("SCRUTIN: code_s %c\n", gshScrutin[i].code_s);
    strncpy(gshScrutin[i].scrutin_max_cand,bufs + 1,2);
    /* Max Eff+Substitute +1 */
    gshScrutin[i].scrutin_max_cand[2] = '\0';
//printf("SCRUTIN: max_cand %s\n", gshScrutin[i].scrutin_max_cand);
    /* Max Substitute */
    strncpy(gshScrutin[i].scrutin_max_supp,bufs + 3,2);
    gshScrutin[i].scrutin_max_supp[2] = '\0';
//printf("SCRUTIN: max_supp %s\n", gshScrutin[i].scrutin_max_supp);
    /* Type */
    gshScrutin[i].nom_s = bufs[5];
//printf("SCRUTIN: nom_s %c\n", gshScrutin[i].nom_s);
    gshScrutin[i].TYPE_SCRUTIN = (int)((int)bufs[5] - (int)'0');
//printf("SCRUTIN: TYPE_SCRUTIN %i\n", gshScrutin[i].TYPE_SCRUTIN);
    /* Incr giVoteMaxBytes */
    giVoteMaxBytes += (gshCfgScrut[gshScrutin[i].TYPE_SCRUTIN].nmxB +2);
    /* Nbr Lists +1 */
    strncpy(gshScrutin[i].nbrelist_s,bufs + 6,2);
    gshScrutin[i].nbrelist_s[2] = '\0';
    gaiNbListScrut[i] = atoi(gshScrutin[i].nbrelist_s);
//printf("SCRUTIN: nbre lists %i\n", gaiNbListScrut[i]);

    giNbrScrutin++;
    /* Read the machvotl.tbl */
    List_Fill(fpl,atoi(gshScrutin[i].nbrelist_s),i);
    i++;
    fread(bufs,8,1,fps);
  }
  fclose(fps);
  fclose(fpl);
  
#ifdef EL2014
  /* adjust giVoteMaxBytes */
  giVoteMaxBytes = MAX_CARD_BYTES - (_C_JETON_MAX_BYTE + _C_TEST_MAX_BYTE + _C_CRC_MAX_BYTE + _C_PASSAGE_MAX_BYTE);
  /* getVoteMaxBytes() need to be used after gshScrutin[] load !!! */
  /*
  giVoteMaxBytes = 0;
  for (i=0; i < giNbrScrutin; i++ ) {
    giVoteMaxBytes += getVoteMaxBytes(i,1) +2;
  }
  */
#endif    

  /* giCardNBytes */
  giCardNBytes = giVoteMaxBytes + _C_JETON_MAX_BYTE;
  giCardNBytes += _C_TEST_MAX_BYTE + _C_CRC_MAX_BYTE + _C_PASSAGE_MAX_BYTE;
}

/**************************************************************************
<FUNCTION>
  <NAME> Scrut_Result </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
  <RETURNS>
    <TYPE> int </TYPE>
    <DESCRIPTION>
    cfr "int (*ptfunct)(void)"
    </DESCRIPTION>
  </RETURNS>
</FUNCTION>
**************************************************************************/
int Scrut_Result(void)
{
  int iList, i, y, iDebut;

  iDebut = _C_VOTE_PLACE;

  /* Specify that a vote has been done */
  Card_Written(guszCard);

  /* For all ballots */
  for(i=0; i<giNbrScrutin; i++)
  {
    /* List's id attribution */
    if(arcMemoScrutin[i] == 0) iList = 99; /* Empty vote */
    else
    {
      if(arcMemoScrutin[i] == 2) iList = 98; /* Vote not allowed (Stranger) */
      else
      {
        for(y = 0;y < atoi(gshScrutin[i].nbrelist_s);y++)
        {
          if(arcMemoList[i][y] == 1) iList = y + 1;
        }
      }
    }
    Vote_Encode(guszCard, i, iList, &iDebut);
  }

  Calcul_Crc(guszCard,_C_TEST_PLACE);
  Card_Crc_Write(guszCard);
  ptfunct = Vote_End;
  return(0);
}

/**************************************************************************
<FUNCTION>
  <NAME> Scrut_Loop </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
  <RETURNS>
    <TYPE> int </TYPE>
    <DESCRIPTION> cfr "int (*ptfunct)(void)" </DESCRIPTION>
  </RETURNS>
</FUNCTION>
**************************************************************************/
int Scrut_Loop(void)
{    
  /* One by one */
  while(giCurrentScr < giNbrScrutin)
  {
#ifdef EL2014
        if(/*(swNOCarte == _NOCARTE)
                 ||*/ (guszCard[_C_TEST_PLACE] == 'N')
                 || ((guszCard[_C_TEST_PLACE] == 'E')
                 && ((gshScrutin[giCurrentScr].TYPE_SCRUTIN == 1) || (gshScrutin[giCurrentScr].TYPE_SCRUTIN == 6)))
                 || ((guszCard[_C_TEST_PLACE] == 'S') && ((gshScrutin[giCurrentScr].TYPE_SCRUTIN== 2) ||
                 (gshScrutin[giCurrentScr].TYPE_SCRUTIN == 3))))
        {
                ccol = 0;
                crl = 0;
                gshList = gshScrutin[giCurrentScr].ptlist;

                if((gshScrutin[giCurrentScr].nom_s == '1'))
                        ClOui = 1;
                else
                        ClOui = 0;

                if((strcmp(gszArrond,"002")==0) && (gshScrutin[giCurrentScr].nom_s== '4'))
                        swVLR = 1;

                if((strcmp(gszArrond,"002")==0) && (gshScrutin[giCurrentScr].nom_s== '7')
                         && (swVLR == 0))
                {
                        arcMemoScrutin[giCurrentScr] = 2; /* on passe */
                        giCurrentScr++;
                        return(0);
                }

                if((strcmp(gszArrond,"002")==0) && (gshScrutin[giCurrentScr].nom_s== '4'))
                {
                        swRLg = 1;
                        RLg = gcLg;
                        Choix_RLg = ' ';
                        swCLG = 0;
                }
                else
                        if((strcmp(gszArrond,"002")==0) && ((gshScrutin[giCurrentScr].nom_s == '1')
                                 || (gshScrutin[giCurrentScr].nom_s == '3')))
                {
                        swRLg = 0;
                        swCLG = 1;
                        RLg = ' ';
                        Choix_RLg = ' ';
                }
                else
                {
                        swRLg = 0;
                        RLg = ' ';
                        Choix_RLg = ' ';
                        swCLG = 0;
                }
                CHOIX = 0;
                Choix_College = 'F';

                ptfunct = les_listes;
                return(0);
        }
        else
        {
                arcMemoScrutin[giCurrentScr] = 2; /* on passe */
                giCurrentScr++;
                return(0);
        }
#else  /* EL2014 */
    if((guszCard[_C_TEST_PLACE] == 'N') /* if National or */
        || ((guszCard[_C_TEST_PLACE] == 'S') && /* if Stranger AND */
           (gshScrutin[giCurrentScr].TYPE_SCRUTIN == 6)))   /* COMM */
    {
      /* Access to the lists */
      ptfunct = List_Display;
      return(0);
    }
    else
    {
      arcMemoScrutin[giCurrentScr] = 2; /* pass over (for example Stranger) */
      ++giCurrentScr;
      return(0);
    }
#endif /* EL2014 */ 
  }

  ptfunct = Scrut_Result;
  return(0);
}
