// SystemConfigure.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "SystemConfigure.h"
#include "afxdialogex.h"


// CSystemConfigure �Ի���

IMPLEMENT_DYNAMIC(CSystemConfigure, CDialogEx)

CSystemConfigure::CSystemConfigure(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSystemConfigure::IDD, pParent)
{

}

CSystemConfigure::~CSystemConfigure()
{
}

void CSystemConfigure::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSystemConfigure, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_CONFIGUREMODIFY, &CSystemConfigure::OnBnClickedBtnConfiguremodify)
    ON_BN_CLICKED(IDC_BTN_CONFIGUREDEFAULT, &CSystemConfigure::OnBnClickedBtnConfiguredefault)
END_MESSAGE_MAP()


// CSystemConfigure ��Ϣ�������


void CSystemConfigure::OnBnClickedBtnConfiguremodify()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CSystemConfigure::OnBnClickedBtnConfiguredefault()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
}
