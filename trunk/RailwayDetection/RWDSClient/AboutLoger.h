#pragma once


// CAboutLoger �Ի���

class CAboutLoger : public CDialogEx
{
	DECLARE_DYNAMIC(CAboutLoger)

public:
	CAboutLoger(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAboutLoger();

// �Ի�������
	enum { IDD = IDD_ABOUTLOGER };
public:
    void SetName(CString aName);
    void SetOrg(CString aOrg);
private:
    CString m_Name;
    CString m_Org;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
};
