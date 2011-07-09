#pragma once
#include "afxwin.h"


// CPermissionGroup 对话框

class CPermissionGroup : public CDialogEx
{
	DECLARE_DYNAMIC(CPermissionGroup)

public:
	CPermissionGroup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPermissionGroup();

// 对话框数据
	enum { IDD = IDD_SETPEMISSIONGROUP };

public:
    void SetPermissionValue(unsigned int aPermissionValue);
    unsigned int GetPermisstionValue();
private:
    unsigned int m_PermissionValue;
public:
    CComboBox m_ComboPermissionGroup;
    vector<CButton*> m_CheckBoxGroup;
    CButton m_CheckBox1;
    CButton m_CheckBox2;
    CButton m_CheckBox3;
    CButton m_CheckBox4;
    CButton m_CheckBox5;
    CButton m_CheckBox6;
    CButton m_CheckBox7;
    CButton m_CheckBox8;
    CButton m_CheckBox9;
    CButton m_CheckBox10;
    CButton m_CheckBox11;
    CButton m_CheckBox12;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    virtual BOOL OnInitDialog();
    
    
    afx_msg void OnCbnSelchangeComboGroup();
    afx_msg void OnBnClickedBtnPermisstiongroupmodify();
};
