#pragma once
#include "BaseThread.h"
#include "IOCPDefine.h"

class CSocketPool;
class CMemoryPool;
class CBaseSocket;

class CWorkThread : public CBaseThread
{
public:
	CWorkThread(void);
	virtual ~CWorkThread(void);

	virtual unsigned int ExecuteThread();
	bool InitWorkThreadInfo(
		HANDLE hCompletionPort, 
		HANDLE hRevcNotify,
		CBaseSocket * pBaseSocket, 
		CSocketPool * pSocketPoolManager, 
		CMemoryPool * pMemoryPoolManager
		);

private:
	void DealRevcDate(DWORD dNumberOfBytes,	LPOverKeyPire pKeyOverPire);
	void DealSendDate(DWORD dNumberOfBytes,	LPOverKeyPire pKeyOverPire);	
	void DealAccpDate(DWORD dNumberOfBytes,	LPOverKeyPire pKeyOverPire);
	void DealPrepRevcDate(DWORD dNumberOfBytes,	LPOverKeyPire pKeyOverPire);

	bool PostRevcMSG(SOCKET scOptSocket, LPOverKeyPire pKeyOverPire);
	bool PostSendMSG(SOCKET scOptSocket, LPOverKeyPire pKeyOverPire);

	bool   m_bIsInitWorkThread;
	HANDLE m_hCompletionPort;
	HANDLE m_hRevcNotify;
	CBaseSocket * m_pBaseSocket;
	CSocketPool * m_pSocketPool;
	CMemoryPool * m_pMemoryPool;
};

