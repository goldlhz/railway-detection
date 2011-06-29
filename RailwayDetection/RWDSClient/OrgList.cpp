// OrgList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "OrgList.h"
#include "afxdialogex.h"
#include "ErrorDefine.h"


// COrgList �Ի���

IMPLEMENT_DYNAMIC(COrgList, CDialogEx)

COrgList::COrgList(CWnd* pParent /*=NULL*/)
	: CDialogEx(COrgList::IDD, pParent)
{
    m_CRWDSClientView = static_cast<CRWDSClientView*>(pParent);
    m_SeletedOrg = NULL;
}

COrgList::~COrgList()
{
}

void COrgList::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_ORGLIST, m_TreeOrg);
    DDX_Control(pDX, IDC_COMBO_ORGPARENT, m_ComboOrgParent);
    DDX_Control(pDX, IDC_COMBO_BOUNDARY, m_ComboBoundaryLine);
}


BEGIN_MESSAGE_MAP(COrgList, CDialogEx)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ORGLIST, &COrgList::OnTvnSelchangedTreeOrglist)
    ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE_ORGLIST, &COrgList::OnTvnItemexpandedTreeOrglist)
    ON_BN_CLICKED(IDC_BTN_ADDORG, &COrgList::OnBnClickedBtnAddorg)
    ON_BN_CLICKED(IDC_BTN_MODIFYORG, &COrgList::OnBnClickedBtnModifyorg)
    ON_BN_CLICKED(IDC_BTN_DELETEORG, &COrgList::OnBnClickedBtnDeleteorg)
END_MESSAGE_MAP()


// COrgList ��Ϣ�������

void COrgList::LoadOrgTreeCtrl()
{
    //m_CRWDSClientView->m_Org
}

void COrgList::OrgListVisitForAddComboOrgParent(OrganizationInfo* aOrg)
{
    if (aOrg)
    {
        CString str;
        str.Format(_T("%d"), aOrg->iOrgID);
        int comboSize = m_ComboOrgParent.GetCount();
        m_ComboOrgParent.AddString(str);
        m_ComboOrgParent.SetItemData(comboSize, (DWORD_PTR)aOrg);
        OrganizationInfo* childOrg = NULL;
        for(size_t i=0; i<aOrg->iChildOrg.size(); i++)
        {
            childOrg = aOrg->iChildOrg[i];
            //str.Format(_T("%d"), childOrg->iOrgID);
            //m_ComboOrgParent.AddString(str);
            OrgListVisitForAddComboOrgParent(childOrg);
        }
    }
}

HTREEITEM COrgList::FindItemFromTreeOrg(HTREEITEM aTreeItem, DWORD_PTR aItemData)
{
    HTREEITEM treeItem;
    if (aTreeItem)
        treeItem = aTreeItem;
    else
        treeItem = m_TreeOrg.GetRootItem();
    if(aItemData == m_TreeOrg.GetItemData(treeItem))
        return treeItem;
    
    if(m_TreeOrg.ItemHasChildren(treeItem))
    {
        HTREEITEM item = m_TreeOrg.GetChildItem(treeItem);
        treeItem = NULL;
        while (item != NULL)
        {
            treeItem = FindItemFromTreeOrg(item, aItemData);
            if (treeItem)
            {
                break;
            }
            item = m_TreeOrg.GetNextItem(item, TVGN_NEXT);
        }
    }
    return treeItem;
}

int COrgList::AddOrgToTreeOrg(OrganizationInfo* aAddedOrg, OrganizationInfo* aParentOrg)
{
    HTREEITEM treeItem = FindItemFromTreeOrg(NULL, (DWORD_PTR)aParentOrg);
    if (treeItem)
    {
        HTREEITEM childItem = m_TreeOrg.InsertItem(aAddedOrg->iOrgName, treeItem);
        m_TreeOrg.SetItemData(childItem, (DWORD_PTR)aAddedOrg);

        //Ϊ�����¼ӵ�������ʾ+��
        m_TreeOrg.InsertItem(_T(""), childItem);
    }
    return KErrNone;
}

