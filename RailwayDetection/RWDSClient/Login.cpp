// Login.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "Login.h"
#include "afxdialogex.h"
#include "DataService.h"
#include "ErrorDefine.h"


// CLogin 对话框

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogin::IDD, pParent)
{
    m_LoginAccount = _T("");
    m_LoginPassword = _T("");
}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
    ON_BN_CLICKED(IDOK, &CLogin::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CLogin::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLogin 消息处理程序


void CLogin::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    GetDlgItem(IDC_EDIT_ACCOUNT)->GetWindowText(m_LoginAccount);
    GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(m_LoginPassword);
    if (m_LoginAccount == _T(""))
    {
        AfxMessageBox(_T("请输入账户名"));
        return;
    }
    GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowText(_T("正在验证身份，请稍后"));
    if(VerifyLogin(m_LoginAccount, m_LoginPassword, &m_LoginOrgID, &m_LoginPermission) != KErrNone)
    {
        GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowText(_T("登录失败，用户名或密码错误"));
        return;
    }
    CDialogEx::OnOK();
}


void CLogin::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    CDialogEx::OnCancel();
}

CString CLogin::GetLoginAccount()
{
    return m_LoginAccount;
}

void CLogin::SetLoginAccount( const CString& aAccount )
{
    m_LoginAccount = aAccount;
}

CString CLogin::GetLoginPassword()
{
    return m_LoginPassword;
}

void CLogin::SetLoginPassword( const CString& aPassword )
{
    m_LoginPassword = aPassword;
}

int CLogin::GetLoginOrgID()
{
    return m_LoginOrgID;
}

Permission CLogin::GetLoginPermission()
{
    return m_LoginPermission;
}
