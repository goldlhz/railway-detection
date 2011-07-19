// StaffLogs.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "StaffLogs.h"
#include "afxdialogex.h"
#include "DataService.h"
#include "CmdDefine.h"


// CStaffLogs 对话框

IMPLEMENT_DYNAMIC(CStaffLogs, CDialogEx)

CStaffLogs::CStaffLogs(int aStartWay, CWnd* pParent /*=NULL*/)
	: CDialogEx(CStaffLogs::IDD, pParent)
{
    m_RWDSClientView = (CRWDSClientView*) pParent;
    m_StartWay = aStartWay;
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
    ON_BN_CLICKED(IDCANCEL, &CStaffLogs::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BTN_SEARCH, &CStaffLogs::OnBnClickedBtnSearch)
END_MESSAGE_MAP()


// CStaffLogs 消息处理程序

void CStaffLogs::SetReportInfo(ReportInfo* aReport)
{
    m_Report = aReport;
}

void CStaffLogs::GetLogs(int aCmd)
{
    m_Detail.iDay.clear();
    m_Detail.iActualArrivedTime.clear();
    m_Detail.iPlanArrivedTime.clear();
    m_Detail.iState.clear();
    if (aCmd == CMD_GET_NORMALREPORT)
    {
        if (!m_Report)
        {
            return;
        }
        GetReportDetail(m_Report->iStaffName, m_Report->iReportDay, &m_Detail);
    }
    else if (aCmd == CMD_GET_ALERTREPORT)
    {
        if (m_RWDSClientView)
        {
            int tmp, year, month;
            tmp = _ttoi(m_AlarmMonth);
            year = tmp / 100;
            month = tmp % 100;
            GetAlarmByMonth(m_RWDSClientView->m_CurrentOrg->iOrgID, year, month, &m_AlarmReport);
        }
    }
}

void CStaffLogs::ShowStaffInfo( CString aInfo )
{
    m_ShowInfo = aInfo;
}

void CStaffLogs::SetTitle(CString aTitle)
{
    m_Title = aTitle;
}


BOOL CStaffLogs::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    if (!m_Report && m_StartWay == 1)
    {
        return FALSE;
    }
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);
    //m_ListCtrl.InsertColumn(0, _T("地点"), LVCFMT_LEFT, clientRect.Width()/4);
    m_ListCtrl.InsertColumn(0, _T("线路名"), LVCFMT_LEFT, clientRect.Width()/6);
    m_ListCtrl.InsertColumn(1, _T("公里处"), LVCFMT_LEFT, clientRect.Width()/6);
    m_ListCtrl.InsertColumn(2, _T("员工号"), LVCFMT_LEFT, clientRect.Width()/6);
    m_ListCtrl.InsertColumn(3, _T("应到时间"), LVCFMT_LEFT, clientRect.Width()/6);
    m_ListCtrl.InsertColumn(4, _T("实到时间"), LVCFMT_LEFT, clientRect.Width()/6);
    m_ListCtrl.InsertColumn(5, _T("状态"), LVCFMT_LEFT, clientRect.Width()/6);
    
    CFont font;
    font.CreateFont(0, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,   
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_MODERN, _T("宋体")); 
    //GetDlgItem(IDC_STATIC_TITILE)->GetDC()->SetTextColor(RGB(255,0,0));
    GetDlgItem(IDC_STATIC_TITILE)->SetFont(&font);
    GetDlgItem(IDC_STATIC_TITILE)->SetWindowText(m_Title);

    if (m_StartWay == 1)
    {//查看员工明细
        SetAlarmViewVisible(FALSE);
        GetDlgItem(IDC_STAFFLOGS)->SetWindowText(m_ShowInfo);
        ShowCtrlList(m_Detail);
    }
    else if (m_StartWay == 2)
    {
        SetStaffLogsVisible(FALSE);
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CStaffLogs::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    CDialogEx::OnCancel();
}


void CStaffLogs::OnBnClickedBtnSearch()
{
    // TODO: 在此添加控件通知处理程序代码
    
    GetDlgItem(IDC_BTN_SEARCH)->GetWindowText(m_AlarmMonth);
    GetLogs(CMD_GET_ALERTREPORT);
    ShowCtrlList(m_AlarmReport);
}

void CStaffLogs::SetAlarmViewVisible(BOOL aVisible)
{
    int show;
    if (aVisible)
    {
        show = SW_SHOW;
    }
    else
    {
        show = SW_HIDE;
    }
    GetDlgItem(IDC_STATIC_TIME)->ShowWindow(show);
    GetDlgItem(IDC_EDIT_REVIEWMONTH)->ShowWindow(show);
    GetDlgItem(IDC_STATIC_EG)->ShowWindow(show);
    GetDlgItem(IDC_BTN_SEARCH)->ShowWindow(show);
}

void CStaffLogs::SetStaffLogsVisible(BOOL aVisible)
{
    int show;
    if (aVisible)
    {
        show = SW_SHOW;
    }
    else
    {
        show = SW_HIDE;
    }
    GetDlgItem(IDC_STAFFLOGS)->ShowWindow(show);
}

void CStaffLogs::ShowCtrlList(ReportDetail& aReport)
{
    for (size_t i=0; i<aReport.iActualArrivedTime.size() 
        && i<aReport.iPlanArrivedTime.size() 
        && i<aReport.iState.size()
        && i<aReport.iDay.size()
        && i<aReport.iPointKM.size()
        && i<aReport.iStaffID.size(); i++)
    {
        int state = aReport.iState[i] < strPointStateCount ? aReport.iState[i]:0;
        //int railID = aReport.iRailLineID[i];
        CString strPointKM;
        strPointKM.Format(_T("%.2f"), aReport.iPointKM[i]);
        m_ListCtrl.InsertItem(i, aReport.iRailLineID[i]);
        m_ListCtrl.SetItemText(i, 1, strPointKM);
        m_ListCtrl.SetItemText(i, 2, aReport.iStaffID[i]);
        m_ListCtrl.SetItemText(i, 3, aReport.iPlanArrivedTime[i]);
        m_ListCtrl.SetItemText(i, 4, aReport.iActualArrivedTime[i]);
        m_ListCtrl.SetItemText(i, 5, strPointState[state]);
    }
}