BOOL COrgList::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��

    DWORD newStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
    DWORD oldStyle = m_TreeOrg.GetStyle();
    m_TreeOrg.ModifyStyle(oldStyle, newStyle);
    CString str;

    HTREEITEM hRoot = m_TreeOrg.InsertItem(m_CRWDSClientView->m_Org[0]->iOrgName);
    m_TreeOrg.SetItemData(hRoot, (DWORD_PTR)m_CRWDSClientView->m_Org[0]);
    m_TreeOrg.InsertItem(_T(""), hRoot);//Ϊ����ʾ+��

    m_ComboOrgParent.ResetContent();
    m_ComboOrgParent.AddString(_T("��"));
    m_ComboOrgParent.SetItemData(0, NULL);
    OrgListVisitForAddComboOrgParent(m_CRWDSClientView->m_Org[0]);
    for(int i=0; i<RailLineNameCount; i++)
    {
        m_ComboBoundaryLine.AddString(RailLineName[i]);
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void COrgList::OnTvnSelchangedTreeOrglist(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    HTREEITEM curItem = m_TreeOrg.GetSelectedItem();
    if (curItem == NULL)
    {
        return;
    }
    OrganizationInfo* curOrg = (OrganizationInfo*) m_TreeOrg.GetItemData(curItem);
    m_SeletedOrg = curOrg;
    CString str;
    str.Format(_T("%d"), curOrg->iOrgID);
    GetDlgItem(IDC_EDIT_ORGID)->SetWindowText(str);
    GetDlgItem(IDC_EDIT_ORGNAME)->SetWindowText(curOrg->iOrgName);
    //int comboIndex = -1;
    //if (curOrg->iParentOrg)
    //{
    //    CString parentID;
    //    parentID.Format(_T("%d"), curOrg->iParentOrg->iOrgID);
    //    comboIndex = m_ComboOrgParent.FindString(0, parentID);
    //}
    m_ComboOrgParent.SetCurSel(curOrg->iParentID);
    m_ComboBoundaryLine.SetCurSel(curOrg->iBoundaryRail);
    //CString str;
    str.Format(_T("%d"), curOrg->iBoundaryStartKM);
    GetDlgItem(IDC_EDIT_BOUNDARYSTARTKM)->SetWindowText(str);
    str.Format(_T("%d"), curOrg->iBoundaryEndKM);
    GetDlgItem(IDC_EDIT_BOUNDARYENDKM)->SetWindowText(str);
    *pResult = 0;
}

void COrgList::OnTvnItemexpandedTreeOrglist(NMHDR *pNMHDR, LRESULT *pResult)
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


void COrgList::OnBnClickedBtnAddorg()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString orgID;
    CString orgName;
    int comboIndex;
    OrganizationInfo* parentOrg = NULL;
    GetDlgItem(IDC_EDIT_ORGID)->GetWindowText(orgID);
    GetDlgItem(IDC_EDIT_ORGNAME)->GetWindowText(orgName);
    comboIndex = m_ComboOrgParent.GetCurSel();
    if (comboIndex == -1)
    {
        AfxMessageBox(_T("��ѡ���ϼ�����"));
        return;
    }
    else
    {
        parentOrg = (OrganizationInfo*)m_ComboOrgParent.GetItemData(comboIndex);
    }
	if(parentOrg->iOrgLevel == 4)
	{
		AfxMessageBox(_T("�û����������"));
		return;
	}
    int iOrgID = _ttoi(orgID);
    BOOL foundID = FALSE;
    for(size_t i=0; i<m_CRWDSClientView->m_Org.size(); i++)
    {//�ж��Ƿ��ظ��Ļ�����
        if (iOrgID == m_CRWDSClientView->m_Org[i]->iOrgID)
        {
            foundID = TRUE;
            break;
        }
    }
    if (foundID)
    {
        AfxMessageBox(_T("�������ظ�"));
        return;
    }

    CString boxMessage;
    boxMessage.Format(_T("ȷ����ӻ���: \r������:%s\r������:%s\r�ϼ�������:%d"), 
                      orgID, orgName, parentOrg->iOrgID);
    if(AfxMessageBox(boxMessage, MB_OKCANCEL) == IDOK)
    {
        OrganizationInfo* newOrg = new OrganizationInfo;
        newOrg->iOrgID = iOrgID;
        newOrg->iOrgName = orgName;
        newOrg->iParentOrg = parentOrg;
        newOrg->iParentID = parentOrg->iOrgID;
		newOrg->iOrgLevel = parentOrg->iOrgLevel+1;
        parentOrg->iChildID.push_back(newOrg->iOrgID);
        parentOrg->iChildOrg.push_back(newOrg);
        newOrg->iBoundaryRail = (RailLine)m_ComboBoundaryLine.GetCurSel();
        CString str;
        GetDlgItem(IDC_EDIT_BOUNDARYSTARTKM)->GetWindowText(str);
        newOrg->iBoundaryStartKM = _ttoi(str);
        GetDlgItem(IDC_EDIT_BOUNDARYENDKM)->GetWindowText(str);
        newOrg->iBoundaryEndKM = _ttoi(str);
        AddOrgToTreeOrg(newOrg, parentOrg);
        
        str.Format(_T("%d"), newOrg->iOrgID);
        m_ComboOrgParent.AddString(str);
        m_ComboOrgParent.SetItemData(m_ComboOrgParent.GetCount()-1, (DWORD_PTR)newOrg);
    }
}


