
// RWDSClientView.h : CRWDSClientView ��Ľӿ�
//

#pragma once
#include "MAPX.H"
#include "Datadef.h"

#define TIMERNTRACK 1
enum DisplayFlag
{
    KNone = 0,
    KRealTime,
    KStaffLog,
    KDeviceLog
};
class CFileView;
class CRWDSClientView : public CView
{
	friend class CPointList;
	friend class CLineList;
	friend class CSchedule;
	friend class CFileView;
    friend class CStaffList;
    friend class CEmergencyTask;
    friend class CRecordStaff;
    friend class COrgTree;
    friend class COrgList;
    friend class CDeviceList;
    friend class CPitureReview;
    friend class CReport;
protected: // �������л�����
	CRWDSClientView();
	DECLARE_DYNCREATE(CRWDSClientView)

// ����
public:
	CRWDSClientDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CRWDSClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
    CString LoadMapInfoFromFile();
    void DeleteAllMapPoint();
    void DeleteAllLine();
    void DeleteAllStaff();
    void ClearAllElement();
    void AddElementFromOrg(OrganizationInfo* aOrg);
	CString GetModulePath();
	void DecimalGeoToStandardGeo(double dX, double dY, int *iXd, int *iXm, int *iXs, int *iYd, int *iYm, int *iYs);
	void MapxDrawCircle(double aMapLon, double aMapLat, CString aLayerName, ColorConstants aColor = miColorBlue);
	void MapxDrawLine(double aMapLon1, double aMapLat1, double aMapLon2, double aMapLat2, ColorConstants aColor = miColorBlue);
	void MapxSetText(double aMapLon, double aMapLat, CString aText);
	void MapxCleanAllFeature(CString aLayerName);
    void DestroyReportForm();
protected:
    PermissionGroup m_CurrentPermission;
	bool m_SymbolMove;
	CMapX m_MapX;
	CString m_SymbolLayer;
	CString m_TrackLayer;
	CString m_MapName;
	double	m_InitZoom;			//��ʼ���ű���
	double	m_InitCenterX;
	double	m_InitCenterY;
	double  m_MouseLon;
	double  m_MouseLat;
	CPoint  m_RightClkPoint;

	vector<OrganizationInfo*> m_Org;
    CReport* m_ReportForm;//�±���
    BOOL m_OpenReportForm;//���±������

    RecordStaff* m_StaffRecord;//�鿴��ԱѲ���¼
	CFileView* m_FileView;
    DisplayFlag m_DisplayFlag;
    OrganizationInfo* m_CurrentOrg;//��ǰѡ�еĻ���
    int m_TestValue;
    BOOL m_MapLoaded;
    //CString m_LoginAccount;
    //CString m_LoginPassword;
// ���ɵ���Ϣӳ�亯��
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
//    afx_msg void OnReviewRecord();
    afx_msg void OnReviewRecorddevice();
    afx_msg void OnReviewRecordstaff();
    afx_msg void OnResetOrg();

    afx_msg void OnSetOrganization();
    //virtual void OnInitialUpdate();
    afx_msg void OnSetDevice();
    afx_msg void OnUpdateSetEmergencytask(CCmdUI *pCmdUI);
    afx_msg void OnUpdateSetLine(CCmdUI *pCmdUI);
    afx_msg void OnUpdateSetOrganization(CCmdUI *pCmdUI);
    afx_msg void OnUpdateSetPoint(CCmdUI *pCmdUI);
    afx_msg void OnUpdateSetSchedule(CCmdUI *pCmdUI);
    afx_msg void OnUpdateSetStaff(CCmdUI *pCmdUI);
   
    afx_msg void OnUpdateSetDevice(CCmdUI *pCmdUI);
    afx_msg void OnReviewPicture();

	afx_msg void OnReviewVoice();
    afx_msg void OnReportMonth();
    afx_msg void OnSetPermissiongroup();
};
extern CRWDSClientView* gClientView;

#ifndef _DEBUG  // RWDSClientView.cpp �еĵ��԰汾
inline CRWDSClientDoc* CRWDSClientView::GetDocument() const
   { return reinterpret_cast<CRWDSClientDoc*>(m_pDocument); }
#endif

