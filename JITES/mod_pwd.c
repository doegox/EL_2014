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
<FILENAME> mod_pwd.c (password) </FILENAME>
<HISTORY>
  <MODIFICATION>
    <DATE> </DATE>
    <AUTHOR>  </AUTHOR>
    <DESCRIPTION>
    Password - catching screen - validation - Rijndael init.
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
#include "fullspec.h"
//#include "libcrdrd.h"
//#include "libalrbx.h"

#ifdef EL2014
#include "variable.h"
#endif

/**************************************************************************
EXTERNAL VARIABLES
**************************************************************************/
extern int gaiPosv[10][4];
extern int giCodeVoteEnd;
extern unsigned char gucMavType;

/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/
extern int  Aff_Clavier(void);
extern int  Initialise_Rijndael(void);
extern void Initialise_RijndaelMini(void);
extern int  Pen_Capture(int *, int *);
extern int (*ptfunct)(void);
extern void Msg_Error_Exit(char *);
extern void Screen_Pwd(void);
extern void Set_Cursor(int);
extern int  Verifie_Password(void);
extern int  Vote_End(void);

/**************************************************************************
GLOBAL VARIABLES
**************************************************************************/
char fullPassword[fullPasswordLength + 1];
char gszMinipassword[12];

/**************************************************************************
LOCAL VARIABLES
**************************************************************************/
static char Minicodage[5];

/**************************************************************************
<FUNCTION>
  <NAME> Pwd_Read </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
int Pwd_Read(void)
{
 char bfs[2];
 int i, x;
 int iPenX, iPenY, iPenCapture, iHwdDisconnected;

 /* Reset the pwd output area */
 setfillstyle(SOLID_FILL,BLACK);
 bar(0,95,639,120);

 settextjustify(LEFT_TEXT,LEFT_TEXT);
 settextstyle(SMALL_FONT, HORIZ_DIR, 7);
 setcolor(WHITE);
 strcpy(fullPassword,"");

 Set_Cursor(CURSOR_ON);

 for(x = 0;x < 10;)
 {
    iHwdDisconnected = iPenCapture = Pen_Capture(&iPenX, &iPenY);
    if(iPenCapture == 1)
    {
      for(i = 0; i < 11;i++)
      {
       if((iPenX > gaiPosv[i][0] + 2) && (iPenX < gaiPosv[i][1] - 2))
       {
         if ((iPenY > gaiPosv[i][2] + 2) && (iPenY < gaiPosv[i][3] - 2))
         {
           if(i == 10)
           {
             strcpy(fullPassword,"");
             x = 0;
           }
           else
           {
             if(i == 9) sprintf(bfs, "%d", 0);
             else       sprintf(bfs, "%d", i+1);
             bfs[1] = 0x00;
             strcat(fullPassword,bfs);
             x++;
           }
         }
         setfillstyle(SOLID_FILL,BLACK);
         bar(0,95,639,120);
         outtextxy(265,110, fullPassword);
       }
      }
    }
    PEN_OR_ALARMBOX_DISCONNECTED
 }

 Set_Cursor(CURSOR_OFF);

 /* Black Screen */
 setfillstyle(SOLID_FILL,BLACK);
 bar(0,0,639,479);

 return(1);
}

/**************************************************************************
<FUNCTION>
  <NAME> Pwd </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Asking for the password + validation + initialization of Rijndael
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Pwd(void)
{
#ifdef EL2014
  strcpy(Minicodage,MINI_PWD);
#else
  strcpy(Minicodage,"6870");
#endif    
  

  /* Ask Password */
  strcpy(fullPassword,"1111111111");
  while(!Verifie_Password())
  {
#ifdef TST_EL2014
    strcpy(fullPassword,TST_PASSWORD);
#else
    Screen_Pwd();
    Pwd_Read();
#endif  
  }

  /*  Create new password  */
#ifdef EL2014
  gszMinipassword[0] = fullPassword[MINI_POS[0]-49];
  gszMinipassword[1] = fullPassword[MINI_POS[1]-49];
  gszMinipassword[2] = fullPassword[MINI_POS[2]-49];
  gszMinipassword[3] = fullPassword[MINI_POS[3]-49];
#else
  gszMinipassword[0] = fullPassword[0];
  gszMinipassword[1] = fullPassword[1];
  gszMinipassword[2] = fullPassword[3];
  gszMinipassword[3] = fullPassword[7];
#endif  
  gszMinipassword[4] = 0x00;
  strcat(gszMinipassword,Minicodage);
#ifdef EL2014
  sprintf(my_msg, "Pwd() - [%s]\n", gszMinipassword); 
  my_Trace();
#endif  
  

  /* looking for a:Floppy.be */
  if(access("FLOPPY/Floppy.be", 0))
    Msg_Error_Exit("FLOPPY/Floppy.be FILE ERROR");

  /* Initialise_Rijndael */
  if(!Initialise_Rijndael())
    Msg_Error_Exit("Initialise_Rijndael FILE ERROR");
  Initialise_RijndaelMini();

  /* looking for a:Machvots.tbl */
  if(access("DATA/machvots.tbl", 0))
    Msg_Error_Exit("DATA/machvots.tbl FILE ERROR");
}
