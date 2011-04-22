#include "StdAfx.h"
#include "PoolManager.h"
#include "GobalConfig.h"

CSocketPoolManager::CSocketPoolManager()
{
	m_vecUsedSocket.clear();
	m_vecPrepSocket.clear();
}

CSocketPoolManager::~CSocketPoolManager()
{
	ClearSocketPool();
}

SOCKET CSocketPoolManager::GetNewSocket()
{
	SOCKET scSocket = INVALID_SOCKET;
	CListSocket::iterator iterSocket;

	m_scSocketCriticalSection.Lock();

	if(SocketPoolAutoManage())
	{
		iterSocket = m_vecPrepSocket.end();
		--iterSocket;

		scSocket = *iterSocket;
		m_vecPrepSocket.erase(iterSocket);
		m_vecUsedSocket.push_back(scSocket);
	}

	m_scSocketCriticalSection.Unlock();
	return scSocket;
}

void CSocketPoolManager::ReleaseSocket(SOCKET scSocket)
{
	CListSocket::iterator iterSocket;
	m_scSocketCriticalSection.Lock();

	for (iterSocket = m_vecUsedSocket.begin(); iterSocket != m_vecUsedSocket.end(); ++iterSocket)
	{
		if(scSocket == (*iterSocket))
		{
			m_vecUsedSocket.erase(iterSocket);
			m_vecPrepSocket.push_back(scSocket);
			break;
		}
	}

	m_scSocketCriticalSection.Unlock();
}

