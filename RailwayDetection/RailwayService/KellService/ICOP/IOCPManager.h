#pragma once
#include "ListenThread.h"
#include "GobalConfig.h"

class CIOCPManager
{
public:
	CIOCPManager(void);
	virtual ~CIOCPManager(void);

	void InitManagerPar(HINSTANCE hInstance, LPTSTR lpCmdLine);					// ����ϵͳ�Ĳ���
	void RunService();															// ���к󲻻᷵��	
	void PostCloseMSG();														// Ͷ��һ����Ϣ�Թر��������еķ���

private:
	void UnInitGlobalEnvironment();
	bool InitGlobalEnvironment(HINSTANCE hInstance);
	void RunListenManager(HANDLE hWaitEvent);

	HMODULE		  m_hInstance;
	wstring		  m_strComLine;													// �����в���
	wstring		  m_strGolbalEventName;											// ȫ���¼���
	CListenThread m_listenThread;												// �����߳�
	CGobalConfig  m_GobalConfig;												// ȫ������
};

