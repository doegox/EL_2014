/*
#define TST_EL2014
*/

/**************************************************************************
<MODULE NAME>
<FILENAME> mod_card.c </FILENAME>
<HISTORY>
  <MODIFICATION>
    <DATE> </DATE>
    <AUTHOR>  </AUTHOR>
    <DESCRIPTION>
    The voting card contents...
     (crc, jeton, Read/Write)
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
#include <bios.h>
#include "defs.h"
#include "..\CrdReade.r\LibCrdRd.h"
#include "..\LibAlrBx\LIBALRBX.H"

/**************************************************************************
EXTERNAL VARIABLES
**************************************************************************/
extern char gcCardRW;
extern int  giSWCard;
extern unsigned char gucMavType;
extern char gszCrcCalcul[5];

/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/
extern void  Leds(char, int);
extern int (*ptfunct)(void);
extern int   Vote_End(void);
extern void  Wait_Loop(int);

/**************************************************************************
<FUNCTION>
  <NAME> Card_Crc_Read </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Get the CRC
  </DESCRIPTION>
  <PARAMETERS>
    <PARAMETER>
      <NAME> _szCard </NAME>
      <TYPE> char * </TYPE>
      <IO> IN </ IO >
      <DESCRIPTION>
      The string read from a voting card
      </DESCRIPTION>
    </PARAMETER>
    <PARAMETER>
      <NAME> _szCrc </NAME>
      <TYPE> char * </TYPE>
      <IO> OUT </ IO >
      <DESCRIPTION>
      The crc read
      </DESCRIPTION>
    </PARAMETER>
  </PARAMETERS>
</FUNCTION>
**************************************************************************/
#pragma argsused
void Card_Crc_Read(char *_szCard, char *_szCrc)
{
  int i;

  for(i=0;i<_C_CRC_MAX_BYTE;_szCrc[i] = _szCard[_C_CRC_PLACE + i], i++);

  _szCrc[4] = 0x00;
}

/**************************************************************************
<FUNCTION>
  <NAME> Card_Crc_Write </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Write the CRC
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
#pragma argsused
void Card_Crc_Write(char *_szCarte)
{
  int i;
  for(i=0;i<_C_CRC_MAX_BYTE;_szCarte[_C_CRC_PLACE+i] = gszCrcCalcul[i], i++);
}

/**************************************************************************
<FUNCTION>
  <NAME> Card_RW </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Does the card contain a vote ? (yes -> readonly, no -> write)
  -> set value to gcCardRW
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Card_RW(char *_szCarte)
{
  if(_szCarte[_C_PASSAGE_PLACE] == '0') gcCardRW = CARD_WRITE;
  else                                  gcCardRW = CARD_READ;

  /* Demo */
  if(giSWCard == _NOCARD) gcCardRW = CARD_WRITE;
}

/**************************************************************************
<FUNCTION>
  <NAME> Card_Written </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Specify that the vote has been done
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Card_Written(char *_szCarte)
{
  _szCarte[_C_PASSAGE_PLACE] = '1';
}


#ifdef TST_EL2014
/**************************************************************************
<FUNCTION>
  <NAME> Card_UnWritten </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Specify that the vote has been done
  </DESCRIPTION>
</FUNCTION>
**************************************************************************/
void Card_UnWritten(char *_szCarte)
{
  _szCarte[_C_PASSAGE_PLACE] = '0';
}
#endif


/**************************************************************************
<FUNCTION>
  <NAME> Card_Jeton </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Get the JETON
  </DESCRIPTION>
  <PARAMETERS>
    <PARAMETER>
      <NAME> _szCard </NAME>
      <TYPE> char * </TYPE>
      <IO> IN </ IO >
      <DESCRIPTION>
      The string read from a voting card
      </DESCRIPTION>
    </PARAMETER>
    <PARAMETER>
      <NAME> _szJeton </NAME>
      <TYPE> char * </TYPE>
      <IO> OUT </ IO >
      <DESCRIPTION>
      The jeton read
      </DESCRIPTION>
    </PARAMETER>
  </PARAMETERS>
</FUNCTION>
**************************************************************************/
void Card_Jeton(char *_szCard, char *_szJeton)
{
  int i;

  for(i=0; i<_C_JETON_MAX_BYTE; _szJeton[i]=_szCard[_C_JETON_PLACE + i], i++);

  _szJeton[_C_JETON_MAX_BYTE] = '\0';
}

/**************************************************************************
<FUNCTION>
  <NAME> Card_Eject </NAME>
  <TYPE> GLOBAL </TYPE>
  <DESCRIPTION>
  Ejection of the card from the card reader
  </DESCRIPTION>
  <RETURNS>
    <TYPE> int </TYPE>
    <DESCRIPTION> Card Ejected (1) or not (0) </DESCRIPTION>
  </RETURNS>
</FUNCTION>
**************************************************************************/
int Card_Eject(void)
{
  int i, iStatus;

  iStatus = Eject_Commande(0);
  if(gucMavType == MAV_DIGIVOTE_1 || gucMavType == MAV_DIGIVOTE_2)
  {
    while(iStatus == CARD_GATE)
    {
      for(i=0; i<5; ++i)
      {
        Wait_Loop(1);
        iStatus = Status_Commande();
        if(iStatus == CARD_NO)
          return 1;
      }
      if(iStatus != CARD_NO)
      {
        iStatus = Eject_Commande(0);
        /* switch on the red led */
        Leds(RR,0);
      }
    }
  }
  else /* JITES */
  {
    while(iStatus != CARD_NO)
    {
      for(i=0; i<5; ++i)
      {
        Wait_Loop(1);
        iStatus = Status_Commande();
        if(iStatus == 0)
          return 1;
      }
      if(iStatus != 0)
      {
        iStatus = Eject_Commande(0);
        /* switch on the red led */
        Leds(RR,0);
      }
    }

  }
/* 8.67c >> */
  return 1;
}
