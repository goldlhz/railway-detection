#pragma once


// CAboutLoger 对话框

class CAboutLoger : public CDialogEx
{
	DECLARE_DYNAMIC(CAboutLoger)

public:
	CAboutLoger(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAboutLoger();

// 对话框数据
	enum { IDD = IDD_ABOUTLOGER };
public:
    void SetName(CString aName);
    void SetOrg(CString aOrg);
private:
    CString m_Name;
    CString m_Org;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
};
