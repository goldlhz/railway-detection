// VoiceList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "VoiceList.h"
#include "afxdialogex.h"


// CVoiceList �Ի���

IMPLEMENT_DYNAMIC(CVoiceList, CDialogEx)

CVoiceList::CVoiceList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVoiceList::IDD, pParent)
{

}

CVoiceList::~CVoiceList()
{
}

void CVoiceList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VOICE_LIST, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CVoiceList, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_VOICE_LIST, &CVoiceList::OnNMDblclkVoiceList)
END_MESSAGE_MAP()


// CVoiceList ��Ϣ�������


BOOL CVoiceList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CRect clientRect;
	m_ListCtrl.GetClientRect(&clientRect);
	m_ListCtrl.InsertColumn(0, _T("�ļ���"), LVCFMT_LEFT, clientRect.Width()/3*2);
	m_ListCtrl.InsertColumn(1, _T("����ʱ��"), LVCFMT_LEFT, clientRect.Width()/3);

	AddVoiceToListCtrl(_T("F:\\test"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CVoiceList::AddVoiceToListCtrl(CString aVoicePath)
{
	CFileFind   finder;
	CString fileName = aVoicePath;
	fileName += _T("\\*");
	BOOL bWorking = finder.FindFile(fileName);
	while(bWorking)
	{
		bWorking = finder.FindNextFile(); 
		if(finder.IsDots()) 
			continue; 

		CString strPath = finder.GetFilePath();
		//if it's a directory, recursively search it 
		if(finder.IsDirectory()) 
		{ 
			AddVoiceToListCtrl(strPath);
		} 
		else 
		{
			//CString strFileName = strPath+_T("\\")+finder.GetFileName();
			CString extName = strPath;
			int pos = extName.ReverseFind('.');
			extName = strPath.Right(strPath.GetLength() - pos);
			if (extName.Compare(_T(".mp3")) == 0)
			{
				m_ListCtrl.InsertItem(m_ListCtrl.GetItemCount(), strPath);
			}
		} 
	}
	finder.Close(); 
}


void CVoiceList::OnNMDblclkVoiceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos;
	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	// �õ���Ŀ����
	int select = m_ListCtrl.GetNextSelectedItem(pos);  
	if (select<0)
	{
		return;
	}
	CString fileName = m_ListCtrl.GetItemText(select, 0);
	ShellExecute(NULL, _T("open"), fileName, NULL, NULL, SW_SHOW);
	*pResult = 0;
}
