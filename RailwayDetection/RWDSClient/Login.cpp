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
    ON_WM_CTLCOLOR()
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

    //刷背景
    CRect rtlbl; 
    GetDlgItem(IDC_STATIC_LOGININFO)->GetWindowRect(&rtlbl); 
    ScreenToClient(&rtlbl);
    InvalidateRect(&rtlbl);//最后刷新对话框背景 

    GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowText(_T("正在验证身份，请稍后...."));

    int err = VerifyLogin(m_LoginAccount, m_LoginPassword, &m_LoginOrgID, &m_LoginPermission);

    //刷背景
    CRect rtlbl1; 
    GetDlgItem(IDC_STATIC_LOGININFO)->GetWindowRect(&rtlbl1); 
    ScreenToClient(&rtlbl1);
    InvalidateRect(&rtlbl1);//最后刷新对话框背景 

    if (err == KErrNotFound)
    {
        GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowText(_T("获取网络数据失败，请重新登录!"));
        return;
    }
    else if (err == KErrUnknowLink)
    {
        GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowText(_T("不能连接服务器!"));
        return;
    }
    else if(err == 1)
    {
        GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowText(_T("登录失败，用户名或密码错误"));
        return;
    }
    else if(err != KErrNone)
    {
        GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowText(_T("未知错误，请重新登录!"));
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

PermissionGroup CLogin::GetLoginPermission()
{
    return m_LoginPermission;
}


HBRUSH CLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  在此更改 DC 的任何特性
    int nID = pWnd->GetDlgCtrlID();
    if( nID == IDC_STATIC_LOGININFO) //对指定的控件设属性,你
    {
        pDC->SetTextColor(RGB(0,0,0) );
        pDC->SetBkMode(TRANSPARENT);
        return HBRUSH(GetStockObject(HOLLOW_BRUSH));
    }
    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}
