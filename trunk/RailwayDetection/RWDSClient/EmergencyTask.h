#pragma once
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"
#include "afxcmn.h"
#include "afxwin.h"


// CEmergencyTask 对话框

class CEmergencyTask : public CDialogEx
{
    friend class COrgTree;
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
    //CComboBox m_Combo_StartKM;
    //CComboBox m_Combo_EndKM;
    CListCtrl m_ListCtrl;
    CComboBox m_ComboEmergencyStatus;
    //CListBox m_ListEmSeletedStaff;
    //CListBox m_ListEmUnseletedStaff;
    CComboBox m_ComboRailLine;
    EmergencyTaskInfo* m_SeletedTask;
    //vector<StaffInfo*> m_StaffSeleted;
    //vector<StaffInfo*> m_StaffUnseleted;//选择的线有哪些员工可以走
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnLvnItemchangedEmergencylist(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnEmergencyadd();
    afx_msg void OnBnClickedBtnEmergencymodify();
    afx_msg void OnBnClickedBtnEmergencydelete();
    afx_msg void OnBnClickedBtnEmergencyok();
//    afx_msg void OnBnClickedBtnEmaddstaff();
//    afx_msg void OnBnClickedBtnEmremovestaff();
    afx_msg void OnBnClickedBtnSetemstaff();
    afx_msg void OnNMDblclkEmergencylist(NMHDR *pNMHDR, LRESULT *pResult);
    
};
