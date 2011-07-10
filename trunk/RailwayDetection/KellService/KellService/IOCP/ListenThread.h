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
	virtual unsigned int ExecuteThread();											// 需要用户实现的线程函数
private:
	bool CreateIOCPHandle(CGobalConfig * pGobalConfig);								// 创建一个完成端口
	bool CreateListenSocket(int nPort);												// 创建套接字
	bool CreateWorkThreadPool();													// 创建工作者线程池
	bool CloseWorkThreadPool();														// 关闭工作者线程池
	bool CloseListenSocket();														// 关闭套接字
	bool CloseIOCPHandle();															// 关闭完成端口
	bool PostAcceptExMSG();															// 投递套接字接收消息
	void ReleaseAllSource();														// 释放所有系统资源,但不退出线程

	HANDLE			m_hJudgeEvent[JUDGEHANDLESIZE];
	HANDLE			m_hCompletionPort;
	CBaseSocket		m_sListenSocket;												// 监听套接字
	CWorkThreadPool	m_WorkThreadPool;												// 工作者线程池
	CSocketPool		m_SocketPool;													// 套接字池
	CMemoryPool		m_MemoryPool;													// 内存池
	CGobalConfig*   m_pGobalConfig;
};

