/*
#define EL2014
#ifdef EL2014
extern char my_msg[];
extern void my_Trace(void);
#endif
*/

/**************************************************************************
<MODULE NAME>
<FILENAME> mod_list.c </FILENAME>
<HISTORY>
  <MODIFICATION>
    <DATE> </DATE>
    <AUTHOR>  </AUTHOR>
    <DESCRIPTION>
    The list (party) administrator
    Initialization, display, selection...
    </DESCRIPTION>
  </MODIFICATION>
</HISTORY>
</MODULE NAME>
**************************************************************************/

/**************************************************************************
INCLUDE FILES
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include "defs.h"
#include "list.h"
#include "scrutin.h"
#include "..\CrdReade.r\LibCrdRd.h"
#include "..\LibAlrBx\LIBALRBX.H"
#ifdef EL2014
#include <io.h>
#include "variable.h"
#endif
/**************************************************************************
EXTERNAL VARIABLES
**************************************************************************/
extern int       gaiNbListScrut[NB_MAX_SCRUT];
extern int       giCodeVoteEnd;
extern int       giCurrentList;
extern int       giCurrentScr;
extern int       giKlavier;
extern s_Scrutin gshScrutin[10];
extern char      gszArrond[4];
extern unsigned char gucMavType;
extern char    **pszMsg;
#ifdef EL2014
extern char   gcLg;
extern char   RLg;
extern char  *pszMsg1[N_SCR_MESSAGE];
extern char  *pszMsg2[N_SCR_MESSAGE];
#endif


/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/
extern int   Cand_Display(void);
extern void  Cand_Select(int, int, int);
extern int   Msg_Election(void);
extern int   Pen_Capture(int *, int *);
extern void  bararrondi( int , int , int , int );
extern int  (*ptfunct)(void);
extern int   Scrut_Loop(void);
extern void  Set_Cursor(int);
extern int   Vote_Empty(void);
extern int   Vote_End(void);
#ifdef EL2014
/***************************************************************************/
/******            INTEGRATION AFFICHAGE LISTES 2010                  ******/
/***************************************************************************/
int les_listes(void);
testposl(int cx, int cy, int k);
int selectc(void);
int Select_Col(void);
int Select_RLg(void);

extern int affichc();
extern bmp(char *,int ,int ,int );
#endif /* EL2014 */

/**************************************************************************
GLOBAL VARIABLES
**************************************************************************/
char    aszTabEff[NB_MAX_SCRUT][NB_MAX_LISTS];   /* effective */
char    aszTabSuppl[NB_MAX_SCRUT][NB_MAX_LISTS]; /* substitute */
s_List *gshList;

/**************************************************************************
LOCAL VARIABLES
**************************************************************************/
static int gaiPosList[NB_MAX_LISTS][4]; /* coordinates */
static int nbre_Liste_Int;
static int giNbrLignes; /* rows of lists */
static int iLineNbCar = 35;
static int iListNbCar = 25;
static int iStartEffPos = 29;
static int iStartSupplPos = 32;

#define QUATRE_EGALE_3_PLUS_1 1 /* sinon 4 = 2+2 */
extern void Wait_Loop(int);
/**************************************************************************
<FUNCTION>
  <NAME> List_Fill </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Reads machvotl.tbl and fills list's structure
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void List_Fill(FILE *_fpl, int _nbi, int _iScr)
{
  int  iS;
  char bufl[50];
  int j;
  char buffer[10];
  int iEffectif, iSuppleant;
  s_List *shAdrL=NULL;
/* << 9.01b */
  /*static long depl = 0.;*/
  static long depl = 0;
/* 9.01b >> */

  shAdrL = gshList = (s_List *) malloc(sizeof(s_List)* _nbi);
  gshScrutin[_iScr].ptlist = gshList;
  for(j=0;j < _nbi;j++)
  {
    /* Reads line */
    fgets(bufl,iLineNbCar+1,_fpl);
    /* Starting position (machvotc.tbl) */
    ltoa(depl,buffer,10);
    strncpy(gshList->adresse_cand,buffer,6);
    gshList->adresse_cand[6] = '\0';
    /* Code */
    gshList->code_l = bufl[0];
    /* electoral college */
    gshList->college = bufl[1];
    /* List Identification (Num) */
    strncpy(gshList->numlist_l,bufl + 2,2);
    if( gshList->numlist_l[0] == '0') gshList->numlist_l[0] = ' ';
    gshList->numlist_l[2] = '\0';
    /* List Label */
    strncpy(gshList->nom_l,bufl + 4,iListNbCar);
    gshList->nom_l[iListNbCar] = '\0';
    for (iS = iListNbCar-1;
        ((gshList->nom_l[iS] == ' ') && (iS >= 0)); iS--) gshList->nom_l[iS] = '\0';
    /* effective */
    strncpy(gshList->nbreff_l,bufl + iStartEffPos,3);
    gshList->nbreff_l[3] = '\0';
    iEffectif = atoi(gshList->nbreff_l);
    aszTabEff[_iScr][j] = iEffectif;
    /* substitute */
    strncpy(gshList->nbrsup_l,bufl + iStartSupplPos,2);
    gshList->nbrsup_l[2] = '\0';
    iSuppleant = atoi(gshList->nbrsup_l);
    aszTabSuppl[_iScr][j] = iSuppleant;

    /* The last one ? */
    if(j < _nbi-1)
    {
      depl += (iEffectif + iSuppleant) * 81;
      gshList++;
    }
  }
  gshList = shAdrL;
}

