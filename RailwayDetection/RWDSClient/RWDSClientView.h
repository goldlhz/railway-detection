
// RWDSClientView.h : CRWDSClientView 类的接口
//

#pragma once
#include "MAPX.H"
#include "Datadef.h"

#define TIMERNTRACK 1

class CFileView;
class CRWDSClientView : public CView
{
	friend class CPointList;
	friend class CLineList;
	friend class CSchedule;
	friend class CFileView;
    friend class CStaffList;
    friend class CEmergencyTask;
protected: // 仅从序列化创建
	CRWDSClientView();
	DECLARE_DYNCREATE(CRWDSClientView)

// 特性
public:
	CRWDSClientDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CRWDSClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	CString GetModulePath();
	void DecimalGeoToStandardGeo(double dX, double dY, int *iXd, int *iXm, int *iXs, int *iYd, int *iYm, int *iYs);
	void MapxDrawCircle(double aMapLon, double aMapLat, CString aLayerName, ColorConstants aColor = miColorRed);
	void MapxDrawLine(double aMapLon1, double aMapLat1, double aMapLon2, double aMapLat2);
	void MapxSetText(double aMapLon, double aMapLat, CString aText);
	void MapxCleanAllFeature(CString aLayerName);
protected:
	bool m_SymbolMove;
	CMapX m_MapX;
	CString m_SymbolLayer;
	CString m_TrackLayer;
	CString m_MapName;
	double	m_InitZoom;			//初始缩放比例
	double	m_InitCenterX;
	double	m_InitCenterY;
	double  m_MouseLon;
	double  m_MouseLat;
	CPoint  m_RightClkPoint;
	vector<MapPoint*> m_MapPoint;
	vector<LineInfo*> m_Line;
	vector<OrganizationInfo*> m_Org;
	CalendarSchedule* m_Calendar;
	vector<LineInfo*> m_CurrentLinePosition;
	vector<StaffInfo*> m_Staff;
    vector<EmergencyTaskInfo*> m_Emergency;
	CFileView* m_FileView;
// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
protected:
	void OnMouseMove_Map(short Button, short Shift, long X, long Y);
	DECLARE_EVENTSINK_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMapPan();
	afx_msg void OnUpdateMapPan(CCmdUI *pCmdUI);
	afx_msg void OnMapZoomin();
	afx_msg void OnUpdateMapZoomin(CCmdUI *pCmdUI);
	afx_msg void OnMapZoomout();
	afx_msg void OnUpdateMapZoomout(CCmdUI *pCmdUI);
	afx_msg void OnSymbolAdd();
	afx_msg void OnSymbolDelete();
	afx_msg void OnSetPoint();
	afx_msg void OnSetLine();
	afx_msg void OnSetSchedule();
	afx_msg void OnMapViewChangedMap();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnSetStaff();
    afx_msg void OnSetEmergencytask();
};
extern CRWDSClientView* gClientView;

#ifndef _DEBUG  // RWDSClientView.cpp 中的调试版本
inline CRWDSClientDoc* CRWDSClientView::GetDocument() const
   { return reinterpret_cast<CRWDSClientDoc*>(m_pDocument); }
#endif

