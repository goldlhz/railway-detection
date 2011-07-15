// LineList.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "LineList.h"
#include "afxdialogex.h"
#include "DataService.h"
#include "CmdDefine.h"


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
	ON_NOTIFY(NM_DBLCLK, IDC_LINELIST, &CLineList::OnNMDblclkLinelist)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LINELIST, &CLineList::OnLvnEndlabeleditLinelist)
	ON_BN_CLICKED(IDC_BTN_ADD1, &CLineList::OnBnClickedBtnAdd1)
	ON_BN_CLICKED(IDC_BTN_REMOVE1, &CLineList::OnBnClickedBtnRemove1)
END_MESSAGE_MAP()


// CLineList 消息处理程序


BOOL CLineList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CRect clientRect;
	m_ListCtrl.GetClientRect(&clientRect);
	m_ListCtrl.InsertColumn(0, _T("路线名"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(1, _T("路线编号"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(2, _T("开始(KM)"), LVCFMT_LEFT, clientRect.Width()/4);
	m_ListCtrl.InsertColumn(3, _T("结束(KM)"), LVCFMT_LEFT, clientRect.Width()/4);
	//m_ListCtrl.InsertColumn(4, _T("终端"), LVCFMT_LEFT, clientRect.Width()/5);

	int count = m_CRWDSClientView->m_CurrentOrg->iLine.size();
	CString id;
	CString name;
	CString startKm;
	CString endKm;

	for (int i=0; i<count; i++)
	{
		id.Format(_T("%d"), m_CRWDSClientView->m_CurrentOrg->iLine[i]->iLineID);
		name = m_CRWDSClientView->m_CurrentOrg->iLine[i]->iLineName;
		if (m_CRWDSClientView->m_CurrentOrg->iLine[i]->iLineKmLonLat.size() > 0)
		{
			startKm.Format(_T("%.0f"), m_CRWDSClientView->m_CurrentOrg->iLine[i]->iLineKmLonLat[0]->iKM);
			endKm.Format(_T("%.0f"), m_CRWDSClientView->m_CurrentOrg->iLine[i]->iLineKmLonLat[m_CRWDSClientView->m_CurrentOrg->iLine[i]->iLineKmLonLat.size()-1]->iKM);
		}
		else
		{
			startKm = _T("");
			endKm = _T("");
		}

		m_ListCtrl.InsertItem(i, name);
		m_ListCtrl.SetItemText(i, 1, id);
		m_ListCtrl.SetItemText(i, 2, startKm);
		m_ListCtrl.SetItemText(i, 3, endKm);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//void CLineList::StrLineKmToLineKm( CString aStrLine, RailLine& aRailLine, double& aKm )
//{//宝成线100公里转换成RailLine与Km格式
//	aStrLine.Delete(aStrLine.GetLength()-StrKm.GetLength(), StrKm.GetLength());
//	for (int i=0; i<RailLineNameCount; i++)
//	{
//		if(aStrLine.Find(RailLineName[i]) == 0)
//		{
//			aRailLine = static_cast<RailLine>(i);
//			aStrLine.Delete(0, RailLineName[i].GetLength());
//			aKm = _ttoi(aStrLine);
//			break;
//		}
//	}
//}

void CLineList::OnBnClickedBtnLineadd()
{
	// TODO: 在此添加控件通知处理程序代码
	LineInfo* line = new LineInfo;
	line->iLineID = 1;
    line->iOrgID = m_CRWDSClientView->m_CurrentOrg->iOrgID;
	line->iLineName = _T("线路");
	line->iStartKm = m_CRWDSClientView->m_CurrentOrg->iMapPoint[0]->iKM;
	line->iLineKmLonLat.push_back(m_CRWDSClientView->m_CurrentOrg->iMapPoint[0]);
	line->iLineKmTime.push_back(0);
	line->iStartNo = KFirstDay;
    GetDlgItem(IDC_EDIT_LINEREMARK)->GetWindowText(line->iLineRemark);
	m_CRWDSClientView->m_CurrentOrg->iLine.push_back(line);
    SetOrgLine(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_LINE_ADD, line);

	CString id;
	CString name;
	CString startKm;
	CString endKm;
	int count = m_ListCtrl.GetItemCount();

	id.Format(_T("%d"), line->iLineID);
	startKm.Format(_T("%.2f"), line->iStartKm);
	endKm.Format(_T("%.2f"), line->iLineKmLonLat[line->iLineKmLonLat.size()-1]->iKM);
	name = line->iLineName;

	m_ListCtrl.InsertItem(count, name);
	m_ListCtrl.SetItemText(count, 1, id);
	m_ListCtrl.SetItemText(count, 2, startKm);
	m_ListCtrl.SetItemText(count, 3, endKm);
}

void CLineList::OnClickedBtnLinemodify()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos;
	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int select = m_ListCtrl.GetNextSelectedItem(pos);  
	if (select<0)
	{
		return;
	}
	LineInfo* line = m_CRWDSClientView->m_CurrentOrg->iLine[select];
    GetDlgItem(IDC_EDIT_LINENAME)->GetWindowText(line->iLineName);
    GetDlgItem(IDC_EDIT_LINEREMARK)->GetWindowText(line->iLineRemark);
	AfxMessageBox(_T("修改成功"), MB_OK);
    m_ListCtrl.SetItemText(select, 0, line->iLineName);
    SetOrgLine(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_LINE_MODIFY, line);
}

void CLineList::OnClickedBtnLinedelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int select = m_ListCtrl.GetSelectionMark();
	if (select < 0)
		return;
	LineInfo* line = m_CRWDSClientView->m_CurrentOrg->iLine[select];
	m_CRWDSClientView->m_CurrentOrg->iLine.erase(m_CRWDSClientView->m_CurrentOrg->iLine.begin()+select);

	m_ListCtrl.DeleteItem(select);
	m_ListAllPoint.ResetContent();
	m_ListSelectedPoint.ResetContent();

    //清除员工设置的线
    //StaffInfo* staff;
    //for (size_t i=0; i<m_CRWDSClientView->m_CurrentOrg->iStaff.size(); i++)
    //{
    //    staff = m_CRWDSClientView->m_CurrentOrg->iStaff[i];
    //    for (size_t j=0; j<staff->iArrangeLine.size(); j++)
    //    {
    //        if(line == staff->iArrangeLine[j])
    //        {
    //            staff->iArrangeLine.erase(staff->iArrangeLine.begin()+j);
    //            SetOrgStaff(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_STAFF_MODIFY, staff);
    //            break;
    //        }
    //    }
    //}
    SetOrgLine(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_LINE_DELETE, line);
    delete line;
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
	// 得到项目索引
	POSITION pos;
	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int select = m_ListCtrl.GetNextSelectedItem(pos);  
	if (select<0)
	{
		return;
	}
	CString str;
    //GetDlgItem(IDC_EDIT_LINEREMARK)->SetWindowText(_T(""));
    m_ListAllPoint.ResetContent();
	m_ListSelectedPoint.ResetContent();
	m_Selected.clear();
	m_Unselected.clear();
    LineInfo* line = m_CRWDSClientView->m_CurrentOrg->iLine[select];
    GetDlgItem(IDC_EDIT_LINEREMARK)->SetWindowText(line->iLineRemark);
    GetDlgItem(IDC_EDIT_LINENAME)->SetWindowText(line->iLineName);
    str.Format(_T("%d"), line->iLineID);
    GetDlgItem(IDC_EDIT_LINEID)->SetWindowText(str);
	for (size_t i=0; i<m_CRWDSClientView->m_CurrentOrg->iMapPoint.size(); i++)
	{//根据已选点与未选点加载界面
		bool addSelect = FALSE;
		for (size_t j=0; j<line->iLineKmLonLat.size(); j++)
		{
			if (m_CRWDSClientView->m_CurrentOrg->iMapPoint[i] == line->iLineKmLonLat[j])
			{
				addSelect = TRUE;
				break;
			}
		}
		ENCODERAILWAYFULLNAME(str, m_CRWDSClientView->m_CurrentOrg->iMapPoint[i]->iRailLine->iRailName, 
                              m_CRWDSClientView->m_CurrentOrg->iMapPoint[i]->iKM, 
                              m_CRWDSClientView->m_CurrentOrg->iMapPoint[i]->iDirect);
		if (addSelect)
		{
			m_ListSelectedPoint.AddString(str);
			m_Selected.push_back(m_CRWDSClientView->m_CurrentOrg->iMapPoint[i]);
		}
		else
		{
			m_ListAllPoint.AddString(str);
			m_Unselected.push_back(m_CRWDSClientView->m_CurrentOrg->iMapPoint[i]);
		}
	}

	*pResult = 0;
}


void CLineList::OnNMDblclkLinelist(NMHDR *pNMHDR, LRESULT *pResult)
{//双击listctrl编辑线路名
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	//int tIndex = m_ListCtrl.GetNextSelectedItem(pos);

	//if ( tIndex >= 0 )
	//{
	//	m_ListCtrl.SetFocus();
	//	CEdit* pEdit = m_ListCtrl.EditLabel( tIndex );
	//}

	*pResult = 0;
}


void CLineList::OnLvnEndlabeleditLinelist(NMHDR *pNMHDR, LRESULT *pResult)
{//完成线路名编辑
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//CString pName = pDispInfo->item.pszText;
	//int tIndex = pDispInfo->item.iItem;
	//m_ListCtrl.SetFocus();

	//if( ( ! pName.IsEmpty() ) && tIndex >= 0 )
	//{
	//	m_ListCtrl.SetItemText( tIndex, 0, pName);
	//}
	*pResult = 0;
}


void CLineList::OnBnClickedBtnAdd1()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_ListAllPoint.GetCurSel();
	if (index<0)
		return;
	MapPoint* point = m_Unselected[index];
	m_Unselected.erase(m_Unselected.begin()+index);
	m_Selected.push_back(point);

	POSITION pos;
	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int select = m_ListCtrl.GetNextSelectedItem(pos);  
	if (select<0)
	{
		return;
	}
	LineInfo* line = m_CRWDSClientView->m_CurrentOrg->iLine[select];
	line->iLineKmLonLat.push_back(point);
	line->iLineKmTime.push_back(0);//为新加点设置时间
    //SetOrgLine(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_LINE_MODIFY, line);

	CString str;
	ENCODERAILWAYFULLNAME(str, point->iRailLine->iRailName, point->iKM, point->iDirect);
	m_ListSelectedPoint.InsertString(m_ListSelectedPoint.GetCount(), str);
	m_ListAllPoint.DeleteString(index);

	str.Format(_T("%.0f"), line->iLineKmLonLat[0]->iKM);
	m_ListCtrl.SetItemText(select, 2, str);
	str.Format(_T("%.0f"), point->iKM);
	m_ListCtrl.SetItemText(select, 3, str);
}


void CLineList::OnBnClickedBtnRemove1()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_ListSelectedPoint.GetCurSel();
	if (index<0)
		return;
	MapPoint* point = m_Selected[index];
	m_Selected.erase(m_Selected.begin()+index);
	m_Unselected.push_back(point);

	POSITION pos;
	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int select = m_ListCtrl.GetNextSelectedItem(pos);  
	if (select<0)
	{
		return;
	}
	LineInfo* line = m_CRWDSClientView->m_CurrentOrg->iLine[select];
	line->iLineKmLonLat.erase(line->iLineKmLonLat.begin()+index);
	line->iLineKmTime.erase(line->iLineKmTime.begin()+index);

    //SetOrgLine(m_CRWDSClientView->m_CurrentOrg->iOrgID, CMD_LINE_MODIFY, line);

	CString str;
	if (line->iLineKmLonLat.size() != 0)
	{
		str.Format(_T("%.0f"), line->iLineKmLonLat[0]->iKM);
		m_ListCtrl.SetItemText(select, 2, str);
		str.Format(_T("%.0f"), line->iLineKmLonLat[line->iLineKmLonLat.size()-1]->iKM);
		m_ListCtrl.SetItemText(select, 3, str);
	}
	else
	{
		m_ListCtrl.SetItemText(select, 2, _T(""));
		m_ListCtrl.SetItemText(select, 3, _T(""));
	}

	ENCODERAILWAYFULLNAME(str, point->iRailLine->iRailName, point->iKM, point->iDirect);
	m_ListAllPoint.InsertString(m_ListAllPoint.GetCount(), str);
	m_ListSelectedPoint.DeleteString(index);

}
