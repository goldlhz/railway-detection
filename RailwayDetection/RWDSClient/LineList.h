#pragma once
#include "afxcmn.h"
#include "Datadef.h"


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
	void SetLine(vector<LineInfo> aLine);
	vector<LineInfo> GetLine();
protected:
	CListCtrl m_ListCtrl;
	vector<LineInfo> m_Line;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedBtnLineadd();
	afx_msg void OnClickedBtnLinemodify();
	afx_msg void OnClickedBtnLinedelete();
	afx_msg void OnClickedBtnLineok();
};
