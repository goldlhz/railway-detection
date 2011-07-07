#pragma once
#include "afxcmn.h"
#include "Datadef.h"


// CStaffLogs �Ի���

class CStaffLogs : public CDialogEx
{
	DECLARE_DYNAMIC(CStaffLogs)

public:
	CStaffLogs(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStaffLogs();

// �Ի�������
	enum { IDD = IDD_REVIEWSTAFFLOGS };
public:
    void SetReportInfo(ReportInfo* aReport);
private:
    ReportInfo* m_Report;
    ReportDetail m_Detail;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_ListCtrl;
    virtual BOOL OnInitDialog();
};
