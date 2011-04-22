#pragma once
#include <WinSock2.h>
#include "CommonFunction.h"

class CSocketPoolManager
{
public:
	CSocketPoolManager();
	virtual ~CSocketPoolManager();

public:
	SOCKET GetNewSocket();													// �ӳ����е�һ��SOCKET
	void ReleaseSocket(SOCKET scSocket);									// �ͷ�һ��SOCKET������
	int  GetUsedSocketPoolSize(){return (int)m_vecUsedSocket.size();};
	int  GetPrepSocketPoolSize(){return (int)m_vecPrepSocket.size();};
	void CloseSpecSocket(SOCKET scSocket);									// �ӳ��йر�һ��socket
	bool InitSocketPool();													// ��ʼ��Socket��
	void ClearSocketPool();													// ����Socket��

private:
	SOCKET CreatePoolSocket();
	bool SocketPoolAutoManage();
	bool IncrementSocketPoolSize(int nCurrentSize);
	void DecrescenceSocketPoolSize(int nCurrentSize);

	typedef list<SOCKET> CListSocket;
	CCriticalSection m_scSocketCriticalSection;								// �ڴ�ر����ٽ���
	CListSocket m_vecUsedSocket;											// �Ѿ���ʹ�õ��׽���
	CListSocket m_vecPrepSocket;											// û��ʹ�õ��׽���
};

class CMemoryPoolManager
{
public:
	CMemoryPoolManager();
	virtual ~CMemoryPoolManager();

public:
	KeyOverPire * GetNewKeyOverPire();										// �ӳ��еõ�һ��Pire
	void ReleaseKeyOverPire(KeyOverPire * pPire);							// �ͷ�һ��Pire������
	int  GetUsedMemoryPoolSize(){return (int)m_vecUsedKeyOverPire.size();};
	int  GetPrepMemoryPoolSize(){return (int)m_vecPrepKeyOverPire.size();};
	bool InitMemoryPool();													// ��ʼ���ڴ��
	void ClearMemoryPool();													// ���Pire��

private:
	bool MemoryPoolAutoManage();											// ɨ��أ������ڴ���Զ�������
	bool IncrementMemoryPoolSize(int nCurrentSize);							// ����ش�С
	void DecrescenceMemoryPoolSize(int nCurrentSize);						// ��С�ش�С

	typedef list<KeyOverPire *> CListKeyOverPire;
	CCriticalSection m_scMemoryCriticalSection;								// �ڴ�ر����ٽ���
	CListKeyOverPire m_vecUsedKeyOverPire;									// �Ѿ���ʹ�õ�����ص���
	CListKeyOverPire m_vecPrepKeyOverPire;									// û��ʹ�õ�����ص���
};
