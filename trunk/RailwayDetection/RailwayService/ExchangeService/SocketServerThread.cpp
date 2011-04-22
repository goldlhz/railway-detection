#include "StdAfx.h"
#include "BaseSocket.h"
#include "SocketServerThread.h"
#include "PoolManager.h"

CSocketServerThread::CSocketServerThread(void)
{
	m_hCompletionPort = NULL;
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
			int nError = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("网络上有一个SOCKET发生错误，错误代码:%d"), nError);
			
			if(pireOverLappedex)
				m_pMemoryPoolManager->ReleaseKeyOverPire((KeyOverPire *)pireOverLappedex);

			if(pireCompletionKey)
				m_pSocketPoolManager->CloseSpecSocket(pireCompletionKey->keySocket);

			//else
			//{
			//	WriteLogInfo(LOG_INFO, _T("监听套接字发生错误,服务器必须停止工作"));
			//	return 0;
			//}
			continue;
		}

		if(-1 == dNumberOfBytes)
		{
			WriteLogInfo(LOG_INFO, _T("CSocketServerThread::ExecuteThread(), 接收到退出线程的完成包,工作都线程退出"));
			return 0;
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
		int nError = 0;
		DWORD dwNumberOfBytesRecvd = 0;
		DWORD dwFlags = 0;
		int nLocalAddrLength = 0;
		int nRmoteAddrLength = 0;
		LPSOCKADDR pLocalAddr = NULL;
		LPSOCKADDR pRemoteAddr = NULL;

		m_pBaseSocket->GetAcceptExSockaddrs(
			pKeyOverPire->pireOverLappedex.wsaAcceptBuffer,
			0,
			sizeof(sockaddr_in) + 16,
			sizeof(sockaddr_in) + 16,
			(SOCKADDR **)&pLocalAddr,
			&nLocalAddrLength,
			(SOCKADDR **)&pRemoteAddr,
			&nRmoteAddrLength);

		GetLocalTime(&(pKeyOverPire->pireCompletionKey.keyLinkTime));
		memcpy(&(pKeyOverPire->pireCompletionKey.keyClientAddr), pRemoteAddr, nRmoteAddrLength);

		nError = WSARecv(
			pKeyOverPire->pireOverLappedex.wsaClientSocket,
			&(pKeyOverPire->pireOverLappedex.wsaWSABuf),
			BUFFER_SIZE_TO_SOCKET,
			&dwNumberOfBytesRecvd,
			&dwFlags,
			&(pKeyOverPire->pireOverLappedex.wsaOverlapped),
			NULL
			);

		if(SOCKET_ERROR == nError)
		{
			int nError = WSAGetLastError();
			if(WSA_IO_PENDING != nError)
			{
				WriteLogInfo(LOG_INFO, _T("CSocketServerThread::DealAccpDate(), 投递WSARecv消息时出错,错误代码:%d"), nError);
			
				m_pSocketPoolManager->CloseSpecSocket(pKeyOverPire->pireOverLappedex.wsaClientSocket);
				m_pMemoryPoolManager->ReleaseKeyOverPire(pKeyOverPire);	
				
				return;
			}
		}
		
		SetEvent(m_hRevcSocket);
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