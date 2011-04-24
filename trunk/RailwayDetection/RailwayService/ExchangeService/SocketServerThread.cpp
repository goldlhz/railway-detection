#include "StdAfx.h"
#include "BaseSocket.h"
#include "SocketServerThread.h"
#include "PoolManager.h"

CSocketServerThread::CSocketServerThread(void)
{
	m_hCompletionPort = NULL;
	m_hRevcSocket = NULL;
	m_pBaseSocket = NULL;
	m_pSocketPoolManager = NULL;
	m_pMemoryPoolManager = NULL;
}

CSocketServerThread::~CSocketServerThread(void)
{

}

unsigned int CSocketServerThread::ExecuteThread()
{
	ASSERT(m_hCompletionPort);
	ASSERT(m_pBaseSocket);
	ASSERT(m_pSocketPoolManager);
	ASSERT(m_pMemoryPoolManager);

	BOOL  bTempTest = FALSE;
	DWORD dNumberOfBytes = 0;

	WsaOverLappedEX * pireOverLappedex;
	CompletionKey   * pireCompletionKey;

	while(!m_bIsTerminate)
	{
		// 读取网络数据
		bTempTest = GetQueuedCompletionStatus(
			m_hCompletionPort,
			&dNumberOfBytes,
			(PULONG_PTR)(&pireCompletionKey),
			(LPOVERLAPPED *)(&pireOverLappedex),
			INFINITE
			);
		if(FALSE == bTempTest)
		{
			int nErrorCode = WSAGetLastError();
			if(ERROR_NETNAME_DELETED == nErrorCode)
			{
				WriteLogInfo(LOG_INFO, _T("CSocketServerThread::ExecuteThread(), 网络上有一个SOCKET发生错误，错误代码:%d"), nErrorCode);

				if(pireCompletionKey)
				{
					m_pSocketPoolManager->CloseSpecSocket(pireCompletionKey->keySocket);
				}
				else
				{
					WriteLogInfo(LOG_INFO, _T("CSocketServerThread::ExecuteThread(), 监听套接字发生错误,服务器必须停止工作"));
					return 0;
				}

				m_pMemoryPoolManager->ReleaseKeyOverPire((KeyOverPire *)pireOverLappedex);
				continue;
			}

			WriteLogInfo(LOG_INFO, _T("CSocketServerThread::ExecuteThread(), GetQueuedCompletionStatus返回一个未知错误,但此时套接字好像还可以用,暂时不关闭套接字"));
			continue;
		}

		if(-1 == dNumberOfBytes)
		{
			WriteLogInfo(LOG_INFO, _T("CSocketServerThread::ExecuteThread(), 接收到退出线程的完成包,工作都线程退出"));
			return 0;
		}
		else if(0 == dNumberOfBytes)
		{
			if(pireOverLappedex->wsaOptType == CT_REVC || pireOverLappedex->wsaOptType == CT_SEND)
			{
				m_pSocketPoolManager->CloseSpecSocket(pireCompletionKey->keySocket);
				m_pMemoryPoolManager->ReleaseKeyOverPire((KeyOverPire *)pireOverLappedex);

				continue;
			}
		}

		switch(pireOverLappedex->wsaOptType)
		{
		case CT_REVC:
			DealRevcDate(dNumberOfBytes, (KeyOverPire *)pireOverLappedex);
			break;

		case CT_SEND:
			DealSendDate(dNumberOfBytes, (KeyOverPire *)pireOverLappedex);	
			break;

		case CT_ACCP:
			DealAccpDate(dNumberOfBytes, (KeyOverPire *)pireOverLappedex);
			break;

		case CT_PREPREVC:
			DealPrepRevcDate(dNumberOfBytes, (KeyOverPire *)pireOverLappedex);
			break;

		default:
			break;
		}
	}
	return 0;
}

void CSocketServerThread::InitServerThreadInfo(
	HANDLE hCompletionPort, 
	HANDLE hRevcSocket,
	CBaseSocket * pBaseSocket, 
	CSocketPoolManager * pSocketPoolManager, 
	CMemoryPoolManager * pMemoryPoolManager
	)
{
	ASSERT(hCompletionPort);
	ASSERT(hRevcSocket);
	ASSERT(pBaseSocket);
	ASSERT(pSocketPoolManager);
	ASSERT(pMemoryPoolManager);

	m_hCompletionPort = hCompletionPort;
	m_hRevcSocket = hRevcSocket;
	m_pBaseSocket = pBaseSocket;
	m_pSocketPoolManager = pSocketPoolManager;
	m_pMemoryPoolManager = pMemoryPoolManager;
}

