// KellService.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "KellService.h"
#include "IOCPManager.h"

// windows service function
VOID ServiceMainProc();
VOID Install(char* pPath, char* pName);
VOID UnInstall(char* pName);
BOOL KillService(char* pName);
BOOL RunService(char* pName);
VOID ExecuteSubProcess();
VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
VOID WINAPI ServiceHandler(DWORD fdwControl);

// windows service values
HINSTANCE				ghInstance;
LPTSTR					glpCmdLine;
CHAR					pServiceName[MAX_PATH];
CHAR					lpCmdLineData[MAX_PATH];
CRITICAL_SECTION		myCS;
SERVICE_STATUS_HANDLE   hServiceStatusHandle; 
SERVICE_STATUS          ServiceStatus; 
SERVICE_TABLE_ENTRY		lpServiceStartTable[] = 
{
	{pServiceName, ServiceMain},
	{NULL, NULL}
};

// entry function
int APIENTRY _tWinMain(	
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	ghInstance = hInstance;
	glpCmdLine = lpCmdLine;

	//CIOCPManager iocpManger;
	//iocpManger.InitSysParam(ghInstance, glpCmdLine);
	//iocpManger.RunService();


	strcpy(lpCmdLineData, lpCmdLine);
	ServiceMainProc();

	return 0;
}

VOID ServiceMainProc()
{
	// initialize variables for .exe and .log file names
	char pModuleFile[MAX_PATH] = {0};
	DWORD dwSize = GetModuleFileName(NULL, pModuleFile, MAX_PATH);
	if(dwSize > 0)
	{
		strcpy(pServiceName, "KellService");

		if(_stricmp("-i",lpCmdLineData) == 0 || _stricmp("-I",lpCmdLineData) == 0)
			Install(pModuleFile, pServiceName);

		else if(_stricmp("-k",lpCmdLineData) == 0 || _stricmp("-K",lpCmdLineData) == 0)
			KillService(pServiceName);

		else if(_stricmp("-u",lpCmdLineData) == 0 || _stricmp("-U",lpCmdLineData) == 0)
			UnInstall(pServiceName);

		else if(_stricmp("-s",lpCmdLineData) == 0 || _stricmp("-S",lpCmdLineData) == 0)
			RunService(pServiceName);

		else
			ExecuteSubProcess();
	}
}

VOID Install(char* pPath, char* pName)
{  
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_CREATE_SERVICE); 
	if (schSCManager==0) 
	{
		::MessageBox(NULL, _T("注册系统服务KellService过程中，打开服务控制管理器出错，注册失败！"), _T("出错"), MB_OK);
		return;
	}

	SC_HANDLE schService = CreateService
		( 
		schSCManager,	/* SCManager database      */ 
		pName,			/* name of service         */ 
		pName,			/* service name to display */ 
		SERVICE_ALL_ACCESS,        /* desired access          */ 
		SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS , /* service type            */ 
		SERVICE_AUTO_START,      /* start type              */ 
		SERVICE_ERROR_NORMAL,      /* error control type      */ 
		pPath,			/* service's binary        */ 
		NULL,                      /* no load ordering group  */ 
		NULL,                      /* no tag identifier       */ 
		NULL,                      /* no dependencies         */ 
		NULL,                      /* LocalSystem account     */ 
		NULL
		);                     /* no password             */ 
	if (schService==0) 
	{
		::MessageBox(NULL, _T("注册系统服务KellService过程中，写入系统服务信息出错，注册失败！"), _T("出错"), MB_OK);
		return;
	}
	else
	{
		::MessageBox(NULL, _T("注册系统服务KellService成功！"), _T("成功"), MB_OK);
		CloseServiceHandle(schService); 
	}
	CloseServiceHandle(schSCManager);
}

VOID UnInstall(char* pName)
{
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager==0) 
	{
		::MessageBox(NULL, _T("打开系统服务管理器出错，未能删除服务！"), _T("出错"), MB_OK);
		return;
	}

	SC_HANDLE schService = OpenService( schSCManager, pName, SERVICE_ALL_ACCESS);
	if (schService==0) 
	{
		::MessageBox(NULL, _T("打开系统服务出错，未能删除服务！"), _T("出错"), MB_OK);
		CloseServiceHandle(schSCManager);	
		return;
	}

	if(!DeleteService(schService)) 
	{
		::MessageBox(NULL, _T("删除系统KellService服务失败！"), _T("出错"), MB_OK);
	}
	else 
	{
		::MessageBox(NULL, _T("删除系统KellService服务成功！"), _T("成功"), MB_OK);
	}

	CloseServiceHandle(schService); 
	CloseServiceHandle(schSCManager);	
}

