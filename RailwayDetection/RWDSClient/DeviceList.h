#pragma once
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"
#include "afxcmn.h"
#include "afxwin.h"


// CDeviceList �Ի���

class CDeviceList : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceList)

public:
	CDeviceList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDeviceList();

// �Ի�������
	enum { IDD = IDD_SETDEVICE };
public:
    BOOL CheckDivceId(int aDeviceID);
private:
    CListCtrl m_ListCtrl;
    CComboBox m_ComboDeviceType;
    CRWDSClientView* m_CRWDSClientView;
    DeviceInfo* m_Selected;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnLvnItemchangedDevicelist(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnDeviceadd();
    afx_msg void OnBnClickedBtnDevicemodify();
    afx_msg void OnBnClickedBtnDevicedelete();

};
