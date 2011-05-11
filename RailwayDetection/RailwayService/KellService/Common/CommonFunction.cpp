#include "StdAfx.h"
#include "CommonFunction.h"
#include <shlwapi.h>

CCommonFunction::CCommonFunction(void)
{
}

int CCommonFunction::GetKernalCount()
{
	SYSTEM_INFO systemInfo;

	memset(&systemInfo, 0x00, sizeof(SYSTEM_INFO));
	GetSystemInfo(&systemInfo);

	return (int)systemInfo.dwNumberOfProcessors;
}

int CCommonFunction::GetBackLogCount()
{
	int nBackLog = 0;
	int nKernalCount = GetKernalCount();

	nBackLog = nKernalCount * 5;
	if(nBackLog < 5)
		return 5;

	return nBackLog;
}

wstring CCommonFunction::GetLogPluginPath(HMODULE hInstance)
{
	DWORD  tempTest = 0;
	const  int nBufLenght = MAX_PATH * sizeof(TCHAR);
	TCHAR  strPluginPathBuf[nBufLenght];

	tempTest = GetModuleFileName(hInstance, strPluginPathBuf, nBufLenght);
	if(tempTest != 0)
	{
		int tempSize = 0;
		PathRemoveFileSpec(strPluginPathBuf);

		tempSize = swprintf_s(strPluginPathBuf, nBufLenght, _T("%s"), strPluginPathBuf);
		if(tempSize == -1)
			return _T("");

		return strPluginPathBuf;
	}
	return _T("");
}