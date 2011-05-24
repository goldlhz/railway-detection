
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
#include "MainFrm.h"
#include "StaticData.h"
#include "StaffList.h"
#include "EmergencyTask.h"
#include "RecordStaff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CRWDSClientView* gClientView = NULL;
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
	ON_WM_TIMER()
    ON_COMMAND(ID_SET_STAFF, &CRWDSClientView::OnSetStaff)
    ON_COMMAND(ID_SET_EMERGENCYTASK, &CRWDSClientView::OnSetEmergencytask)
//    ON_COMMAND(ID_REVIEW_RECORD, &CRWDSClientView::OnReviewRecord)
    ON_COMMAND(ID_REVIEW_RECORDDEVICE, &CRWDSClientView::OnReviewRecorddevice)
    ON_COMMAND(ID_REVIEW_RECORDSTAFF, &CRWDSClientView::OnReviewRecordstaff)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CRWDSClientView, CView)
	//	ON_EVENT(MapMarkView, IDC_MAP, DISPID_DBLCLICK, OnDblClick,NULL)
	ON_EVENT(CRWDSClientView, IDC_MAP, DISPID_MOUSEMOVE, OnMouseMove_Map, VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS)
	//ON_EVENT(CRWDSClientView, IDC_MAP, DISPID_MOUSEDOWN, OnMouseDown_Map, VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS)
	//ON_EVENT(CRWDSClientView, IDC_MAP, DISPID_MOUSEUP, OnMouseUp_Map, VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS)
	//ON_EVENT(CRWDSClientView, IDC_MAP, MAPX_DISPID_TOOLUSED, OnToolUsed, VTS_I2 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_BOOL VTS_BOOL VTS_PBOOL)
	ON_EVENT(CRWDSClientView, IDC_MAP, 6 /* MapViewChanged */, OnMapViewChangedMap, VTS_NONE)
	//	ON_EVENT(CMapMarkView, IDC_MAP, 11 /* PolyToolUsed */, OnPolyToolUsedMap, VTS_I2 VTS_I4 VTS_DISPATCH VTS_BOOL VTS_BOOL VTS_PBOOL)
END_EVENTSINK_MAP()
// CRWDSClientView ����/����

CRWDSClientView::CRWDSClientView()
{
	// TODO: �ڴ˴����ӹ������
	m_SymbolLayer = "SymbolLayer";
	m_TrackLayer = "TrackLayer";
	m_MapName = "RailwayMap.GST";
    m_RecordStaff = NULL;
    m_DisplayFlag = KNone;
	m_Calendar = new CalendarSchedule;
	m_Calendar->iDateSchedule = &m_Line;
	m_Calendar->iStartDay = 1288915200;
	m_Calendar->iPeriods = 3;
}

