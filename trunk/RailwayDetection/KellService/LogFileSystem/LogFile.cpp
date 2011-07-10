#include "stdafx.h"
#include "FileSystemLog.h"
#include "LogFile.h"

typedef struct _nameHandle
{
	tstring strFileName;
	FILE * fHandle;
}NameHandle;
//////////////////////////////////////////////////////////////////////////
// 全局变量定义
//////////////////////////////////////////////////////////////////////////
FILE *  fullFile  = NULL;
FILE *	emergFile = NULL;
FILE *  critFile  = NULL;
FILE *	errFile   = NULL;
FILE *  infoFile  = NULL;
FILE *	debugFile = NULL;

tstring strLastHour = _T("");
tstring strDirPath  = _T("");
tstring strModuleName=_T("");
int     InitPluginCount= 0;
int		CreateFullLoger = 1;					// 是否生成全量的日志文件,default:1
int		CreateAloneLoger = 0;					// 是否创建单独等级的日志文件,default:0
int		CreateLogerEx = 1;						// 是否创建扩展日志文件
vector<NameHandle> vecNameHandle;
//////////////////////////////////////////////////////////////////////////
// 日志函数实现区
//////////////////////////////////////////////////////////////////////////
tstring CreateLogDirectory(tstring dirPath, const SYSTEMTIME & createTime)
{
	BOOL  bTempTest = FALSE;
	DWORD nTempTest = 0;

	dirPath += _T("\\..\\Log");
	bTempTest = CreateDirectory(dirPath.c_str(), NULL);
	if(bTempTest == FALSE)
	{
		nTempTest = GetLastError();
		if(nTempTest != ERROR_ALREADY_EXISTS)
		{
			return _T("");
		}
	}

	dirPath += _T("\\");
	dirPath += strModuleName;
	bTempTest = CreateDirectory(dirPath.c_str(), NULL);
	if(bTempTest == FALSE)
	{
		nTempTest = GetLastError();
		if(nTempTest != ERROR_ALREADY_EXISTS)
		{
			return _T("");
		}
	}

	dirPath += _T("\\");
	dirPath += BuildMonthString(createTime);
	bTempTest = CreateDirectory(dirPath.c_str(), NULL);
	if(bTempTest == FALSE)
	{
		nTempTest = GetLastError();
		if(nTempTest != ERROR_ALREADY_EXISTS)
		{
			return _T("");
		}
	}
	return dirPath;
}

bool CreateLogFile(const tstring & fileName, FILE *& file)
{
	if(file == NULL)
	{
#ifdef UNICODE
		file = _wfsopen(fileName.c_str(), _T("a, ccs=UTF-8"), _SH_DENYNO);
#else
		file = _fsopen(fileName.c_str(), _T("a"), _SH_DENYNO);
#endif
		return (file == NULL) ? false : true;
	}
	return true;
}

DWORD WriteLogFile(const tstring & logInfomation, FILE * file)
{
	if(file != NULL)
	{
		int nCount = 0;
		nCount = tputs(logInfomation.c_str(), file);
		_flushall();
		return (nCount >= 0) ? 1 : 0;	
	}
	return 0;
}

void CloseAllFile()
{
#define CLOSE_FILE(handle)	if(handle)	fclose(handle); handle = NULL;

	CLOSE_FILE(fullFile)
	CLOSE_FILE(emergFile)
	CLOSE_FILE(critFile)
	CLOSE_FILE(errFile)
	CLOSE_FILE(infoFile)
	CLOSE_FILE(debugFile)

	vector<NameHandle>::size_type nCount = vecNameHandle.size();
	for (vector<NameHandle>::size_type nIndex = 0; nIndex < nCount; ++nIndex)
	{
		fclose(vecNameHandle.at(nIndex).fHandle);
	}
	vecNameHandle.clear();
}

