
// RWDSClientView.cpp : CRWDSClientView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "RWDSClient.h"
#endif

//#pragma  pack()

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
#include "DataService.h"
#include "OrgList.h"
#include "DeviceList.h"
#include "PitureReview.h"
#include "ErrorDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CRWDSClientView* gClientView = NULL;
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
	ON_COMMAND(ID_SET_LINE, &CRWDSClientView::OnSetLine)
	ON_COMMAND(ID_SET_SCHEDULE, &CRWDSClientView::OnSetSchedule)
	ON_WM_TIMER()
    ON_COMMAND(ID_SET_STAFF, &CRWDSClientView::OnSetStaff)
    ON_COMMAND(ID_SET_EMERGENCYTASK, &CRWDSClientView::OnSetEmergencytask)
//    ON_COMMAND(ID_REVIEW_RECORD, &CRWDSClientView::OnReviewRecord)
    ON_COMMAND(ID_REVIEW_RECORDDEVICE, &CRWDSClientView::OnReviewRecorddevice)
    ON_COMMAND(ID_REVIEW_RECORDSTAFF, &CRWDSClientView::OnReviewRecordstaff)
    ON_COMMAND(ID_RESET_ORG, &CRWDSClientView::OnResetOrg)
    ON_COMMAND(ID_SET_ORGANIZATION, &CRWDSClientView::OnSetOrganization)
    ON_COMMAND(ID_SET_DEVICE, &CRWDSClientView::OnSetDevice)
    ON_UPDATE_COMMAND_UI(ID_SET_EMERGENCYTASK, &CRWDSClientView::OnUpdateSetEmergencytask)
    ON_UPDATE_COMMAND_UI(ID_SET_LINE, &CRWDSClientView::OnUpdateSetLine)
    ON_UPDATE_COMMAND_UI(ID_SET_ORGANIZATION, &CRWDSClientView::OnUpdateSetOrganization)
    ON_UPDATE_COMMAND_UI(ID_SET_POINT, &CRWDSClientView::OnUpdateSetPoint)
    ON_UPDATE_COMMAND_UI(ID_SET_SCHEDULE, &CRWDSClientView::OnUpdateSetSchedule)
    ON_UPDATE_COMMAND_UI(ID_SET_STAFF, &CRWDSClientView::OnUpdateSetStaff)
    ON_UPDATE_COMMAND_UI(ID_SET_DEVICE, &CRWDSClientView::OnUpdateSetDevice)
    ON_COMMAND(ID_REVIEW_PICTURE, &CRWDSClientView::OnReviewPicture)
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
// CRWDSClientView 构造/析构

CRWDSClientView::CRWDSClientView()
{
	// TODO: 在此处添加构造代码
	m_SymbolLayer = _T("SymbolLayer");
	m_TrackLayer = _T("TrackLayer");
	m_MapName = _T("RailwayMap.GST");
    m_StaffRecord = NULL;
    //m_StaffCurrentTrack = NULL;
    m_DisplayFlag = KNone;
	m_Calendar = new CalendarSchedule;
    //m_Calendar->iScheduleLine = &m_Line;
    m_MapLoaded = FALSE;
    m_CurrentOrg = NULL;
}

