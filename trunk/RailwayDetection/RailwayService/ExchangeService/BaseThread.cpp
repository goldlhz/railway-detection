#include "StdAfx.h"
#include <process.h>
#include "BaseThread.h"


CBaseThread::CBaseThread(void)
{
	m_hThread = NULL;
	m_bIsTerminate = false;
}

CBaseThread::~CBaseThread(void)
{
	SafeTerminate();
}

bool CBaseThread::CreateThreadInstance(bool bIsRun)
{
	if(NULL == m_hThread)
	{
		if(bIsRun)
			m_hThread = (HANDLE)_beginthreadex(NULL, 0, &CBaseThread::Execute, (PVOID)this, 0, NULL);
		else
			m_hThread = (HANDLE)_beginthreadex(NULL, 0, &CBaseThread::Execute, (PVOID)this, CREATE_SUSPENDED, NULL);

		if(m_hThread)
		{
			DoWriteLogInfo(LOG_DEBUG, _T("CBaseThread::CreateThreadInstance(), 创建子线程成功"));
			return true;
		}

		WriteLogInfo(LOG_INFO, _T("CBaseThread::CreateThreadInstance(), 创建子线程出错,错误代码:%d"), errno);
	}

	return false;			
}

bool CBaseThread::ForceTerminate()
{
	if(m_hThread)
	{
		BOOL bError = FALSE;
		bError = ::TerminateThread(m_hThread, -1);
		if(bError)
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
			m_bIsTerminate = true;

			return true;
		}

		int nError = GetLastError();
		CloseHandle(m_hThread);
		m_hThread = NULL;
		m_bIsTerminate = true;

		WriteLogInfo(LOG_INFO, _T("CBaseThread::ForceTerminate(), 强制关闭线程出错,错误代码:%d"), nError);
		return false;
	}

	return true;
}

DWORD CBaseThread::ResumeThread()
{
	if(m_hThread)
	{
		int nError = 0;
		nError = ::ResumeThread(m_hThread);
		if(-1 != nError)
		{
			return nError;
		}

		nError = GetLastError();
		WriteLogInfo(LOG_INFO, _T("CBaseThread::ResumeThread(), 启动挂起的线程出错,错误代码:%d"), nError);
	}

	return -1;
}

DWORD CBaseThread::SuspendThread()
{
	if(m_hThread)
	{
		int nError = 0;
		nError = ::SuspendThread(m_hThread);
		if(-1 != nError)
		{
			return nError;
		}

		nError = GetLastError();
		WriteLogInfo(LOG_INFO, _T("CBaseThread::SuspendThread(), 挂起正在运行的线程出错,错误代码:%d"), nError);
	}

	return -1;
}

void  CBaseThread::SafeTerminate()
{
	if(m_hThread)
	{
		WaitForSingleObject(m_hThread, 2000);
		CloseHandle(m_hThread);
		m_bIsTerminate = true;
		m_hThread = NULL;
	}
}

unsigned int WINAPI CBaseThread::Execute(PVOID pArguments)
{
	CBaseThread * tThread = (CBaseThread*)pArguments;
	if(tThread)
	{
		return tThread->ExecuteThread();
	}

	return 0;
}
