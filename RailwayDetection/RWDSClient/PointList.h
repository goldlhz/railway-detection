#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Datadef.h"
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"


// CPointList 对话框

class CPointList : public CDialogEx
{
	DECLARE_DYNAMIC(CPointList)

public:
	CPointList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPointList();

// 对话框数据
	enum { IDD = IDD_SETPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	bool m_Modifying;
	CRWDSClientView* m_CRWDSClientView;
	CListCtrl m_ListCtrl;
	CComboBox m_ComboUpDown;
	CComboBox m_ComboRailLine;

protected:
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnPointadd();
	afx_msg void OnBnClickedBtnPointmodify();
	afx_msg void OnBnClickedBtnPointdelete();
	afx_msg void OnBnClickedBtnPointok();
	afx_msg void OnLvnItemchangedPointlist(NMHDR *pNMHDR, LRESULT *pResult);
};
