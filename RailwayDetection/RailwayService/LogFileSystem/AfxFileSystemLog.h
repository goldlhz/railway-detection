// Re-mine.LogPlugin.FileSystemLog.h : Re-mine.LogPlugin.FileSystemLog DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRemineLogPluginFileSystemLogApp
// �йش���ʵ�ֵ���Ϣ������� Re-mine.LogPlugin.FileSystemLog.cpp
//

class CLogFileSystemApp : public CWinApp
{
public:
	CLogFileSystemApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
