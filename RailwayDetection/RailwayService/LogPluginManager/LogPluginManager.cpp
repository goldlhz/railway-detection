#include "stdafx.h"
#include "ManagerTool.h"
#include "LogPluginManager.h"

extern int InitPluginCount;
extern CCriticalSection csSection;

LOGMOUDLE DWORD __stdcall InitLogPluginManagerA(char* moduleName)
{
	USES_CONVERSION;
	return InitLogPluginManagerW(A2W(moduleName));
}

LOGMOUDLE DWORD __stdcall InitLogPluginManagerW(wchar_t* moduleName)
{
	if(moduleName == _T(""))
		return -1;

	if(InitPluginCount <= 0)
	{
		bool			bTempTest = false;
		DWORD			nTempTest = 0;
		vector<tstring> vecPlugin;
		tstring         strPluginPath = GetLogPluginPath();

		if(strPluginPath != _T(""))
		{
			ReadLogerIniFile(strPluginPath);

			if(CreateSynLogEvent())
			{
				if(CreateWriteThread())
				{
					if(EnumInstallPlugin(strPluginPath, vecPlugin))
					{
						BuildPluginFileName(strPluginPath, vecPlugin);

						nTempTest = LoadPlugin(strPluginPath, moduleName, vecPlugin);
						if(nTempTest == 0 || nTempTest == 1)
						{
							++InitPluginCount;
							return nTempTest;
						}
						return nTempTest;
					}
				}
			}
		}
		return -1;
	}

	++InitPluginCount;
	return 0;
}

LOGMOUDLE DWORD __stdcall WriteLogInfoBaseA(DWORD errorLevel, const char* file, int line, const char* fmt, ...)
{
	string strInfo;
	if (NULL != fmt)
	{
		va_list vargs;
		va_start(vargs, fmt);

		size_t len = _vscprintf(fmt, vargs)+1;
		vector<char> buffer(len, '\0');

		int nWritten = vsprintf_s(&buffer[0], buffer.size(), fmt, vargs);
		if (nWritten>0)
			strInfo = &buffer[0];

		va_end(vargs);

		return DoWriteLogInfoBaseA(errorLevel, strInfo.c_str(), file, line);
	}
	return -1;
}

LOGMOUDLE DWORD __stdcall WriteLogInfoBaseW(DWORD errorLevel, const wchar_t* file, int line, const wchar_t* fmt, ...)
{
	tstring strInfo;
	if (NULL != fmt)
	{
		va_list vargs;
		va_start(vargs, fmt);

		size_t len = _vsctprintf(fmt, vargs)+1;
		vector<TCHAR> buffer(len, _T('\0'));

		int nWritten = _vstprintf_s(&buffer[0], buffer.size(), fmt, vargs);
		if (nWritten>0)
			strInfo = &buffer[0];

		va_end(vargs);

		return DoWriteLogInfoBaseW(errorLevel, strInfo.c_str(), file, line);
	}
	return -1;
}

LOGMOUDLE DWORD __stdcall DoWriteLogInfoBaseA(DWORD errorLevel, const char* infoStr, const char* file, int line)
{
	USES_CONVERSION;
	return DoWriteLogInfoBaseW(errorLevel, A2W(infoStr), A2W(file), line);
}

LOGMOUDLE DWORD __stdcall DoWriteLogInfoBaseW(DWORD errorLevel, const wchar_t* infoStr, const wchar_t* file, int line)
{
	if(InitPluginCount > 0)
	{
		if(IsErrorLevelLog(errorLevel))
		{
			tstring strLine = IntToTString(line);
			if(errorLevel <= LOG_ERR)
			{
				SYSTEMTIME createTime;
				GetLocalTime(&createTime);

				int nBack;
				csSection.Lock();
				nBack = LoopWriteLog(errorLevel, createTime, infoStr, file, strLine);
				csSection.Unlock();

				return nBack;
			}
			else
			{
				WriteLogToDeque(errorLevel, infoStr, file, strLine);
			}
		}
		return 0;
	}
	return -1;
}

LOGMOUDLE DWORD __stdcall WriteLogInfoBaseExA(DWORD errorLevel, const char* file, int line, const char * fileName, const char* fmt, ...)
{
	string strInfo;
	if (NULL != fmt)
	{
		va_list vargs;
		va_start(vargs, fmt);

		size_t len = _vscprintf(fmt, vargs)+1;
		vector<char> buffer(len, '\0');

		int nWritten = vsprintf_s(&buffer[0], buffer.size(), fmt, vargs);
		if (nWritten>0)
			strInfo = &buffer[0];

		va_end(vargs);

		return DoWriteLogInfoBaseExA(errorLevel, strInfo.c_str(), file, line, fileName);
	}
	return -1;
}

LOGMOUDLE DWORD __stdcall WriteLogInfoBaseExW(DWORD errorLevel, const wchar_t* file, int line, const wchar_t * fileName, const wchar_t* fmt, ...)
{
	tstring strInfo;
	if (NULL != fmt)
	{
		va_list vargs;
		va_start(vargs, fmt);

		size_t len = _vsctprintf(fmt, vargs)+1;
		vector<TCHAR> buffer(len, _T('\0'));

		int nWritten = _vstprintf_s(&buffer[0], buffer.size(), fmt, vargs);
		if (nWritten>0)
			strInfo = &buffer[0];

		va_end(vargs);

		return DoWriteLogInfoBaseExW(errorLevel, strInfo.c_str(), file, line, fileName);
	}
	return -1;
}

LOGMOUDLE DWORD __stdcall DoWriteLogInfoBaseExA(DWORD errorLevel, const char* infoStr, const char* file, int line, const char * fileName)
{
	USES_CONVERSION;
	return DoWriteLogInfoBaseExW(errorLevel, A2W(infoStr), A2W(file), line, A2W(fileName));
}

LOGMOUDLE DWORD __stdcall DoWriteLogInfoBaseExW(DWORD errorLevel, const wchar_t* infoStr, const wchar_t* file, int line, const wchar_t * fileName)
{
	if(fileName == NULL)
		return DoWriteLogInfoBaseW(errorLevel, infoStr, file, line);
	else
	{
		if(InitPluginCount > 0)
		{
			if(IsErrorLevelLog(errorLevel))
			{
				SYSTEMTIME createTime;
				tstring strLine = IntToTString(line);

				strLine = IntToTString(line);
				GetLocalTime(&createTime);

				int nBack;
				csSection.Lock();
				nBack = LoopWriteLogEx(errorLevel, createTime, infoStr, file, strLine, fileName);
				csSection.Unlock();

				return nBack;
			}
			return 0;
		}
		return -1;
	}
}

LOGMOUDLE DWORD __stdcall UnInitLogPluginManager()
{
	--InitPluginCount;
	if(InitPluginCount <= 0)
	{
		CloseWriteThread();
		CloseSynLogEvent();
		FlashLogQueue();
		ReleasePlugin();
	}
	return 0;
}