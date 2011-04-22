// ExchangeService.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ExchangeService.h"
#include "IOCPManager.h"

void SetAppICON(HINSTANCE hInstance);
int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR    lpCmdLine,
                      int       nCmdShow)
{
	CIOCPManager iocpManger;
	iocpManger.RunService();
	return 0;
}
