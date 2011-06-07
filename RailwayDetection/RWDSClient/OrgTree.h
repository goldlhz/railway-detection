#pragma once
#include "afxcmn.h"
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"


// COrgTree �Ի���

class COrgTree : public CDialogEx
{
    friend class CEmergencyTask;
	DECLARE_DYNAMIC(COrgTree)

public:
	COrgTree(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COrgTree();

// �Ի�������
	enum { IDD = IDD_ORGTREE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    void SetEmergencyTaskInfo(EmergencyTaskInfo* a_EmergencyTask);
    void InsertListSelectedData();
public:
    CTreeCtrl m_TreeOrg;
    CListCtrl m_ListOrgData;
    CListCtrl m_ListSelectedData;
private:
    CImageList m_OrgImages;
    CRWDSClientView* m_RWDSClientView;
    EmergencyTaskInfo* m_EmergencyTask;
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedBtnAdddata();
    afx_msg void OnBnClickedBtnDeletedata();
    afx_msg void OnBnClickedBtnOk();
    afx_msg void OnBnClickedCancel();
    
    afx_msg void OnTvnItemexpandingTreeOrg(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnItemexpandedTreeOrg(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickTreeOrg(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnSelchangedTreeOrg(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMDblclkListOrgdata(NMHDR *pNMHDR, LRESULT *pResult);
    
};
