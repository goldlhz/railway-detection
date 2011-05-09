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
		// û�жԺ������г�ʼ��
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

		// ��ȡ��������
		bTempTest = GetQueuedCompletionStatus(
			m_hCompletionPort,
			&dNumberOfBytes,
			(PULONG_PTR)(&pireCompletionKey),
			(LPOVERLAPPED *)(&pireOverLappedex),
			INFINITE
			);

		if(-1 == dNumberOfBytes)
		{
			DoWriteLogInfo(LOG_INFO, _T("CWorkThread::ExecuteThread(), ���յ��˳��̵߳���ɰ�,�������߳��˳�"));
			return 0;
		}
		if(FALSE == bTempTest)
		{
			int nErrorCode = GetLastError();
			if(ERROR_NETNAME_DELETED == nErrorCode)
			{
				TRACE(("������GetQueuedCompletionStatus�յ�һ������ERROR_NETNAME_DELETED\n"));
				WriteLogInfo(LOG_DEBUG, _T("CWorkThread::ExecuteThread(), ��������һ��SOCKET�������󣬴������:%d"), nErrorCode);

				if(!pireCompletionKey)
				{
					m_pSocketPool->CloseSpecSocket(pireCompletionKey->keyOptSocket);
					m_pMemoryPool->ReleaseOverKeyPire(pireOverLappedex);
				}
				continue;
			}
			else
			{
				TRACE(("������GetQueuedCompletionStatus�յ�δ֪����:%d\n"), nErrorCode);
				WriteLogInfo(LOG_DEBUG, _T("CWorkThread::ExecuteThread(), GetQueuedCompletionStatus����һ��δ֪����:%d,����ʱ�׽��ֺ��񻹿�����,��ʱ���ر��׽���"), nErrorCode);
				continue;
			}
		}
		else if(0 == dNumberOfBytes)
		{
			if(pireOverLappedex)
			{
				if(pireOverLappedex->pireOverLappedex.wsaOptType == CT_REVC || pireOverLappedex->pireOverLappedex.wsaOptType == CT_SEND)
				{
					TRACE(("�пͶ����Ӱ�ȫ�Ͽ�\n"));

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

	// ֻ�е�����ֵ��ȷ��Ϊ��Чʱ�����óɳ�ʼ���ɹ�
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
		WriteLogInfo(LOG_ERR, _T("�յ���һ�������õ����ݰ�,��СΪ:%d"), dNumberOfBytes);
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
			TRACE(("���ÿͻ����׽��ֵ����Ը�������׽���һ��ʱ����:%d\n"), nErrorCode);
			WriteLogInfo(LOG_INFO, _T("CWorkThread::DealAccpDate(), ���ÿͻ����׽��ֵ����Ը�������׽���һ��ʱ����,�������:%d"), nErrorCode);

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
			TRACE(("���յ����׽��ְ󶨵���ɶ˿ڳ���:%d\n"), nErrorCode);

			WriteLogInfo(LOG_INFO, _T("CWorkThread::DealAccpDate(), ���յ����׽�������ɶ˿ڰ�ʱ����,�������:%d"), nErrorCode);

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
			// Ͷ�ݳɹ�,ʲô������
			return true;
		}
		else if(WSAENOBUFS == nErrorCode)
		{
			// ��Դ����
			TRACE(("Ͷ��WSARecv��Ϣ��Դ�������,�������:%d\n"), nErrorCode);
			WriteLogInfo(LOG_INFO, _T("CWorkThread::DealAccpDate(), Ͷ��WSARecv��Ϣ��Դ�������,�������:%d"), nErrorCode);

			m_pSocketPool->CloseSpecSocket(scOptSocket);
			m_pMemoryPool->ReleaseOverKeyPire(pKeyOverPire);	

			return false;
		}
		else
		{
			TRACE(("Ͷ��WSARecv��Ϣ����δ֪ʱ����,�������:%d\n"), nErrorCode);
			WriteLogInfo(LOG_INFO, _T("CWorkThread::DealAccpDate(), Ͷ��WSARecv��Ϣ����δ֪ʱ����,�������:%d"), nErrorCode);

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
			// Ͷ�ݳɹ�,ʲô������
			return true;
		}
		else if(WSAENOBUFS == nErrorCode)
		{
			// ��Դ����
			TRACE(("Ͷ��WSARecv��Ϣ��Դ�������,�������:%d\n"), nErrorCode);
			WriteLogInfo(LOG_INFO, _T("CWorkThread::DealAccpDate(), Ͷ��WSARecv��Ϣ��Դ�������,�������:%d"), nErrorCode);

			m_pSocketPool->CloseSpecSocket(scOptSocket);
			m_pMemoryPool->ReleaseOverKeyPire(pKeyOverPire);	

			return false;
		}
		else
		{
			TRACE(("Ͷ��WSARecv��Ϣ����δ֪ʱ����,�������:%d\n"), nErrorCode);
			WriteLogInfo(LOG_INFO, _T("CWorkThread::DealAccpDate(), Ͷ��WSARecv��Ϣ����δ֪ʱ����,�������:%d"), nErrorCode);

			m_pSocketPool->CloseSpecSocket(scOptSocket);
			m_pMemoryPool->ReleaseOverKeyPire(pKeyOverPire);	

			return false;
		}
	}
	return true;
}
