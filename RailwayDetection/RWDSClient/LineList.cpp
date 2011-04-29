// LineList.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "LineList.h"
#include "afxdialogex.h"


// CLineList 对话框

IMPLEMENT_DYNAMIC(CLineList, CDialogEx)

CLineList::CLineList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLineList::IDD, pParent)
{
	m_CRWDSClientView = static_cast<CRWDSClientView*>(pParent);
}

CLineList::~CLineList()
{
}

void CLineList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LINELIST, m_ListCtrl);
	DDX_Control(pDX, IDC_LIST_ALLPOINT, m_ListAllPoint);
	DDX_Control(pDX, IDC_LIST_SELETED, m_ListSelectedPoint);
}


BEGIN_MESSAGE_MAP(CLineList, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LINEADD, &CLineList::OnBnClickedBtnLineadd)
	ON_BN_CLICKED(IDC_BTN_LINEMODIFY, &CLineList::OnClickedBtnLinemodify)
	ON_BN_CLICKED(IDC_BTN_LINEDELETE, &CLineList::OnClickedBtnLinedelete)
	ON_BN_CLICKED(IDC_BTN_LINEOK, &CLineList::OnClickedBtnLineok)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LINELIST, &CLineList::OnLvnItemchangedLinelist)
END_MESSAGE_MAP()


// CLineList 消息处理程序


BOOL CLineList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CRect clientRect;
	m_ListCtrl.GetClientRect(&clientRect);
	m_ListCtrl.InsertColumn(0, _T("路线编号"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(1, _T("路线名"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(2, _T("开始(KM)"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(3, _T("结束(KM)"), LVCFMT_LEFT, clientRect.Width()/4);
	//m_ListCtrl.InsertColumn(4, _T("终端"), LVCFMT_LEFT, clientRect.Width()/5);

	int count = m_CRWDSClientView->m_Line.size();
	CString id;
	CString name;
	CString startKm;
	CString endKm;

	for (int i=0; i<count; i++)
	{
		id.Format(_T("%d"), m_CRWDSClientView->m_Line[i]->iLineID);
		startKm.Format(_T("%.0f"), m_CRWDSClientView->m_Line[i]->iStartKm);
		endKm.Format(_T("%.0f"), m_CRWDSClientView->m_Line[i]->iLineKmLonLat[m_CRWDSClientView->m_Line[i]->iLineKmLonLat.size()-1]->iKM);
		name = m_CRWDSClientView->m_Line[i]->iLineName;

		m_ListCtrl.InsertItem(i, id);
		m_ListCtrl.SetItemText(i, 1, name);
		m_ListCtrl.SetItemText(i, 2, startKm);
		m_ListCtrl.SetItemText(i, 3, endKm);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLineList::StrLineKmToLineKm( CString aStrLine, RailLine& aRailLine, double& aKm )
{//宝成线100公里转换成RailLine与Km格式
	aStrLine.Delete(aStrLine.GetLength()-StrKm.GetLength(), StrKm.GetLength());
	for (int i=0; i<RailLineName->GetLength(); i++)
	{
		if(aStrLine.Find(RailLineName[i]) == 0)
		{
			aRailLine = static_cast<RailLine>(i);
			aStrLine.Delete(0, RailLineName[i].GetLength());
			aKm = _ttoi(aStrLine);
			break;
		}
	}
}

void CLineList::OnBnClickedBtnLineadd()
{
	// TODO: 在此添加控件通知处理程序代码
	LineInfo* line = new LineInfo;
	line->iLineID = 1;
	line->iLineName = _T("线路");
	line->iStartKm = m_CRWDSClientView->m_MapPoint[0]->iKM;
	line->iLineKmLonLat.push_back(m_CRWDSClientView->m_MapPoint[0]);
	line->iLineKmLonLat.push_back(m_CRWDSClientView->m_MapPoint[0]);
	m_CRWDSClientView->m_Line.push_back(line);

	CString id;
	CString name;
	CString startKm;
	CString endKm;
	int count = m_ListCtrl.GetItemCount();

	id.Format(_T("%d"), line->iLineID);
	startKm.Format(_T("%.0f"), line->iStartKm);
	endKm.Format(_T("%.0f"), line->iLineKmLonLat[line->iLineKmLonLat.size()-1]->iKM);
	name = line->iLineName;

	m_ListCtrl.InsertItem(count, id);
	m_ListCtrl.SetItemText(count, 1, name);
	m_ListCtrl.SetItemText(count, 2, startKm);
	m_ListCtrl.SetItemText(count, 3, endKm);
}

void CLineList::OnClickedBtnLinemodify()
{
	// TODO: 在此添加控件通知处理程序代码
	int select = m_ListCtrl.GetSelectionMark();
}

void CLineList::OnClickedBtnLinedelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int select = m_ListCtrl.GetSelectionMark();
	if (select < 0)
		return;
	LineInfo* line = m_CRWDSClientView->m_Line[select];
	m_CRWDSClientView->m_Line.erase(m_CRWDSClientView->m_Line.begin()+select);
	delete line;

	m_ListCtrl.DeleteItem(select);
	m_ListAllPoint.ResetContent();
	m_ListSelectedPoint.ResetContent();
}

void CLineList::OnClickedBtnLineok()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CLineList::OnLvnItemchangedLinelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos;
	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	// 得到项目索引
	int select = m_ListCtrl.GetNextSelectedItem(pos);  
	if (select<0)
	{
		return;
	}
	CString str;
	m_ListAllPoint.ResetContent();
	m_ListSelectedPoint.ResetContent();
	for (size_t i=0; i<m_CRWDSClientView->m_MapPoint.size(); i++)
	{
		bool addSelect = FALSE;
		for (size_t j=0; j<m_CRWDSClientView->m_Line[select]->iLineKmLonLat.size(); j++)
		{//找到相同的点
			if (m_CRWDSClientView->m_MapPoint[i] == m_CRWDSClientView->m_Line[select]->iLineKmLonLat[j])
			{
				addSelect = TRUE;
				break;
			}
		}
		ENCODERAILWAYFULLNAME(str, m_CRWDSClientView->m_MapPoint[i]->iRailLine, m_CRWDSClientView->m_MapPoint[i]->iKM, m_CRWDSClientView->m_MapPoint[i]->iDirect);
		if (addSelect)
		{
			m_ListSelectedPoint.AddString(str);
			m_Selected.push_back(m_CRWDSClientView->m_MapPoint[i]);
		}
		else
		{
			m_ListAllPoint.AddString(str);
			m_Unselected.push_back(m_CRWDSClientView->m_MapPoint[i]);
		}
	}

	*pResult = 0;
}
