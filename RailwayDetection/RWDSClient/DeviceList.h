#pragma once
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"


// CDeviceList �Ի���

class CDeviceList : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceList)

public:
	CDeviceList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDeviceList();

// �Ի�������
	enum { IDD = IDD_SETDEVICE };
private:
    CRWDSClientView* m_CRWDSClientView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
