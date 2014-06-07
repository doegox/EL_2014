
#ifndef __StrType_H__
#define __StrType_H__

#include <stdio.h>

#ifdef __cplusplus
/*C++ define*/

extern "C"
{
#endif
	/*C define*/

	typedef struct s_PcType
	{
		int iPcType;
		int iMgCardType;
		int iLigthPenType;
		int iStatus;
	} s_PcType;

#ifdef __cplusplus
}
#endif
#endif /* __StrType_H__ */