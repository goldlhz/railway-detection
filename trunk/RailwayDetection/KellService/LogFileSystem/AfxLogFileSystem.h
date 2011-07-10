// LogFileSystem.h : main header file for the LogFileSystem DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CLogFileSystemApp
// See LogFileSystem.cpp for the implementation of this class
//

class CLogFileSystemApp : public CWinApp
{
public:
	CLogFileSystemApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
