#include "StdAfx.h"
#include "WorkThreadPool.h"
#include "BaseSocket.h"
#include "SocketPool.h"
#include "MemoryPool.h"
#include "WorkThread.h"
#include "CommonFunction.h"

CWorkThreadPool::CWorkThreadPool(void)
{
	m_vecServerThread.clear();

	m_hCompletionPort = NULL;
	m_hRevcNotify = NULL;
	m_pBaseSocket = NULL; 
	m_pSocketPool = NULL; 
	m_pMemoryPool = NULL;
}

CWorkThreadPool::~CWorkThreadPool(void)
{
	CloseWorkThreads();
}

bool CWorkThreadPool::CreateWorkThreads(
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

	CWorkThread * pThread;
	int nServerThreadSize = CCommonFunction::GetKernalCount() * 2 + 2;

	for (int nIndex = 0; nIndex < 1; ++nIndex)
	{
		pThread = new CWorkThread();
		if(pThread)
		{
			if(pThread->BeginThread(false))
			{
				if(pThread->InitWorkThread(hCompletionPort, 
					hRevcNotify, 
					pBaseSocket, 
					pSocketPool, 
					pMemoryPool, 
					pGobalConfig))
				{
					if(-1 != pThread->ResumeThread())
					{
						m_vecServerThread.push_back(pThread);
						continue;
					}
				}

				delete pThread;

				CloseWorkThreads();

				return false;
			}
		}
		return false;
	}
	return true;
}

bool CWorkThreadPool::CloseWorkThreads()
{
	if(m_hCompletionPort)
	{
		if(m_vecServerThread.size() > 0)
		{
			BOOL bTest = FALSE;
			int  nThreadSize = 0;
			HANDLE * hThreadHandle = NULL;
			vector<CWorkThread *>::iterator iterWorkThread;

			nThreadSize    = m_vecServerThread.size();
			hThreadHandle  = new HANDLE[nThreadSize];
			iterWorkThread = m_vecServerThread.begin();

			for(int nIndex = 0; nIndex < nThreadSize; ++iterWorkThread, ++nIndex)
			{
				hThreadHandle[nIndex] = (*iterWorkThread)->GetThreaadHandle();
			}

			// post quit completion pack
			::PostQueuedCompletionStatus(m_hCompletionPort, -1, (ULONG_PTR)NULL, NULL);

			// wait for work threads exit
			::WaitForMultipleObjects(nThreadSize, hThreadHandle, TRUE, 30000);

			for (int nIndex = 0; nIndex < nThreadSize; ++nIndex)
			{
				CloseHandle(hThreadHandle[nIndex]);
			}

			delete [] hThreadHandle;
			hThreadHandle = NULL;

			for (iterWorkThread = m_vecServerThread.begin(); iterWorkThread != m_vecServerThread.end(); ++iterWorkThread)
			{
				delete static_cast<CWorkThread *>(*iterWorkThread);
			}

			m_vecServerThread.clear();
		}
	}

	return true;
}