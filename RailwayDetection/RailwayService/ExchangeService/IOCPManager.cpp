#include "StdAfx.h"
#include "IOCPManager.h"
#include "GobalConfig.h"

CIOCPManager::CIOCPManager(void)
	: m_strGolbalEventName(_T("Global\\Info_ExchangeServer_Event"))
{
}

CIOCPManager::~CIOCPManager(void)
{
}

void CIOCPManager::RunService()
{
	if(InitGlobalEnvironment())
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
					OpenListenManager(hWaitEvent);
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

	// ж����־ϵͳ
	UnInitLogPluginManager();
}

bool CIOCPManager::InitGlobalEnvironment()
{
	// ��ʼ����־ϵͳ
	InitLogPluginManager(_T("RailwayService"));

	CGobalConfig::GetGobalConfig().InitGoblaConfig();

	return true;
}

void CIOCPManager::OpenListenManager(HANDLE hWaitEvent)
{
	if(NULL != hWaitEvent)
	{
		if(m_sltListenThread.CreateThreadInstance(false))
		{
			if(m_sltListenThread.InitListenThreadInstance())
			{
				if(-1 != m_sltListenThread.ResumeThread())
				{
					// �ȴ�����ϵͳ�˳���Ϣ
					WaitForSingleObject(hWaitEvent, INFINITE);
					DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::OpenListenManager(), ���յ��˳�ϵͳ�¼�����ʼ�ر�ϵͳ"));

					// �ͷ�ϵͳ��Դ
					m_sltListenThread.UnInitListenThreadInstance();

					return;
				}
			}
		}

		DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::OpenListenManager(), ��ʼ�������߳�ʵ������"));
		return;
	}
}