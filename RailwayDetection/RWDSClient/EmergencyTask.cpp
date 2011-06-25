// EmergencyTask.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "EmergencyTask.h"
#include "afxdialogex.h"
#include "Datadef.h"
#include "DataService.h"
#include "CmdDefine.h"
#include "OrgTree.h"


// CEmergencyTask �Ի���

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
    DDX_Control(pDX, IDC_COMBO_STARTKM, m_Combo_StartKM);
    DDX_Control(pDX, IDC_COMBO_ENDKM, m_Combo_EndKM);
    //DDX_Control(pDX, IDC_LIST_EMSELETED, m_ListEmSeletedStaff);
    //DDX_Control(pDX, IDC_LIST_EMUNSELETED, m_ListEmUnseletedStaff);
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
END_MESSAGE_MAP()


// CEmergencyTask ��Ϣ��������


BOOL CEmergencyTask::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ����Ӷ���ĳ�ʼ��
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);
    m_ListCtrl.InsertColumn(0, _T("·����"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(1, _T("·�߱��"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(2, _T("��ʼ(KM)"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(3, _T("����(KM)"), LVCFMT_LEFT, clientRect.Width()/5);
    m_ListCtrl.InsertColumn(4, _T("״̬"), LVCFMT_LEFT, clientRect.Width()/5);

    m_ComboEmergencyStatus.AddString(_T("����"));
    m_ComboEmergencyStatus.AddString(_T("����"));

    CString str;
    for(size_t i=0; i<m_CRWDSClientView->m_CurrentOrg->iMapPoint.size(); i++)
    {
        
        ENCODERAILWAYFULLNAME(str, m_CRWDSClientView->m_CurrentOrg->iMapPoint[i]->iRailLine, 
                              m_CRWDSClientView->m_CurrentOrg->iMapPoint[i]->iKM, 
                              m_CRWDSClientView->m_CurrentOrg->iMapPoint[i]->iDirect);
        m_Combo_StartKM.AddString(str);
        //m_Combo_StartKM.SetItemData(i, reinterpret_cast<DWORD_PTR>(m_CRWDSClientView->m_MapPoint[i]));
        m_Combo_EndKM.AddString(str);
        //m_Combo_EndKM.SetItemData(i, reinterpret_cast<DWORD_PTR>(m_CRWDSClientView->m_MapPoint[i]));
    }

    int count = m_CRWDSClientView->m_CurrentOrg->iEmergency.size();
    CString id;
    CString name;
    CString startKm;
    CString endKm;
    CString flag;
    for (int i=0; i<count; i++)
    {
        id.Format(_T("%d"), m_CRWDSClientView->m_CurrentOrg->iEmergency[i]->iTaskID);
        name = m_CRWDSClientView->m_CurrentOrg->iEmergency[i]->iTaskName;
        if (m_CRWDSClientView->m_CurrentOrg->iEmergency[i]->iBeginKm)
        {
            startKm.Format(_T("%.0f"), m_CRWDSClientView->m_CurrentOrg->iEmergency[i]->iBeginKm->iKM);
        }
        else
        {
            startKm = _T("");
        }
        if (m_CRWDSClientView->m_CurrentOrg->iEmergency[i]->iEndKm)
        {
            endKm.Format(_T("%.0f"), m_CRWDSClientView->m_CurrentOrg->iEmergency[i]->iEndKm->iKM);
        }
        else
        {
            endKm = _T("");
        }

        m_ListCtrl.InsertItem(i, name);
        m_ListCtrl.SetItemText(i, 1, id);
        m_ListCtrl.SetItemText(i, 2, startKm);
        m_ListCtrl.SetItemText(i, 3, endKm);
        if (m_CRWDSClientView->m_CurrentOrg->iEmergency[i]->iStatus == KNormal)
        {
            flag = _T("����");
        }
        else
        {
            flag = _T("����");
        }
        m_ListCtrl.SetItemText(i, 4, flag);
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CEmergencyTask::OnLvnItemchangedEmergencylist(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
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

    for (size_t i=0; i<m_CRWDSClientView->m_CurrentOrg->iMapPoint.size(); i++)
    {
        if (m_CRWDSClientView->m_CurrentOrg->iMapPoint[i] == task->iBeginKm)
        {
            m_Combo_StartKM.SetCurSel(i);
        }
        if (m_CRWDSClientView->m_CurrentOrg->iMapPoint[i] == task->iEndKm)
        {
            m_Combo_EndKM.SetCurSel(i);
        }
    }

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
    //����1��Ϊ�±��
    taskID++;
    return taskID;
}

void CEmergencyTask::OnBnClickedBtnEmergencyadd()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
    EmergencyTaskInfo* task = new EmergencyTaskInfo;
    task->iTaskID = CreateEmergencyTaskID();
    task->iTaskName = _T("��������");
    task->iStatus = KNormal;
    task->iBeginKm = NULL;
    task->iEndKm = NULL;
    task->iBeginTime = 10;
    task->iEndTime = 10;
    task->iEmergencyRemark = _T("");
    m_CRWDSClientView->m_CurrentOrg->iEmergency.push_back(task);

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
    flag = _T("����");

	m_ListCtrl.InsertItem(count, name);
	m_ListCtrl.SetItemText(count, 1, id);
	m_ListCtrl.SetItemText(count, 2, startKm);
	m_ListCtrl.SetItemText(count, 3, endKm);
    m_ListCtrl.SetItemText(count, 4, flag);

    m_SeletedTask = NULL;
}

void CEmergencyTask::OnBnClickedBtnEmergencymodify()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
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
    CString str;
    if (m_Combo_StartKM.GetCurSel() > -1)
    {
        task->iBeginKm = m_CRWDSClientView->m_CurrentOrg->iMapPoint[m_Combo_StartKM.GetCurSel()];
        
    }
    if (m_Combo_EndKM.GetCurSel() > -1)
    {
        task->iEndKm = m_CRWDSClientView->m_CurrentOrg->iMapPoint[m_Combo_EndKM.GetCurSel()];

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
    AfxMessageBox(_T("�޸ĳɹ�"), MB_OK);

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
        str = _T("����");
    else
        str =  _T("����");
    m_ListCtrl.SetItemText(select, 4, str);

}

void CEmergencyTask::OnBnClickedBtnEmergencydelete()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
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

    m_Combo_StartKM.SetCurSel(-1);
    m_Combo_EndKM.SetCurSel(-1);
    m_ComboEmergencyStatus.SetCurSel(-1);

    m_SeletedTask = NULL;
    delete task;
}

void CEmergencyTask::OnBnClickedBtnEmergencyok()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
    OnOK();
}

void CEmergencyTask::OnBnClickedBtnSetemstaff()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
    if (!m_SeletedTask)
    {
        return;
    }
    COrgTree staffTree(m_CRWDSClientView);
    staffTree.SetEmergencyTaskInfo(m_SeletedTask);
    staffTree.DoModal();
}