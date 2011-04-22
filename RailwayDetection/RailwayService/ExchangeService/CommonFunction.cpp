#include "StdAfx.h"
#include "CommonFunction.h"

CCommonFunction::CCommonFunction(void)
{
}

int CCommonFunction::GetCurrentKernalCount()
{
	SYSTEM_INFO systemInfo;

	memset(&systemInfo, 0x00, sizeof(SYSTEM_INFO));
	GetSystemInfo(&systemInfo);

	return (int)systemInfo.dwNumberOfProcessors;
}

int CCommonFunction::GetBackLogCount()
{
	int nBackLog = 0;
	int nKernalCount = GetCurrentKernalCount();

	nBackLog = nKernalCount * 5;
	if(nBackLog < 5)
		return 5;

	return nBackLog;
}

