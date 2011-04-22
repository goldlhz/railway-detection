#pragma once

class CBaseThread
{
public:
	CBaseThread(void);
	virtual ~CBaseThread(void);

	bool CreateThreadInstance(bool bIsRun);							// 线程创建后是否马上运行
	bool ForceTerminate();											// 强制关闭线程

	virtual unsigned int ExecuteThread() = 0;						// 需要用户实现的线程函数

	DWORD ResumeThread();											// 减少线程悬挂计数
	DWORD SuspendThread();											// 增加线程悬挂计数
	void  SafeTerminate();											// 安全终止线程
	bool  IsTerminate(){return m_bIsTerminate;};
	HANDLE GetThreaadHandle(){return m_hThread;};
protected:
	bool m_bIsTerminate;											// 标识线程是否需要终止

private:
	static unsigned int WINAPI Execute(PVOID pArguments);			// 线程的入口函数

	HANDLE m_hThread;												// 线程句柄
	CCriticalSection m_csCritial;									// 临界区
};
