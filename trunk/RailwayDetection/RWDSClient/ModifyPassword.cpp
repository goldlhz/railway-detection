// ModifyPassword.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "ModifyPassword.h"
#include "afxdialogex.h"


// CModifyPassword �Ի���

IMPLEMENT_DYNAMIC(CModifyPassword, CDialogEx)

CModifyPassword::CModifyPassword(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModifyPassword::IDD, pParent)
{

}

CModifyPassword::~CModifyPassword()
{
}

void CModifyPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CModifyPassword, CDialogEx)
    ON_BN_CLICKED(IDOK, &CModifyPassword::OnBnClickedOk)
END_MESSAGE_MAP()


// CModifyPassword ��Ϣ�������


void CModifyPassword::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString newPassword;
    CString confirmPassword;
    CString originalPassword;
    GetDlgItem(IDC_EDIT_NEWPASSWORD)->GetWindowText(newPassword);
    GetDlgItem(IDC_EDIT_CONFIRMPASSWORD)->GetWindowText(confirmPassword);
    if (newPassword.Compare(confirmPassword) != 0)
    {
        AfxMessageBox(_T("�����벻һ�£�����������"));
        return;
    }
    m_Password = newPassword;
    CDialogEx::OnOK();
}

void CModifyPassword::SetPassword( CString aPassword )
{
    m_Password = aPassword;
}

CString CModifyPassword::GetPassword()
{
    return m_Password;
}
