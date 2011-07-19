// SystemConfigure.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "SystemConfigure.h"
#include "afxdialogex.h"
#include "DataService.h"
#include "Datadef.h"


// CSystemConfigure �Ի���

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


// CSystemConfigure ��Ϣ�������


void CSystemConfigure::OnBnClickedBtnConfiguremodify()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    
    CString day, minute;
    GetDlgItem(IDC_EDIT_LOGDURATION)->GetWindowText(day);
    GetDlgItem(IDC_EDIT_OVERTIME)->GetWindowText(minute);
    gSystemConfigure.iDuration = _ttoi(day);
    gSystemConfigure.iAlarmMinute = _ttoi(minute);
    SetSystemConfigure(&gSystemConfigure);
}


void CSystemConfigure::OnBnClickedBtnConfiguredefault()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    
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

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    GetSystemConfigure(&gSystemConfigure);
    CString day, minute;
    day.Format(_T("%d"), gSystemConfigure.iDuration);
    minute.Format(_T("%d"), gSystemConfigure.iAlarmMinute);
    GetDlgItem(IDC_EDIT_LOGDURATION)->SetWindowText(day);
    GetDlgItem(IDC_EDIT_OVERTIME)->SetWindowText(minute);
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}
