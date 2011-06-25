#pragma once
#include "afxcmn.h"


// CVoiceList �Ի���

class CVoiceList : public CDialogEx
{
	DECLARE_DYNAMIC(CVoiceList)

public:
	CVoiceList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVoiceList();

// �Ի�������
	enum { IDD = IDD_PLAYVOICE };

public:
	void AddVoiceToListCtrl(CString aVoicePath);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CListCtrl m_ListCtrl;
	afx_msg void OnNMDblclkVoiceList(NMHDR *pNMHDR, LRESULT *pResult);
};
