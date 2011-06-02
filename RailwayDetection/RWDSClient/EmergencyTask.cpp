// EmergencyTask.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "EmergencyTask.h"
#include "afxdialogex.h"
#include "Datadef.h"
#include "DataService.h"
#include "CmdDefine.h"


// CEmergencyTask 对话框

IMPLEMENT_DYNAMIC(CEmergencyTask, CDialogEx)

CEmergencyTask::CEmergencyTask(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEmergencyTask::IDD, pParent)
{
    m_CRWDSClientView = static_cast<CRWDSClientView*>(pParent);
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
    DDX_Control(pDX, IDC_COMBO_STARTKM, m_Combo_StartKM);
    DDX_Control(pDX, IDC_COMBO_ENDKM, m_Combo_EndKM);
    DDX_Control(pDX, IDC_LIST_EMSELETED, m_ListEmSeletedStaff);
    DDX_Control(pDX, IDC_LIST_EMUNSELETED, m_ListEmUnseletedStaff);
}


BEGIN_MESSAGE_MAP(CEmergencyTask, CDialogEx)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_EMERGENCYLIST, &CEmergencyTask::OnLvnItemchangedEmergencylist)
//    ON_BN_CLICKED(IDC_BTN_EMERGENCYADD1, &CEmergencyTask::OnBnClickedBtnEmergencyadd1)
//    ON_BN_CLICKED(IDC_BTN_EMERGENCYREMOVE1, &CEmergencyTask::OnBnClickedBtnEmergencyremove1)
    ON_BN_CLICKED(IDC_BTN_EMERGENCYADD, &CEmergencyTask::OnBnClickedBtnEmergencyadd)
    ON_BN_CLICKED(IDC_BTN_EMERGENCYMODIFY, &CEmergencyTask::OnBnClickedBtnEmergencymodify)
    ON_BN_CLICKED(IDC_BTN_EMERGENCYDELETE, &CEmergencyTask::OnBnClickedBtnEmergencydelete)
    ON_BN_CLICKED(IDC_BTN_EMERGENCYOK, &CEmergencyTask::OnBnClickedBtnEmergencyok)
    ON_BN_CLICKED(IDC_BTN_EMADDSTAFF, &CEmergencyTask::OnBnClickedBtnEmaddstaff)
    ON_BN_CLICKED(IDC_BTN_EMREMOVESTAFF, &CEmergencyTask::OnBnClickedBtnEmremovestaff)
END_MESSAGE_MAP()


// CEmergencyTask 消息处理程序


