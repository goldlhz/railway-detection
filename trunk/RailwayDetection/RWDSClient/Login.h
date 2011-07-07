#pragma once
#include "resource.h"
#include "Datadef.h"

// CLogin 对话框

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogin();

// 对话框数据
	enum { IDD = IDD_LOGIN };
public:
    CString GetLoginAccount();
    void SetLoginAccount(const CString& aAccount);
    CString GetLoginPassword();
    void SetLoginPassword(const CString& aPassword);
    int GetLoginOrgID();
    PermissionGroup GetLoginPermission();
private:
    CString m_LoginAccount;
    CString m_LoginPassword;
    int m_LoginOrgID;
    PermissionGroup m_LoginPermission;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
};
