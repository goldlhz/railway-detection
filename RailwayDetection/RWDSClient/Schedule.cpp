// Schedule.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "Schedule.h"
#include "afxdialogex.h"
#include "DataService.h"
#include "CmdDefine.h"


// CSchedule 对话框

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
    DDX_Control(pDX, IDC_LIST_SELECTEDSTAFF, m_ListStaffSelected);
    DDX_Control(pDX, IDC_LIST_UNSELECTSTAFF, m_ListStaffUnselected);
}


BEGIN_MESSAGE_MAP(CSchedule, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SCHEDULELIST, &CSchedule::OnLvnItemchangedSchedulelist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTARRIVETTIME, &CSchedule::OnLvnItemchangedListarrivettime)
	ON_BN_CLICKED(IDC_BTN_MODIFYTIME, &CSchedule::OnBnClickedBtnModifytime)
	ON_BN_CLICKED(IDC_BTN_MODIFYCALENDER, &CSchedule::OnBnClickedBtnModifycalender)
    //ON_BN_CLICKED(IDC_BTN_CLEANSTAFF, &CSchedule::OnBnClickedBtnCleanstaff)
    ON_BN_CLICKED(IDC_BTN_ADDLISTSTAFF, &CSchedule::OnBnClickedBtnAddliststaff)
    ON_BN_CLICKED(IDC_BTN_REMOVELISTSTAFF, &CSchedule::OnBnClickedBtnRemoveliststaff)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CSchedule::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CSchedule::OnBnClickedBtnSave)
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
	m_ListCtrl.InsertColumn(1, _T("线路名"), LVCFMT_LEFT, clientRect.Width()/4);
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

	LineInfo* line = NULL;
	int count = m_CRWDSClientView->m_CurrentOrg->iLine.size();
	for (int i=0; i<count; i++)
	{
		line = m_CRWDSClientView->m_CurrentOrg->iLine[i];
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

	//初始化日程信息
	m_ComboStartDay.ResetContent();
	for (int i=0; i<strStartNoCount; i++)
	{
		m_ComboStartDay.AddString(strStartNo[i]);
	}
	CTime startTime(m_CRWDSClientView->m_CurrentOrg->iCalendar->iStartDay);
	int i = startTime.GetMonth();
	int ii = startTime.GetDay();
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STARTDAY))->SetTime(&startTime);

    AddStaffToListBox();
	GetDlgItem(IDC_EDIT_SCHEDULEREMARK)->SetWindowText(m_CRWDSClientView->m_CurrentOrg->iCalendar->iScheduleRemark);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSchedule::AddStaffToListBox()
{//加载排班员工至listbox
    StaffInfo* staffSeleted;
    StaffInfo* staffUnseleted;

    for (size_t i=0; i<m_CRWDSClientView->m_CurrentOrg->iCalendar->iScheduleStaff.size(); i++)
    {
        staffSeleted = m_CRWDSClientView->m_CurrentOrg->iCalendar->iScheduleStaff[i];
        m_StaffSeleted.push_back(staffSeleted);
        m_ListStaffSelected.AddString(staffSeleted->iName);
    }

    BOOL addStaff = TRUE;
    for (size_t i=0; i<m_CRWDSClientView->m_CurrentOrg->iStaff.size(); i++)
    {
        addStaff = TRUE;
        staffUnseleted = m_CRWDSClientView->m_CurrentOrg->iStaff[i];
        for (size_t j=0; j<m_CRWDSClientView->m_CurrentOrg->iCalendar->iScheduleStaff.size(); j++)
        {//添加已选员工
            staffSeleted = m_CRWDSClientView->m_CurrentOrg->iCalendar->iScheduleStaff[j];
            if (staffSeleted == staffUnseleted)
            {//该员工已选择该线路
                addStaff = FALSE;
                break;
            }
        }
        if (addStaff)
        {
            m_StaffUnseleted.push_back(staffUnseleted);
            m_ListStaffUnselected.AddString(staffUnseleted->iName);
        }
    }
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
	LineInfo* line = m_CRWDSClientView->m_CurrentOrg->iLine[select];
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

	//处理日程表
	
	CTime startTime(m_CRWDSClientView->m_CurrentOrg->iCalendar->iStartDay);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STARTDAY))->SetTime(&startTime);
	CString str;
	str.Format(_T("%d"), m_CRWDSClientView->m_CurrentOrg->iCalendar->iPeriods);
	GetDlgItem(IDC_EDIT_PERIODS)->SetWindowText(str);
	//按周期显示天数
	m_ComboStartDay.ResetContent();
    for (int i=0; i<strStartNoCount; i++)
	{
		m_ComboStartDay.AddString(strStartNo[i]);
	}
	int curSel = static_cast<int>(line->iStartNo);
	if (line->iStartNo >= m_ComboStartDay.GetCount())
	{
		curSel = -1;
	}
	m_ComboStartDay.SetCurSel(curSel);
	
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
	CTime t1(m_SelectedLine->iLineKmTime[select]);
	CString str;
	str.Format(_T("%02d"), t1.GetHour());
	GetDlgItem(IDC_EDIT_HOUR)->SetWindowText(str);
	str.Format(_T("%02d"), t1.GetMinute());
	GetDlgItem(IDC_EDIT_MINUTE)->SetWindowText(str);
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
	struct tm schTime;
    localtime_s(&schTime, &m_SelectedLine->iLineKmTime[select]);
	GetDlgItem(IDC_EDIT_HOUR)->GetWindowText(strHour);
	schTime.tm_hour = _ttoi(strHour);
	GetDlgItem(IDC_EDIT_MINUTE)->GetWindowText(strMin);
	schTime.tm_min = _ttoi(strMin);
	m_SelectedLine->iLineKmTime[select] = mktime(&schTime);
	AfxMessageBox(_T("修改成功"));
	CString str;
	str.Format(_T("%02d:%02d"), schTime.tm_hour, schTime.tm_min);
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

	//保存修改过到达时间
	BOOL pushback = TRUE;
	for (size_t i=0; i<m_ModifyLineArrvieTime.size(); i++)
	{
		if (m_SelectedLine == m_ModifyLineArrvieTime[i])
		{
			pushback = FALSE;
			break;
		}
	}
	if (pushback)
	{
		m_ModifyLineArrvieTime.push_back(m_SelectedLine);
		//m_ModifyArrvieTime = TRUE;
	}
	
}

