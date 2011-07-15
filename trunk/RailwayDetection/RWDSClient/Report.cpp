// Report.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "Report.h"
#include "afxdialogex.h"
#include "DataService.h"
#include "ErrorDefine.h"
#include "DataListControl.h"
#include "CApplication.h"
#include "CWorkbooks.h"
#include "CWorksheets.h"
#include "CRange.h"
#include "CFont0.h"
#include "CWorkbook.h"
#include "CWorksheet.h"
#include "StaffLogs.h"


// CReport 对话框

IMPLEMENT_DYNAMIC(CReport, CDialogEx)

CReport::CReport(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReport::IDD, pParent)
{
    m_RWDSClientView = (CRWDSClientView*) pParent;
}

CReport::~CReport()
{
}

void CReport::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_REPORT, m_ListCtrl);
   // DDX_Control(pDX, IDC_COMBO_REPORTMONTH, m_ComboReportMonth);
}


BEGIN_MESSAGE_MAP(CReport, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_SEARCHREPORT, &CReport::OnBnClickedBtnSearchreport)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_REPORT, &CReport::OnNMDblclkListReport)
    ON_BN_CLICKED(IDC_BTN_VIEWDETAIL, &CReport::OnBnClickedBtnViewdetail)
    ON_BN_CLICKED(IDCANCEL, &CReport::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BTN_EXPORTREPORT, &CReport::OnBnClickedBtnExportreport)
END_MESSAGE_MAP()


// CReport 消息处理程序


