
#include "libalrbx.h"
#include "defs.h"
#include <stdio.h>
#include <time.h>

int InitLibrary(void)
{
	return 1;
}

int InitAlarmBox(int iTypePc)
{
	printf("InitAlarmBox()\n");
	return 1;
}

void FreeResource()
{
	printf("FreeResource()\n");
}

int GetStatus(void)
{
	printf("GetStatus()\n");
	return 1;
}

void SetGreenLed(void)
{
	printf("SetGreenLed()\n");
}

void SetRedLed(void)
{
	printf("SetRedLed()\n");
}

void SetLeds(void)
{
	printf("SetLeds()\n");
}

void ResetLeds(void)
{
	printf("ResetLeds()\n");
}

unsigned char ButtonPressed(int iSeconds)
{
	printf("ButtonPressed(%i)\n", iSeconds);
}


s_PcType LoadDefinitionFile(void)
{
    int iRep = 0;
	s_PcType sResult;
	sResult.iPcType = MAV_JITES_1;
//	sResult.iLigthPenType = (int)hPcType.GetLightPenType();
//	sResult.iMgCardType = (int)hPcType.GetMagCardType();
	sResult.iStatus = 0;
    return sResult;
}