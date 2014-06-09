/*
#define EL2014
#ifdef EL2014
extern char my_msg[];
extern void my_Trace(void);
#endif
*/

/**************************************************************************
<MODULE NAME>
<FILENAME> mod_cand.c </FILENAME>
<HISTORY>
  <MODIFICATION>
    <DATE> </DATE>
    <AUTHOR>  </AUTHOR>
    <DESCRIPTION>
    Candidates Administrator
    </DESCRIPTION>
  </MODIFICATION>
</HISTORY>
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
#include "candid.h"
#include "defs.h"
#include "scrutin.h"
#include "libcrdrd.h"
#include "libalrbx.h"
#ifdef EL2014
#include "variable.h"
#include "cfgscrut.h"
#endif
/**************************************************************************
EXTERNAL VARIABLES
**************************************************************************/
extern char      arcMemoCandid[NB_MAX_SCRUT][NB_MAX_LISTS][NB_MAX_CANDID];
extern char      arcMemoList[NB_MAX_SCRUT][NB_MAX_LISTS];
extern char      arcMemoScrutin[NB_MAX_SCRUT];
extern char      aszTabEff[NB_MAX_SCRUT][NB_MAX_LISTS];
extern char      aszTabSuppl[NB_MAX_SCRUT][NB_MAX_LISTS];
extern char      gcCardRW;
extern int       giCodeVoteEnd;
extern int       giCurrentCandid;
extern int       giCurrentList;
extern int       giCurrentScr;
extern int       giKlavier;
extern s_List   *gshList;
extern s_Scrutin gshScrutin[10];
extern int       giSWCard;
extern char      gszArrond[4];
extern unsigned char gucMavType;
extern char    **pszMsg;

/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/
#ifdef EL2014
extern int les_listes();
#endif
extern int   List_Display(void);
extern int   Msg_Election(void);
extern void  Msg_Error_Exit(char *);
extern int   Pen_Capture(int *, int *);
extern int (*ptfunct)(void);
extern void  Screen_Cand_Actions_Allowed(int);
extern void  Screen_Start_Verif(void);
extern void  Screen_Viewing_Next(void);
extern int   Scrut_Loop(void);
extern void  Set_Cursor(int);
extern int   Vote_End(void);
extern void  zero_select(int, int, int);
extern void  Wait_Loop_Int(int);
#ifdef EL2014
/***************************************************************************/
/******            INTEGRATION AFFICHAGE CANDIDATS 2010               ******/
/***************************************************************************/
void Cal_affiche(int x, int y);
void type_ecran(int *_nb_cdt_par_colonne);
void affiche_num_cdt(int x, int y, int k, int _iNum);
void Garnir_param(void);
int param_Candidat(void);

int bmp(char *BmpName,int X,int Y,int Inverse);
#endif /* EL2014 */

/**************************************************************************
GLOBAL VARIABLES
**************************************************************************/
int gaiPosc[100][4];

/**************************************************************************
LOCAL VARIABLES
**************************************************************************/
static int     gaiCandidCol[5];
static int     giDebutAffichageY;
static int     giCadreCandidH;
static int     giInterCandidL, giInterCandidC;
static int     giNbCandidCol;
static int     giNbCandidSel; /* Number of selected candidates */
static int     giNombreCol;
static int     giSaisieCandidH, giSaisieCandidL,giCadreCandidL;
static int     giXDep;
static int     giXCapture, giYCapture;
static int     giNomY, giPrenomY;
static s_Cand *gshCand=NULL;
#ifdef EL2014
static int     swconf;
static int     swnoir;
static int     swpas;
#endif

static int     giViewCandSelectedMode = 0; /* if 1 -> selected candidates
                                 are directly shown like selected (faster) */

/* << el2012 - changed to 1*/
#ifdef EL2014
#define SECOND_LINE_CENTERED 1
#else
#define SECOND_LINE_CENTERED 0
#endif
/* el2012 >> */

/**************************************************************************
<FUNCTION>
  <NAME> Cand_Fill </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  Reads machvotc.tbl and fills candidate's structure
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
static void Cand_Fill(void)
{
  long deplacement_file_cand;
  char bufc[100],caisse[35];
  char vide[26];
  int j, k, kp;
  char *p;
  FILE *fpc;
  static s_Cand *shCand=NULL;

  memset(vide,' ',26);
  if(shCand != NULL) free(shCand);
  /* Get the list */
  gshList = gshScrutin[giCurrentScr].ptlist;
  for (giCurrentCandid = 0; giCurrentCandid < giCurrentList;giCurrentCandid++)gshList++;
  /* Get the starting position in machvotc.tbl */
  deplacement_file_cand = atol(gshList->adresse_cand);

  /* Number of candidates */
  kp = aszTabEff[giCurrentScr][giCurrentList] + aszTabSuppl[giCurrentScr][giCurrentList];
  /* Open the machvotc.tbl */
  fpc = fopen("DATA/machvotc.tbl","r");
  if(fpc == NULL) Msg_Error_Exit("DATA/machvotc.tbl FILE ERROR");

  /* Go to the right position */
  fseek(fpc,deplacement_file_cand,0);

  /* structures' allocation */
  shCand = gshCand = (s_Cand *) malloc(sizeof(s_Cand) * kp);
  if(shCand == NULL) Msg_Error_Exit("MALLOC RAM ERROR");
  /* Read candidates */
  for(k=0; k < kp; k++)
  {
    /* Read line */
    fgets(bufc,81,fpc);
//printf("CAND_FILL: (%i/%i) %s\n", k, kp, bufc);
    /* code */
    gshCand->code_c = bufc[0];
//printf("CAND_FILL: code_c=%c\n", gshCand->code_c);
    /* NumList */
    strncpy(gshCand->numlist_c,bufc + 1,2);
    gshCand->numlist_c[2] = '\0';
//printf("CAND_FILL: numlist_c=%s\n", gshCand->numlist_c);
    /* Name */
    caisse[25] = '\0';
    strncpy(caisse,vide,25);
    strncpy(caisse,bufc + 3,25);
    j = 24;
    while(caisse[j] == ' ' && j >= 0) caisse[j--] = 0x00;
    strcpy(gshCand->nom_c,caisse);
//printf("CAND_FILL: gshCand->nom_c=%s;\n", gshCand->nom_c);
    /* firstname */
    strncpy(caisse,vide,25);
    strncpy(caisse,bufc + 3 + 25,25);
    j = 24;
    while(caisse[j] == ' ' && j >= 0) caisse[j--] = 0x00;
    strcpy(gshCand->prenom_c,caisse);
//printf("CAND_FILL: gshCand->prenom_c=%s;\n", gshCand->prenom_c);
    /* "second" firstname */
    strncpy(caisse,vide,25);
    strncpy(caisse,bufc + 3 + 50,25);
    j = 24;
    while(caisse[j] == ' ' && j >= 0) caisse[j--] = 0x00;
    strcpy(gshCand->prenom2_c,caisse);
//printf("CAND_FILL: gshCand->prenom2_c=%s;\n", gshCand->prenom2_c);
    /* Effective or Subsitute */
    gshCand->E_S = bufc[78];
//printf("CAND_FILL: E_S=%c\n", gshCand->E_S);

    /* Next Candidate */
    if(k < kp-1) gshCand++;
  }
  gshCand = shCand;
  fclose(fpc);
}

