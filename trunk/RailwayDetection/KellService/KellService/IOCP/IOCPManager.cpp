#include "stdafx.h"
#include "LogPluginManager.h"
#include "IOCPManager.h"

CIOCPManager::CIOCPManager(void)
	: m_strGlbEvent(_T("Global\\KellService_Event"))
{
	m_hInstance = NULL;
	m_strComLine = _T("");
}

CIOCPManager::~CIOCPManager(void)
{
	UnInitGlbEnvironment();
}

void CIOCPManager::InitSysParam(HINSTANCE hInstance, LPTSTR lpCmdLine)
{
	ASSERT(hInstance);
	ASSERT(lpCmdLine);

	m_hInstance = hInstance;
	m_strComLine = lpCmdLine;
}

void CIOCPManager::RunService()
{
	ASSERT(m_hInstance);

	if(InitGlbEnvironment(m_hInstance))
	{
		HANDLE hWaitEvent = NULL;

		hWaitEvent = CreateEvent(NULL, TRUE, FALSE, m_strGlbEvent.c_str());
		if(NULL == hWaitEvent)
		{
			DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunService(), ����ϵͳ�������¼�����"));
			UnInitGlbEnvironment();
			return;
		}

		if(ERROR_ALREADY_EXISTS == GetLastError())
		{
			DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunService(), �Ѿ���һ��������ʵ����������"));
		}
		else
		{
			RunListener(hWaitEvent);
		}

		DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunService(), RunListener�Ѿ��˳��������Ѿ�ֹͣ"));
		CloseHandle(hWaitEvent);
		UnInitGlbEnvironment();
	}
}

void CIOCPManager::PostCloseMSG()
{
	HANDLE hWaitEvent = NULL;
	hWaitEvent = CreateEvent(NULL, TRUE, FALSE, m_strGlbEvent.c_str());
	if(hWaitEvent)
	{
		if(ERROR_ALREADY_EXISTS == GetLastError())
		{
			SetEvent(hWaitEvent);
		}
		CloseHandle(hWaitEvent);
	}
}

void CIOCPManager::StopService()
{
	HANDLE hWaitEvent = NULL;

	hWaitEvent = CreateEvent(NULL, TRUE, FALSE, m_strGlbEvent.c_str());
	if(NULL != hWaitEvent)
	{
		if(ERROR_ALREADY_EXISTS == GetLastError())
		{
			SetEvent(hWaitEvent);
		}
	}
}
bool CIOCPManager::InitGlbEnvironment(HINSTANCE hInstance)
{
	InitLogPluginManager(_T("KellService"));

	m_GobalConfig.InitGlbConfig(hInstance);

	if(CBaseSocket::InitSocketLib())
	{
		DoWriteLogInfo(LOG_DEBUG, _T("CIOCPManager::InitGlbEnvironment(), ��ʼ��ȫ�ֻ�����Ϣ���"));
		return true;
	}

	DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::InitGlbEnvironment(), ��ʼ��ȫ�ֻ�����Ϣʧ��"));
	return false;
}

void CIOCPManager::UnInitGlbEnvironment()
{
	CBaseSocket::UnInitSocketLib();

	UnInitLogPluginManager();
}

void CIOCPManager::RunListener(HANDLE hWaitEvent)
{
	if(NULL == hWaitEvent)
	{
		DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunListener(), �յ�һ����Ч�ĵȴ��¼����"));
		return;
	}

	if(!m_listenThread.BeginThread(false))
	{
		DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunListener(), ���������߳�ʧ��"));
		return;
	}

	if(!m_listenThread.InitListener(&m_GobalConfig))
	{
		DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunListener(), ��ʼ�������̲߳���ʧ��"));
		return;
	}

	if(-1 == m_listenThread.ResumeThread())
	{
		m_listenThread.UnInitListener();
		DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunListener(), ���������߳�ʵ������"));
		return;
	}	
	DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunListener(), �����������ɹ�"));

	WaitForSingleObject(hWaitEvent, INFINITE);
	m_listenThread.UnInitListener();

	DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunListener(), ���յ��˳�ϵͳ�¼�����ʼ�ر�ϵͳ"));
	return;
}
