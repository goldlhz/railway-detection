#include "StdAfx.h"
#include "SocketPool.h"
#include "GobalConfig.h"
#include "IOCPDefine.h"

CSocketPool::CSocketPool()
{
	m_pGobalConfig = NULL;
	m_vecUsedSocket.clear();
	m_vecPrepSocket.clear();
}

CSocketPool::~CSocketPool()
{
	ClearSocketPool();
}

SOCKET CSocketPool::GetNewSocket()
{
	SOCKET scSocket = INVALID_SOCKET;
	CListSocket::iterator iterSocket;

	m_scSocketCriticalSection.Lock();

	if(SocketPoolAutoManage())
	{
		iterSocket = m_vecPrepSocket.begin();

		scSocket = (SOCKET)(*iterSocket);
		m_vecPrepSocket.erase(iterSocket);
		m_vecUsedSocket.push_back(scSocket);
	}

	m_scSocketCriticalSection.Unlock();
	return scSocket;
}

void CSocketPool::ReleaseSocket(SOCKET scSocket)
{
	CListSocket::iterator iterSocket;
	m_scSocketCriticalSection.Lock();

	for (iterSocket = m_vecUsedSocket.begin(); iterSocket != m_vecUsedSocket.end(); ++iterSocket)
	{
		if(scSocket == (SOCKET)(*iterSocket))
		{
			m_vecUsedSocket.erase(iterSocket);
			m_vecPrepSocket.push_back(scSocket);
			break;
		}
	}

	m_scSocketCriticalSection.Unlock();
}

void CSocketPool::CloseSpecSocket(SOCKET scSocket)
{
	int nErrorCode = 0;
	CListSocket::iterator iterSocket;
	m_scSocketCriticalSection.Lock();

	if(0 == nErrorCode)
	{
		for (iterSocket = m_vecUsedSocket.begin(); iterSocket != m_vecUsedSocket.end(); ++iterSocket)
		{
			if(scSocket == (*iterSocket))
			{
				shutdown(scSocket, SD_BOTH);
				closesocket(scSocket);
				m_vecUsedSocket.erase(iterSocket);

				nErrorCode = 1;
				break;
			}
		}
	}

	if(0 == nErrorCode)
	{
		for (iterSocket = m_vecPrepSocket.begin(); iterSocket != m_vecPrepSocket.end(); ++iterSocket)
		{
			if(scSocket == (*iterSocket))
			{
				shutdown(scSocket, SD_BOTH);
				closesocket(scSocket);
				m_vecPrepSocket.erase(iterSocket);
				break;
			}
		}
	}

	m_scSocketCriticalSection.Unlock();
}

bool CSocketPool::InitSocketPool(CGobalConfig *pGobalConfig)
{
	ASSERT(pGobalConfig);
	if(!pGobalConfig)
		return false;

	m_pGobalConfig = pGobalConfig;

	unsigned int nPrepSocketPoolSize = (unsigned int)m_vecPrepSocket.size();
	unsigned int nSocketPoolSize = m_pGobalConfig->GetSocketPoolSize();

	if(nPrepSocketPoolSize < nSocketPoolSize)
	{
		SOCKET scSocket;
		for (unsigned int nIndex = nPrepSocketPoolSize; nIndex < nSocketPoolSize; ++nIndex)
		{

			scSocket = CreatePoolSocket();
			if(INVALID_SOCKET != scSocket)
			{
				m_vecPrepSocket.push_back(scSocket);
				continue;
			}

			ClearSocketPool();

			return false;
		}
	}
	return true;
}

void CSocketPool::ClearSocketPool()
{
	CListSocket::iterator iterTemp;
	m_scSocketCriticalSection.Lock();

	if(!m_vecUsedSocket.empty())
	{
		for (iterTemp = m_vecUsedSocket.begin(); iterTemp != m_vecUsedSocket.end(); ++iterTemp)
		{
			shutdown((SOCKET)(*iterTemp), SD_BOTH);
			closesocket((SOCKET)(*iterTemp));
		}
		m_vecUsedSocket.clear();
	}

	if(!m_vecPrepSocket.empty())
	{
		for (iterTemp = m_vecPrepSocket.begin(); iterTemp != m_vecPrepSocket.end(); ++iterTemp)
		{
			shutdown((SOCKET)(*iterTemp), SD_BOTH);
			closesocket((SOCKET)(*iterTemp));
		}
		m_vecPrepSocket.clear();
	}

	m_scSocketCriticalSection.Unlock();
}

