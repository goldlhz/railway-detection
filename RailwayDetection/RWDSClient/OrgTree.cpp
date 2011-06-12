// OrgTree.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "OrgTree.h"
#include "afxdialogex.h"
#include "DataService.h"


// COrgTree �Ի���

IMPLEMENT_DYNAMIC(COrgTree, CDialogEx)

COrgTree::COrgTree(CWnd* pParent /*=NULL*/)
	: CDialogEx(COrgTree::IDD, pParent)
{
    m_RWDSClientView = static_cast<CRWDSClientView*>(pParent);
}

COrgTree::~COrgTree()
{
}

void COrgTree::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_ORG, m_TreeOrg);
    DDX_Control(pDX, IDC_LIST_ORGDATA, m_ListOrgData);
    DDX_Control(pDX, IDC_LIST_SELECTEDDATA, m_ListSelectedData);
}


BEGIN_MESSAGE_MAP(COrgTree, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_ADDDATA, &COrgTree::OnBnClickedBtnAdddata)
    ON_BN_CLICKED(IDC_BTN_DELETEDATA, &COrgTree::OnBnClickedBtnDeletedata)
    ON_BN_CLICKED(ID_BTN_OK, &COrgTree::OnBnClickedBtnOk)
    ON_BN_CLICKED(IDCANCEL, &COrgTree::OnBnClickedCancel)
    ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_ORG, &COrgTree::OnTvnItemexpandingTreeOrg)
    ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE_ORG, &COrgTree::OnTvnItemexpandedTreeOrg)
    ON_NOTIFY(NM_CLICK, IDC_TREE_ORG, &COrgTree::OnNMClickTreeOrg)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ORG, &COrgTree::OnTvnSelchangedTreeOrg)
ON_NOTIFY(NM_DBLCLK, IDC_LIST_ORGDATA, &COrgTree::OnNMDblclkListOrgdata)
END_MESSAGE_MAP()


// COrgTree ��Ϣ�������

BOOL COrgTree::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_ListOrgData.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    m_ListSelectedData.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

    CRect clientRect;
    m_ListOrgData.GetClientRect(&clientRect);
    while(m_ListOrgData.DeleteColumn(0));
    m_ListOrgData.InsertColumn(0, _T("������"), LVCFMT_LEFT, clientRect.Width()/2);
    m_ListOrgData.InsertColumn(1, _T("����"), LVCFMT_LEFT, clientRect.Width()/2);
    
    m_ListSelectedData.GetClientRect(&clientRect);
    while(m_ListSelectedData.DeleteColumn(0));
    m_ListSelectedData.InsertColumn(0, _T("������"), LVCFMT_LEFT, clientRect.Width()/2);
    m_ListSelectedData.InsertColumn(1, _T("����"), LVCFMT_LEFT, clientRect.Width()/2);
    if (m_EmergencyTask)
    {
        StaffInfo* staff;
        for (size_t i=0; i<m_EmergencyTask->iAppointStaff.size(); i++)
        {//��ʼ��������������Ա
            staff = m_EmergencyTask->iAppointStaff[i];
            CString str;
            str.Format(_T("%d"), staff->iOrgID);
            m_ListSelectedData.InsertItem(i, str);
            m_ListSelectedData.SetItemText(i, 1, staff->iName);
            m_ListSelectedData.SetItemData(i, (DWORD_PTR)staff);
        }
        
    }


    DWORD newStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
    DWORD oldStyle = m_TreeOrg.GetStyle();
    m_TreeOrg.ModifyStyle(oldStyle, newStyle);
    CString str;
    
    HTREEITEM hRoot = m_TreeOrg.InsertItem(m_RWDSClientView->m_Org[0]->iOrgName);
    m_TreeOrg.SetItemData(hRoot, (DWORD_PTR)m_RWDSClientView->m_Org[0]);
    m_TreeOrg.InsertItem(_T(""), hRoot);//Ϊ����ʾ+��

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}




//////////////////////////////////////////////////////////////////////////
//������

void COrgTree::OnTvnItemexpandingTreeOrg(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (pNMTreeView->action ==  TVE_EXPAND)
    {
        HTREEITEM curItem = reinterpret_cast<LPNMTREEVIEW>(pNMHDR)->itemNew.hItem;
        if (!m_TreeOrg.ItemHasChildren(curItem))
        {
            return;
        }
        HTREEITEM childItem = m_TreeOrg.GetChildItem(curItem);
        OrganizationInfo* curOrg = (OrganizationInfo*) m_TreeOrg.GetItemData(curItem);
        //m_RWDSClientView->m_CurrentOrg = curOrg;
        if (m_TreeOrg.GetItemText(childItem).Compare(_T("")) == 0)
        {//��һ��չ��
            if (curOrg->iStaff.size() == 0)
            {//��ȡ�û�������Ա
                //GetOrgStaff(curOrg->iOrgID, &(curOrg->iStaff));
            }
        }
    }

    *pResult = 0;
}


