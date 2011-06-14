#pragma once
#include "RWDSClientDoc.h"
#include "RWDSClientView.h"


// CDeviceList 对话框

class CDeviceList : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceList)

public:
	CDeviceList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeviceList();

// 对话框数据
	enum { IDD = IDD_SETDEVICE };
private:
    CRWDSClientView* m_CRWDSClientView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