CRWDSClientView::~CRWDSClientView()
{
	delete m_Calendar;
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
	//BOOL bo = theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_MAPX, point.x, point.y, this, TRUE);
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
	gClientView = this;
	// TODO:  �ڴ�������ר�õĴ�������
	CString strLic = _T("uQnZi2sFw22L0-MRa8pYX-2E6P1077-3N6M0499-5C038223-10884-12802-36882-8029");
	BSTR bstrLic = strLic.AllocSysString();
	if(!m_MapX.Create(NULL, WS_VISIBLE, CRect(0,0,100,100), this, IDC_MAP, NULL, FALSE, bstrLic))
	{
		return -1;
	}
	::SysFreeString(bstrLic); 
	//if(!m_MapX.Create(NULL, WS_VISIBLE, CRect(0,0,100,100), this, IDC_MAP))
	//{
	//	return -1;
	//}
	try 
	{	
		CString curDir;
		curDir = GetModulePath();

		CString mapPath = curDir + _T("\\map\\") + m_MapName;
		//m_MapX.geoset(_T(".\\map"));
		m_MapX.SetGeoSet(mapPath);		//ָ����ͼ��
		m_MapX.SetTitleText(_T(""));
		m_MapX.SetPanAnimationLayer(TRUE); 

		CMapXLayers layers=m_MapX.GetLayers();
		CMapXLayer layer;
		BOOL flagSymbol=FALSE;
		BOOL flagTrack = FALSE;
		int layerSymbolIndex = 0;
		int layerTrackIndex = 0;
		for(int i=0; i<layers.GetCount(); i++)
		{
			layer=layers.Item(i+1);
			if(layer.GetName() == m_SymbolLayer) 
			{
				flagSymbol=TRUE; 
				layerSymbolIndex = i;
				//break;
			}
			else if (layer.GetName() == m_TrackLayer)
			{
				flagTrack = TRUE;
				layerTrackIndex = i;
			}
		}
		//û��tempLayerͼ�㣬���½�
		if (!flagSymbol)
		{
			layer=m_MapX.GetLayers().CreateLayer(m_SymbolLayer);
			m_MapX.GetLayers().SetAnimationLayer(layer); //��Ϊ��̬ͼ��  
			layerSymbolIndex = 0;
		}

		m_MapX.GetLayers().Move(layerSymbolIndex+1, 1);

		if (!flagTrack)
		{
			layer=m_MapX.GetLayers().CreateLayer(m_TrackLayer);
			m_MapX.GetLayers().SetAnimationLayer(layer); //��Ϊ��̬ͼ��  
			layerTrackIndex = 0;
		}
		m_MapX.GetLayers().Move(layerTrackIndex+1, 1);

		m_InitCenterX=m_MapX.GetCenterX();
		m_InitCenterY=m_MapX.GetCenterY();
		//m_InitZoom=m_MapX.GetZoom();
		m_InitZoom = 4096;
		m_MapX.SetZoom(4096);
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
	m_FileView = &((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetFileView();
	//m_FileView->SetRWDSClientView(this);
	//m_FileView->FillFileView();

	//SetTimer(TIMERNTRACK, 500, NULL);


	////////////////////////////////////////////////////user data
	MapPoint *pt = new MapPoint;
	pt->iRailLine = Chengdu_Chongqing; 
	pt->iKM = 251;
	pt->iLon = 104.064531;
	pt->iLat = 30.699965;
	pt->iDirect = KUpLine;
	m_MapPoint.push_back(pt);

	pt = new MapPoint;
	pt->iRailLine = Chengdu_Chongqing; 
	pt->iKM = 252;
	pt->iLon = 104.075530;
	pt->iLat = 30.699484;
	pt->iDirect = KUpLine;
	m_MapPoint.push_back(pt);

	pt = new MapPoint;
	pt->iRailLine = Chengdu_Chongqing; 
	pt->iKM = 253;
	pt->iLon = 104.086526;
	pt->iLat = 30.699484;
	pt->iDirect = KUpLine;
	m_MapPoint.push_back(pt);

	pt = new MapPoint;
	pt->iRailLine = Chengdu_Chongqing; 
	pt->iKM = 254;
	pt->iLon = 104.097521;
	pt->iLat = 30.699484;
	pt->iDirect = KUpLine;
	m_MapPoint.push_back(pt);

	LineInfo *line = new LineInfo;
	line->iLineID = 1;
	line->iLineName = _T("�ɾ�1��");
	line->iStartKm = m_MapPoint[0]->iKM;
	line->iStartNo = KFirstDay;
	line->iLineKmLonLat.push_back(m_MapPoint[0]);
	line->iLineKmTime.push_back(100);
	line->iLineKmLonLat.push_back(m_MapPoint[1]);
	line->iLineKmTime.push_back(100);
	line->iLineKmLonLat.push_back(m_MapPoint[2]);
	line->iLineKmTime.push_back(100);
	line->iLineKmLonLat.push_back(m_MapPoint[3]);
	line->iLineKmTime.push_back(100);
	m_Line.push_back(line);

	line = new LineInfo;
	line->iLineID = 2;
	line->iLineName = _T("�ɾ�2��");
	line->iStartKm = m_MapPoint[1]->iKM;
	line->iStartNo = KThirdDay;
	line->iLineKmLonLat.push_back(m_MapPoint[1]);
	line->iLineKmTime.push_back(0);
	line->iLineKmLonLat.push_back(m_MapPoint[3]);
	line->iLineKmTime.push_back(0);
	m_Line.push_back(line);

	OrganizationInfo* org = new OrganizationInfo;
	org->iOrgName = _T("Admin");
	org->iParentOrg = NULL;
	org->iOrgID = 1;
	org->iChildID.push_back(2);
	m_Org.push_back(org);

    StaffInfo* staff = new StaffInfo;
    staff->iID = 1;
    staff->iOrgID = 1;
    staff->iPassword = _T("111");
    staff->iLoginPermission = TRUE;
    staff->iName = _T("����");
    m_Staff.push_back(staff);

    staff = new StaffInfo;
    staff->iID = 2;
    staff->iOrgID = 1;
    staff->iPassword = _T("");
    staff->iLoginPermission = FALSE;
    staff->iName = _T("����");
    m_Staff.push_back(staff);

    staff = new StaffInfo;
    staff->iID = 3;
    staff->iOrgID = 1;
    staff->iName = _T("����");
    staff->iPassword = _T("");
    staff->iLoginPermission = FALSE;
    m_Staff.push_back(staff);
	//////////////////////////////////////////////////

	MapxCleanAllFeature(m_SymbolLayer);
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

CString CRWDSClientView::GetModulePath()
{
	CString modDir;
	GetModuleFileName(NULL, modDir.GetBuffer(MAX_PATH), MAX_PATH); 
	modDir.ReleaseBuffer();
	int pos;   
	pos=modDir.ReverseFind('\\');   
	modDir=modDir.Left(pos);  
	return modDir;
}

void CRWDSClientView::DecimalGeoToStandardGeo(double dX, double dY, int *iXd, int *iXm, int *iXs, int *iYd, int *iYm, int *iYs)
{
	double dLongitude,dLatitude,dTemp;
	dLongitude=dX;
	dLatitude=dY;
	dTemp=dLongitude-(int)(dLongitude);
	*iXd=int(dLongitude);
	*iXm=int(dTemp*60);
	*iXs=int((dTemp*60-int(dTemp*60))*60);
	dTemp=dLatitude-(int)(dLatitude);
	*iYd=int(dLatitude);
	*iYm=int(dTemp*60);
	*iYs=int((dTemp*60-int(dTemp*60))*60);
}

void CRWDSClientView::MapxDrawCircle(double aMapLon, double aMapLat, CString aLayerName, ColorConstants aColor)
{
	CMapXPoint point;
	point.CreateDispatch(point.GetClsid());
	point.Set(aMapLon, aMapLat);
	CMapXFeature ft;
	ft=m_MapX.GetFeatureFactory().CreateCircularRegion(miCircleTypeScreen, point, 0.0003, miUnitDegree,100);
	CMapXStyle style;
	style = ft.GetStyle();
	style.SetRegionColor(aColor);
	style.SetRegionBorderStyle(0);//�ޱ߿�
	m_MapX.GetLayers().Item(aLayerName).AddFeature(ft);
}

void CRWDSClientView::MapxDrawLine(double aMapLon1, double aMapLat1, double aMapLon2, double aMapLat2)
{
	CMapXPoints points;
	points.CreateDispatch(points.GetClsid());
	//point.Set(aMapLon1, aMapLat1);
	points.AddXY(aMapLon1, aMapLat1);
	points.AddXY(aMapLon2, aMapLat2);

	COleVariant vtPoints;
	vtPoints.vt=VT_DISPATCH;
	vtPoints.pdispVal=points.m_lpDispatch;
	vtPoints.pdispVal->AddRef();
	CMapXFeature ft;
	ft=m_MapX.GetFeatureFactory().CreateLine(vtPoints);
	CMapXStyle style;
	style = ft.GetStyle();
	style.SetLineWidth(2);
	style.SetLineColor(miColorBlue);
	m_MapX.GetLayers().Item(m_SymbolLayer).AddFeature(ft);
	//m_MapX.GetLayers().Item(m_SymbolLayer).Refresh();
}

void CRWDSClientView::MapxSetText(double aMapLon, double aMapLat, CString aText)
{
	try 
	{
		CMapXLayer layer;
		CMapXFeatureFactory ftFy;
		CMapXFeature ftr;
		COleVariant pntVt;
		CY			fontSize;
		fontSize.Hi = 0x00000;
		fontSize.Lo = 0x28000;

		layer=m_MapX.GetLayers().Item(m_SymbolLayer);

		CMapXFeature    m_fMarkText;        //�������ͼԪ
		CMapXFeature    m_fMarkSymbol;		//��Ƿ���ͼԪ
		CMapXPoint		m_ptMarkPoint;		//��Ƕ�λ��

		m_ptMarkPoint.CreateDispatch(m_ptMarkPoint.GetClsid());
		m_ptMarkPoint.Set(aMapLon, aMapLat);

		pntVt.vt = VT_DISPATCH;
		pntVt.pdispVal = m_ptMarkPoint.m_lpDispatch;
		pntVt.pdispVal->AddRef();

		ftFy=m_MapX.GetFeatureFactory();

		//����bmpͼ��
		ftr=ftFy.CreateSymbol(pntVt);
		//Ftr.GetStyle().SetSymbolType(miSymbolTypeBitmap);
		//Ftr.GetStyle().SetSymbolBitmapSize(16);
		//Ftr.GetStyle().SetSymbolBitmapTransparent(TRUE);	
		//Ftr.GetStyle().SetSymbolBitmapName(m_strBmpFileName);
		ftr.GetStyle().SetSymbolCharacter(0);
		layer.GetLabelProperties().GetStyle().SetTextFontColor(miColorBlack);
		layer.GetLabelProperties().GetStyle().GetTextFont().SetSize(fontSize);
		layer.GetLabelProperties().GetStyle().GetTextFont().SetBold(FALSE);
		//layer.GetLabel().SetOffset(5);
		//layer.GetLabel().SetPosition(5);
		ftr.SetKeyValue(aText);
		m_fMarkSymbol = layer.AddFeature(ftr);  
	}
	catch (COleDispatchException *e) 
	{
		e->ReportError();
		e->Delete();
	}
	catch (COleException *e) {
		e->ReportError();
		e->Delete();
	}
}

void CRWDSClientView::MapxCleanAllFeature(CString aLayerName)
{
	CMapXLayer layer;
	CMapXFeature Ftr;
	CMapXFeatures Ftrs;
	int featureCount;

	layer = m_MapX.GetLayers().Item(aLayerName);
	Ftrs = layer.AllFeatures();
	featureCount = layer.AllFeatures().GetCount();

	for (int i=1; i<=featureCount; i++)
	{
		Ftr=Ftrs.Item(i);
		layer.DeleteFeature(Ftrs.Item(i).GetFeatureKey());
	}	

}

void CRWDSClientView::OnMouseMove_Map(short Button, short Shift, long X, long Y)
{
	CString sCoordinate;

	float singleX = (float)X;
	float singleY = (float)Y;
	int iXd;
	int iXm;
	int iXs;
	int iYd;
	int iYm;
	int iYs;

	try
	{
		CMainFrame* mainFrame = (CMainFrame *)GetParentFrame();		//��ȡ�����ھ��

		m_MapX.ConvertCoord(&singleX, &singleY, &m_MouseLon, &m_MouseLat, miScreenToMap);	//��Ļ����ת��Ϊ��ͼ����
		DecimalGeoToStandardGeo(m_MouseLon,m_MouseLat,&iXd,&iXm,&iXs,&iYd,&iYm,&iYs);			//С������==>��׼��������
		sCoordinate.Format(_T("����:%d��%d��%d\" ,γ��:%d��%d��%d\" "),iXd,iXm,iXs,iYd,iYm,iYs);	//��״̬������ʾ��ǰ����         
		mainFrame->m_wndStatusBar.SetPaneText(mainFrame->m_wndStatusBar.CommandToIndex(ID_INDICATOR_COORDINATE) ,sCoordinate);
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

	//if (m_SymbolMove)
	//{
	//	try 
	//	{
	//		CMapXFeature Ftr;
	//		CMapXFeatures Ftrs;
	//		CMapXPoint   ps;
	//		int iFeatureCount;
	//		CString strKeyValue;
	//		ps.CreateDispatch(ps.GetClsid());
	//		ps.Set(lon,lat);

	//		Ftrs = m_Selectlayer.AllFeatures();
	//		iFeatureCount = m_Selectlayer.AllFeatures().GetCount();

	//		Ftr=Ftrs.Item(1);
	//		if (Ftr.GetKeyValue() == m_strSelectKey)
	//		{
	//			Ftr.SetPoint(ps);	//�ı䶨λ��
	//			Ftr.Update();
	//		}		
	//	}
	//	catch (COleDispatchException *e) 
	//	{
	//		e->ReportError();
	//		e->Delete();
	//	}
	//	catch (COleException *e) 
	//	{
	//		e->ReportError();
	//		e->Delete();
	//	}
	//}
}

void CRWDSClientView::OnMapViewChangedMap() 
{
	double dZoom;
	double dPaperWidth;
	double dScale;
	long lScale;

	CMainFrame* mainFrame = (CMainFrame *)GetParentFrame();		//��ȡ�����ھ��

	dZoom=m_MapX.GetZoom();
	dPaperWidth=m_MapX.GetMapPaperWidth();
	dScale=dZoom/dPaperWidth*100000;
	lScale=(long)dScale;

	if (mainFrame->m_wndStatusBar.m_hWnd == NULL)
	{
		return;
	}
	CString strScale;
	strScale.Format(_T("�����ߣ�1:%ld"),lScale);
	mainFrame->m_wndStatusBar.SetPaneText(mainFrame->m_wndStatusBar.CommandToIndex(ID_INDICATOR_SCALE),strScale);
}

void CRWDSClientView::OnSymbolAdd()
{
	// TODO: �ڴ�����������������
	CPointList pointDlg(this);
	pointDlg.SetDefaltPoint(m_MouseLon, m_MouseLat);
	pointDlg.DoModal();
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


void CRWDSClientView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ�����������������
	static BOOL displayLastPoint = TRUE;
	switch(nIDEvent)
	{
	case TIMERNTRACK:
		//��ȡ��·�ĵ�ǰѲ��λ��
		//for(size_t i=0; i<m_CurrentLinePosition.size(); i++)
		//{
		//	//GetCurrentPosition(m_CurrentLinePosition[i]->iLineID, m_CurrentLinePosition[i]);
		//}
		if (m_MapPoint.size() == 0)
		{
			break;
		}
		MapPoint* point;
		
		MapxCleanAllFeature(m_TrackLayer);
		size_t i;
		for (i=0; i<m_MapPoint.size()-1; i++)
		{
			point =m_MapPoint[i];
			MapxDrawCircle(point->iLon-0.001, point->iLat-0.001, m_TrackLayer, miColorYellow);
		}
		if (displayLastPoint)
		{
			point = m_MapPoint[i];
			MapxDrawCircle(point->iLon-0.001, point->iLat-0.001, m_TrackLayer, miColorYellow);
			displayLastPoint = FALSE;
		}
		else
			displayLastPoint = TRUE;
		break;
	default:
		break;
	}
	CView::OnTimer(nIDEvent);
}


void CRWDSClientView::OnSetStaff()
{
    // TODO: �ڴ�����������������
    CStaffList setStaff(this);
    setStaff.DoModal();
}


void CRWDSClientView::OnSetEmergencytask()
{
    // TODO: �ڴ�����������������
    CEmergencyTask task(this);
    task.DoModal();
}


void CRWDSClientView::OnReviewRecorddevice()
{
    // TODO: �ڴ�����������������
}


void CRWDSClientView::OnReviewRecordstaff()
{
    // TODO: �ڴ�����������������
    m_DisplayFlag = KStaffLog;
    CRecordStaff rstaff(this);
    if(rstaff.DoModal() == IDOK)
    {
        /////////////////////////////
        if (!m_RecordStaff)
        {
            m_RecordStaff = new RecordStaff;
            if(m_Staff.size()>0)
                m_RecordStaff->iStaff = m_Staff[0];
            MapPoint* point;
            for(int i=0; i<3; i++)
            {
                point = new MapPoint;
                point->iDirect = m_MapPoint[i]->iDirect;
                point->iKM = m_MapPoint[i]->iKM;
                point->iLon = m_MapPoint[i]->iLon - 0.0001;
                point->iLat = m_MapPoint[i]->iLat + 0.0001;
                point->iRailLine = m_MapPoint[i]->iRailLine;
                m_RecordStaff->iRecordPoint.push_back(point);
            }
        }
        /////////////////////////////
        int recordSelect = rstaff.GetSelect();
        m_RecordStaff->iStaff = m_Staff[recordSelect];
        MapxCleanAllFeature(m_SymbolLayer);
        MapxCleanAllFeature(m_TrackLayer);
        MapPoint* point = NULL;
        LineInfo* line = NULL;
        double centerX = 0.0;
        double centerY = 0.0;
        for (size_t i=0; i<m_RecordStaff->iStaff->iArrangeLine.size(); i++)
        {//��ʾ��Ա������Ƶ�·��
            line = m_RecordStaff->iStaff->iArrangeLine[i];
            for (size_t j=0; j<line->iLineKmLonLat.size(); j++)
            {
                point = line->iLineKmLonLat[j];
                MapxDrawCircle(point->iLon, point->iLat, m_SymbolLayer);
                if (centerX == 0 || centerY == 0)
                {
                    centerX = point->iLon;
                    centerY = point->iLat;
                }
            }
        }
        for (size_t i=0; i<m_RecordStaff->iRecordPoint.size(); i++)
        {
            point = m_RecordStaff->iRecordPoint[i];
            MapxDrawCircle(point->iLon, point->iLat, m_TrackLayer, miColorBlue);
            if (centerX == 0 || centerY == 0)
            {
                centerX = point->iLon;
                centerY = point->iLat;
            }
        }
        m_MapX.SetCenterX(centerX);
        m_MapX.SetCenterY(centerY);
        m_MapX.SetZoom(m_InitZoom/256);
        CMainFrame* mainFrame = (CMainFrame *)GetParentFrame();
        CString str;
        str = m_RecordStaff->iStaff->iName + _T("Ѳ���¼");
        mainFrame->m_wndStatusBar.SetPaneText(mainFrame->m_wndStatusBar.CommandToIndex(ID_RECORD), str);
    }
}