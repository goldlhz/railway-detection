#include "StdAfx.h"
#include "SocketListenThread.h"
#include "CommonFunction.h"
#include "GobalConfig.h"
#include "PoolManager.h"


CSocketListenThread::CSocketListenThread(void)
{
	m_hJudgeEvent[0] = NULL;									// acceptex不足事件
	m_hJudgeEvent[1] = NULL;									// 接收到一个连接事件
	m_hCompletionPort = NULL;
}

CSocketListenThread::~CSocketListenThread(void)
{
}

unsigned int CSocketListenThread::ExecuteThread()
{
	ASSERT(m_hJudgeEvent[0]);
	ASSERT(m_hJudgeEvent[1]);

	int nError = 0;
	int nPostCount = 0;

	CGobalConfig::POSTACCEPTMODE PostMode = CGobalConfig::GetGobalConfig().GetPostMode();
	if(CGobalConfig::POST_BYKERNAL == PostMode)
		nPostCount = CCommonFunction::GetCurrentKernalCount() * CGobalConfig::GetGobalConfig().GetPostTiems();		
	else
		nPostCount = CGobalConfig::GetGobalConfig().GetPostMaxSize();

	for (int nIndex = 0; nIndex < nPostCount; ++nIndex)
	{
		if(!PostAcceptExMSG())
		{
			ReleaseAllSource();
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), 第一波次投递AcceptEx消息出错,监听管理线程退出"));
			return 0;
		}
	}

	while(!m_bIsTerminate)
	{
		int nIndex = ::WSAWaitForMultipleEvents(JUDGEHANDLESIZE, m_hJudgeEvent, FALSE, WSA_INFINITE, FALSE);

		if(WSA_WAIT_FAILED == nIndex)
		{
			ReleaseAllSource();
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), 监听管理线程中的WSAWaitForMultipleEvents()返回出错,线程退出"));
			return 0;
		}	

		nIndex = nIndex - WAIT_OBJECT_0;			
		if(nIndex >= 0)
		{
			WSANETWORKEVENTS networkEvent;
			switch (nIndex)
			{
			case 0:
				{
					::WSAEnumNetworkEvents(m_BaseSocket.GetSocket(), m_hJudgeEvent[0], &networkEvent);
					if(networkEvent.lNetworkEvents & FD_ACCEPT)
					{
						for (int nIndex = 0; nIndex < nPostCount; ++nIndex)
						{
							if(!PostAcceptExMSG())
							{
								DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), AcceptEx不足时再投递AcceptEx出错,监听管理线程休息2秒后再投递"));
								SleepEx(2000, TRUE);
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
					}
				}
				break;
			default: 
				{
					WriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), WSAWaitForMultipleEvents()返回了未知的消息,代码为:%d"), nIndex + WAIT_OBJECT_0);
				}
				break;
			}
		}
	}

	return 0;
}

bool CSocketListenThread::InitListenThreadInstance()
{
	int nPort = CGobalConfig::GetGobalConfig().GetServerPort();

	if(CreateIOCPHandle())
	{
		if(CreateServerSocket(nPort))
		{
			if(m_MemoryPoolManager.InitMemoryPool())
			{
				DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::InitListenThreadInstance(), 初始化内存池成功"));

				if(m_SocketPoolManager.InitSocketPool())
				{
					DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::InitListenThreadInstance(), 初始化套接字池成功"));

					HANDLE hHandle = CreateIoCompletionPort((HANDLE)m_BaseSocket.GetSocket(), m_hCompletionPort, (ULONG_PTR)NULL, NULL);
					if(hHandle)
					{
						DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::InitListenThreadInstance(), 绑定监听套接字到完成端口时成功"));

						if(SOCKET_ERROR == WSAEventSelect(m_BaseSocket.GetSocket(), m_hJudgeEvent[0], FD_ACCEPT))
						{
							int nError = WSAGetLastError();
							WriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), 注册FD_ACCEPT事件错误, 错误代码:%d"), nError);
						}
						else
						{
							DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::InitListenThreadInstance(), 注册FD_ACCEPT事件成功"));

							if(CreateServerThreadPool())
							{
								DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::InitListenThreadInstance(), 初始化监听线程完成"));
								return true;
							}
						}
					}
					else
					{
						int nError = GetLastError();
						WriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), 绑定监听套接字到完成端口时出错,错误代码:%d"), nError);
					}
				}

				m_MemoryPoolManager.ClearMemoryPool();
			}
			CloseServerSocket();
		}
		CloseIOCPHandle();
	}

	DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), 初始化内存池出错"));
	return false;
}