tstring BuildMonthString(const SYSTEMTIME & createTime)
{
	const int  nBufLength = MAX_PATH * sizeof(TCHAR);
	TCHAR	   strBuf[nBufLength];	
	tprintf(strBuf, nBufLength, _T("%d-%d"), createTime.wYear, createTime.wMonth);
	return strBuf;
}

tstring BuildHourString(const SYSTEMTIME & createTime)
{
	const int  nBufLength = MAX_PATH * sizeof(TCHAR);
	TCHAR	   strBuf[nBufLength];
	tprintf(strBuf, nBufLength, _T("[%d-%d-%d: %d hour]"), createTime.wYear, createTime.wMonth, createTime.wDay, createTime.wHour);
	return strBuf;
}

tstring BuildDayString(const SYSTEMTIME & createTime)
{
	const int  nBufLength = MAX_PATH * sizeof(TCHAR);
	TCHAR	   strBuf[nBufLength];	
	tprintf(strBuf, nBufLength, _T("%d-%d-%d"), createTime.wYear, createTime.wMonth, createTime.wDay);
	return strBuf;
}

tstring BuildTimeString(const SYSTEMTIME & createTime)
{
	const int  nBufLength = MAX_PATH * sizeof(TCHAR);
	TCHAR	   strBuf[nBufLength];	
	tprintf(strBuf, nBufLength, _T("[%d-%d-%d] [%d:%d:%d]"), createTime.wYear, createTime.wMonth, createTime.wDay, createTime.wHour, createTime.wMinute, createTime.wSecond);
	return strBuf;
}

tstring GetErrorString(DWORD err)
{
	tstring strErr = _T("");
	switch(err)
	{
	case LOG_EMERG:
		strErr = _T("紧急");
		break;
	case LOG_CRIT:
		strErr = _T("严重");
		break;
	case LOG_ERR:
		strErr = _T("错误");
		break;
	case LOG_INFO:
		strErr = _T("信息");
		break;
	case LOG_DEBUG:
		strErr = _T("调试");
		break;
	default:
		strErr = _T("此行无效");
		break;
	}
	return strErr;
}

tstring BuildLogLine(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & infoStr, const tstring & file, const tstring & line)
{
	tstring strTime = BuildTimeString(createTime);
	strTime += _T("\t");
	strTime += GetErrorString(errorLevel);
	strTime += _T("\t");
	strTime += file;
	strTime += _T("\t");
	strTime += line;
	strTime += _T("\t");
	strTime += infoStr;
	strTime += _T("\n");
	return strTime;
}

void ReadLogerIniFile(tstring strPluginPath)
{
	if(strPluginPath != _T(""))
	{
		strPluginPath += _T("\\loger.ini");
		CreateFullLoger = GetPrivateProfileInt(_T("filesystemlog"), _T("CreateFullLoger"), 1, strPluginPath.c_str());
		CreateAloneLoger = GetPrivateProfileInt(_T("filesystemlog"), _T("CreateAloneLoger"), 0, strPluginPath.c_str());
		CreateLogerEx = GetPrivateProfileInt(_T("filesystemlog"), _T("CreateLogerEx"), 0, strPluginPath.c_str());

		if(CreateFullLoger < 0 || CreateFullLoger > 1)
			CreateFullLoger = 0;

		if(CreateAloneLoger < 0 || CreateAloneLoger > 1)
			CreateAloneLoger = 0;

		if(CreateLogerEx < 0 || CreateLogerEx > 1)
			CreateLogerEx = 0;
	}
}

bool WriteFullLog(DWORD errorLevel, const SYSTEMTIME & createTime, tstring infoStr, const tstring & file, const tstring & line, const tstring & filepath)
{
	if(CreateFullLoger == 1)
	{
		if(fullFile == NULL)
		{
			tstring strFileName = filepath;
			tstring strMiddle = _T("//======");
			strFileName += _T("\\");
			strFileName += BuildDayString(createTime);
			strFileName += _T(".log");
			if(!CreateLogFile(strFileName, fullFile))
				return false;

			strLastHour= BuildHourString(createTime);
			strMiddle += strLastHour;
			strMiddle += _T("\n");

			WriteLogFile(_T("//================================================================\n"), fullFile);
			WriteLogFile(strMiddle, fullFile);
			WriteLogFile(_T("//================================================================\n"), fullFile);
		}

		int nTempTest = 0;
		infoStr = BuildLogLine(errorLevel, createTime, infoStr, file, line);
		nTempTest = WriteLogFile(infoStr, fullFile);
		return (nTempTest == 0) ? false : true;
	}
	return true;
}

