#include "StdAfx.h"
#include "ManagerTool.h"
#include "AfxLogPluginManager.h"
#include <process.h> 
#include <shlwapi.h>

//////////////////////////////////////////////////////////////////////////
// 全局变量定义
//////////////////////////////////////////////////////////////////////////
DeqLogInfo			deqNoWriteLogInfo;										// 日志队列 使用规则：在尾加入，在头读出
VecPluginPointer	vecHadLoadPluginPointer;								// 已经加载的日志组件的函数指针数据
VecHModule			vecHadLoadPluginHModule;								// 已经加载的日志组件的模块句柄
int				    InitPluginCount = 0;									// 标识是否已经初始化
unsigned int		WriteLevelMask = 31;									// 需要归档的等级掩码,default:31
HANDLE				hLogManager[2] = {NULL, NULL};							// 0为同步队列读写的事件，1为生产者消费者事件
HANDLE				hThread = NULL;											// 写入线程的句柄
bool				IsTerminal = false;										// 标识线程是否结束
bool				IsSuspend  = true;										// 写入线程是否暂停
CCriticalSection	csSection;

//////////////////////////////////////////////////////////////////////////
// 管理器工具函数实现区
//////////////////////////////////////////////////////////////////////////
tstring GetLogPluginPath()
{
	DWORD  tempTest = 0;
	const  int nBufLenght = MAX_PATH * sizeof(TCHAR);
	TCHAR  strPluginPathBuf[nBufLenght];
	extern CLogPluginManagerApp theApp;

	tempTest = GetModuleFileName(theApp.m_hInstance, strPluginPathBuf, nBufLenght);
	if(tempTest != 0)
	{
		int tempSize = 0;
		PathRemoveFileSpec(strPluginPathBuf);

		tempSize = tprintf(strPluginPathBuf, nBufLenght, _T("%s%s"), strPluginPathBuf, _T("\\LogPlugin"));
		if(tempSize == -1)
			return _T("");

		return strPluginPathBuf;
	}
	return _T("");
}

bool EnumInstallPlugin(tstring strPluginPath, vector<tstring> &vecPlugin)
{
	BOOL   bTempTest = FALSE;
	HANDLE hPlugin = NULL;
	WIN32_FIND_DATA dataFind;

	strPluginPath += _T("\\*.logplugin");
	memset(&dataFind, 0x00, sizeof(WIN32_FIND_DATA));

	hPlugin = FindFirstFile(strPluginPath.c_str(), &dataFind);
	if(hPlugin != INVALID_HANDLE_VALUE)
	{
		vecPlugin.clear();
		do 
		{
			vecPlugin.push_back(dataFind.cFileName);

			memset(&dataFind, 0x00, sizeof(WIN32_FIND_DATA));
			bTempTest = FindNextFile(hPlugin, &dataFind);

		}while (bTempTest);

		FindClose(hPlugin);
		return true;
	}
	return false;
}

void BuildPluginFileName(tstring strPluginPath, vector<tstring> &vecPlugin)
{
	tstring strTemp;
	vector<tstring>::size_type nCount = vecPlugin.size();
	strPluginPath += _T("\\");
	
	for (vector<tstring>::size_type nIndex = 0; nIndex < nCount; ++nIndex)
	{	
		strTemp = strPluginPath;	
		strTemp += vecPlugin.at(nIndex);
		vecPlugin.at(nIndex) = strTemp;
	}
	return ;
}

