#pragma once
#include "ListenThread.h"
#include "GobalConfig.h"

class CIOCPManager
{
public:
	CIOCPManager(void);
	virtual ~CIOCPManager(void);

	void InitSysParam(HINSTANCE hInstance, LPTSTR lpCmdLine);					// ����ϵͳ�Ĳ���
	void RunService();															// ���к󲻻᷵��	
	void PostCloseMSG();														// Ͷ��һ����Ϣ�Թر��������еķ���

private:
	bool InitGlbEnvironment(HINSTANCE hInstance);
	void UnInitGlbEnvironment();
	void RunListener(HANDLE hWaitEvent);

private:
	HMODULE		  m_hInstance;
	string		  m_strComLine;													// �����в���
	const string  m_strGlbEvent;												// ȫ���¼���
	CListenThread m_listenThread;												// �����߳�
	CGobalConfig  m_GobalConfig;												// ȫ������
};