#ifndef EL2014
/**************************************************************************
<FUNCTION>
  <NAME> List_Init_Display </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
    </DESCRIPTION>
</FUNCTION>
**************************************************************************/
static void List_Init_Display(int *_iNCol, int *_iLg, int *_iRecl,
                              int *_iEspl, int *_iEsph, int *_iH)
{
  int k;
  int diff;
  int ncolonne,lon,recl,espl,esph,h;

  nbre_Liste_Int = 0;
  k = 0;

  while(k <(gaiNbListScrut[giCurrentScr] - 1))
  {
    gshList++;
    k++;
  }
  nbre_Liste_Int = k+1;

  giNbrLignes =  nbre_Liste_Int / 3;
  giNbrLignes +=((nbre_Liste_Int % 3) > 0) ? 1 : 0;
  ncolonne = 3;

  lon = 208; /*210 trop long pour copie d'écran */
  espl = 3;
  esph = 3;
  settextstyle( SMALL_FONT, HORIZ_DIR, 6);

  if(giNbrLignes <= 5)
  {
    h = 60;
    recl = 60;
  }

  if(giNbrLignes > 5 && giNbrLignes <= 6)
  {
    h = 60;
    recl = 56;
  }

  if(giNbrLignes > 6 && giNbrLignes <= 7)
  {
    h = 55;
    recl = 50;
  }

  if(giNbrLignes > 7 && giNbrLignes <= 8)
  {
    h = 55;
    recl = 42;
  }

  if(giNbrLignes > 8 && giNbrLignes <= 9)
  {
    h = 55;
    recl = 36;
  }

  if(giNbrLignes > 9 && giNbrLignes <= 10)
  {
    h = 55;
    recl = 32;
  }

  if(giNbrLignes > 10 && giNbrLignes <= 11)
  {
    h = 55;
    recl = 30;
  }

  if(giNbrLignes > 11 && giNbrLignes <= 12)
  {
    h = 55;
    recl = 25;
  }

  if(giNbrLignes > 12 && giNbrLignes <= 13)
  {
    h = 55;
    recl = 25;
  }

  if(giNbrLignes > 13 && giNbrLignes <= 14)
  {
    --esph;
    h = 55;
    recl = 25;
  }

  /* Output */
  (*_iNCol) = ncolonne;
  (*_iLg)   = lon;
  (*_iRecl) = recl;
  (*_iEspl) = espl;
  (*_iEsph) = esph;
  (*_iH)    = h;
}

/**************************************************************************
<FUNCTION>
  <NAME> List_Check_Selection </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
    </DESCRIPTION>
</FUNCTION>
**************************************************************************/
static int List_Check_Selection(int _iX, int _iY)
{
  /* Empty (Blanco Stem) ? */
  if((_iX > gaiPosList[nbre_Liste_Int - 1][0] + 2) && (_iX < gaiPosList[nbre_Liste_Int - 1][1] - 2))
    if((_iY > gaiPosList[nbre_Liste_Int - 1][2] + 2) && (_iY < gaiPosList[nbre_Liste_Int - 1][3] - 2))
    {
      ptfunct = Vote_Empty;
      return(1);
    }

  /* Not a empty vote */
  for(giCurrentList = 0;giCurrentList < nbre_Liste_Int - 1;giCurrentList++)
  {
    if((_iX > gaiPosList[giCurrentList][0] + 2) && (_iX < gaiPosList[giCurrentList][1] - 2))
      if ((_iY > gaiPosList[giCurrentList][2] + 2) && (_iY < gaiPosList[giCurrentList][3] - 2))
      {
        /* Display the candidates' screen */
        ptfunct = Cand_Display;
        return(1);
      }
  }

  return(0);
}

