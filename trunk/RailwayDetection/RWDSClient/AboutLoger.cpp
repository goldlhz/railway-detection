// AboutLoger.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "AboutLoger.h"
#include "afxdialogex.h"


// CAboutLoger 对话框

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


// CAboutLoger 消息处理程序


BOOL CAboutLoger::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_Name);
    GetDlgItem(IDC_STATIC_ORG)->SetWindowText(m_Org);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
