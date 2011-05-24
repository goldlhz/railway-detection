#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"


// CRecordStaff 对话框

class CRecordStaff : public CDialogEx
{
	DECLARE_DYNAMIC(CRecordStaff)

public:
	CRecordStaff(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRecordStaff();

// 对话框数据
	enum { IDD = IDD_RECORDSTAFF };
public:
    int GetSelect();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
    CRWDSClientView* m_CRWDSClientView;
    CComboBox m_ComboRecord;
    CDateTimeCtrl m_DateTimeRecord;
    int m_Select;
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    
    virtual BOOL OnInitDialog();
};
