#pragma once
#include <WinSock2.h>

class CGobalConfig;
class CSocketPool
{
public:
	CSocketPool();
	virtual ~CSocketPool();

public:
	SOCKET GetNewSocket();													// �ӳ��еõ�һ��SOCKET
	void ReleaseSocket(SOCKET scSocket);									// �ͷ�һ��SOCKET������
	unsigned int GetUsedSocketPoolSize(){return m_vecUsedSocket.size();};
	unsigned int GetPrepSocketPoolSize(){return m_vecPrepSocket.size();};
	void CloseSpecSocket(SOCKET scSocket);									// �ӳ��йر�һ��socket
	bool InitSocketPool(CGobalConfig *pGobalConfig);						// ��ʼ��Socket��
	void ClearSocketPool();													// ����Socket��
	void RecycleSocket();													// ���ն���ĳ���Դ
private:
	SOCKET CreatePoolSocket();
	bool SocketPoolAutoManage();
	bool IncrementSocketPoolSize(unsigned int nCurrentSize);
	bool DecrescenceSocketPoolSize(unsigned int nCurrentSize);

	typedef list<SOCKET> CListSocket;										// ��������
	CGobalConfig    *m_pGobalConfig;										// ȫ������
	CCriticalSection m_scSocketCriticalSection;								// �ڴ�ر����ٽ���
	CListSocket m_vecUsedSocket;											// �Ѿ���ʹ�õ��׽���
	CListSocket m_vecPrepSocket;											// û��ʹ�õ��׽���
};
