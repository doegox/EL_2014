#include <stdio.h>
#include "libcrdrd.h"
int InitLibCardReader(int iTypePc)
{
	printf("InitLibCardReader()\n");
	int iResult = 0;
	return iResult;
}

int Lecteur_Init(void)
{
	printf("LecteurInit()\n");
	return 0;
}

void FreeResourceCardMagn()
{
	printf("FreeResourceCardMagnt()\n");
}

int Status_Commande(void)
{
	printf("Status_Commande()\n");
        return 0;
}
int Read_Commande(char* pzLectBuffer)
{
	printf("Read_Commande(%s)\n", pzLectBuffer);
        return 0;
}
int Write_Commande(const char* pzString)
{
	printf("Write_Commande(%s)\n", pzString);
        return 0;
}

int Eject_Commande(unsigned char ucDirection)
{
	printf("Eject_Commande(%s)\n", ucDirection);
        return 0;
}

int Tempo(void)
{
	printf("Temppe()\n");
        return 0;
}

int Wait_Card(void)
{
	printf("Wait_Card()\n");
        return 0;
}

void CheckQueue(void)
{
	printf("CheckQueue()\n");
}

char* CommandDirect(char cCommande, char cParam, char* pzData)
{
	printf("CommandDirect(%c, %c, %s)\n", cCommande, cParam, pzData);
        return NULL;
}
