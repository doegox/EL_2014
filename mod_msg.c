/*
#define EL2014
*/

/**************************************************************************
<MODULE NAME>
<FILENAME> mod_msg.c </FILENAME>
<HISTORY>
  <MODIFICATION>
    <DATE> </DATE>
    <AUTHOR>  </AUTHOR>
    < DESCRIPTION>
    Messages Management
    < /DESCRIPTION>
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
#include "scrutin.h"

/**************************************************************************
EXTERNAL VARIABLES
**************************************************************************/
extern char      gcCodeLg;
extern int       giCurrentScr;
extern s_Scrutin gshScrutin[10];

/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/

/**************************************************************************
GLOBAL VARIABLES
**************************************************************************/
char **pszMsg;
char  *pszMsg1[N_SCR_MESSAGE];
char  *pszMsg2[N_SCR_MESSAGE];

/**************************************************************************
<FUNCTION>
  <NAME> Msg_Init </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Memory allocation of globals (msg) pointers
    </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Msg_Init(void)
{
 static char aszMsg1[N_SCR_MESSAGE][L_SCR_MESSAGE];
 static char aszMsg2[N_SCR_MESSAGE][L_SCR_MESSAGE];
 int i;

  for(i = 0; (i < N_SCR_MESSAGE); i++)
  {
    pszMsg1[i] = aszMsg1[i];
    pszMsg2[i] = aszMsg2[i];
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Msg_Error_Exit </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  puts string to stdout and exit
    </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Msg_Error_Exit(char *p)
{
  puts(p);
  exit(1);
}

/**************************************************************************
<FUNCTION>
  <NAME> Msg_Read </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  Read a msg file and
    </DESCRIPTION>
</FUNCTION>
**************************************************************************/
static void Msg_Read(char *fichier,char **table)
{
  FILE *fpm;
  char bufm[L_SCR_MESSAGE];
  int i, j;

  fpm = fopen(fichier,"r");
  if(fpm == NULL) Msg_Error_Exit("MESSAGE FILE ERROR");
  i = 0;

  while((fgets(bufm,L_SCR_MESSAGE-1,fpm)!= NULL))
  {
    j = strlen(bufm);
    if(j < L_SCR_MESSAGE-1) bufm[j-1] = 0x00;
    else   bufm[L_SCR_MESSAGE-1] = 0x00;
    strcpy((char *)table[i],bufm);
    ++i;
  }
  fclose(fpm);
}

/**************************************************************************
<FUNCTION>
  <NAME> Msg_Fill </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  calls Msg_Read with appropriate parameters
    </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Msg_Fill(void)
{
  switch(gcCodeLg)
  {
    case '3': /* German */
         Msg_Read("messagea",pszMsg1);
         break;

    case '5': /* Choose between French or German */
         Msg_Read("messagef",pszMsg1);
         Msg_Read("messagea",pszMsg2);
         break;

    case '6': /* Choose between French or Dutch */
         Msg_Read("messagef",pszMsg1);
         Msg_Read("messagen",pszMsg2);
         break;

    case '7': /* French */
         Msg_Read("messagef",pszMsg1);
         break;

    case '8': /* Choose between German or French */
         Msg_Read("messagea",pszMsg1);
         Msg_Read("messagef",pszMsg2);
         break;

    default:
         Msg_Error_Exit("Unknow language");
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Msg_Election </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION></DESCRIPTION>
  <RETURNS>
    <TYPE> int </TYPE>
    <DESCRIPTION>
    Returns the message id of the current election (gemeente, ...)
    </DESCRIPTION>
  </RETURNS>
</FUNCTION>
**************************************************************************/
int Msg_Election(void)
{
  switch(gshScrutin[giCurrentScr].TYPE_SCRUTIN)
  {
#ifdef EL2014
    case 1:
      return 0;
    case 2:
      return 3;
    case 4:
      return 9;
    case 7:
      return 139;
    case 9:
      return 141;
    default:
      return 0;
#else
    case 1:
      return 0;
    case 5:
      return 12;
    case 6:
      return 15;
    case 8:
      return 134;
    case 9:
      return 137;
    default:
      return 0;
#endif
  }
}
