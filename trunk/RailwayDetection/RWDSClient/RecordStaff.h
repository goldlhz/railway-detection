#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"


// CRecordStaff �Ի���

class CRecordStaff : public CDialogEx
{
	DECLARE_DYNAMIC(CRecordStaff)

public:
	CRecordStaff(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRecordStaff();

// �Ի�������
	enum { IDD = IDD_RECORDSTAFF };
public:
    int GetSelect();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
