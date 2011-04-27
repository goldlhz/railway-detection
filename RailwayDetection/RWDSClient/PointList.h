#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Datadef.h"


// CPointList �Ի���

class CPointList : public CDialogEx
{
	DECLARE_DYNAMIC(CPointList)

public:
	CPointList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPointList();

// �Ի�������
	enum { IDD = IDD_SETPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	CListCtrl m_ListCtrl;
	CComboBox m_ComboUpDown;
	vector<MapPoint> m_Point;
public:
	vector<MapPoint> GetPoint();
	void SetPoint(vector<MapPoint> aPoint);

public:
	afx_msg void OnNMClickPointlist(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnBnClickedBtnAdd();
	//afx_msg void OnBnClickedBtnModify();
	//afx_msg void OnBnClickedBtnDelete();
protected:
	
public:
	virtual BOOL OnInitDialog();
	//afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnPointadd();
	afx_msg void OnBnClickedBtnPointmodify();
	afx_msg void OnBnClickedBtnPointdelete();
	afx_msg void OnBnClickedBtnPointok();
};
