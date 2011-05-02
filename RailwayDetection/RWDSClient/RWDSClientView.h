
// RWDSClientView.h : CRWDSClientView ��Ľӿ�
//

#pragma once
#include "MAPX.H"
#include "Datadef.h"

class CPointList;
class CLineList;
class CSchedule;
class CRWDSClientView : public CView
{
	friend class CPointList;
	friend class CLineList;
	friend class CSchedule;
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

protected:
	CMapX m_MapX;
	CString m_SymbolLayer;
	CString m_MapName;
	double	m_InitZoom;			//��ʼ���ű���0822
	double	m_InitCenterX;
	double	m_InitCenterY;
	CPoint  m_RightClkPoint;
	vector<MapPoint*> m_MapPoint;
	vector<LineInfo*> m_Line;
	vector<ScheduleLine*> m_Schedule;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
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
};

#ifndef _DEBUG  // RWDSClientView.cpp �еĵ��԰汾
inline CRWDSClientDoc* CRWDSClientView::GetDocument() const
   { return reinterpret_cast<CRWDSClientDoc*>(m_pDocument); }
#endif
