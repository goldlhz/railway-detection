#pragma once
#include "afxcmn.h"
#include "Datadef.h"


// CStaffLogs 对话框

class CStaffLogs : public CDialogEx
{
	DECLARE_DYNAMIC(CStaffLogs)

public:
	CStaffLogs(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStaffLogs();

// 对话框数据
	enum { IDD = IDD_REVIEWSTAFFLOGS };
public:
    void SetReportInfo(ReportInfo* aReport);
private:
    ReportInfo* m_Report;
    ReportDetail m_Detail;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_ListCtrl;
    virtual BOOL OnInitDialog();
};
