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
		// 没有对函数进行初始化
		return 0;
	}

	BOOL  bTempTest = FALSE;
	DWORD dNumberOfBytes = 0;

	LPOverKeyPire pireOverLappedex;
	LPCompKey     pireCompletionKey;

	while(!m_bIsTerminate)
	{
		// 读取网络数据
		bTempTest = GetQueuedCompletionStatus(
			m_hCompletionPort,
			&dNumberOfBytes,
			(PULONG_PTR)(&pireCompletionKey),
			(LPOVERLAPPED*)(&pireOverLappedex),
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
				TRACE(("服务器GetQueuedCompletionStatus收到未知错误:%d\n"), nErrorCode);
				WriteLogInfo(LOG_DEBUG, _T("CWorkThread::ExecuteThread(), GetQueuedCompletionStatus返回一个未知错误:%d,但此时套接字好像还可以用,暂时不关闭套接字"), nErrorCode);
				continue;
			}
		}
		else if(0 == dNumberOfBytes)
		{
			if(pireOverLappedex)
			{
				if(pireOverLappedex->pireOverLappedex.wsaOptType != CT_ACCP)
				{
					TRACE(("有客端连接安全断开\n"));

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
		// 只有到所有值都确定为有效时才设置成初始化成功
		m_bIsInitWorkThread = true;
		WriteLogInfo(LOG_DEBUG, _T("CWorkThread::InitWorkThread(), 连接数据库成功"));

		return true;
	}
	WriteLogInfo(LOG_INFO, _T("CWorkThread::InitWorkThread(), 连接数据库失败"));
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

		// 接收数据时，设定接收的长度小于缓冲长度2个字节
		// 以保证在接收GPS数据时不会出现缓冲溢出
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
		// 数据逻辑处理器
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
			// 数据发送完后准备接收数据
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
	// 这个标识现在不用
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