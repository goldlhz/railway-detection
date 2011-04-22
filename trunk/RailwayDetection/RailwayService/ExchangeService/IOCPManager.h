#pragma once
#include "SocketListenThread.h"

class CIOCPManager
{
public:
	CIOCPManager(void);
	virtual ~CIOCPManager(void);

	void RunService();															// ���к󲻻᷵��	
	void PostCloseMSG();														// Ͷ��һ����Ϣ�Թر��������еķ���

private:
	void UnInitGlobalEnvironment();
	bool InitGlobalEnvironment();
	void OpenListenManager(HANDLE hWaitEvent);

	const wstring			m_strGolbalEventName;								// ȫ���¼���
	CSocketListenThread     m_sltListenThread;									// �����߳�
};
