// LineList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "LineList.h"
#include "afxdialogex.h"


// CLineList �Ի���

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


// CLineList ��Ϣ�������


BOOL CLineList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CRect clientRect;
	m_ListCtrl.GetClientRect(&clientRect);
	m_ListCtrl.InsertColumn(0, _T("·�߱��"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(1, _T("·����"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(2, _T("��ʼ(KM)"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(3, _T("����(KM)"), LVCFMT_LEFT, clientRect.Width()/4);
	//m_ListCtrl.InsertColumn(3, _T("�ն�"), LVCFMT_LEFT, clientRect.Width()/5);

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
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CLineList::OnClickedBtnLinemodify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CLineList::OnClickedBtnLinedelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CLineList::OnClickedBtnLineok()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
