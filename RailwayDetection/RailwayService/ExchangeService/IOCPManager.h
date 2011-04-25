#pragma once
#include "SocketListenThread.h"

class CIOCPManager
{
public:
	CIOCPManager(void);
	virtual ~CIOCPManager(void);

	void RunService(HINSTANCE hInstance);										// 运行后不会返回	
	void PostCloseMSG();														// 投递一个消息以关闭正在运行的服务


private:
	void UnInitGlobalEnvironment();
	bool InitGlobalEnvironment(HINSTANCE hInstance);
	void OpenListenManager(HANDLE hWaitEvent);

	HMODULE					m_hInstance;
	const wstring			m_strGolbalEventName;								// 全局事件名
	CSocketListenThread     m_sltListenThread;									// 监听线程
};
