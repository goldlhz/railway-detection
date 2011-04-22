#pragma once
#include "basethread.h"
#include "BaseSocket.h"
#include "PoolManager.h"
#include "SocketServerThreadPool.h"

#define JUDGEHANDLESIZE																2
class CSocketListenThread :
	public CBaseThread
{
public:
	CSocketListenThread(void);
	virtual ~CSocketListenThread(void);

	virtual unsigned int ExecuteThread();											// ��Ҫ�û�ʵ�ֵ��̺߳���

	bool InitListenThreadInstance();
	void UnInitListenThreadInstance();

private:
	bool CreateIOCPHandle();														// ����һ����ɶ˿�
	bool CreateServerSocket(int nPort);												// �����׽���
	bool CreateServerThreadPool();													// �����������̳߳�
	bool CloseServerThreadPool();													// �رչ������̳߳�
	bool CloseServerSocket();														// �ر��׽���
	bool CloseIOCPHandle();															// �ر���ɶ˿�
	bool PostAcceptExMSG();															// Ͷ���׽��ֽ�����Ϣ
	void ReleaseAllSource();														// �ͷ�����ϵͳ��Դ,�����˳��߳�

	HANDLE					m_hJudgeEvent[JUDGEHANDLESIZE];
	HANDLE					m_hCompletionPort;
	CBaseSocket				m_BaseSocket;
	CSocketServerThreadPool m_ServerThreadPool;										// �������̳߳�
	CSocketPoolManager		m_SocketPoolManager;									// �׽��ֳ�
	CMemoryPoolManager		m_MemoryPoolManager;									// �ڴ��
};
