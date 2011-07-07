#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"


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
private:
    CRWDSClientView* m_RWDSClientView;
public:
    CListCtrl m_ListCtrl;
    CComboBox m_ComboReportMonth;
public:
    virtual BOOL OnInitDialog();
    
    afx_msg void OnBnClickedBtnSearchreport();
    afx_msg void OnNMDblclkListReport(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnViewdetail();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedBtnExportreport();
    void GetCellName(int nRow, int nCol, CString &strName);
};
