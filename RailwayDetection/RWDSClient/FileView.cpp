
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "RWDSClient.h"

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

	// ����һЩ��̬����ͼ����(�˴�ֻ������������룬�����Ǹ��ӵ�����)
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
	//HTREEITEM hRoot = m_wndFileView.InsertItem(_T("FakeApp �ļ�"), 0, 0);
	//m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	//HTREEITEM hSrc = m_wndFileView.InsertItem(_T("FakeApp Դ�ļ�"), 0, 0, hRoot);

	//m_wndFileView.InsertItem(_T("FakeApp.cpp"), 1, 1, hSrc);
	//m_wndFileView.InsertItem(_T("FakeApp.rc"), 1, 1, hSrc);
	//m_wndFileView.InsertItem(_T("FakeAppDoc.cpp"), 1, 1, hSrc);
	//m_wndFileView.InsertItem(_T("FakeAppView.cpp"), 1, 1, hSrc);
	//m_wndFileView.InsertItem(_T("MainFrm.cpp"), 1, 1, hSrc);
	//m_wndFileView.InsertItem(_T("StdAfx.cpp"), 1, 1, hSrc);

	//HTREEITEM hInc = m_wndFileView.InsertItem(_T("FakeApp ͷ�ļ�"), 0, 0, hRoot);

	//m_wndFileView.InsertItem(_T("FakeApp.h"), 2, 2, hInc);
	//m_wndFileView.InsertItem(_T("FakeAppDoc.h"), 2, 2, hInc);
	//m_wndFileView.InsertItem(_T("FakeAppView.h"), 2, 2, hInc);
	//m_wndFileView.InsertItem(_T("Resource.h"), 2, 2, hInc);
	//m_wndFileView.InsertItem(_T("MainFrm.h"), 2, 2, hInc);
	//m_wndFileView.InsertItem(_T("StdAfx.h"), 2, 2, hInc);

	//HTREEITEM hRes = m_wndFileView.InsertItem(_T("FakeApp ��Դ�ļ�"), 0, 0, hRoot);

	//m_wndFileView.InsertItem(_T("FakeApp.ico"), 2, 2, hRes);
	//m_wndFileView.InsertItem(_T("FakeApp.rc2"), 2, 2, hRes);
	//m_wndFileView.InsertItem(_T("FakeAppDoc.ico"), 2, 2, hRes);
	//m_wndFileView.InsertItem(_T("FakeToolbar.bmp"), 2, 2, hRes);

	//m_wndFileView.Expand(hRoot, TVE_EXPAND);
	//m_wndFileView.Expand(hSrc, TVE_EXPAND);
	//m_wndFileView.Expand(hInc, TVE_EXPAND);


	OrganizationInfo* org = m_RWDSClientView->m_Org[0];
	HTREEITEM hRoot = m_wndFileView.InsertItem(org->iOrgName, 0, 0);
	m_wndFileView.SetItemData(hRoot, (DWORD_PTR)org);
	m_wndFileView.InsertItem(_T(""), 0, 0, hRoot);//Ϊ����ʾ+��
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

void CFileView::OnNMDblclkFileView(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ֻ������·��double�¼�
	HTREEITEM curItem = m_wndFileView.GetSelectedItem();
	if (m_wndFileView.ItemHasChildren(curItem))
	{
		*pResult = 0;
		return;
	}
	m_RWDSClientView->MapxCleanAllFeature();
	LineInfo* line = (LineInfo*)m_wndFileView.GetItemData(curItem);
	for (int i=0; i<line->iLineKmLonLat.size(); i++)
	{
		m_RWDSClientView->MapxDrawCircle(line->iLineKmLonLat[i]->iLon, line->iLineKmLonLat[i]->iLat);
		if (i>0)
		{
			m_RWDSClientView->MapxDrawLine(line->iLineKmLonLat[i-1]->iLon, line->iLineKmLonLat[i-1]->iLat,
										   line->iLineKmLonLat[i]->iLon, line->iLineKmLonLat[i]->iLat);
		}
	}
	int centerPoint = line->iLineKmLonLat.size()/2;
	m_RWDSClientView->m_MapX.SetCenterX(line->iLineKmLonLat[centerPoint]->iLon);
	m_RWDSClientView->m_MapX.SetCenterY(line->iLineKmLonLat[centerPoint]->iLat);
	m_RWDSClientView->m_MapX.SetZoom(m_RWDSClientView->m_InitZoom/256);
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
		
		HTREEITEM tmpChild;
		if(curOrg->iChildOrg.size() > 0)
		{//��������
			OrganizationInfo* org;
			for(size_t i=0; i<curOrg->iChildOrg.size(); i++)
			{
				org = curOrg->iChildOrg[i];
				tmpChild = m_wndFileView.InsertItem(org->iOrgName, 0, 0, curItem);
				m_wndFileView.SetItemData(tmpChild, (DWORD_PTR)org);
				m_wndFileView.InsertItem(_T(""), 0, 0, tmpChild);//Ϊ����ʾ+��
			}
		}
		else if (curOrg->iLine.size() > 0)
		{//������·
			LineInfo* line;
			CString str;
			for(size_t i=0; i<curOrg->iLine.size(); i++)
			{
				line = curOrg->iLine[i];
				tmpChild = m_wndFileView.InsertItem(line->iLineName, 1, 1, curItem);
				m_wndFileView.SetItemData(tmpChild, (DWORD_PTR)line);
			}
		}
	}
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
		if (m_wndFileView.GetItemText(childItem).Compare(_T("")) != 0)
		{//��item�Ѿ�չ����
			
			return;
		}
		OrganizationInfo* curOrg = (OrganizationInfo*) m_wndFileView.GetItemData(curItem);
		///////////////////////////�������ȡ����////////////////////////////////////////
		/*
		OrganizationInfo* org= new OrganizationInfo;
		org->iOrgID = 2;
		org->iOrgName = curOrg->iOrgName + _T("Child1");
		org->iParentOrg = curOrg;
		org->iChildID.push_back(3);

		curOrg->iChildOrg.push_back(org);

		org= new OrganizationInfo;
		org->iOrgID = 4;
		org->iOrgName = curOrg->iOrgName + _T("Child2");
		org->iParentOrg = curOrg;
		org->iChildID.push_back(5);

		curOrg->iChildOrg.push_back(org);*/

		///////////////////////////�������ȡ����////////////////////////////////////////

		//DeviceInfo* dev = new DeviceInfo;
		//dev->iDevID = 123456;
		//curOrg->iDevice.push_back(dev);
		//dev = new DeviceInfo;
		//dev->iDevID = 44444;
		//curOrg->iDevice.push_back(dev);
		//dev = new DeviceInfo;
		//dev->iDevID = 55555;
		//curOrg->iDevice.push_back(dev);
		for(size_t i=0; i<m_RWDSClientView->m_Line.size(); i++)
			curOrg->iLine.push_back(m_RWDSClientView->m_Line[i]);
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
