#pragma once
#include "afxcmn.h"


// CReport �Ի���

class CReport : public CDialogEx
{
	DECLARE_DYNAMIC(CReport)

public:
	CReport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReport();

// �Ի�������
	enum { IDD = IDD_REVIEWREPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_ListCtrl;
    virtual BOOL OnInitDialog();
};
