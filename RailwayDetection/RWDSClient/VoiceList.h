#pragma once
#include "afxcmn.h"


// CVoiceList 对话框

class CVoiceList : public CDialogEx
{
	DECLARE_DYNAMIC(CVoiceList)

public:
	CVoiceList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVoiceList();

// 对话框数据
	enum { IDD = IDD_PLAYVOICE };

public:
	void AddVoiceToListCtrl(CString aVoicePath);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CListCtrl m_ListCtrl;
	afx_msg void OnNMDblclkVoiceList(NMHDR *pNMHDR, LRESULT *pResult);
};
