#pragma once
#include "afxcmn.h"
#include "Datadef.h"
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"


// CStaffLogs 对话框

class CStaffLogs : public CDialogEx
{
	DECLARE_DYNAMIC(CStaffLogs)

public:
	CStaffLogs(int aStartWay, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStaffLogs();

// 对话框数据
	enum { IDD = IDD_REVIEWSTAFFLOGS };
public:
    void SetReportInfo(ReportInfo* aReport);
    void GetLogs(int aCmd);
    void ShowStaffInfo(CString aInfo);
    void SetTitle(CString aTitle);
    void SetAlarmViewVisible(BOOL avisible);
    void SetStaffLogsVisible(BOOL aVisible);
    void ShowCtrlList(ReportDetail& aReport);
private:
    CRWDSClientView* m_RWDSClientView;
    ReportInfo* m_Report;
    ReportDetail m_AlarmReport;
    CString m_AlarmMonth;
    ReportDetail m_Detail;
    CString m_ShowInfo;
    CString m_Title;
    int m_StartWay;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_ListCtrl;
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedCancel();

    afx_msg void OnBnClickedBtnSearch();

};
