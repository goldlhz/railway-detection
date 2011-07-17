// SystemConfigure.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "SystemConfigure.h"
#include "afxdialogex.h"


// CSystemConfigure 对话框

IMPLEMENT_DYNAMIC(CSystemConfigure, CDialogEx)

CSystemConfigure::CSystemConfigure(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSystemConfigure::IDD, pParent)
{

}

CSystemConfigure::~CSystemConfigure()
{
}

void CSystemConfigure::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSystemConfigure, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_CONFIGUREMODIFY, &CSystemConfigure::OnBnClickedBtnConfiguremodify)
    ON_BN_CLICKED(IDC_BTN_CONFIGUREDEFAULT, &CSystemConfigure::OnBnClickedBtnConfiguredefault)
END_MESSAGE_MAP()


// CSystemConfigure 消息处理程序


void CSystemConfigure::OnBnClickedBtnConfiguremodify()
{
    // TODO: 在此添加控件通知处理程序代码
}


void CSystemConfigure::OnBnClickedBtnConfiguredefault()
{
    // TODO: 在此添加控件通知处理程序代码
}
