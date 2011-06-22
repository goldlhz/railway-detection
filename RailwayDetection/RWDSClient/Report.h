#pragma once
#include "afxcmn.h"


// CReport 对话框

class CReport : public CDialogEx
{
	DECLARE_DYNAMIC(CReport)

public:
	CReport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReport();

// 对话框数据
	enum { IDD = IDD_REVIEWREPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_ListCtrl;
    virtual BOOL OnInitDialog();
};
