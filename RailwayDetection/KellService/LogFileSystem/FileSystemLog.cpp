#include "stdafx.h"
#include "LogFile.h"
#include "FileSystemLog.h"

extern tstring strLastHour;
extern tstring strDirPath;
extern tstring strModuleName;
extern int     InitPluginCount;


LOGMOUDLE bool InitPlugin(const tstring & filePath, const tstring & moduleName)
{
	ReadLogerIniFile(filePath);

	strDirPath = filePath;
	strModuleName=moduleName;
	++InitPluginCount;
	return true;
}

LOGMOUDLE bool UnInitPlugin()
{
	--InitPluginCount;
	if(InitPluginCount <= 0)
	{
		CloseAllFile();
		strDirPath = _T("");
		strLastHour= _T("");
		InitPluginCount = false;
	}
	return true;
}

LOGMOUDLE bool WriteLogInfo(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & infoStr, const tstring & file, const tstring & line)
{
	if(InitPluginCount > 0)
	{
		tstring strTempTest = _T("");
		strTempTest = CreateLogDirectory(strDirPath, createTime);
		if(strTempTest != _T(""))
		{
			tstring strCurrentTime = BuildHourString(createTime);
			if(strCurrentTime != strLastHour)
			{
				strLastHour = strCurrentTime;
				CloseAllFile();
			}

			if(!WriteFullLog(errorLevel, createTime, infoStr, file, line, strTempTest))
				return false;

			if(!WriteAloneLog(errorLevel, createTime, infoStr, file, line, strTempTest))
				return false;

			return true;
		}
	}
	return false;
}

LOGMOUDLE bool WriteLogInfoEx(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & infoStr, const tstring & file, const tstring & line, const tstring & fileName)
{
	if(InitPluginCount > 0)
	{
		tstring strTempTest = _T("");
		strTempTest = CreateLogDirectory(strDirPath, createTime);
		if(strTempTest != _T(""))
		{
			tstring strCurrentTime = BuildHourString(createTime);
			if(strCurrentTime != strLastHour)
			{
				strLastHour = strCurrentTime;
				CloseAllFile();
			}

			if(!WriteFullLogEx(errorLevel, createTime, infoStr, file, line, strTempTest,fileName))
				return false;

			return true;
		}
	}
	return false;
}