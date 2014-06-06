/*
#define EL2014
*/

/**************************************************************************
<MODULE NAME>
<FILENAME> mod_lg.c (language) </FILENAME>
<HISTORY>
  <MODIFICATION>
    <DATE> </DATE>
    <AUTHOR>  </AUTHOR>
    <DESCRIPTION>
    Language initialization, selection, confirmation
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

#include "defs.h"

/**************************************************************************
EXTERNAL VARIABLES
**************************************************************************/
extern int    giCodeVoteEnd;
extern char **pszMsg;
extern char  *pszMsg1[N_SCR_MESSAGE];
extern char  *pszMsg2[N_SCR_MESSAGE];

/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/
extern void Msg_Error_Exit(char *);
extern void Msg_Fill(void);
extern int  Pen_Capture(int *, int *);
extern void Screen_Lg_Select(void);
extern void Screen_Lg_Confirm(void);
extern void Set_Cursor(int);

/**************************************************************************
GLOBAL VARIABLES
**************************************************************************/
/*
<VARIABLE>
  <NAME> gcCodeLgOri </NAME>
  <TYPE> char </TYPE>
  <DESCRIPTION>
  Specifies the language(s) managed
  '3': German
  '5': Choose between French or German
  '6': Choose between French or Dutch
  '7': French
  '8': Choose between German or French
  </DESCRIPTION>
</VARIABLE>
*/
char gcCodeLgOri;
/*
<VARIABLE>
  <NAME> gcCodeLg </NAME>
  <TYPE> char </TYPE>
  <DESCRIPTION>
  Specifies the language currently used
  (specifically done to manage the case of
   gcCodeLgOri = '1' or gcCodeLgOri = '2')
  </DESCRIPTION>
</VARIABLE>
*/
char gcCodeLg;
/*
<VARIABLE>
  <NAME> gcLg </NAME>
  <TYPE> char </TYPE>
  <DESCRIPTION>
  'F'  French
  'N' (Other Language)
  </DESCRIPTION>
</VARIABLE>
*/
char gcLg;

/**************************************************************************
<FUNCTION>
  <NAME> Lg_Init </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Read machvots.tbl and initialize gcCodeLgOri, gcCodeLg, gcLg
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Lg_Init(void)
{
  FILE *fps;
  char bufs[32];

  fps = fopen("machvots.Tbl","r");
  if(fps == NULL) Msg_Error_Exit("Machvots.tbl FILE ERROR");

  fread(bufs,16,1,fps);
  fclose(fps);

  /* Init Code Language*/
  gcCodeLgOri = gcCodeLg = bufs[15];

  switch(gcCodeLg)
  {
    case '3': /* German */
         gcLg = 'A';
         break;

    case '5': /* Choose between French or German */
         gcLg = 'F';
         break;

    case '6':  /* Choose between French or Dutch */
         gcLg = 'F';
         break;

    case '7': /* French */
         gcLg = 'F';
         break;

    case '8': /* Choose between German or French */
         gcLg = 'A';
         break;

    default:
         gcLg = 'F';
  }

  Msg_Fill();
  pszMsg = pszMsg1;
}

/**************************************************************************
<FUNCTION>
  <NAME> Lg_Change </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  To change the current language used (for example from Dutch to French)
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
int Lg_Change(void)
{
  switch(gcCodeLg)
  {
    case '1':
         gcLg = 'N';
         gcCodeLg = '2';
         break;
    case '2':
         gcLg = 'F';
         gcCodeLg = '1';
         break;
    default:
         return(0);
  }

  Msg_Fill();
  pszMsg = pszMsg1;

  return(1);
}

/**************************************************************************
<FUNCTION>
  <NAME> Lg_Select </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Ask which language to use (gcCodeLgOri = 4, 5 or 6)
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
unsigned char Lg_Select(void)
{
  int iPenX, iPenY, iPenCapture;

  /* Display screen */
  Screen_Lg_Select();

  Set_Cursor(CURSOR_XY);

  while(1)
  {
    Set_Cursor(CURSOR_ON);

    iPenCapture = Pen_Capture(&iPenX, &iPenY);
    if(iPenCapture == 1 && ((iPenY > 200) && (iPenY < 290)))
    {
      Set_Cursor(CURSOR_OFF);
      if((iPenX > 360) && (iPenX < 530))
      {
#ifdef EL2014
        if      (gcCodeLgOri == '5')	gcLg = 'A';
		else if (gcCodeLgOri == '6')	gcLg = 'N';
		else if (gcCodeLgOri == '8')	gcLg = 'F';
		else							gcLg = 'F';
#else
/* << 9.03 */
        /*gcLg = 'N';*/
        if(gcCodeLgOri != '8')
          gcLg = 'N';
        else
          gcLg = 'F';
/* 9.03 >> */
#endif  
        pszMsg = pszMsg2;
        return 1;
      }
      else
      {
        if((iPenX > 110) && (iPenX <  280))
        {
/* << 9.03 */
#ifdef EL2014
        if      (gcCodeLgOri == '5')	gcLg = 'F';
		else if (gcCodeLgOri == '6')	gcLg = 'F';
		else if (gcCodeLgOri == '8')	gcLg = 'A';
		else							gcLg = 'F';
#else
        /*gcLg = 'F';*/
        if(gcCodeLgOri != '8')
          gcLg = 'F';
        else
          gcLg = 'A';
#endif  
/* 9.03 >> */
          pszMsg = pszMsg1;
          return 1;
        }
      }
    }
    else
    {
      if(iPenCapture < 0)
        return iPenCapture;
    }
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Lg_Confirm </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Asks a confirmation (after a selection)
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
int Lg_Confirm(void)
{
  int iPenX, iPenY, iPenCapture;

  /* Display screen */
  Screen_Lg_Confirm();

  Set_Cursor(CURSOR_XY);
  while(1)
  {
    Set_Cursor(CURSOR_ON);
    iPenCapture = Pen_Capture(&iPenX, &iPenY);
    if(iPenCapture == 1)
    {
      Set_Cursor(CURSOR_OFF);
      if((iPenY > 200) && (iPenY < 290))
      {
#ifdef EL2014
        if((iPenX > 110) && (iPenX <  280)) return(0);
        else
          if((iPenX >  360) && (iPenX < 530)) return(1);
#else
        if((iPenX > 110) && (iPenX <  280)) return(1);
        else
          if((iPenX >  360) && (iPenX < 530)) return(0);
#endif          
      }
    }
    else
    {
      if(iPenCapture < 0)
        return iPenCapture;
    }
  }
}
