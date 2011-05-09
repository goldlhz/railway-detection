#include "StdAfx.h"
#include "WorkThread.h"
#include "SocketPool.h"
#include "MemoryPool.h"
#include "BaseSocket.h"

CWorkThread::CWorkThread(void)
{
	m_bIsInitWorkThread = false;
	m_hCompletionPort = NULL;
	m_hRevcNotify = NULL;
	m_pBaseSocket = NULL;
	m_pSocketPool = NULL;
	m_pMemoryPool = NULL;
}

CWorkThread::~CWorkThread(void)
{
	m_bIsInitWorkThread = false;
}

unsigned int CWorkThread::ExecuteThread()
{
	ASSERT(m_hCompletionPort);
	ASSERT(m_hRevcNotify);
	ASSERT(m_pBaseSocket);
	ASSERT(m_pSocketPool);
	ASSERT(m_pMemoryPool);

	if(!m_bIsInitWorkThread)
	{
		// 没有对函数进行初始化
		return 0;
	}

	BOOL  bTempTest = FALSE;
	DWORD dNumberOfBytes = 0;

	LPOverKeyPire pireOverLappedex;
	LPCompKey     pireCompletionKey;

	while(!m_bIsTerminate)
	{
		m_pSocketPool->RecycleSocket();
		m_pMemoryPool->RecycleMemory();

		// 读取网络数据
		bTempTest = GetQueuedCompletionStatus(
			m_hCompletionPort,
			&dNumberOfBytes,
			(PULONG_PTR)(&pireCompletionKey),
			(LPOVERLAPPED *)(&pireOverLappedex),
			INFINITE
			);

		if(-1 == dNumberOfBytes)
		{
			DoWriteLogInfo(LOG_INFO, _T("CWorkThread::ExecuteThread(), 接收到退出线程的完成包,工作都线程退出"));
			return 0;
		}
		if(FALSE == bTempTest)
		{
			int nErrorCode = GetLastError();
			if(ERROR_NETNAME_DELETED == nErrorCode)
			{
				TRACE(("服务器GetQueuedCompletionStatus收到一个连接ERROR_NETNAME_DELETED\n"));
				WriteLogInfo(LOG_DEBUG, _T("CWorkThread::ExecuteThread(), 网络上有一个SOCKET发生错误，错误代码:%d"), nErrorCode);

				if(!pireCompletionKey)
				{
					m_pSocketPool->CloseSpecSocket(pireCompletionKey->keyOptSocket);
					m_pMemoryPool->ReleaseOverKeyPire(pireOverLappedex);
				}
				continue;
			}
			else
			{
				TRACE(("服务器GetQueuedCompletionStatus收到未知错误:%d\n"), nErrorCode);
				WriteLogInfo(LOG_DEBUG, _T("CWorkThread::ExecuteThread(), GetQueuedCompletionStatus返回一个未知错误:%d,但此时套接字好像还可以用,暂时不关闭套接字"), nErrorCode);
				continue;
			}
		}
		else if(0 == dNumberOfBytes)
		{
			if(pireOverLappedex)
			{
				if(pireOverLappedex->pireOverLappedex.wsaOptType == CT_REVC || pireOverLappedex->pireOverLappedex.wsaOptType == CT_SEND)
				{
					TRACE(("有客端连接安全断开\n"));

					m_pSocketPool->CloseSpecSocket(pireOverLappedex->pireCompletionKey.keyOptSocket);
					m_pMemoryPool->ReleaseOverKeyPire(pireOverLappedex);

					continue;
				}
			}
		}

		ASSERT(pireOverLappedex);
		if(pireOverLappedex)
		{
			switch(pireOverLappedex->pireOverLappedex.wsaOptType)
			{
			case CT_REVC:
				DealRevcDate(dNumberOfBytes, pireOverLappedex);
				break;

			case CT_SEND:
				DealSendDate(dNumberOfBytes, pireOverLappedex);	
				break;

			case CT_ACCP:
				DealAccpDate(dNumberOfBytes, pireOverLappedex);
				break;

			case CT_PREPREVC:
				DealPrepRevcDate(dNumberOfBytes, pireOverLappedex);
				break;

			default:
				break;
			}
		}		
	}

	return 0;
}

bool CWorkThread::InitWorkThreadInfo(
	HANDLE hCompletionPort, 
	HANDLE hRevcNotify,
	CBaseSocket * pBaseSocket, 
	CSocketPool * pSocketPool, 
	CMemoryPool * pMemoryPool
	)
{
	ASSERT(hCompletionPort);
	ASSERT(hRevcNotify);
	ASSERT(pBaseSocket);
	ASSERT(pSocketPool);
	ASSERT(pMemoryPool);

	if(!hCompletionPort)
		return false;
	m_hCompletionPort = hCompletionPort;

	if(!hRevcNotify)
		return false;
	m_hRevcNotify = hRevcNotify;

	if(!pBaseSocket)
		return false;
	m_pBaseSocket = pBaseSocket;

	if(!pSocketPool)
		return false;
	m_pSocketPool = pSocketPool;

	if(!pMemoryPool)
		return false;
	m_pMemoryPool = pMemoryPool;

	// 只有到所有值都确定为有效时才设置成初始化成功
	m_bIsInitWorkThread = true;
	return true;
}

