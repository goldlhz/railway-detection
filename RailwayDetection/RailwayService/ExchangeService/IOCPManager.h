#pragma once
#include "SocketListenThread.h"

class CIOCPManager
{
public:
	CIOCPManager(void);
	virtual ~CIOCPManager(void);

	void RunService();															// 运行后不会返回	
	void PostCloseMSG();														// 投递一个消息以关闭正在运行的服务

private:
	void UnInitGlobalEnvironment();
	bool InitGlobalEnvironment();
	void OpenListenManager(HANDLE hWaitEvent);

	const wstring			m_strGolbalEventName;								// 全局事件名
	CSocketListenThread     m_sltListenThread;									// 监听线程
};
