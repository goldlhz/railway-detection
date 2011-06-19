#pragma once
#include "afxcmn.h"
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"
#include "afxwin.h"


// CSchedule 对话框

class CSchedule : public CDialogEx
{
	DECLARE_DYNAMIC(CSchedule)

public:
	CSchedule(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSchedule();

// 对话框数据
	enum { IDD = IDD_SCHEDULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    void AddStaffToListBox();
protected:
	CRWDSClientView* m_CRWDSClientView;
	//ScheduleLine* m_SelectedSchedule;
	LineInfo* m_SelectedLine;
	CListCtrl m_ListCtrl;
	CListCtrl m_ListArriveTime;
    CComboBox m_ComboStartDay;
    CListBox m_ListStaffSelected;
    CListBox m_ListStaffUnselected;
    vector<StaffInfo*> m_StaffSeleted;
    vector<StaffInfo*> m_StaffUnseleted;//选择的线有哪些员工可以走
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedSchedulelist(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	
public:
	afx_msg void OnLvnItemchangedListarrivettime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnModifytime();
	afx_msg void OnBnClickedBtnModifycalender();
    //afx_msg void OnBnClickedBtnCleanstaff();
    afx_msg void OnBnClickedBtnAddliststaff();
    afx_msg void OnBnClickedBtnRemoveliststaff();
    //afx_msg void OnBnClickedBtnConfirmstaff();

};
