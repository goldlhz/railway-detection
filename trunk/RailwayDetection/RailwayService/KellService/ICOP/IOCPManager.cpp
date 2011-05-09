#include "stdafx.h"
#include "IOCPManager.h"
#include "LogPluginManager.h"

CIOCPManager::CIOCPManager(void)
{
	m_hInstance = NULL;
	m_strComLine = _T("");
	m_strGolbalEventName = _T("Global\\Info_ExchangeServer_Event");
}

CIOCPManager::~CIOCPManager(void)
{
	UnInitGlobalEnvironment();
}

void CIOCPManager::InitManagerPar(HINSTANCE hInstance, LPTSTR lpCmdLine)
{
	ASSERT(hInstance);
	ASSERT(lpCmdLine);

	m_hInstance = hInstance;
	m_strComLine = lpCmdLine;
}

void CIOCPManager::RunService()
{
	if(InitGlobalEnvironment(m_hInstance))
	{
		HANDLE hWaitEvent = NULL;

		hWaitEvent = CreateEvent(NULL, TRUE, FALSE, m_strGolbalEventName.c_str());
		if(NULL != hWaitEvent)
		{
			if(ERROR_ALREADY_EXISTS == GetLastError())
			{
				DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunService(), �Ѿ���һ��������ʵ����������"));
			}
			else
			{
				if(CBaseSocket::InitSocketLib())
				{
					RunListenManager(hWaitEvent);
					CBaseSocket::UnInitSocketLib();
				}
			}
			CloseHandle(hWaitEvent);
		}
		UnInitGlobalEnvironment();
	}
}

void CIOCPManager::PostCloseMSG()
{
	HANDLE hWaitEvent = NULL;
	hWaitEvent = CreateEvent(NULL, TRUE, FALSE, m_strGolbalEventName.c_str());
	if(hWaitEvent)
	{
		if(ERROR_ALREADY_EXISTS == GetLastError())
		{
			SetEvent(hWaitEvent);
		}
		CloseHandle(hWaitEvent);
	}
}

void CIOCPManager::UnInitGlobalEnvironment()
{
	UnInitLogPluginManager();
}

bool CIOCPManager::InitGlobalEnvironment(HINSTANCE hInstance)
{
	InitLogPluginManager(_T("KellService"));
	m_GobalConfig.InitGoblaConfig(hInstance);

	return true;
}

void CIOCPManager::RunListenManager(HANDLE hWaitEvent)
{
	if(NULL != hWaitEvent)
	{
		if(m_listenThread.BeginThread(false))
		{
			if(m_listenThread.InitListenThread(&m_GobalConfig))
			{
				if(-1 != m_listenThread.ResumeThread())
				{
					// �ȴ�����ϵͳ�˳���Ϣ
					WaitForSingleObject(hWaitEvent, INFINITE);
					DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::OpenListenManager(), ���յ��˳�ϵͳ�¼�����ʼ�ر�ϵͳ"));
				}

				// �ͷ�ϵͳ��Դ
				m_listenThread.UnInitListenThread();
				return;
			}
		}

		DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::OpenListenManager(), ��ʼ�������߳�ʵ������"));
		return;
	}
}
