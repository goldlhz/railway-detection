// Schedule.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "Schedule.h"
#include "afxdialogex.h"


// CSchedule �Ի���

IMPLEMENT_DYNAMIC(CSchedule, CDialogEx)

CSchedule::CSchedule(CWnd* pParent)
	: CDialogEx(CSchedule::IDD, pParent)
{
	m_CRWDSClientView = static_cast<CRWDSClientView*>(pParent);
}

CSchedule::~CSchedule()
{
}

void CSchedule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCHEDULELIST, m_ListCtrl);
	DDX_Control(pDX, IDC_LISTARRIVETTIME, m_ListArriveTime);
	DDX_Control(pDX, IDC_COMBO_STARTDAY, m_ComboStartDay);
}


BEGIN_MESSAGE_MAP(CSchedule, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SCHEDULELIST, &CSchedule::OnLvnItemchangedSchedulelist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTARRIVETTIME, &CSchedule::OnLvnItemchangedListarrivettime)
	ON_BN_CLICKED(IDC_BTN_MODIFYTIME, &CSchedule::OnBnClickedBtnModifytime)
	ON_BN_CLICKED(IDC_BTN_MODIFYCALENDER, &CSchedule::OnBnClickedBtnModifycalender)
END_MESSAGE_MAP()


// CSchedule ��Ϣ�������


BOOL CSchedule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ListArriveTime.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	CRect clientRect;
	m_ListCtrl.GetClientRect(&clientRect);
	while(m_ListCtrl.DeleteColumn(0));

	m_ListCtrl.InsertColumn(0, _T("���"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(1, _T("��·��"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(2, _T("��ʼʱ��"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(3, _T("����ʱ��"), LVCFMT_LEFT, clientRect.Width()/4);

	m_ListArriveTime.GetClientRect(&clientRect);
	while(m_ListArriveTime.DeleteColumn(0));
	m_ListArriveTime.InsertColumn(0, _T("���ﴦ"), LVCFMT_LEFT, clientRect.Width()*3/5);
	m_ListArriveTime.InsertColumn(1, _T("����ʱ��"), LVCFMT_LEFT, clientRect.Width()*2/5);

	CString id;
	CString schName;
	CString lineName;
	CString dev;
	CString wk;
	CString beTime;
	CString endTime;

	LineInfo* line = NULL;
	int count = m_CRWDSClientView->m_Line.size();
	for (int i=0; i<count; i++)
	{
		line = m_CRWDSClientView->m_Line[i];
		id.Format(_T("%d"), line->iLineID);
		lineName = line->iLineName;

		if(line->iLineKmTime.size()>0)
		{
			CTime tb(line->iLineKmTime[0]);
			CTime te(line->iLineKmTime[line->iLineKmTime.size()-1]);
			beTime.Format(_T("%02d:%02d"), tb.GetHour(), tb.GetMinute());
			endTime.Format(_T("%02d:%02d"), te.GetHour(), te.GetMinute());
		}
		else
		{
			beTime = _T("");
			endTime = _T("");
		}

		m_ListCtrl.InsertItem(i, id);
		m_ListCtrl.SetItemText(i, 1, lineName);
		m_ListCtrl.SetItemText(i, 2, beTime);
		m_ListCtrl.SetItemText(i, 3, endTime);
	}

	//��ʼ���ճ���Ϣ
	m_ComboStartDay.ResetContent();
	for (int i=0; i<StrStartNoCount; i++)
	{
		m_ComboStartDay.AddString(StrStartNo[i]);
	}
	CTime startTime(m_CRWDSClientView->m_Calendar->iStartDay);
	int i = startTime.GetMonth();
	int ii = startTime.GetDay();
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STARTDAY))->SetTime(&startTime);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSchedule::OnLvnItemchangedSchedulelist(NMHDR *pNMHDR, LRESULT *pResult)
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
	LineInfo* line = m_CRWDSClientView->m_Line[select];
	CString km;
	CString at;
	m_ListArriveTime.DeleteAllItems();
	for (size_t i=0; i<line->iLineKmLonLat.size(); i++)
	{
		ENCODERAILWAYFULLNAME(km, line->iLineKmLonLat[i]->iRailLine, 
			line->iLineKmLonLat[i]->iKM, line->iLineKmLonLat[i]->iDirect);
		if (i < line->iLineKmTime.size())
		{
			CTime t1(line->iLineKmTime[i]);
			at.Format(_T("%02d:%02d"), t1.GetHour(), t1.GetMinute());
		}
		else
		{
			at = _T("00:00");
		}
		m_ListArriveTime.InsertItem(i, km);
		m_ListArriveTime.SetItemText(i, 1, at);
	}
	m_SelectedLine = line;

	//�����ճ̱�
	m_ComboStartDay.SetCurSel(line->iStartNo);
	CTime startTime(m_CRWDSClientView->m_Calendar->iStartDay);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STARTDAY))->SetTime(&startTime);
	CString str;
	str.Format(_T("%d"), m_CRWDSClientView->m_Calendar->iPeriods);
	GetDlgItem(IDC_EDIT_PERIODS)->SetWindowText(str);
	*pResult = 0;
}


void CSchedule::OnLvnItemchangedListarrivettime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos;
	pos = m_ListArriveTime.GetFirstSelectedItemPosition();
	int select = m_ListArriveTime.GetNextSelectedItem(pos);  
	if (select<0)
	{
		return;
	}
	CTime t1(m_SelectedLine->iLineKmTime[select]);
	CString str;
	str.Format(_T("%02d"), t1.GetHour());
	GetDlgItem(IDC_EDIT_HOUR)->SetWindowTextW(str);
	str.Format(_T("%02d"), t1.GetMinute());
	GetDlgItem(IDC_EDIT_MINUTE)->SetWindowTextW(str);
	*pResult = 0;
}


