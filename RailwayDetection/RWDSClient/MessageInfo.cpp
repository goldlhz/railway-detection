// MessageInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "MessageInfo.h"
#include "afxdialogex.h"


// CMessageInfo �Ի���

IMPLEMENT_DYNAMIC(CMessageInfo, CDialogEx)

CMessageInfo::CMessageInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessageInfo::IDD, pParent)
{

}

CMessageInfo::~CMessageInfo()
{
}

void CMessageInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMessageInfo, CDialogEx)
END_MESSAGE_MAP()


// CMessageInfo ��Ϣ�������
