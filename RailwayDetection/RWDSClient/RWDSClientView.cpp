
// RWDSClientView.cpp : CRWDSClientView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "RWDSClient.h"
#endif

#include "RWDSClientDoc.h"
#include "RWDSClientView.h"
#include "PointList.h"
#include "LineList.h"
#include "Schedule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum MapTool
{
	miAddSymbol = 1,		//����ͼ��1
	miDeleteSymbol = 4,		//ɾ�����
	miMoveSymbol = 5		//�ƶ����
};

// CRWDSClientView

IMPLEMENT_DYNCREATE(CRWDSClientView, CView)

BEGIN_MESSAGE_MAP(CRWDSClientView, CView)
	// ��׼��ӡ����
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
ON_COMMAND(ID_SET_LINE, &CRWDSClientView::OnSetLine)
ON_COMMAND(ID_SET_SCHEDULE, &CRWDSClientView::OnSetSchedule)
END_MESSAGE_MAP()

// CRWDSClientView ����/����

CRWDSClientView::CRWDSClientView()
{
	// TODO: �ڴ˴����ӹ������
	m_SymbolLayer = "SymbolLayer";
	m_MapName = "CHINA-m.GST";
}

CRWDSClientView::~CRWDSClientView()
{
}

BOOL CRWDSClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CRWDSClientView ����

void CRWDSClientView::OnDraw(CDC* /*pDC*/)
{
	CRWDSClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ�����������ӻ��ƴ���
}


// CRWDSClientView ��ӡ


void CRWDSClientView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CRWDSClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CRWDSClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CRWDSClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӵ�ӡ����е���������
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


// CRWDSClientView ���

#ifdef _DEBUG
void CRWDSClientView::AssertValid() const
{
	CView::AssertValid();
}

void CRWDSClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRWDSClientDoc* CRWDSClientView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRWDSClientDoc)));
	return (CRWDSClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CRWDSClientView ��Ϣ��������


int CRWDSClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ�������ר�õĴ�������
	if(!m_MapX.Create(NULL, WS_VISIBLE, CRect(0,0,100,100), this, IDC_MAP))
	{
		return -1;
	}
	try 
	{	
		m_MapX.SetGeoSet(m_MapName);		//ָ����ͼ��
		m_MapX.SetTitleText(_T(""));

		m_InitCenterX=m_MapX.GetCenterX();
		m_InitCenterY=m_MapX.GetCenterY();
		m_InitZoom=m_MapX.GetZoom();
		if (m_InitZoom <= 0)
		{
			MessageBox(_T("δָ����ͼ������ȷ�ϵ�ͼ�����Ƿ���ȷ��װ��"));
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

	////////////////////////////////////////////////////user data
	MapPoint *pt = new MapPoint;
	pt->iRailLine = Baoji_Chengdu; 
	pt->iKM = 251;
	pt->iLon = 103.114;
	pt->iLat = 30.54741256;
	pt->iDirect = KDownLine;
	m_MapPoint.push_back(pt);

	pt = new MapPoint;
	pt->iRailLine = Chengdu_Chongqing; 
	pt->iKM = 252;
	pt->iLon = 103.114;
	pt->iLat = 30.54741256;
	pt->iDirect = KUpLine;
	m_MapPoint.push_back(pt);

	pt = new MapPoint;
	pt->iRailLine = Baoji_Chengdu; 
	pt->iKM = 253;
	pt->iLon = 103.114;
	pt->iLat = 30.54741256;
	pt->iDirect = KDownLine;
	m_MapPoint.push_back(pt);

	pt = new MapPoint;
	pt->iRailLine = Chengdu_Chongqing; 
	pt->iKM = 254;
	pt->iLon = 103.114;
	pt->iLat = 30.54741256;
	pt->iDirect = KUpLine;
	m_MapPoint.push_back(pt);

	LineInfo *line = new LineInfo;
	line->iLineID = 1;
	line->iLineName = _T("�ɾ�1��");
	line->iStartKm = m_MapPoint[0]->iKM;
	line->iLineKmLonLat.push_back(m_MapPoint[0]);
	line->iLineKmLonLat.push_back(m_MapPoint[2]);
	m_Line.push_back(line);

	line = new LineInfo;
	line->iLineID = 2;
	line->iLineName = _T("�ɾ�2��");
	line->iStartKm = m_MapPoint[1]->iKM;
	line->iLineKmLonLat.push_back(m_MapPoint[1]);
	line->iLineKmLonLat.push_back(m_MapPoint[3]);
	m_Line.push_back(line);



	ScheduleLine* sc = new ScheduleLine;
	sc->iScheduleID = 1;
	sc->iLine = m_Line[0];
	sc->iScheduleName = _T("1�����");
	sc->iULineKmTime.push_back(10545020);
	sc->iULineKmTime.push_back(10554900);
	Worker* wk = new Worker;
	wk->iID = 1;
	wk->iName = _T("ְ��1");
	DeviceInfo* dv = new DeviceInfo;
	dv->iDevID = 1254;
	sc->iWorker = wk;
	sc->iDevice = dv;
	m_Schedule.push_back(sc);

	sc = new ScheduleLine;
	sc->iScheduleID = 1;
	sc->iLine = m_Line[1];
	sc->iScheduleName = _T("2�����");
	sc->iULineKmTime.push_back(10545020);
	sc->iULineKmTime.push_back(10554900);
	wk = new Worker;
	wk->iID = 2;
	wk->iName = _T("ְ��2");
	dv = new DeviceInfo;
	dv->iDevID = 1255;
	sc->iWorker = wk;
	sc->iDevice = dv;
	m_Schedule.push_back(sc);

	//////////////////////////////////////////////////
	return 0;
}


void CRWDSClientView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴�������Ϣ�����������
	m_MapX.MoveWindow(0, 0, cx, cy, TRUE);
}

void CRWDSClientView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: �ڴ˴�������Ϣ�����������
	m_MapX.SetFocus();
}


