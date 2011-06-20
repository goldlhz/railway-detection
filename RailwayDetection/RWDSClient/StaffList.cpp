// StaffList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "StaffList.h"
#include "afxdialogex.h"
#include "Datadef.h"
#include "ModifyPassword.h"
#include "DataService.h"
#include "CmdDefine.h"


// CStaffList �Ի���

IMPLEMENT_DYNAMIC(CStaffList, CDialogEx)

CStaffList::CStaffList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStaffList::IDD, pParent)
{
    m_CRWDSClientView = static_cast<CRWDSClientView*>(pParent);
}

CStaffList::~CStaffList()
{
}

void CStaffList::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STAFFLIST, m_ListCtrl);
    DDX_Control(pDX, IDC_LIST_ARRANGELINE, m_ArrangeLine);
    DDX_Control(pDX, IDC_CHECK_LOGINPERMISSION, m_CheckLoginPermission);
    DDX_Control(pDX, IDC_BTN_SETPASSWORD, m_BtnSetPassword);
    DDX_Control(pDX, IDC_CHECK_PERMISSIONBASICAL, m_PermissionBasical);
    DDX_Control(pDX, IDC_CHECK_PERMISSIONOPERATE, m_PermissionOperate);
    DDX_Control(pDX, IDC_CHECK_PERMISSIONREPORTFORM, m_PermissionReportform);
    DDX_Control(pDX, IDC_COMBO_DEVICE, m_ComboStaffDevice);
    DDX_Control(pDX, IDC_COMBO_PERMISSION, m_ComboStaffPermission);
}


BEGIN_MESSAGE_MAP(CStaffList, CDialogEx)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_STAFFLIST, &CStaffList::OnLvnItemchangedStafflist)
    ON_BN_CLICKED(IDC_CHECK_LOGINPERMISSION, &CStaffList::OnBnClickedCheckLoginpermission)
    ON_BN_CLICKED(IDC_BTN_SETPASSWORD, &CStaffList::OnBnClickedBtnSetpassword)
    ON_BN_CLICKED(IDC_BTN_ADDSTAFF, &CStaffList::OnBnClickedBtnAddstaff)
    ON_BN_CLICKED(IDC_BTNMODIFYSTAFF, &CStaffList::OnBnClickedBtnmodifystaff)
    ON_BN_CLICKED(IDC_BTN_DELSTAFF, &CStaffList::OnBnClickedBtnDelstaff)
END_MESSAGE_MAP()


// CStaffList ��Ϣ�������


BOOL CStaffList::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);
    while(m_ListCtrl.DeleteColumn(0));

    m_ListCtrl.InsertColumn(0, _T("�˺�"), LVCFMT_LEFT, clientRect.Width()/3);
    m_ListCtrl.InsertColumn(1, _T("����"), LVCFMT_LEFT, clientRect.Width()/3);
    m_ListCtrl.InsertColumn(2, _T("��·��"), LVCFMT_LEFT, clientRect.Width()/3);

    StaffInfo* staff;
    CString str;
    for (size_t i=0; i<m_CRWDSClientView->m_Staff.size(); i++)
    {
        staff = m_CRWDSClientView->m_Staff[i];
        str.Format(_T("%d"), staff->iID);
        m_ListCtrl.InsertItem(i, str);
        m_ListCtrl.SetItemText(i, 1, staff->iName);
        str.Format(_T("%d"), staff->iArrangeLine.size());
        m_ListCtrl.SetItemText(i, 2, str);
    }
    m_ComboStaffPermission.AddString(_T("����Ȩ��"));
    m_ComboStaffPermission.AddString(_T("����Ȩ��"));
    m_ComboStaffPermission.AddString(_T("����Ȩ��"));

    DeviceInfo* device;
    for(size_t i=0; i<m_CRWDSClientView->m_Device.size(); i++)
    {
        device = m_CRWDSClientView->m_Device[i];
        m_ComboStaffDevice.AddString(device->iPhoneNum);
        m_ComboStaffDevice.SetItemData(i, (DWORD_PTR)device);
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CStaffList::OnLvnItemchangedStafflist(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    POSITION pos;
    pos = m_ListCtrl.GetFirstSelectedItemPosition();
    // �õ���Ŀ����
    int select = m_ListCtrl.GetNextSelectedItem(pos);  
    if (select<0)
    {
        return;
    }
    m_SeletedStaff = m_CRWDSClientView->m_Staff[select];
    CString str;
    str.Format(_T("%d"), m_SeletedStaff->iID);
    GetDlgItem(IDC_EDIT_STAFFID)->SetWindowText(str);
    GetDlgItem(IDC_EDIT_STAFFNAME)->SetWindowText(m_SeletedStaff->iName);
    if (m_SeletedStaff->iLoginPermission)
    {
        m_CheckLoginPermission.SetCheck(TRUE);
        m_BtnSetPassword.ShowWindow(SW_SHOW);
    }
    else
    {
        m_CheckLoginPermission.SetCheck(FALSE);
        m_BtnSetPassword.ShowWindow(SW_HIDE);
    }
    m_ArrangeLine.ResetContent();
    for(size_t i=0; i<m_SeletedStaff->iArrangeLine.size(); i++)
    {
        m_ArrangeLine.AddString(m_SeletedStaff->iArrangeLine[i]->iLineName);
    }
    int comboIndex = -1;
    for(size_t i=0; i<m_ComboStaffDevice.GetCount(); i++)
    {
        if((DeviceInfo*)m_ComboStaffDevice.GetItemData(i) == m_SeletedStaff->iTakeDevice)
        {
            comboIndex = i;
            break;
        }
    }
    m_ComboStaffDevice.SetCurSel(comboIndex);
    //����Ȩ��
    //m_PermissionBasical.SetCheck(m_SeletedStaff->iPermission.iBasical);
    //m_PermissionOperate.SetCheck(m_SeletedStaff->iPermission.iOperate);
    //m_PermissionReportform.SetCheck(m_SeletedStaff->iPermission.iReportForm);
    *pResult = 0;
}

void CStaffList::OnBnClickedCheckLoginpermission()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (m_CheckLoginPermission.GetCheck() == BST_CHECKED)
    {
        m_BtnSetPassword.ShowWindow(SW_SHOW);
    }
    else
    {
        m_BtnSetPassword.ShowWindow(SW_HIDE);
    }
}

