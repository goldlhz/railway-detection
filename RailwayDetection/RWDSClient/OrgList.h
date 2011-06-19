#pragma once
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"
#include "afxcmn.h"
#include "afxwin.h"


// COrgList 对话框

class COrgList : public CDialogEx
{
	DECLARE_DYNAMIC(COrgList)

public:
	COrgList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COrgList();

// 对话框数据
	enum { IDD = IDD_SETORG };
public:
    void LoadOrgTreeCtrl();
    void OrgListVisitForAddComboOrgParent(OrganizationInfo* aOrg);
    HTREEITEM FindItemFromTreeOrg(HTREEITEM aTreeItem, DWORD_PTR aItemData);
    int AddOrgToTreeOrg(OrganizationInfo* aAddedOrg, OrganizationInfo* aParentOrg);
    //OrganizationInfo* GetOrgParent(int aParentID);
private:
    CRWDSClientView* m_CRWDSClientView;
    CTreeCtrl m_TreeOrg;
    CComboBox m_ComboOrgParent;
    OrganizationInfo* m_SeletedOrg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTvnSelchangedTreeOrglist(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL OnInitDialog();
    afx_msg void OnTvnItemexpandedTreeOrglist(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnAddorg();
    afx_msg void OnBnClickedBtnModifyorg();
    afx_msg void OnBnClickedBtnDeleteorg();
    CComboBox m_ComboBoundaryLine;
};
