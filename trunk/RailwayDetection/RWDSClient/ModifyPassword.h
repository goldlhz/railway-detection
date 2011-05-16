#pragma once


// CModifyPassword 对话框

class CModifyPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyPassword)

public:
	CModifyPassword(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModifyPassword();

// 对话框数据
	enum { IDD = IDD_SETPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    void SetPassword(CString aPassword);
    CString GetPassword();
private:
    CString m_Password;
public:
    afx_msg void OnBnClickedOk();
};
