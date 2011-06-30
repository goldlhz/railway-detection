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
			DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunService(), 创建系统服务检测事件出错"));
			UnInitGlbEnvironment();
			return;
		}

		if(ERROR_ALREADY_EXISTS == GetLastError())
		{
			DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunService(), 已经有一个服务器实例在运行了"));
		}
		else
		{
			RunListener(hWaitEvent);
		}

		DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunService(), RunListener已经退出，服务已经停止"));
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
		DoWriteLogInfo(LOG_DEBUG, _T("CIOCPManager::InitGlbEnvironment(), 初始化全局环境信息完成"));
		return true;
	}

	DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::InitGlbEnvironment(), 初始化全局环境信息失败"));
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
		DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunListener(), 收到一个无效的等待事件句柄"));
		return;
	}

	if(!m_listenThread.BeginThread(false))
	{
		DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunListener(), 创建监听线程失败"));
		return;
	}

	if(!m_listenThread.InitListener(&m_GobalConfig))
	{
		DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunListener(), 初始化监听线程参数失败"));
		return;
	}

	if(-1 == m_listenThread.ResumeThread())
	{
		m_listenThread.UnInitListener();
		DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunListener(), 启动监听线程实例出错"));
		return;
	}	
	DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunListener(), 服务器开启成功"));

	WaitForSingleObject(hWaitEvent, INFINITE);
	m_listenThread.UnInitListener();

	DoWriteLogInfo(LOG_INFO, _T("CIOCPManager::RunListener(), 接收到退出系统事件，开始关闭系统"));
	return;
}
