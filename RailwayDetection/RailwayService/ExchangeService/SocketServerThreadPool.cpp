#include "StdAfx.h"
#include "SocketServerThreadPool.h"
#include "SocketServerThread.h"
#include "CommonFunction.h"
#include "BaseSocket.h"

CSocketServerThreadPool::CSocketServerThreadPool(void)
{
	m_vecServerThread.clear();

	m_hCompletionPort = NULL;
	m_pBaseSocket = NULL; 
	m_pSocketPoolManager = NULL; 
	m_pMemoryPoolManager = NULL;
}

CSocketServerThreadPool::~CSocketServerThreadPool(void)
{
}

bool CSocketServerThreadPool::CreateServerThreads(HANDLE hCompletionPort, 
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

	CSocketServerThread * pThread;
	int nServerThreadSize = CCommonFunction::GetCurrentKernalCount() * 2 + 2;

	for (int nIndex = 0; nIndex < nServerThreadSize; ++nIndex)
	{
		pThread = new CSocketServerThread();
		if(pThread)
		{
			if(pThread->CreateThreadInstance(false))
			{
				pThread->InitServerThreadInfo(hCompletionPort, hRevcSocket, pBaseSocket, pSocketPoolManager, pMemoryPoolManager);
				if(-1 != pThread->ResumeThread())
				{
					m_vecServerThread.push_back(pThread);
					continue;
				}
				
				CloseServerThreads();
				return false;
			}
		}
		return false;
	}
	return true;
}

bool CSocketServerThreadPool::CloseServerThreads()
{
	if(m_hCompletionPort)
	{
		if(m_vecServerThread.size() > 0)
		{
			BOOL bTest = FALSE;
			int  nThreadSize = 0;
			HANDLE * hThreadHandle = NULL;
			vector<CSocketServerThread *>::iterator iterServerThread;

			nThreadSize = m_vecServerThread.size();
			hThreadHandle = new HANDLE[nThreadSize];
			iterServerThread = m_vecServerThread.begin();

			for(int nIndex = 0; nIndex < nThreadSize; ++iterServerThread, ++nIndex)
			{
				hThreadHandle[nIndex] = (*iterServerThread)->GetThreaadHandle();
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

			for (iterServerThread = m_vecServerThread.begin(); iterServerThread != m_vecServerThread.end(); ++iterServerThread)
			{
				delete (*iterServerThread);
			}

			m_vecServerThread.clear();
		}
	}

	return true;
}