/**************************************************************************
<FUNCTION>
  <NAME> List_Display </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>

  </DESCRIPTION>
  <RETURNS>
    <TYPE> int </TYPE>
    <DESCRIPTION> cfr "int (*ptfunct)(void)" </DESCRIPTION>
  </RETURNS>
</FUNCTION>
**************************************************************************/
int List_Display(void)
{
 char caisse[30],buf[30];
 int i,j,k,x,y, iCl, iMsgId;
 int ncolonne,lon,recl,espl,esph,h;
 int iPenX, iPenY, iPenCapture, iHwdDisconnected;
 int iNumX;

 setfillstyle(SOLID_FILL, BLACK);
 bar(0,0,639,479);
 setcolor(WHITE);
 settextstyle( SMALL_FONT, HORIZ_DIR, 6);
 settextjustify( CENTER_TEXT, CENTER_TEXT );
 /* Looking for the message id corresponding to the current ballot */
 iMsgId = Msg_Election();
 outtextxy(320, 10, pszMsg[iMsgId]);
 outtextxy(319, 10, pszMsg[iMsgId]);
 outtextxy(320, 35, pszMsg[54]);
 settextjustify( LEFT_TEXT, CENTER_TEXT );
 setfillstyle(SOLID_FILL,WHITE);
 setcolor(BLACK);

 gshList = gshScrutin[giCurrentScr].ptlist;
 List_Init_Display(&ncolonne,&lon,&recl,&espl,&esph,&h);
 gshList = gshScrutin[giCurrentScr].ptlist;

 settextstyle(SMALL_FONT, HORIZ_DIR, 4);
 for(iCl=0,k=0,x=4;
     (k < gaiNbListScrut[giCurrentScr]) && (iCl < ncolonne);
     x+= espl + lon,iCl++)
   for(j = 0,y = h;
       (j < giNbrLignes) && (k < gaiNbListScrut[giCurrentScr]);
       k++)
   {
     if(QUATRE_EGALE_3_PLUS_1)
     {
       if(iCl == 1) /* If the second column */
       {
         if(j == giNbrLignes-1) /* If "last but one" row */
         {
           if(gaiNbListScrut[giCurrentScr] != (giNbrLignes*3) &&
              gaiNbListScrut[giCurrentScr] != (giNbrLignes*3)-1)
           {
             break; /* if 9 lists + empty -> 10 items
                       col1 = 4 items, col2 = 3 items, col3 = 3 items
                       X                X                 X
                       X                X                 X
                       X                X                 X
                       X*/
           }
         }
       }
     } /* QUATRE_EGALE_3_PLUS_1 */
     /* Get coordinates */
     gaiPosList[k][0] = x;
     gaiPosList[k][1] = x + lon;
     gaiPosList[k][2] = y;
     gaiPosList[k][3] = y + recl;
     /* Draw bar */
     bar(x,y,x + lon,y + recl);

     /* Displays num and label */
     if(k < (gaiNbListScrut[giCurrentScr] - 1))
     {
       buf[0]=gshScrutin[giCurrentScr].nom_s;
       buf[1]= 0x00;
       strcat(buf,gshList->numlist_l);
       buf[3]=gshList->college;
       buf[4]=0x00;
       if(buf[1] == ' ') buf[1] = '0';
       settextjustify( CENTER_TEXT, CENTER_TEXT );
       iNumX = x+11;
       outtextxy( iNumX, y + recl/2, gshList->numlist_l);
       outtextxy( iNumX+1, y + recl/2, gshList->numlist_l);
       settextjustify( LEFT_TEXT, CENTER_TEXT );
       outtextxy( x + 28, y + recl/2, gshList->nom_l);
       outtextxy( x + 29, y + recl/2, gshList->nom_l);
     }
     else
     {
/* << 9.01c */
       settextstyle(SMALL_FONT, HORIZ_DIR, 5);
/* 9.01c >> */
       outtextxy(x + 28, y + recl/2, pszMsg[46]);
       outtextxy(x + 29, y + recl/2, pszMsg[46]);
     }
     j++;
     y+=esph + recl;
     gshList++;
   }

  Set_Cursor(CURSOR_XY);

  while(giKlavier) /* while ! interrupted */
  {
    Set_Cursor(CURSOR_ON);
    iHwdDisconnected = iPenCapture = Pen_Capture(&iPenX, &iPenY);
    if(iPenCapture == 1)
    {
      Set_Cursor(CURSOR_OFF);
      if(List_Check_Selection(iPenX, iPenY)) return(0);
    }
    PEN_OR_ALARMBOX_DISCONNECTED
  }
  Set_Cursor(CURSOR_OFF);

  giCodeVoteEnd = 13;
  ptfunct = Vote_End;
  return(0);
}

#else /* EL2014 */

/***************************************************************************/
/******            INTEGRATION AFFICHAGE LISTES 2010                  ******/
/***************************************************************************/