/**************************************************************************
<FUNCTION>
  <NAME> Cand_Init_Display </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  Get the graphicals coordinates, lengths... to manage the screen
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
static void Cand_Init_Display(void)
{
  int nbr_max;

  nbr_max = atoi(gshScrutin[giCurrentScr].scrutin_max_cand) - 1; /* 1: head */

  if (nbr_max <= 16)
  {
    giNombreCol = 1;
    gaiCandidCol[giCurrentScr] = giNbCandidCol =  16;
    giDebutAffichageY =  34;
    giCadreCandidH   =  23;
    giNomY           =   5;
    giPrenomY        =  16;
    giInterCandidL   =   2;
    giCadreCandidL   = 202;
    giInterCandidC   = 216;
    giSaisieCandidH  =  20;
    giSaisieCandidL  =  30;
    giXDep           =  (640 - giCadreCandidL) /2;
    giXCapture       = giCadreCandidL / 2;
    giYCapture       = (giCadreCandidH -8) / 2; /* 8 = circle's diam */
    return;
  }

  if (nbr_max <= 32)
  {
    giNombreCol = 2;
    gaiCandidCol[giCurrentScr] = giNbCandidCol =  16;
    giDebutAffichageY =  34;
    giCadreCandidH   =  23;
    giNomY           =   5;
    giPrenomY        =  16;
    giInterCandidL   =   2;
    giCadreCandidL   = 202;
    giInterCandidC   = 126;

    giSaisieCandidH  =  20;
    giSaisieCandidL  =  30;
    giXDep           =  (640 - 2* giCadreCandidL - giInterCandidC) /2;
    giXCapture       = giCadreCandidL / 2;
    giYCapture       = (giCadreCandidH -8) / 2; /* 8 = circle's diam */
    return;
  }

  if ((nbr_max > 32) && (nbr_max <= 48))
  {
    giNombreCol = 3;
    gaiCandidCol[giCurrentScr] = giNbCandidCol =  16;
    giDebutAffichageY =  34;
    giCadreCandidH   =  23;
    giNomY           =   5;
    giPrenomY        =  16;
    giInterCandidL   =   2;
    giCadreCandidL   = 202;
    giInterCandidC   = 10;
    giXDep           = (640 - 3*giCadreCandidL - 2*giInterCandidC) /2;
    giSaisieCandidH  =  20;
    giSaisieCandidL  =  30;
    giXCapture       = giCadreCandidL / 2;
    giYCapture       = (giCadreCandidH -8) / 2; /* 8 = circle's diam */
    return;
  }

  if ((nbr_max > 48) && (nbr_max <= 51))
  {
    giNombreCol = 3;
    gaiCandidCol[giCurrentScr] = giNbCandidCol =  17;
    giDebutAffichageY =  34;
    giCadreCandidH   =  22;
    giNomY           =   5;
    giPrenomY        =  14;
    giInterCandidL   =   2;
    giCadreCandidL   = 202;
    giInterCandidC   = 10;
    giXDep           = (640 - 3*giCadreCandidL - 2*giInterCandidC) /2;
    giSaisieCandidH  =  20;
    giSaisieCandidL  =  30;
    giXCapture       = giCadreCandidL / 2;
    giYCapture       = (giCadreCandidH -8) / 2; /* 8 = circle's diam */
    return;
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Cand_Update </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  The "user" did something (de/selection) ... update the display
  </DESCRIPTION>
  <PARAMETERS>
    <PARAMETER>
      <NAME> _iCandid </NAME>
      <TYPE> int </TYPE>
      <IO> IN </IO>
      <DESCRIPTION>
      Candidate's id
      </DESCRIPTION>
    </PARAMETER>
  </PARAMETERS>
</FUNCTION>
**************************************************************************/
#pragma argsused
void Cand_Update(int _iCandid)
{
  /* Candidate selected ? */
  if(arcMemoCandid[giCurrentScr][giCurrentList][_iCandid] == 1)
  {
    if((swnoir == 0) && (gcCardRW == CARD_WRITE))
    {
      swnoir = 1;
      if(swconf == 0)
      {
        swconf = 1;
        swpas = 1;
        Screen_Cand_Actions_Allowed(swconf);
      }
    }
  }
  else /* Candidate unselected */
  {
    if((swpas == 0) && (gcCardRW == CARD_WRITE))
    {
      Screen_Cand_Actions_Allowed(swconf);
      swpas = 1;
    }
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Cand_Coord </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  Gives the coordinates of a candidate's (it's frame)
  </DESCRIPTION>
  <PARAMETERS>
    <PARAMETER>
      <NAME> z </NAME>
      <TYPE> int </TYPE>
      <IO> IN </IO>
      <DESCRIPTION>
      Candidate's id
      </DESCRIPTION>
    </PARAMETER>
    <PARAMETER>
      <NAME> x1, x2, y1, y2 </NAME>
      <TYPE> int * </TYPE>
      <IO> OUT </IO>
      <DESCRIPTION>
      Rect's coordinates
      </DESCRIPTION>
    </PARAMETER>
  </PARAMETERS>
</FUNCTION>
**************************************************************************/
static void Cand_Coord(int z, int *x1, int *x2, int *y1, int *y2)
{
  if(z != 0) /* A single candidate */
  {
    (*x1) = (gaiPosc[z][1] - giCadreCandidL) + 1;
    (*x2) = ((*x1) + giCadreCandidL);
#ifndef EL2014	
    (*x2) -= giSaisieCandidL;
#endif
    (*y1) = gaiPosc[z][2];
    (*y2) = (*y1) + giCadreCandidH;
  }
  else /* list's head */
  {
    (*x1) = gaiPosc[z][0];
    (*x2) = gaiPosc[z][1]+1;
    (*y1) = gaiPosc[z][2];
    (*y2) = gaiPosc[z][3]+1;
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Cand_Reverse </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  Reverse the background color (e.g. the de/selection of a candidate)
  </DESCRIPTION>
  <PARAMETERS>
    <PARAMETER>
      <NAME> _iX1,_iX2,_iY1,_iY2 </NAME>
      <TYPE> int </TYPE>
      <IO> IN </ IO >
      <DESCRIPTION>
      The coordinates of the area
      </DESCRIPTION>
    </PARAMETER>
    <PARAMETER>
      <NAME> _iCol1 </NAME>
      <TYPE> int </TYPE>
      <IO> IN </ IO >
      <DESCRIPTION>
      The color to replace
      </DESCRIPTION>
    </PARAMETER>
    <PARAMETER>
      <NAME> _iCol2 </NAME>
      <TYPE> int </TYPE>
      <IO> IN </ IO >
      <DESCRIPTION>
      The new color
      </DESCRIPTION>
    </PARAMETER>
  </PARAMETERS>
</FUNCTION>
**************************************************************************/
static void Cand_Reverse(int _iX1, int _iX2, int _iY1, int _iY2,
                         int _iCol1, int _iCol2)
{
  int x,y;

  for(x = _iX1;x < _iX2;x++)
    for(y = _iY1;y < _iY2;y++)
      if(getpixel(x,y) == _iCol1)
        putpixel(x,y,_iCol2);
}

/**************************************************************************
<FUNCTION>
  <NAME> Cand_Switch </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
static void Cand_Switch(int _iZ1)
{
 int ax1, ax2, ay1, ay2;

  /* Get coordinates */
  Cand_Coord(_iZ1, &ax1, &ax2, &ay1, &ay2);
  setfillstyle(SOLID_FILL, BLACK);
  if(_iZ1 > 0)/*|| !giSWCandidRadio)*/
  {
    if(arcMemoCandid[giCurrentScr][giCurrentList][_iZ1] == 1)/* was Selected */
      // Emulating pixel painting is super slow -> let's paint less...
      Cand_Reverse(ax1,/*ax2*/ax1+28,ay1,ay2, WHITE, 7); /* White -> LightGrey */
    else
      Cand_Reverse(ax1,/*ax2*/ax1+28,ay1,ay2, 7, WHITE); /* LightGrey -> White */
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Cand_Circle </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  Draw a circle ("checkbox", like paper vote)
  </DESCRIPTION>
  <PARAMETERS>
    <PARAMETER>
      <NAME> z </NAME>
      <TYPE> int </TYPE>
      <IO> IN </IO>
      <DESCRIPTION>
      Candidate's id
      </DESCRIPTION>
    </PARAMETER>
  </PARAMETERS>
</FUNCTION>
**************************************************************************/
static void Cand_Circle(int z)
{
 int xc,yc,diam;
 int ax1, ax2, ay1, ay2;
 
#ifdef EL2014
  if (z != 0) return;
#endif 
 xc = gaiPosc[z][1];

 /* Get Coordinates */
 Cand_Coord(z, &ax1, &ax2, &ay1, &ay2);

 if(z == 0) /* list's head */
 {
    xc -= (gaiPosc[z][1] - gaiPosc[z][0] ) / 2;
    yc = 15;
    diam = 11;
 }
 else /* One specific candidate */
 {
    xc = xc - giSaisieCandidL / 2;
    yc = gaiPosc[z][2];
    yc = yc + (giCadreCandidH / 2);
    diam = 8;
 }

 settextstyle(SMALL_FONT, HORIZ_DIR, 6);
 setcolor(8);
 circle(xc ,yc , diam);
 if(arcMemoCandid[giCurrentScr][giCurrentList][z] == 1) /* Selected */
 {
//   setfillstyle(SOLID_FILL, BLACK);
   setcolor(BLACK);
   floodfill(xc ,yc ,8);
   Cand_Switch(z);
 }
 else /* Unselected */
 {
//   setfillstyle(SOLID_FILL, WHITE);
   setcolor(WHITE);
   floodfill(xc,yc, 8);
 }
 setcolor(BLACK);
 setfillstyle(SOLID_FILL, WHITE);
 settextstyle(SMALL_FONT, HORIZ_DIR, 4);
}

/**************************************************************************
<FUNCTION>
  <NAME> Display_Candidates_Attributes </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  Display the candidate's attributes (name, firstname)
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
static void Display_Candidates_Attributes(int _iX, int _iY)
{
 int iType; /* Name(1), Name & Firstname(2) */

  iType = 0;
  if(gshCand->nom_c[0] != 0x00)    iType = 1;
  if(gshCand->prenom_c[0] != 0x00) iType = 2;

  settextjustify(LEFT_TEXT,CENTER_TEXT);
  outtextxy(_iX +22/*1*/, _iY + giNomY, gshCand->nom_c);
  if(iType == 2)
  {
    if(!SECOND_LINE_CENTERED) {
      outtextxy(_iX +22/*1*/,_iY + giPrenomY, gshCand->prenom_c);
	}
    else
    {
      settextjustify(CENTER_TEXT,CENTER_TEXT);
#ifdef EL2014	  
      outtextxy(_iX+ (giCadreCandidL) /2,_iY + giPrenomY, gshCand->prenom_c);
#else	  
      outtextxy(_iX+ (giCadreCandidL-28) /2,_iY + giPrenomY, gshCand->prenom_c);
#endif	  
    }
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Cancel_Selection </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  Reset vote's memory
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
static void Cancel_Selection(int x, int y, int sw)
{
  int m;

  arcMemoScrutin[x] = 0;
  arcMemoList[x][y] = 0;

  for(m = 0; m < NB_MAX_CANDID-1; m++)
  {
    if(arcMemoCandid[x][y][m] != 0)
    {
      arcMemoCandid[x][y][m] = 0;
      if(sw)
      {
        Cand_Update(m);
        Cand_Circle(m);
        Cand_Switch(m);
      }
    }
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Cand_Select </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Specify that a selection has been done
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Cand_Select(int _x, int _y, int _z)
{
  arcMemoCandid[_x][_y][_z] = 1;
  arcMemoList[_x][_y] = 1;
  arcMemoScrutin[_x] = 1;
}

#ifdef EL2014
/**************************************************************************
<FUNCTION>
  <NAME> Cand_Unselect </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Specify that a selection has been undone
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Cand_Unselect(int _x, int _y, int _z)
{
  arcMemoCandid[_x][_y][_z] = 0;
  //280613 arcMemoList[_x][_y] = 0;
  //280613 arcMemoScrutin[_x] = 0;
}
#endif /* EL2014 */

/**************************************************************************
<FUNCTION>
  <NAME> Cand_Check_Selection </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  Action (go back, validation) (with the Light Pen)
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
static int Cand_Check_Selection(int _iX, int _iY)
{
  int i, x1, x2, y1, y2;
  

  if(_iY > 452) /* At the bottom (Validate/Cancel or
                   go back tho the lists) */
  {
    if(giNbCandidSel != 0) /* Almost one candidate selected */
    {
#ifdef EL2014
      if((_iX > 10) && (_iX < 310)) /* Cancel */
      {
        giNbCandidSel = 0;
        swpas   = 0;
        swnoir  = 0;
        swconf  = 0;
        Cancel_Selection(giCurrentScr,giCurrentList,1);
        return(0);
      }
      if((_iX > 330) && (_iX < 630)) /* Validation */
      {
        if((gshScrutin[giCurrentScr].nom_s == '4') && (Choix_RLg == 'F'))
			swVLR = 0;        
		++giCurrentScr;
        ptfunct = Scrut_Loop;
        return(1);
      }
#else        
      if((_iX > 10) && (_iX < 310)) /* Validation */
      {
        ++giCurrentScr;
        ptfunct = Scrut_Loop;
        return(1);
      }
      if((_iX > 330) && (_iX < 630)) /* Cancel */
      {
        giNbCandidSel = 0;
        swpas   = 0;
        swnoir  = 0;
        swconf  = 0;
        Cancel_Selection(giCurrentScr,giCurrentList,1);
        return(0);
      }
#endif	  
      return(0);
    }
    else /* No candidate selected -> go back to the list selection */
    {
      if((_iX > 110) && (_iX < 530))
      {
#ifdef EL2014
        ptfunct = les_listes;
#else        
        ptfunct = List_Display;
#endif      
        Cancel_Selection(giCurrentScr,giCurrentList,1);
        return(1);
      }
      else /* Not managed */
        return(0);
    }
  }
  else /* Candidate's areas */
  {
    for(i=0;i<=giCurrentCandid;i++) /* Who has been selected ? (LightPen) */
    {
      x1 = gaiPosc[i][0];
      x2 = gaiPosc[i][1];
      y1 = gaiPosc[i][2];
      y2 = gaiPosc[i][3];
      if(i) /* A specific candidate (not the "head of the list") */
      {
        x1 += giXCapture;
        x2 -= giYCapture;
        y1 += giYCapture;
        y2 -= giYCapture;
      }
      if((_iX>=x1) && (_iX<x2))
      {
        if((_iY>=y1) && (_iY<=y2)) /* This area */
        {
  
          if(arcMemoCandid[giCurrentScr][giCurrentList][i] == 0) /* Unselected -> Selected */
          {
            ++giNbCandidSel; /* cfr iSWDeselectC */
            Cand_Select(giCurrentScr,giCurrentList,i);
            Cand_Update(i);
			Cand_Circle(i);
            Cand_Switch(i);
            return(0);
          }
#ifdef EL2014		  
          else /* Selected -> Unselected */
          {
            --giNbCandidSel; /* cfr iSWDeselectC */
			if (giNbCandidSel == 0) {
				swpas = swnoir = swconf = 0;
			}
            Cand_Unselect(giCurrentScr,giCurrentList,i);
            Cand_Update(i);
			Cand_Circle(i);
            Cand_Switch(i);
            return(0);
          }
#endif		  
        }
      }
    }
    return(0);
  }
}

#ifndef EL2014
/**************************************************************************
<FUNCTION>
  <NAME> Cand_Display </NAME>
  <TYPE> GLOBAL </TYPE>
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
int Cand_Display(void)
{
 int Rest_Div, Tampon,nombre_col_bis;
 char buf[30], bufitoa[10];
 int k, j, x, y, iMsgId;
 int iPenX, iPenY, iPenCapture, iHwdDisconnected;

  Rest_Div = giNbCandidSel = swconf = swnoir = swpas = 0;

  /* Init positions, coordinates, lengths... */
  Cand_Init_Display();

  /* Reads machvotc.tbl and fills candidate's structure */
  Cand_Fill();

  /**************************   Head  ****************************/
  buf[0]=gshScrutin[giCurrentScr].nom_s;
  buf[1]= 0x00;
  strcat(buf,gshList->numlist_l);
  buf[3]=gshList->college;
  buf[4]=0x00;
  if(buf[1] == ' ')   buf[1] = '0';

  setfillstyle(SOLID_FILL, BLACK);
  bar(0,0,639,479);
  setfillstyle(SOLID_FILL,WHITE);
  settextstyle(SMALL_FONT, HORIZ_DIR, 6);
  setfillstyle(SOLID_FILL,7);

  /* Area */
  gaiPosc[0][0] = 82;
  gaiPosc[0][1] = gaiPosc[0][0] + 50;
  gaiPosc[0][2] = 2;
  gaiPosc[0][3] = 27;
  bar(gaiPosc[0][0] + 10 ,gaiPosc[0][2], gaiPosc[0][1] - 10 , gaiPosc[0][3]);
  Cand_Circle(0);

  /* Top Right -> current election (commune/province...) */
  setfillstyle(SOLID_FILL,WHITE);
  setcolor(WHITE);
  settextjustify(RIGHT_TEXT,CENTER_TEXT);
  settextstyle( SMALL_FONT, HORIZ_DIR, 5);
  /* Looking for the Label_id corresponding to the current election */
  iMsgId = Msg_Election() +1;
  outtextxy( 630, 13, pszMsg[iMsgId]);
  outtextxy( 629, 13, pszMsg[iMsgId]);
  settextstyle( SMALL_FONT, HORIZ_DIR, 6);

  /* List 's label */
  settextjustify(RIGHT_TEXT,CENTER_TEXT);
  outtextxy(  gaiPosc[0][0] -10, 13, gshList->numlist_l);
  outtextxy(  gaiPosc[0][0] -11, 13, gshList->numlist_l);
  outtextxy(  gaiPosc[0][0] -10, 14, gshList->numlist_l);
  outtextxy(  gaiPosc[0][0] -11, 14, gshList->numlist_l);

  settextjustify(LEFT_TEXT,CENTER_TEXT);
  outtextxy( gaiPosc[0][1] +10, 13, gshList->nom_l);
  outtextxy( gaiPosc[0][1] +11, 13, gshList->nom_l);
  outtextxy( gaiPosc[0][1] +10, 14, gshList->nom_l);
  outtextxy( gaiPosc[0][1] +11, 14, gshList->nom_l);

  /* If view: selected ? */
  if(!giViewCandSelectedMode) Cand_Switch(0);

  /* A separation line */
  setcolor( WHITE);
  line(0,30,639,30);  /* ligne de sparation. */

  /* The candidate's area */
  settextjustify(CENTER_TEXT,CENTER_TEXT);
  settextstyle( SMALL_FONT, HORIZ_DIR, 4);
  setcolor( BLACK);

  /* How much candidates to manage ? */
  giCurrentCandid = aszTabEff[giCurrentScr][giCurrentList] + aszTabSuppl[giCurrentScr][giCurrentList];

  if(aszTabSuppl[giCurrentScr][giCurrentList] > 0) giNombreCol--;

  nombre_col_bis  = aszTabEff[giCurrentScr][giCurrentList] / giNbCandidCol;
  if((aszTabEff[giCurrentScr][giCurrentList] % giNbCandidCol) > 0) nombre_col_bis++;
  if(nombre_col_bis < giNombreCol)   giNombreCol = nombre_col_bis;

  giNbCandidCol = (aszTabEff[giCurrentScr][giCurrentList] / (giNombreCol));
  Rest_Div = (aszTabEff[giCurrentScr][giCurrentList] % (giNombreCol));
  if(giNbCandidCol > 23)
  { giNbCandidCol  = 23;
    Rest_Div =  0;
  }

  /* Display the candidates */
  for(k=1,x = giXDep;(k < aszTabEff[giCurrentScr][giCurrentList] + 1);x+= giCadreCandidL + giInterCandidC)
  {
     if((Rest_Div == 0) || (giNbCandidCol == gaiCandidCol[giCurrentScr])) Tampon = giNbCandidCol;
     else
     {
       Tampon = giNbCandidCol + 1;
       Rest_Div--;
     }
     for( j = 0,y = giDebutAffichageY;
          (k < aszTabEff[giCurrentScr][giCurrentList] + 1) && j < Tampon;
          y+= giCadreCandidH + giInterCandidL,k++,j++)
     {
       gaiPosc[k][0]= x;
       gaiPosc[k][1]= (x + giCadreCandidL) - 1;
       gaiPosc[k][2]=  y;
       gaiPosc[k][3]= (y + giCadreCandidH) - 1;

       settextjustify(CENTER_TEXT,CENTER_TEXT);
       settextstyle( SMALL_FONT, HORIZ_DIR, 5);
       setcolor( WHITE);
       settextjustify(CENTER_TEXT,CENTER_TEXT);
       settextstyle( SMALL_FONT, HORIZ_DIR, 4);
       setcolor( BLACK);

       setfillstyle(SOLID_FILL,WHITE);
       bar(x,y,gaiPosc[k][1],gaiPosc[k][3]);

       /* Num */
       settextjustify(CENTER_TEXT,CENTER_TEXT);
       settextstyle( SMALL_FONT, HORIZ_DIR, 5);
       outtextxy(x + 10,(y + giCadreCandidH/2) -2, sprintf(bufitoa, "%d", k));
       outtextxy(x + 11,(y + giCadreCandidH/2) -2, sprintf(bufitoa, "%d", k));
       settextstyle( SMALL_FONT, HORIZ_DIR, 4);
       /* Sep */
       setfillstyle(SOLID_FILL,BLACK);
       bar(x+20,y,x+20,gaiPosc[k][3]);

       setfillstyle(SOLID_FILL,7);
       bar((x+giCadreCandidL)-giSaisieCandidL, y, gaiPosc[k][1], gaiPosc[k][3]);
       Display_Candidates_Attributes(x, y);
       Cand_Update(k);
       Cand_Circle(k);
       /*Cand_Switch(k);*/
       gshCand++;
     }
  }

  settextjustify(CENTER_TEXT,CENTER_TEXT);
  settextstyle(SMALL_FONT, HORIZ_DIR, 6);
  setcolor( WHITE);

  /* Substitute(s) */
  if ((aszTabSuppl[giCurrentScr][giCurrentList] > 0) && ( aszTabEff[giCurrentScr][giCurrentList] < 70))
  {
    x = 639 -( giXDep + giCadreCandidL);
    y = giDebutAffichageY;
    j = 1;
    outtextxy(x + (giCadreCandidL + giSaisieCandidL) / 2 ,y + ((giCadreCandidH/2) - giInterCandidL) - 1,pszMsg[47]);
    outtextxy(x + ((giCadreCandidL + giSaisieCandidL) / 2 ) + 1 ,y + ((giCadreCandidH/2) - giInterCandidL) - 1,pszMsg[47]);
    settextstyle( SMALL_FONT, HORIZ_DIR, 4);
    setcolor( BLACK);
  }
  else
  {
    if((aszTabSuppl[giCurrentScr][giCurrentList] > 0) && ( aszTabEff[giCurrentScr][giCurrentList] > 69))
    {
      j = j++;
      x-= giCadreCandidL + giInterCandidC;
      outtextxy(x + (giCadreCandidL + giSaisieCandidL) / 2,y + ((giCadreCandidH/2) - giInterCandidL) - 1,pszMsg[47]);
      outtextxy(x + ((giCadreCandidL + giSaisieCandidL)/ 2 ) + 1 ,y + ((giCadreCandidH/2) - giInterCandidL) - 1,pszMsg[47]);
      settextstyle( SMALL_FONT, HORIZ_DIR, 4);
      setcolor( BLACK);
    }
  }
  settextjustify(LEFT_TEXT,CENTER_TEXT);

  for(;(k<giCurrentCandid+1);x+=giCadreCandidL+giInterCandidC,y=0,j=0)
  {
    for(y +=((giCadreCandidH+giInterCandidL));
        (k<giCurrentCandid+1);
        y+=giCadreCandidH + giInterCandidL,k++,j++)
    {
      if(j == 0) y += giDebutAffichageY - (giCadreCandidH + giInterCandidL);

      gaiPosc[k][0]= x;
      gaiPosc[k][1]= (x + giCadreCandidL) - 1;
      gaiPosc[k][2]=  y;
      gaiPosc[k][3]= (y + giCadreCandidH) - 1;

      setfillstyle(SOLID_FILL,WHITE);
      bar(x,y,gaiPosc[k][1],gaiPosc[k][3]);

      setfillstyle(SOLID_FILL,7);
      bar((x+giCadreCandidL)-giSaisieCandidL, y, gaiPosc[k][1], gaiPosc[k][3]);

      Display_Candidates_Attributes(x, y);
      Cand_Update(k);
      Cand_Circle(k);
      Cand_Switch(k);
      gshCand++;
    }
  }
  Set_Cursor(CURSOR_XY);

  /* Visualisation mode */
  if(gcCardRW == CARD_READ)
  {
    Screen_Viewing_Next();
    Wait_Loop_Int(10);
    return(0);
  }

  /* Waiting for an intervention (Interruption (button pressed)
     or selection (LightPen)) */
  giKlavier = 1;
  while(giKlavier)
  {
    Set_Cursor(CURSOR_ON);
    iHwdDisconnected = iPenCapture = Pen_Capture(&iPenX, &iPenY);
    if(iPenCapture == 1)
    {
      Set_Cursor(CURSOR_OFF);
      if(Cand_Check_Selection(iPenX, iPenY))
        return(0);
    }
    PEN_OR_ALARMBOX_DISCONNECTED
  }

  /* Interruption */
  Set_Cursor(CURSOR_OFF);
  giCodeVoteEnd = 13;
  ptfunct = Vote_End;
  return(0);
}

#else /* EL2014 */

/***************************************************************************/
/******            INTEGRATION AFFICHAGE CANDIDATS 2010               ******/
/***************************************************************************/

/***************************************************************************/
/******                       aff_cand.c                              ******/
/***************************************************************************/
/*
* Candidates are displayed using 2010 algorithm 'affichc'
* This algorithm has been update to fit into 2012 version
*    - Cand_Fill() is used instead of Garnir_Cand()
*    - Cand_Circle()
*    - Cand_Update()
*    - Cand_Check_Selection()
* Therefore, some 2012 global variables need to be initialized/used
*/

int affichc()
{
	char buf[30];
	int iNumCdt;
	int nb_cdt_par_colonne;
#ifdef EL2014    
    int i,j,k,x,y,z;
    int code_scr;
	int iPenX, iPenY, iPenCapture, iHwdDisconnected;
#else
	swpas = 0;
	swDeselect = 0;
#endif
       
	Garnir_param();
	param_Candidat();
	
#ifdef EL2014	
	/* init 2012 global variables */
	giCadreCandidL = Lcadre_candid;
	giCadreCandidH = Hcadre_candid;
    giXCapture       = 0;
    giYCapture       = 0;
    giNbCandidSel = swconf = swnoir = swpas = 0;

	Cand_Fill();
#else
	garnir_cand();
#endif	
        
	buf[0]=gshScrutin[giCurrentScr].nom_s;
	buf[1]= 0x00;
	strcat(buf,gshList->numlist_l);
	if(strcmp(gszArrond,"005") == 0 )
		buf[3]='N';
	else
		buf[3]=gshList->college;
	buf[4]=0x00;
	strcat(buf,".bmp");
	if(buf[1] == ' ')   buf[1] = '0';

/**************************   ENTETE CANDIDAT  ****************************/

	setfillstyle(SOLID_FILL,BLACK); /* Set to solid fill in color   */
	bar( 0,0,639,479);      /* Draw backgnd */
	setcolor( WHITE);
	settextstyle(SMALL_FONT, HORIZ_DIR, 6);
	setfillstyle(SOLID_FILL,7);

/**************************   ENTETE CANDIDAT  ****************************/


	if(affTL_code == 1) {
		gaiPosc[0][0] = 320 - 25;
	}
	else if (affTL_code == 2) {
		gaiPosc[0][0] = 80;
	}
	else if (affTL_code == 3) {
		gaiPosc[0][0] = 72;
	}
	else if (affTL_code == 4) {
		gaiPosc[0][0] = 52;
	}
	else if (affTL_code == 5) {
		gaiPosc[0][0] = 52;
	}
	gaiPosc[0][1] = gaiPosc[0][0] + 50;
	gaiPosc[0][2] = 2;
	gaiPosc[0][3] = 27;
	bar(gaiPosc[0][0] + 10 ,gaiPosc[0][2], gaiPosc[0][1] - 10 , gaiPosc[0][3]);
	Cand_Circle(0);
	/**************************   ENTETE CANDIDAT  ****************************/

	setfillstyle(SOLID_FILL,WHITE); /* Set to solid fill in color   */
	setcolor( WHITE);

	settextjustify(RIGHT_TEXT,CENTER_TEXT);
	settextstyle( SMALL_FONT, HORIZ_DIR, 5);

#ifdef EL2014	
	code_scr = Msg_Election();
	if(code_scr == 9)
	   {
		if(strcmp(gszArrond,"005") == 0 ) code_scr+=1;
		if(strcmp(gszArrond,"002") == 0 ) code_scr+=2;
	   }          
#endif	
#ifdef EL2014	
    outtextxy( 640, 13, pszMsg[code_scr]);
	outtextxy( 639, 13, pszMsg[code_scr]);
#else
	outtextxy( 630, 13, pszMsg[code_scr]);
	outtextxy( 629, 13, pszMsg[code_scr]);
#endif	



	settextstyle( SMALL_FONT, HORIZ_DIR, 6);
	if      (affTL_code == 1) {
		settextjustify(LEFT_TEXT,CENTER_TEXT);
		outtextxy(  5, 13, gshList->numlist_l);
		outtextxy(  6, 13, gshList->numlist_l);
		outtextxy(  5, 14, gshList->numlist_l);
		outtextxy(  6, 14, gshList->numlist_l);

		if(access(buf,0) == 0)
			bmp(buf, gaiPosc[0][0] -10,2,0);

		else
		{
			settextjustify(RIGHT_TEXT,CENTER_TEXT);
			outtextxy( gaiPosc[0][0] -10, 13, gshList->nom_l);
			outtextxy( gaiPosc[0][0] -11, 13, gshList->nom_l);
			outtextxy( gaiPosc[0][0] -10, 14, gshList->nom_l);
			outtextxy( gaiPosc[0][0] -11, 14, gshList->nom_l);
		}
	}
	else if (affTL_code == 2) {
		settextjustify(RIGHT_TEXT,CENTER_TEXT);
		outtextxy( gaiPosc[0][0] -10, 13, gshList->numlist_l);
		outtextxy( gaiPosc[0][0] -11, 13, gshList->numlist_l);
		outtextxy( gaiPosc[0][0] -10, 14, gshList->numlist_l);
		outtextxy( gaiPosc[0][0] -11, 14, gshList->numlist_l);
		if(access(buf,0) == 0)
			bmp(buf, gaiPosc[0][1] +10,2,0);
		else
		{
			settextjustify(LEFT_TEXT,CENTER_TEXT);
			outtextxy( gaiPosc[0][1] +10, 13, gshList->nom_l);
			outtextxy( gaiPosc[0][1] +11, 13, gshList->nom_l);
			outtextxy( gaiPosc[0][1] +10, 14, gshList->nom_l);
			outtextxy( gaiPosc[0][1] +11, 14, gshList->nom_l);
		}
	}
	else if (affTL_code == 3) {
		settextjustify(RIGHT_TEXT,CENTER_TEXT);
		outtextxy( gaiPosc[0][0] -10, 13, gshList->numlist_l);
		outtextxy( gaiPosc[0][0] -11, 13, gshList->numlist_l);
		outtextxy( gaiPosc[0][0] -10, 14, gshList->numlist_l);
		outtextxy( gaiPosc[0][0] -11, 14, gshList->numlist_l);

		if(access(buf,0) == 0)
			bmp(buf, gaiPosc[0][1] +10,2,0);
		else
		{
		settextjustify(LEFT_TEXT,CENTER_TEXT);
		outtextxy( gaiPosc[0][1] +10, 13, gshList->nom_l);
		outtextxy( gaiPosc[0][1] +11, 13, gshList->nom_l);
		outtextxy( gaiPosc[0][1] +10, 14, gshList->nom_l);
		outtextxy( gaiPosc[0][1] +11, 14, gshList->nom_l);
		}
	}
	else if (affTL_code == 4) {
		settextjustify(RIGHT_TEXT,CENTER_TEXT);
		outtextxy( gaiPosc[0][0] -10, 13, gshList->numlist_l);
		outtextxy( gaiPosc[0][0] -11, 13, gshList->numlist_l);
		outtextxy( gaiPosc[0][0] -10, 14, gshList->numlist_l);
		outtextxy( gaiPosc[0][0] -11, 14, gshList->numlist_l);

		if(access(buf,0) == 0)
			bmp(buf, gaiPosc[0][1] +10,2,0);
		else
		{
		settextjustify(LEFT_TEXT,CENTER_TEXT);
		outtextxy( gaiPosc[0][1] +10, 13, gshList->nom_l);
		outtextxy( gaiPosc[0][1] +11, 13, gshList->nom_l);
		outtextxy( gaiPosc[0][1] +10, 14, gshList->nom_l);
		outtextxy( gaiPosc[0][1] +11, 14, gshList->nom_l);
		}
	}
	else if (affTL_code == 5) {
		settextjustify(RIGHT_TEXT,CENTER_TEXT);
		outtextxy( gaiPosc[0][0] -10, 13, gshList->numlist_l);
		outtextxy( gaiPosc[0][0] -11, 13, gshList->numlist_l);
		outtextxy( gaiPosc[0][0] -10, 14, gshList->numlist_l);
		outtextxy( gaiPosc[0][0] -11, 14, gshList->numlist_l);

		if(access(buf,0) == 0)
			bmp(buf, gaiPosc[0][1] +10,2,0);

		else
		{
			settextjustify(LEFT_TEXT,CENTER_TEXT);
			outtextxy( gaiPosc[0][1] +10, 13, gshList->nom_l);
			outtextxy( gaiPosc[0][1] +11, 13, gshList->nom_l);
			outtextxy( gaiPosc[0][1] +10, 14, gshList->nom_l);
			outtextxy( gaiPosc[0][1] +11, 14, gshList->nom_l);
		}
	}

	line(0,30,639,30);  /* ligne de sparation. */
/**************                AFFICHAGE DES CANDIDATS        ***************/

	settextjustify(CENTER_TEXT,CENTER_TEXT);
	settextstyle( SMALL_FONT, HORIZ_DIR, 4);
	setcolor( BLACK);

	giCurrentCandid = aszTabEff[giCurrentScr][giCurrentList] + aszTabSuppl[giCurrentScr][giCurrentList];
	type_ecran(&nb_cdt_par_colonne);
	for(k=1,x = x_dep;(k < aszTabEff[giCurrentScr][giCurrentList] + 1);x+= Lcadre_candid + Interc_candid)
	{
#ifdef EL2014
        /* pour l'election DE, les candidats de la colonne 2 sont affichés en colonne 3 */
        if ((gshScrutin[giCurrentScr].TYPE_SCRUTIN == IS_REGION_GERMANO) && (x == x_dep + Lcadre_candid + Interc_candid)) {
            x+= Lcadre_candid + Interc_candid;      
        }             
#endif
		for(j = 0,y = Debut_Affichage;(k < aszTabEff[giCurrentScr][giCurrentList] + 1) && j < nb_cdt_par_colonne;y+= Hcadre_candid + Interl_candid,k++,j++)
		{
#ifdef EL2014		
			gaiPosc[k][0]= x;
			gaiPosc[k][1]= (x + Lcadre_candid) - 1;
			gaiPosc[k][2]= y;
			gaiPosc[k][3]= (y + Hcadre_candid) - 1;			

			setfillstyle(SOLID_FILL,WHITE);
			bar(x,y,gaiPosc[k][1],gaiPosc[k][3]);
#else		
			gaiPosc[k][0]= (x + Lcadre_candid) - LAsaisie_candid;
			gaiPosc[k][1]= (x + Lcadre_candid) - 1;
			gaiPosc[k][2]=  y + HAsaisie_candid;
			gaiPosc[k][3]= (y + Hcadre_candid) - 1 - HAsaisie_candid;			

			setfillstyle(SOLID_FILL,WHITE);
			bar(x,y,gaiPosc[k][1],gaiPosc[k][3] + HAsaisie_candid);
#endif			
			Cal_affiche(x,y);
			Cand_Update(k);
			Cand_Circle(k);
			affiche_num_cdt(x,y,k,k);
#ifdef EL2014
            if(gcCardRW == CARD_READ && arcMemoCandid[giCurrentScr][giCurrentList][k] == 1) 
				Cand_Switch(k);
#endif  
			gshCand++;
		}
	}

	settextstyle(SMALL_FONT, HORIZ_DIR, 6);
	setcolor( WHITE);

	if ((aszTabSuppl[giCurrentScr][giCurrentList] > 0) && ( aszTabEff[giCurrentScr][giCurrentList] < 70))
	{
		x = 639 -( x_dep + Lcadre_candid);
		y = Debut_Affichage;
		j = 1;
		outtextxy(x + (Lcadre_candid / 2 ) ,y + ((Hcadre_candid/2) - Interl_candid),pszMsg[47]);
		outtextxy(x + (Lcadre_candid / 2 ) + 1 ,y + ((Hcadre_candid/2) - Interl_candid),pszMsg[47]);
		settextstyle( SMALL_FONT, HORIZ_DIR, 4);
		setcolor( BLACK);
	}
	else
		if((aszTabSuppl[giCurrentScr][giCurrentList] > 0) && ( aszTabEff[giCurrentScr][giCurrentList] > 69))
		{
			j = j++;
			x-= Lcadre_candid + Interc_candid;
			outtextxy(x + (Lcadre_candid / 2 ) ,y + ((Hcadre_candid/2) - Interl_candid),pszMsg[47]);
			outtextxy(x + (Lcadre_candid / 2 ) + 1 ,y + ((Hcadre_candid/2) - Interl_candid),pszMsg[47]);
			settextstyle( SMALL_FONT, HORIZ_DIR, 4);
			setcolor( BLACK);
		}
	for(;(k < giCurrentCandid + 1);x+= Lcadre_candid + Interc_candid,y =  0,j = 0)
		for(iNumCdt=1, y += ((Hcadre_candid + Interl_candid)) ;(k < giCurrentCandid + 1) ;y+= Hcadre_candid + Interl_candid,k++,j++, iNumCdt++)
		{
		if(j == 0)
			y += Debut_Affichage - (Hcadre_candid + Interl_candid);
			
#ifdef EL2014		
		gaiPosc[k][0]= x;
		gaiPosc[k][1]= (x + Lcadre_candid) - 1;
		gaiPosc[k][2]= y;
		gaiPosc[k][3]= (y + Hcadre_candid) - 1;			

		setfillstyle(SOLID_FILL,WHITE);
		bar(x,y,gaiPosc[k][1],gaiPosc[k][3]);
#else		
		gaiPosc[k][0]= (x + Lcadre_candid) - LAsaisie_candid;
		gaiPosc[k][1]= (x + Lcadre_candid) - 1;
		gaiPosc[k][2]=  y + HAsaisie_candid;
		gaiPosc[k][3]= (y + Hcadre_candid) - 1 - HAsaisie_candid;			

		setfillstyle(SOLID_FILL,WHITE);
		bar(x,y,gaiPosc[k][1],gaiPosc[k][3] + HAsaisie_candid);
#endif			
		Cal_affiche(x,y);
		Cand_Update(k);
		Cand_Circle(k);
		affiche_num_cdt(x,y,k,iNumCdt);
#ifdef EL2014
            if(gcCardRW == CARD_READ && arcMemoCandid[giCurrentScr][giCurrentList][k] == 1) 
				Cand_Switch(k);
#endif  
		gshCand++;
		}
        
        Set_Cursor(CURSOR_XY);

        /* Visualisation mode */
        if(gcCardRW == CARD_READ)
        {
          Screen_Viewing_Next();
          Wait_Loop_Int(10);
          return(0);
        }
        
  /* Waiting for an intervention (Interruption (button pressed)
     or selection (LightPen)) */
  giKlavier = 1;
  while(giKlavier)
  {
    Set_Cursor(CURSOR_ON);
    iHwdDisconnected = iPenCapture = Pen_Capture(&iPenX, &iPenY);
    if(iPenCapture == 1)
    {
      Set_Cursor(CURSOR_OFF);
      if(Cand_Check_Selection(iPenX, iPenY)) return(0);
    }
    PEN_OR_ALARMBOX_DISCONNECTED
  }

  /* Interruption */
  Set_Cursor(CURSOR_OFF);
  giCodeVoteEnd = 13;
  ptfunct = Vote_End;
  return(0);
}

#ifdef EL2014
void Cal_affiche(int x, int y)
#else
void Cal_affiche(void)
#endif
{
	int nombre/*, diff3*/;


	nombre = 0;
	if(gshCand->nom_c[0] != 0x00)
	nombre = 1;
	if(gshCand->prenom_c[0] != 0x00)
	nombre = 2;
	if(Condense == 1)
	{
		(gshCand->nom_c[18] = 0x00);
		(gshCand->prenom_c[18] = 0x00);
	}
	switch(nombre)
	{
	case 1 :
		settextjustify(LEFT_TEXT,CENTER_TEXT);
		outtextxy(x+CDT_NUM_WIDTH +5,(y + Hcadre_candid/3) -2, gshCand->nom_c);
		break;
	case 2 :
		settextjustify(LEFT_TEXT,CENTER_TEXT);
		outtextxy(x+CDT_NUM_WIDTH +5,(y + Hcadre_candid/3) -2, gshCand->nom_c);
		settextjustify(CENTER_TEXT,CENTER_TEXT);
		outtextxy(x+((CDT_NUM_WIDTH+Lcadre_candid) / 2 ),y + ((Hcadre_candid * 2) / 3) , gshCand->prenom_c);
		break;
	}
}

void type_ecran(int *_nb_cdt_par_colonne)
{
	int iNbreDeSiegeAPourvoir;
	int iNbreColonne;
	int iNbreEffectif;
	
	iNbreDeSiegeAPourvoir = atoi(gshScrutin[giCurrentScr].scrutin_max_cand) - atoi(gshScrutin[giCurrentScr].scrutin_max_supp) - 1;
	iNbreEffectif = aszTabEff[giCurrentScr][giCurrentList];

	if(iNbreDeSiegeAPourvoir <= iRuptureColonne)
				iNbreColonne = 1;
	else 
	{
	  if(iNbreEffectif <= iRuptureColonne) /* candidats titulaires <= iNbreDeSiegeAPourvoir */
				iNbreColonne = 1;
	  else /* candidats titulaires >= 15 */
				iNbreColonne = iNbreEffectif/iRuptureColonne;
	}
	if((iNbreColonne * iRuptureColonne) < iNbreEffectif)
				iNbreColonne++;

	/* Nbr cdt par colonne */
	if(iNbreColonne == 1)
		(*_nb_cdt_par_colonne) = iNbreEffectif;
	else 
	{  if(iNbreColonne < 4)
	       {
		(*_nb_cdt_par_colonne) = iNbreEffectif / iNbreColonne;
		if((*_nb_cdt_par_colonne * iNbreColonne)< iNbreEffectif)
		++(*_nb_cdt_par_colonne);
	       }
	    else
		*_nb_cdt_par_colonne=iRuptureColonne;

	}

}

#ifdef EL2014
void affiche_num_cdt(int x, int y, int k, int _iNum)
#else
void affiche_num_cdt(int _iNum)
#endif
{
	char c[3];

	/* Separateur */
	setfillstyle(SOLID_FILL,BLACK);
	bar(x+CDT_NUM_WIDTH-1,y,x+CDT_NUM_WIDTH-1,gaiPosc[k][3] + HAsaisie_candid);

	/* Num?o */
	sprintf(c, "%2d", _iNum);
	settextstyle( SMALL_FONT, HORIZ_DIR, 5);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	outtextxy(x+(CDT_NUM_WIDTH/2)-1,(y + Hcadre_candid/2) -1, c);
	settextstyle( SMALL_FONT, HORIZ_DIR, 4);
}

/***************************************************************************/
/******                       garnir.c                                ******/
/***************************************************************************/

/****************************** GARNIR LES MESSAGES **************************/
#ifdef EL2014
void Garnir_param()
{
	switch(gshScrutin[giCurrentScr].TYPE_SCRUTIN)
	 {
		case 1:
			iRuptureColonne = 15;
			break;
		case 2:
			iRuptureColonne = 15;
			break;
		case 3:
			iRuptureColonne = 22;
			break;
		case 4: 
			iRuptureColonne = 15;
			if(strcmp(gszArrond,"005") == 0) 
				  iRuptureColonne = 22;
			if(strcmp(gszArrond,"002") == 0)
				  iRuptureColonne = 23;
			break;
		case 7:
			iRuptureColonne = 15;
			break;
		case 9:
			iRuptureColonne = 15;
			break;
	 }

}
#else
void Garnir_param()
{
	switch(gshScrutin[giCurrentScr].TYPE_SCRUTIN)
	 {
		case 1:
			iRuptureColonne = 14;
			break;
		case 2:
			iRuptureColonne = 14;
			break;
		case 3:
			iRuptureColonne = 22;
			break;
		case 4: 
			iRuptureColonne = 14;
			if(strcmp(gszArrond,"005") == 0) 
				  iRuptureColonne = 22;
			if(strcmp(gszArrond,"002") == 0)
				  iRuptureColonne = 23;
			break;
		case 7:
			iRuptureColonne = 14;
			break;
	 }

}
#endif

/***************************************************************************/
/******                       blanc.c                                ******/
/***************************************************************************/

int param_Candidat(void)
{
	
	nbr_sup = atoi(gshScrutin[giCurrentScr].scrutin_max_supp);
	nbr_max = atoi(gshScrutin[giCurrentScr].scrutin_max_cand) - 1;/* tete de liste */
	nbc_col = iRuptureColonne;
	nombre_col  = (nbr_max - nbr_sup) / nbc_col;
	if(((nbr_max - nbr_sup) % nbc_col) > 0) nombre_col++;
	if(nbr_sup > 0)
		nombre_col++;
	if(nombre_col == 5)
		nombre_col--;
	if(nombre_col > 5)
                Msg_Error_Exit("Trop de colonnes...");
	if(nbr_sup > (nbc_col - 1))
		Msg_Error_Exit("Trop de supplants...");

#include "c_meth4.c"
	return(0);
}

#include "bmp.c"

#endif /* EL2014 */