#pragma once

class CBaseSocket;
class CSocketServerThread;
class CSocketPoolManager;
class CMemoryPoolManager;
class CSocketServerThreadPool
{
public:
	CSocketServerThreadPool(void);
	virtual ~CSocketServerThreadPool(void);

	bool CreateServerThreads(
		HANDLE hCompletionPort, 
		HANDLE hRevcSocket,
		CBaseSocket * pBaseSocket, 
		CSocketPoolManager * pSocketPoolManager, 
		CMemoryPoolManager * pMemoryPoolManager
		);
	bool CloseServerThreads();

private:
	vector<CSocketServerThread *> m_vecServerThread;

	HANDLE m_hCompletionPort;
	HANDLE m_hRevcSocket;
	CBaseSocket * m_pBaseSocket; 
	CSocketPoolManager * m_pSocketPoolManager; 
	CMemoryPoolManager * m_pMemoryPoolManager;
};