int	LoadPlugin(const tstring & strPluginPath, const tstring & moduleName, const vector<tstring> & vecPlugin)
{
	vector<tstring>::size_type nCount = vecPlugin.size();
	vector<tstring>::size_type nBack  = nCount;

	vecHadLoadPluginHModule.clear();
	vecHadLoadPluginPointer.clear();

	for(vector<tstring>::size_type nIndex = 0; nIndex < nCount; ++nIndex)
	{
		HMODULE			hTempModule;
		PluginPointer	hTempPluginPointer;

		hTempModule = LoadLibrary(vecPlugin.at(nIndex).c_str());
		if(hTempModule == NULL)
		{
			--nBack;
			continue;
		}

		hTempPluginPointer.InitPlugin	= (InitPlugin_Type)GetProcAddress(hTempModule, "InitPlugin");
		hTempPluginPointer.UnInitPlugin	= (UnInitPlugin_Type)GetProcAddress(hTempModule, "UnInitPlugin");
		hTempPluginPointer.WriteLogInfo	= (WriteLogInfo_Type)GetProcAddress(hTempModule, "WriteLogInfo");
		hTempPluginPointer.WriteLogInfoEx= (WriteLogInfo_TypeEx)GetProcAddress(hTempModule, "WriteLogInfoEx");
		if(hTempPluginPointer.InitPlugin == NULL || hTempPluginPointer.UnInitPlugin == NULL || hTempPluginPointer.WriteLogInfo == NULL || hTempPluginPointer.WriteLogInfoEx == NULL)
		{
			--nBack;
			FreeLibrary(hTempModule);
			continue;
		}

		try
		{
			bool bTempTest = false;
			bTempTest = hTempPluginPointer.InitPlugin(strPluginPath, moduleName);
			if(bTempTest == true)
			{
				vecHadLoadPluginHModule.push_back(hTempModule);
				vecHadLoadPluginPointer.push_back(hTempPluginPointer);
				continue;
			}
			throw;
		}
		catch (...)
		{
			--nBack;
			FreeLibrary(hTempModule);
		}
	}
	return (nBack == nCount) ? 0 : ((nBack == 0) ? -1 : 1);
}

DWORD LoopWriteLog(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & strInfo, const tstring & strFile, const tstring & strLine)
{
	VecPluginPointer::size_type nCount = vecHadLoadPluginPointer.size();
	VecPluginPointer::size_type nBack  = nCount;

	for(VecPluginPointer::size_type nIndex = 0; nIndex < nCount; ++nIndex)
	{
		try
		{
			if(vecHadLoadPluginPointer.at(nIndex).WriteLogInfo(errorLevel, createTime, strInfo, strFile, strLine) == false)
				--nBack;
		}
		catch(...)
		{
			--nBack;
		}
	}

	return (nBack == nCount) ? 0 : ((nBack == 0) ? -1 : 1);
}

DWORD LoopWriteLogEx(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & strInfo, const tstring & strFile, const tstring & strLine, const tstring & fileName)
{
	VecPluginPointer::size_type nCount = vecHadLoadPluginPointer.size();
	VecPluginPointer::size_type nBack  = nCount;

	for(VecPluginPointer::size_type nIndex = 0; nIndex < nCount; ++nIndex)
	{
		try
		{
			if(vecHadLoadPluginPointer.at(nIndex).WriteLogInfoEx(errorLevel, createTime, strInfo, strFile, strLine, fileName) == false)
				--nBack;
		}
		catch(...)
		{
			--nBack;
		}
	}

	return (nBack == nCount) ? 0 : ((nBack == 0) ? -1 : 1);
}

void ReleasePlugin()
{
	VecHModule::size_type nCount = vecHadLoadPluginHModule.size();

	for(VecHModule::size_type nIndex = 0; nIndex < nCount; ++nIndex)
	{
		vecHadLoadPluginPointer.at(nIndex).UnInitPlugin();
		FreeLibrary(vecHadLoadPluginHModule.at(nIndex));
	}
	vecHadLoadPluginHModule.clear();
	vecHadLoadPluginPointer.clear();
}

tstring IntToTString(int data)
{
	int nTempTest = 0;
	const int data_length = 20;
	static TCHAR intBuffer[data_length];
	memset(intBuffer, 0x00, data_length);

	if(data > 0)
	{
		for(int nIndex = 2; nIndex <= data_length; ++nIndex)
		{
			nTempTest = data % 10;
			intBuffer[data_length - nIndex] = nTempTest + '0';
			data = data / 10;
			if(data == 0)
				return &(intBuffer[data_length - nIndex]);
		}
		return _T("");
	}
	return _T("");
}

bool CreateSynLogEvent()
{
	if(hLogManager[0] == NULL && hLogManager[1] == NULL)
	{
		hLogManager[0] = CreateEvent(NULL, FALSE, TRUE, BuildTimeString(0).c_str());			// 自动重置，初始有信号
		if(hLogManager[0] != NULL)
		{
			hLogManager[1] = CreateEvent(NULL, FALSE, FALSE, BuildTimeString(1).c_str());	// 自动重置，初始无信号
			if(hLogManager[1] != NULL)
			{
				return true;
			}
			CloseHandle(hLogManager[0]);
			hLogManager[0] = NULL;
		}
	}
	return false;
}

bool CloseSynLogEvent()
{
	if(hLogManager[0] != NULL)
	{
		CloseHandle(hLogManager[0]);
		hLogManager[0] = NULL;
	}

	if(hLogManager[1] != NULL)
	{
		CloseHandle(hLogManager[1]);
		hLogManager[1] = NULL;
	}
	return true;
}

