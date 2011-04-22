#pragma once
#include "BaseThread.h"
#include "CommonFunction.h"

class CBaseSocket;
class CSocketPoolManager;
class CMemoryPoolManager;
class CSocketServerThread
	: public CBaseThread
{
public:
	CSocketServerThread(void);
	virtual ~CSocketServerThread(void);

	virtual unsigned int ExecuteThread();
	void InitServerThreadInfo(
		HANDLE hCompletionPort, 
		HANDLE hRevcSocket,
		CBaseSocket * pBaseSocket, 
		CSocketPoolManager * pSocketPoolManager, 
		CMemoryPoolManager * pMemoryPoolManager
		);

private:
	void DealRevcDate(DWORD dNumberOfBytes,	KeyOverPire * pKeyOverPire);
	void DealSendDate(DWORD dNumberOfBytes,	KeyOverPire * pKeyOverPire);	
	void DealAccpDate(DWORD dNumberOfBytes,	KeyOverPire * pKeyOverPire);
	void DealPrepRevcDate(DWORD dNumberOfBytes,	KeyOverPire * pKeyOverPire);

	bool PostRevcMSG();
	bool PostSendMSG();

	HANDLE m_hCompletionPort;
	HANDLE m_hRevcSocket;
	CBaseSocket        * m_pBaseSocket;
	CSocketPoolManager * m_pSocketPoolManager;
	CMemoryPoolManager * m_pMemoryPoolManager;
};
