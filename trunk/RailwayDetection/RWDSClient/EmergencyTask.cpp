// EmergencyTask.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "EmergencyTask.h"
#include "afxdialogex.h"
#include "Datadef.h"
#include "DataService.h"
#include "CmdDefine.h"
#include "OrgTree.h"
#include "EmergencyLogs.h"


// CEmergencyTask 对话框

IMPLEMENT_DYNAMIC(CEmergencyTask, CDialogEx)

CEmergencyTask::CEmergencyTask(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEmergencyTask::IDD, pParent)
{
    m_CRWDSClientView = static_cast<CRWDSClientView*>(pParent);
    m_SeletedTask = NULL;
}

CEmergencyTask::~CEmergencyTask()
{
}

void CEmergencyTask::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EMERGENCYLIST, m_ListCtrl);
    DDX_Control(pDX, IDC_COMBO_EMERGENCYSTATUS, m_ComboEmergencyStatus);
    //    DDX_Control(pDX, IDC_EMERGENCY_ALLPOINT, m_ListAllPoint);
    //    DDX_Control(pDX, IDC_EMERGENCY_SELETED, m_ListSelectedPoint);
    //DDX_Control(pDX, IDC_COMBO_STARTKM, m_Combo_StartKM);
    //DDX_Control(pDX, IDC_COMBO_ENDKM, m_Combo_EndKM);
    //DDX_Control(pDX, IDC_LIST_EMSELETED, m_ListEmSeletedStaff);
    //DDX_Control(pDX, IDC_LIST_EMUNSELETED, m_ListEmUnseletedStaff);
    DDX_Control(pDX, IDC_COMBO_RAILLINE, m_ComboRailLine);
    DDX_Control(pDX, IDC_DATETIMEPICKER_STARTDATE, m_PickStartDay);
    DDX_Control(pDX, IDC_DATETIMEPICKER_ENDDATE, m_PickEndDay);
}


BEGIN_MESSAGE_MAP(CEmergencyTask, CDialogEx)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_EMERGENCYLIST, &CEmergencyTask::OnLvnItemchangedEmergencylist)
//    ON_BN_CLICKED(IDC_BTN_EMERGENCYADD1, &CEmergencyTask::OnBnClickedBtnEmergencyadd1)
//    ON_BN_CLICKED(IDC_BTN_EMERGENCYREMOVE1, &CEmergencyTask::OnBnClickedBtnEmergencyremove1)
    ON_BN_CLICKED(IDC_BTN_EMERGENCYADD, &CEmergencyTask::OnBnClickedBtnEmergencyadd)
    ON_BN_CLICKED(IDC_BTN_EMERGENCYMODIFY, &CEmergencyTask::OnBnClickedBtnEmergencymodify)
    ON_BN_CLICKED(IDC_BTN_EMERGENCYDELETE, &CEmergencyTask::OnBnClickedBtnEmergencydelete)
    ON_BN_CLICKED(IDC_BTN_EMERGENCYOK, &CEmergencyTask::OnBnClickedBtnEmergencyok)
//    ON_BN_CLICKED(IDC_BTN_EMADDSTAFF, &CEmergencyTask::OnBnClickedBtnEmaddstaff)
//    ON_BN_CLICKED(IDC_BTN_EMREMOVESTAFF, &CEmergencyTask::OnBnClickedBtnEmremovestaff)
ON_BN_CLICKED(IDC_BTN_SETEMSTAFF, &CEmergencyTask::OnBnClickedBtnSetemstaff)
ON_NOTIFY(NM_DBLCLK, IDC_EMERGENCYLIST, &CEmergencyTask::OnNMDblclkEmergencylist)
END_MESSAGE_MAP()


// CEmergencyTask 消息处理程序


