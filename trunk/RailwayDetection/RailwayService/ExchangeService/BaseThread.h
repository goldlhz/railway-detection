#pragma once

class CBaseThread
{
public:
	CBaseThread(void);
	virtual ~CBaseThread(void);

	bool CreateThreadInstance(bool bIsRun);							// �̴߳������Ƿ���������
	bool ForceTerminate();											// ǿ�ƹر��߳�

	virtual unsigned int ExecuteThread() = 0;						// ��Ҫ�û�ʵ�ֵ��̺߳���

	DWORD ResumeThread();											// �����߳����Ҽ���
	DWORD SuspendThread();											// �����߳����Ҽ���
	void  SafeTerminate();											// ��ȫ��ֹ�߳�
	bool  IsTerminate(){return m_bIsTerminate;};
	HANDLE GetThreaadHandle(){return m_hThread;};
protected:
	bool m_bIsTerminate;											// ��ʶ�߳��Ƿ���Ҫ��ֹ

private:
	static unsigned int WINAPI Execute(PVOID pArguments);			// �̵߳���ں���

	HANDLE m_hThread;												// �߳̾��
	CCriticalSection m_csCritial;									// �ٽ���
};
