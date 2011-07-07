// EmergencyLogs.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "EmergencyLogs.h"
#include "afxdialogex.h"
#include "DataService.h"
#include "ErrorDefine.h"


// CEmergencyLogs 对话框

IMPLEMENT_DYNAMIC(CEmergencyLogs, CDialogEx)

CEmergencyLogs::CEmergencyLogs(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEmergencyLogs::IDD, pParent)
{
    m_EmergencyTask = NULL;
}

CEmergencyLogs::~CEmergencyLogs()
{
}

void CEmergencyLogs::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_EMERGENCYLOGS, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CEmergencyLogs, CDialogEx)
END_MESSAGE_MAP()


// CEmergencyLogs 消息处理程序


BOOL CEmergencyLogs::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);
    m_ListCtrl.InsertColumn(0, _T("紧急任务号"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(1, _T("员工号"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(2, _T("员工所属机构"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(3, _T("总计时间"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(4, _T("总计路程(KM)"), LVCFMT_LEFT, clientRect.Width()/5);

    m_LogsList.clear();
    if (m_EmergencyTask)
    {
        int err = GetEmergencyLogs(m_EmergencyTask->iTaskID, &m_LogsList);
        if (err == KErrNone)
        {
            EmergencyLogs* logs = NULL;
            for(size_t i=0; i<m_LogsList.size(); i++)
            {
                logs = m_LogsList[i];
                CString taskID;
                CString orgID;
                CString totalTime;
                CString totalKM;
                taskID.Format(_T("%d"), logs->iTaskID);
                orgID.Format(_T("%d"), logs->iStaffOrgID);
                totalTime.Format(_T("%d"), logs->iTotalTime);
                totalKM.Format(_T("%f"), logs->iTotalKM);

                m_ListCtrl.InsertItem(i, taskID);
                m_ListCtrl.SetItemText(i, 1, logs->iStaffID);
                m_ListCtrl.SetItemText(i, 2, orgID);
                m_ListCtrl.SetItemText(i, 3, totalTime);
                m_ListCtrl.SetItemText(i, 4, totalKM);
            }
        }
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CEmergencyLogs::SetEmgergencyTask( EmergencyTaskInfo* aEmergencyTask )
{
    m_EmergencyTask = aEmergencyTask;
}
