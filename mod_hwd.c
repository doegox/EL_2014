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
#include <bios.h>
#include <io.h>
#include <fcntl.h>
#include <alloc.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <graphics.h>
#include "libcrdrd.h"
#include "..\LibAlrBx\LIBALRBX.H"
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
  else
  {
    /*Enable screen and Mc reader/writer  */
    if(iType == 1 | iType == 2)
      outportb(0x308,0xf4);
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

  sParametersMagnCard.iIoAddr = 0x300;
  sParametersMagnCard.iIrq = 5;
  sParametersMagnCard.iPort = 1;
  sParametersMagnCard.iSetting = _COM_CHR7 | _COM_STOP1 | _COM_EVENPARITY| _COM_9600;

  /* Initialization of the Card Reader */
  iRet = Lecteur_Init(&sParametersMagnCard);
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
  unsigned char iStatus = 0;

  if(gucMavType >= MAV_DIGIVOTE_1 && gucMavType <= MAV_DIGIVOTE_2)
  {

    if(!(inp(0x3BC + 1) & 0x08))
      iStatus = 1;
    if(!(inp(0x378 + 1) & 0x08))
      iStatus = 1;
    if(!(inp(0x278 + 1) & 0x08))
      iStatus = 1;
  }
  else /* JITES */
  {
    iStatus = 1;
  }

  return(iStatus);
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
    sound(2000);
    delay(3000);
    nosound();
  }
}

/**************************************************************************/
unsigned char Test_LightPen(void)
{
  if(gucMavType == MAV_DIGIVOTE_1 || gucMavType == MAV_DIGIVOTE_2)
  {
    asm {
     PUSH CX
     PUSH DX
     MOV  AH,4
     MOV  BX,'FT'
     MOV  CX,'GD'
     MOV  DX,'PN'
     INT  10H
     POP  DX
     POP  CX
    }

    if(_BX == 0) return 1;
  }
  else /* JITES */
  {
    return 1;
  }

  return 0;
}
