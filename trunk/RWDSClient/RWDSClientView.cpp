
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRWDSClientView

IMPLEMENT_DYNCREATE(CRWDSClientView, CView)

BEGIN_MESSAGE_MAP(CRWDSClientView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRWDSClientView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CRWDSClientView ����/����

CRWDSClientView::CRWDSClientView()
{
	// TODO: �ڴ˴���ӹ������

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

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CRWDSClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CRWDSClientView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRWDSClientView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
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


// CRWDSClientView ��Ϣ�������
