
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "RWDSClient.h"
#include "DataService.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
    ON_NOTIFY(NM_CLICK, 4, &CFileView::OnNMClickFileView)
	ON_NOTIFY(NM_DBLCLK, 4, &CFileView::OnNMDblclkFileView)
	ON_NOTIFY(TVN_ITEMEXPANDED, 4, &CFileView::OnTvnItemexpandedFileView)
	ON_NOTIFY(TVN_ITEMEXPANDING, 4, &CFileView::OnTvnItemexpandingFileView)
		
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar ��Ϣ�������

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ������ͼ:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("δ�ܴ����ļ���ͼ\n");
		return -1;      // δ�ܴ���
	}

	// ������ͼͼ��:
    m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
    m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ������*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// ����һЩ��̬����ͼ����
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
    int i = m_RWDSClientView->m_TestValue;
    OrganizationInfo* org = m_RWDSClientView->m_Org[0];
    HTREEITEM hRoot = m_wndFileView.InsertItem(org->iOrgName, 8, 8);
    m_wndFileView.SetItemData(hRoot, (DWORD_PTR)org);
    m_wndFileView.InsertItem(_T(""), 8, 8, hRoot);//Ϊ����ʾ+��
}

void CFileView::CleanFileView()
{
    m_wndFileView.DeleteAllItems();
    //m_wndFileView.GetItemData()
}

void CFileView::TreeVisitForDeleteItemData(HTREEITEM aItem)
{
    if(m_wndFileView.ItemHasChildren(aItem))
    {
        HTREEITEM item = m_wndFileView.GetChildItem(aItem);

        OrganizationInfo* curOrg = (OrganizationInfo*) m_wndFileView.GetItemData(aItem);
        curOrg->iLine.clear();

        while (item != NULL)
        {
            TreeVisitForDeleteItemData(item);
            item = m_wndFileView.GetNextItem(item, TVGN_NEXT);
        }
    }
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// ѡ���ѵ�������:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}
	pWndTree->SetFocus();
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("����...."));

}

void CFileView::OnFileOpen()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnFileOpenWith()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnDummyCompile()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnEditCut()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnEditCopy()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnEditClear()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ����*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�޷�����λͼ: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void CFileView::OnNMClickFileView(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    HTREEITEM curItem = reinterpret_cast<LPNMTREEVIEW>(pNMHDR)->itemNew.hItem;
    if (!m_wndFileView.ItemHasChildren(curItem))
    {
        return;
    }
    HTREEITEM childItem = m_wndFileView.GetChildItem(curItem);
    OrganizationInfo* curOrg = (OrganizationInfo*) m_wndFileView.GetItemData(curItem);
    //ɾ��view����ĵ���
    m_RWDSClientView->DeleteAllLine();
    m_RWDSClientView->DeleteAllMapPoint();
    m_RWDSClientView->DeleteAllStaff();
    //��ȡ�û���ӵ�еĵ���Ա��
    GetOrgPoint(curOrg->iOrgID, &m_RWDSClientView->m_MapPoint);
    GetOrgStaff(curOrg->iOrgID, &m_RWDSClientView->m_Staff);
    GetOrgDevice(curOrg->iOrgID, &m_RWDSClientView->m_Device);
    m_RWDSClientView->m_CurrentOrg = curOrg;
    if (m_wndFileView.GetItemText(childItem).Compare(_T("")) == 0)
    {//��δչ�������һ�����
        //��ȡ�û���ӵ�е�·��
        GetOrgLine(curOrg->iOrgID, m_RWDSClientView->m_MapPoint, &m_RWDSClientView->m_Line);
    }
    else
    {
        for(size_t i=0; i<curOrg->iLine.size(); i++)
        {
            m_RWDSClientView->m_Line.push_back(curOrg->iLine[i]);
        }
    }
    *pResult = 0;
}

void CFileView::OnNMDblclkFileView(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ֻ������·��double�¼�
	HTREEITEM curItem = m_wndFileView.GetSelectedItem();
	if (m_wndFileView.ItemHasChildren(curItem))
	{//���ڵ�Ϊ·�ߣ���������ӽڵ������ǰ�ڵ㲻��·��
		*pResult = 0;
		return;
	}
    m_RWDSClientView->m_DisplayFlag = KRealTime;
	m_RWDSClientView->MapxCleanAllFeature(m_RWDSClientView->m_SymbolLayer);
	LineInfo* line = (LineInfo*)m_wndFileView.GetItemData(curItem);
	for (size_t i=0; i<line->iLineKmLonLat.size(); i++)
	{//�ڵ�ͼ�ϻ��Ƶ���
		m_RWDSClientView->MapxDrawCircle(line->iLineKmLonLat[i]->iLon, line->iLineKmLonLat[i]->iLat, m_RWDSClientView->m_SymbolLayer, miColorBlue);
		if (i>0)
		{
			m_RWDSClientView->MapxDrawLine(line->iLineKmLonLat[i-1]->iLon, line->iLineKmLonLat[i-1]->iLat,
										   line->iLineKmLonLat[i]->iLon, line->iLineKmLonLat[i]->iLat, miColorBlue);
		}
		CString str;
		CTime arrTime(line->iLineKmTime[i]);
		str.Format(_T("%02d:%02d"), arrTime.GetHour(), arrTime.GetMinute());
		m_RWDSClientView->MapxSetText(line->iLineKmLonLat[i]->iLon-0.0001, line->iLineKmLonLat[i]->iLat-0.0005, str);
	}
	int centerPoint = line->iLineKmLonLat.size()/2;
	m_RWDSClientView->m_MapX.SetCenterX(line->iLineKmLonLat[centerPoint]->iLon);
	m_RWDSClientView->m_MapX.SetCenterY(line->iLineKmLonLat[centerPoint]->iLat);
	m_RWDSClientView->m_MapX.SetZoom(m_RWDSClientView->m_InitZoom/256);
	*pResult = 0;
}

