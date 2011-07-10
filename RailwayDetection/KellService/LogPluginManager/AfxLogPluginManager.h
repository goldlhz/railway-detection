// LogPluginManager.h : main header file for the LogPluginManager DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CLogPluginManagerApp
// See LogPluginManager.cpp for the implementation of this class
//

class CLogPluginManagerApp : public CWinApp
{
public:
	CLogPluginManagerApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