void COrgList::OnBnClickedBtnModifyorg()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (!m_SeletedOrg)
    {
        return;
    }
    CString orgID;
    CString orgName;
    //GetDlgItem(IDC_EDIT_ORGID)->GetWindowText(orgID);
    GetDlgItem(IDC_EDIT_ORGNAME)->GetWindowText(orgName);
    //int iOrgID = _ttoi(orgID);
    int comboIndex = m_ComboOrgParent.GetCurSel();
    OrganizationInfo* parentOrg = (OrganizationInfo*)m_ComboOrgParent.GetItemData(comboIndex);
    if (parentOrg && parentOrg != m_SeletedOrg->iParentOrg)
    {
        AfxMessageBox(_T("�ϼ����������޸�"));
        return;
    }
    if (AfxMessageBox(_T("ȷ���޸ģ�"), MB_OKCANCEL) == IDOK)
    {
        //if (m_SeletedOrg->iOrgID != iOrgID)
        //{//�޸�combo����������
        //    CString str;
        //    str.Format(_T("%d"), m_SeletedOrg->iOrgID);
        //    int original = m_ComboOrgParent.FindString(0, str);
        //    m_ComboOrgParent.DeleteString(original);
        //    str.Format(_T("%d"), iOrgID);
        //    m_ComboOrgParent.InsertString(original, str);
        //    m_ComboOrgParent.SetItemData(original, (DWORD_PTR)m_SeletedOrg);
        //}
        //m_SeletedOrg->iOrgID = iOrgID;
        m_SeletedOrg->iOrgName = orgName;

        m_SeletedOrg->iBoundaryRail = (RailLine)m_ComboBoundaryLine.GetCurSel();
        CString str;
        GetDlgItem(IDC_EDIT_BOUNDARYSTARTKM)->GetWindowText(str);
        m_SeletedOrg->iBoundaryStartKM = _ttoi(str);
        GetDlgItem(IDC_EDIT_BOUNDARYENDKM)->GetWindowText(str);
        m_SeletedOrg->iBoundaryEndKM = _ttoi(str);
    }
}


void COrgList::OnBnClickedBtnDeleteorg()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    HTREEITEM curItem = m_TreeOrg.GetSelectedItem();
    if (curItem == NULL)
    {
        return;
    }
    if (AfxMessageBox(_T("ȷ��ɾ����"), MB_OKCANCEL) == IDOK)
    {
        OrganizationInfo* org = (OrganizationInfo*)m_TreeOrg.GetItemData(curItem);
        delete org;
        m_TreeOrg.DeleteItem(curItem);
    }
}

