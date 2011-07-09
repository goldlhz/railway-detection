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
    time_t GetPickDateTime();
    StaffInfo* GetSelectedStaff();
    void SetDateVisible(BOOL aVisible);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