void CWorkThread::DealRevcDate(DWORD dNumberOfBytes, LPOverKeyPire pKeyOverPire)
{
	ASSERT(m_hCompletionPort);
	ASSERT(m_pBaseSocket);
	ASSERT(m_hRevcNotify);
	ASSERT(m_pSocketPool);
	ASSERT(m_pMemoryPool);

	if(pKeyOverPire)
	{
		WriteLogInfo(LOG_ERR, _T("收到了一个测试用的数据包,大小为:%d"), dNumberOfBytes);
		PostRevcMSG(pKeyOverPire->pireOverLappedex.wsaClientSocket, pKeyOverPire);

		return;
	}
}

void CWorkThread::DealSendDate(DWORD dNumberOfBytes, LPOverKeyPire pKeyOverPire)
{
	ASSERT(m_hCompletionPort);
	ASSERT(m_pBaseSocket);
	ASSERT(m_hRevcNotify);
	ASSERT(m_pSocketPool);
	ASSERT(m_pMemoryPool);

	if(pKeyOverPire)
	{
		PostRevcMSG(pKeyOverPire->pireOverLappedex.wsaClientSocket, pKeyOverPire);
		return;
	}
}

void CWorkThread::DealAccpDate(DWORD dNumberOfBytes, LPOverKeyPire pKeyOverPire)
{
	ASSERT(m_hCompletionPort);
	ASSERT(m_pBaseSocket);
	ASSERT(m_hRevcNotify);
	ASSERT(m_pSocketPool);
	ASSERT(m_pMemoryPool);

	SetEvent(m_hRevcNotify);
	if(pKeyOverPire)
	{
		int nErrorCode = 0;
		DWORD dwNumberOfBytesRecvd = 0;
		DWORD dwFlags = 0;
		int nLocalAddrLength = 0;
		int nRmoteAddrLength = 0;
		LPSOCKADDR pLocalAddr = NULL;
		LPSOCKADDR pRemoteAddr = NULL;

		m_pBaseSocket->GetAcceptExSockaddrs(
			pKeyOverPire->pireOverLappedex.wsaAcceptBuffer,
			0,
			ADDRESS_LENGTH,
			ADDRESS_LENGTH,
			(SOCKADDR **)&pLocalAddr,
			&nLocalAddrLength,
			(SOCKADDR **)&pRemoteAddr,
			&nRmoteAddrLength);

		GetLocalTime(&(pKeyOverPire->pireCompletionKey.keyLinkTime));
		memcpy(&(pKeyOverPire->pireCompletionKey.keyClientAddr), pRemoteAddr, sizeof(sockaddr_in));
		pKeyOverPire->pireCompletionKey.keyOptSocket = pKeyOverPire->pireOverLappedex.wsaClientSocket;

		SOCKET scServerSocket = m_pBaseSocket->GetSocket();
		SOCKET scClientSocket = pKeyOverPire->pireOverLappedex.wsaClientSocket;		
		
		if (SOCKET_ERROR == setsockopt(
			scClientSocket, 
			SOL_SOCKET, 
			SO_UPDATE_ACCEPT_CONTEXT, 
			(char *)&scServerSocket, 
			sizeof (SOCKET)))
		{
			nErrorCode = GetLastError();
			TRACE(("设置客户端套接字的属性跟服务端套接字一样时出错:%d\n"), nErrorCode);
			WriteLogInfo(LOG_INFO, _T("CWorkThread::DealAccpDate(), 设置客户端套接字的属性跟服务端套接字一样时出错,错误代码:%d"), nErrorCode);

			m_pSocketPool->CloseSpecSocket(scClientSocket);
			m_pMemoryPool->ReleaseOverKeyPire(pKeyOverPire);

			return;
		}

		HANDLE hCompletion = NULL;
		hCompletion = CreateIoCompletionPort((HANDLE)scClientSocket, m_hCompletionPort, (ULONG_PTR)&(pKeyOverPire->pireCompletionKey), 0);
		if(hCompletion)
		{
			PostRevcMSG(scClientSocket, pKeyOverPire);
			return;
		}	
		else
		{
			nErrorCode = GetLastError();
			TRACE(("将收到的套接字绑定到完成端口出错:%d\n"), nErrorCode);

			WriteLogInfo(LOG_INFO, _T("CWorkThread::DealAccpDate(), 接收到的套接字与完成端口绑定时出错,错误代码:%d"), nErrorCode);

			m_pSocketPool->CloseSpecSocket(scClientSocket);
			m_pMemoryPool->ReleaseOverKeyPire(pKeyOverPire);

			return;
		}
	}
}

