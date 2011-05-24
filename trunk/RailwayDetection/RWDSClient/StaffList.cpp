// StaffList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "StaffList.h"
#include "afxdialogex.h"
#include "Datadef.h"
#include "ModifyPassword.h"


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
}


BEGIN_MESSAGE_MAP(CStaffList, CDialogEx)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_STAFFLIST, &CStaffList::OnLvnItemchangedStafflist)
    ON_BN_CLICKED(IDC_CHECK_LOGINPERMISSION, &CStaffList::OnBnClickedCheckLoginpermission)
    ON_BN_CLICKED(IDC_BTN_SETPASSWORD, &CStaffList::OnBnClickedBtnSetpassword)
    ON_BN_CLICKED(IDC_BTN_ADDSTAFF, &CStaffList::OnBnClickedBtnAddstaff)
    ON_BN_CLICKED(IDC_BTNMODIFYSTAFF, &CStaffList::OnBnClickedBtnmodifystaff)
    ON_BN_CLICKED(IDC_BTN_DELSTAFF, &CStaffList::OnBnClickedBtnDelstaff)
END_MESSAGE_MAP()


// CStaffList ��Ϣ��������


BOOL CStaffList::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ����Ӷ���ĳ�ʼ��
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
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CStaffList::OnLvnItemchangedStafflist(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
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
    *pResult = 0;
}

void CStaffList::OnBnClickedCheckLoginpermission()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
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
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
    if(!m_SeletedStaff)
        return;
    CModifyPassword modifyPassword;
    modifyPassword.SetPassword(m_SeletedStaff->iPassword);
    if(modifyPassword.DoModal() == IDOK)
    {
        m_SeletedStaff->iPassword = modifyPassword.GetPassword();
        m_SeletedStaff->iLoginPermission = TRUE;
        AfxMessageBox(_T("�����޸ĳɹ�"));
    }
}

int CStaffList::CreateStaffID()
{
    int staffID = 0;
    for (size_t i=0; i<m_CRWDSClientView->m_Staff.size(); i++)
    {
        if(m_CRWDSClientView->m_Staff[i]->iID > staffID)
        {
            staffID = m_CRWDSClientView->m_Staff[i]->iID;
        }
    }
    //����1��Ϊ�±��
    staffID++;
    return staffID;
}

void CStaffList::OnBnClickedBtnAddstaff()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
    StaffInfo* staff = new StaffInfo;
    staff->iID = CreateStaffID();
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
}


void CStaffList::OnBnClickedBtnmodifystaff()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
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
    AfxMessageBox(_T("�޸ĳɹ�"), MB_OK);
}


void CStaffList::OnBnClickedBtnDelstaff()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
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

    LineInfo* line;
    for (size_t i=0; i<m_CRWDSClientView->m_Line.size(); i++)
    {//ɾ����·���ŵ�Ա��
        line = m_CRWDSClientView->m_Line[i];
        for (size_t j=0; j<line->iArrangeStaff.size(); j++)
        {
            if (m_SeletedStaff == line->iArrangeStaff[j])
            {
                line->iArrangeStaff.erase(line->iArrangeStaff.begin()+j);
                break;
            }
        }
    }
    for (size_t i=0; i<m_CRWDSClientView->m_Staff.size(); i++)
    {//ɾ����������
        if (m_SeletedStaff == m_CRWDSClientView->m_Staff[i])
        {
            m_CRWDSClientView->m_Staff.erase(m_CRWDSClientView->m_Staff.begin()+i);
            break;
        }
    }
    m_SeletedStaff->iArrangeLine.clear();
    delete m_SeletedStaff;
    m_SeletedStaff = NULL;
}