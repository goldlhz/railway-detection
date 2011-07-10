#include "StdAfx.h"
#include <shlwapi.h>
#include <string>
#include "CommonFunction.h"

using namespace std;

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

string CCommonFunction::GetLogPluginPath(HMODULE hInstance)
{
	DWORD  tempTest = 0;
	const  int nBufLenght = MAX_PATH * sizeof(TCHAR);
	TCHAR  strPluginPathBuf[nBufLenght];

	tempTest = GetModuleFileName(hInstance, strPluginPathBuf, nBufLenght);
	if(tempTest != 0)
	{
		int tempSize = 0;
		PathRemoveFileSpec(strPluginPathBuf);

		tempSize = sprintf_s(strPluginPathBuf, nBufLenght, _T("%s"), strPluginPathBuf);
		if(tempSize == -1)
			return _T("");

		return strPluginPathBuf;
	}
	return _T("");
}

string CCommonFunction::IntToString(int n)
{
	int nTemp1, nIndex;
	const int nBufferLen = 20;
	char intBuffer[nBufferLen] = {0};
	nIndex = nBufferLen - 2;

	for(; nIndex >= 0; --nIndex)
	{
		nTemp1 = n / 10;
		intBuffer[nIndex] = '0' + (n % 10);

		if(nTemp1 > 0)
		{
			n = nTemp1;
			continue;
		}
		break;
	}

	return intBuffer + nIndex;
}

unsigned int CCommonFunction::StringToInt(string& strData)
{
	unsigned int nBack = 0;

	for (string::iterator iterbegin = strData.begin(); iterbegin != strData.end(); ++iterbegin)
	{
		if(( (*iterbegin) >= '0') && ( (*iterbegin) <= '9'))
		{
			nBack *= 10;
			nBack += ((unsigned int)(*iterbegin) - (unsigned int)('0'));
		}
	}
	return nBack;
}

string CCommonFunction::GetCurrentTimeByFormat()
{
#define  TIMEBUFFER	32

	SYSTEMTIME systime;
	char strTimeBuffer[TIMEBUFFER] = {0};

	GetLocalTime(&systime);
	sprintf_s(strTimeBuffer, TIMEBUFFER, "%.04d%.02d%.02d%.02d%.02d%.02d%.03d",//"%.04d%.02d%.02d%.02d%.02d%.02d",
		systime.wYear,
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds);

	return strTimeBuffer;
}
