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

	virtual unsigned int ExecuteThread();											// 需要用户实现的线程函数

	bool InitListenThreadInstance();
	void UnInitListenThreadInstance();

private:
	bool CreateIOCPHandle();														// 创建一个完成端口
	bool CreateServerSocket(int nPort);												// 创建套接字
	bool CreateServerThreadPool();													// 创建工作者线程池
	bool CloseServerThreadPool();													// 关闭工作者线程池
	bool CloseServerSocket();														// 关闭套接字
	bool CloseIOCPHandle();															// 关闭完成端口
	bool PostAcceptExMSG();															// 投递套接字接收消息
	void ReleaseAllSource();														// 释放所有系统资源,但不退出线程

	HANDLE					m_hJudgeEvent[JUDGEHANDLESIZE];
	HANDLE					m_hCompletionPort;
	CBaseSocket				m_BaseSocket;
	CSocketServerThreadPool m_ServerThreadPool;										// 工作者线程池
	CSocketPoolManager		m_SocketPoolManager;									// 套接字池
	CMemoryPoolManager		m_MemoryPoolManager;									// 内存池
};