void CSocketServerThread::DealRevcDate(DWORD dNumberOfBytes, KeyOverPire * pKeyOverPire)
{
	ASSERT(m_pBaseSocket);
	ASSERT(m_hRevcSocket);
	ASSERT(m_pSocketPoolManager);
	ASSERT(m_pMemoryPoolManager);

	DWORD dwNumberOfBytesRecvd;
	DWORD dwFlags;

	if(pKeyOverPire)
	{
		USES_CONVERSION;

		DoWriteLogInfo(LOG_INFO, CA2T(pKeyOverPire->pireOverLappedex.wsaBuffer));

		SOCKET scClientSocket = pKeyOverPire->pireOverLappedex.wsaClientSocket;
		WSARecv(
			scClientSocket,
			&(pKeyOverPire->pireOverLappedex.wsaWSABuf),
			BUFFER_SIZE_TO_SOCKET,
			&dwNumberOfBytesRecvd,
			&dwFlags,
			&(pKeyOverPire->pireOverLappedex.wsaOverlapped),
			NULL
			);
	}
}

void CSocketServerThread::DealSendDate(DWORD dNumberOfBytes, KeyOverPire * pKeyOverPire)
{

}

void CSocketServerThread::DealAccpDate(DWORD dNumberOfBytes, KeyOverPire * pKeyOverPire)
{
	ASSERT(m_pBaseSocket);
	ASSERT(m_hRevcSocket);
	ASSERT(m_pSocketPoolManager);
	ASSERT(m_pMemoryPoolManager);

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
		memcpy(&(pKeyOverPire->pireCompletionKey.keyClientAddr), pRemoteAddr, nRmoteAddrLength);
		pKeyOverPire->pireCompletionKey.keySocket = pKeyOverPire->pireOverLappedex.wsaClientSocket;

		SOCKET scServerSocket = m_pBaseSocket->GetSocket();
		SOCKET scClientSocket = pKeyOverPire->pireOverLappedex.wsaClientSocket;

		//if (SOCKET_ERROR == setsockopt(
		//	scClientSocket, 
		//	SOL_SOCKET, 
		//	SO_UPDATE_ACCEPT_CONTEXT, 
		//	(char *)&scServerSocket, 
		//	sizeof (SOCKET)))
		//{
		//	m_pSocketPoolManager->CloseSpecSocket(scClientSocket);
		//	m_pMemoryPoolManager->ReleaseKeyOverPire(pKeyOverPire);

		//	return;
		//}
		
		
		HANDLE hCompletion = NULL;
		hCompletion = CreateIoCompletionPort((HANDLE)scClientSocket, m_hCompletionPort, (ULONG_PTR)&(pKeyOverPire->pireCompletionKey), 0);
		if(hCompletion)
		{
			WSABUF wsaBuffer;
			wsaBuffer.len = BUFFER_SIZE_TO_SOCKET;
			wsaBuffer.buf = new char[4096];//(CHAR *)::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, BUFFER_SIZE_TO_SOCKET);
			//wsaBuffer.len = BUFFER_SIZE_TO_SOCKET;
			//wsaBuffer.buf = new CHAR[BUFFER_SIZE_TO_SOCKET];
			//wsaBuffer.buf = (CHAR *)pKeyOverPire->pireOverLappedex.wsaBuffer;
			//pKeyOverPire->pireOverLappedex.wsaWSABuf.len = BUFFER_SIZE_TO_SOCKET;
			//pKeyOverPire->pireOverLappedex.wsaWSABuf.buf = (CHAR *)pKeyOverPire->pireOverLappedex.wsaBuffer;

			nErrorCode = WSARecv(
				scClientSocket,
				&wsaBuffer,
				4096,
				&dwNumberOfBytesRecvd,
				&dwFlags,
				&(pKeyOverPire->pireOverLappedex.wsaOverlapped),
				NULL
				);

			if(SOCKET_ERROR == nErrorCode)
			{
				int nErrorCode = WSAGetLastError();
				if(WSA_IO_PENDING != nErrorCode)
				{
					WriteLogInfo(LOG_INFO, _T("CSocketServerThread::DealAccpDate(), 投递WSARecv消息时出错,错误代码:%d"), nErrorCode);

					m_pSocketPoolManager->CloseSpecSocket(scClientSocket);
					m_pMemoryPoolManager->ReleaseKeyOverPire(pKeyOverPire);	

					return;
				}
			}

			SetEvent(m_hRevcSocket);
			return;
		}	
		else
		{
			m_pSocketPoolManager->CloseSpecSocket(scClientSocket);
			m_pMemoryPoolManager->ReleaseKeyOverPire(pKeyOverPire);

			return;
		}
	}
}

void CSocketServerThread::DealPrepRevcDate(DWORD dNumberOfBytes, KeyOverPire * pKeyOverPire)
{

}

bool CSocketServerThread::PostRevcMSG()
{
	
	return true;
}

bool CSocketServerThread::PostSendMSG()
{

	return true;
}