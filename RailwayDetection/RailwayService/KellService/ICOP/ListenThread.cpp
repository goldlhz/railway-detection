#include "StdAfx.h"
#include "ListenThread.h"
#include "GobalConfig.h"
#include "CommonFunction.h"

#define ACCEPTEX_FAIL_WAIT_TIME									1000

CListenThread::CListenThread(void)
{
	m_hJudgeEvent[0] = NULL;									// acceptex不足事件
	m_hJudgeEvent[1] = NULL;									// 接收到一个连接到来
	m_hCompletionPort = NULL;
	m_pGobalConfig = NULL;
}

CListenThread::~CListenThread(void)
{
	ReleaseAllSource();
}

bool CListenThread::InitListenThread(CGobalConfig * pGobalConfig)
{
	ASSERT(pGobalConfig);

	m_pGobalConfig = pGobalConfig;
	if(m_pGobalConfig)
	{
		int nListenPort = m_pGobalConfig->GetListenPort();

		if(!CreateIOCPHandle())
		{
			// 创建IOCP架构所要用的句柄出错
			return false;
		}
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThread(), CreateIOCPHandle()调用成功"));

		if(!CreateListenSocket(nListenPort))
		{
			// 监听套接字创建失败
			CloseIOCPHandle();
			return false;
		}
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThread(), CreateListenSocket()调用成功"));

		if(!m_MemoryPool.InitMemoryPool(m_pGobalConfig))
		{
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThread(), 初始化内存池错误"));

			CloseListenSocket();
			CloseIOCPHandle();

			return false;
		}
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThread(), 初始化内存池成功"));

		if(!m_SocketPool.InitSocketPool(m_pGobalConfig))
		{
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThread(), 初始化套接字池错误"));

			CloseListenSocket();
			CloseIOCPHandle();

			return false;
		}
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThread(), 初始化套接字池成功"));

		if(SOCKET_ERROR == WSAEventSelect(m_sListenSocket.GetSocket(), m_hJudgeEvent[0], FD_ACCEPT))
		{
			int nErrorCode = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), 注册FD_ACCEPT事件错误, 错误代码:%d"), nErrorCode);

			CloseListenSocket();
			CloseIOCPHandle();

			return false;
		}
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), 注册FD_ACCEPT事件成功"));


		HANDLE hHandle = CreateIoCompletionPort((HANDLE)m_sListenSocket.GetSocket(), m_hCompletionPort, (ULONG_PTR)NULL, NULL);
		if(!hHandle)
		{
			int nErrorCode = GetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), 绑定监听套接字到完成端口时出错,错误代码:%d"), nErrorCode);

			CloseListenSocket();
			CloseIOCPHandle();

			return false;
		}
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), 绑定监听套接字到完成端口时成功"));

		if(CreateWorkThreadPool())
		{
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), 初始化监听线程完成"));
			return true;
		}

		CloseListenSocket();
		CloseIOCPHandle();
		return false;

	}

	return false;
}
void CListenThread::UnInitListenThread()
{
	ReleaseAllSource();
	SafeTerminate();
}
unsigned int CListenThread::ExecuteThread()
{
	ASSERT(m_hJudgeEvent[0]);
	ASSERT(m_hJudgeEvent[1]);
	ASSERT(m_pGobalConfig);

	if(m_pGobalConfig)
	{
		int nErrorCode = 0;
		int nPostCount = 0;
		CGobalConfig::POSTACCEPTMODE PostMode;

		PostMode = m_pGobalConfig->GetPostMode();

		if(CGobalConfig::POST_BYKERNAL == PostMode)
			nPostCount = CCommonFunction::GetKernalCount() * m_pGobalConfig->GetPostTiems();		
		else
			nPostCount = m_pGobalConfig->GetPostMaxSize();

		for (int nEventIndex = 0; nEventIndex < nPostCount; ++nEventIndex)
		{
			if(!PostAcceptExMSG())
			{
				ReleaseAllSource();
				DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), 第一波次投递AcceptEx消息出错,监听管理线程退出"));
				return 0;
			}
		}

		int nEventIndex = 0;
		WSANETWORKEVENTS networkEvent;

		while(!m_bIsTerminate)
		{			
			nEventIndex = ::WSAWaitForMultipleEvents(JUDGEHANDLESIZE, m_hJudgeEvent, FALSE, 60 * 1000, FALSE);

			if(WSA_WAIT_FAILED == nEventIndex)
			{
				nErrorCode = WSAGetLastError();
				ReleaseAllSource();
				WriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), 监听管理线程中的WSAWaitForMultipleEvents()返回出错,线程退出:%d"), nErrorCode);
				return 0;
			}	

			if(WSA_WAIT_TIMEOUT == nEventIndex)
			{
				// 可以此检测一些东西,现在先不用
				continue;
			}

			nEventIndex = nEventIndex - WAIT_OBJECT_0;			
			if(nEventIndex >= 0)
			{
				switch (nEventIndex)
				{
				case 0:
					{
						::WSAEnumNetworkEvents(m_sListenSocket.GetSocket(), m_hJudgeEvent[0], &networkEvent);
						if(networkEvent.lNetworkEvents & FD_ACCEPT)
						{
							static int nCount;
							for (nCount = 0; nCount < nPostCount; ++nCount)
							{
								if(!PostAcceptExMSG())
								{
									DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), AcceptEx不足时再投递AcceptEx出错,监听管理线程休息1秒后再投递"));
									SleepEx(ACCEPTEX_FAIL_WAIT_TIME, TRUE);
								}
							}
						}
					}
					break;
				case 1:
					{
						if(!PostAcceptExMSG())
						{
							DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), 接收到新连接后再增加AcceptEx投递时出错"));
							SleepEx(ACCEPTEX_FAIL_WAIT_TIME, TRUE);
						}
					}
					break;
				default: 
					{
						WriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), WSAWaitForMultipleEvents()返回了未知的消息,代码为:%d"), nEventIndex + WAIT_OBJECT_0);
					}
					break;
				}
			}
		}
	}

	return 0;
}

