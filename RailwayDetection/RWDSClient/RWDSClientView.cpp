
// RWDSClientView.cpp : CRWDSClientView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "RWDSClient.h"
#endif

#include "RWDSClientDoc.h"
#include "RWDSClientView.h"
#include "PointList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum MapTool
{
	miAddSymbol = 1,		//添加图标1
	miDeleteSymbol = 4,		//删除标记
	miMoveSymbol = 5		//移动标记
};

// CRWDSClientView

IMPLEMENT_DYNCREATE(CRWDSClientView, CView)

BEGIN_MESSAGE_MAP(CRWDSClientView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRWDSClientView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
ON_WM_SETFOCUS()
ON_COMMAND(ID_MAP_PAN, &CRWDSClientView::OnMapPan)
ON_UPDATE_COMMAND_UI(ID_MAP_PAN, &CRWDSClientView::OnUpdateMapPan)
ON_COMMAND(ID_MAP_ZOOMIN, &CRWDSClientView::OnMapZoomin)
ON_UPDATE_COMMAND_UI(ID_MAP_ZOOMIN, &CRWDSClientView::OnUpdateMapZoomin)
ON_COMMAND(ID_MAP_ZOOMOUT, &CRWDSClientView::OnMapZoomout)
ON_UPDATE_COMMAND_UI(ID_MAP_ZOOMOUT, &CRWDSClientView::OnUpdateMapZoomout)
ON_COMMAND(ID_SYMBOL_ADD, &CRWDSClientView::OnSymbolAdd)
ON_COMMAND(ID_SYMBOL_DELETE, &CRWDSClientView::OnSymbolDelete)
ON_COMMAND(ID_SET_POINT, &CRWDSClientView::OnSetPoint)
END_MESSAGE_MAP()

// CRWDSClientView 构造/析构

CRWDSClientView::CRWDSClientView()
{
	// TODO: 在此处添加构造代码
	m_SymbolLayer = "SymbolLayer";
	m_MapName = "CHINA-m.GST";
}

CRWDSClientView::~CRWDSClientView()
{
}

BOOL CRWDSClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CRWDSClientView 绘制

void CRWDSClientView::OnDraw(CDC* /*pDC*/)
{
	CRWDSClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CRWDSClientView 打印


void CRWDSClientView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CRWDSClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CRWDSClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CRWDSClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CRWDSClientView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRWDSClientView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
//#ifndef SHARED_HANDLERS
	m_RightClkPoint = point;
	BOOL bo = theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_MAPX, point.x, point.y, this, TRUE);
	//bo = theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//#endif
}


// CRWDSClientView 诊断

#ifdef _DEBUG
void CRWDSClientView::AssertValid() const
{
	CView::AssertValid();
}

void CRWDSClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRWDSClientDoc* CRWDSClientView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRWDSClientDoc)));
	return (CRWDSClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CRWDSClientView 消息处理程序


int CRWDSClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if(!m_MapX.Create(NULL, WS_VISIBLE, CRect(0,0,100,100), this, IDC_MAP))
	{
		return -1;
	}
	try 
	{	
		m_MapX.SetGeoSet(m_MapName);		//指定地图集
		m_MapX.SetTitleText(_T(""));

		m_InitCenterX=m_MapX.GetCenterX();
		m_InitCenterY=m_MapX.GetCenterY();
		m_InitZoom=m_MapX.GetZoom();
		if (m_InitZoom <= 0)
		{
			MessageBox(_T("未指定地图集，请确认地图数据是否正确安装。"));
		}

		m_MapX.CreateCustomTool(miAddSymbol, 0, miArrowCursor);	
		m_MapX.CreateCustomTool(miDeleteSymbol, 0, miArrowCursor);
		m_MapX.CreateCustomTool(miMoveSymbol, 0, miArrowCursor);
		m_MapX.SetMousewheelSupport(miFullMousewheelSupport);
		m_MapX.GetLayers().Item(m_SymbolLayer).SetAutoLabel(true);
	}
	catch (COleDispatchException *e) 
	{
		e->ReportError();
		e->Delete();
	} 
	catch (COleException *e)
	{
		e->ReportError();
		e->Delete();
	}

	m_MapX.SetRedrawInterval(2000);
	return 0;
}


