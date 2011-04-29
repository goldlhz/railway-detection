#pragma once
#include "afxcmn.h"
#include "Datadef.h"
#include "afxwin.h"
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"


// CLineList 对话框

class CLineList : public CDialogEx
{
	DECLARE_DYNAMIC(CLineList)

public:
	CLineList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLineList();

// 对话框数据
	enum { IDD = IDD_SETLINE };

public:
	void StrLineKmToLineKm(CString aStrLine, RailLine& aRailLine, double& aKm);
protected:
	CRWDSClientView* m_CRWDSClientView;
	CListCtrl m_ListCtrl;
	CListBox m_ListAllPoint;
	CListBox m_ListSelectedPoint;
	vector<MapPoint*> m_Selected;
	vector<MapPoint*> m_Unselected;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnLineadd();
	afx_msg void OnClickedBtnLinemodify();
	afx_msg void OnClickedBtnLinedelete();
	afx_msg void OnClickedBtnLineok();
	afx_msg void OnLvnItemchangedLinelist(NMHDR *pNMHDR, LRESULT *pResult);
};
