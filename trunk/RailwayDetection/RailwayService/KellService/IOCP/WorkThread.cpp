#include "StdAfx.h"
#include "WorkThread.h"
#include "SocketPool.h"
#include "MemoryPool.h"
#include "BaseSocket.h"
#include "LogicManager.h"
#include "GobalConfig.h"
#include "adoConnect.h"

CWorkThread::CWorkThread(void)
{
	m_bIsInitWorkThread = false;
	m_hCompletionPort = NULL;
	m_hRevcNotify = NULL;
	m_pBaseSocket = NULL;
	m_pSocketPool = NULL;
	m_pMemoryPool = NULL;
	m_pGobalConfig = NULL;
	m_pAdoDatabase = new CADODatabase;
	m_pLogicManager = new CLogicManager;
}

CWorkThread::~CWorkThread(void)
{
	delete m_pLogicManager;
	delete m_pAdoDatabase;
}

unsigned int CWorkThread::ExecuteThread()
{
	ASSERT(m_hCompletionPort);
	ASSERT(m_hRevcNotify);
	ASSERT(m_pBaseSocket);
	ASSERT(m_pSocketPool);
	ASSERT(m_pMemoryPool);
	ASSERT(m_pGobalConfig);

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
		// ��ȡ��������
		bTempTest = GetQueuedCompletionStatus(
			m_hCompletionPort,
			&dNumberOfBytes,
			(PULONG_PTR)(&pireCompletionKey),
			(LPOVERLAPPED*)(&pireOverLappedex),
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

				if(pireCompletionKey)
				{
					m_pSocketPool->CloseSpecSocket(pireCompletionKey->keyOptSocket);
					m_pMemoryPool->ReleaseOverKeyPire(pireOverLappedex);

					m_pSocketPool->RecycleSocket();
					m_pMemoryPool->RecycleMemory();
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
				if(pireOverLappedex->pireOverLappedex.wsaOptType != CT_ACCP)
				{
					TRACE(("�пͶ����Ӱ�ȫ�Ͽ�\n"));

					m_pSocketPool->CloseSpecSocket(pireOverLappedex->pireCompletionKey.keyOptSocket);
					m_pMemoryPool->ReleaseOverKeyPire(pireOverLappedex);


					m_pSocketPool->RecycleSocket();
					m_pMemoryPool->RecycleMemory();

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
				DealRevcDatePack(dNumberOfBytes, pireOverLappedex);
				break;

			case CT_SEND:
				DealSendDatePack(dNumberOfBytes, pireOverLappedex);	
				break;

			case CT_ACCP:
				DealAccpDatePack(dNumberOfBytes, pireOverLappedex);
				break;

			default:
				break;
			}
		}		
	}

	return 0;
}

bool CWorkThread::InitWorkThread(
	HANDLE hCompletionPort, 
	HANDLE hRevcNotify,
	CBaseSocket * pBaseSocket, 
	CSocketPool * pSocketPool, 
	CMemoryPool * pMemoryPool,
	CGobalConfig* pGobalConfig
	)
{
	ASSERT(hCompletionPort);
	ASSERT(hRevcNotify);
	ASSERT(pBaseSocket);
	ASSERT(pSocketPool);
	ASSERT(pMemoryPool);
	ASSERT(pGobalConfig);

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

	if(!pGobalConfig)
		return false;
	m_pGobalConfig = pGobalConfig;

	string strConnection;

	m_pAdoDatabase->SetConnectionTimeout(10);
	strConnection= FormatConnectString(pGobalConfig);

	if(m_pAdoDatabase->Open(strConnection.c_str()))
	{
		// ֻ�е�����ֵ��ȷ��Ϊ��Чʱ�����óɳ�ʼ���ɹ�
		m_bIsInitWorkThread = true;
		WriteLogInfo(LOG_DEBUG, _T("CWorkThread::InitWorkThread(), �������ݿ�ɹ�"));

		return true;
	}
	WriteLogInfo(LOG_INFO, _T("CWorkThread::InitWorkThread(), �������ݿ�ʧ��"));
	return false;
}

int CWorkThread::DealSendData(LPOverKeyPire pKeyOverPire, void* parameter)
{
	ASSERT(pKeyOverPire);
	ASSERT(parameter);
	if(pKeyOverPire && parameter)
	{
		int nTemp;
		int nErrorCode = 0;
		CWorkThread* pWorkThread = (CWorkThread*)parameter;

		while(1)
		{
			nTemp = send(pKeyOverPire->pireOverLappedex.wsaClientSocket, 
				pKeyOverPire->pireOverLappedex.wsaBuffer,
				pKeyOverPire->pireOverLappedex.wsaWSABUF.len,
				0);
			if(nTemp == SOCKET_ERROR)
			{
				nErrorCode = WSAGetLastError();
				if(WSAEWOULDBLOCK == nErrorCode)
				{
					Sleep(0);
					continue;
				}
				return 1;
			}
			break;
		}
		return 0;
	}
	return -1;
}

