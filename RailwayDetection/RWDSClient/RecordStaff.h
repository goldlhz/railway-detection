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
    time_t GetPickDateTime();
    StaffInfo* GetSelectedStaff();
    void SetDateVisible(BOOL aVisible);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
    CRWDSClientView* m_CRWDSClientView;
    CComboBox m_ComboRecord;
    CDateTimeCtrl m_DateTimeRecord;
    CButton m_RadioCurrentTrack;
    int m_Select;
    CTime m_PickDateTime;
    StaffInfo* m_Staff;
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedRadioCurrenttrack();
    
    afx_msg void OnBnClickedRadioRecord();
};
