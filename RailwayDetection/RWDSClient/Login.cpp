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
    ON_WM_CTLCOLOR()
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

    //ˢ����
    CRect rtlbl; 
    GetDlgItem(IDC_STATIC_LOGININFO)->GetWindowRect(&rtlbl); 
    ScreenToClient(&rtlbl);
    InvalidateRect(&rtlbl);//���ˢ�¶Ի��򱳾� 

    GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowText(_T("������֤��ݣ����Ժ�...."));

    int err = VerifyLogin(m_LoginAccount, m_LoginPassword, &m_LoginOrgID, &m_LoginPermission);

    //ˢ����
    CRect rtlbl1; 
    GetDlgItem(IDC_STATIC_LOGININFO)->GetWindowRect(&rtlbl1); 
    ScreenToClient(&rtlbl1);
    InvalidateRect(&rtlbl1);//���ˢ�¶Ի��򱳾� 

    if (err == KErrNotFound)
    {
        GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowText(_T("��ȡ��������ʧ�ܣ������µ�¼!"));
        return;
    }
    else if (err == KErrUnknowLink)
    {
        GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowText(_T("�������ӷ�����!"));
        return;
    }
    else if(err == 1)
    {
        GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowText(_T("��¼ʧ�ܣ��û������������"));
        return;
    }
    else if(err != KErrNone)
    {
        GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowText(_T("δ֪���������µ�¼!"));
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

PermissionGroup CLogin::GetLoginPermission()
{
    return m_LoginPermission;
}


HBRUSH CLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  �ڴ˸��� DC ���κ�����
    int nID = pWnd->GetDlgCtrlID();
    if( nID == IDC_STATIC_LOGININFO) //��ָ���Ŀؼ�������,��
    {
        pDC->SetTextColor(RGB(0,0,0) );
        pDC->SetBkMode(TRANSPARENT);
        return HBRUSH(GetStockObject(HOLLOW_BRUSH));
    }
    // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
    return hbr;
}
