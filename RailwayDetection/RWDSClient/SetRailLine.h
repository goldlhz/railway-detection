#pragma once
#include "afxcmn.h"


// CSetRailLine 对话框

class CSetRailLine : public CDialogEx
{
	DECLARE_DYNAMIC(CSetRailLine)

public:
	CSetRailLine(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetRailLine();

// 对话框数据
	enum { IDD = IDD_SETRAILLINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_ListCtrl;
    virtual BOOL OnInitDialog();
    afx_msg void OnLvnItemchangedListRail(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnRailadd();
    afx_msg void OnBnClickedBtnRailmodify();
    afx_msg void OnBnClickedBtnRaildelete();
};
