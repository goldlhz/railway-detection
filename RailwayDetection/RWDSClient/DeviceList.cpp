// DeviceList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "DeviceList.h"
#include "afxdialogex.h"


// CDeviceList �Ի���

IMPLEMENT_DYNAMIC(CDeviceList, CDialogEx)

CDeviceList::CDeviceList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceList::IDD, pParent)
{
    m_CRWDSClientView = static_cast<CRWDSClientView*>(pParent);
}

CDeviceList::~CDeviceList()
{
}

void CDeviceList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDeviceList, CDialogEx)
END_MESSAGE_MAP()


// CDeviceList ��Ϣ�������
