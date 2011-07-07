// StaffLogs.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "StaffLogs.h"
#include "afxdialogex.h"
#include "DataService.h"


// CStaffLogs 对话框

IMPLEMENT_DYNAMIC(CStaffLogs, CDialogEx)

CStaffLogs::CStaffLogs(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStaffLogs::IDD, pParent)
{
    m_Report = NULL;
}

CStaffLogs::~CStaffLogs()
{
}

void CStaffLogs::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_LOGS, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CStaffLogs, CDialogEx)
END_MESSAGE_MAP()


// CStaffLogs 消息处理程序

void CStaffLogs::SetReportInfo(ReportInfo* aReport)
{
    m_Report = aReport;
}


BOOL CStaffLogs::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    if (!m_Report)
    {
        return FALSE;
    }
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);
    //m_ListCtrl.InsertColumn(0, _T("地点"), LVCFMT_LEFT, clientRect.Width()/4);
    m_ListCtrl.InsertColumn(0, _T("应到时间"), LVCFMT_LEFT, clientRect.Width()/3);
    m_ListCtrl.InsertColumn(1, _T("实到时间"), LVCFMT_LEFT, clientRect.Width()/3);
    m_ListCtrl.InsertColumn(2, _T("状态"), LVCFMT_LEFT, clientRect.Width()/3);
    CString textShow;
    textShow.Format(_T("员工:%s, 计划到达: %d, 实际到达: %d, 未到达: %d, 异常: %d"),
                    m_Report->iStaffName, m_Report->iPlanArrived, m_Report->iActualArrived,
                    m_Report->iUnArrived, m_Report->iAbnormal);
    GetDlgItem(IDC_STAFFLOGS)->SetWindowText(textShow);

    m_Detail.iDay = _T("");
    m_Detail.iActualArrivedTime.clear();
    m_Detail.iPlanArrivedTime.clear();
    m_Detail.iState.clear();
    GetReportDetail(m_Report->iStaffName, m_Report->iReportDay, &m_Detail);
    for (size_t i=0; i<m_Detail.iActualArrivedTime.size() && i<m_Detail.iPlanArrivedTime.size() && i<m_Detail.iState.size(); i++)
    {
        m_ListCtrl.InsertItem(i, m_Detail.iPlanArrivedTime[i]);
        m_ListCtrl.SetItemText(i, 1, m_Detail.iPlanArrivedTime[i]);
        m_ListCtrl.SetItemText(i, 2, strPointState[m_Detail.iState[i]]);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
