// Re-mine.LogPlugin.LogPluginManager.h : Re-mine.LogPlugin.LogPluginManager DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRemineLogPluginLogPluginManagerApp
// �йش���ʵ�ֵ���Ϣ������� Re-mine.LogPlugin.LogPluginManager.cpp
//

class CLogPluginManagerApp : public CWinApp
{
public:
	CLogPluginManagerApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