BOOL CEmergencyTask::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);
    m_ListCtrl.InsertColumn(0, _T("任务名"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(1, _T("任务编号"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(2, _T("开始(KM)"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(3, _T("结束(KM)"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(4, _T("状态"), LVCFMT_LEFT, clientRect.Width()/5);

    m_ComboEmergencyStatus.AddString(_T("正常"));
    m_ComboEmergencyStatus.AddString(_T("结束"));

    for(size_t i=0; i<strRailLineName.size(); i++)
        m_ComboRailLine.AddString(strRailLineName[i]);

    int count = m_CRWDSClientView->m_CurrentOrg->iEmergency.size();
    CString id;
    CString name;
    CString startKm;
    CString endKm;
    CString flag;
    EmergencyTaskInfo* task = NULL;
    for (int i=0; i<count; i++)
    {
        task = m_CRWDSClientView->m_CurrentOrg->iEmergency[i];
        id.Format(_T("%d"), task->iTaskID);
        name = task->iTaskName;
        if (task->iBeginKm)
        {
            startKm.Format(_T("%d"), task->iBeginKm);
        }
        else
        {
            startKm = _T("0");
        }
        if (task->iEndKm)
        {
            endKm.Format(_T("%d"), task->iEndKm);
        }
        else
        {
            endKm = _T("0");
        }

        m_ListCtrl.InsertItem(i, name);
        m_ListCtrl.SetItemText(i, 1, id);
        m_ListCtrl.SetItemText(i, 2, startKm);
        m_ListCtrl.SetItemText(i, 3, endKm);
        m_ListCtrl.SetItemData(i, (DWORD_PTR) task);
        if (task->iStatus == KNormal)
        {
            flag = _T("正常");
        }
        else
        {
            flag = _T("结束");
        }
        m_ListCtrl.SetItemText(i, 4, flag);
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CEmergencyTask::OnLvnItemchangedEmergencylist(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    POSITION pos;
    pos = m_ListCtrl.GetFirstSelectedItemPosition();
    int select = m_ListCtrl.GetNextSelectedItem(pos);  
    if (select<0)
    {
        m_SeletedTask = NULL;
        return;
    }
    CString str;
    EmergencyTaskInfo* task = m_CRWDSClientView->m_CurrentOrg->iEmergency[select];
    m_SeletedTask = task;
    GetDlgItem(IDC_EDIT_EMERGENCYREMARK)->SetWindowText(task->iEmergencyRemark);
    GetDlgItem(IDC_EDIT_EMERGENCYNAME)->SetWindowText(task->iTaskName);
    str.Format(_T("%d"), task->iTaskID);
    GetDlgItem(IDC_EDIT_EMERGENCYID)->SetWindowText(str);
    m_ComboEmergencyStatus.SetCurSel(task->iStatus);

    m_ComboRailLine.SetCurSel(task->iLineName);

    CString startKM;
    CString endKM;
    startKM.Format(_T("%d"), task->iBeginKm);
    endKM.Format(_T("%d"), task->iEndKm);
    GetDlgItem(IDC_EDIT_EMERGENCYSTARTKM)->SetWindowText(startKM);
    GetDlgItem(IDC_EDIT_EMERGENCYENDKM)->SetWindowText(endKM);

    m_PickStartDay.SetTime(task->iBeginTime);
    m_PickEndDay.SetTime(task->iEndTime);

    //CString strBeginHour;
    //CString strBeginMin;
    //CString strEndHour;
    //CString strEndMin;
    //struct tm beginTime;
    //localtime_s(&beginTime, &task->iBeginTime);
    //strBeginHour.Format(_T("%02d"), beginTime.tm_hour);
    //strBeginMin.Format(_T("%02d"), beginTime.tm_min);

    //struct tm endTime;
    //localtime_s(&endTime, &task->iEndTime);
    //strEndHour.Format(_T("%02d"), endTime.tm_hour);
    //strEndMin.Format(_T("%02d"), endTime.tm_min);

    //GetDlgItem(IDC_EDIT_STARTHOUR)->SetWindowText(strBeginHour);
    //GetDlgItem(IDC_EDIT_STARTMINUTE)->SetWindowText(strBeginMin);
    //GetDlgItem(IDC_EDIT_ENDHOUR)->SetWindowText(strEndHour);
    //GetDlgItem(IDC_EDIT_ENDMINUTE)->SetWindowText(strEndMin);

    

    *pResult = 0;
}

int CEmergencyTask::CreateEmergencyTaskID()
{
    int taskID = 0;
    for (size_t i=0; i<m_CRWDSClientView->m_CurrentOrg->iEmergency.size(); i++)
    {
        if(m_CRWDSClientView->m_CurrentOrg->iEmergency[i]->iTaskID > taskID)
        {
            taskID = m_CRWDSClientView->m_CurrentOrg->iEmergency[i]->iTaskID;
        }
    }
    //增加1作为新编号
    taskID++;
    return taskID;
}

void CEmergencyTask::OnBnClickedBtnEmergencyadd()
{
    // TODO: 在此添加控件通知处理程序代码
    EmergencyTaskInfo* task = new EmergencyTaskInfo;
    //task->iTaskID = CreateEmergencyTaskID();
    task->iTaskName = _T("紧急任务");
    task->iStatus = KNormal;
    task->iBeginKm = m_CRWDSClientView->m_CurrentOrg->iBoundaryStartKM;
    task->iEndKm = m_CRWDSClientView->m_CurrentOrg->iBoundaryEndKM;;
    task->iLineName = m_CRWDSClientView->m_CurrentOrg->iBoundaryRail;
    time_t now; // 获取当前时间
    time(&now);
    task->iBeginTime = now;
    task->iEndTime = now;
    task->iEmergencyRemark = _T("");
    //由服务器返回id
    task->iTaskID = SetEmergencyTask(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_EMERGENCY_ADD, task);
    
    m_CRWDSClientView->m_CurrentOrg->iEmergency.push_back(task);
   
    CString id;
    CString name;
    CString startKm;
    CString endKm;
    CString flag;

	int count = m_ListCtrl.GetItemCount();

	id.Format(_T("%d"), task->iTaskID);
	startKm.Format(_T("%d"), task->iBeginKm);
	endKm.Format(_T("%d"), task->iEndKm);
	name = task->iTaskName;
    flag = _T("正常");

	m_ListCtrl.InsertItem(count, name);
	m_ListCtrl.SetItemText(count, 1, id);
	m_ListCtrl.SetItemText(count, 2, startKm);
	m_ListCtrl.SetItemText(count, 3, endKm);
    m_ListCtrl.SetItemText(count, 4, flag);
    m_ListCtrl.SetItemData(count, (DWORD_PTR)task);

    m_SeletedTask = NULL;
}

void CEmergencyTask::OnBnClickedBtnEmergencymodify()
{
    // TODO: 在此添加控件通知处理程序代码
    POSITION pos;
    pos = m_ListCtrl.GetFirstSelectedItemPosition();
    int select = m_ListCtrl.GetNextSelectedItem(pos);  
    if (select<0)
    {
        return;
    }
    EmergencyTaskInfo* task = m_CRWDSClientView->m_CurrentOrg->iEmergency[select];
    m_SeletedTask = task;
    GetDlgItem(IDC_EDIT_EMERGENCYNAME)->GetWindowText(task->iTaskName);
    
    task->iStatus = static_cast<EmergencyStatus>(m_ComboEmergencyStatus.GetCurSel());
    task->iLineName = m_ComboRailLine.GetCurSel();

    CString strBegin;
    CString strEnd;
    GetDlgItem(IDC_EDIT_EMERGENCYSTARTKM)->GetWindowText(strBegin);
    GetDlgItem(IDC_EDIT_EMERGENCYENDKM)->GetWindowText(strEnd);
    task->iBeginKm = _ttoi(strBegin);
    task->iEndKm = _ttoi(strEnd);

    GetDlgItem(IDC_EDIT_EMERGENCYREMARK)->GetWindowText(task->iEmergencyRemark);

    CTime startTime;
    CTime endTime;
    m_PickStartDay.GetTime(startTime);
    m_PickEndDay.GetTime(endTime);
    task->iBeginTime = startTime.GetTime();
    task->iEndTime = endTime.GetTime();
    //CString strBeginHour;
    //CString strBeginMin;
    //CString strEndHour;
    //CString strEndMin;

    //GetDlgItem(IDC_EDIT_STARTHOUR)->GetWindowText(strBeginHour);
    //GetDlgItem(IDC_EDIT_STARTMINUTE)->GetWindowText(strBeginMin);
    //GetDlgItem(IDC_EDIT_ENDHOUR)->GetWindowText(strEndHour);
    //GetDlgItem(IDC_EDIT_ENDMINUTE)->GetWindowText(strEndMin);

    //struct tm beginTime;
    //localtime_s(&beginTime, &task->iBeginTime);
    //beginTime.tm_hour = _ttoi(strBeginHour);
    //beginTime.tm_min = _ttoi(strBeginMin);
    //task->iBeginTime = mktime(&beginTime);

    //struct tm endTime;
    //localtime_s(&endTime, &task->iEndTime);
    //endTime.tm_hour = _ttoi(strEndHour);
    //endTime.tm_min = _ttoi(strEndMin);
    //task->iEndTime = mktime(&endTime);

    SetEmergencyTask(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_EMERGENCY_MODIFY, task);
    AfxMessageBox(_T("修改成功"), MB_OK);

    m_ListCtrl.SetItemText(select, 0, task->iTaskName);
    m_ListCtrl.SetItemText(select, 2, strBegin);
    m_ListCtrl.SetItemText(select, 3, strEnd);

    CString str;
    if (task->iStatus == KNormal)
        str = _T("正常");
    else
        str =  _T("结束");
    m_ListCtrl.SetItemText(select, 4, str);

}

void CEmergencyTask::OnBnClickedBtnEmergencydelete()
{
    // TODO: 在此添加控件通知处理程序代码
    int select = m_ListCtrl.GetSelectionMark();
    if (select < 0)
        return;

    EmergencyTaskInfo* task = m_CRWDSClientView->m_CurrentOrg->iEmergency[select];

    m_CRWDSClientView->m_CurrentOrg->iEmergency.erase(m_CRWDSClientView->m_CurrentOrg->iEmergency.begin()+select);

    SetEmergencyTask(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_EMERGENCY_DELETE, task);

    m_ListCtrl.DeleteItem(select);

    GetDlgItem(IDC_EDIT_EMERGENCYID)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_EMERGENCYNAME)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_EMERGENCYREMARK)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_STARTHOUR)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_STARTMINUTE)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_ENDHOUR)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_ENDMINUTE)->SetWindowText(_T(""));

    //m_Combo_StartKM.SetCurSel(-1);
    //m_Combo_EndKM.SetCurSel(-1);
    m_ComboEmergencyStatus.SetCurSel(-1);
    m_ComboRailLine.SetCurSel(-1);

    m_SeletedTask = NULL;
    delete task;
}

void CEmergencyTask::OnBnClickedBtnEmergencyok()
{
    // TODO: 在此添加控件通知处理程序代码
    OnOK();
}

void CEmergencyTask::OnBnClickedBtnSetemstaff()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_SeletedTask)
    {
        return;
    }
    COrgTree staffTree(m_CRWDSClientView);
    staffTree.SetEmergencyTaskInfo(m_SeletedTask);
    staffTree.DoModal();
    SetEmergencyTask(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_EMERGENCY_MODIFYSTAFF,
                     m_SeletedTask);
}


void CEmergencyTask::OnNMDblclkEmergencylist(NMHDR *pNMHDR, LRESULT *pResult)
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
    EmergencyTaskInfo* task = (EmergencyTaskInfo*)m_ListCtrl.GetItemData(select);
    CEmergencyLogs logs;
    logs.SetEmgergencyTask(task);
    logs.DoModal();
    *pResult = 0;
}
