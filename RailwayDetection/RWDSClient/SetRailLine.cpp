// SetRailLine.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "SetRailLine.h"
#include "afxdialogex.h"
#include "DataService.h"
#include "CmdDefine.h"


// CSetRailLine �Ի���

IMPLEMENT_DYNAMIC(CSetRailLine, CDialogEx)

CSetRailLine::CSetRailLine(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetRailLine::IDD, pParent)
{

}

CSetRailLine::~CSetRailLine()
{
}

void CSetRailLine::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_RAIL, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CSetRailLine, CDialogEx)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RAIL, &CSetRailLine::OnLvnItemchangedListRail)
    ON_BN_CLICKED(IDC_BTN_RAILADD, &CSetRailLine::OnBnClickedBtnRailadd)
    ON_BN_CLICKED(IDC_BTN_RAILMODIFY, &CSetRailLine::OnBnClickedBtnRailmodify)
    ON_BN_CLICKED(IDC_BTN_RAILDELETE, &CSetRailLine::OnBnClickedBtnRaildelete)
END_MESSAGE_MAP()


// CSetRailLine ��Ϣ�������


BOOL CSetRailLine::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);

    m_ListCtrl.InsertColumn(0, _T("���"), LVCFMT_LEFT, clientRect.Width()/4);
    m_ListCtrl.InsertColumn(1, _T("��·��"), LVCFMT_LEFT, clientRect.Width()/4*3);
    CString str;
    for (size_t i=0; i<gRailLineList.size(); i++)
    {
        str.Format(_T("%d"), gRailLineList[i]->iRailID);
        m_ListCtrl.InsertItem(i, str);
        m_ListCtrl.SetItemText(i, 1, gRailLineList[i]->iRailName);
        m_ListCtrl.SetItemData(i, (DWORD_PTR) gRailLineList[i]);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CSetRailLine::OnLvnItemchangedListRail(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    POSITION pos;
    pos = m_ListCtrl.GetFirstSelectedItemPosition();
    int select = m_ListCtrl.GetNextSelectedItem(pos);  
    if (select<0)
    {
        return;
    }
    RailLine* rail = (RailLine*)m_ListCtrl.GetItemData(select);
    GetDlgItem(IDC_EDIT_RAILNAME)->SetWindowText(rail->iRailName);
    *pResult = 0;
}


void CSetRailLine::OnBnClickedBtnRailadd()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    RailLine* rail = new RailLine;
    rail->iRailName = _T("��·1");
    rail->iRailID = 0;
    rail->iRailID = SetRailLine(CMD_RAIL_ADD, rail);
    if (rail->iRailID <= 0)
    {
        delete rail;
        AfxMessageBox(_T("���ʧ��, ���ܻ�ȡ��·���!"));
    }
    gRailLineList.push_back(rail);
    CString str;
    str.Format(_T("%d"), rail->iRailID);
    int index = m_ListCtrl.GetItemCount();
    m_ListCtrl.InsertItem(index, str);
    m_ListCtrl.SetItemText(index, 1, rail->iRailName);
    m_ListCtrl.SetItemData(index, (DWORD_PTR)rail);

}


void CSetRailLine::OnBnClickedBtnRailmodify()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int select = m_ListCtrl.GetSelectionMark();
    if (select < 0)
        return;
    RailLine* rail = (RailLine*)m_ListCtrl.GetItemData(select);
    GetDlgItem(IDC_EDIT_RAILNAME)->GetWindowText(rail->iRailName);
    SetRailLine(CMD_RAIL_MODIFY, rail);
    AfxMessageBox(_T("�޸ĳɹ�!"));
}


void CSetRailLine::OnBnClickedBtnRaildelete()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int select = m_ListCtrl.GetSelectionMark();
    if (select < 0)
        return;
    RailLine* rail = (RailLine*)m_ListCtrl.GetItemData(select);
    if(AfxMessageBox(_T("ȷ��ɾ��?")) == IDOK)
    {
        SetRailLine(CMD_RAIL_DELETE, rail);
        for (size_t i=0; i<gRailLineList.size(); i++)
        {
            if (rail == gRailLineList[i])
            {
                gRailLineList.erase(gRailLineList.begin()+i);
                break;
            }
        }
        delete rail;
    }
}
