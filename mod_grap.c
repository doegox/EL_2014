/**************************************************************************
<MODULE NAME>
<FILENAME> mod_grap.c (graphic) </FILENAME>
<HISTORY>
  <MODIFICATION>
    <DATE> </DATE>
    <AUTHOR>  </AUTHOR>
    <DESCRIPTION>
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
#include "defs.h"
#include "..\CrdReade.r\LibCrdRd.h"
#include "..\LibAlrBx\LIBALRBX.H"

/**************************************************************************
EXTERNAL VARIABLES
**************************************************************************/
extern int    swLPcursor;
extern unsigned char gucMavType;

/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/
#if (LPMNGLIB >= 1)
extern void lpCursorOn(void);
extern void lpCursorOff(void);
extern void lpPosxy(int, int);
#endif
extern void  Leds(char, int);
extern void  Screen_AlarmBox(unsigned char);
extern void  Screen_LigthPen(unsigned char);
extern unsigned char Test_AlarmBox(void);
extern unsigned char Test_LightPen(void);

/**************************************************************************
GLOBAL VARIABLES
**************************************************************************/
/*
<VARIABLE>
  <NAME> giKlavier </NAME>
  <TYPE> int </TYPE>
  <DESCRIPTION>
  Button Status (to specify if the button (interrupt) has been pressed)
  </DESCRIPTION>
</VARIABLE>
*/
int giKlavier;

/**************************************************************************
LOCAL VARIABLES
**************************************************************************/
static int swLPcursor  = 0;
static int GraphDriver = 9;  /* The Graphics device driver */
static int GraphMode   = 2;  /* The Graphics mode value    */

/**************************************************************************
<FUNCTION>
  <NAME> Graph_Init </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Graphical initialization (to link with GRAPHICS libraries)
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Graph_Init(void)
{
  int errorcode;

  registerbgidriver(EGAVGA_driver);
  registerbgifont(small_font);
  registerbgifont(sansserif_font);

  initgraph( &GraphDriver, &GraphMode, "" );
  /* read result of initialization */
  errorcode = graphresult();

  if(errorcode != grOk)  /* an error occurred */
  {
     printf("Graphics error: %s\n", grapherrormsg(errorcode));
     printf("Press any key to halt:");
     getch();
     exit(1); /* return with error code */
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> bararrondi </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>                  _______
  Draw a bar with semi-circular (_______)
  </DESCRIPTION>
  <PARAMETERS>
    <PARAMETER>
      <NAME> x1, y1, x2, y2 </NAME>
      <TYPE> int </TYPE>
      <IO> IN </IO>
      <DESCRIPTION>
      Form's coordinates
      </DESCRIPTION>
    </PARAMETER>
  </PARAMETERS>
</FUNCTION>
**************************************************************************/
void bararrondi( int x1, int y1, int x2, int y2 )
{
  int rayon , cy ;

  rayon = ( y2 - y1 ) / 2;
  cy = y1 + rayon ;
  setcolor ( WHITE ) ;
  pieslice ( x1+rayon , cy , 90 , 270 , rayon ) ;
  pieslice ( x2-rayon , cy , 270 , 360 , rayon ) ;
  pieslice ( x2-rayon , cy , 0 , 90 , rayon ) ;
  bar ( x1+rayon , y1 , x2-rayon, y2 ) ;

  setcolor( BLACK ) ;
  arc ( x1+rayon , cy , 90 , 270 , rayon ) ;
  arc ( x2-rayon , cy , 270 , 360 , rayon ) ;
  arc ( x2-rayon , cy , 0 , 90 , rayon ) ;
  line ( x1+rayon , y1 , x2-rayon , y1 ) ;
  line ( x1+rayon , y2 , x2-rayon , y2 ) ;
}

/**************************************************************************
<FUNCTION>
  <NAME> Pen_Capture </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Does the user use the light pen ?
  Yes: gets giCoordX and giCoordY

  Checks if the button has been pressed (to cancel the vote)
  </DESCRIPTION>
  <RETURNS>
    <TYPE> int </TYPE>
    <DESCRIPTION>
    0  -> nothing happened
    1  -> light pen used
    -1 -> light pen disconnected / connected
    </DESCRIPTION>
  </RETURNS>

</FUNCTION>
**************************************************************************/
int Pen_Capture(int *_iX, int *_iY)
{
  union REGS rin,rout;
  int rc = 0;
  static int stcSwClicked = 1;

  /* Light Pen */
  if(!Test_LightPen())
  {
    Screen_LigthPen(0);
    /* switch on the red and green leds + bips */
    Leds(RG,1);
    while(!Test_LightPen());
    Leds(OFF,0);
    return(-1);
  }

  if(gucMavType == MAV_JITES_1 || gucMavType == MAV_JITES_2)
  {
    rin.x.ax = 5;
    rin.x.bx = 0;
    int86(0x33,&rin,&rout);

    if(rout.x.bx)
    {
      rin.h.ah = 4;
      int86(0x10,&rin,&rout);
      if(rout.h.ah)
      {
        (*_iX) = rout.x.bx;
        (*_iY) = rout.x.cx;
        rc = 1;
      }

      CheckQueue();
    }
  }
  else /* Digivote */
  {
    /* Light Pen used ? */
    rin.x.ax = 3;
    int86(0x33,&rin,&rout);
    if(!(rout.x.bx&7)) /* No "button" pressed (cfr Mouse management) */
    {
      stcSwClicked = 1;
    }
    else
    {
      if(stcSwClicked)
      {
        stcSwClicked = 0;

        rin.h.ah = 4;
        int86(0x10,&rin,&rout);
        if(rout.h.ah)
        {
          (*_iX) = rout.x.bx;
          (*_iY) = rout.x.cx;
          rc = 1;
        }

        CheckQueue();
      }
    }
  }

  giKlavier = ButtonPressed(0);

  if(rc) return(1);

  return(0);
}

/**************************************************************************
<FUNCTION>
  <NAME> setLPcursor </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  To specify if the light pen is managed or not
  </DESCRIPTION>
  <PARAMETERS>
    <PARAMETER>
      <NAME> _b </NAME>
      <TYPE> bool </TYPE>
      <IO> IN </ IO >
      <DESCRIPTION>
      Used or not (1, 0)
      </DESCRIPTION>
    </PARAMETER>
  </PARAMETERS>
</FUNCTION>
**************************************************************************/
void setLPcursor(bool _b)
{
  swLPcursor = _b;
}

/**************************************************************************
<FUNCTION>
  <NAME> Set_Cursor </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>

  </DESCRIPTION>
  <PARAMETERS>
    <PARAMETER>
      <NAME> _iAction </NAME>
      <TYPE> int </TYPE>
      <IO> IN </ IO >
      <DESCRIPTION>
        CURSOR_OFF -> lpCursorOff() -> cursor hidden
        CURSOR_ON  -> lpCursorOn()  -> cursor shown
        CURSOR_XY  -> lpPosxy() -> set the cursor at the position (639,479)
                    (To avoid to point on a specific area (a list, a candidate))
      </DESCRIPTION>
    </PARAMETER>
  </PARAMETERS>
</FUNCTION>
**************************************************************************/
void Set_Cursor(int _iAction)
{
#if (LPMNGLIB >=1)
  if(!swLPcursor) return;

  switch(_iAction)
  {
    case CURSOR_OFF:
         lpCursorOff();
         break;
    case CURSOR_ON:
         lpCursorOn();
         break;
    case CURSOR_XY:
         setfillstyle(SOLID_FILL, WHITE);
         bar(640,480,640,480);
         lpPosxy(640,480);
         break;
  }
#endif
}