/***************************************************************************/
/******                       blanc.c                                 ******/
/***************************************************************************/
/*
* La fonction 'les_listes' affiche les listes suivant le modèle de 2010.
* L'algortihme a été mis à jour afin d'être intégré dans la version 2012.
*    - Cand_Fill() est utilisé à la place de Garnir_Cand()
*    - Cand_Circle()
*    - Cand_Update()
*    - Cand_Check_Selection()
* Certaines variables globales doivent être initialisées/utilisées
*/

int les_listes()
{
        int i,j,k,x,y,z;
        int code_scr;

        int Cl, h;
        int ncolonne;
        
	char caisse[30],buf[30];
#ifdef EL2014
	int iPenX, iPenY, iPenCapture, iHwdDisconnected;
#endif

/*
	dejasel = 0;
	typesel = 0;
*/

	gshList = gshScrutin[giCurrentScr].ptlist;
	setfillstyle(SOLID_FILL,BLACK);
	bar( 0,0,639,479);
	setcolor(WHITE);
	settextstyle( SMALL_FONT, HORIZ_DIR, 6);
	settextjustify( CENTER_TEXT, CENTER_TEXT );
	code_scr = Msg_Election();
	if(code_scr == 9)
	   {
		if(strcmp(gszArrond,"005") == 0 ) code_scr+=1;
		if(strcmp(gszArrond,"002") == 0 ) code_scr+=2;
	   }  
	outtextxy(320, 10, pszMsg[code_scr]);
	outtextxy(319, 10, pszMsg[code_scr]);

	/********************** Y A PLUSIEURS COLLEGES ?*****************************/
	if (swCLG)
	{
		gshList = gshScrutin[giCurrentScr].ptlist;
		swCLG = 2; /* Force 1 college par page, attendant Choix MI */
	}
	if(swCLG == 2)
	{
		if(ccol == 0)
		{
			if(Select_Col()) /* SELECTION DU COLLEGE */
			{
                                giCodeVoteEnd = 13;
                                ptfunct = Vote_End;
				return(0);
			}
			ccol = 1;
		}
		setfillstyle(SOLID_FILL,BLACK);
		bar( 0,0,639,479);
		strcpy(Entete_Col,pszMsg[code_scr]);
		strcat(Entete_Col," - ");
		if( Choix_College == 'F')
		{
			strcat(Entete_Col,pszMsg[69]);
			strcat(Entete_Col," ");
			strcat(Entete_Col,pszMsg[70]);
		}
		else
		{
			strcat(Entete_Col,pszMsg[71]);
			strcat(Entete_Col," ");
			strcat(Entete_Col,pszMsg[72]);
		}
		setcolor(WHITE);
		settextstyle( SMALL_FONT, HORIZ_DIR, 6);
		settextjustify( CENTER_TEXT, CENTER_TEXT );
		outtextxy(320, 10, Entete_Col);
		outtextxy(319, 10, Entete_Col);
	}
	/********************** Y A PLUSIEURS RLG ?**********************************/
	if (swRLg)
	{
		gshList = gshScrutin[giCurrentScr].ptlist;
		swRLg = 2;
	}

	if (swRLg == 2)
	{
		/* SELECTION DU REGIME LINGUISTIQUE                                      */
		if(crl == 0)
		{
		if(Select_RLg())
		{
                        giCodeVoteEnd = 13;
                        ptfunct = Vote_End;
			return(0);
		}
#ifdef EL2014                        
        //071113 swVLR = (Choix_RLg == 'N');
#endif                        
		crl = 1;
		}
		setfillstyle(SOLID_FILL,BLACK);
		bar( 0,0,639,479);
		strcpy(Entete_RLg,pszMsg[code_scr]);

		strcat(Entete_RLg,"  ");
		if( Choix_RLg == 'F')
			strcat(Entete_RLg,pszMsg[84]);
		else
			strcat(Entete_RLg,pszMsg[85]);

		setcolor(WHITE);
		settextstyle( SMALL_FONT, HORIZ_DIR, 5);
		settextjustify( CENTER_TEXT, CENTER_TEXT );
		outtextxy(320, 10, Entete_RLg);
		outtextxy(319, 10, Entete_RLg);
		settextstyle( SMALL_FONT, HORIZ_DIR, 6);
		settextjustify( CENTER_TEXT, CENTER_TEXT );
	}

	/************************ FIN PLUSIEURS RLG ?********************************/

	if (!swRLg || (swRLg == 2))
		outtextxy(320, 35, pszMsg[54]);
	else
		outtextxy(320, 35, pszMsg[54]);

	settextjustify( LEFT_TEXT, CENTER_TEXT );
	setfillstyle(SOLID_FILL,WHITE);
	setcolor(BLACK);

	gshList = gshScrutin[giCurrentScr].ptlist;
#ifdef EL2014
        /* insertion directe du fichier 'LPARAM1.C'
           pour éviter l'utilisation de la variable 'k' en global
         */

        #include "LPARAM1.C"

#else /* EL2014 */       
	param();
#endif /* EL2014 */
	gshList = gshScrutin[giCurrentScr].ptlist;

	poslBL2[0][0] = 0;
	poslBL2[0][1] = 0;
	poslBL2[0][2] = 0;
	poslBL2[0][3] = 0;

	if (swRLg == 2)
	{
	  for(Cl=0,k=0,x=espw;(k < gaiNbListScrut[giCurrentScr]) && (Cl < ncolonne);x+=espw + lon,Cl++)
		for(j = 0,y = h;(j < giNbrLignes) && (k < gaiNbListScrut[giCurrentScr]);k++)
		{
			if((gshList->college == Choix_RLg) || (k == (gaiNbListScrut[giCurrentScr] - 1)))
			{
				gaiPosList[k][0] = x;
				gaiPosList[k][1] = x + lon;
				gaiPosList[k][2] = y;
				gaiPosList[k][3] = y + recl;
			}
			else
			{
				gaiPosList[k][0] = 0;
				gaiPosList[k][1] = 0;
				gaiPosList[k][2] = 0;
				gaiPosList[k][3] = 0;
				gshList++;
				continue;
			}

			bar(x,y,x + lon,y + recl);
			if(k < (gaiNbListScrut[giCurrentScr] - 1))
			{
				buf[0]=gshScrutin[giCurrentScr].nom_s;
				buf[1]= 0x00;
				strcat(buf,gshList->numlist_l);
				if(strcmp(gszArrond,"005") == 0 )
					buf[3]='N';
				else
					buf[3]=gshList->college;
				buf[4]=0x00;
				strcat(buf,".bmp");
				if(buf[1] == ' ')
					buf[1] = '0';
				settextjustify( CENTER_TEXT, CENTER_TEXT );
				outtextxy( x + 13, y + recl/2, gshList->numlist_l);
				outtextxy( x + 14, y + recl/2, gshList->numlist_l);
				if(access(buf,0) == 0)
					bmp(buf, x + 29,y +((recl- 26)/2),0);
				else
				{
				 settextjustify( LEFT_TEXT, CENTER_TEXT );
				 outtextxy( x + 28, y + recl/2, gshList->nom_l);
				 outtextxy( x + 29, y + recl/2, gshList->nom_l);
				}
			}
			else
			{
#ifdef EL2014
				settextjustify( LEFT_TEXT, CENTER_TEXT );
				outtextxy(x + 28, y + recl/2, pszMsg[46]);
				outtextxy(x + 29, y + recl/2, pszMsg[46]);
#else
				outtextxy(x + 24, y + recl/2, pszMsg[46]);
				outtextxy(x + 25, y + recl/2, pszMsg[46]);
#endif  
			}
			j++;
			y+=(espl/2) + recl;
			gshList++;
		}
	}
	else
	{
	  for(Cl=0,k=0,x=espw;(k < gaiNbListScrut[giCurrentScr]) && (Cl < ncolonne);x+= espw + lon,Cl++)
		for(j = 0,y = h;(j < giNbrLignes) && (k < gaiNbListScrut[giCurrentScr]);k++)
		{
			if((gshList->college == Choix_College) || (k == (gaiNbListScrut[giCurrentScr] - 1)))
			{
				gaiPosList[k][0] = x;
				gaiPosList[k][1] = x + lon;
				gaiPosList[k][2] = y;
				gaiPosList[k][3] = y + recl;
			}
			else
			{
				gaiPosList[k][0] = 0;
				gaiPosList[k][1] = 0;
				gaiPosList[k][2] = 0;
				gaiPosList[k][3] = 0;
				gshList++;
				continue;
			}

			bar(x,y,x + lon,y + recl);
			if(k < (gaiNbListScrut[giCurrentScr] - 1))
			{
				buf[0]=gshScrutin[giCurrentScr].nom_s;
				buf[1]= 0x00;
				strcat(buf,gshList->numlist_l);
				if(strcmp(gszArrond,"005") == 0 )
					buf[3]='N';
				else
					buf[3]=gshList->college;
				buf[4]=0x00;
				strcat(buf,".bmp");
				if(buf[1] == ' ')
					buf[1] = '0';
				settextjustify( CENTER_TEXT, CENTER_TEXT );
				outtextxy( x + 13, y + recl/2, gshList->numlist_l);
				outtextxy( x + 14, y + recl/2, gshList->numlist_l);
				if(access(buf,0) == 0)
					bmp(buf, x + 29,y +((recl- 26)/2),0);
				else
				{
				 settextjustify( LEFT_TEXT, CENTER_TEXT );
				 outtextxy( x + 28, y + recl/2, gshList->nom_l);
				 outtextxy( x + 29, y + recl/2, gshList->nom_l);
				}
			}
			else
			{
			  settextjustify( LEFT_TEXT, CENTER_TEXT );
			  outtextxy(x + 28, y + recl/2, pszMsg[46]);
			  outtextxy(x + 29, y + recl/2, pszMsg[46]);
			}
			j++;
			y+=(espl/2) + recl;
			gshList++;
		}
	}

	/******************* BAS , RETOUR VERS AUTRES, DEBUT*************************/
	if (swCLG == 2)
	{
		setfillstyle(SOLID_FILL, WHITE);
		settextstyle( SMALL_FONT, HORIZ_DIR, 5);
		bararrondi(110,448,530,478);
		settextjustify(CENTER_TEXT, CENTER_TEXT);
		outtextxy(320,460, pszMsg[73]);
	}
	if (swRLg == 2)
	{
		setfillstyle(SOLID_FILL, WHITE);
		settextstyle( SMALL_FONT, HORIZ_DIR, 5);
		bararrondi(110,448,530,478);
		settextjustify(CENTER_TEXT, CENTER_TEXT);
		outtextxy(320,460, pszMsg[86]);
	}
	/******************* BAS , RETOUR VERS AUTRES, FIN  *************************/

  while(giKlavier) /* while ! interrupted */
  {
    Set_Cursor(CURSOR_ON);
    iHwdDisconnected = iPenCapture = Pen_Capture(&iPenX, &iPenY);
    if(iPenCapture == 1)
    {
      Set_Cursor(CURSOR_OFF);
      if(testposl(iPenX, iPenY, k)) return(0);
    }
    PEN_OR_ALARMBOX_DISCONNECTED
  }
  Set_Cursor(CURSOR_OFF);

  giCodeVoteEnd = 13;
  ptfunct = Vote_End;
  return(0);    
}


