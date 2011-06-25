#pragma once
#include "BaseThread.h"
#include "IOCPDefine.h"

class CSocketPool;
class CMemoryPool;
class CBaseSocket;
class CGobalConfig;
class CADODatabase;
class CLogicManager;
class CWorkThread : public CBaseThread
{
public:
	CWorkThread(void);
	virtual ~CWorkThread(void);

	virtual unsigned int ExecuteThread();
	bool InitWorkThread(
		HANDLE hCompletionPort, 
		HANDLE hRevcNotify,
		CBaseSocket * pBaseSocket, 
		CSocketPool * pSocketPoolManager, 
		CMemoryPool * pMemoryPoolManager,
		CGobalConfig* pGobalConfig
		);

	// 返回-1 表示参数无效
	// 返回0  表示成功
	// 返回1 表示出错
	static int DealSendData(LPOverKeyPire pKeyOverPire, void* parameter);
	static int DealRecvData(LPOverKeyPire pKeyOverPire, void* parameter, string& strContext);

private:
	void DealRevcDatePack(DWORD dNumberOfBytes,	LPOverKeyPire pKeyOverPire);
	void DealSendDatePack(DWORD dNumberOfBytes,	LPOverKeyPire pKeyOverPire);	
	void DealAccpDatePack(DWORD dNumberOfBytes,	LPOverKeyPire pKeyOverPire);
	void DealPrepRevcDate(DWORD dNumberOfBytes,	LPOverKeyPire pKeyOverPire);

	bool PostRevcMSG(SOCKET scOptSocket, LPOverKeyPire pKeyOverPire, OptType optType = CT_REVC);
	bool PostSendMSG(SOCKET scOptSocket, LPOverKeyPire pKeyOverPire);

	string FormatConnectString(CGobalConfig* pGobalConfig);

	bool   m_bIsInitWorkThread;
	HANDLE m_hCompletionPort;
	HANDLE m_hRevcNotify;
	CBaseSocket * m_pBaseSocket;
	CSocketPool * m_pSocketPool;
	CMemoryPool * m_pMemoryPool;
	CGobalConfig* m_pGobalConfig;
	CADODatabase* m_pAdoDatabase;
	CLogicManager* m_pLogicManager;
};

