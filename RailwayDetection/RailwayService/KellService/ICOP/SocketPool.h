#pragma once
#include <WinSock2.h>

class CGobalConfig;
class CSocketPool
{
public:
	CSocketPool();
	virtual ~CSocketPool();

public:
	SOCKET GetNewSocket();													// 从池中得到一个SOCKET
	void ReleaseSocket(SOCKET scSocket);									// 释放一个SOCKET到池中
	unsigned int GetUsedSocketPoolSize(){return m_vecUsedSocket.size();};
	unsigned int GetPrepSocketPoolSize(){return m_vecPrepSocket.size();};
	void CloseSpecSocket(SOCKET scSocket);									// 从池中关闭一个socket
	bool InitSocketPool(CGobalConfig *pGobalConfig);						// 初始化Socket池
	void ClearSocketPool();													// 清理Socket池
	void RecycleSocket();													// 回收多余的池资源
private:
	SOCKET CreatePoolSocket();
	bool SocketPoolAutoManage();
	bool IncrementSocketPoolSize(unsigned int nCurrentSize);
	bool DecrescenceSocketPoolSize(unsigned int nCurrentSize);

	typedef list<SOCKET> CListSocket;										// 定义类型
	CGobalConfig    *m_pGobalConfig;										// 全局配置
	CCriticalSection m_scSocketCriticalSection;								// 内存池保护临界区
	CListSocket m_vecUsedSocket;											// 已经在使用的套接字
	CListSocket m_vecPrepSocket;											// 没有使用的套接字
};
