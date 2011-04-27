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
	ON_NOTIFY(NM_CLICK, IDC_POINTLIST, &CPointList::OnNMClickPointlist)
	ON_BN_CLICKED(IDC_BTN_POINTADD, &CPointList::OnBnClickedBtnPointadd)
	ON_BN_CLICKED(IDC_BTN_POINTMODIFY, &CPointList::OnBnClickedBtnPointmodify)
	ON_BN_CLICKED(IDC_BTN_POINTDELETE, &CPointList::OnBnClickedBtnPointdelete)
	ON_BN_CLICKED(IDC_BTN_POINTOK, &CPointList::OnBnClickedBtnPointok)
END_MESSAGE_MAP()

BOOL CPointList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: 在此添加控件通知处理程序代码
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CRect clientRect;
	m_ListCtrl.GetClientRect(&clientRect);
	m_ListCtrl.InsertColumn(0, _T("公里"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(1, _T("经度"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(2, _T("纬度"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(3, _T("上下行"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ComboUpDown.ResetContent();
	m_ComboUpDown.AddString(_T("上行"));
	m_ComboUpDown.AddString(_T("下行"));

	int count = m_Point.size();
	CString km;
	CString lon;
	CString lat;
	CString direct;

	for (int i=0; i<count; i++)
	{
		km.Format(_T("%f"), m_Point[i].iKM);
		lon.Format(_T("%f"), m_Point[i].iLon);
		lat.Format(_T("%f"), m_Point[i].iLat);
		if (m_Point[i].iDirect == KUpLine)
			direct = _T("上行");
		else
			direct = _T("下行");

		m_ListCtrl.InsertItem(i, km);
		m_ListCtrl.SetItemText(i, 1, lon);
		m_ListCtrl.SetItemText(i, 2, lat);
		m_ListCtrl.SetItemText(i, 3, direct);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CPointList::OnNMClickPointlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int select = m_ListCtrl.GetSelectionMark();
	GetDlgItem(IDC_EDIT_KM)->SetWindowText(m_ListCtrl.GetItemText(select, 0));
	GetDlgItem(IDC_EDIT_LON)->SetWindowText(m_ListCtrl.GetItemText(select, 1));
	GetDlgItem(IDC_EDIT_LAT)->SetWindowText(m_ListCtrl.GetItemText(select, 2));
	if (m_ListCtrl.GetItemText(select, 3).Compare(_T("上行")) == 0)
	{
		m_ComboUpDown.SetCurSel(0);
	}
	else
		m_ComboUpDown.SetCurSel(1);
}

void CPointList::OnBnClickedBtnPointadd()
{
	// TODO: 在此添加控件通知处理程序代码
	int itemCount = m_ListCtrl.GetItemCount();
	m_ListCtrl.InsertItem(itemCount, _T("100"));
	m_ListCtrl.SetItemText(itemCount, 1, _T("100"));
	m_ListCtrl.SetItemText(itemCount, 2, _T("100"));
	m_ListCtrl.SetItemText(itemCount, 3, _T("上行"));
}

void CPointList::OnBnClickedBtnPointmodify()
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
	GetDlgItem(IDC_EDIT_LON)->GetWindowText(str);
	m_ListCtrl.SetItemText(select, 1, str);
	GetDlgItem(IDC_EDIT_LAT)->GetWindowText(str);
	m_ListCtrl.SetItemText(select, 2, str);
	if (m_ComboUpDown.GetCurSel() == 0)
		m_ListCtrl.SetItemText(select, 3, _T("上行"));
	else
		m_ListCtrl.SetItemText(select, 3, _T("下行"));
}

void CPointList::OnBnClickedBtnPointdelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int select = m_ListCtrl.GetSelectionMark();
	if (select < 0)
	{
		return;
	}
	m_ListCtrl.DeleteItem(select);
	GetDlgItem(IDC_EDIT_KM)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_LON)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_LAT)->SetWindowText(_T(""));
	m_ComboUpDown.SetCurSel(-1);
}

void CPointList::OnBnClickedBtnPointok()
{
	// TODO: 在此添加控件通知处理程序代码
	int listCount = m_ListCtrl.GetItemCount();
	m_Point.clear();
	CString km;
	CString lon;
	CString lat;
	CString direct;
	for (int i=0; i<listCount; i++)
	{
		MapPoint point;
		km = m_ListCtrl.GetItemText(i, 0);
		lon = m_ListCtrl.GetItemText(i, 1);
		lat = m_ListCtrl.GetItemText(i, 2);
		direct = m_ListCtrl.GetItemText(i, 3);
		point.iKM = _ttoi(km);
		point.iLon = _tstof(lon);
		point.iLat = _tstof(lat);
		//point.iLat = _ttoi(lat);
		//point.iLat = atof((LPSTR)(LPCTSTR)lat.GetBuffer(lat.GetLength()));

		if (direct.Compare(_T("上行")) == 0)
			point.iDirect = KUpLine;
		else
			point.iDirect = KDownLine;
		m_Point.push_back(point);
	}
	OnOK();
}

vector<MapPoint> CPointList::GetPoint()
{
	return m_Point;
}

void CPointList::SetPoint( vector<MapPoint> aPoint )
{
	m_Point = aPoint;
}