/******************** TEST POSITION CRAYON / LISTES ***********************/
testposl(int cx, int cy, int k)
{
	if((cx > gaiPosList[k - 1][0] + 2) && (cx < gaiPosList[k - 1][1] - 2))
	if((cy > gaiPosList[k - 1][2] + 2) && (cy < gaiPosList[k - 1][3] - 2))
	{
		ptfunct = selectc;
		return(1);
	}
	
	if(((swRLg == 1) || (swCLG == 1)) &&
	   ((cx > poslBL2[0][0] + 2) && (cx < poslBL2[0][1] - 2) &&
	   (cy > poslBL2[0][2] + 2) && (cy < poslBL2[0][3] - 2))   )
	{
		ptfunct = selectc;
		return(1);
	}


	if((swCLG == 2) && (cy > 438))
	{
		if((cx > 110) && (cx < 530))
		{
			ccol = 0;
			CHOIX = 0;
			ptfunct = les_listes;
			return(1);
		}
	}

	if((swRLg == 2) && (cy > 438))
	{
		if ((cx > 110) && (cx < 530))
		{
			crl = 0;
			Choix_RLg = ' ';
			ptfunct = les_listes;
			return(1);
		}
	}


	for(giCurrentList = 0;giCurrentList < k - 1;giCurrentList++)
	{
		if((cx > gaiPosList[giCurrentList][0] + 2) && (cx < gaiPosList[giCurrentList][1] - 2))
		if ((cy > gaiPosList[giCurrentList][2] + 2) && (cy < gaiPosList[giCurrentList][3] - 2))
		{
			ptfunct = affichc;
			return(1);
		}
	}
return(0);
}