void CSchedule::OnBnClickedBtnModifytime()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos;
	pos = m_ListArriveTime.GetFirstSelectedItemPosition();
	int select = m_ListArriveTime.GetNextSelectedItem(pos);  
	if (select<0)
	{
		return;
	}
	//CTime t1(m_SelectedSchedule->iULineKmTime[select]);
	CString strHour;
	CString strMin;
	struct tm* schTime = NULL;
	localtime_s(schTime, &m_SelectedLine->iLineKmTime[select]);
	GetDlgItem(IDC_EDIT_HOUR)->GetWindowTextW(strHour);
	schTime->tm_hour = _ttoi(strHour);
	GetDlgItem(IDC_EDIT_MINUTE)->GetWindowTextW(strMin);
	schTime->tm_min = _ttoi(strMin);
	m_SelectedLine->iLineKmTime[select] = mktime(schTime);
	AfxMessageBox(_T("�޸ĳɹ�"));
	CString str;
	str.Format(_T("%02d:%02d"), schTime->tm_hour, schTime->tm_min);
	m_ListArriveTime.SetItemText(select, 1, str);
	
	POSITION posCtrl;
	posCtrl = m_ListCtrl.GetFirstSelectedItemPosition();
	int ctrlIndex = m_ListCtrl.GetNextSelectedItem(posCtrl);  
	if (ctrlIndex < 0)
	{
		return;
	}
	if (select == 0)
	{
		m_ListCtrl.SetItemText(ctrlIndex, 2, str);
	}
	else if (select == m_ListArriveTime.GetItemCount()-1)
	{
		m_ListCtrl.SetItemText(ctrlIndex, 3, str);
	}
}

void CSchedule::OnBnClickedBtnModifycalender()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CTime startTime;
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STARTDAY))->GetTime(startTime);
	m_CRWDSClientView->m_Calendar->iStartDay = startTime.GetTime();
	CString str;
	GetDlgItem(IDC_EDIT_PERIODS)->GetWindowText(str);
	m_CRWDSClientView->m_Calendar->iPeriods = _ttoi(str);
	m_SelectedLine->iStartNo = static_cast<LineStartNo>(m_ComboStartDay.GetCurSel());

}
