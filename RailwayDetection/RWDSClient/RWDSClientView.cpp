
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
#include "VoiceList.h"
#include "Report.h"
#include "DataListControl.h"
#include "PermissionGroup.h"

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
	ON_COMMAND(ID_REVIEW_VOICE, &CRWDSClientView::OnReviewVoice)
    ON_COMMAND(ID_REPORT_MONTH, &CRWDSClientView::OnReportMonth)
    ON_COMMAND(ID_SET_PERMISSIONGROUP, &CRWDSClientView::OnSetPermissiongroup)
    ON_UPDATE_COMMAND_UI(ID_REVIEW_PICTURE, &CRWDSClientView::OnUpdateReviewPicture)
    ON_UPDATE_COMMAND_UI(ID_REVIEW_VOICE, &CRWDSClientView::OnUpdateReviewVoice)
    ON_UPDATE_COMMAND_UI(ID_REPORT_MONTH, &CRWDSClientView::OnUpdateReportMonth)
    ON_UPDATE_COMMAND_UI(ID_REVIEW_RECORDSTAFF, &CRWDSClientView::OnUpdateReviewRecordstaff)
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
	//m_Calendar = new CalendarSchedule;
    //m_Calendar->iScheduleLine = &m_Line;
    m_MapLoaded = TRUE;
    m_CurrentOrg = NULL;
    m_ReportForm = NULL;
    m_OpenReportForm = FALSE;
}

