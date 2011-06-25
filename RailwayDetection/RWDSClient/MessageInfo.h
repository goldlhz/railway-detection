#pragma once


// CMessageInfo 对话框

class CMessageInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageInfo)

public:
	CMessageInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMessageInfo();

// 对话框数据
	enum { IDD = IDD_FLASHWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