void CRWDSClientView::OnMapPan()
{
	// TODO: �ڴ�����������������
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
	// TODO: �ڴ�������������û����洦���������
	BOOL bCheck;
	if(miPanTool==m_MapX.GetCurrentTool())
		bCheck=TRUE;
	else
		bCheck=FALSE;
	pCmdUI->SetCheck(bCheck);
}


void CRWDSClientView::OnMapZoomin()
{
	// TODO: �ڴ�����������������
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
	// TODO: �ڴ�������������û����洦���������
	BOOL bCheck;
	if(miZoomInTool==m_MapX.GetCurrentTool())
		bCheck=TRUE;
	else
		bCheck=FALSE;
	pCmdUI->SetCheck(bCheck);
}


void CRWDSClientView::OnMapZoomout()
{
	// TODO: �ڴ�����������������
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
	// TODO: �ڴ�������������û����洦���������
	BOOL bCheck;
	if(miZoomOutTool==m_MapX.GetCurrentTool())
		bCheck=TRUE;
	else
		bCheck=FALSE;
	pCmdUI->SetCheck(bCheck);
}

void CRWDSClientView::OnSymbolAdd()
{
	// TODO: �ڴ�����������������
}

void CRWDSClientView::OnSymbolDelete()
{
	// TODO: �ڴ�����������������
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

		m_MapX.Refresh();		//��Ļ��ʾˢ��
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
	// TODO: �ڴ�����������������
		
	CPointList setPoint(this);
	setPoint.DoModal();
}

void CRWDSClientView::OnSetLine()
{
	// TODO: �ڴ�����������������
	CLineList lineList(this);
	lineList.DoModal();
}


void CRWDSClientView::OnSetSchedule()
{
	// TODO: �ڴ�����������������
	CSchedule schedule(this);
	schedule.DoModal();
}