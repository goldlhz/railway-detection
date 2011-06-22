// RecordStaff.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "RecordStaff.h"
#include "afxdialogex.h"


// CRecordStaff 对话框

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
    DDX_Control(pDX, IDC_RADIO_CURRENTTRACK, m_RadioCurrentTrack);
}


BEGIN_MESSAGE_MAP(CRecordStaff, CDialogEx)
    ON_BN_CLICKED(IDOK, &CRecordStaff::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CRecordStaff::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_RADIO_CURRENTTRACK, &CRecordStaff::OnBnClickedRadioCurrenttrack)
    ON_BN_CLICKED(IDC_RADIO_RECORD, &CRecordStaff::OnBnClickedRadioRecord)
END_MESSAGE_MAP()


// CRecordStaff 消息处理程序


BOOL CRecordStaff::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    if (m_CRWDSClientView->m_DisplayFlag == KStaffLog)
    {
        for(size_t i=0; i<m_CRWDSClientView->m_CurrentOrg->iStaff.size(); i++)
        {
            m_ComboRecord.AddString(m_CRWDSClientView->m_CurrentOrg->iStaff[i]->iName);
        }
    }
    else if (m_CRWDSClientView->m_DisplayFlag == KDeviceLog)
    {
    }
    SetDateVisible(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

int CRecordStaff::GetSelect()
{
    return m_Select;
}



void CRecordStaff::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    m_Select = m_ComboRecord.GetCurSel();
    if (m_Select == -1)
    {
        if (m_CRWDSClientView->m_DisplayFlag == KStaffLog)
        {
            AfxMessageBox(_T("请选择查看人员"));
            return;
        }
        else if (m_CRWDSClientView->m_DisplayFlag == KDeviceLog)
        {
        }
    }
    if (GetCheckedRadioButton(IDC_RADIO_CURRENTTRACK, IDC_RADIO_RECORD) == IDC_RADIO_RECORD)
    {
        m_DateTimeRecord.GetTime(m_PickDateTime);
    }
    else if (GetCheckedRadioButton(IDC_RADIO_CURRENTTRACK, IDC_RADIO_RECORD) == IDC_RADIO_CURRENTTRACK)
    {
        m_PickDateTime = CTime::GetTickCount();
    }
    else
    {
        AfxMessageBox(_T("请记录"));
    }

    CDialogEx::OnOK();
}


void CRecordStaff::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    CDialogEx::OnCancel();
}

void CRecordStaff::OnBnClickedRadioCurrenttrack()
{
    // TODO: 在此添加控件通知处理程序代码
    if(GetCheckedRadioButton(IDC_RADIO_CURRENTTRACK, IDC_RADIO_RECORD) == IDC_RADIO_CURRENTTRACK)
    {
        SetDateVisible(FALSE);
    }
}

void CRecordStaff::OnBnClickedRadioRecord()
{
    // TODO: 在此添加控件通知处理程序代码
    if(GetCheckedRadioButton(IDC_RADIO_CURRENTTRACK, IDC_RADIO_RECORD) == IDC_RADIO_RECORD)
    {
        SetDateVisible(TRUE);
    }
}

void CRecordStaff::SetDateVisible( BOOL aVisible )
{
    if (aVisible)
    {
        GetDlgItem(IDC_DATETIME_RECORD)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_DATE)->ShowWindow(SW_SHOW);
    }
    else
    {
        GetDlgItem(IDC_DATETIME_RECORD)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_DATE)->ShowWindow(SW_HIDE);
    }

}

time_t CRecordStaff::GetPickDateTime()
{
    return m_PickDateTime.GetTime();
}