BOOL CEmergencyTask::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);
    m_ListCtrl.InsertColumn(0, _T("路线名"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(1, _T("路线编号"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(2, _T("开始(KM)"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(3, _T("结束(KM)"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(4, _T("状态"), LVCFMT_LEFT, clientRect.Width()/5);

    m_ComboEmergencyStatus.AddString(_T("正常"));
    m_ComboEmergencyStatus.AddString(_T("结束"));

    CString str;
    for(size_t i=0; i<m_CRWDSClientView->m_MapPoint.size(); i++)
    {
        
        ENCODERAILWAYFULLNAME(str, m_CRWDSClientView->m_MapPoint[i]->iRailLine, 
                              m_CRWDSClientView->m_MapPoint[i]->iKM, 
                              m_CRWDSClientView->m_MapPoint[i]->iDirect);
        m_Combo_StartKM.AddString(str);
        //m_Combo_StartKM.SetItemData(i, reinterpret_cast<DWORD_PTR>(m_CRWDSClientView->m_MapPoint[i]));
        m_Combo_EndKM.AddString(str);
        //m_Combo_EndKM.SetItemData(i, reinterpret_cast<DWORD_PTR>(m_CRWDSClientView->m_MapPoint[i]));
    }

    int count = m_CRWDSClientView->m_Emergency.size();
    CString id;
    CString name;
    CString startKm;
    CString endKm;
    CString flag;
    for (int i=0; i<count; i++)
    {
        id.Format(_T("%d"), m_CRWDSClientView->m_Emergency[i]->iTaskID);
        name = m_CRWDSClientView->m_Emergency[i]->iTaskName;
        if (m_CRWDSClientView->m_Emergency[i]->iBeginKm)
        {
            startKm.Format(_T("%.0f"), m_CRWDSClientView->m_Emergency[i]->iBeginKm->iKM);
        }
        else
        {
            startKm = _T("");
        }
        if (m_CRWDSClientView->m_Emergency[i]->iEndKm)
        {
            endKm.Format(_T("%.0f"), m_CRWDSClientView->m_Emergency[i]->iEndKm->iKM);
        }
        else
        {
            endKm = _T("");
        }

        m_ListCtrl.InsertItem(i, name);
        m_ListCtrl.SetItemText(i, 1, id);
        m_ListCtrl.SetItemText(i, 2, startKm);
        m_ListCtrl.SetItemText(i, 3, endKm);
        if (m_CRWDSClientView->m_Emergency[i]->iStatus == KNormal)
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
        return;
    }
    CString str;
    EmergencyTaskInfo* task = m_CRWDSClientView->m_Emergency[select];
    m_SeletedTask = task;
    GetDlgItem(IDC_EDIT_EMERGENCYREMARK)->SetWindowText(task->iEmergencyRemark);
    GetDlgItem(IDC_EDIT_EMERGENCYNAME)->SetWindowText(task->iTaskName);
    str.Format(_T("%d"), task->iTaskID);
    GetDlgItem(IDC_EDIT_EMERGENCYID)->SetWindowText(str);
    m_ComboEmergencyStatus.SetCurSel(task->iStatus);

    CString strBeginHour;
    CString strBeginMin;
    CString strEndHour;
    CString strEndMin;
    struct tm* beginTime = localtime(&task->iBeginTime);
    strBeginHour.Format(_T("%02d"), beginTime->tm_hour);
    strBeginMin.Format(_T("%02d"), beginTime->tm_min);

    struct tm* endTime = localtime(&task->iEndTime);
    strEndHour.Format(_T("%02d"), endTime->tm_hour);
    strEndMin.Format(_T("%02d"), endTime->tm_min);

    GetDlgItem(IDC_EDIT_STARTHOUR)->SetWindowText(strBeginHour);
    GetDlgItem(IDC_EDIT_STARTMINUTE)->SetWindowText(strBeginMin);
    GetDlgItem(IDC_EDIT_ENDHOUR)->SetWindowText(strEndHour);
    GetDlgItem(IDC_EDIT_ENDMINUTE)->SetWindowText(strEndMin);

    m_Combo_StartKM.SetCurSel(-1);
    m_Combo_EndKM.SetCurSel(-1);

    for (size_t i=0; i<m_CRWDSClientView->m_MapPoint.size(); i++)
    {
        if (m_CRWDSClientView->m_MapPoint[i] == task->iBeginKm)
        {
            m_Combo_StartKM.SetCurSel(i);
        }
        if (m_CRWDSClientView->m_MapPoint[i] == task->iEndKm)
        {
            m_Combo_EndKM.SetCurSel(i);
        }
    }

    //选择员工
    m_ListEmSeletedStaff.ResetContent();
    m_ListEmUnseletedStaff.ResetContent();
    m_StaffSeleted.clear();
    m_StaffUnseleted.clear();
    StaffInfo* staffSeleted;
    StaffInfo* staffUnseleted;

    for (size_t i=0; i<task->iAppointStaff.size(); i++)
    {
        staffSeleted = task->iAppointStaff[i];
        m_StaffSeleted.push_back(staffSeleted);
        m_ListEmSeletedStaff.AddString(staffSeleted->iName);
    }

    BOOL addStaff = TRUE;
    for (size_t i=0; i<m_CRWDSClientView->m_Staff.size(); i++)
    {
        addStaff = TRUE;
        staffUnseleted = m_CRWDSClientView->m_Staff[i];
        for (size_t j=0; j<task->iAppointStaff.size(); j++)
        {//添加已选员工
            staffSeleted = task->iAppointStaff[j];
            if (staffSeleted == staffUnseleted)
            {//该员工已选择该线路
                addStaff = FALSE;
                break;
            }
        }
        if (addStaff)
        {
            m_StaffUnseleted.push_back(staffUnseleted);
            m_ListEmUnseletedStaff.AddString(staffUnseleted->iName);
        }
    }

    *pResult = 0;
}

//void CEmergencyTask::OnBnClickedBtnEmergencyadd1()
//{
//    // TODO: 在此添加控件通知处理程序代码
//}
//
//
//void CEmergencyTask::OnBnClickedBtnEmergencyremove1()
//{
//    // TODO: 在此添加控件通知处理程序代码
//}

int CEmergencyTask::CreateEmergencyTaskID()
{
    int taskID = 0;
    for (size_t i=0; i<m_CRWDSClientView->m_Emergency.size(); i++)
    {
        if(m_CRWDSClientView->m_Emergency[i]->iTaskID > taskID)
        {
            taskID = m_CRWDSClientView->m_Emergency[i]->iTaskID;
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
    task->iTaskID = CreateEmergencyTaskID();
    task->iTaskName = _T("紧急任务");
    task->iStatus = KNormal;
    task->iBeginKm = NULL;
    task->iEndKm = NULL;
    task->iBeginTime = 10;
    task->iEndTime = 10;
    task->iEmergencyRemark = _T("");
    m_CRWDSClientView->m_Emergency.push_back(task);

    SetEmergencyTask(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_EMERGENCY_ADD, task);

    CString id;
    CString name;
    CString startKm;
    CString endKm;
    CString flag;

	int count = m_ListCtrl.GetItemCount();

	id.Format(_T("%d"), task->iTaskID);
	startKm.Format(_T("0"));
	endKm.Format(_T("0"));
	name = task->iTaskName;
    flag = _T("正常");

	m_ListCtrl.InsertItem(count, name);
	m_ListCtrl.SetItemText(count, 1, id);
	m_ListCtrl.SetItemText(count, 2, startKm);
	m_ListCtrl.SetItemText(count, 3, endKm);
    m_ListCtrl.SetItemText(count, 4, flag);
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
    EmergencyTaskInfo* task = m_CRWDSClientView->m_Emergency[select];
    GetDlgItem(IDC_EDIT_EMERGENCYNAME)->GetWindowText(task->iTaskName);
    
    task->iStatus = static_cast<EmergencyStatus>(m_ComboEmergencyStatus.GetCurSel());
    CString str;
    if (m_Combo_StartKM.GetCurSel() > -1)
    {
        task->iBeginKm = m_CRWDSClientView->m_MapPoint[m_Combo_StartKM.GetCurSel()];
        
    }
    if (m_Combo_EndKM.GetCurSel() > -1)
    {
        task->iEndKm = m_CRWDSClientView->m_MapPoint[m_Combo_EndKM.GetCurSel()];

    }
    GetDlgItem(IDC_EDIT_EMERGENCYREMARK)->GetWindowText(task->iEmergencyRemark);

    CString strBeginHour;
    CString strBeginMin;
    CString strEndHour;
    CString strEndMin;

    GetDlgItem(IDC_EDIT_STARTHOUR)->GetWindowText(strBeginHour);
    GetDlgItem(IDC_EDIT_STARTMINUTE)->GetWindowText(strBeginMin);
    GetDlgItem(IDC_EDIT_ENDHOUR)->GetWindowText(strEndHour);
    GetDlgItem(IDC_EDIT_ENDMINUTE)->GetWindowText(strEndMin);

    struct tm* beginTime = localtime(&task->iBeginTime);
    beginTime->tm_hour = _ttoi(strBeginHour);
    beginTime->tm_min = _ttoi(strBeginMin);
    task->iBeginTime = mktime(beginTime);

    struct tm* endTime = localtime(&task->iEndTime);
    endTime->tm_hour = _ttoi(strEndHour);
    endTime->tm_min = _ttoi(strEndMin);
    task->iEndTime = mktime(endTime);

    SetEmergencyTask(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_EMERGENCY_MODIFY, task);
    AfxMessageBox(_T("修改成功"), MB_OK);

    m_ListCtrl.SetItemText(select, 0, task->iTaskName);

    str = _T("");
    if(task->iBeginKm)
        str.Format(_T("%s%.0f"), RailLineName[task->iBeginKm->iRailLine], task->iBeginKm->iKM);
    m_ListCtrl.SetItemText(select, 2, str);

    str = _T("");
    if(task->iEndKm)
        str.Format(_T("%s%.0f"), RailLineName[task->iEndKm->iRailLine], task->iEndKm->iKM);
    m_ListCtrl.SetItemText(select, 3, str);

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

    EmergencyTaskInfo* task = m_CRWDSClientView->m_Emergency[select];

    m_CRWDSClientView->m_Emergency.erase(m_CRWDSClientView->m_Emergency.begin()+select);

    SetEmergencyTask(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_EMERGENCY_DELETE, task);

    m_ListCtrl.DeleteItem(select);

    GetDlgItem(IDC_EDIT_EMERGENCYID)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_EMERGENCYNAME)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_EMERGENCYREMARK)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_STARTHOUR)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_STARTMINUTE)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_ENDHOUR)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_ENDMINUTE)->SetWindowText(_T(""));

    m_Combo_StartKM.SetCurSel(-1);
    m_Combo_EndKM.SetCurSel(-1);
    m_ComboEmergencyStatus.SetCurSel(-1);



    delete task;
}

void CEmergencyTask::OnBnClickedBtnEmergencyok()
{
    // TODO: 在此添加控件通知处理程序代码
    OnOK();
}


void CEmergencyTask::OnBnClickedBtnEmaddstaff()
{
    // TODO: 在此添加控件通知处理程序代码

    int curStaffIndex = m_ListEmUnseletedStaff.GetCurSel();
    if (curStaffIndex<0)
    {
        return;
    }
    StaffInfo* curStaff = m_StaffUnseleted[curStaffIndex];
    //把员工添加到当前选择的路线
    m_SeletedTask->iAppointStaff.push_back(curStaff);
    SetEmergencyTask(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_EMERGENCY_MODIFY, m_SeletedTask);
    //更改状态
    m_ListEmUnseletedStaff.DeleteString(curStaffIndex);
    m_ListEmSeletedStaff.AddString(curStaff->iName);
    m_StaffUnseleted.erase(m_StaffUnseleted.begin()+curStaffIndex);
    m_StaffSeleted.push_back(curStaff);
    
}

void CEmergencyTask::OnBnClickedBtnEmremovestaff()
{
    // TODO: 在此添加控件通知处理程序代码
    int curStaffIndex = m_ListEmSeletedStaff.GetCurSel();
    if (curStaffIndex<0)
    {
        return;
    }
    StaffInfo* curStaff = m_StaffSeleted[curStaffIndex];
    //把员工移除当前选择的路线
    m_SeletedTask->iAppointStaff.erase(m_SeletedTask->iAppointStaff.begin()+curStaffIndex);
    SetEmergencyTask(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_EMERGENCY_MODIFY, m_SeletedTask);

    //更改状态
    m_ListEmSeletedStaff.DeleteString(curStaffIndex);
    m_ListEmUnseletedStaff.AddString(curStaff->iName);
    m_StaffSeleted.erase(m_StaffSeleted.begin()+curStaffIndex);
    m_StaffUnseleted.push_back(curStaff);
}
