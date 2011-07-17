// AboutLoger.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "AboutLoger.h"
#include "afxdialogex.h"


// CAboutLoger �Ի���

IMPLEMENT_DYNAMIC(CAboutLoger, CDialogEx)

CAboutLoger::CAboutLoger(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAboutLoger::IDD, pParent)
{

}

CAboutLoger::~CAboutLoger()
{
}

void CAboutLoger::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutLoger, CDialogEx)
END_MESSAGE_MAP()

void CAboutLoger::SetName( CString aName )
{
    m_Name = aName;
}

void CAboutLoger::SetOrg( CString aOrg )
{
    m_Org =aOrg;
}


// CAboutLoger ��Ϣ�������


BOOL CAboutLoger::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_Name);
    GetDlgItem(IDC_STATIC_ORG)->SetWindowText(m_Org);
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}