void CSocketPoolManager::CloseSpecSocket(SOCKET scSocket)
{
	int nError = 0;
	CListSocket::iterator iterSocket;
	m_scSocketCriticalSection.Lock();

	if(0 == nError)
	{
		for (iterSocket = m_vecUsedSocket.begin(); iterSocket != m_vecUsedSocket.end(); ++iterSocket)
		{
			if(scSocket == (*iterSocket))
			{
				shutdown(scSocket, SD_BOTH);
				closesocket(scSocket);
				m_vecUsedSocket.erase(iterSocket);

				nError = 1;
				break;
			}
		}
	}

	if(0 == nError)
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

bool CSocketPoolManager::InitSocketPool()
{
	int nPrepSocketPoolSize = (int)m_vecPrepSocket.size();
	int nSocketPoolSize = CGobalConfig::GetGobalConfig().GetSocketPoolSize();

	if(nPrepSocketPoolSize < nSocketPoolSize)
	{
		SOCKET scSocket;
		for (int nIndex = nPrepSocketPoolSize; nIndex < nSocketPoolSize; ++nIndex)
		{
			scSocket = CreatePoolSocket();
			if(INVALID_SOCKET != scSocket)
			{
				m_vecPrepSocket.push_back(scSocket);
				continue;
			}
			return false;
		}
	}
	return true;
}

void CSocketPoolManager::ClearSocketPool()
{
	CListSocket::iterator iterTemp;
	m_scSocketCriticalSection.Lock();

	if(!m_vecUsedSocket.empty())
	{
		for (iterTemp = m_vecUsedSocket.begin(); iterTemp != m_vecUsedSocket.end(); ++iterTemp)
		{
			shutdown(*iterTemp, SD_BOTH);
			closesocket(*iterTemp);
		}
		m_vecUsedSocket.clear();
	}

	if(!m_vecPrepSocket.empty())
	{
		for (iterTemp = m_vecPrepSocket.begin(); iterTemp != m_vecPrepSocket.end(); ++iterTemp)
		{
			shutdown(*iterTemp, SD_BOTH);
			closesocket(*iterTemp);
		}
		m_vecPrepSocket.clear();
	}

	m_scSocketCriticalSection.Unlock();
}

SOCKET CSocketPoolManager::CreatePoolSocket()
{
	SOCKET scSocket;
	int nError = 0;
	int nSendSocketBufSize = BUFFER_SIZE_TO_SOCKET;
	int nRevcSockerBufSize = BUFFER_SIZE_TO_SOCKET;

	scSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(INVALID_SOCKET != scSocket)
	{
		nError = setsockopt(scSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&nRevcSockerBufSize, sizeof(int));
		if(SOCKET_ERROR  == nError)
		{
			nError = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketPoolManager::CreatePoolSocket(), 设置监听套接字接收缓冲大小时出错,错误代码:%d"), nError);

			return INVALID_SOCKET;
		}

		nError = setsockopt(scSocket, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendSocketBufSize, sizeof(int));
		if(SOCKET_ERROR  == nError)
		{
			nError = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketPoolManager::CreatePoolSocket(), 设置监听套接字发送缓冲大小时出错,错误代码:%d"), nError);

			return INVALID_SOCKET;
		}

		return scSocket;
	}

	// 记录失败日志
	nError = WSAGetLastError();
	WriteLogInfo(LOG_INFO, _T("CSocketPoolManager::CreatePoolSocket(), 套接字池在生成套接字时出错，错误代码:%d"), nError);
	return INVALID_SOCKET;
}

bool CSocketPoolManager::SocketPoolAutoManage()
{
	int nPrepSocketSize = (int)m_vecPrepSocket.size();

	if(nPrepSocketSize < CGobalConfig::GetGobalConfig().GetSocketDownLine())
	{
		return IncrementSocketPoolSize(nPrepSocketSize);
	}
	else if(nPrepSocketSize > CGobalConfig::GetGobalConfig().GetSocketRecycleSize())
	{
		DecrescenceSocketPoolSize(nPrepSocketSize);
	}
	return true;
}

bool CSocketPoolManager::IncrementSocketPoolSize(int nCurrentSize)
{
	int nSocketPoolSize = CGobalConfig::GetGobalConfig().GetSocketPoolSize();

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

void CSocketPoolManager::DecrescenceSocketPoolSize(int nCurrentSize)
{
	int nSocketPoolSize = CGobalConfig::GetGobalConfig().GetSocketPoolSize();

	if(nCurrentSize <= nSocketPoolSize)
		return;

	CListSocket::iterator iterSocket = m_vecPrepSocket.end();
	for(--iterSocket; nCurrentSize > nSocketPoolSize; --nCurrentSize, --iterSocket)
	{
		shutdown(*iterSocket, SD_BOTH);
		closesocket(*iterSocket);

		m_vecPrepSocket.erase(iterSocket);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CMemoryPoolManager::CMemoryPoolManager()
{
	m_vecPrepKeyOverPire.clear();
	m_vecUsedKeyOverPire.clear();
}

CMemoryPoolManager::~CMemoryPoolManager()
{
	ClearMemoryPool();
}

KeyOverPire * CMemoryPoolManager::GetNewKeyOverPire()
{
	KeyOverPire * pKeyOverPire = NULL;
	CListKeyOverPire::iterator iterBack;

	m_scMemoryCriticalSection.Lock();

	if(MemoryPoolAutoManage())
	{
		iterBack = m_vecPrepKeyOverPire.end();
		--iterBack;

		pKeyOverPire = *iterBack;
		m_vecPrepKeyOverPire.erase(iterBack);
		m_vecUsedKeyOverPire.push_back(pKeyOverPire);
	}

	m_scMemoryCriticalSection.Unlock();
	return pKeyOverPire;
}

void CMemoryPoolManager::ReleaseKeyOverPire(KeyOverPire * pPire)
{
	CListKeyOverPire::iterator iterTemp;
	m_scMemoryCriticalSection.Lock();

	for (iterTemp = m_vecUsedKeyOverPire.begin(); iterTemp != m_vecUsedKeyOverPire.end(); ++iterTemp)
	{
		if(pPire == (*iterTemp))
		{
			m_vecUsedKeyOverPire.erase(iterTemp);
			m_vecPrepKeyOverPire.push_back(pPire);
			break;
		}
	}

	m_scMemoryCriticalSection.Unlock();
}

bool CMemoryPoolManager::InitMemoryPool()
{
	int nMemoryPoolSize = CGobalConfig::GetGobalConfig().GetMemoryPoolSize();
	int nPrepMemoryPoolSize = (int)m_vecPrepKeyOverPire.size();

	if(nPrepMemoryPoolSize < nMemoryPoolSize)
	{
		KeyOverPire * pPire;
		for(int nIndex = nPrepMemoryPoolSize; nIndex < nMemoryPoolSize; ++nIndex)
		{
			pPire = (KeyOverPire *)::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(KeyOverPire));
			if(pPire)
			{
				pPire->pireOverLappedex.wsaOptType = CT_UNKOWN;
				pPire->pireOverLappedex.wsaWSABuf.len = BUFFER_SIZE_TO_SOCKET;
				pPire->pireOverLappedex.wsaWSABuf.buf = (CHAR *)pPire->pireOverLappedex.wsaBuffer;

				m_vecPrepKeyOverPire.push_back(pPire);
				continue;
			}
			return false;
		}
	}
	return true;
}

void CMemoryPoolManager::ClearMemoryPool()
{
	CListKeyOverPire::iterator iterTemp;
	m_scMemoryCriticalSection.Lock();

	if(!m_vecUsedKeyOverPire.empty())
	{
		for (iterTemp = m_vecUsedKeyOverPire.begin(); iterTemp != m_vecUsedKeyOverPire.end(); ++iterTemp)
		{
			::HeapFree(::GetProcessHeap(), 0, (LPVOID)(*iterTemp));
		}
		m_vecUsedKeyOverPire.clear();
	}

	if(!m_vecPrepKeyOverPire.empty())
	{
		for (iterTemp = m_vecPrepKeyOverPire.begin(); iterTemp != m_vecPrepKeyOverPire.end(); ++iterTemp)
		{
			::HeapFree(::GetProcessHeap(), 0, (LPVOID)(*iterTemp));
		}
		m_vecPrepKeyOverPire.clear();
	}

	m_scMemoryCriticalSection.Unlock();
}

bool CMemoryPoolManager::MemoryPoolAutoManage()
{
	int nPrepKeyOverPireSize = (int)m_vecPrepKeyOverPire.size();
	if(nPrepKeyOverPireSize < CGobalConfig::GetGobalConfig().GetMemoryDownLine())
	{
		return IncrementMemoryPoolSize(nPrepKeyOverPireSize);
	}
	else if(nPrepKeyOverPireSize > CGobalConfig::GetGobalConfig().GetMemoryRecycleSize())
	{
		DecrescenceMemoryPoolSize(nPrepKeyOverPireSize);
	}
	return true;
}

bool CMemoryPoolManager::IncrementMemoryPoolSize(int nCurrentSize)
{
	if(nCurrentSize >= CGobalConfig::GetGobalConfig().GetMemoryPoolSize())
		return true;

	KeyOverPire * pPire;
	for( ; nCurrentSize < CGobalConfig::GetGobalConfig().GetMemoryPoolSize(); ++nCurrentSize)
	{
		pPire = (KeyOverPire *)::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(KeyOverPire));
		if(pPire)
		{
			pPire->pireOverLappedex.wsaOptType = CT_UNKOWN;
			pPire->pireOverLappedex.wsaWSABuf.len = BUFFER_SIZE_TO_SOCKET;
			pPire->pireOverLappedex.wsaWSABuf.buf = (CHAR *)pPire->pireOverLappedex.wsaBuffer;

			m_vecPrepKeyOverPire.push_back(pPire);
			continue;
		}
		return false;
	}
	return true;
}

void CMemoryPoolManager::DecrescenceMemoryPoolSize(int nCurrentSize)
{
	if(nCurrentSize <= CGobalConfig::GetGobalConfig().GetMemoryPoolSize())
		return;

	CListKeyOverPire::iterator iterPire = m_vecPrepKeyOverPire.end();
	for(--iterPire; nCurrentSize > CGobalConfig::GetGobalConfig().GetMemoryPoolSize(); --nCurrentSize, --iterPire)
	{
		::HeapFree(::GetProcessHeap(), 0, (LPVOID)(*iterPire));
		m_vecPrepKeyOverPire.erase(iterPire);
	}
}
