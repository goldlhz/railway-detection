#pragma once


// CSystemConfigure �Ի���

class CSystemConfigure : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemConfigure)

public:
	CSystemConfigure(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSystemConfigure();

// �Ի�������
	enum { IDD = IDD_SETCONFIGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnConfiguremodify();
    afx_msg void OnBnClickedBtnConfiguredefault();
};
