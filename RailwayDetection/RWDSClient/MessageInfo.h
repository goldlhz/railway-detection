#pragma once


// CMessageInfo �Ի���

class CMessageInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageInfo)

public:
	CMessageInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMessageInfo();

// �Ի�������
	enum { IDD = IDD_FLASHWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