/***************************************************************************/
/******          MENU DE CONFIRMATION DU CHOIX DU BLANC              ******/
/***************************************************************************/
int selectc()
{
    int iPenX, iPenY, iPenCapture, iHwdDisconnected;
    
	setfillstyle(SOLID_FILL, BLACK);
	bar(0, 30, 639, 479);
	setfillstyle(SOLID_FILL, WHITE);
	bar(20,80,620,300);
	setcolor (BLACK);
	rectangle(38,198,302,292);
	rectangle(39,199,301,291);
	rectangle(338,198,602,292);
	rectangle(339,199,601,291);
	settextstyle( SMALL_FONT, HORIZ_DIR, 6);
	settextjustify ( CENTER_TEXT , TOP_TEXT ) ;
	if(gcLg == 'F')
	  {
			outtextxy(170,220, pszMsg[59]);
			outtextxy(170,260, pszMsg[58]);

			outtextxy(470,220, pszMsg[57]);
			if(strstr(pszMsg[57],"confirmer"))
				    outtextxy(552,220,"confirmer");
			outtextxy(470,260, pszMsg[58]);
	  }
	else if(gcLg == 'N')
	  {
			outtextxy(170,220, pszMsg[57]);
			outtextxy(170,260, pszMsg[59]);

			outtextxy(470,220, pszMsg[57]);
			outtextxy(470,260, pszMsg[58]);
			if(strstr(pszMsg[58],"bevestigen"))
				    outtextxy(509,260, "bevestigen");

	  }
	else if(gcLg == 'A')
	  {
			outtextxy(170,220, pszMsg[59]);
			outtextxy(170,260, pszMsg[58]);

			outtextxy(470,220, pszMsg[57]);
			outtextxy(470,260, pszMsg[58]);
			if(strstr(pszMsg[57],"Best„tigen"))
				    outtextxy(491,220, "Best„tigen");

	  }

	settextstyle( SMALL_FONT, HORIZ_DIR, 7);
	outtextxy( 320, 110, pszMsg[55]);
	outtextxy( 320, 140, pszMsg[56]);

      /*********** TEST DE CONFIRMATION **********/
      while(1)
      {
        Set_Cursor(CURSOR_ON);
        iHwdDisconnected = iPenCapture = Pen_Capture(&iPenX, &iPenY);
        if(iPenCapture == 1)
        {
          Set_Cursor(CURSOR_OFF);
          if((iPenY > 200) && (iPenY < 290))
          {
            if((iPenX > 340) && (iPenX <  600))
            {
                if((gshScrutin[giCurrentScr].nom_s == '4') && (Choix_RLg == 'F'))
                swVLR = 0;
                giCurrentScr++;

                ptfunct = Scrut_Loop;
                return(0);
            }
            else
            {
              if((iPenX >  40) && (iPenX < 300))
              {
                ptfunct = les_listes;
                return(0);
              }
            }
          }
        }
        PEN_OR_ALARMBOX_DISCONNECTED
      }

        giCodeVoteEnd = 13;
        ptfunct = Vote_End;
        return(0);
}

