// RecordStaff.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "RecordStaff.h"
#include "afxdialogex.h"


// CRecordStaff �Ի���

IMPLEMENT_DYNAMIC(CRecordStaff, CDialogEx)

CRecordStaff::CRecordStaff(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRecordStaff::IDD, pParent)
{
    m_CRWDSClientView = static_cast<CRWDSClientView*>(pParent);
}

CRecordStaff::~CRecordStaff()
{
}

void CRecordStaff::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_RECORD, m_ComboRecord);
    DDX_Control(pDX, IDC_DATETIME_RECORD, m_DateTimeRecord);
}


BEGIN_MESSAGE_MAP(CRecordStaff, CDialogEx)
    ON_BN_CLICKED(IDOK, &CRecordStaff::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CRecordStaff::OnBnClickedCancel)
END_MESSAGE_MAP()


// CRecordStaff ��Ϣ�������


void CRecordStaff::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_Select = m_ComboRecord.GetCurSel();
    if (m_Select == -1)
    {
        if (m_CRWDSClientView->m_DisplayFlag == KStaffLog)
        {
            AfxMessageBox(_T("��ѡ��鿴��Ա"));
            return;
        }
        else if (m_CRWDSClientView->m_DisplayFlag == KDeviceLog)
        {
        }
    }
    CDialogEx::OnOK();
}


void CRecordStaff::OnBnClickedCancel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CDialogEx::OnCancel();
}


BOOL CRecordStaff::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    if (m_CRWDSClientView->m_DisplayFlag == KStaffLog)
    {
        for(size_t i=0; i<m_CRWDSClientView->m_Staff.size(); i++)
        {
            m_ComboRecord.AddString(m_CRWDSClientView->m_Staff[i]->iName);
        }
    }
    else if (m_CRWDSClientView->m_DisplayFlag == KDeviceLog)
    {
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

int CRecordStaff::GetSelect()
{
    return m_Select;
}
