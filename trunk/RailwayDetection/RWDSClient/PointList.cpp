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
	m_Modifying = FALSE;
	m_CRWDSClientView = static_cast<CRWDSClientView*>(pParent);
}

CPointList::~CPointList()
{
}

void CPointList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POINTLIST, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO_UPDOWN, m_ComboUpDown);
	DDX_Control(pDX, IDC_COMBO_RAILLINE, m_ComboRailLine);
}


BEGIN_MESSAGE_MAP(CPointList, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_POINTADD, &CPointList::OnBnClickedBtnPointadd)
	ON_BN_CLICKED(IDC_BTN_POINTMODIFY, &CPointList::OnBnClickedBtnPointmodify)
	ON_BN_CLICKED(IDC_BTN_POINTDELETE, &CPointList::OnBnClickedBtnPointdelete)
	ON_BN_CLICKED(IDC_BTN_POINTOK, &CPointList::OnBnClickedBtnPointok)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_POINTLIST, &CPointList::OnLvnItemchangedPointlist)
END_MESSAGE_MAP()

BOOL CPointList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: 在此添加控件通知处理程序代码
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	
	CRect clientRect;
	m_ListCtrl.GetClientRect(&clientRect);
	while(m_ListCtrl.DeleteColumn(0));

	m_ListCtrl.InsertColumn(0, _T("线路"), LVCFMT_LEFT, clientRect.Width()/5);
	m_ListCtrl.InsertColumn(1, _T("公里"), LVCFMT_LEFT, clientRect.Width()/5);
	m_ListCtrl.InsertColumn(2, _T("经度"), LVCFMT_LEFT, clientRect.Width()/5);
	m_ListCtrl.InsertColumn(3, _T("纬度"), LVCFMT_LEFT, clientRect.Width()/5);
	m_ListCtrl.InsertColumn(4, _T("上下行"), LVCFMT_LEFT, clientRect.Width()/5);
	m_ComboUpDown.ResetContent();
	m_ComboUpDown.AddString(_T("上行"));
	m_ComboUpDown.AddString(_T("下行"));

	m_ComboRailLine.ResetContent();
	for (int i=0; i<RailLineName->GetLength(); i++)
	{
		m_ComboRailLine.AddString(RailLineName[i]);
	}

	int count = m_CRWDSClientView->m_MapPoint.size();
	CString km;
	CString lon;
	CString lat;
	CString direct;

	for (int i=0; i<count; i++)
	{
		km.Format(_T("%f"), m_CRWDSClientView->m_MapPoint[i]->iKM);
		lon.Format(_T("%f"), m_CRWDSClientView->m_MapPoint[i]->iLon);
		lat.Format(_T("%f"), m_CRWDSClientView->m_MapPoint[i]->iLat);
		if (m_CRWDSClientView->m_MapPoint[i]->iDirect == KUpLine)
			direct = _T("上行");
		else
			direct = _T("下行");

		m_ListCtrl.InsertItem(i, RailLineName[m_CRWDSClientView->m_MapPoint[i]->iRailLine]);
		m_ListCtrl.SetItemText(i, 1, km);
		m_ListCtrl.SetItemText(i, 2, lon);
		m_ListCtrl.SetItemText(i, 3, lat);
		m_ListCtrl.SetItemText(i, 4, direct);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CPointList::OnBnClickedBtnPointadd()
{
	// TODO: 在此添加控件通知处理程序代码
	MapPoint* point = new MapPoint;
	point->iRailLine = Baoji_Chengdu;
	point->iKM = 240;
	point->iLon = 100;
	point->iLat = 30;
	point->iDirect = KDownLine;
	m_CRWDSClientView->m_MapPoint.push_back(point);

	CString km;
	CString lon;
	CString lat;
	CString direct;
	int itemCount = m_ListCtrl.GetItemCount();
	km.Format(_T("%f"), point->iKM);
	lon.Format(_T("%f"), point->iLon);
	lat.Format(_T("%f"), point->iLat);
	if (point->iDirect == KUpLine)
		direct = _T("上行");
	else
		direct = _T("下行");

	m_ListCtrl.InsertItem(itemCount, RailLineName[point->iRailLine]);
	m_ListCtrl.SetItemText(itemCount, 1, km);
	m_ListCtrl.SetItemText(itemCount, 2, lon);
	m_ListCtrl.SetItemText(itemCount, 3, lat);
	m_ListCtrl.SetItemText(itemCount, 4, direct);


}

void CPointList::OnBnClickedBtnPointmodify()
{
	// TODO: 在此添加控件通知处理程序代码
	int select = m_ListCtrl.GetSelectionMark();
	if (select < 0)
	{
		return;
	}
	m_Modifying = TRUE;
	MapPoint* point = m_CRWDSClientView->m_MapPoint[select];
	CString str;
	m_ListCtrl.SetItemText(select, 0, RailLineName[m_ComboRailLine.GetCurSel()]);
	point->iRailLine = static_cast<RailLine>(m_ComboRailLine.GetCurSel());

	GetDlgItem(IDC_EDIT_KM)->GetWindowText(str);
	m_ListCtrl.SetItemText(select, 1, str);
	point->iKM = _ttoi(str);
	GetDlgItem(IDC_EDIT_LON)->GetWindowText(str);
	m_ListCtrl.SetItemText(select, 2, str);
	point->iLon = _tstof(str);
	GetDlgItem(IDC_EDIT_LAT)->GetWindowText(str);
	m_ListCtrl.SetItemText(select, 3, str);
	point->iLat = _tstof(str);

	if (m_ComboUpDown.GetCurSel() == 0)
	{
		m_ListCtrl.SetItemText(select, 4, _T("上行"));
		point->iDirect = KUpLine;
	}
	else
	{
		m_ListCtrl.SetItemText(select, 4, _T("下行"));
		point->iDirect = KDownLine;
	}
	m_Modifying = FALSE;
}

void CPointList::OnBnClickedBtnPointdelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int select = m_ListCtrl.GetSelectionMark();
	if (select < 0)
	{
		return;
	}
	MapPoint* point = m_CRWDSClientView->m_MapPoint[select];
	m_CRWDSClientView->m_MapPoint.erase(m_CRWDSClientView->m_MapPoint.begin()+select);
	delete point;

	m_ListCtrl.DeleteItem(select);
	GetDlgItem(IDC_EDIT_KM)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_LON)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_LAT)->SetWindowText(_T(""));
	m_ComboUpDown.SetCurSel(-1);
}

void CPointList::OnBnClickedBtnPointok()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CPointList::OnLvnItemchangedPointlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (m_Modifying)
		return;

	POSITION pos;
	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	// 得到项目索引
	int select = m_ListCtrl.GetNextSelectedItem(pos);  
	if (select<0)
		return;

	m_ComboRailLine.SetCurSel(m_CRWDSClientView->m_MapPoint[select]->iRailLine);
	GetDlgItem(IDC_EDIT_KM)->SetWindowText(m_ListCtrl.GetItemText(select, 1));
	GetDlgItem(IDC_EDIT_LON)->SetWindowText(m_ListCtrl.GetItemText(select, 2));
	GetDlgItem(IDC_EDIT_LAT)->SetWindowText(m_ListCtrl.GetItemText(select, 3));
	if (m_ListCtrl.GetItemText(select, 4).Compare(_T("上行")) == 0)
		m_ComboUpDown.SetCurSel(0);
	else
		m_ComboUpDown.SetCurSel(1);

	*pResult = 0;
}

