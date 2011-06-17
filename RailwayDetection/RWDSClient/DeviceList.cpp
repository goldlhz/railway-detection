// DeviceList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RWDSClient.h"
#include "DeviceList.h"
#include "afxdialogex.h"
#include "DataService.h"
#include "CmdDefine.h"


// CDeviceList �Ի���

IMPLEMENT_DYNAMIC(CDeviceList, CDialogEx)

CDeviceList::CDeviceList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceList::IDD, pParent)
{
    m_CRWDSClientView = static_cast<CRWDSClientView*>(pParent);
}

CDeviceList::~CDeviceList()
{
}

void CDeviceList::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DEVICELIST, m_ListCtrl);
    DDX_Control(pDX, IDC_COMBO_DEVICETYPE, m_ComboDeviceType);
}


BEGIN_MESSAGE_MAP(CDeviceList, CDialogEx)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_DEVICELIST, &CDeviceList::OnLvnItemchangedDevicelist)
    ON_BN_CLICKED(IDC_BTN_DEVICEADD, &CDeviceList::OnBnClickedBtnDeviceadd)
    ON_BN_CLICKED(IDC_BTN_DEVICEMODIFY, &CDeviceList::OnBnClickedBtnDevicemodify)
    ON_BN_CLICKED(IDC_BTN_DEVICEDELETE, &CDeviceList::OnBnClickedBtnDevicedelete)
END_MESSAGE_MAP()


// CDeviceList ��Ϣ�������

BOOL CDeviceList::CheckDivceId(int aDeviceID)
{
    DeviceInfo* device = NULL;
    for(size_t i=0; i<m_CRWDSClientView->m_Device.size(); i++)
    {
        device = m_CRWDSClientView->m_Device[i];
        if (aDeviceID == device->iDevID)
        {
            break;
        }
        device = NULL;
    }
    if (device)
    {//���ظ���ID
        return FALSE;
    }
    return TRUE;
}

BOOL CDeviceList::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    CRect clientRect;
    m_ListCtrl.GetClientRect(&clientRect);
    m_ListCtrl.InsertColumn(0, _T("�豸���"), LVCFMT_LEFT, clientRect.Width()/4);
    m_ListCtrl.InsertColumn(1, _T("�绰����"), LVCFMT_LEFT, clientRect.Width()/4*3);

    for (size_t i=0; i<m_CRWDSClientView->m_Device.size(); i++)
    {
        CString str;
        str.Format(_T("%d"), m_CRWDSClientView->m_Device[i]->iDevID);
        m_ListCtrl.InsertItem(i, str);
        m_ListCtrl.SetItemText(i, 1, m_CRWDSClientView->m_Device[i]->iPhoneNum);
        m_ListCtrl.SetItemData(i, (DWORD_PTR)m_CRWDSClientView->m_Device[i]);
    }
    m_ComboDeviceType.AddString(_T("ͼ��"));
    m_ComboDeviceType.AddString(_T("��Ƶ"));
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CDeviceList::OnLvnItemchangedDevicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    POSITION pos;
    pos = m_ListCtrl.GetFirstSelectedItemPosition();
    int select = m_ListCtrl.GetNextSelectedItem(pos);  
    if (select<0)
    {
        return;
    }
    DeviceInfo* device = (DeviceInfo*)m_ListCtrl.GetItemData(select);
    m_Selected = device;
    CString str;
    str.Format(_T("%d"), device->iDevID);
    GetDlgItem(IDC_EDIT_DEVICEID)->SetWindowText(str);
    GetDlgItem(IDC_EDIT_DEVICENUM)->SetWindowText(device->iPhoneNum);
    m_ComboDeviceType.SetCurSel(device->iDeviceType-1);
 
    *pResult = 0;
}


