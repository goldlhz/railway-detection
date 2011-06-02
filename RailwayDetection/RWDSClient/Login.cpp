// Login.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "Login.h"
#include "afxdialogex.h"


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