/***************************************************************************/
/******          MENU DE CONFIRMATION DU CHOIX DU College            ******/
/***************************************************************************/
int Select_Col()
{
    int iPenX, iPenY, iPenCapture, iHwdDisconnected;
    
	setfillstyle(SOLID_FILL, BLACK);
	bar(0, 30, 639, 479);
	setfillstyle(SOLID_FILL, WHITE);
	bar(100,80,540,300);
	setcolor (BLACK);

	rectangle(108,198,282,292);
	rectangle(109,199,281,291);
	rectangle(358,198,532,292);
	rectangle(359,199,531,291);

	settextstyle( SMALL_FONT, HORIZ_DIR, 6);
	settextjustify ( CENTER_TEXT , TOP_TEXT ) ;
	if(gcLg == 'F')
	{
		outtextxy(195,220, pszMsg[69]);
		outtextxy(196,220, pszMsg[69]);
	
		outtextxy(195,240, pszMsg[70]);
		outtextxy(196,240, pszMsg[70]);
	
		outtextxy(446,220, pszMsg[71]);
		outtextxy(447,220, pszMsg[71]);
	
		outtextxy(446,240, pszMsg[72]);
		outtextxy(447,240, pszMsg[72]);
	}
	else
	{
		outtextxy(195,220, pszMsg[71]);
		outtextxy(196,220, pszMsg[71]);
	
		outtextxy(195,240, pszMsg[72]);
		outtextxy(196,240, pszMsg[72]);
	
		outtextxy(446,220, pszMsg[69]);
		outtextxy(447,220, pszMsg[69]);
	
		outtextxy(446,240, pszMsg[70]);
		outtextxy(447,240, pszMsg[70]);
	}

	settextstyle( SMALL_FONT, HORIZ_DIR, 7);
	outtextxy( 320, 110, pszMsg[68]);
	outtextxy( 320, 111, pszMsg[68]);
	settextstyle( SMALL_FONT, HORIZ_DIR, 6);

      /*********** TEST DE CONFIRMATION **********/
      while(1)
      {
        Set_Cursor(CURSOR_ON);
        iHwdDisconnected = iPenCapture = Pen_Capture(&iPenX, &iPenY);
        if(iPenCapture == 1)
        {
          Set_Cursor(CURSOR_OFF);
          if((iPenY > 200) && (iPenY < 290))
          {
            if((iPenX > 360) && (iPenX <  530))
            {
                if(gcLg == 'F')
                        Choix_College = 'N';
                else
                        Choix_College = 'F';
                return(0);
            }
            else
            {
              if((iPenX >  110) && (iPenX < 280))
              {
                if(gcLg == 'F')
                        Choix_College = 'F';
                else
                        Choix_College = 'N';
                return(0);
              }
            }
          }
        }
        PEN_OR_ALARMBOX_DISCONNECTED
      }

  return(1);
}