bool CListenThread::CreateIOCPHandle()
{
	if(!m_hCompletionPort)
	{
		int nErrorCode = 0;

		m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if(!m_hCompletionPort)
		{
			nErrorCode = GetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), 创建完成端口出错，错误代码:%d"), nErrorCode);
			return false;
		}
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), 创建完成端口成功"));

		m_hJudgeEvent[0] = WSACreateEvent();
		if(WSA_INVALID_EVENT == m_hJudgeEvent[0])
		{
			nErrorCode = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), 创建事件选择模型的事件出错，错误代码:%d"), nErrorCode);
			CloseHandle(m_hCompletionPort);

			return false;
		}
		DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateIOCPHandle(), 创建事件选择模型的事件成功"));

		m_hJudgeEvent[1] = CreateEvent(NULL, FALSE, FALSE, _T("Global\\Notify_Accept_Link"));
		if(NULL == m_hJudgeEvent[1])
		{
			nErrorCode = GetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), 创建通知接收到一个连接事件出错，错误代码:%d"), nErrorCode);

			CloseHandle(m_hJudgeEvent[0]);
			CloseHandle(m_hCompletionPort);

			return false;
		}
		DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateIOCPHandle(), 创建通知接收到一个连接事件成功"));
	}

	return true;
}

bool CListenThread::CreateListenSocket(int nPort)
{
	ASSERT(m_hCompletionPort);

	if(nPort < 1024 || nPort > 65535)
	{
		DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateListenSocket(), 配置文件中设置的服务器监听端口不在合法范围内"));
		return false;
	}

	if(m_hCompletionPort)
	{
		if(m_sListenSocket.CreateLocalSocket(nPort))
		{
			DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateListenSocket(), 创建服务器套接字成功"));
			return true;
		}

		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateListenSocket(), 创建服务器套接字失败"));
	}
	return false;
}

