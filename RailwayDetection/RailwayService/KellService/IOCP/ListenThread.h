#pragma once
#include <WinSock2.h>
#include <MSWSock.h>
#include "BaseThread.h"
#include "MemoryPool.h"
#include "SocketPool.h"
#include "BaseSocket.h"
#include "WorkThreadPool.h"

#define JUDGEHANDLESIZE																2
class CGobalConfig;
class CListenThread : public CBaseThread
{
public:
	CListenThread(void);
	virtual ~CListenThread(void);
	
	bool InitListener(CGobalConfig * pGobalConfig);
	void UnInitListener();
	virtual unsigned int ExecuteThread();											// ��Ҫ�û�ʵ�ֵ��̺߳���
private:
	bool CreateIOCPHandle(CGobalConfig * pGobalConfig);								// ����һ����ɶ˿�
	bool CreateListenSocket(int nPort);												// �����׽���
	bool CreateWorkThreadPool();													// �����������̳߳�
	bool CloseWorkThreadPool();														// �رչ������̳߳�
	bool CloseListenSocket();														// �ر��׽���
	bool CloseIOCPHandle();															// �ر���ɶ˿�
	bool PostAcceptExMSG();															// Ͷ���׽��ֽ�����Ϣ
	void ReleaseAllSource();														// �ͷ�����ϵͳ��Դ,�����˳��߳�

	HANDLE			m_hJudgeEvent[JUDGEHANDLESIZE];
	HANDLE			m_hCompletionPort;
	CBaseSocket		m_sListenSocket;												// �����׽���
	CWorkThreadPool	m_WorkThreadPool;												// �������̳߳�
	CSocketPool		m_SocketPool;													// �׽��ֳ�
	CMemoryPool		m_MemoryPool;													// �ڴ��
	CGobalConfig*   m_pGobalConfig;
};