/*****************************************************************************/
/******       MENU DE CONFIRMATION DU CHOIX DU REGIME LINGUISTIQUE      ******/
/*****************************************************************************/

int Select_RLg()
{
    int iPenX, iPenY, iPenCapture, iHwdDisconnected;
    
	setfillstyle(SOLID_FILL, BLACK);
	bar(0, 30, 639, 479);
	setfillstyle(SOLID_FILL, WHITE);
	bar( 50,80,590,300);

	setcolor (BLACK);
	rectangle(108,198,282,292);
	rectangle(109,199,281,291);

	rectangle(358,198,532,292);
	rectangle(359,199,531,291);
	
	settextjustify ( CENTER_TEXT , TOP_TEXT ) ;
	
#ifdef EL2014
	if(RLg == 'F')
	{
	  settextstyle( SMALL_FONT, HORIZ_DIR, 5);
	  outtextxy(195,220, pszMsg[80]);
	  outtextxy(196,220, pszMsg[80]);

	  settextstyle( SMALL_FONT, HORIZ_DIR, 6);
	  outtextxy(195,240, pszMsg[81]);
	  outtextxy(196,240, pszMsg[81]);

	  settextstyle( SMALL_FONT, HORIZ_DIR, 5);
	  outtextxy(446,220, pszMsg[82]);
	  outtextxy(447,220, pszMsg[82]);

	  settextstyle( SMALL_FONT, HORIZ_DIR, 6);
	  outtextxy(446,240, pszMsg[83]);
	  outtextxy(447,240, pszMsg[83]);
	}
	else
	{
	  settextstyle( SMALL_FONT, HORIZ_DIR, 5);
	  outtextxy(195,220, pszMsg[82]);
	  outtextxy(196,220, pszMsg[82]);

	  settextstyle( SMALL_FONT, HORIZ_DIR, 6);
	  outtextxy(195,240, pszMsg[83]);
	  outtextxy(196,240, pszMsg[83]);
	  
	  settextstyle( SMALL_FONT, HORIZ_DIR, 5);
	  outtextxy(446,220, pszMsg[80]);
	  outtextxy(447,220, pszMsg[80]);

	  settextstyle( SMALL_FONT, HORIZ_DIR, 6);
	  outtextxy(446,240, pszMsg[81]);
	  outtextxy(447,240, pszMsg[81]);
	}
#else /* EL2014	 */
	settextstyle( SMALL_FONT, HORIZ_DIR, 5);
	outtextxy(195,220, pszMsg[80]);
	outtextxy(196,220, pszMsg[80]);

	settextstyle( SMALL_FONT, HORIZ_DIR, 6);
	outtextxy(195,240, pszMsg[81]);
	outtextxy(196,240, pszMsg[81]);

	settextstyle( SMALL_FONT, HORIZ_DIR, 5);
	outtextxy(446,220, pszMsg[82]);
	outtextxy(447,220, pszMsg[82]);

	settextstyle( SMALL_FONT, HORIZ_DIR, 6);
	outtextxy(446,240, pszMsg[83]);
	outtextxy(447,240, pszMsg[83]);
#endif /* EL2014	 */

	settextstyle( SMALL_FONT, HORIZ_DIR, 7);
	outtextxy( 320, 110, pszMsg[79]);
	outtextxy( 320, 111, pszMsg[79]);

	settextstyle( SMALL_FONT, HORIZ_DIR, 6);

      /*********** TEST DE CONFIRMATION **********/
      while(1)
      {
        Set_Cursor(CURSOR_ON);
        iHwdDisconnected = iPenCapture = Pen_Capture(&iPenX, &iPenY);
        if(iPenCapture == 1)
        {
          Set_Cursor(CURSOR_OFF);
          if((iPenY > 200) && (iPenY < 290))
          {
            if((iPenX > 360) && (iPenX <  580))
            {
#ifdef EL2014
				if(RLg == 'F')
                    Choix_RLg = 'N';
				else
                    Choix_RLg = 'F';
				return(0);
#else
                Choix_RLg = 'N'; return(0);
#endif
            }
            else
            {
              if((iPenX >  110) && (iPenX < 280))
              {
#ifdef EL2014
                if(RLg == 'F')
                    Choix_RLg = 'F';
                else
                    Choix_RLg = 'N';
                return(0);
#else
                Choix_RLg = 'F';
                return(0);
#endif			  
              }
            }
          }
        }
        PEN_OR_ALARMBOX_DISCONNECTED
      }

	return(1);
}

#endif /* EL2014 */