CRWDSClientView::~CRWDSClientView()
{
	//delete m_Calendar;
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
    //PRINTTRACE(_T("123"));
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

    //PRINTTRACE(_T("456"));
	gClientView = this;
	// TODO:  在此添加您专用的创建代码
	//CString strLic = _T("uQnZi2sFw22L0-MRa8pYX-2E6P1077-3N6M0499-5C038223-10884-12802-36882-8029");
	//BSTR bstrLic = strLic.AllocSysString();
	if(!m_MapX.Create(NULL, WS_VISIBLE, CRect(0,0,100,100), this, IDC_MAP, NULL, FALSE))
	{
        m_MapLoaded = FALSE;
		//return -1;
	}
    //PRINTTRACE(_T("789"));
	//::SysFreeString(bstrLic); 
	//if(!m_MapX.Create(NULL, WS_VISIBLE, CRect(0,0,100,100), this, IDC_MAP))
	//{
	//	return -1;
	//}
    if (m_MapLoaded)
    {
        //PRINTTRACE(_T("111111"));
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
            //PRINTTRACE(_T("aaaa"));
		    e->ReportError();
		    e->Delete();
	    } 
	    catch (COleException *e)
	    {
            //PRINTTRACE(_T("bbbbbbb"));
		    e->ReportError();
		    e->Delete();
	    }
        m_MapX.SetRedrawInterval(2000);
    }
	//PRINTTRACE(_T("ccccccccc"));
	m_FileView = &((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetFileView();
	//m_FileView->SetRWDSClientView(this);
	//m_FileView->FillFileView();

	//SetTimer(TIMERNTRACK, 500, NULL);

    //获取初始化数据
    m_Org.clear();
    GetOrgTree(theApp.m_LoginOrgID, &m_Org);
    if (m_Org.size() == 0)
    {
        AfxMessageBox(_T("机构获取失败，请重新登录"));
        return -1;
    }
    //OrganizationInfo* org = m_Org[0];
	MapxCleanAllFeature(m_SymbolLayer);

    //获取权限
    m_CurrentPermission.iBasical = theApp.m_LoginPermission.iBasical;
    m_CurrentPermission.iOperate = theApp.m_LoginPermission.iOperate;
    m_CurrentPermission.iReportForm = theApp.m_LoginPermission.iReportForm;

    //读取铁路线
    CStdioFile file;
    CString filename = GetModulePath() + _T("\\linename.cfg");
    if( file.Open(filename, CFile::modeRead) )
    {
        CString strLine;
        CString strName;
        CString strNo;
        while(file.ReadString(strLine))
        {
            int equare = strLine.Find('=', 0);
            if (equare == -1)
            {
                continue;
            }
            strName = strLine.Left(equare);
            strNo = strLine.Right(strLine.GetLength() - equare - 1);
            strRailLineName.push_back(strName);
            strRailLineNameCount++;
        }
    }
    else
    {//不能打开文件，填入默认值
        for (int i=0; i<50; i++)
        {
            strRailLineName.push_back(_T("未知线路"));
        }
        strRailLineNameCount = 50;
    }
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
	CString curDir;
	curDir = GetModulePath();
    CString mapPath = _T("");

    if(pFile.Open(curDir + _T("\\map.cfg"), CFile::modeRead, &e))
    {
        char* tmp = mapPath.GetBuffer(MAX_PATH);
        memset(tmp, '\0', MAX_PATH);
        pFile.Read(tmp, MAX_PATH);
        mapPath.ReleaseBuffer();
        if (mapPath.Left(1) == ".")
        {//文件设置为当前路径
            mapPath = curDir + mapPath.Right(mapPath.GetLength()-1);
        }
        int pos = mapPath.ReverseFind('\\');
        m_MapName = mapPath.Right(mapPath.GetLength() - pos -1 );
    }
    else
    {
        mapPath = curDir + _T("\\map\\") + m_MapName;
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
        if (m_CurrentOrg)
        {

            if (m_CurrentOrg->iMapPoint.size() == 0)
            {
                break;
            }
        }
        MapPoint* point;

        MapxCleanAllFeature(m_TrackLayer);
        size_t i;
        for (i=0; i<m_CurrentOrg->iMapPoint.size()-1; i++)
        {
            point =m_CurrentOrg->iMapPoint[i];
            MapxDrawCircle(point->iLon-0.001, point->iLat-0.001, m_TrackLayer, miColorYellow);
        }
        if (displayLastPoint)
        {
            point = m_CurrentOrg->iMapPoint[i];
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



void CRWDSClientView::DeleteAllMapPoint()
{
    if (!m_CurrentOrg)
    {
        return;
    }
    MapPoint* point = NULL;
    while(m_CurrentOrg->iMapPoint.size() > 0)
    {
        point = m_CurrentOrg->iMapPoint[0];
        m_CurrentOrg->iMapPoint.erase(m_CurrentOrg->iMapPoint.begin());
        delete point;
    }
}

void CRWDSClientView::DeleteAllLine()
{
    if (!m_CurrentOrg)
    {
        return;
    }
    LineInfo* line = NULL;
    while(m_CurrentOrg->iLine.size() > 0)
    {
        line = m_CurrentOrg->iLine[0];
        m_CurrentOrg->iLine.erase(m_CurrentOrg->iLine.begin());
        delete line;
    }
}

void CRWDSClientView::DeleteAllStaff()
{
    if (!m_CurrentOrg)
    {
        return;
    }
    StaffInfo* staff = NULL;
    while (m_CurrentOrg->iStaff.size() > 0)
    {
        staff = m_CurrentOrg->iStaff[0];
        m_CurrentOrg->iStaff.erase(m_CurrentOrg->iStaff.begin());
        delete staff;
    }
}

void CRWDSClientView::ClearAllElement()
{
    //m_MapPoint.clear();
    //m_Line.clear();
    //m_Staff.clear();
    //m_Device.clear();
    //m_Calendar->iScheduleStaff.clear();
}

void CRWDSClientView::AddElementFromOrg(OrganizationInfo* aOrg)
{
    //m_Calendar = aOrg->iCalendar;
    //for (size_t i=0; i<aOrg->iMapPoint.size(); i++)
    //{
    //    m_MapPoint.push_back(aOrg->iMapPoint[i]);
    //}

    //for (size_t i=0; i<aOrg->iLine.size(); i++)
    //{
    //    m_Line.push_back(aOrg->iLine[i]);
    //}

    //for (size_t i=0; i<aOrg->iStaff.size(); i++)
    //{
    //    m_Staff.push_back(aOrg->iStaff[i]);
    //}

    //for (size_t i=0; i<aOrg->iDevice.size(); i++)
    //{
    //    m_Device.push_back(aOrg->iDevice[i]);
    //}
}

void CRWDSClientView::OnSetPoint()
{
	// TODO: 在此添加命令处理程序代码
    if (!m_CurrentOrg)
    {
        AfxMessageBox(_T("请选择机构"));
        return;
    }
	CPointList setPoint(this);
	setPoint.DoModal();
}

void CRWDSClientView::OnSetLine()
{
	// TODO: 在此添加命令处理程序代码
    if (!m_CurrentOrg)
    {
        AfxMessageBox(_T("请选择机构"));
        return;
    }
	CLineList lineList(this);
	lineList.DoModal();
}


void CRWDSClientView::OnSetSchedule()
{
	// TODO: 在此添加命令处理程序代码
    if (!m_CurrentOrg)
    {
        AfxMessageBox(_T("请选择机构"));
            return;
    }
	CSchedule schedule(this);
	schedule.DoModal();
}

void CRWDSClientView::OnSetStaff()
{
    // TODO: 在此添加命令处理程序代码
    if (!m_CurrentOrg)
    {
        AfxMessageBox(_T("请选择机构"));
            return;
    }
    CStaffList setStaff(this);
    setStaff.DoModal();
}


void CRWDSClientView::OnSetEmergencytask()
{
    // TODO: 在此添加命令处理程序代码
    if (!m_CurrentOrg)
    {
        AfxMessageBox(_T("请选择机构"));
            return;
    }
    if(!m_CurrentOrg->iEmergencyDataSet)
    {
        GetEmergencyTask(m_CurrentOrg->iOrgID, &m_CurrentOrg->iEmergency);
        m_CurrentOrg->iEmergencyDataSet = TRUE;
    }
    CEmergencyTask task(this);
    task.DoModal();
}

void CRWDSClientView::OnSetOrganization()
{
    // TODO: 在此添加命令处理程序代码
    if (!m_CurrentOrg)
    {
        AfxMessageBox(_T("请选择机构"));
        return;
    }
    COrgList org(this);
    org.DoModal();
}

void CRWDSClientView::OnSetDevice()
{
    // TODO: 在此添加命令处理程序代码
    if (!m_CurrentOrg)
    {
        AfxMessageBox(_T("请选择机构"));
        return;
    }
    CDeviceList device(this);
    device.DoModal();
}

void CRWDSClientView::OnUpdateSetEmergencytask(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!(m_CurrentPermission.iBasical & PERMISSIONSETEMERGENCY) )
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnUpdateSetLine(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!(m_CurrentPermission.iBasical & PERMISSIONSETLINE) )
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnUpdateSetOrganization(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!(m_CurrentPermission.iBasical & PERMISSIONSETORG) )
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnUpdateSetPoint(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!(m_CurrentPermission.iBasical & PERMISSIONSETPOINT) )
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnUpdateSetSchedule(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!(m_CurrentPermission.iBasical & PERMISSIONSCHEDULE) )
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnUpdateSetStaff(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!(m_CurrentPermission.iBasical & PERMISSIONSETSTAFF) )
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}

void CRWDSClientView::OnUpdateSetDevice(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!(m_CurrentPermission.iBasical & PERMISSIONSETDEVICE) )
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnUpdateReviewPicture(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!(m_CurrentPermission.iBasical & PERMISSIONVIEWPICTURE) )
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnUpdateReviewVoice(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!(m_CurrentPermission.iBasical & PERMISSIONVIEWVOICE) )
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnUpdateReportMonth(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!(m_CurrentPermission.iBasical & PERMISSIONVIEWREPORT) )
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}


void CRWDSClientView::OnUpdateReviewRecordstaff(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    if (!(m_CurrentPermission.iBasical & PERMISSIONVIEWRECORDE))
    {//登录者不能设置任务
        pCmdUI->Enable(FALSE);
    }
}

void CRWDSClientView::OnResetOrg()
{
    // TODO: 在此添加命令处理程序代码
    vector<OrganizationInfo*> orgList;
    GetOrgTree(theApp.m_LoginOrgID, &orgList);
    if (orgList.size() > 0)
    {
        CMainFrame* pMain=static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd);
        pMain->m_wndFileView.TreeVisitForDeleteItemData(pMain->m_wndFileView.m_wndFileView.GetRootItem());
        //清空树形结构
        pMain->m_wndFileView.CleanFileView();
        //重新获取机构
        m_Org.clear();
        DeleteOrgListElement(&m_Org);
        for (size_t i=0; i<orgList.size(); i++)
        {
            m_Org.push_back(orgList[i]);
        }
        //GetOrgTree(theApp.m_LoginOrgID, &m_Org);
        pMain->m_wndFileView.FillFileView();
        m_CurrentOrg = NULL;
        AfxMessageBox(_T("刷新成功！"));
    }
    else
    {
        AfxMessageBox(_T("刷新失败！"));
    }
    orgList.clear();
}

void CRWDSClientView::OnReviewPicture()
{
    // TODO: 在此添加命令处理程序代码
    if (!m_CurrentOrg)
    {
        AfxMessageBox(_T("请选择机构"));
        return;
    }
    CPitureReview picture(this);
    picture.DoModal();
}


void CRWDSClientView::OnReviewVoice()
{
	// TODO: 在此添加命令处理程序代码
    if (!m_CurrentOrg)
    {
        AfxMessageBox(_T("请选择机构"));
        return;
    }
	CVoiceList voice;
	voice.DoModal();
}


void CRWDSClientView::OnReportMonth()
{
    // TODO: 在此添加命令处理程序代码
    if (!m_CurrentOrg)
    {
        AfxMessageBox(_T("请选择机构"));
        return;
    }
    if (!m_OpenReportForm)
    {
        m_ReportForm = new CReport(this);
        m_ReportForm->Create(IDD_REVIEWREPORT);
        m_ReportForm->ShowWindow(SW_SHOW); 
        m_OpenReportForm = TRUE;
    }
}


void CRWDSClientView::OnReviewRecorddevice()
{
    // TODO: 在此添加命令处理程序代码
}


void CRWDSClientView::OnReviewRecordstaff()
{
    // TODO: 在此添加命令处理程序代码
    if (!m_CurrentOrg)
    {
        AfxMessageBox(_T("请选择机构"));
        return;
    }
    m_DisplayFlag = KStaffLog;
    CRecordStaff rstaff(this);
    if(m_CurrentOrg && rstaff.DoModal() == IDOK)
    {
        if (!m_StaffRecord)
        {
            m_StaffRecord = new RecordStaff;
        }
        m_StaffRecord->iRecordLat.clear();
        m_StaffRecord->iRecordLon.clear();
        m_StaffRecord->iArrivedTime.clear();
        //int recordSelect = rstaff.GetSelect();
        time_t pickTime = rstaff.GetPickDateTime();
        StaffInfo* staff = rstaff.GetSelectedStaff();
        GetStaffScheduleTrack(staff->iID, pickTime, m_StaffRecord);

        MapxCleanAllFeature(m_SymbolLayer);
        MapxCleanAllFeature(m_TrackLayer);

        MapPoint* point = NULL;
        LineInfo* line = NULL;
        double centerX = 0.0;
        double centerY = 0.0;
        
        if (m_StaffRecord->iRecordLon.size() > 0 
            && m_StaffRecord->iRecordLat.size() > 0)
        {
            for (size_t i=0; i<m_StaffRecord->iRecordLon.size() && i<m_StaffRecord->iRecordLat.size() ; i++)
            {//描绘员工巡查路线
                MapxDrawCircle(m_StaffRecord->iRecordLon[i], m_StaffRecord->iRecordLat[i], m_TrackLayer, miColorRed);
                if (centerX == 0 || centerY == 0)
                {
                    centerX = m_StaffRecord->iRecordLon[0];
                    centerY = m_StaffRecord->iRecordLat[0];
                }
            }
            if (m_MapLoaded)
            {
                m_MapX.SetCenterX(centerX);
                m_MapX.SetCenterY(centerY);
                m_MapX.SetZoom(m_InitZoom/256);
            }
            CMainFrame* mainFrame = (CMainFrame *)GetParentFrame();
            CString str;
            str = staff->iName + _T("巡查记录");
            mainFrame->m_wndStatusBar.SetPaneText(mainFrame->m_wndStatusBar.CommandToIndex(ID_RECORD), str);
        }
        else
        {
            AfxMessageBox(_T("无该员工记录"));
        }
    }
}

void CRWDSClientView::DestroyReportForm()
{
    if (m_OpenReportForm)
    {
        m_OpenReportForm = FALSE;
        delete m_ReportForm;
    }
}


void CRWDSClientView::OnSetPermissiongroup()
{
    // TODO: 在此添加命令处理程序代码
    if (!m_CurrentOrg)
    {
        return;
    }
    CPermissionGroup pGroup(this);
    pGroup.DoModal();
}
