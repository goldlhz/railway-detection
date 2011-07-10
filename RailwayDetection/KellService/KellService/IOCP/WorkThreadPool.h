#pragma once

class CSocketPool;
class CMemoryPool;
class CWorkThread;
class CBaseSocket;
class CGobalConfig;
class CWorkThreadPool
{
public:
	CWorkThreadPool(void);
	virtual ~CWorkThreadPool(void);

	bool CreateWorkThreads(
		HANDLE hCompletionPort, 
		HANDLE hRevcNotify,
		CBaseSocket * pBaseSocket, 
		CSocketPool * pSocketPool, 
		CMemoryPool * pMemoryPool,
		CGobalConfig* pGobalConfig
		);

	bool CloseWorkThreads();

private:
	vector<CWorkThread *> m_vecServerThread;

	HANDLE m_hCompletionPort;
	HANDLE m_hRevcNotify;
	CBaseSocket * m_pBaseSocket; 
	CSocketPool * m_pSocketPool; 
	CMemoryPool * m_pMemoryPool;
	CGobalConfig  * m_pGobalConfig;
};