void CSchedule::OnBnClickedBtnModifycalender()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime startTime;

	CString str;
	GetDlgItem(IDC_EDIT_PERIODS)->GetWindowText(str);
	int periods = _ttoi(str);
	if (periods<1)
	{
		AfxMessageBox(_T("排班周期必须要大于或者等于1"));
		return;
	}
	if (m_ComboStartDay.GetCurSel() < 0)
	{
		AfxMessageBox(_T("请选择开始日期"));
		return;
	}
    //排班开始时间
	m_CRWDSClientView->m_CurrentOrg->iCalendar->iPeriods = _ttoi(str);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STARTDAY))->GetTime(startTime);
	m_CRWDSClientView->m_CurrentOrg->iCalendar->iStartDay = startTime.GetTime();

	m_SelectedLine->iStartNo = static_cast<LineStartNo>(m_ComboStartDay.GetCurSel());
	//m_ComboStartDay.ResetContent();
	//for (int i=0; i<m_CRWDSClientView->m_CurrentOrg->iCalendar->iPeriods && i<StrStartNoCount; i++)
	//{
	//	m_ComboStartDay.AddString(StrStartNo[i]);
	//}
	//if (m_SelectedLine->iStartNo >= m_ComboStartDay.GetCount())
	//{
	//	m_SelectedLine->iStartNo = KUndefine;
	//}
	//m_ComboStartDay.SetCurSel(m_SelectedLine->iStartNo);
	GetDlgItem(IDC_EDIT_SCHEDULEREMARK)->GetWindowText(m_CRWDSClientView->m_CurrentOrg->iCalendar->iScheduleRemark);
	//传排班表
    SetCalendarSchedule(m_CRWDSClientView->m_CurrentOrg->iOrgID, m_CRWDSClientView->m_CurrentOrg->iCalendar);
	
    AfxMessageBox(_T("修改成功"));
}


void CSchedule::OnBnClickedBtnAddliststaff()
{
    // TODO: 在此添加控件通知处理程序代码
    int curStaffIndex = m_ListStaffUnselected.GetCurSel();
    if (curStaffIndex<0)
    {
        return;
    }
    StaffInfo* curStaff = m_StaffUnseleted[curStaffIndex];
    //把员工添加到当前选择的路线
    //m_SelectedLine->iArrangeStaff.push_back(curStaff);
    m_CRWDSClientView->m_CurrentOrg->iCalendar->iScheduleStaff.push_back(curStaff);

    //更改状态
    m_ListStaffUnselected.DeleteString(curStaffIndex);
    m_ListStaffSelected.AddString(curStaff->iName);
    m_StaffUnseleted.erase(m_StaffUnseleted.begin()+curStaffIndex);
    m_StaffSeleted.push_back(curStaff);
    //curStaff->iArrangeLine.push_back(m_SelectedLine);
}


void CSchedule::OnBnClickedBtnRemoveliststaff()
{
    // TODO: 在此添加控件通知处理程序代码
    int curStaffIndex = m_ListStaffSelected.GetCurSel();
    if (curStaffIndex<0)
    {
        return;
    }
    StaffInfo* curStaff = m_StaffSeleted[curStaffIndex];
    //把员工移除排班表
    m_CRWDSClientView->m_CurrentOrg->iCalendar->iScheduleStaff.erase(
                        m_CRWDSClientView->m_CurrentOrg->iCalendar->iScheduleStaff.begin()+curStaffIndex);
    //更改状态
    m_ListStaffSelected.DeleteString(curStaffIndex);
    m_ListStaffUnselected.AddString(curStaff->iName);
    m_StaffSeleted.erase(m_StaffSeleted.begin()+curStaffIndex);
    m_StaffUnseleted.push_back(curStaff);
    //for (size_t i=0; i<curStaff->iArrangeLine.size(); i++)
    //{
    //    if (m_SelectedLine == curStaff->iArrangeLine[i])
    //    {
    //        curStaff->iArrangeLine.erase(curStaff->iArrangeLine.begin()+i);
    //        break;
    //    }
    //}

}



void CSchedule::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


void CSchedule::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	for( size_t i=0; i<m_ModifyLineArrvieTime.size(); i++)
	{
		SetOrgLine(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_LINE_MODIFY, m_ModifyLineArrvieTime[i]);
	}
	m_ModifyLineArrvieTime.clear();
}
