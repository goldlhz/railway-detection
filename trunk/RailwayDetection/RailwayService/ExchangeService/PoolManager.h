#pragma once
#include <WinSock2.h>
#include "CommonFunction.h"

class CSocketPoolManager
{
public:
	CSocketPoolManager();
	virtual ~CSocketPoolManager();

public:
	SOCKET GetNewSocket();													// 从池中行到一个SOCKET
	void ReleaseSocket(SOCKET scSocket);									// 释放一个SOCKET到池中
	int  GetUsedSocketPoolSize(){return (int)m_vecUsedSocket.size();};
	int  GetPrepSocketPoolSize(){return (int)m_vecPrepSocket.size();};
	void CloseSpecSocket(SOCKET scSocket);									// 从池中关闭一个socket
	bool InitSocketPool();													// 初始化Socket池
	void ClearSocketPool();													// 清理Socket池

private:
	SOCKET CreatePoolSocket();
	bool SocketPoolAutoManage();
	bool IncrementSocketPoolSize(int nCurrentSize);
	void DecrescenceSocketPoolSize(int nCurrentSize);

	typedef list<SOCKET> CListSocket;
	CCriticalSection m_scSocketCriticalSection;								// 内存池保护临界区
	CListSocket m_vecUsedSocket;											// 已经在使用的套接字
	CListSocket m_vecPrepSocket;											// 没有使用的套接字
};

class CMemoryPoolManager
{
public:
	CMemoryPoolManager();
	virtual ~CMemoryPoolManager();

public:
	KeyOverPire * GetNewKeyOverPire();										// 从池中得到一个Pire
	void ReleaseKeyOverPire(KeyOverPire * pPire);							// 释放一个Pire到池中
	int  GetUsedMemoryPoolSize(){return (int)m_vecUsedKeyOverPire.size();};
	int  GetPrepMemoryPoolSize(){return (int)m_vecPrepKeyOverPire.size();};
	bool InitMemoryPool();													// 初始化内存池
	void ClearMemoryPool();													// 清空Pire池

private:
	bool MemoryPoolAutoManage();											// 扫描池，并作内存池自动管理工作
	bool IncrementMemoryPoolSize(int nCurrentSize);							// 增大池大小
	void DecrescenceMemoryPoolSize(int nCurrentSize);						// 减小池大小

	typedef list<KeyOverPire *> CListKeyOverPire;
	CCriticalSection m_scMemoryCriticalSection;								// 内存池保护临界区
	CListKeyOverPire m_vecUsedKeyOverPire;									// 已经在使用的完成重叠对
	CListKeyOverPire m_vecPrepKeyOverPire;									// 没有使用的完成重叠对
};