void CDeviceList::OnBnClickedBtnDeviceadd()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    
    CString strID;
    CString strNum;
    DeviceType type = static_cast<DeviceType>(m_ComboDeviceType.GetCurSel()+1);
    GetDlgItem(IDC_EDIT_DEVICEID)->GetWindowText(strID);
    GetDlgItem(IDC_EDIT_DEVICENUM)->GetWindowText(strNum);
    if (strID.Compare(_T("")) == 0 || strID.Compare(_T("")) == 0 || type <= 0 )
    {
        AfxMessageBox(_T("�������豸��Ϣ"));
        return;
    }
    int devID = _ttoi(strID);
    if (!CheckDivceId(devID))
    {
        AfxMessageBox(_T("�豸ID�ظ�"));
        return;
    }
    DeviceInfo* device = new DeviceInfo;
    device->iDevID = devID;
    device->iPhoneNum = strNum;
    device->iDeviceType = type;
    device->iOrgID = m_CRWDSClientView->m_CurrentOrg->iOrgID;
    m_CRWDSClientView->m_CurrentOrg->iDevice.push_back(device);
    m_CRWDSClientView->m_Device.push_back(device);
    int ctrlIndex = m_ListCtrl.GetItemCount();
    m_ListCtrl.InsertItem(ctrlIndex, strID);
    m_ListCtrl.SetItemText(ctrlIndex, 1, strNum);
    m_ListCtrl.SetItemData(ctrlIndex, (DWORD_PTR)device);
    SetOrgDevice(devID, CMD_DEVICE_ADD, device);
}


void CDeviceList::OnBnClickedBtnDevicemodify()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int select = m_ListCtrl.GetSelectionMark();
    if (select < 0)
        return;
    DeviceInfo* device = (DeviceInfo*)m_ListCtrl.GetItemData(select);
    CString str;
    GetDlgItem(IDC_EDIT_DEVICEID)->GetWindowText(str);
    device->iDevID = _ttoi(str);
    GetDlgItem(IDC_EDIT_DEVICENUM)->GetWindowText(device->iPhoneNum);
    device->iDeviceType = static_cast<DeviceType>(m_ComboDeviceType.GetCurSel()+1);
    AfxMessageBox(_T("�޸ĳɹ�"));
    m_ListCtrl.SetItemText(select, 0, str);
    m_ListCtrl.SetItemText(select, 1, device->iPhoneNum);
    SetOrgDevice(device->iDevID, CMD_DEVICE_MODIFY, device);
}


void CDeviceList::OnBnClickedBtnDevicedelete()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int select = m_ListCtrl.GetSelectionMark();
    if (select < 0)
        return;
    DeviceInfo* device = (DeviceInfo*)m_ListCtrl.GetItemData(select);
    if (!device)
    {
        return;
    }
    if (AfxMessageBox(_T("ȷ��ɾ��?"), MB_OKCANCEL) == IDOK)
    {
        for (size_t i=0; i<m_CRWDSClientView->m_Device.size(); i++)
        {//ɾ��view�е�list
            if (device == m_CRWDSClientView->m_Device[i])
            {
                m_CRWDSClientView->m_Device.erase(m_CRWDSClientView->m_Device.begin()+i);
                break;
            }
        }
        for (size_t i=0; i<m_CRWDSClientView->m_CurrentOrg->iDevice.size(); i++)
        {//ɾ��org�ﱣ���device
            if (device == m_CRWDSClientView->m_CurrentOrg->iDevice[i])
            {
                m_CRWDSClientView->m_CurrentOrg->iDevice.erase(
                    m_CRWDSClientView->m_CurrentOrg->iDevice.begin()+i);
                break;
            }
        }
        SetOrgDevice(device->iOrgID, CMD_DEVICE_DELETE, device);
        m_ListCtrl.DeleteItem(select);
        GetDlgItem(IDC_EDIT_DEVICEID)->SetWindowText(_T(""));
        GetDlgItem(IDC_EDIT_DEVICENUM)->SetWindowText(_T(""));
        m_ComboDeviceType.SetCurSel(-1);
        delete device;
        device = NULL;
    }
}