bool FlashLogQueue()
{
	bool bTempTest = false;
	DeqLogInfo::size_type nCount = 0;
	LogInfo tempLogInfo;
	while(1)
	{
		bTempTest = deqNoWriteLogInfo.empty();
		if(bTempTest == false)
		{
			tempLogInfo = deqNoWriteLogInfo.front();
			deqNoWriteLogInfo.pop_front();
			LoopWriteLog(tempLogInfo.errorLevel, tempLogInfo.createTime, tempLogInfo.strInfo, tempLogInfo.strFile, tempLogInfo.strLine);	
			continue;
		}	
		else
		{
			deqNoWriteLogInfo.clear();
			break;
		}
	}
	return true;
}

bool CreateWriteThread()
{
	if(hThread == NULL)
	{
		IsTerminal = false;
		hThread = (HANDLE)_beginthreadex(NULL, 0,  WriteLogThreadFunc, NULL, 0, NULL);
		if(hThread != 0)
		{
			return true;
		}
		return false;
	}
	return true;
}

bool CloseWriteThread()
{
	if(hThread)
	{
		IsTerminal = true;
		SetEvent(hLogManager[1]);
		WaitForSingleObject(hThread, 5000);
		hThread = NULL;
	}

	return true;
}

unsigned __stdcall WriteLogThreadFunc(void * pArguments)
{
	DWORD	nTempTest = 0;
	bool	bTempTest = false;
	DeqLogInfo::size_type nCount = 0;
	LogInfo tempLogInfo;

	deqNoWriteLogInfo.clear();
	while(!IsTerminal)
	{
		nTempTest = WaitForSingleObject(hLogManager[1], INFINITE);
		if(nTempTest == WAIT_OBJECT_0)
		{
			while(!IsTerminal)
			{
				nTempTest = WaitForSingleObject(hLogManager[0], INFINITE);
				if(nTempTest == WAIT_OBJECT_0)
				{
					bTempTest = deqNoWriteLogInfo.empty();
					if(bTempTest == false)
					{
						tempLogInfo = deqNoWriteLogInfo.front();
						deqNoWriteLogInfo.pop_front();
						SetEvent(hLogManager[0]);
						LoopWriteLog(tempLogInfo.errorLevel, tempLogInfo.createTime, tempLogInfo.strInfo, tempLogInfo.strFile, tempLogInfo.strLine);	
					}	
					else
					{
						deqNoWriteLogInfo.clear();
						SetEvent(hLogManager[0]);
						break;
					}
				}
			}
		}
	}
	return 0;
}

bool WriteLogToDeque(DWORD errorLevel, const tstring & strInfo, const tstring & strFile, const tstring & strLine)
{
	LogInfo tempLogInfo;
	SYSTEMTIME createTime;
	GetLocalTime(&createTime);

	tempLogInfo.errorLevel = errorLevel;
	tempLogInfo.createTime = createTime;
	tempLogInfo.strInfo    = strInfo;
	tempLogInfo.strFile    = strFile;
	tempLogInfo.strLine    = strLine;

	WaitForSingleObject(hLogManager[0], INFINITE);
	deqNoWriteLogInfo.push_back(tempLogInfo);
	SetEvent(hLogManager[0]);
	SetEvent(hLogManager[1]);
	return true;
}

void ReadLogerIniFile(tstring strPluginPath)
{
	if(strPluginPath != _T(""))
	{
		strPluginPath += _T("\\loger.ini");
		WriteLevelMask = GetPrivateProfileInt(_T("logplugin"), _T("WriteLevelMask"), 31, strPluginPath.c_str());
    	WriteLevelMask = WriteLevelMask & 0x1F;
	}
}

bool IsErrorLevelLog(DWORD errorLevel)
{
	if(errorLevel > 0)
	{
		unsigned int nTestVaule = 1;
		nTestVaule = nTestVaule << (errorLevel - 1);
		if((nTestVaule & WriteLevelMask) > 0)
			return true;
		else 
			return false;
	}
	return false;
}

tstring BuildTimeString(int nTimes)
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	const int  nBufLength = MAX_PATH * sizeof(TCHAR);
	TCHAR	   strBuf[nBufLength];	
	tprintf(strBuf, nBufLength, _T("[%d:%d-%d-%d] [%d:%d:%d:%d]"), nTimes, systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
	return strBuf;
}