#pragma once
#include "afxcmn.h"
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"
#include "afxwin.h"


// CStaffList 对话框

class CStaffList : public CDialogEx
{
	DECLARE_DYNAMIC(CStaffList)

public:
	CStaffList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStaffList();

// 对话框数据
	enum { IDD = IDD_SETSTAFF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
    int CreateStaffID();
private:
    CRWDSClientView* m_CRWDSClientView;
    StaffInfo* m_SeletedStaff;
    CButton m_CheckLoginPermission;
    CButton m_BtnSetPassword;
    CListCtrl m_ListCtrl;
    CComboBox m_ComboStaffDevice;
    //CComboBox m_ComboStaffPermission;
public:

    virtual BOOL OnInitDialog();
    afx_msg void OnLvnItemchangedStafflist(NMHDR *pNMHDR, LRESULT *pResult);

    afx_msg void OnBnClickedCheckLoginpermission();

    afx_msg void OnBnClickedBtnSetpassword();
    afx_msg void OnBnClickedBtnAddstaff();
    afx_msg void OnBnClickedBtnmodifystaff();
    afx_msg void OnBnClickedBtnDelstaff();
    


    afx_msg void OnBnClickedBtnSetpermission();
    afx_msg void OnBnClickedBtnInitialpassword();
};
