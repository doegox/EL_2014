
#ifndef __LibCrdRd_H__
#define __LibCrdRd_H__

#include <stdio.h>

#ifdef __cplusplus
/*C++ define*/

extern "C"
{
#endif
	/*C define*/

//	int Lecteur_Init(s_Parameters* shParm);
	int InitLibCardReader(int iTypePc);
	/* << 8.67c */
	int Status_Commande(void);
	/* 8.67c >> */
	int Read_Commande(char* pzLectBuffer);
	int Write_Commande(const char* pzString);
	int Eject_Commande(unsigned char ucDirection);
	int Tempo(void);
	int Wait_Card(void);
	void CheckQueue(void);
	char* CommandDirect(char cCommande, char cParam, char* pzData);

#ifdef __cplusplus
}
#endif
#endif /* __LibCrdRd_H__ */