// PointList.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "PointList.h"
#include "afxdialogex.h"


// CPointList 对话框

IMPLEMENT_DYNAMIC(CPointList, CDialogEx)

CPointList::CPointList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPointList::IDD, pParent)
{

}

CPointList::~CPointList()
{
}

void CPointList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POINTLIST, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO_UPDOWN, m_ComboUpDown);
}


BEGIN_MESSAGE_MAP(CPointList, CDialogEx)
//	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_NOTIFY(NM_CLICK, IDC_POINTLIST, &CPointList::OnNMClickPointlist)
	ON_BN_CLICKED(IDC_BTN_ADD, &CPointList::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_MODIFY, &CPointList::OnBnClickedBtnModify)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CPointList::OnBnClickedBtnDelete)
END_MESSAGE_MAP()


// CPointList 消息处理程序

void CPointList::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CRect clientRect;
	m_ListCtrl.GetClientRect(&clientRect);
	m_ListCtrl.InsertColumn(0, _T("公里"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(1, _T("经度"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(2, _T("纬度"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(4, _T("上下行"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ComboUpDown.ResetContent();
	m_ComboUpDown.AddString(_T("上行"));
	m_ComboUpDown.AddString(_T("下行"));

	

	m_ListCtrl.InsertItem(0, _T("210"));
	m_ListCtrl.SetItemText(0, 1, _T("100"));
	m_ListCtrl.SetItemText(0, 2, _T("110"));
	m_ListCtrl.SetItemText(0, 3, _T("上行"));
	m_ListCtrl.InsertItem(1, _T("123"));
	m_ListCtrl.SetItemText(1, 1, _T("43"));
	m_ListCtrl.SetItemText(1, 2, _T("551"));
	m_ListCtrl.SetItemText(1, 3, _T("下行"));
}


void CPointList::OnNMClickPointlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int select = m_ListCtrl.GetSelectionMark();
	GetDlgItem(IDC_EDIT_KM)->SetWindowText(m_ListCtrl.GetItemText(select, 0));
	GetDlgItem(IDC_EDIT_LAT)->SetWindowText(m_ListCtrl.GetItemText(select, 1));
	GetDlgItem(IDC_EDIT_LON)->SetWindowText(m_ListCtrl.GetItemText(select, 2));
	if (m_ListCtrl.GetItemText(select, 3).Compare(_T("上行")) == 0)
	{
		m_ComboUpDown.SetCurSel(0);
	}
	else
		m_ComboUpDown.SetCurSel(1);
}


void CPointList::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int itemCount = m_ListCtrl.GetItemCount();
	m_ListCtrl.InsertItem(itemCount, _T("100"));
	m_ListCtrl.SetItemText(itemCount, 1, _T("100"));
	m_ListCtrl.SetItemText(itemCount, 2, _T("100"));
	m_ListCtrl.SetItemText(itemCount, 3, _T("上行"));
}


void CPointList::OnBnClickedBtnModify()
{
	// TODO: 在此添加控件通知处理程序代码
	int select = m_ListCtrl.GetSelectionMark();
	if (select < 0)
	{
		return;
	}
	CString str;
	GetDlgItem(IDC_EDIT_KM)->GetWindowText(str);
	m_ListCtrl.SetItemText(select, 0, str);
	GetDlgItem(IDC_EDIT_LAT)->GetWindowText(str);
	m_ListCtrl.SetItemText(select, 1, str);
	GetDlgItem(IDC_EDIT_LON)->GetWindowText(str);
	m_ListCtrl.SetItemText(select, 2, str);
	if (m_ComboUpDown.GetCurSel() == 0)
	{
		m_ListCtrl.SetItemText(select, 3, _T("上行"));
	}
	else
		m_ListCtrl.SetItemText(select, 3, _T("下行"));
}


void CPointList::OnBnClickedBtnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int select = m_ListCtrl.GetSelectionMark();
	if (select < 0)
	{
		return;
	}
	m_ListCtrl.DeleteItem(select);
	GetDlgItem(IDC_EDIT_KM)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_LAT)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_LON)->SetWindowText(_T(""));
	m_ComboUpDown.SetCurSel(-1);
}
