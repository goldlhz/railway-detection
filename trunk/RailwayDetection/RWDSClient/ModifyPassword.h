#pragma once


// CModifyPassword �Ի���

class CModifyPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyPassword)

public:
	CModifyPassword(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModifyPassword();

// �Ի�������
	enum { IDD = IDD_SETPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    void SetPassword(CString aPassword);
    CString GetPassword();
private:
    CString m_Password;
public:
    afx_msg void OnBnClickedOk();
};