void CSocketPool::RecycleSocket()
{
	unsigned int nSocketPoolSize = 0;
	m_scSocketCriticalSection.Lock();

	nSocketPoolSize = m_vecPrepSocket.size();
	if(nSocketPoolSize > m_pGobalConfig->GetSocketRecycleSize())
	{
		DecrescenceSocketPoolSize(nSocketPoolSize);
	}

	m_scSocketCriticalSection.Unlock();
}

SOCKET CSocketPool::CreatePoolSocket()
{
	SOCKET scSocket;
	int  nErrorCode = 0;
	BOOL bDONTLINGER = TRUE;
	BOOL bREUSEADDR  = TRUE;
	int  nSendSocketBufSize = BUFFER_SIZE_TO_CLIENT;
	int  nRevcSockerBufSize = BUFFER_SIZE_TO_CLIENT;

	scSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(INVALID_SOCKET != scSocket)
	{
		nErrorCode = setsockopt(scSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&nRevcSockerBufSize, sizeof(int));
		if(SOCKET_ERROR  == nErrorCode)
		{
			nErrorCode = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketPool::CreatePoolSocket(), 设置客户套接字接收缓冲大小时出错,错误代码:%d"), nErrorCode);

			return INVALID_SOCKET;
		}

		nErrorCode = setsockopt(scSocket, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendSocketBufSize, sizeof(int));
		if(SOCKET_ERROR  == nErrorCode)
		{
			nErrorCode = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketPool::CreatePoolSocket(), 设置客户套接字发送缓冲大小时出错,错误代码:%d"), nErrorCode);

			return INVALID_SOCKET;
		}

		nErrorCode = setsockopt(scSocket, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDONTLINGER, sizeof(BOOL));
		if(SOCKET_ERROR  == nErrorCode)
		{
			nErrorCode = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketPool::CreatePoolSocket(), 设置客户套接字为不托延出错,错误代码:%d"), nErrorCode);

			return INVALID_SOCKET;
		}

		nErrorCode = setsockopt(scSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&bREUSEADDR, sizeof(BOOL));
		if(SOCKET_ERROR  == nErrorCode)
		{
			nErrorCode = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketPool::CreatePoolSocket(), 设置客户套接字为不托延出错,错误代码:%d"), nErrorCode);

			return INVALID_SOCKET;
		}

		return scSocket;
	}

	// 记录失败日志
	nErrorCode = WSAGetLastError();
	WriteLogInfo(LOG_INFO, _T("CSocketPool::CreatePoolSocket(), 套接字池在生成套接字时出错，错误代码:%d"), nErrorCode);
	return INVALID_SOCKET;
}

bool CSocketPool::SocketPoolAutoManage()
{
	ASSERT(m_pGobalConfig);
	if(!m_pGobalConfig)
		return false;

	unsigned int nPrepSocketSize = m_vecPrepSocket.size();
	if(nPrepSocketSize < m_pGobalConfig->GetSocketDownLine())
	{
		return IncrementSocketPoolSize(nPrepSocketSize);
	}
	else if(nPrepSocketSize > m_pGobalConfig->GetSocketRecycleSize())
	{
		return DecrescenceSocketPoolSize(nPrepSocketSize);
	}
	return true;
}

bool CSocketPool::IncrementSocketPoolSize(unsigned int nCurrentSize)
{
	ASSERT(m_pGobalConfig);
	if(!m_pGobalConfig)
		return false;

	unsigned int nSocketPoolSize = m_pGobalConfig->GetSocketPoolSize();

	if(nCurrentSize >= nSocketPoolSize)
		return true;

	SOCKET scSocket;
	for( ; nCurrentSize < nSocketPoolSize; ++nCurrentSize)
	{
		scSocket = CreatePoolSocket();
		if(INVALID_SOCKET != scSocket)
		{
			m_vecPrepSocket.push_back(scSocket);
			continue;
		}
		return false;
	}
	return true;
}

bool CSocketPool::DecrescenceSocketPoolSize(unsigned int nCurrentSize)
{
	ASSERT(m_pGobalConfig);
	if(!m_pGobalConfig)
		return false;

	unsigned int nSocketPoolSize = m_pGobalConfig->GetSocketPoolSize();
	if(nCurrentSize > nSocketPoolSize)
	{
		CListSocket::iterator iterSocket;

		for ( ; nCurrentSize > nSocketPoolSize; --nCurrentSize)
		{
			iterSocket = m_vecPrepSocket.end();
			--iterSocket;

			shutdown((SOCKET)(*iterSocket), SD_BOTH);
			closesocket((SOCKET)(*iterSocket));

			m_vecPrepSocket.erase(iterSocket);
		}
	}
	return true;
}