
#pragma once

#include "ViewTree.h"
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
    friend class CRWDSClientView;
// 构造
public:
	CFileView();

	void AdjustLayout();
	void FillFileView();
    void CleanFileView();
    void TreeVisitForDeleteItemData(HTREEITEM aItem);
	void OnChangeVisualStyle();
	void SetRWDSClientView(CRWDSClientView* aRWDSClientView);
    void GetOrgData(OrganizationInfo* aOrg);
	void DeleteAllOrgData(OrganizationInfo* aOrg);
// 特性
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;
	CRWDSClientView* m_RWDSClientView;

// 实现
public:
	virtual ~CFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnNMClickFileView(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkFileView(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpandedFileView(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpandingFileView(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
	

};