bool WriteFullLogEx(DWORD errorLevel, const SYSTEMTIME & createTime, tstring infoStr, const tstring & file, const tstring & line, const tstring & filepath, const tstring & fileName)
{
	if(CreateLogerEx == 1)
	{
		FILE * fileHandle = NULL;
		tstring strFileName = filepath;
		strFileName += _T("\\");
		strFileName += fileName;
		strFileName += _T("_");
		strFileName += BuildDayString(createTime);
		strFileName += _T(".log");

		fileHandle = FindHandleByFileName(strFileName);
		if(fileHandle == NULL)
		{
			if(!CreateLogFile(strFileName, fileHandle))
				return false;

			tstring strMiddle = _T("//======");
			strMiddle += BuildHourString(createTime);
			strMiddle += _T("\n");

			WriteLogFile(_T("//================================================================\n"), fileHandle);
			WriteLogFile(strMiddle, fileHandle);
			WriteLogFile(_T("//================================================================\n"), fileHandle);

			NameHandle tempNameHandle;
			tempNameHandle.strFileName = strFileName;
			tempNameHandle.fHandle = fileHandle;

			vecNameHandle.push_back(tempNameHandle);
		}

		int nTempTest = 0;
		infoStr = BuildLogLine(errorLevel, createTime, infoStr, file, line);
		nTempTest = WriteLogFile(infoStr, fileHandle);
		return (nTempTest == 0) ? false : true;
	}
	return true;
}

bool WriteAloneLog(DWORD errorLevel, const SYSTEMTIME & createTime, tstring infoStr, const tstring & file, const tstring & line, const tstring & filepath)
{
	if(CreateAloneLoger == 1)
	{
		FILE ** pFile = NULL;
		switch(errorLevel)
		{
		case 1:
			pFile = &emergFile;
			break;
		case 2:
			pFile = &critFile;
			break;
		case 3:
			pFile = &errFile;
			break;
		case 4:
			pFile = &infoFile;
			break;
		case 5:
			pFile = &debugFile;
			break;
		}

		if(pFile != NULL)
		{
			if(*pFile == NULL)
			{
				tstring strFileName = filepath;
				tstring strMiddle = _T("//======");
				strFileName += _T("\\");
				strFileName += BuildDayString(createTime);
				strFileName += _T(".");
				strFileName += GetErrorString(errorLevel);
				strFileName += _T(".log");
				if(!CreateLogFile(strFileName, *pFile))
					return false;

				strLastHour= BuildHourString(createTime);
				strMiddle += strLastHour;
				strMiddle += _T("\n");

				WriteLogFile(_T("//================================================================\n"), *pFile);
				WriteLogFile(strMiddle, *pFile);
				WriteLogFile(_T("//================================================================\n"), *pFile);
			}

			int nTempTest = 0;
			infoStr = BuildLogLine(errorLevel, createTime, infoStr, file, line);
			nTempTest = WriteLogFile(infoStr, *pFile);
			return (nTempTest == 0) ? false : true;
		}
	}
	return true;
}

FILE * FindHandleByFileName(const tstring & fileName)
{
	vector<NameHandle>::size_type nCount = vecNameHandle.size();
	for(vector<NameHandle>::size_type nIndex = 0; nIndex < nCount; ++nIndex)
	{
		if(vecNameHandle.at(nIndex).strFileName == fileName)
			return vecNameHandle.at(nIndex).fHandle;
	}

	return NULL;
}