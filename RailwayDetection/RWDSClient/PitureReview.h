#pragma once
#include "afxdtctl.h"
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"
#include "afxcmn.h"

// CPitureReview 对话框

class CPitureReview : public CDialogEx
{
	DECLARE_DYNAMIC(CPitureReview)

public:
	CPitureReview(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPitureReview();

// 对话框数据
	enum { IDD = IDD_REVIEWPICTURE };
public:
    CString GetModulePath();
private:
    CRWDSClientView* m_CRWDSClientView;
    CDateTimeCtrl m_DatePicture;
    CDateTimeCtrl m_EndDatePicture;
    vector<PictureInfo*> m_PictureList;
    CListCtrl m_ListCtrl;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedBtnShowpicture();
    afx_msg void OnBnClickedBtnDownloadpicture();

    virtual BOOL OnInitDialog();
    afx_msg void OnNMDblclkPicturelist(NMHDR *pNMHDR, LRESULT *pResult);
    void ShowPictureInWindows(PictureInfo* aPicture);
};