BOOL KillService(char* pName) 
{ 
	// kill service with given name
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager==0) 
	{
		::MessageBox(NULL, _T("打开系统服务管理器出错，未能停止服务！"), _T("出错"), MB_OK);
		return FALSE;
	}

	// open the service
	SC_HANDLE schService = OpenService( schSCManager, pName, SERVICE_ALL_ACCESS);
	if (schService==0) 
	{
		::MessageBox(NULL, _T("打开系统服务出错，未能停止服务！"), _T("出错"), MB_OK);
		CloseServiceHandle(schSCManager);	
		return FALSE;
	}

	// call ControlService to kill the given service
	SERVICE_STATUS status;
	if(ControlService(schService,SERVICE_CONTROL_STOP,&status))
	{
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager); 
		return TRUE;
	}

	::MessageBox(NULL, _T("停止服务过程中出现未知异常，未能停止服务！"), _T("出错"), MB_OK);

	CloseServiceHandle(schService); 
	CloseServiceHandle(schSCManager); 

	return FALSE;
}

BOOL RunService(char* pName) 
{ 
	// run service with given name
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager==0) 
	{
		::MessageBox(NULL, _T("打开系统服务管理器出错，未能启动服务！"), _T("出错"), MB_OK);
		return FALSE;
	}

	// open the service
	SC_HANDLE schService = OpenService( schSCManager, pName, SERVICE_ALL_ACCESS);
	if (schService==0) 
	{
		::MessageBox(NULL, _T("打开系统服务出错，未能启动服务！"), _T("出错"), MB_OK);
		CloseServiceHandle(schSCManager);	
		return FALSE;
	}

	// call StartService to run the service
	if(StartService(schService, 0, (const char**)NULL))
	{
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager); 
		return TRUE;
	}
	::MessageBox(NULL, _T("启动服务过程出错，未能启动服务！"), _T("出错"), MB_OK);

	CloseServiceHandle(schService); 
	CloseServiceHandle(schSCManager); 

	return FALSE;
}


VOID ExecuteSubProcess()
{
	if(!StartServiceCtrlDispatcher(lpServiceStartTable))
	{
		::MessageBox(NULL, _T("启动服务过程出错，未能启动服务！"), _T("出错"), MB_OK);
	}
}

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
	DWORD   status = 0; 
	DWORD   specificError = 0xfffffff; 

	ServiceStatus.dwServiceType        = SERVICE_WIN32; 
	ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
	ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE; 
	ServiceStatus.dwWin32ExitCode      = 0; 
	ServiceStatus.dwServiceSpecificExitCode = 0; 
	ServiceStatus.dwCheckPoint         = 0; 
	ServiceStatus.dwWaitHint           = 0; 

	hServiceStatusHandle = RegisterServiceCtrlHandler(pServiceName, ServiceHandler); 
	if (hServiceStatusHandle==0) 
	{
		::MessageBox(NULL, _T("RegisterServiceCtrlHandler failed"), _T("出错"), MB_OK);
		return; 
	} 

	// Initialization complete - report running status 
	ServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
	ServiceStatus.dwCheckPoint         = 0; 
	ServiceStatus.dwWaitHint           = 0;  
	if(!SetServiceStatus(hServiceStatusHandle, &ServiceStatus)) 
	{ 
		::MessageBox(NULL, _T("SetServiceStatus failed"), _T("出错"), MB_OK);
	} 

	CIOCPManager iocpManger;
	iocpManger.InitSysParam(ghInstance, glpCmdLine);
	iocpManger.RunService();
}


VOID WINAPI ServiceHandler(DWORD fdwControl)
{
	switch(fdwControl) 
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		ServiceStatus.dwWin32ExitCode = 0; 
		ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
		ServiceStatus.dwCheckPoint    = 0; 
		ServiceStatus.dwWaitHint      = 0;
		// terminate all processes started by this service before shutdown
		{
			CIOCPManager iocpManager;
			iocpManager.StopService();
		}
		break; 
	case SERVICE_CONTROL_PAUSE:
		ServiceStatus.dwCurrentState = SERVICE_PAUSED; 
		break;
	case SERVICE_CONTROL_CONTINUE:
		ServiceStatus.dwCurrentState = SERVICE_RUNNING; 
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	default:
		break;
	};
	if (!SetServiceStatus(hServiceStatusHandle,  &ServiceStatus)) 
	{ 
		::MessageBox(NULL, _T("SetServiceStatus failed"), _T("出错"), MB_OK);
	} 
}