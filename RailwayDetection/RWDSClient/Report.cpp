// Report.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "Report.h"
#include "afxdialogex.h"
#include "DataService.h"
#include "ErrorDefine.h"
#include "DataListControl.h"


// CReport �Ի���

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


// CReport ��Ϣ�������


BOOL CReport::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);
    //m_ListCtrl.InsertColumn(0, _T("Ա����"), LVCFMT_LEFT, clientRect.Width()/4);
    m_ListCtrl.InsertColumn(0, _T("����"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(1, _T("����"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(2, _T("����"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(3, _T("�ƻ���¼"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(4, _T("ʵ�ʼ�¼"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(5, _T("�쳣����"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(6, _T("�쳣����"), LVCFMT_LEFT, clientRect.Width()/7);

    m_ComboReportMonth.AddString(_T("1��"));
    m_ComboReportMonth.AddString(_T("2��"));
    m_ComboReportMonth.AddString(_T("3��"));
    m_ComboReportMonth.AddString(_T("4��"));
    m_ComboReportMonth.AddString(_T("5��"));
    m_ComboReportMonth.AddString(_T("6��"));
    m_ComboReportMonth.AddString(_T("7��"));
    m_ComboReportMonth.AddString(_T("8��"));
    m_ComboReportMonth.AddString(_T("9��"));
    m_ComboReportMonth.AddString(_T("10��"));
    m_ComboReportMonth.AddString(_T("11��"));
    m_ComboReportMonth.AddString(_T("12��"));
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CReport::OnBnClickedBtnSearchreport()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int month = m_ComboReportMonth.GetCurSel()+1;
    if (month <=0)
    {
        AfxMessageBox(_T("��ѡ���·�"));
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
            strDate.Format(_T("%02d��%02d��"), rTime.GetMonth(), rTime.GetDay());
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
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    POSITION pos;
    pos = m_ListCtrl.GetFirstSelectedItemPosition();
    // �õ���Ŀ����
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
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    POSITION pos;
    pos = m_ListCtrl.GetFirstSelectedItemPosition();
    // �õ���Ŀ����
    int select = m_ListCtrl.GetNextSelectedItem(pos);  
    if (select<0)
    {
        return;
    }
    ReportInfo* report = (ReportInfo*)m_ListCtrl.GetItemData(select);
}


void CReport::OnBnClickedCancel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_RWDSClientView->m_OpenReportForm = FALSE;
    CWnd::DestroyWindow();
    //CDialogEx::OnCancel();
}
