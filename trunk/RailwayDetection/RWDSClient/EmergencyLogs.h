#pragma once
#include "afxcmn.h"
#include "Datadef.h"


// CEmergencyLogs 对话框

class CEmergencyLogs : public CDialogEx
{
	DECLARE_DYNAMIC(CEmergencyLogs)

public:
	CEmergencyLogs(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEmergencyLogs();

// 对话框数据
	enum { IDD = IDD_EMERGENCYLOGS };
public:
    void SetEmgergencyTask(EmergencyTaskInfo* aEmergencyTask);
private:
    EmergencyTaskInfo* m_EmergencyTask;
    vector<EmergencyLogs*> m_LogsList;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_ListCtrl;
    virtual BOOL OnInitDialog();
};
