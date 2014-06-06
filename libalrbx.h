
#ifndef __LIBALRBX_H__
#define __LIBALRBX_H__

#include <stdio.h>

#ifdef __cplusplus
/*C++ define*/

extern "C"
{
#endif
	/*C define*/
#include "strtype.h"

	int InitAlarmBox(int iTypePc);
	void FreeResource(void);
	int GetStatus(void);

	void SetGreenLed(void);
	void SetRedLed(void);
	void SetLeds(void);
	void ResetLeds(void);
	unsigned char ButtonPressed(int Seconds);
	s_PcType LoadDefinitionFile(void);

#ifdef __cplusplus
}
#endif
#endif /* __LIBALRBX_H__ */