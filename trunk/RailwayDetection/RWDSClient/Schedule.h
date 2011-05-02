#pragma once
#include "afxcmn.h"
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"


// CSchedule �Ի���

class CSchedule : public CDialogEx
{
	DECLARE_DYNAMIC(CSchedule)

public:
	CSchedule(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSchedule();

// �Ի�������
	enum { IDD = IDD_SCHEDULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	CRWDSClientView* m_CRWDSClientView;
	ScheduleLine* m_SelectedSchedule;
	CListCtrl m_ListCtrl;
	CListCtrl m_ListArriveTime;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedSchedulelist(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	
public:
	afx_msg void OnLvnItemchangedListarrivettime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnModifytime();
};