void CStaffList::OnBnClickedBtnSetpassword()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if(!m_SeletedStaff)
        return;
    CModifyPassword modifyPassword;
    modifyPassword.SetPassword(m_SeletedStaff->iPassword);
    if(modifyPassword.DoModal() == IDOK)
    {
        m_SeletedStaff->iPassword = modifyPassword.GetPassword();
        m_SeletedStaff->iLoginPermission = TRUE;

        SetOrgStaff(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_STAFF_MODIFY, m_SeletedStaff);
        AfxMessageBox(_T("�����޸ĳɹ�"));
    }
}

int CStaffList::CreateStaffID()
{
    int staffID = 0;
    int staffListID;
    for (size_t i=0; i<m_CRWDSClientView->m_Staff.size(); i++)
    {
        staffListID = _ttoi(m_CRWDSClientView->m_Staff[i]->iID);
        if(staffListID > staffID)
        {
            staffID = staffListID;
        }
    }
    //����1��Ϊ�±��
    staffID++;
    return staffID;
}

void CStaffList::OnBnClickedBtnAddstaff()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    StaffInfo* staff = new StaffInfo;
    staff->iID.Format(_T("%d"), CreateStaffID());
    staff->iLoginPermission = FALSE;
    staff->iName = _T("Ա��");
    CString str;
    str.Format(_T("%d"), staff->iID);
    int mask = m_ListCtrl.GetItemCount();
    m_ListCtrl.InsertItem(mask, str);
    m_ListCtrl.SetItemText(mask, 1, staff->iName);
    str.Format(_T("%d"), staff->iArrangeLine.size());
    m_ListCtrl.SetItemText(mask, 2, str);
    //���浽view����
    m_CRWDSClientView->m_Staff.push_back(staff);

    SetOrgStaff(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_STAFF_ADD, m_SeletedStaff);
}


void CStaffList::OnBnClickedBtnmodifystaff()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if(!m_SeletedStaff)
        return;
    GetDlgItem(IDC_EDIT_STAFFNAME)->GetWindowText(m_SeletedStaff->iName);
    if(m_CheckLoginPermission.GetCheck() == BST_CHECKED)
    {
        m_SeletedStaff->iLoginPermission = TRUE;
    }
    else
    {
        m_SeletedStaff->iLoginPermission = FALSE;
        m_SeletedStaff->iPassword = _T("");
    }
    //m_SeletedStaff->iPermission.iBasical = m_PermissionBasical.GetCheck();
    //m_SeletedStaff->iPermission.iOperate = m_PermissionOperate.GetCheck();
    //m_SeletedStaff->iPermission.iReportForm = m_PermissionReportform.GetCheck();
    SetOrgStaff(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_STAFF_MODIFY, m_SeletedStaff);
    AfxMessageBox(_T("�޸ĳɹ�"), MB_OK);
}


void CStaffList::OnBnClickedBtnDelstaff()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if(!m_SeletedStaff)
        return;
    int select = m_ListCtrl.GetSelectionMark();
    if (select < 0)
        return;
    if(AfxMessageBox(_T("ȷ��ɾ����"), MB_OKCANCEL) == IDCANCEL)
        return;
    m_ListCtrl.DeleteItem(select);
    GetDlgItem(IDC_EDIT_STAFFID)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_STAFFNAME)->SetWindowText(_T(""));
    m_CheckLoginPermission.SetCheck(BST_UNCHECKED);
    m_ArrangeLine.ResetContent();

    for (size_t i=0; i<m_CRWDSClientView->m_Calendar->iScheduleStaff.size(); i++)
    {//ɾ����·���ŵ�Ա��
        if (m_SeletedStaff == m_CRWDSClientView->m_Calendar->iScheduleStaff[i])
        {
            m_CRWDSClientView->m_Calendar->iScheduleStaff.erase(
                     m_CRWDSClientView->m_Calendar->iScheduleStaff.begin()+i);
            break;
        }
    }

    EmergencyTaskInfo* task = NULL;
    for (size_t i=0; i<m_CRWDSClientView->m_Emergency.size(); i++)
    {//ɾ���������������Ա��
        task = m_CRWDSClientView->m_Emergency[i];
        for (size_t j=0; j<task->iAppointStaff.size(); j++)
        {
            if (m_SeletedStaff == task->iAppointStaff[j])
            {
                task->iAppointStaff.erase(task->iAppointStaff.begin()+j);
                break;
            }
        }
    }
    SetEmergencyTask(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_EMERGENCY_MODIFY, task);

    for (size_t i=0; i<m_CRWDSClientView->m_Staff.size(); i++)
    {//ɾ����������
        if (m_SeletedStaff == m_CRWDSClientView->m_Staff[i])
        {
            m_CRWDSClientView->m_Staff.erase(m_CRWDSClientView->m_Staff.begin()+i);
            break;
        }
    }
    SetOrgStaff(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_STAFF_DELETE, m_SeletedStaff);
    m_SeletedStaff->iArrangeLine.clear();
    delete m_SeletedStaff;
    m_SeletedStaff = NULL;
}
