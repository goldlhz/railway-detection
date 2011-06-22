// Report.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "Report.h"
#include "afxdialogex.h"


// CReport 对话框

IMPLEMENT_DYNAMIC(CReport, CDialogEx)

CReport::CReport(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReport::IDD, pParent)
{

}

CReport::~CReport()
{
}

void CReport::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_REPORT, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CReport, CDialogEx)
END_MESSAGE_MAP()


// CReport 消息处理程序


BOOL CReport::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);
    m_ListCtrl.InsertColumn(0, _T("员工号"), LVCFMT_LEFT, clientRect.Width()/4);
    m_ListCtrl.InsertColumn(1, _T("姓名"), LVCFMT_LEFT, clientRect.Width()/4);
    m_ListCtrl.InsertColumn(2, _T("出勤次数"), LVCFMT_LEFT, clientRect.Width()/4);
    m_ListCtrl.InsertColumn(3, _T("异常次数"), LVCFMT_LEFT, clientRect.Width()/4);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
