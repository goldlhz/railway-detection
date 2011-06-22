// PitureReview.cpp : 实现文件
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "PitureReview.h"
#include "afxdialogex.h"
#include "DataService.h"


// CPitureReview 对话框

IMPLEMENT_DYNAMIC(CPitureReview, CDialogEx)

CPitureReview::CPitureReview(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPitureReview::IDD, pParent)
{
    m_CRWDSClientView = static_cast<CRWDSClientView*>(pParent);
}

CPitureReview::~CPitureReview()
{
}

void CPitureReview::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DATETIMEPICKER_PICTURE, m_DatePicture);
    DDX_Control(pDX, IDC_DATETIMEPICKER_PICTUREEND, m_EndDatePicture);
    DDX_Control(pDX, IDC_PICTURELIST, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CPitureReview, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_SHOWPICTURE, &CPitureReview::OnBnClickedBtnShowpicture)
    ON_BN_CLICKED(IDC_BTN_DOWNLOADPICTURE, &CPitureReview::OnBnClickedBtnDownloadpicture)
    ON_NOTIFY(NM_DBLCLK, IDC_PICTURELIST, &CPitureReview::OnNMDblclkPicturelist)
END_MESSAGE_MAP()


// CPitureReview 消息处理程序

BOOL CPitureReview::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);
    m_ListCtrl.InsertColumn(0, _T("图片名称"), LVCFMT_LEFT, clientRect.Width()/3);
    m_ListCtrl.InsertColumn(1, _T("拍摄时间"), LVCFMT_LEFT, clientRect.Width()/3);
    m_ListCtrl.InsertColumn(2, _T("错误类型"), LVCFMT_LEFT, clientRect.Width()/3);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CPitureReview::OnBnClickedBtnDownloadpicture()
{
    // TODO: 在此添加控件通知处理程序代码
    CTime pictureTimeStart;
    CTime pictureTimeEnd;
    m_DatePicture.GetTime(pictureTimeStart);
    m_EndDatePicture.GetTime(pictureTimeEnd);
    time_t start = pictureTimeStart.GetTime();
    time_t end = pictureTimeEnd.GetTime();
    GetPictureInfo(m_CRWDSClientView->m_CurrentOrg->iOrgID, start, end, &m_PictureList);
    PictureInfo* picture;
    //在ctrllist上显示
    for (size_t i=0; i<m_PictureList.size(); i++)
    {
        picture = m_PictureList[i];
        m_ListCtrl.InsertItem(i, picture->iPicName);
        //CTime shootingTime(picture->iShootingTime);
        //CString str = shootingTime.Format(_T("%Y-%m-%d %H:%M:%S"));//2011-1-1 12:00:00
        m_ListCtrl.SetItemText(i, 1, picture->iShootingTime);
        m_ListCtrl.SetItemText(i, 2, strPictureErrorType[picture->iErrorType]);
        m_ListCtrl.SetItemData(i, (DWORD_PTR)picture);
    }
}

CString CPitureReview::GetModulePath()
{
    CString modDir;
    GetModuleFileName(NULL, modDir.GetBuffer(MAX_PATH), MAX_PATH); 
    modDir.ReleaseBuffer();
    int pos;   
    pos = modDir.ReverseFind('\\');   
    modDir = modDir.Left(pos);  
    return modDir;
}

void CPitureReview::ShowPictureInWindows(PictureInfo* aPicture)
{
    CString picturePath;
    picturePath = GetModulePath() + _T("\\pic\\");
    CString pictureDirect = picturePath + aPicture->iPicName;
    BOOL fileExist = PathFileExists(picturePath);
    if(!fileExist)
    {//文件不存在，下载文件
        int retValue = SavePictureToDirect(m_CRWDSClientView->m_CurrentOrg->iOrgID, aPicture, picturePath);
        if (retValue == 0)
        {
            fileExist = TRUE;
        }
    }
    if (fileExist)
    {
        ShellExecute(NULL, _T("open"), pictureDirect, NULL, NULL, SW_SHOW);
    }
}

void CPitureReview::OnNMDblclkPicturelist(NMHDR *pNMHDR, LRESULT *pResult)
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
    PictureInfo* selectPicture = (PictureInfo*)m_ListCtrl.GetItemData(select);
    if (selectPicture)
    {
        ShowPictureInWindows(selectPicture);
    }
    //
    *pResult = 0;
}


void CPitureReview::OnBnClickedBtnShowpicture()
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
    PictureInfo* selectPicture = (PictureInfo*)m_ListCtrl.GetItemData(select);
    if (selectPicture)
    {
        ShowPictureInWindows(selectPicture);
    }
}

