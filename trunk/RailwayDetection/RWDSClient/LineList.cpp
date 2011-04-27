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

}

CLineList::~CLineList()
{
}

void CLineList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LINELIST, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CLineList, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LINEADD, &CLineList::OnBnClickedBtnLineadd)
	ON_BN_CLICKED(IDC_BTN_LINEMODIFY, &CLineList::OnClickedBtnLinemodify)
	ON_BN_CLICKED(IDC_BTN_LINEDELETE, &CLineList::OnClickedBtnLinedelete)
	ON_BN_CLICKED(IDC_BTN_LINEOK, &CLineList::OnClickedBtnLineok)
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
	//m_ListCtrl.InsertColumn(3, _T("终端"), LVCFMT_LEFT, clientRect.Width()/5);

	int count = m_Line.size();
	CString id;
	CString name;
	CString startKm;
	CString endKm;

	for (int i=0; i<count; i++)
	{
		id.Format(_T("%d"), m_Line[i].iLineID);
		startKm.Format(_T("%.0f"), m_Line[i].iStartKm);
		endKm.Format(_T("%.0f"), m_Line[i].iDownlineKmLonLat[m_Line[i].iDownlineKmLonLat.size()-1]);
		name = m_Line[i].iLineName;

		m_ListCtrl.InsertItem(i, id);
		m_ListCtrl.SetItemText(i, 1, name);
		m_ListCtrl.SetItemText(i, 2, startKm);
		m_ListCtrl.SetItemText(i, 3, endKm);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLineList::SetLine( vector<LineInfo> aLine )
{
	m_Line = aLine;
}

vector<LineInfo> CLineList::GetLine()
{
	return m_Line;
}


void CLineList::OnBnClickedBtnLineadd()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CLineList::OnClickedBtnLinemodify()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CLineList::OnClickedBtnLinedelete()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CLineList::OnClickedBtnLineok()
{
	// TODO: 在此添加控件通知处理程序代码
}
