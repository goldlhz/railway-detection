#pragma once
#include "afxcmn.h"


// CSetRailLine �Ի���

class CSetRailLine : public CDialogEx
{
	DECLARE_DYNAMIC(CSetRailLine)

public:
	CSetRailLine(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetRailLine();

// �Ի�������
	enum { IDD = IDD_SETRAILLINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_ListCtrl;
    virtual BOOL OnInitDialog();
    afx_msg void OnLvnItemchangedListRail(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnRailadd();
    afx_msg void OnBnClickedBtnRailmodify();
    afx_msg void OnBnClickedBtnRaildelete();
};
