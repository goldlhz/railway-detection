#pragma once
#include "SocketListenThread.h"

class CIOCPManager
{
public:
	CIOCPManager(void);
	virtual ~CIOCPManager(void);

	void RunService(HINSTANCE hInstance);										// ���к󲻻᷵��	
	void PostCloseMSG();														// Ͷ��һ����Ϣ�Թر��������еķ���


private:
	void UnInitGlobalEnvironment();
	bool InitGlobalEnvironment(HINSTANCE hInstance);
	void OpenListenManager(HANDLE hWaitEvent);

	HMODULE					m_hInstance;
	const wstring			m_strGolbalEventName;								// ȫ���¼���
	CSocketListenThread     m_sltListenThread;									// �����߳�
};
