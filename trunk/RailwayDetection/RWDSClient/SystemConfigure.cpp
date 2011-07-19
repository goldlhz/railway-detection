// SystemConfigure.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "SystemConfigure.h"
#include "afxdialogex.h"
#include "DataService.h"
#include "Datadef.h"


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
    
    CString day, minute;
    GetDlgItem(IDC_EDIT_LOGDURATION)->GetWindowText(day);
    GetDlgItem(IDC_EDIT_OVERTIME)->GetWindowText(minute);
    gSystemConfigure.iDuration = _ttoi(day);
    gSystemConfigure.iAlarmMinute = _ttoi(minute);
    SetSystemConfigure(&gSystemConfigure);
}


void CSystemConfigure::OnBnClickedBtnConfiguredefault()
{
    // TODO: 在此添加控件通知处理程序代码
    
    gSystemConfigure.iDuration = 90;
    gSystemConfigure.iAlarmMinute = 60;
    //SetSystemConfigure(&gSystemConfigure);

    CString day, minute;
    day.Format(_T("%d"), gSystemConfigure.iDuration);
    minute.Format(_T("%d"), gSystemConfigure.iAlarmMinute);
    GetDlgItem(IDC_EDIT_LOGDURATION)->SetWindowText(day);
    GetDlgItem(IDC_EDIT_OVERTIME)->SetWindowText(minute);
}


BOOL CSystemConfigure::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    GetSystemConfigure(&gSystemConfigure);
    CString day, minute;
    day.Format(_T("%d"), gSystemConfigure.iDuration);
    minute.Format(_T("%d"), gSystemConfigure.iAlarmMinute);
    GetDlgItem(IDC_EDIT_LOGDURATION)->SetWindowText(day);
    GetDlgItem(IDC_EDIT_OVERTIME)->SetWindowText(minute);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