void CFileView::OnTvnItemexpandingFileView(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (pNMTreeView->action ==  TVE_EXPAND)
    {//չ��
        HTREEITEM curItem = reinterpret_cast<LPNMTREEVIEW>(pNMHDR)->itemNew.hItem;
        if (!m_wndFileView.ItemHasChildren(curItem))
        {
            return;
        }
        HTREEITEM childItem = m_wndFileView.GetChildItem(curItem);
        OrganizationInfo* curOrg = (OrganizationInfo*) m_wndFileView.GetItemData(curItem);
        m_RWDSClientView->m_CurrentOrg = curOrg;
        if (m_wndFileView.GetItemText(childItem).Compare(_T("")) == 0)
        {//��δչ�������һ�����

            //ɾ��view����ĵ���
            m_RWDSClientView->DeleteAllLine();
            m_RWDSClientView->DeleteAllMapPoint();
            m_RWDSClientView->DeleteAllStaff();
            //��ȡ�û���ӵ�еĵ�/��/Ա��
            GetOrgPoint(curOrg->iOrgID, &m_RWDSClientView->m_MapPoint);
            GetOrgLine(curOrg->iOrgID, m_RWDSClientView->m_MapPoint, &curOrg->iLine);
            GetOrgStaff(curOrg->iOrgID, &m_RWDSClientView->m_Staff);
            
            for(size_t i=0; i<curOrg->iLine.size(); i++)
            {
                m_RWDSClientView->m_Line.push_back(curOrg->iLine[i]);
            }
            GetCalendarSchedule(curOrg->iOrgID, m_RWDSClientView->m_Calendar);
        }
    }
    *pResult = 0;
}

void CFileView::OnTvnItemexpandedFileView(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pNMTreeView->action ==  TVE_EXPAND)
	{
		//HTREEITEM curItem = m_wndFileView.GetSelectedItem();
		HTREEITEM curItem = reinterpret_cast<LPNMTREEVIEW>(pNMHDR)->itemNew.hItem;
		HTREEITEM childItem = m_wndFileView.GetChildItem(curItem);
		if (m_wndFileView.GetItemText(childItem).Compare(_T("")) != 0)
		{//��item�Ѿ�չ����
			return;
		}
		else
		{
			m_wndFileView.DeleteItem(childItem);//�����ʱ��Чitem
		}
		OrganizationInfo* curOrg = (OrganizationInfo*) m_wndFileView.GetItemData(curItem);
		m_RWDSClientView->m_CurrentOrg = curOrg;
		HTREEITEM tmpChild;
		if(curOrg->iChildOrg.size() > 0)
		{//��������
			OrganizationInfo* org;
			for(size_t i=0; i<curOrg->iChildOrg.size(); i++)
			{
				org = curOrg->iChildOrg[i];
				tmpChild = m_wndFileView.InsertItem(org->iOrgName, 8, 8, curItem);
				m_wndFileView.SetItemData(tmpChild, (DWORD_PTR)org);
				m_wndFileView.InsertItem(_T(""), 8, 8, tmpChild);//Ϊ����ʾ+��
			}
		}
		else if (curOrg->iLine.size() > 0)
		{//������·
			LineInfo* line;
			CString str;
			for(size_t i=0; i<curOrg->iLine.size(); i++)
			{
				line = curOrg->iLine[i];
				tmpChild = m_wndFileView.InsertItem(line->iLineName, 8, 8, curItem);
				m_wndFileView.SetItemData(tmpChild, (DWORD_PTR)line);
			}
		}
	}
	*pResult = 0;
}


void CFileView::SetRWDSClientView( CRWDSClientView* aRWDSClientView )
{
	//m_RWDSClientView = aRWDSClientView;
	m_RWDSClientView = gClientView;
	//CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	//m_RWDSClientView=(CRWDSClientView *)pMain->GetActiveView();
}
