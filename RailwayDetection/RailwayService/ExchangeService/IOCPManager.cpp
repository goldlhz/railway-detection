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
				DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunService(), 已经有一个服务器实例在运行了"));
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

	// 卸载日志系统
	UnInitLogPluginManager();
}

bool CIOCPManager::InitGlobalEnvironment()
{
	// 初始化日志系统
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
					// 等待接收系统退出消息
					WaitForSingleObject(hWaitEvent, INFINITE);
					DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::OpenListenManager(), 接收到退出系统事件，开始关闭系统"));

					// 释放系统资源
					m_sltListenThread.UnInitListenThreadInstance();

					return;
				}
			}
		}

		DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::OpenListenManager(), 初始化监听线程实例出错"));
		return;
	}
}