void COrgTree::OnTvnItemexpandedTreeOrg(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (pNMTreeView->action ==  TVE_EXPAND)
    {
        //HTREEITEM curItem = m_wndFileView.GetSelectedItem();
        HTREEITEM curItem = reinterpret_cast<LPNMTREEVIEW>(pNMHDR)->itemNew.hItem;
        HTREEITEM childItem = m_TreeOrg.GetChildItem(curItem);
        if (m_TreeOrg.GetItemText(childItem).Compare(_T("")) != 0)
        {//��item�Ѿ�չ���������û�ȡ����
            return;
        }
        else
        {
            m_TreeOrg.DeleteItem(childItem);//�����ʱ��Чitem
        }
        OrganizationInfo* curOrg = (OrganizationInfo*) m_TreeOrg.GetItemData(curItem);
        HTREEITEM tmpChild;
        if(curOrg->iChildOrg.size() > 0)
        {//��������
            OrganizationInfo* org;
            for(size_t i=0; i<curOrg->iChildOrg.size(); i++)
            {
                org = curOrg->iChildOrg[i];
                tmpChild = m_TreeOrg.InsertItem(org->iOrgName, 8, 8, curItem);
                m_TreeOrg.SetItemData(tmpChild, (DWORD_PTR)org);
                m_TreeOrg.InsertItem(_T(""), tmpChild);//Ϊ����ʾ+��
            }
        }
    }

    *pResult = 0;
}


void COrgTree::OnNMClickTreeOrg(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    *pResult = 0;
}

void COrgTree::OnTvnSelchangedTreeOrg(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    HTREEITEM curItem = m_TreeOrg.GetSelectedItem();
    if (curItem == NULL)
    {
        return;
    }
    OrganizationInfo* curOrg = (OrganizationInfo*) m_TreeOrg.GetItemData(curItem);
    if (curOrg->iStaff.size() == 0)
    {
        HTREEITEM childItem = m_TreeOrg.GetChildItem(curItem);
        if (!childItem || childItem && m_TreeOrg.GetItemText(childItem).Compare(_T("")) == 0)
        {//
            GetOrgStaff(curOrg->iOrgID, &(curOrg->iStaff));
        }
    }
    m_ListOrgData.DeleteAllItems();
    StaffInfo* staff;
    for(size_t i=0; i<curOrg->iStaff.size(); i++)
    {
        staff = curOrg->iStaff[i];
        CString str;
        str.Format(_T("%d"), curOrg->iOrgID);
        m_ListOrgData.InsertItem(i, str);
        m_ListOrgData.SetItemText(i, 1, staff->iName);
        m_ListOrgData.SetItemData(i, (DWORD_PTR)staff);
    }
    *pResult = 0;
}


void COrgTree::OnNMDblclkListOrgdata(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    InsertListSelectedData();
    *pResult = 0;
}


void COrgTree::OnBnClickedBtnAdddata()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    InsertListSelectedData();
}


void COrgTree::InsertListSelectedData()
{
    HTREEITEM curItem = m_TreeOrg.GetSelectedItem();
    if (curItem == NULL)
    {
        return;
    }
    OrganizationInfo* curOrg = (OrganizationInfo*) m_TreeOrg.GetItemData(curItem);
    POSITION pos;
    pos = m_ListOrgData.GetFirstSelectedItemPosition();
    int select = m_ListOrgData.GetNextSelectedItem(pos);  
    if (select<0)
    {
        return;
    }
    StaffInfo* staff = (StaffInfo*)m_ListOrgData.GetItemData(select);
    StaffInfo* addedStaff = NULL;
    BOOL flagInsert = TRUE;
    for (size_t i=0; i<m_ListSelectedData.GetItemCount(); i++)
    {
        addedStaff = (StaffInfo*)m_ListSelectedData.GetItemData(i);
        if (staff == addedStaff)
        {
            flagInsert = FALSE;
            break;
        }
    }

    if (flagInsert)
    {
        int index = m_ListSelectedData.GetItemCount();
        CString str;
        str.Format(_T("%d"), curOrg->iOrgID);
        m_ListSelectedData.InsertItem(index, str);
        m_ListSelectedData.SetItemText(index, 1, staff->iName);
        m_ListSelectedData.SetItemData(index, (DWORD_PTR)staff);
    }
}
void COrgTree::OnBnClickedBtnDeletedata()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    POSITION pos;
    pos = m_ListSelectedData.GetFirstSelectedItemPosition();
    int select = m_ListSelectedData.GetNextSelectedItem(pos);  
    if (select<0)
    {
        return;
    }
    m_ListSelectedData.DeleteItem(select);
}

void COrgTree::OnBnClickedBtnOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_EmergencyTask->iAppointStaff.clear();
    for (size_t i=0; i<m_ListSelectedData.GetItemCount(); i++)
    {
        m_EmergencyTask->iAppointStaff.push_back((StaffInfo*)m_ListSelectedData.GetItemData(i));
    }
    CDialogEx::OnOK();
}

void COrgTree::OnBnClickedCancel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CDialogEx::OnCancel();
}

void COrgTree::SetEmergencyTaskInfo( EmergencyTaskInfo* a_EmergencyTask )
{
    m_EmergencyTask = a_EmergencyTask;
}