void CRWDSClientView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_MapX.MoveWindow(0, 0, cx, cy, TRUE);
}

void CRWDSClientView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	m_MapX.SetFocus();
}


void CRWDSClientView::OnMapPan()
{
	// TODO: 在此添加命令处理程序代码
	try 
	{
		if(miPanTool==m_MapX.GetCurrentTool())
			m_MapX.SetCurrentTool(miArrowTool);
		else
			m_MapX.SetCurrentTool(miPanTool);
	} 
	catch (COleDispatchException *e) 
	{
		e->ReportError();
		e->Delete();
	} 
	catch (COleException *e)
	{
		e->ReportError();
		e->Delete();
	}
}


void CRWDSClientView::OnUpdateMapPan(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	BOOL bCheck;
	if(miPanTool==m_MapX.GetCurrentTool())
		bCheck=TRUE;
	else
		bCheck=FALSE;
	pCmdUI->SetCheck(bCheck);
}


void CRWDSClientView::OnMapZoomin()
{
	// TODO: 在此添加命令处理程序代码
	try 
	{
		if(miZoomInTool==m_MapX.GetCurrentTool())
		{
			m_MapX.SetCurrentTool(miArrowTool);
			//m_MapX.SetMousewheelSupport(miNoMousewheelSupport);
		}
		else
		{
			//m_MapX.GetLayers().SetInsertionLayer(m_MapX.GetLayers().Item(m_SymbolLayer));
			m_MapX.SetCurrentTool(miZoomInTool);
			//m_MapX.SetMousewheelSupport(miFullMousewheelSupport);
		}
	} 
	catch (COleDispatchException *e) 
	{
		e->ReportError();
		e->Delete();
	} 
	catch (COleException *e)
	{
		e->ReportError();
		e->Delete();
	}
}


void CRWDSClientView::OnUpdateMapZoomin(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	BOOL bCheck;
	if(miZoomInTool==m_MapX.GetCurrentTool())
		bCheck=TRUE;
	else
		bCheck=FALSE;
	pCmdUI->SetCheck(bCheck);
}


void CRWDSClientView::OnMapZoomout()
{
	// TODO: 在此添加命令处理程序代码
	try 
	{
		if(miZoomOutTool==m_MapX.GetCurrentTool())
		{
			m_MapX.SetCurrentTool(miArrowTool);
			//m_MapX.SetMousewheelSupport(miNoMousewheelSupport);
		}
		else
		{
			m_MapX.SetCurrentTool(miZoomOutTool);
			//m_MapX.SetMousewheelSupport(miFullMousewheelSupport);
		}
	} 
	catch (COleDispatchException *e) 
	{
		e->ReportError();
		e->Delete();
	} 
	catch (COleException *e)
	{
		e->ReportError();
		e->Delete();
	}
}


void CRWDSClientView::OnUpdateMapZoomout(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	BOOL bCheck;
	if(miZoomOutTool==m_MapX.GetCurrentTool())
		bCheck=TRUE;
	else
		bCheck=FALSE;
	pCmdUI->SetCheck(bCheck);
}

void CRWDSClientView::OnSymbolAdd()
{
	// TODO: 在此添加命令处理程序代码
}

void CRWDSClientView::OnSymbolDelete()
{
	// TODO: 在此添加命令处理程序代码
	CMapXLayer layer;
	CMapXFeature Ftr;
	CMapXFeatures Ftrs;
	int iFtrNum;

	try 
	{
		layer=m_MapX.GetLayers().Item(m_SymbolLayer);
		Ftrs=layer.AllFeatures();
		iFtrNum=Ftrs.GetCount();
		for(int i=1; i<=iFtrNum; i++)
		{
			Ftr=Ftrs.Item(i);
			layer.DeleteFeature(Ftr.GetFeatureKey());
		}

		m_MapX.Refresh();		//屏幕显示刷新
	}
	catch (COleDispatchException *e) {
		e->ReportError();
		e->Delete();
	}
	catch (COleException *e) {
		e->ReportError();
		e->Delete();
	}
}


void CRWDSClientView::OnSetPoint()
{
	// TODO: 在此添加命令处理程序代码
	CPointList setPoint;
	setPoint.DoModal();
}
