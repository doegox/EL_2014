/**************************************************************************
<MODULE NAME>
<FILENAME> mod_hwd.c (hardware) </FILENAME>
<HISTORY>
  <MODIFICATION>
    <DATE> </DATE>
    <AUTHOR></AUTHOR>
    <DESCRIPTION>
    Hardware initialization (using Libcrdrd.lib)
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
#include <time.h>
#include <graphics.h>
#include "libcrdrd.h"
#include "libalrbx.h"
#include "defs.h"

/**************************************************************************
EXTERNAL VARIABLES
**************************************************************************/
extern int giCodeVoteEnd;

/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/
extern void Msg_Error_Exit(char *);
extern void Wait_Loop_Int(int _iSecs);

/**************************************************************************
GLOBAL VARIABLES
**************************************************************************/
unsigned char gucMavType;

/**************************************************************************
<FUNCTION>
  <NAME> Check_Terminal </NAME>
  <TYPE> LOCAL </TYPE>
  <DESCRIPTION>
  Checks the AlarmBox and the magnetic card reader
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
static int Check_Terminal(int iType)
{
  int iResult = InitAlarmBox(iType);
  if(iResult < 0)
  {
    Msg_Error_Exit("Voting Terminal ?");
    iResult = -1;
  }
  return iResult;
}

/**************************************************************************
<FUNCTION>
  <NAME> Hwd_Init </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Hardware initialization
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Hwd_Init(void)
{
  int iRet;

  s_PcType sPcType; /* informations, type of pc, type of lightpen, ... */
  s_Parameters sParametersMagnCard;

  /* Load informations, type of pc, type of lightpen, ... */
  sPcType =  LoadDefinitionFile();

  if(sPcType.iStatus == -1)
  {
    ;/*DRO Error */
    printf("\r\nError status\r\n");
    exit(-1);
  }
  else if(Check_Terminal(sPcType.iPcType) < 0)
  {
    ;/*DRO Error */
    printf("\r\nError Check_Terminal\r\n");
    exit(-2);
  }
  else if(InitLibCardReader(sPcType.iPcType) < 0)
  {
    ;/*DRO Error */
    printf("\r\nError InitLibCardReader\r\n");
    exit(-3);
  }
  gucMavType = sPcType.iPcType;

  /* Initialization of the Card Reader */
  iRet = Lecteur_Init();
  if(iRet != 0)
  {
    printf("Initialization of the Card Reader...\n");
    printf("\tLecteur_Init returns %d \n", iRet);
    Wait_Loop_Int(10); /* Waiting for 10 seconds but don't exit */
  }
}

/**************************************************************************
<FUNCTION>
  <NAME> Test_AlarmBox </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Does the alarm box still connected ?
  </DESCRIPTION>
  <RETURNS>
    <TYPE> unsigned char </TYPE>
    <DESCRIPTION>
    O -> problem
    1 -> ok
    </DESCRIPTION>
  </RETURNS>
</FUNCTION>
**************************************************************************/
unsigned char Test_AlarmBox(void)
{
  return(1);
}

/**************************************************************************
<FUNCTION>
  <NAME> Leds (light-emitting diode) </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION> Leds Management + alarm (bip) </DESCRIPTION>
  <PARAMETERS>
    <PARAMETER>
      <NAME> _cType </NAME>
      <TYPE> char </TYPE>
      <IO> IN </IO>
      <DESCRIPTION>
      OFF
            RR -> red
            GG -> green
            RG -> red & green
      </DESCRIPTION>
    </PARAMETER>
    <PARAMETER>
      <NAME> _iBell </NAME>
      <TYPE> int </TYPE>
      <IO> IN </IO>
      <DESCRIPTION>
      + bells(1) or not(0)
      </DESCRIPTION>
    </PARAMETER>
  </PARAMETERS>
</FUNCTION>
**************************************************************************/
void Leds(char _cType, int _iBell)
{
  int i;

  switch(_cType)
  {
    case OFF: /* switch off the leds */
      ResetLeds();
      break;
    case RR: /* switch on the red led */
      SetRedLed();
      break;
    case RG: /* switch on the red and green leds */
      SetLeds();
      break;
    case GG: /* switch on the green led */
      SetGreenLed();
      break;
  }

  /* Bip */
  if(_iBell)
  {
    printf("Beeeep....\n");
  }
}

/**************************************************************************/
unsigned char Test_LightPen(void)
{
    return 1;
}
