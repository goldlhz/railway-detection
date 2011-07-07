#pragma once
#include "afxcmn.h"
#include "Datadef.h"


// CEmergencyLogs �Ի���

class CEmergencyLogs : public CDialogEx
{
	DECLARE_DYNAMIC(CEmergencyLogs)

public:
	CEmergencyLogs(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEmergencyLogs();

// �Ի�������
	enum { IDD = IDD_EMERGENCYLOGS };
public:
    void SetEmgergencyTask(EmergencyTaskInfo* aEmergencyTask);
private:
    EmergencyTaskInfo* m_EmergencyTask;
    vector<EmergencyLogs*> m_LogsList;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_ListCtrl;
    virtual BOOL OnInitDialog();
};