CRWDSClientView::~CRWDSClientView()
{
	delete m_Calendar;
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
	//BOOL bo = theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_MAPX, point.x, point.y, this, TRUE);
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
	gClientView = this;
	// TODO:  在此添加您专用的创建代码
	CString strLic = _T("uQnZi2sFw22L0-MRa8pYX-2E6P1077-3N6M0499-5C038223-10884-12802-36882-8029");
	BSTR bstrLic = strLic.AllocSysString();
	if(!m_MapX.Create(NULL, WS_VISIBLE, CRect(0,0,100,100), this, IDC_MAP, NULL, FALSE, bstrLic))
	{
        m_MapLoaded = FALSE;
		//return -1;
	}
	::SysFreeString(bstrLic); 
	//if(!m_MapX.Create(NULL, WS_VISIBLE, CRect(0,0,100,100), this, IDC_MAP))
	//{
	//	return -1;
	//}
    if (m_MapLoaded)
    {
	    try 
        {	
            //CString curDir;
		    //curDir = GetModulePath();

		    CString mapPath = LoadMapInfoFromFile();
		    //m_MapX.geoset(_T(".\\map"));
		    m_MapX.SetGeoSet(mapPath);		//指定地图集
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
		    //没有tempLayer图层，就新建
		    if (!flagSymbol)
		    {
			    layer=m_MapX.GetLayers().CreateLayer(m_SymbolLayer);
			    m_MapX.GetLayers().SetAnimationLayer(layer); //设为动态图层  
			    layerSymbolIndex = 0;
		    }

		    m_MapX.GetLayers().Move(layerSymbolIndex+1, 1);

		    if (!flagTrack)
		    {
			    layer=m_MapX.GetLayers().CreateLayer(m_TrackLayer);
			    m_MapX.GetLayers().SetAnimationLayer(layer); //设为动态图层  
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
    }
	
	m_FileView = &((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetFileView();
	//m_FileView->SetRWDSClientView(this);
	//m_FileView->FillFileView();

	//SetTimer(TIMERNTRACK, 500, NULL);

    //获取初始化数据
    m_Org.clear();
    GetOrgTree(theApp.m_LoginOrgID, &m_Org);
    //OrganizationInfo* org = m_Org[0];
	MapxCleanAllFeature(m_SymbolLayer);

    //获取权限
    m_CurrentPermission.iBasical = theApp.m_LoginPermission.iBasical;
    m_CurrentPermission.iOperate = theApp.m_LoginPermission.iOperate;
    m_CurrentPermission.iReportForm = theApp.m_LoginPermission.iReportForm;
	return 0;
}


void CRWDSClientView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
    if (m_MapLoaded)
    {
	    m_MapX.MoveWindow(0, 0, cx, cy, TRUE);
    }
}

void CRWDSClientView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
    if (m_MapLoaded)
    {
	    m_MapX.SetFocus(); 
    }
}


void CRWDSClientView::OnMapPan()
{
	// TODO: 在此添加命令处理程序代码
    if (m_MapLoaded)
    {
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
}


void CRWDSClientView::OnUpdateMapPan(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
    if (m_MapLoaded)
    {
        BOOL bCheck;
        if(miPanTool==m_MapX.GetCurrentTool())
            bCheck=TRUE;
        else
            bCheck=FALSE;
        pCmdUI->SetCheck(bCheck);
    }
}


void CRWDSClientView::OnMapZoomin()
{
	// TODO: 在此添加命令处理程序代码
    if (m_MapLoaded)
    {
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
}


void CRWDSClientView::OnUpdateMapZoomin(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
    if (m_MapLoaded)
    {
        BOOL bCheck;
        if(miZoomInTool==m_MapX.GetCurrentTool())
            bCheck=TRUE;
        else
            bCheck=FALSE;
        pCmdUI->SetCheck(bCheck);
    }
}


void CRWDSClientView::OnMapZoomout()
{
	// TODO: 在此添加命令处理程序代码
    if (m_MapLoaded)
    {
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
}

void CRWDSClientView::OnUpdateMapZoomout(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
    if (m_MapLoaded)
    {
	    BOOL bCheck;
	    if(miZoomOutTool==m_MapX.GetCurrentTool())
		    bCheck=TRUE;
	    else
		    bCheck=FALSE;
	    pCmdUI->SetCheck(bCheck);
    }
}


CString CRWDSClientView::LoadMapInfoFromFile()
{
    CFile pFile;
    CFileException e;
    CString mapPath = _T("");
    if(!pFile.Open(_T("map.cfg"), CFile::modeRead, &e))
    {//return default
        CString curDir;
        curDir = GetModulePath();
        mapPath = curDir + _T("\\map\\") + m_MapName;
    }
    else
    {
        pFile.Read(mapPath.GetBuffer(MAX_PATH), MAX_PATH);
        mapPath.ReleaseBuffer();
        int pos = mapPath.ReverseFind('\\');
        m_MapName = mapPath.Right(mapPath.GetLength() - pos);
    }
    return mapPath;
}


CString CRWDSClientView::GetModulePath()
{
	CString modDir;
	GetModuleFileName(NULL, modDir.GetBuffer(MAX_PATH), MAX_PATH); 
	modDir.ReleaseBuffer();
	int pos;   
	pos = modDir.ReverseFind('\\');   
	modDir = modDir.Left(pos);  
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
    if (m_MapLoaded)
    {
	    CMapXPoint point;
	    point.CreateDispatch(point.GetClsid());
	    point.Set(aMapLon, aMapLat);
	    CMapXFeature ft;
	    ft=m_MapX.GetFeatureFactory().CreateCircularRegion(miCircleTypeScreen, point, 0.0003, miUnitDegree,100);
	    CMapXStyle style;
	    style = ft.GetStyle();
	    style.SetRegionColor(aColor);
	    style.SetRegionBorderStyle(0);//无边框
	    m_MapX.GetLayers().Item(aLayerName).AddFeature(ft);
    }
}

void CRWDSClientView::MapxDrawLine(double aMapLon1, double aMapLat1, double aMapLon2, double aMapLat2, ColorConstants aColor)
{
    if (m_MapLoaded)
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
	    style.SetLineColor(aColor);
	    m_MapX.GetLayers().Item(m_SymbolLayer).AddFeature(ft);
	    //m_MapX.GetLayers().Item(m_SymbolLayer).Refresh();
    }
}

void CRWDSClientView::MapxSetText(double aMapLon, double aMapLat, CString aText)
{
    if (m_MapLoaded)
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

		    CMapXFeature    m_fMarkText;        //标记文字图元
		    CMapXFeature    m_fMarkSymbol;		//标记符号图元
		    CMapXPoint		m_ptMarkPoint;		//标记定位点

		    m_ptMarkPoint.CreateDispatch(m_ptMarkPoint.GetClsid());
		    m_ptMarkPoint.Set(aMapLon, aMapLat);

		    pntVt.vt = VT_DISPATCH;
		    pntVt.pdispVal = m_ptMarkPoint.m_lpDispatch;
		    pntVt.pdispVal->AddRef();

		    ftFy=m_MapX.GetFeatureFactory();

		    //绘制bmp图标
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
}

void CRWDSClientView::MapxCleanAllFeature(CString aLayerName)
{
    if (m_MapLoaded)
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
}

void CRWDSClientView::OnMouseMove_Map(short Button, short Shift, long X, long Y)
{
    if (m_MapLoaded)
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
		    CMainFrame* mainFrame = (CMainFrame *)GetParentFrame();		//获取父窗口句柄

		    m_MapX.ConvertCoord(&singleX, &singleY, &m_MouseLon, &m_MouseLat, miScreenToMap);	//屏幕坐标转换为地图坐标
		    DecimalGeoToStandardGeo(m_MouseLon,m_MouseLat,&iXd,&iXm,&iXs,&iYd,&iYm,&iYs);			//小数坐标==>标准地理坐标
		    sCoordinate.Format(_T("经度:%d°%d′%d\" ,纬度:%d°%d′%d\" "),iXd,iXm,iXs,iYd,iYm,iYs);	//在状态栏中显示当前坐标         
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
	//			Ftr.SetPoint(ps);	//改变定位点
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
    if (m_MapLoaded)
    {
	    double dZoom;
	    double dPaperWidth;
	    double dScale;
	    long lScale;

	    CMainFrame* mainFrame = (CMainFrame *)GetParentFrame();		//获取父窗口句柄

	    dZoom=m_MapX.GetZoom();
	    dPaperWidth=m_MapX.GetMapPaperWidth();
	    dScale=dZoom/dPaperWidth*100000;
	    lScale=(long)dScale;

	    if (mainFrame->m_wndStatusBar.m_hWnd == NULL)
	    {
		    return;
	    }
	    CString strScale;
	    strScale.Format(_T("比例尺：1:%ld"),lScale);
	    mainFrame->m_wndStatusBar.SetPaneText(mainFrame->m_wndStatusBar.CommandToIndex(ID_INDICATOR_SCALE),strScale);
    }
}

void CRWDSClientView::OnSymbolAdd()
{
	// TODO: 在此添加命令处理程序代码
	CPointList pointDlg(this);
	pointDlg.SetDefaltPoint(m_MouseLon, m_MouseLat);
	pointDlg.DoModal();
}

void CRWDSClientView::OnSymbolDelete()
{
	// TODO: 在此添加命令处理程序代码
    if (m_MapLoaded)
    {
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
}


void CRWDSClientView::OnSetPoint()
{
	// TODO: 在此添加命令处理程序代码
	//if (m_CurrentPermission.iSinglePermission.iBasical)
	//{
	//}
	CPointList setPoint(this);
	setPoint.DoModal();
}

void CRWDSClientView::OnSetLine()
{
	// TODO: 在此添加命令处理程序代码
	CLineList lineList(this);
	lineList.DoModal();
}


void CRWDSClientView::OnSetSchedule()
{
	// TODO: 在此添加命令处理程序代码
	CSchedule schedule(this);
	schedule.DoModal();
}


void CRWDSClientView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加命令处理程序代码
	static BOOL displayLastPoint = TRUE;
	switch(nIDEvent)
	{
	case TIMERNTRACK:
		//获取线路的当前巡查位置
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
    // TODO: 在此添加命令处理程序代码
    CStaffList setStaff(this);
    setStaff.DoModal();
}


void CRWDSClientView::OnSetEmergencytask()
{
    // TODO: 在此添加命令处理程序代码
    CEmergencyTask task(this);
    task.DoModal();
}


void CRWDSClientView::OnReviewRecorddevice()
{
    // TODO: 在此添加命令处理程序代码
}


void CRWDSClientView::OnReviewRecordstaff()
{
    // TODO: 在此添加命令处理程序代码
    m_DisplayFlag = KStaffLog;
    CRecordStaff rstaff(this);
    if(rstaff.DoModal() == IDOK)
    {
        if (!m_StaffRecord)
        {
            m_StaffRecord = new RecordStaff;
        }
        int recordSelect = rstaff.GetSelect();
        m_StaffRecord->iStaff = m_Staff[recordSelect];

        //获取员工巡查记录
        GetStaffCurrentTrack(rstaff.GetPickDateTime(), m_StaffRecord);
        vector<double> recordLon;
        vector<double> recordLat;
        GetStaffScheduleTrack(_ttoi(m_StaffRecord->iStaff->iID), rstaff.GetPickDateTime(), 
                              &recordLon, &recordLat);

        MapxCleanAllFeature(m_SymbolLayer);
        MapxCleanAllFeature(m_TrackLayer);

        MapPoint* point = NULL;
        LineInfo* line = NULL;
        double centerX = 0.0;
        double centerY = 0.0;
        for (size_t i=0; i<m_StaffRecord->iStaff->iArrangeLine.size(); i++)
        {//显示该员工所设计的路线
            line = m_StaffRecord->iStaff->iArrangeLine[i];
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

        for (size_t i=0; i<m_StaffRecord->iRecordLon.size() && i<m_StaffRecord->iRecordLat.size() ; i++)
        {//描绘员工巡查路线
            MapxDrawCircle(m_StaffRecord->iRecordLon[i], m_StaffRecord->iRecordLat[i], m_TrackLayer, miColorRed);
            if (centerX == 0 || centerY == 0)
            {
                centerX = m_StaffRecord->iRecordLon[0];
                centerY = m_StaffRecord->iRecordLat[0];
            }
        }
        m_MapX.SetCenterX(centerX);
        m_MapX.SetCenterY(centerY);
        m_MapX.SetZoom(m_InitZoom/256);
        CMainFrame* mainFrame = (CMainFrame *)GetParentFrame();
        CString str;
        str = m_StaffRecord->iStaff->iName + _T("巡查记录");
        mainFrame->m_wndStatusBar.SetPaneText(mainFrame->m_wndStatusBar.CommandToIndex(ID_RECORD), str);
    }
}

void CRWDSClientView::DeleteAllMapPoint()
{
    MapPoint* point = NULL;
    while(m_MapPoint.size() > 0)
    {
        point = m_MapPoint[0];
        m_MapPoint.erase(m_MapPoint.begin());
        delete point;
    }
}

void CRWDSClientView::DeleteAllLine()
{
    LineInfo* line = NULL;
    while(m_Line.size() > 0)
    {
        line = m_Line[0];
        m_Line.erase(m_Line.begin());
        delete line;
    }
}

void CRWDSClientView::DeleteAllStaff()
{
    StaffInfo* staff = NULL;
    while (m_Staff.size() > 0)
    {
        staff = m_Staff[0];
        m_Staff.erase(m_Staff.begin());
        delete staff;
    }
}

void CRWDSClientView::ClearAllElement()
{
    m_MapPoint.clear();
    m_Line.clear();
    m_Staff.clear();
    m_Device.clear();
}

void CRWDSClientView::OnResetOrg()
{
    // TODO: 在此添加命令处理程序代码
    CMainFrame* pMain=static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd);
    pMain->m_wndFileView.TreeVisitForDeleteItemData(pMain->m_wndFileView.m_wndFileView.GetRootItem());
    pMain->m_wndFileView.CleanFileView();
    pMain->m_wndFileView.FillFileView();
}


void CRWDSClientView::OnSetOrganization()
{
    // TODO: 在此添加命令处理程序代码
    COrgList org(this);
    org.DoModal();
}

void CRWDSClientView::OnSetDevice()
{
    // TODO: 在此添加命令处理程序代码
    CDeviceList device(this);
    device.DoModal();
}


void CRWDSClientView::OnUpdateSetEmergencytask(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!m_CurrentPermission.iOperate)
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnUpdateSetLine(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!m_CurrentPermission.iOperate)
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnUpdateSetOrganization(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!m_CurrentPermission.iOperate)
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnUpdateSetPoint(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!m_CurrentPermission.iOperate)
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnUpdateSetSchedule(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!m_CurrentPermission.iOperate)
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnUpdateSetStaff(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!m_CurrentPermission.iOperate)
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}

void CRWDSClientView::OnUpdateSetDevice(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!m_CurrentPermission.iOperate)
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnReviewPicture()
{
    // TODO: 在此添加命令处理程序代码
    if (!m_CurrentOrg)
    {
        return;
    }
    CPitureReview picture(this);
    picture.DoModal();
}
