// Re-mine.LogPlugin.FileSystemLog.h : Re-mine.LogPlugin.FileSystemLog DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CRemineLogPluginFileSystemLogApp
// 有关此类实现的信息，请参阅 Re-mine.LogPlugin.FileSystemLog.cpp
//

class CLogFileSystemApp : public CWinApp
{
public:
	CLogFileSystemApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
