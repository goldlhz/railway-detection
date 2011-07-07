// StaffLogs.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "StaffLogs.h"
#include "afxdialogex.h"
#include "DataService.h"


// CStaffLogs �Ի���

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


// CStaffLogs ��Ϣ�������

void CStaffLogs::SetReportInfo(ReportInfo* aReport)
{
    m_Report = aReport;
}


BOOL CStaffLogs::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    if (!m_Report)
    {
        return FALSE;
    }
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);
    //m_ListCtrl.InsertColumn(0, _T("�ص�"), LVCFMT_LEFT, clientRect.Width()/4);
    m_ListCtrl.InsertColumn(0, _T("Ӧ��ʱ��"), LVCFMT_LEFT, clientRect.Width()/3);
    m_ListCtrl.InsertColumn(1, _T("ʵ��ʱ��"), LVCFMT_LEFT, clientRect.Width()/3);
    m_ListCtrl.InsertColumn(2, _T("״̬"), LVCFMT_LEFT, clientRect.Width()/3);
    CString textShow;
    textShow.Format(_T("Ա��:%s, �ƻ�����: %d, ʵ�ʵ���: %d, δ����: %d, �쳣: %d"),
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
    // �쳣: OCX ����ҳӦ���� FALSE
}
