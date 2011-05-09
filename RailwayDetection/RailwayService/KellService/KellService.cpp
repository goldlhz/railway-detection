// KellService.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "KellService.h"
#include "icop/IOCPManager.h"


int APIENTRY _tWinMain(	
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	CIOCPManager iocpmanager;

	iocpmanager.InitManagerPar(hInstance, lpCmdLine);
	iocpmanager.RunService();

	return 0;
}
