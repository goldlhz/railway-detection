// Schedule.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "Schedule.h"
#include "afxdialogex.h"


// CSchedule 对话框

IMPLEMENT_DYNAMIC(CSchedule, CDialogEx)

CSchedule::CSchedule(CWnd* pParent /*=NULL*/)
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
}


BEGIN_MESSAGE_MAP(CSchedule, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SCHEDULELIST, &CSchedule::OnLvnItemchangedSchedulelist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTARRIVETTIME, &CSchedule::OnLvnItemchangedListarrivettime)
	ON_BN_CLICKED(IDC_BTN_MODIFYTIME, &CSchedule::OnBnClickedBtnModifytime)
END_MESSAGE_MAP()


// CSchedule 消息处理程序


BOOL CSchedule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ListArriveTime.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	CRect clientRect;
	m_ListCtrl.GetClientRect(&clientRect);
	while(m_ListCtrl.DeleteColumn(0));

	m_ListCtrl.InsertColumn(0, _T("编号"), LVCFMT_LEFT, clientRect.Width()/4);
	//m_ListCtrl.InsertColumn(1, _T("排班名"), LVCFMT_LEFT, clientRect.Width()/7);
	m_ListCtrl.InsertColumn(1, _T("线路名"), LVCFMT_LEFT, clientRect.Width()/4);
	//m_ListCtrl.InsertColumn(3, _T("设备"), LVCFMT_LEFT, clientRect.Width()/7);
	//m_ListCtrl.InsertColumn(4, _T("员工"), LVCFMT_LEFT, clientRect.Width()/7);
	m_ListCtrl.InsertColumn(2, _T("开始时间"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(3, _T("结束时间"), LVCFMT_LEFT, clientRect.Width()/4);

	m_ListArriveTime.GetClientRect(&clientRect);
	while(m_ListArriveTime.DeleteColumn(0));
	m_ListArriveTime.InsertColumn(0, _T("公里处"), LVCFMT_LEFT, clientRect.Width()*3/5);
	m_ListArriveTime.InsertColumn(1, _T("到达时间"), LVCFMT_LEFT, clientRect.Width()*2/5);

	CString id;
	CString schName;
	CString lineName;
	CString dev;
	CString wk;
	CString beTime;
	CString endTime;
	ScheduleLine* sch = NULL;
	int count = m_CRWDSClientView->m_Schedule.size();
	for (int i=0; i<count; i++)
	{
		sch = m_CRWDSClientView->m_Schedule[i];
		id.Format(_T("%d"), sch->iScheduleID);
		//schName = sch->iScheduleName;
		lineName = sch->iLine->iLineName;
		//dev.Format(_T("%d"), sch->iDevice->iDevID);
		//wk = sch->iWorker->iName;
		CTime tb(sch->iULineKmTime[0]);
		CTime te(sch->iULineKmTime[sch->iULineKmTime.size()-1]);
		beTime.Format(_T("%02d:%02d"), tb.GetHour(), tb.GetMinute());
		endTime.Format(_T("%02d:%02d"), te.GetHour(), te.GetMinute());

		m_ListCtrl.InsertItem(i, id);
		//m_ListCtrl.SetItemText(i, 1, schName);
		m_ListCtrl.SetItemText(i, 1, lineName);
		//m_ListCtrl.SetItemText(i, 3, dev);
		//m_ListCtrl.SetItemText(i, 4, wk);
		m_ListCtrl.SetItemText(i, 2, beTime);
		m_ListCtrl.SetItemText(i, 3, endTime);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSchedule::OnLvnItemchangedSchedulelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos;
	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int select = m_ListCtrl.GetNextSelectedItem(pos);  
	if (select<0)
	{
		return;
	}
	ScheduleLine* sch = m_CRWDSClientView->m_Schedule[select];
	CString km;
	CString at;
	m_ListArriveTime.DeleteAllItems();
	for (size_t i=0; i<sch->iLine->iLineKmLonLat.size(); i++)
	{
		ENCODERAILWAYFULLNAME(km, sch->iLine->iLineKmLonLat[i]->iRailLine, 
			sch->iLine->iLineKmLonLat[i]->iKM, sch->iLine->iLineKmLonLat[i]->iDirect);
		if (i < sch->iULineKmTime.size())
		{
			CTime t1(sch->iULineKmTime[i]);
			at.Format(_T("%02d:%02d"), t1.GetHour(), t1.GetMinute());
		}
		else
		{
			at = _T("00:00");
		}
		m_ListArriveTime.InsertItem(i, km);
		m_ListArriveTime.SetItemText(i, 1, at);
	}
	m_SelectedSchedule = sch;
	*pResult = 0;
}


void CSchedule::OnLvnItemchangedListarrivettime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos;
	pos = m_ListArriveTime.GetFirstSelectedItemPosition();
	int select = m_ListArriveTime.GetNextSelectedItem(pos);  
	if (select<0)
	{
		return;
	}
	CTime t1(m_SelectedSchedule->iULineKmTime[select]);
	CString str;
	str.Format(_T("%02d"), t1.GetHour());
	GetDlgItem(IDC_EDIT_HOUR)->SetWindowTextW(str);
	str.Format(_T("%02d"), t1.GetMinute());
	GetDlgItem(IDC_EDIT_MINUTE)->SetWindowTextW(str);
	*pResult = 0;
}


void CSchedule::OnBnClickedBtnModifytime()
{
	// TODO: 在此添加控件通知处理程序代码
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
	struct tm* schTime = localtime(&m_SelectedSchedule->iULineKmTime[select]);
	GetDlgItem(IDC_EDIT_HOUR)->GetWindowTextW(strHour);
	schTime->tm_hour = _ttoi(strHour);
	GetDlgItem(IDC_EDIT_MINUTE)->GetWindowTextW(strMin);
	schTime->tm_min = _ttoi(strMin);
	m_SelectedSchedule->iULineKmTime[select] = mktime(schTime);
	AfxMessageBox(_T("修改成功"));
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