void CWorkThread::DealPrepRevcDate(DWORD dNumberOfBytes,LPOverKeyPire pKeyOverPire)
{

}

bool CWorkThread::PostRevcMSG(SOCKET scOptSocket, LPOverKeyPire pKeyOverPire)
{
	ASSERT(pKeyOverPire);

	if(!pKeyOverPire)
		return false;

	int nErrorCode = 0;
	DWORD dwFlags = 0;
	DWORD dwNumberOfBytesRecvd = 0;

	memset(pKeyOverPire, 0x00, sizeof(OVERLAPPED));

	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = BUFFER_SIZE_TO_CLIENT;
	pKeyOverPire->pireOverLappedex.wsaWSABUF.buf = pKeyOverPire->pireOverLappedex.wsaBuffer;
	pKeyOverPire->pireOverLappedex.wsaOptType = CT_REVC;

	nErrorCode = WSARecv(
		scOptSocket,
		&(pKeyOverPire->pireOverLappedex.wsaWSABUF),
		1,
		&dwNumberOfBytesRecvd,
		&dwFlags,
		(LPWSAOVERLAPPED)pKeyOverPire,
		NULL
		);

	if(SOCKET_ERROR == nErrorCode)
	{
		nErrorCode = WSAGetLastError();
		if(WSA_IO_PENDING == nErrorCode)
		{
			// 投递成功,什么都不做
			return true;
		}
		else if(WSAENOBUFS == nErrorCode)
		{
			// 资源不足
			TRACE(("投递WSARecv消息资源不足出错,错误代码:%d\n"), nErrorCode);
			WriteLogInfo(LOG_INFO, _T("CWorkThread::DealAccpDate(), 投递WSARecv消息资源不足出错,错误代码:%d"), nErrorCode);

			m_pSocketPool->CloseSpecSocket(scOptSocket);
			m_pMemoryPool->ReleaseOverKeyPire(pKeyOverPire);	

			return false;
		}
		else
		{
			TRACE(("投递WSARecv消息发生未知时出错,错误代码:%d\n"), nErrorCode);
			WriteLogInfo(LOG_INFO, _T("CWorkThread::DealAccpDate(), 投递WSARecv消息发生未知时出错,错误代码:%d"), nErrorCode);

			m_pSocketPool->CloseSpecSocket(scOptSocket);
			m_pMemoryPool->ReleaseOverKeyPire(pKeyOverPire);	

			return false;
		}
	}
	return true;
}

bool CWorkThread::PostSendMSG(SOCKET scOptSocket, LPOverKeyPire pKeyOverPire)
{
	ASSERT(pKeyOverPire);

	if(!pKeyOverPire)
		return false;

	int nErrorCode = 0;
	DWORD dwFlags = 0;
	DWORD dwNumberOfBytesRecvd = 0;

	memset(pKeyOverPire, 0x00, sizeof(OVERLAPPED));
	pKeyOverPire->pireOverLappedex.wsaOptType = CT_SEND;

	nErrorCode = WSASend(
		scOptSocket,
		&(pKeyOverPire->pireOverLappedex.wsaWSABUF),
		1,
		&dwNumberOfBytesRecvd,
		dwFlags,
		(LPWSAOVERLAPPED)pKeyOverPire,
		NULL
		);

	if(SOCKET_ERROR == nErrorCode)
	{
		nErrorCode = WSAGetLastError();
		if(WSA_IO_PENDING == nErrorCode)
		{
			// 投递成功,什么都不做
			return true;
		}
		else if(WSAENOBUFS == nErrorCode)
		{
			// 资源不足
			TRACE(("投递WSARecv消息资源不足出错,错误代码:%d\n"), nErrorCode);
			WriteLogInfo(LOG_INFO, _T("CWorkThread::DealAccpDate(), 投递WSARecv消息资源不足出错,错误代码:%d"), nErrorCode);

			m_pSocketPool->CloseSpecSocket(scOptSocket);
			m_pMemoryPool->ReleaseOverKeyPire(pKeyOverPire);	

			return false;
		}
		else
		{
			TRACE(("投递WSARecv消息发生未知时出错,错误代码:%d\n"), nErrorCode);
			WriteLogInfo(LOG_INFO, _T("CWorkThread::DealAccpDate(), 投递WSARecv消息发生未知时出错,错误代码:%d"), nErrorCode);

			m_pSocketPool->CloseSpecSocket(scOptSocket);
			m_pMemoryPool->ReleaseOverKeyPire(pKeyOverPire);	

			return false;
		}
	}
	return true;
}