bool CListenThread::CreateWorkThreadPool()
{
	if(m_hCompletionPort)
	{
		if(m_WorkThreadPool.CreateWorkThreads(
			m_hCompletionPort, 
			m_hJudgeEvent[1], 
			&m_sListenSocket, 
			&m_SocketPool, 
			&m_MemoryPool))

		{
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateWorkThreadPool(), 创建工作者线程池成功"));
			return true;
		}

		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateWorkThreadPool(), 创建工作者线程池失败"));
	}
	return false;
}

bool CListenThread::CloseWorkThreadPool()
{
	return m_WorkThreadPool.CloseWorkThreads();
}

bool CListenThread::CloseListenSocket()
{
	return m_sListenSocket.CloseLocalSocket();
}

bool CListenThread::CloseIOCPHandle()
{
	ASSERT(m_hCompletionPort);

	if(m_hCompletionPort)
	{
		CloseHandle(m_hJudgeEvent[1]);
		m_hJudgeEvent[1] = NULL;

		CloseHandle(m_hJudgeEvent[0]);
		m_hJudgeEvent[0] = NULL;

		CloseHandle(m_hCompletionPort);
		m_hCompletionPort = NULL;

		DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CloseIOCPHandle(), 关闭完成端口"));
	}
	return true;
}

bool CListenThread::PostAcceptExMSG()
{
	BOOL   bTempTest = FALSE;
	DWORD  dReceviedSize = 0;
	SOCKET sClientSocket = INVALID_SOCKET;
	LPOverKeyPire pOverKeyPire = NULL;

	sClientSocket= m_SocketPool.GetNewSocket();
	pOverKeyPire = m_MemoryPool.GetNewOverKeyPire();

	if((INVALID_SOCKET != sClientSocket) && (NULL != pOverKeyPire))
	{
		pOverKeyPire->pireCompletionKey.keyOptSocket   = m_sListenSocket.GetSocket();
		pOverKeyPire->pireOverLappedex.wsaClientSocket = sClientSocket;
		pOverKeyPire->pireOverLappedex.wsaOptType      = CT_ACCP;

		bTempTest = m_sListenSocket.AcceptEx(
			m_sListenSocket.GetSocket(), 
			sClientSocket,
			pOverKeyPire->pireOverLappedex.wsaAcceptBuffer,
			0,
			ADDRESS_LENGTH,
			ADDRESS_LENGTH,
			&(dReceviedSize),
			(LPOVERLAPPED)pOverKeyPire
			);

		if(!bTempTest)
		{
			int nErrorCode = WSAGetLastError();
			if(ERROR_IO_PENDING == nErrorCode)
			{
				return true;
			}
			else if(WSAENOBUFS == nErrorCode)
			{
				TRACE(("accpetex 返回一个资源不足错误:%d\n"), nErrorCode);
				WriteLogInfo(LOG_INFO, _T("CSocketListenThread::PostAcceptEx(), 监听线程投递AccpetEX时系统资源不足，错误代码:%d"), nErrorCode);

				m_SocketPool.CloseSpecSocket(sClientSocket);
				m_MemoryPool.ReleaseOverKeyPire(pOverKeyPire);
				
				return false;
			}
			else
			{
				m_SocketPool.CloseSpecSocket(sClientSocket);
				m_MemoryPool.ReleaseOverKeyPire(pOverKeyPire);

				TRACE(("accpetex 返回一个未知错误:%d\n"), nErrorCode);
				WriteLogInfo(LOG_INFO, _T("CSocketListenThread::PostAcceptEx(), 监听线程投递AccpetEX时出错，错误代码:%d"), nErrorCode);
				return false;
			}
		}

		return true;
	}

	DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::PostAcceptEx(), 池管理器出错，无法分配可以使用的资源"));
	return false;
}

void CListenThread::ReleaseAllSource()
{
	CloseWorkThreadPool();
	CloseListenSocket();
	CloseIOCPHandle();
}
