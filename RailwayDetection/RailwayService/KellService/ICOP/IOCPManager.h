#pragma once
#include "ListenThread.h"
#include "GobalConfig.h"

class CIOCPManager
{
public:
	CIOCPManager(void);
	virtual ~CIOCPManager(void);

	void InitManagerPar(HINSTANCE hInstance, LPTSTR lpCmdLine);					// 传递系统的参数
	void RunService();															// 运行后不会返回	
	void PostCloseMSG();														// 投递一个消息以关闭正在运行的服务

private:
	void UnInitGlobalEnvironment();
	bool InitGlobalEnvironment(HINSTANCE hInstance);
	void RunListenManager(HANDLE hWaitEvent);

	HMODULE		  m_hInstance;
	wstring		  m_strComLine;													// 命令行参数
	wstring		  m_strGolbalEventName;											// 全局事件名
	CListenThread m_listenThread;												// 监听线程
	CGobalConfig  m_GobalConfig;												// 全局配置
};

