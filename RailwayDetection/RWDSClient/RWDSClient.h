
// RWDSClient.h : RWDSClient Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CRWDSClientApp:
// �йش����ʵ�֣������ RWDSClient.cpp
//

class CRWDSClientApp : public CWinAppEx
{
public:
	CRWDSClientApp();

public:
    CString m_LoginAccount;
    CString m_LoginPassword;

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRWDSClientApp theApp;
