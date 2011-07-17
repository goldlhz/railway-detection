#pragma once


// CSystemConfigure 对话框

class CSystemConfigure : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemConfigure)

public:
	CSystemConfigure(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSystemConfigure();

// 对话框数据
	enum { IDD = IDD_SETCONFIGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnConfiguremodify();
    afx_msg void OnBnClickedBtnConfiguredefault();
};