void CSocketListenThread::UnInitListenThreadInstance()
{
	ReleaseAllSource();
	SafeTerminate();
}

bool CSocketListenThread::CreateIOCPHandle()
{
	if(!m_hCompletionPort)
	{
		m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if(m_hCompletionPort)
		{
			DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateIOCPHandle(), 创建完成端口成功"));

			m_hJudgeEvent[0] = WSACreateEvent();
			if(WSA_INVALID_EVENT != m_hJudgeEvent[0])
			{
				DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateIOCPHandle(), 创建关联于接收连接事件成功"));

				m_hJudgeEvent[1] = CreateEvent(NULL, FALSE, FALSE, _T("Global\\Notify_Accept_Link"));
				if(m_hJudgeEvent[1])
				{
					DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateIOCPHandle(), 创建通知接收到一个连接事件成功"));
					return true;
				}

				int nError = GetLastError();
				WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), 创建通知接收到一个连接事件出错，错误代码:%d"), nError);
				return false;
			}

			int nError = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), 创建关联于接收连接事件出错，错误代码:%d"), nError);
			return false;
		}

		int nError = GetLastError();
		WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), 创建完成端口出错，错误代码:%d"), nError);
		return false;
	}
	return true;
}

bool CSocketListenThread::CreateServerSocket(int nPort)
{
	if(m_hCompletionPort)
	{
		if(m_BaseSocket.CreateListenSocket(nPort))
		{
			DoWriteLogInfo(LOG_DEBUG, _T("创建服务器套接字成功"));
			return true;
		}

		DoWriteLogInfo(LOG_DEBUG, _T("创建服务器套接字失败"));
	}
	return false;
}

bool CSocketListenThread::CreateServerThreadPool()
{
	if(m_hCompletionPort)
	{
		if(m_ServerThreadPool.CreateServerThreads(m_hCompletionPort, 
			m_hJudgeEvent[1], 
			&m_BaseSocket, 
			&m_SocketPoolManager, 
			&m_MemoryPoolManager))

		{
			DoWriteLogInfo(LOG_DEBUG, _T("创建工作者线程池成功"));
			return true;
		}

		DoWriteLogInfo(LOG_DEBUG, _T("创建工作者线程池失败"));
	}
	return false;
}

bool CSocketListenThread::CloseServerThreadPool()
{
	return m_ServerThreadPool.CloseServerThreads();
}

bool CSocketListenThread::CloseServerSocket()
{
	return m_BaseSocket.CloseSocket();
}

bool CSocketListenThread::CloseIOCPHandle()
{
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

bool CSocketListenThread::PostAcceptExMSG()
{
	// 断言 
	ASSERT(m_hCompletionPort);

	BOOL   bTempTest = FALSE;
	DWORD  dReceviedSize = 0;
	SOCKET scSocket = INVALID_SOCKET;
	KeyOverPire * pKeyOverPire = NULL;

	scSocket     = m_SocketPoolManager.GetNewSocket();
	pKeyOverPire = m_MemoryPoolManager.GetNewKeyOverPire();

	if(scSocket != INVALID_SOCKET && pKeyOverPire != NULL)
	{
		pKeyOverPire->pireCompletionKey.keySocket = m_BaseSocket.GetSocket();
		pKeyOverPire->pireOverLappedex.wsaClientSocket = scSocket;
		pKeyOverPire->pireOverLappedex.wsaWSABuf.buf = (CHAR *)pKeyOverPire->pireOverLappedex.wsaBuffer;
		pKeyOverPire->pireOverLappedex.wsaOptType = CT_ACCP;

		bTempTest = m_BaseSocket.AcceptEx(
			m_BaseSocket.GetSocket(), 
			scSocket,
			pKeyOverPire->pireOverLappedex.wsaAcceptBuffer,
			0,
			sizeof(sockaddr_in) + 16,
			sizeof(sockaddr_in) + 16,
			&(dReceviedSize),
			&(pKeyOverPire->pireOverLappedex.wsaOverlapped)
			);

		if(!bTempTest)
		{
			int nError = WSAGetLastError();
			if(ERROR_IO_PENDING == nError)
				return true;

			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::PostAcceptEx(), 监听套接字置为监听状态时出错，错误代码:%d"), nError);
			return false;
		}

		return true;
	}

	DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::PostAcceptEx(), 池管理器出错，无法分配可以使用的资源"));
	return false;
}

void CSocketListenThread::ReleaseAllSource()
{
	CloseServerThreadPool();
	CloseServerSocket();
	CloseIOCPHandle();
}