// Login.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "Login.h"
#include "afxdialogex.h"
#include "DataService.h"
#include "ErrorDefine.h"


// CLogin �Ի���

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


// CLogin ��Ϣ�������


void CLogin::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    GetDlgItem(IDC_EDIT_ACCOUNT)->GetWindowText(m_LoginAccount);
    GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(m_LoginPassword);
    if (m_LoginAccount == _T(""))
    {
        AfxMessageBox(_T("�������˻���"));
        return;
    }
    GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowText(_T("������֤��ݣ����Ժ�"));
    if(VerifyLogin(m_LoginAccount, m_LoginPassword, &m_LoginOrgID, &m_LoginPermission) != KErrNone)
    {
        GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowText(_T("��¼ʧ�ܣ��û������������"));
        return;
    }
    CDialogEx::OnOK();
}


void CLogin::OnBnClickedCancel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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
