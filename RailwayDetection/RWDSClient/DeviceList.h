#pragma once
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"
#include "afxcmn.h"
#include "afxwin.h"


// CDeviceList 对话框

class CDeviceList : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceList)

public:
	CDeviceList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeviceList();

// 对话框数据
	enum { IDD = IDD_SETDEVICE };
public:
    BOOL CheckDivceId(int aDeviceID);
private:
    CListCtrl m_ListCtrl;
    CComboBox m_ComboDeviceType;
    CRWDSClientView* m_CRWDSClientView;
    DeviceInfo* m_Selected;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnLvnItemchangedDevicelist(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnDeviceadd();
    afx_msg void OnBnClickedBtnDevicemodify();
    afx_msg void OnBnClickedBtnDevicedelete();

};
