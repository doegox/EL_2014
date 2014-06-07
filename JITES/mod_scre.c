/*
#define EL2014
*/

/**************************************************************************
<MODULE NAME>
<FILENAME> mod_scre.c (like screen) </FILENAME>
<HISTORY>
  <MODIFICATION>
    <DATE> </DATE>
    <AUTHOR>  </AUTHOR>
    <DESCRIPTION>
    Specific screens...
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
#include <graphics.h>
//#include "libcrdrd.h"
//#include "libalrbx.h"
#include "defs.h"

/**************************************************************************
EXTERNAL VARIABLES
**************************************************************************/
extern char   gcLg;
extern char   gcCodeLg;
extern char   gcCodeLgOri;
extern int    giSWCard;
extern char **pszMsg;
extern char  *pszMsg1[N_SCR_MESSAGE];
extern char  *pszMsg2[N_SCR_MESSAGE];


/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/
extern void bararrondi(int, int , int, int);
extern void Leds(char, int);
extern int  Msg_Election(void);
extern void Set_Cursor(int);

/**************************************************************************
GLOBAL VARIABLES
**************************************************************************/
int gaiPosv[10][4];

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Wait </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Wait(void)
{
  setfillstyle(SOLID_FILL,WHITE);
  bar(0,0,639,479);
  setcolor(BLACK);
  settextstyle(SMALL_FONT, HORIZ_DIR,13);
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  if(gcCodeLg == '4' ||
     gcCodeLg == '5' ||
/* << 9.01c */
     /*gcCodeLg == '6' )*/
     gcCodeLg == '6' ||
     gcCodeLg == '8' )
/* 9.01c >> */
  {
    outtextxy(320, 160, pszMsg1[88]);
    outtextxy(321, 160, pszMsg1[88]);
    outtextxy(320, 340, pszMsg2[88]);
    outtextxy(321, 340, pszMsg2[88]);
  }
  else
  {
    outtextxy(320, 240, pszMsg1[88]);
    outtextxy(321, 240, pszMsg1[88]);
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Bad_Crc </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Bad_Crc(void)
{
  setfillstyle(SOLID_FILL,WHITE);
  bar(0,0,639,479);
  setcolor(BLACK);
  settextstyle(SMALL_FONT, HORIZ_DIR,13);
  if(gcCodeLg == '4' ||
     gcCodeLg == '5' ||
/* << 9.01c */
     /*gcCodeLg == '6' )*/
     gcCodeLg == '6' ||
     gcCodeLg == '8' )
/* 9.01c >> */
  {
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(320, 160, pszMsg1[87]);
    outtextxy(321, 160, pszMsg1[87]);
    settextjustify(CENTER_TEXT, BOTTOM_TEXT);
    outtextxy (320, 340,pszMsg2[87]);
    outtextxy (321, 340,pszMsg2[87]);
    /* switch on the red and green leds + bips */
    Leds(RG,1);
  }
  else
  {
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(320, 160, pszMsg1[87]);
    outtextxy(321, 160, pszMsg1[87]);
    settextjustify(CENTER_TEXT, BOTTOM_TEXT);
    outtextxy (320, 340,pszMsg1[87]);
    outtextxy (321, 340,pszMsg1[87]);
    /* switch on the red and green leds + bips */
    Leds(RG,1);
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Pwd </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Pwd(void)
{
  int j, k, x, y, nbcol;
  char cBuff[100];

  /* Screen Background color */
  setfillstyle(SOLID_FILL,BLACK);
  bar(0,0,639,479);

  /* Title: "Code ?" */
  setcolor(WHITE);
  settextstyle(SMALL_FONT, HORIZ_DIR,12);
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  if(gcCodeLg == '4' ||
     gcCodeLg == '5' ||
/* << 9.01c */
     /*gcCodeLg == '6' )*/
     gcCodeLg == '6' ||
     gcCodeLg == '8' )
/* 9.01c >> */
  {
    outtextxy(160, 50, pszMsg1[89]);
    outtextxy(161, 50, pszMsg1[89]);
    outtextxy(480, 50, pszMsg2[89]);
    outtextxy(481, 50, pszMsg2[89]);
  }
  else
  {
    outtextxy(320, 50, pszMsg1[89]);
    outtextxy(321, 50, pszMsg1[89]);
  }

  settextjustify(CENTER_TEXT,CENTER_TEXT);
  settextstyle( SMALL_FONT, HORIZ_DIR, 4);
  setcolor(WHITE);

  /* Build "Num Pad" */
  for(y = 120,k = 0,j = 0 ; j < 4; y+= 75,j++)
  for(nbcol = 0,x = 210; (nbcol < 3) && (k < 11); k++ ,x+= 75,nbcol++ )
  {
    gaiPosv[k][0]= x;
    if(k == 10) gaiPosv[k][1]= x + 140;
    else        gaiPosv[k][1]= x + 65;
    gaiPosv[k][2]= y ;
    gaiPosv[k][3]= y + 65;
    setfillstyle(SOLID_FILL,WHITE);
    bar(x,y,gaiPosv[k][1],gaiPosv[k][3]);
    setfillstyle(SOLID_FILL,BLACK); /* Set to solid fill in color   */
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    settextstyle(SMALL_FONT, HORIZ_DIR, 6);
    if(k== 9) strcpy(cBuff,"0");
    else      sprintf(cBuff, "%d", k+1);
    setcolor(BLACK);
    if(k < 10)
    {
      outtextxy(gaiPosv[k][0] + 10,gaiPosv[k][2] + 10,cBuff);
      outtextxy(gaiPosv[k][0] + 11,gaiPosv[k][2] + 10,cBuff);
    }
    else
    {
      if(gcCodeLg == '4' ||
         gcCodeLg == '5' ||
/* << 9.01c */
         /*gcCodeLg == '6' )*/
         gcCodeLg == '6' ||
         gcCodeLg == '8' )
/* 9.01c >> */
      {
        outtextxy(gaiPosv[k][0] + 64,gaiPosv[k][2] + 16, pszMsg1[94]);
        outtextxy(gaiPosv[k][0] + 64,gaiPosv[k][2] + 48, pszMsg2[94]);
      }
      else outtextxy(gaiPosv[k][0] + 64,gaiPosv[k][2] + 32, pszMsg1[94]);
    }
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_New_Vote </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Screen asking the introduction of a voting card
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_New_Vote(void)
{
#ifdef TST_EL2014
    char *p;
    
    p = strstr (pszMsg1[107], "vote");
    if (p != NULL) strcpy(pszMsg1[107], "pour d‚marrer le TEST.");
    p = strstr (pszMsg1[107], "stemmen");
    if (p != NULL) strcpy(pszMsg1[107], "om de TEST te starten.");
    p = strstr (pszMsg1[107], "Wahlvorgang");
    if (p != NULL) strcpy(pszMsg1[107], "um den TEST zu starten.");
    
    p = strstr (pszMsg2[107], "vote");
    if (p != NULL) strcpy(pszMsg2[107], "pour d‚marrer le TEST.");
    p = strstr (pszMsg2[107], "stemmen");
    if (p != NULL) strcpy(pszMsg2[107], "om de TEST te starten.");
    p = strstr (pszMsg2[107], "Wahlvorgang");
    if (p != NULL) strcpy(pszMsg2[107], "um den TEST zu starten.");
#endif  
    
  setfillstyle(SOLID_FILL, WHITE);
  bar(0,0,639,479);
  setcolor ( BLACK );
  settextstyle( SMALL_FONT, HORIZ_DIR, 9);
  settextjustify(CENTER_TEXT, CENTER_TEXT);

  /* Introduce the card */
  if(gcCodeLg == '4' ||
     gcCodeLg == '5' ||
/* << 9.01c */
     /*gcCodeLg == '6' )*/
     gcCodeLg == '6' ||
     gcCodeLg == '8' )
/* 9.01c >> */
  {
    outtextxy (320, 120,pszMsg1[30]);
    outtextxy (320, 150,pszMsg1[31]);
    outtextxy (320, 180,pszMsg1[107]);
    outtextxy (321, 120,pszMsg1[30]);
    outtextxy (321, 150,pszMsg1[31]);
    outtextxy (321, 180,pszMsg1[107]);

    outtextxy (320, 320,pszMsg2[30]);
    outtextxy (320, 350,pszMsg2[31]);
    outtextxy (320, 380,pszMsg2[107]);
    outtextxy (321, 320,pszMsg2[30]);
    outtextxy (321, 350,pszMsg2[31]);
    outtextxy (321, 380,pszMsg2[107]);
  }
  else
  {
     outtextxy (320, 210,pszMsg1[30]);
     outtextxy (320, 240,pszMsg1[31]);
     outtextxy (320, 270,pszMsg1[107]);
     outtextxy (321, 210,pszMsg1[30]);
     outtextxy (321, 240,pszMsg1[31]);
     outtextxy (321, 270,pszMsg1[107]);

     if(gcCodeLg == '1' || gcCodeLg == '2') /* Button for changing */
     {
       setfillstyle(SOLID_FILL, WHITE);
       bararrondi(110,450,530,479-2);
       setcolor(BLACK);
       settextjustify(CENTER_TEXT, CENTER_TEXT);
       settextstyle( SMALL_FONT, HORIZ_DIR, 7);
       outtextxy(320,459, pszMsg[2]);
       outtextxy(321,459, pszMsg[2]);
     }
  }

  if(giSWCard == _NOCARD) outtextxy (300, 40,"Demo");
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Ask_Viewing </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Ask_Viewing(void)
{
  setfillstyle(SOLID_FILL, WHITE);
  bar(0,0,639,479);
  setcolor(BLACK);
  settextstyle(SMALL_FONT,HORIZ_DIR, 8);
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtextxy(320, 160, pszMsg[32]);
  outtextxy(321, 160, pszMsg[32]);
  outtextxy(320, 161, pszMsg[32]);

  outtextxy(320, 230, pszMsg[33]);
  outtextxy(321, 230, pszMsg[33]);
  outtextxy(320, 231, pszMsg[33]);

  outtextxy(320, 320, pszMsg[90]);
  outtextxy(321, 320, pszMsg[90]);
  outtextxy(320, 321, pszMsg[90]);

  setcolor (BLACK);
#ifdef EL2014
  /* No */
  rectangle(107,374,283,446);
  rectangle(108,375,282,445);
  outtextxy(195,407,pszMsg[91]);
  outtextxy(196,407,pszMsg[91]);

  /* Yes */
  rectangle(357,374,533,446);
  rectangle(358,375,532,445);
  outtextxy(445,407,pszMsg[74]);
  outtextxy(446,407,pszMsg[74]);
#else
  /* Yes */
  rectangle(107,374,283,446);
  rectangle(108,375,282,445);
  outtextxy(195,407,pszMsg[74]);
  outtextxy(196,407,pszMsg[74]);

  /* No */
  rectangle(357,374,533,446);
  rectangle(358,375,532,445);
  outtextxy(445,407,pszMsg[91]);
  outtextxy(446,407,pszMsg[91]);
#endif  
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Take_Back </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Take_Back(void)
{
  setfillstyle(SOLID_FILL, WHITE);
  bar(0,0,639,479);
  setcolor ( BLACK );
  settextstyle( SMALL_FONT, HORIZ_DIR, 9 );
  outtextxy(320,160, pszMsg[39]);
  outtextxy(321,160, pszMsg[39]);
  outtextxy(320,161, pszMsg[39]);

  outtextxy(320,300, pszMsg[40]);
  outtextxy(321,300, pszMsg[40]);
  outtextxy(320,301, pszMsg[40]);
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Interrupt </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Interrupt(void)
{
#ifdef EL2014
    int sz1,sz2;
    
	if(gcLg == 'A') {
            sz1 = 8;
            sz2 = 7;
        }
        else 
	{
            sz1 = 10;
            sz2 = 7;
        }

        setfillstyle(SOLID_FILL, WHITE);
        bar(0,0,639,479);
        setcolor ( BLACK );
        settextstyle( SMALL_FONT, HORIZ_DIR, sz1 );
        outtextxy(320,80, pszMsg[38]);
        outtextxy(321,80, pszMsg[38]);
        outtextxy(320,81, pszMsg[38]);
        settextstyle( SMALL_FONT, HORIZ_DIR, sz2 );

        outtextxy(320,150, pszMsg[39]);
        outtextxy(321,150, pszMsg[39]);
        outtextxy(320,151, pszMsg[39]);

        outtextxy(320,200, pszMsg[40]);
        outtextxy(321,200, pszMsg[40]);
        outtextxy(320,201, pszMsg[40]);

        outtextxy(320,250, pszMsg[108]);
        outtextxy(321,250, pszMsg[108]);
        outtextxy(320,251, pszMsg[108]);

        outtextxy(320,300, pszMsg[109]);
        outtextxy(321,300, pszMsg[109]);
        outtextxy(320,301, pszMsg[109]);

        outtextxy(320,350, pszMsg[110]);
        outtextxy(321,350, pszMsg[110]);
        outtextxy(320,351, pszMsg[110]);
                
#else /* EL2014 */
/* << 9.03 */
	if(gcLg != 'A') /* French or Dutch */
	{
/* 9.03 >> */
		setfillstyle(SOLID_FILL, WHITE);
		bar(0,0,639,479);
		setcolor ( BLACK );
		settextstyle( SMALL_FONT, HORIZ_DIR, 10 );
		outtextxy(320,80, pszMsg[38]);
		outtextxy(321,80, pszMsg[38]);
		outtextxy(320,81, pszMsg[38]);
		settextstyle( SMALL_FONT, HORIZ_DIR, 7 );

		outtextxy(320,150, pszMsg[39]);
		outtextxy(321,150, pszMsg[39]);
		outtextxy(320,151, pszMsg[39]);

		outtextxy(320,200, pszMsg[40]);
		outtextxy(321,200, pszMsg[40]);
		outtextxy(320,201, pszMsg[40]);

		outtextxy(320,250, pszMsg[108]);
		outtextxy(321,250, pszMsg[108]);
		outtextxy(320,251, pszMsg[108]);

		outtextxy(320,300, pszMsg[109]);
		outtextxy(321,300, pszMsg[109]);
		outtextxy(320,301, pszMsg[109]);

		outtextxy(320,350, pszMsg[110]);
		outtextxy(321,350, pszMsg[110]);
		outtextxy(320,351, pszMsg[110]);
/* << 9.03 */
	}
	else /* German */
	{
		setfillstyle(SOLID_FILL, WHITE);
		bar(0,0,639,479);
		setcolor ( BLACK );
		settextstyle( SMALL_FONT, HORIZ_DIR, 8 );
		outtextxy(320,80, pszMsg[0]);
		outtextxy(321,80, pszMsg[0]);
		outtextxy(320,81, pszMsg[0]);

		outtextxy(320,150, pszMsg[1]);
		outtextxy(321,150, pszMsg[1]);
		outtextxy(320,151, pszMsg[1]);

		outtextxy(320,180, pszMsg[2]);
		outtextxy(321,180, pszMsg[2]);
		outtextxy(320,181, pszMsg[2]);

		outtextxy(320,230, pszMsg[3]);
		outtextxy(321,230, pszMsg[3]);
		outtextxy(320,231, pszMsg[3]);

		outtextxy(320,260, pszMsg[4]);
		outtextxy(321,260, pszMsg[4]);
		outtextxy(320,261, pszMsg[4]);

		outtextxy(320,290, pszMsg[5]);
		outtextxy(321,290, pszMsg[5]);
		outtextxy(320,291, pszMsg[5]);

		outtextxy(320,340, pszMsg[6]);
		outtextxy(321,340, pszMsg[6]);
		outtextxy(320,341, pszMsg[6]);

		outtextxy(320,370, pszMsg[7]);
		outtextxy(321,370, pszMsg[7]);
		outtextxy(320,371, pszMsg[7]);
	}
/* 9.03 >> */
#endif /* EL2014 */    
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Technical_Error </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Technical_Error(void)
{
  setfillstyle(SOLID_FILL, WHITE);
  bar(0,0,639,479);
  setcolor(BLACK);
  settextstyle(SMALL_FONT,HORIZ_DIR, 10);
  settextjustify(CENTER_TEXT, TOP_TEXT);

  outtextxy(320,200, pszMsg[53]);
  outtextxy(321,200, pszMsg[53]);
  outtextxy(320,201, pszMsg[53]);

  settextjustify(CENTER_TEXT, BOTTOM_TEXT);
  settextstyle(SMALL_FONT, HORIZ_DIR, 7);
  outtextxy(320,300, pszMsg[44]);
  outtextxy(321,300, pszMsg[44]);
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Eject_Card </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Eject_Card(void)
{
  setfillstyle(SOLID_FILL, WHITE);
  bar(0,0,639,479);
  setcolor(BLACK);
  settextstyle(SMALL_FONT,HORIZ_DIR,10);
  settextjustify(CENTER_TEXT, CENTER_TEXT);

  outtextxy(320, 240, pszMsg[37]);
  outtextxy(321, 240, pszMsg[37]);
  outtextxy(320, 241, pszMsg[37]);
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Bad_Card </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Bad_Card(void)
{
  setfillstyle(SOLID_FILL, WHITE);
  bar(0,0,639,479);
  setcolor(BLACK);
  settextstyle( SMALL_FONT,HORIZ_DIR, 8);
  settextjustify(CENTER_TEXT, CENTER_TEXT);

  outtextxy(320,150, pszMsg[34]);
  outtextxy(321,150, pszMsg[34]);
  outtextxy(320,151, pszMsg[34]);

  outtextxy(320,250, pszMsg[35]);
  outtextxy(321,250, pszMsg[35]);
  outtextxy(320,251, pszMsg[35]);

  outtextxy(320,350, pszMsg[36]);
  outtextxy(321,350, pszMsg[36]);
  outtextxy(320,351, pszMsg[36]);
}

/* << 9.01b */
/**************************************************************************
<FUNCTION>
  <NAME> Screen_Bad_Card </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Bad_Card_Blocked(void)
{
  setfillstyle(SOLID_FILL, WHITE);
  bar(0,0,639,479);
  setcolor(BLACK);
  settextstyle( SMALL_FONT,HORIZ_DIR, 8);
  settextjustify(CENTER_TEXT, CENTER_TEXT);

  outtextxy(320,200, pszMsg[17]);
  outtextxy(321,200, pszMsg[17]);
  outtextxy(320,201, pszMsg[17]);

  outtextxy(320,300, pszMsg[20]);
  outtextxy(321,300, pszMsg[20]);
  outtextxy(320,301, pszMsg[20]);
}
/* 9.01b >> */

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Incompatible_Card </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Incompatible_Card(void)
{
  setfillstyle(SOLID_FILL, WHITE);
  bar(0,0,639,479);
  setcolor(BLACK);
  settextstyle(SMALL_FONT, HORIZ_DIR,10);
  settextjustify(CENTER_TEXT, BOTTOM_TEXT);

  outtextxy(320, 180, pszMsg[41]);
  outtextxy(321, 180, pszMsg[41]);
  outtextxy(320, 181, pszMsg[41]);

  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtextxy(320, 230, pszMsg[42]);
  outtextxy(321, 230, pszMsg[42]);
  outtextxy(320, 231, pszMsg[42]);

  settextjustify(CENTER_TEXT, TOP_TEXT);
  outtextxy(320, 280, pszMsg[43]);
  outtextxy(321, 280, pszMsg[43]);
  outtextxy(320, 281, pszMsg[43]);

  settextjustify(CENTER_TEXT, BOTTOM_TEXT);
  settextstyle(SMALL_FONT, HORIZ_DIR, 7);

  outtextxy(320,400, pszMsg[44]);
  outtextxy(320,401, pszMsg[44]);
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Cand_Actions_Allowed </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  (Bottom of the "selection of a candidate" screen)
  "Going Back" or "Cancel / Validate"
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Cand_Actions_Allowed(int _iType)
{
  if(_iType == 0) /* Back to Lists */
  {
      setfillstyle(SOLID_FILL, BLACK);
      bar(0,447,639,479);
      setfillstyle(SOLID_FILL, WHITE);
/* << 9.03 */
      /*bararrondi(110,452,530,479);*/
      if(gcLg != 'A') /* Not german */
        bararrondi(110,452,530,479);
      else
        bararrondi(100,452,540,479);
/* 9.03 >> */
      setcolor(BLACK);
      settextjustify(CENTER_TEXT, CENTER_TEXT);
      settextstyle( SMALL_FONT, HORIZ_DIR, 5);
      outtextxy(320,464, pszMsg[48]);
  }
  else /* Cancel <> Validate */
  {
      setfillstyle(SOLID_FILL, BLACK);
      bar(0,447,639,479);
      setfillstyle(SOLID_FILL,WHITE);
      setcolor(BLACK);
      bararrondi(10,452,310,479);
      bararrondi(330,452,630,479);
      settextjustify(CENTER_TEXT, CENTER_TEXT);
      settextstyle( SMALL_FONT, HORIZ_DIR, 5);
#ifdef EL2014  
      outtextxy(160,462, pszMsg[49]);
      outtextxy(480,462, pszMsg[50]);
      if(strstr(pszMsg[50],"bevestigen"))
        outtextxy(573,462, "bevestigen"); 
      else if(strstr(pszMsg[50],"confirmer"))
        outtextxy(501,462, "confirmer");
      else if(strstr(pszMsg[50],"best„tigen"))
        outtextxy(558,462, "best„tigen");
		
#else      
      outtextxy(160,462, pszMsg[50]);
      outtextxy(161,462, pszMsg[50]);
      outtextxy(480,462, pszMsg[49]);
      outtextxy(481,462, pszMsg[49]);
#endif      
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Start_Verif </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Start_Verif(void)
{
  setcolor(WHITE);
  settextstyle( SMALL_FONT, HORIZ_DIR, 12);
  settextjustify ( CENTER_TEXT , CENTER_TEXT ) ;
  outtextxy(320,240,pszMsg[129]);
  outtextxy(321,240,pszMsg[129]);
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Viewing_Next </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Viewing_Next(void)
{
  setfillstyle(SOLID_FILL, WHITE);
  bararrondi(110,452,530,479);
  setcolor(BLACK);
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  settextstyle( SMALL_FONT, HORIZ_DIR, 7);
  outtextxy(320,464, pszMsg[92]);
  outtextxy(321,464, pszMsg[92]);
}

/**************************************************************************
<FUNCTION>
	<NAME> Screen_Lg_Select </NAME>
	<TYPE> GLOBAL </TYPE>
	<DESCRIPTION>
    </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Lg_Select(void)
{
  setfillstyle(SOLID_FILL, BLACK);
  bar(0,0,639,479);
  setcolor(WHITE);
  setfillstyle(SOLID_FILL, WHITE);
/* << 9.04 */
  /*bar(50,80,590,300);*/
    bar(40,80,600,300);
/* 9.04 >> */
  setcolor (BLACK);

  rectangle(108,198,282,292);
  rectangle(109,199,281,291);

  rectangle(358,198,532,292);
  rectangle(359,199,531,291);

  settextstyle( SMALL_FONT, HORIZ_DIR, 6);
  settextjustify ( CENTER_TEXT , TOP_TEXT ) ;
  outtextxy(195,240,pszMsg1[45]);
  outtextxy(196,240,pszMsg1[45]);
  outtextxy(445,240,pszMsg2[45]);
  outtextxy(446,240,pszMsg2[45]);
  outtextxy( 320, 110, pszMsg1[52]);
  outtextxy( 321, 110, pszMsg1[52]);

  outtextxy( 320, 140, pszMsg2[52]);
  outtextxy( 321, 140, pszMsg2[52]);
}


/**************************************************************************
<FUNCTION>
  <NAME> Screen_Lg_Confirm </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Lg_Confirm(void)
{
  setfillstyle(SOLID_FILL, BLACK);
  bar(0,0,639,479);
  setcolor(WHITE);
  setfillstyle(SOLID_FILL, WHITE);
  bar(50,80,590,300);
  setcolor(BLACK);

  rectangle(108,198,282,292);
  rectangle(109,199,281,291);

  rectangle(358,198,532,292);
  rectangle(359,199,531,291);

  settextstyle( SMALL_FONT, HORIZ_DIR, 6);
  settextjustify ( CENTER_TEXT , TOP_TEXT ) ;

#ifdef EL2014
  /* Cancel */
  outtextxy(195,230,pszMsg[75]);
  outtextxy(196,230,pszMsg[75]);
  if(pszMsg == pszMsg1)
  {
    outtextxy(195,250,pszMsg2[75]);
    outtextxy(196,250,pszMsg2[75]);
  }
  else
  {
    outtextxy(195,250,pszMsg1[75]);
    outtextxy(196,250,pszMsg1[75]);
  }
  
  /* Confirmation */
  outtextxy(445,230,pszMsg[74]);
  outtextxy(446,230,pszMsg[74]);
  if(pszMsg == pszMsg1)
  {
    outtextxy(445,250,pszMsg2[74]);
    outtextxy(446,250,pszMsg2[74]);
  }
  else
  {
    outtextxy(445,250,pszMsg1[74]);
    outtextxy(446,250,pszMsg1[74]);
  }
#else
  /* Confirmation */
  outtextxy(195,230,pszMsg[74]);
  outtextxy(196,230,pszMsg[74]);
  if(pszMsg == pszMsg1)
  {
    outtextxy(195,250,pszMsg2[74]);
    outtextxy(196,250,pszMsg2[74]);
  }
  else
  {
    outtextxy(195,250,pszMsg1[74]);
    outtextxy(196,250,pszMsg1[74]);
  }

  /* Cancel */
  outtextxy(445,230,pszMsg[75]);
  outtextxy(446,230,pszMsg[75]);
  if(pszMsg == pszMsg1)
  {
    outtextxy(445,250,pszMsg2[75]);
    outtextxy(446,250,pszMsg2[75]);
  }
  else
  {
    outtextxy(445,250,pszMsg1[75]);
    outtextxy(446,250,pszMsg1[75]);
  }
#endif  

  outtextxy( 320, 120, pszMsg[65]);
  outtextxy( 321, 120, pszMsg[65]);

  switch(gcCodeLgOri)
  {
    case '5': /* French - German */
#ifdef EL2014
      outtextxy( 320, 140, pszMsg[93]);
      outtextxy( 321, 140, pszMsg[93]);
#else
      outtextxy( 320, 140, pszMsg[76]);
      outtextxy( 321, 140, pszMsg[76]);
#endif        
      break;

    case '6': /* French - Dutch */
#ifdef EL2014
      outtextxy( 320, 140, pszMsg[76]);
      outtextxy( 321, 140, pszMsg[76]);
#else
      outtextxy( 320, 140, pszMsg[29]);
      outtextxy( 321, 140, pszMsg[29]);
#endif        
      break;

    case '8': /* German - French */
#ifdef EL2014
      outtextxy( 320, 140, pszMsg[93]);
      outtextxy( 321, 140, pszMsg[93]);
#else
/* << 9.01c */
/*
      outtextxy( 320, 140, "pszMsg[??]");
      outtextxy( 321, 140, "pszMsg[??]");
*/
      outtextxy( 320, 140, pszMsg[76]);
      outtextxy( 321, 140, pszMsg[76]);
/* 9.01c >> */
#endif        
      break;
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Vote_Empty_Confirmation </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Vote_Empty_Confirmation(void)
{
  int iPenX, iPenY;

  setfillstyle(SOLID_FILL, BLACK);
  bar(0,30,639,479);
  setfillstyle(SOLID_FILL, WHITE);
  bar(20,80,620,300);
  setcolor (BLACK);
  rectangle(38,198,302,292);
  rectangle(39,199,301,291);
  rectangle(338,198,602,292);
  rectangle(339,199,601,291);
  settextstyle( SMALL_FONT, HORIZ_DIR, 6);
  settextjustify ( CENTER_TEXT , TOP_TEXT ) ;

/* << 9.03 */
/*
/ << 9.01c /
  /if(gcLg == 'F')/
  if(gcLg == 'F' || gcCodeLgOri == '8') / French or German /
/ 9.01c >> /
  {
    outtextxy(470,220, pszMsg[59]);
    outtextxy(470,260, pszMsg[58]);

    outtextxy(170,220, pszMsg[57]);
    if(strstr(pszMsg[57],"confirmer")) outtextxy(252,220,"confirmer");
/ << 9.01c /
    if(strstr(pszMsg[57],"Best„tigen")) outtextxy(191,220, "Best„tigen");
/ 9.01c >> /
    outtextxy(170,260, pszMsg[58]);
  }
  else
  {
    if(gcCodeLgOri == '6') / French - Dutch /
    {
      outtextxy(470,220, pszMsg[57]);
      outtextxy(470,260, pszMsg[59]);

      outtextxy(170,220, pszMsg[57]);
      outtextxy(170,260, pszMsg[58]);
      if(strstr(pszMsg[58],"bevestigen")) outtextxy(209,260, "bevestigen");
    }
/ << 9.01c
    else / French - German /
    {
      outtextxy(470,220, pszMsg[59]);
      outtextxy(470,260, pszMsg[58]);

      outtextxy(170,220, pszMsg[57]);
      outtextxy(170,260, pszMsg[58]);
      if(strstr(pszMsg[58],"bevestigen")) outtextxy(205,260, "bevestigen");
    }
/ 9.01c >> /
  }
*/
	switch(gcLg)
	{
		case 'A':
			outtextxy(470,210, pszMsg[22]);
			outtextxy(470,235, pszMsg[23]);
			outtextxy(470,260, pszMsg[25]);

			outtextxy(170,210, pszMsg[22]);
			outtextxy(170,235, pszMsg[23]);
			outtextxy(170,260, pszMsg[24]);
			if(strstr(pszMsg[24],"best„tigen")) outtextxy(184,260, "best„tigen");
			break;
		case 'F':
			outtextxy(470,220, pszMsg[59]);
			outtextxy(470,260, pszMsg[58]);

			outtextxy(170,220, pszMsg[57]);
			outtextxy(170,260, pszMsg[58]);
			if(strstr(pszMsg[57],"confirmer")) outtextxy(252,220,"confirmer");
			break;
		case 'N':
			outtextxy(470,220, pszMsg[57]);
			outtextxy(470,260, pszMsg[59]);

			outtextxy(170,220, pszMsg[57]);
			outtextxy(170,260, pszMsg[58]);
			/* << el2012 */
			/*if(strstr(pszMsg[58],"bevestigen")) outtextxy(205,260, "bevestigen");*/
			if(strstr(pszMsg[58],"bevestigen")) outtextxy(209,260, "bevestigen");
			/* el2012 >> */
			break;
	}
/* 9.03 >> */
  settextstyle( SMALL_FONT, HORIZ_DIR, 7);
  outtextxy( 320, 110, pszMsg[55]);
  outtextxy( 320, 140, pszMsg[56]);
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_View_Vote_Empty </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_View_Vote_Empty(void)
{
  int iMsgId;

  setfillstyle(SOLID_FILL, BLACK);
  bar(0,0,639,479);
  setcolor(WHITE);
  settextstyle( SMALL_FONT, HORIZ_DIR, 7);
  settextjustify( CENTER_TEXT, CENTER_TEXT );
  /* Looking for the message id corresponding to the current election */
  iMsgId = Msg_Election();
  outtextxy(320, 10, pszMsg[iMsgId]);
  outtextxy(321, 10, pszMsg[iMsgId]);
  setfillstyle(SOLID_FILL, WHITE);
/* << 9.03 */
  /*bar(20,80,620,300);*/
  if(gcLg != 'A') /* French or Dutch */
    bar(20,80,620,300);
  else
    bar(5,80,634,300);
/* 9.03 >> */
  setcolor (BLACK);
  settextstyle( SMALL_FONT, HORIZ_DIR, 9);
  settextjustify ( CENTER_TEXT , CENTER_TEXT) ;
  outtextxy(320,190, pszMsg[55]);
  outtextxy(320,191, pszMsg[55]);
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_LigthPen </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_LigthPen(unsigned char _uc)
{
  setfillstyle(SOLID_FILL, WHITE);
  bar(0,0,639,479);
  setcolor(BLACK);
  settextstyle(SMALL_FONT,HORIZ_DIR, 10);
  settextjustify(CENTER_TEXT, TOP_TEXT);

  if(!_uc)
  {
    outtextxy(320,200, pszMsg[26]);
    outtextxy(321,200, pszMsg[26]);
    outtextxy(320,201, pszMsg[26]);
    settextjustify(CENTER_TEXT, BOTTOM_TEXT);
    settextstyle(SMALL_FONT, HORIZ_DIR, 7);
    outtextxy(320,300, pszMsg[44]);
    outtextxy(321,300, pszMsg[44]);
  }
  else
  {
    outtextxy(320,200, pszMsg[27]);
    outtextxy(321,200, pszMsg[27]);
    outtextxy(320,201, pszMsg[27]);

    settextjustify(CENTER_TEXT, BOTTOM_TEXT);
    settextstyle(SMALL_FONT, HORIZ_DIR, 7);
    outtextxy(320,300, pszMsg[133]);
    outtextxy(321,300, pszMsg[133]);
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Screen_Vote_End </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Screen_Vote_End(void)
{
  setfillstyle(SOLID_FILL, WHITE);
  bar(0,0,639,479);
  setcolor(BLACK);
/* << 9.03 */
  /*settextstyle( SMALL_FONT, HORIZ_DIR, 12);*/
  if(gcLg != 'A') /* French or Dutch */
    settextstyle( SMALL_FONT, HORIZ_DIR, 12);
  else /* German */
    settextstyle( SMALL_FONT, HORIZ_DIR, 8);
/* 9.03 >> */
  settextjustify ( CENTER_TEXT , CENTER_TEXT ) ;
  outtextxy(320,240,pszMsg[38]);
  outtextxy(321,240,pszMsg[38]);
}
/* << 9.03
extern void Wait_Loop_Int(void);

void Whole_Other_Screens(void)
{
  Screen_Technical_Error();
  Wait_Loop_Int();
  Screen_LigthPen(0);
  Wait_Loop_Int();
  Screen_LigthPen(1);
  Wait_Loop_Int();
  Screen_Wait();
  Wait_Loop_Int();
  Screen_Bad_Crc();
  Wait_Loop_Int();
  /Screen_Pwd();
  Wait_Loop_Int();/
  Screen_Eject_Card();
  Wait_Loop_Int();
  Screen_Bad_Card_Blocked();
  Wait_Loop_Int();
  Screen_Incompatible_Card();
  Wait_Loop_Int();
}
 9.03 >> */
