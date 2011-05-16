#pragma once
#include "afxcmn.h"
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"
#include "afxwin.h"


// CStaffList �Ի���

class CStaffList : public CDialogEx
{
	DECLARE_DYNAMIC(CStaffList)

public:
	CStaffList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStaffList();

// �Ի�������
	enum { IDD = IDD_SETSTAFF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
    int CreateStaffID();
private:
    CRWDSClientView* m_CRWDSClientView;
    StaffInfo* m_SeletedStaff;
public:
    CListCtrl m_ListCtrl;
    virtual BOOL OnInitDialog();
    afx_msg void OnLvnItemchangedStafflist(NMHDR *pNMHDR, LRESULT *pResult);
    CListBox m_ArrangeLine;
    afx_msg void OnBnClickedCheckLoginpermission();
    CButton m_CheckLoginPermission;
    CButton m_BtnSetPassword;
    afx_msg void OnBnClickedBtnSetpassword();
    afx_msg void OnBnClickedBtnAddstaff();
    afx_msg void OnBnClickedBtnmodifystaff();
    afx_msg void OnBnClickedBtnDelstaff();
    
};