int CWorkThread::DealRecvData(LPOverKeyPire pKeyOverPire, void* parameter, string& strContext)
{
	ASSERT(pKeyOverPire);
	ASSERT(parameter);
	if(pKeyOverPire && parameter)
	{
		int nRevcCount;

		// ��������ʱ���趨���յĳ���С�ڻ��峤��2���ֽ�
		// �Ա�֤�ڽ���GPS����ʱ������ֻ������
		memset(pKeyOverPire->pireOverLappedex.wsaBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		nRevcCount = recv(pKeyOverPire->pireOverLappedex.wsaClientSocket,
			pKeyOverPire->pireOverLappedex.wsaBuffer,
			BUFFER_SIZE_TO_CLIENT - 2,
			0);

		if(nRevcCount != SOCKET_ERROR)
		{
			strContext += pKeyOverPire->pireOverLappedex.wsaBuffer;
			return 0;
		}
		else
		{
			if(WSAEWOULDBLOCK == WSAGetLastError())
			{
				Sleep(0);
				return 0;
			}
			return 1;
		}
	}

	return -1;
}

void CWorkThread::DealRevcDatePack(DWORD dNumberOfBytes, LPOverKeyPire pKeyOverPire)
{
	ASSERT(m_hCompletionPort);
	ASSERT(m_pBaseSocket);
	ASSERT(m_hRevcNotify);
	ASSERT(m_pSocketPool);
	ASSERT(m_pMemoryPool);
	ASSERT(m_pGobalConfig);

	if(pKeyOverPire)
	{
		// �����߼�������
		int nSendCount = 0;
		nSendCount = m_pLogicManager->SetLogicData(dNumberOfBytes, 
			pKeyOverPire, 
			m_pAdoDatabase, 
			(void*)this, 
			(void*)&CWorkThread::DealSendData,
			(void*)&CWorkThread::DealRecvData,
			m_pGobalConfig);

		if(nSendCount > 0)
		{
			PostSendMSG(pKeyOverPire->pireOverLappedex.wsaClientSocket, pKeyOverPire);

			return;
		}
		else if(nSendCount == 0)
		{
			// ���ݷ������׼����������
			PostRevcMSG(pKeyOverPire->pireOverLappedex.wsaClientSocket, pKeyOverPire);
		}
		else if(nSendCount == -2)
		{
			m_pSocketPool->CloseSpecSocket(pKeyOverPire->pireOverLappedex.wsaClientSocket);
			m_pMemoryPool->ReleaseOverKeyPire(pKeyOverPire);
		}
	}
}

void CWorkThread::DealSendDatePack(DWORD dNumberOfBytes, LPOverKeyPire pKeyOverPire)
{
	ASSERT(m_hCompletionPort);
	ASSERT(m_pBaseSocket);
	ASSERT(m_hRevcNotify);
	ASSERT(m_pSocketPool);
	ASSERT(m_pMemoryPool);
	ASSERT(m_pGobalConfig);

	if(pKeyOverPire)
	{
		PostRevcMSG(pKeyOverPire->pireOverLappedex.wsaClientSocket, pKeyOverPire);

		return;
	}
}

void CWorkThread::DealAccpDatePack(DWORD dNumberOfBytes, LPOverKeyPire pKeyOverPire)

{
	ASSERT(m_hCompletionPort);
	ASSERT(m_pBaseSocket);
	ASSERT(m_hRevcNotify);
	ASSERT(m_pSocketPool);
	ASSERT(m_pMemoryPool);
	ASSERT(m_pGobalConfig);

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
	// �����ʶ���ڲ���
}

bool CWorkThread::PostRevcMSG(SOCKET scOptSocket, LPOverKeyPire pKeyOverPire, OptType optType)
{
	ASSERT(pKeyOverPire);

	if(!pKeyOverPire)
		return false;

	int nErrorCode = 0;
	DWORD dwFlags = 0;
	DWORD dwNumberOfBytesRecvd = 0;

	memset(pKeyOverPire, 0x00, sizeof(OVERLAPPED));
	memset(pKeyOverPire->pireOverLappedex.wsaBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);

	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = BUFFER_SIZE_TO_CLIENT - 2;
	pKeyOverPire->pireOverLappedex.wsaWSABUF.buf = pKeyOverPire->pireOverLappedex.wsaBuffer;
	pKeyOverPire->pireOverLappedex.wsaOptType = optType;

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

string CWorkThread::FormatConnectString(CGobalConfig* pGobalConfig)
{
	if(pGobalConfig)
	{
		char strConnection[MAX_PATH];
		sprintf_s(strConnection, MAX_PATH, "Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s",
			pGobalConfig->GetPassword().c_str(), pGobalConfig->GetUserName().c_str(), pGobalConfig->GetDatabase().c_str(), pGobalConfig->GetHost().c_str());

		//sprintf_s(strConnection, MAX_PATH, "Provider=SQLOLEDB.1;Persist Security Info=False;Initial Catalog=%s;Data Source=%s",
		//	pGobalConfig->GetDatabase().c_str(), pGobalConfig->GetHost().c_str());
		
		return strConnection;
	}
	return "";
}