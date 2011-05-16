// EmergencyTask.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "EmergencyTask.h"
#include "afxdialogex.h"


// CEmergencyTask �Ի���

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
    DDX_Control(pDX, IDC_EMERGENCY_ALLPOINT, m_ListAllPoint);
    DDX_Control(pDX, IDC_EMERGENCY_SELETED, m_ListSelectedPoint);
}


BEGIN_MESSAGE_MAP(CEmergencyTask, CDialogEx)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_EMERGENCYLIST, &CEmergencyTask::OnLvnItemchangedEmergencylist)
    ON_BN_CLICKED(IDC_BTN_EMERGENCYADD1, &CEmergencyTask::OnBnClickedBtnEmergencyadd1)
    ON_BN_CLICKED(IDC_BTN_EMERGENCYREMOVE1, &CEmergencyTask::OnBnClickedBtnEmergencyremove1)
    ON_BN_CLICKED(IDC_BTN_EMERGENCYADD, &CEmergencyTask::OnBnClickedBtnEmergencyadd)
    ON_BN_CLICKED(IDC_BTN_EMERGENCYMODIFY, &CEmergencyTask::OnBnClickedBtnEmergencymodify)
    ON_BN_CLICKED(IDC_BTN_EMERGENCYDELETE, &CEmergencyTask::OnBnClickedBtnEmergencydelete)
    ON_BN_CLICKED(IDC_BTN_EMERGENCYOK, &CEmergencyTask::OnBnClickedBtnEmergencyok)
END_MESSAGE_MAP()


// CEmergencyTask ��Ϣ�������


BOOL CEmergencyTask::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
        if (m_CRWDSClientView->m_Emergency[i]->iLineKmLonLat.size() > 0)
        {
            startKm.Format(_T("%.0f"), m_CRWDSClientView->m_Emergency[i]->iLineKmLonLat[0]->iKM);
            endKm.Format(_T("%.0f"), m_CRWDSClientView->m_Emergency[i]->iLineKmLonLat[m_CRWDSClientView->m_Line[i]->iLineKmLonLat.size()-1]->iKM);
        }
        else
        {
            startKm = _T("");
            endKm = _T("");
        }

        m_ListCtrl.InsertItem(i, name);
        m_ListCtrl.SetItemText(i, 1, id);
        m_ListCtrl.SetItemText(i, 2, startKm);
        m_ListCtrl.SetItemText(i, 3, endKm);
        if (m_CRWDSClientView->m_Emergency[i]->iStatus == KNormal)
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
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    POSITION pos;
    pos = m_ListCtrl.GetFirstSelectedItemPosition();
    int select = m_ListCtrl.GetNextSelectedItem(pos);  
    if (select<0)
    {
        return;
    }
    CString str;
    //GetDlgItem(IDC_EDIT_LINEREMARK)->SetWindowText(_T(""));
    m_ListAllPoint.ResetContent();
    m_ListSelectedPoint.ResetContent();
    m_Selected.clear();
    m_Unselected.clear();
    EmergencyTaskInfo* task = m_CRWDSClientView->m_Emergency[select];
    //GetDlgItem(IDC_EDIT_LINEREMARK)->SetWindowText(task->iLineRemark);
    GetDlgItem(IDC_EDIT_EMERGENCYNAME)->SetWindowText(task->iTaskName);
    str.Format(_T("%d"), task->iTaskID);
    GetDlgItem(IDC_EDIT_EMERGENCYID)->SetWindowText(str);
    m_ComboEmergencyStatus.SetCurSel(task->iStatus);
    for (size_t i=0; i<m_CRWDSClientView->m_MapPoint.size(); i++)
    {//������ѡ����δѡ����ؽ���
        bool addSelect = FALSE;
        for (size_t j=0; j<task->iLineKmLonLat.size(); j++)
        {
            if (m_CRWDSClientView->m_MapPoint[i] == task->iLineKmLonLat[j])
            {
                addSelect = TRUE;
                break;
            }
        }
        ENCODERAILWAYFULLNAME(str, m_CRWDSClientView->m_MapPoint[i]->iRailLine, 
            m_CRWDSClientView->m_MapPoint[i]->iKM, 
            m_CRWDSClientView->m_MapPoint[i]->iDirect);
        if (addSelect)
        {
            m_ListSelectedPoint.AddString(str);
            m_Selected.push_back(m_CRWDSClientView->m_MapPoint[i]);
        }
        else
        {
            m_ListAllPoint.AddString(str);
            m_Unselected.push_back(m_CRWDSClientView->m_MapPoint[i]);
        }
    }
    *pResult = 0;
}

void CEmergencyTask::OnBnClickedBtnEmergencyadd1()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CEmergencyTask::OnBnClickedBtnEmergencyremove1()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
}

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
    //����1��Ϊ�±��
    taskID++;
    return taskID;
}

void CEmergencyTask::OnBnClickedBtnEmergencyadd()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    EmergencyTaskInfo* task = new EmergencyTaskInfo;
    task->iTaskID = CreateEmergencyTaskID();
    task->iTaskName = _T("��������");
    task->iStatus = KNormal;
    m_CRWDSClientView->m_Emergency.push_back(task);

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
}

void CEmergencyTask::OnBnClickedBtnEmergencymodify()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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

    //LineInfo* line = m_CRWDSClientView->m_Line[select];
    //GetDlgItem(IDC_EDIT_LINENAME)->GetWindowText(line->iLineName);
    //GetDlgItem(IDC_EDIT_LINEREMARK)->GetWindowText(line->iLineRemark);
    AfxMessageBox(_T("����ɹ�"), MB_OK);
    m_ListCtrl.SetItemText(select, 0, task->iTaskName);
}

void CEmergencyTask::OnBnClickedBtnEmergencydelete()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int select = m_ListCtrl.GetSelectionMark();
    if (select < 0)
        return;

    EmergencyTaskInfo* task = m_CRWDSClientView->m_Emergency[select];

    m_CRWDSClientView->m_Emergency.erase(m_CRWDSClientView->m_Emergency.begin()+select);

    m_ListCtrl.DeleteItem(select);
    m_ListAllPoint.ResetContent();
    m_ListSelectedPoint.ResetContent();

    GetDlgItem(IDC_EDIT_EMERGENCYID)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_EMERGENCYNAME)->SetWindowText(_T(""));
    m_ComboEmergencyStatus.SetCurSel(-1);

    delete task;
}

void CEmergencyTask::OnBnClickedBtnEmergencyok()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
}