BOOL CReport::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);
    //m_ListCtrl.InsertColumn(0, _T("员工号"), LVCFMT_LEFT, clientRect.Width()/4);
    m_ListCtrl.InsertColumn(0, _T("日期"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(1, _T("星期"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(2, _T("员工号"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(3, _T("计划记录"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(4, _T("实际记录"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(5, _T("异常次数"), LVCFMT_LEFT, clientRect.Width()/7);
    m_ListCtrl.InsertColumn(6, _T("未到达次数"), LVCFMT_LEFT, clientRect.Width()/7);

    //m_ComboReportMonth.AddString(_T("1月"));
    //m_ComboReportMonth.AddString(_T("2月"));
    //m_ComboReportMonth.AddString(_T("3月"));
    //m_ComboReportMonth.AddString(_T("4月"));
    //m_ComboReportMonth.AddString(_T("5月"));
    //m_ComboReportMonth.AddString(_T("6月"));
    //m_ComboReportMonth.AddString(_T("7月"));
    //m_ComboReportMonth.AddString(_T("8月"));
    //m_ComboReportMonth.AddString(_T("9月"));
    //m_ComboReportMonth.AddString(_T("10月"));
    //m_ComboReportMonth.AddString(_T("11月"));
    //m_ComboReportMonth.AddString(_T("12月"));
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CReport::OnBnClickedBtnSearchreport()
{
    // TODO: 在此添加控件通知处理程序代码
    //int month = m_ComboReportMonth.GetCurSel()+1;
    //if (month <=0)
    //{
    //    AfxMessageBox(_T("请选择月份"));
    //    return;
    //}
    CString date;
    GetDlgItem(IDC_EDIT_REVIEWDAY)->GetWindowText(date);
    if (date.GetLength() != 6)
    {
        AfxMessageBox(_T("请正确输入查询时间"));
        return;
    }
    DeleteReportListElement(&m_RWDSClientView->m_CurrentOrg->iReportInfo);
    int year = _ttoi(date.Left(4));
    int month = _ttoi(date.Right(2));
    

    int err = GetReportInfoList(m_RWDSClientView->m_CurrentOrg->iOrgID, year, month, &m_RWDSClientView->m_CurrentOrg->iReportInfo);
    if (err == KErrNone)
    {
        CString str;
        str.Format(_T("记录条数: %d"), m_RWDSClientView->m_CurrentOrg->iReportInfo.size());
        GetDlgItem(IDC_STATIC_TOTAL)->SetWindowText(str);
        m_ListCtrl.DeleteAllItems();
        ReportInfo* report = NULL;
        CString strDate;
        CString strPlanArrived;
        CString strActualArrived;
        CString strAbnormal;
        CString strUnArrived;
        for(size_t i=0; i<m_RWDSClientView->m_CurrentOrg->iReportInfo.size(); i++)
        {
            report = m_RWDSClientView->m_CurrentOrg->iReportInfo[i];
            //CTime rTime(report->iReportDay);
            //strDate.Format(_T("%02d月%02d日"), rTime.GetMonth(), rTime.GetDay());
            strPlanArrived.Format(_T("%d"), report->iPlanArrived);
            strActualArrived.Format(_T("%d"), report->iActualArrived);
            strAbnormal.Format(_T("%d"), report->iAbnormal);
            strUnArrived.Format(_T("%d"), report->iUnArrived);

            int count = m_ListCtrl.GetItemCount();
            m_ListCtrl.InsertItem(count, report->iReportDay);
            m_ListCtrl.SetItemText(count, 1, report->iWeekDay);
            m_ListCtrl.SetItemText(count, 2, report->iStaffName);
            m_ListCtrl.SetItemText(count, 3, strPlanArrived);
            m_ListCtrl.SetItemText(count, 4, strActualArrived);
            m_ListCtrl.SetItemText(count, 5, strAbnormal);
            m_ListCtrl.SetItemText(count, 6, strUnArrived);
            m_ListCtrl.SetItemData(count, (DWORD_PTR)report);
        }
    }
    else
    {
        GetDlgItem(IDC_STATIC_TOTAL)->SetWindowText(_T("获取数据失败"));
    }
}

void CReport::ViewReportDetail(ReportInfo* aReport)
{
    if (aReport)
    {
        CStaffLogs logs;
        logs.SetReportInfo(aReport);
        logs.DoModal();
    }
}

void CReport::OnNMDblclkListReport(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    POSITION pos;
    pos = m_ListCtrl.GetFirstSelectedItemPosition();
    // 得到项目索引
    int select = m_ListCtrl.GetNextSelectedItem(pos);  
    if (select<0)
    {
        return;
    }
    ReportInfo* report = (ReportInfo*)m_ListCtrl.GetItemData(select);
    ViewReportDetail(report);
    *pResult = 0;
}


void CReport::OnBnClickedBtnViewdetail()
{
    // TODO: 在此添加控件通知处理程序代码
    POSITION pos;
    pos = m_ListCtrl.GetFirstSelectedItemPosition();
    // 得到项目索引
    int select = m_ListCtrl.GetNextSelectedItem(pos);  
    if (select<0)
    {
        return;
    }
    ReportInfo* report = (ReportInfo*)m_ListCtrl.GetItemData(select);
    ViewReportDetail(report);
}


void CReport::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    m_RWDSClientView->m_OpenReportForm = FALSE;
    CWnd::DestroyWindow();
    //CDialogEx::OnCancel();
}

//获取单元头的函数：

void   CReport::GetCellName(int nRow, int nCol, CString &strName) 
{ 
    int nSeed = nCol; 
    CString strRow; 
    char cCell = 'A' + nCol - 1;
    strName.Format(_T("%c"), cCell); 
    strRow.Format(_T( "%d "), nRow); 
    strName += strRow; 
}

void CReport::OnBnClickedBtnExportreport()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_ListCtrl.GetItemCount() == 0)
    {
        return;
    }

    CFileDialog hFileDlg(FALSE, NULL, NULL,
        OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST,
        TEXT(_T("excel文件 (*.xlsx, *.xls)|*.xlsx, *.xls|所有文件(*.*)|*.*|")),
        NULL);
    CString filer;

    if(hFileDlg.DoModal() == IDCANCEL)
    {
        return;
    }
    switch( hFileDlg.GetOFN().nFilterIndex)
    {
    case 1:
        filer = _T(".xlsx");
        break;
    default:
        filer = _T("");
        break;
    }
    CString strFile = hFileDlg.GetPathName();
    strFile += filer;
    UpdateData(FALSE);
    
    COleVariant covTrue((short)TRUE), covFalse((short)FALSE), covOptional((long)DISP_E_PARAMNOTFOUND,   VT_ERROR); 
    CApplication   app; 
    CWorkbooks   books; 
    CWorkbook   book; 
    CWorksheets   sheets; 
    CWorksheet   sheet; 
    CRange   range; 
    CFont0   font;
    if (!app.CreateDispatch(_T("Excel.Application")))
    {
        AfxMessageBox(_T("创建失败！"));
        return;
    }
    //Get   a   new   workbook. 
    books = app.get_Workbooks();
    book = books.Add(covOptional);
    sheets = book.get_Worksheets();
    sheet = sheets.get_Item(COleVariant((short)1));

    CHeaderCtrl   *pmyHeaderCtrl; 
    pmyHeaderCtrl = m_ListCtrl.GetHeaderCtrl();//此句取得CListCtrl控件的列表頭
    int   iRow,iCol; 
    int   m_cols   =   pmyHeaderCtrl-> GetItemCount(); 
    int   m_rows = m_ListCtrl.GetItemCount(); 
    HDITEM   hdi; 
    TCHAR     lpBuffer[256]; 
    bool       fFound   =   false; 
    hdi.mask   =   HDI_TEXT; 
    hdi.pszText   =   lpBuffer; 
    hdi.cchTextMax   =   256; 
    CString   colname; 
    CString strTemp;
    for(iCol=0;   iCol <m_cols;   iCol++)//将列表的标题头写入EXCEL 
    { 
        GetCellName(1 ,iCol + 1, colname);
        range   =   sheet.get_Range(COleVariant(colname),COleVariant(colname)); 
        pmyHeaderCtrl-> GetItem(iCol,   &hdi); 
        range.put_Value2(COleVariant(hdi.pszText));
        int   nWidth   =   m_ListCtrl.GetColumnWidth(iCol)/6; 
        //得到第iCol+1列   
        range.AttachDispatch(range.get_Item(_variant_t((long)(iCol+1)),vtMissing).pdispVal,true);   

        //设置列宽  
        range.put_ColumnWidth(_variant_t((long)nWidth));
    } 

    range   =   sheet.get_Range(COleVariant( _T("A1 ")),   COleVariant(colname)); 
    range.put_RowHeight(_variant_t((long)50));//设置行的高度 
    font = range.get_Font();
    font.put_Bold(covTrue);
    range.put_VerticalAlignment(COleVariant((short)-4108));//xlVAlignCenter   =   -4108

    COleSafeArray   saRet; 
    DWORD   numElements[]={m_rows,m_cols};       //5x2   element   array 
    saRet.Create(VT_BSTR,   2,   numElements); 
    range   =   sheet.get_Range(COleVariant( _T("A2 ")),covOptional); 
    range = range.get_Resize(COleVariant((short)m_rows),COleVariant((short)m_cols));
    long   index[2]; 
    range   =   sheet.get_Range(COleVariant( _T("A2 ")),covOptional); 
    range   =   range.get_Resize(COleVariant((short)m_rows),COleVariant((short)m_cols)); 
    for   (   iRow   =   1;   iRow   <=   m_rows;   iRow++)//将列表内容写入EXCEL 
    { 
        for(iCol = 1; iCol <= m_cols; iCol++)   
        { 
            index[0]=iRow-1; 
            index[1]=iCol-1; 
            CString   szTemp; 
            szTemp=m_ListCtrl.GetItemText(iRow-1,iCol-1); 
            BSTR bstr = szTemp.AllocSysString(); 
            saRet.PutElement(index,bstr); 
            SysFreeString(bstr); 
        } 
    } 
    range.put_Value2(COleVariant(saRet));

    //       Cnterior cellinterior;
    //       range   =   sheet.get_Range(COleVariant( _T("A1 ")),covOptional); 
    //       range   =   range.get_Resize(COleVariant((short)1),COleVariant((short)m_cols)); 
    //       books = range.get_Interior();
    //       cellinterior.AttachDispatch(books); 
    //       cellinterior.put_ColorIndex(COleVariant((short)37));//设置EXCEL头一行的背景颜色
    //       for(   iRow=1;   iRow   <=   m_rows;   iRow++)//设置EXCEL其余的背景颜色（颜色交替变换）
    //       { 
    //                 int   state=iRow%2; 
    //                 CString   index; 
    //                 index.Format( _T("A%d "),iRow+1); 
    //                 range   =   sheet.get_Range(COleVariant(index),covOptional); 
    //                 range   =   range.get_Resize(COleVariant((short)1),COleVariant((short)m_cols)); 
    //                 books=range.get_Interior(); 
    //                 cellinterior.AttachDispatch(books); 
    //                 if(!state) 
    //                          cellinterior.put_ColorIndex(COleVariant((short)36));
    //                 else 
    //                          cellinterior.put_ColorIndex(COleVariant((short)24));
    //       } 

    saRet.Detach(); 

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    book.SaveCopyAs(COleVariant(strFile)); 
    //       cellinterior.ReleaseDispatch(); 
    book.put_Saved(true);
    book.ReleaseDispatch();   
    books.ReleaseDispatch();   
    app.Quit(); 
    app.ReleaseDispatch();
}
