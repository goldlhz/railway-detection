#pragma once
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"
#include "afxcmn.h"
#include "afxwin.h"


// CEmergencyTask 对话框

class CEmergencyTask : public CDialogEx
{
	DECLARE_DYNAMIC(CEmergencyTask)

public:
	CEmergencyTask(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEmergencyTask();

// 对话框数据
	enum { IDD = IDD_EMERGENCYTASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    int CreateEmergencyTaskID();
private:
    CRWDSClientView* m_CRWDSClientView;
    CListBox m_ListAllPoint;
    CListBox m_ListSelectedPoint;
    vector<MapPoint*> m_Selected;
    vector<MapPoint*> m_Unselected;
    CListCtrl m_ListCtrl;
    CComboBox m_ComboEmergencyStatus;
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnLvnItemchangedEmergencylist(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnEmergencyadd1();
    afx_msg void OnBnClickedBtnEmergencyremove1();
    afx_msg void OnBnClickedBtnEmergencyadd();
    afx_msg void OnBnClickedBtnEmergencymodify();
    afx_msg void OnBnClickedBtnEmergencydelete();
    afx_msg void OnBnClickedBtnEmergencyok();


};
