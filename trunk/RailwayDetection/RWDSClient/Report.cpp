// Report.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "Report.h"
#include "afxdialogex.h"
#include "DataService.h"
#include "ErrorDefine.h"
#include "DataListControl.h"


// CReport 对话框

IMPLEMENT_DYNAMIC(CReport, CDialogEx)

CReport::CReport(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReport::IDD, pParent)
{
    m_RWDSClientView = (CRWDSClientView*) pParent;
}

CReport::~CReport()
{
}

void CReport::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_REPORT, m_ListCtrl);
    DDX_Control(pDX, IDC_COMBO_REPORTMONTH, m_ComboReportMonth);
}


BEGIN_MESSAGE_MAP(CReport, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_SEARCHREPORT, &CReport::OnBnClickedBtnSearchreport)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_REPORT, &CReport::OnNMDblclkListReport)
    ON_BN_CLICKED(IDC_BTN_VIEWDETAIL, &CReport::OnBnClickedBtnViewdetail)
    ON_BN_CLICKED(IDCANCEL, &CReport::OnBnClickedCancel)
END_MESSAGE_MAP()


// CReport 消息处理程序


BOOL CReport::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);
    //m_ListCtrl.InsertColumn(0, _T("员工号"), LVCFMT_LEFT, clientRect.Width()/4);
    m_ListCtrl.InsertColumn(0, _T("日期"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(1, _T("星期"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(2, _T("姓名"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(3, _T("计划记录"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(4, _T("实际记录"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(5, _T("异常次数"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(6, _T("异常次数"), LVCFMT_LEFT, clientRect.Width()/7);

    m_ComboReportMonth.AddString(_T("1月"));
    m_ComboReportMonth.AddString(_T("2月"));
    m_ComboReportMonth.AddString(_T("3月"));
    m_ComboReportMonth.AddString(_T("4月"));
    m_ComboReportMonth.AddString(_T("5月"));
    m_ComboReportMonth.AddString(_T("6月"));
    m_ComboReportMonth.AddString(_T("7月"));
    m_ComboReportMonth.AddString(_T("8月"));
    m_ComboReportMonth.AddString(_T("9月"));
    m_ComboReportMonth.AddString(_T("10月"));
    m_ComboReportMonth.AddString(_T("11月"));
    m_ComboReportMonth.AddString(_T("12月"));
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CReport::OnBnClickedBtnSearchreport()
{
    // TODO: 在此添加控件通知处理程序代码
    int month = m_ComboReportMonth.GetCurSel()+1;
    if (month <=0)
    {
        AfxMessageBox(_T("请选择月份"));
        return;
    }
    DeleteReportListElement(&m_RWDSClientView->m_CurrentOrg->iReportInfo);

    int err = GetReportInfoList(m_RWDSClientView->m_CurrentOrg->iOrgID, 2011, month, &m_RWDSClientView->m_CurrentOrg->iReportInfo);
    if (err == KErrNone)
    {
        m_ListCtrl.DeleteAllItems();
        ReportInfo* report = NULL;
        CString strDate;
        CString strPlanArrived;
        CString strActualArrived;
        CString strAbnormal;
        for(size_t i=0; i<m_RWDSClientView->m_CurrentOrg->iReportInfo.size(); i++)
        {
            report = m_RWDSClientView->m_CurrentOrg->iReportInfo[i];
            CTime rTime(report->iReportDay);
            strDate.Format(_T("%02d月%02d日"), rTime.GetMonth(), rTime.GetDay());
            strPlanArrived.Format(_T("%d"), report->iPlanArrived);
            strActualArrived.Format(_T("%d"), report->iActualArrived);
            strAbnormal.Format(_T("%d"), report->iAbnormal);

            int count = m_ListCtrl.GetItemCount();
            m_ListCtrl.InsertItem(count, strDate);
            m_ListCtrl.SetItemText(count, 1, report->iStaffName);
            m_ListCtrl.SetItemText(count, 2, strPlanArrived);
            m_ListCtrl.SetItemText(count, 3, strActualArrived);
            m_ListCtrl.SetItemText(count, 4, strAbnormal);
            m_ListCtrl.SetItemData(count, (DWORD_PTR)report);
        }
    }
}


void CReport::OnNMDblclkListReport(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    POSITION pos;
    pos = m_ListCtrl.GetFirstSelectedItemPosition();
    // 得到项目索引
    int select = m_ListCtrl.GetNextSelectedItem(pos);  
    if (select<0)
    {
        return;
    }
    ReportInfo* report = (ReportInfo*)m_ListCtrl.GetItemData(select);
    *pResult = 0;
}


void CReport::OnBnClickedBtnViewdetail()
{
    // TODO: 在此添加控件通知处理程序代码
    POSITION pos;
    pos = m_ListCtrl.GetFirstSelectedItemPosition();
    // 得到项目索引
    int select = m_ListCtrl.GetNextSelectedItem(pos);  
    if (select<0)
    {
        return;
    }
    ReportInfo* report = (ReportInfo*)m_ListCtrl.GetItemData(select);
}


void CReport::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    m_RWDSClientView->m_OpenReportForm = FALSE;
    CWnd::DestroyWindow();
    //CDialogEx::OnCancel();
}
