#pragma once
#include "afxcmn.h"
#include "Datadef.h"


// CLineList �Ի���

class CLineList : public CDialogEx
{
	DECLARE_DYNAMIC(CLineList)

public:
	CLineList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLineList();

// �Ի�������
	enum { IDD = IDD_SETLINE };

public:
	void SetLine(vector<LineInfo> aLine);
	vector<LineInfo> GetLine();
protected:
	CListCtrl m_ListCtrl;
	vector<LineInfo> m_Line;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedBtnLineadd();
	afx_msg void OnClickedBtnLinemodify();
	afx_msg void OnClickedBtnLinedelete();
	afx_msg void OnClickedBtnLineok();
};
