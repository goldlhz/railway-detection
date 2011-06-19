#include "StdAfx.h"
#include "DataService.h"
#include "ErrorDefine.h"
#include "CmdDefine.h"


int VerifyLogin( const CString& aLoginAccount, const CString& aLoginPassword )
{
    return KErrNone;
}


int GetLoginerPermission(const CString& aLoginAccount)
{//����Ȩ��ֵ
    return 0x01010101;
    //return 0;
}


int GetOrgTree(const CString& aLoginCount, vector<OrganizationInfo*>* a_OrgTree)
{
    OrganizationInfo* org = new OrganizationInfo;
    org->iOrgName = _T("Admin");
    org->iOrgAddress = _T("chengdu");
    org->iParentID = 0;
    org->iParentOrg = NULL;
    org->iOrgID = 1;
    org->iChildID.push_back(2);
    org->iChildID.push_back(3);
    org->iBoundaryRail = Baoji_Chengdu;
    org->iBoundaryStartKM = 0;
    org->iBoundaryEndKM = 300;
    a_OrgTree->push_back(org);

    org = new OrganizationInfo;
    org->iOrgName = _T("Child1");
    org->iOrgAddress = _T("chengdu");
    org->iParentOrg = (*a_OrgTree)[0];
    org->iParentID = org->iParentOrg->iOrgID;
    (*a_OrgTree)[0]->iChildOrg.push_back(org);
    org->iOrgID = 2;
    org->iChildID.push_back(4);
    org->iBoundaryRail = Chengdu_Kunming;
    org->iBoundaryStartKM = 0;
    org->iBoundaryEndKM = 300;
    a_OrgTree->push_back(org);

    org = new OrganizationInfo;
    org->iOrgName = _T("Child2");
    org->iOrgAddress = _T("chengdu");
    org->iParentOrg = (*a_OrgTree)[0];
    org->iParentID = org->iParentOrg->iOrgID;
    (*a_OrgTree)[0]->iChildOrg.push_back(org);
    org->iOrgID = 3;
    org->iChildID.push_back(5);
    org->iBoundaryRail = Chengdu_Chongqing;
    org->iBoundaryStartKM = 0;
    org->iBoundaryEndKM = 300;
    a_OrgTree->push_back(org);
    return KErrNone;
}


int SetOrganization(int aCmd, const OrganizationInfo* aOrganization )
{
//#define CMD_ORG_ADD 0x3C
//#define CMD_ORG_MODIFY 0x3D
//#define CMD_ORG_DELETE 0x3E
    return KErrNone;
}


int GetOrgPoint(int iOrgID, vector<MapPoint*>* aPointList)
{
    //if(iOrgID == 1)
    //{//�������һ������
    //    return kErrNotLastOrg;
    //}
    MapPoint *pt = new MapPoint;
    pt->iRailLine = Chengdu_Chongqing; 
    pt->iKM = 251;
    pt->iLon = 104.064531;
    pt->iLat = 30.699965;
    pt->iDirect = KUpLine;
    aPointList->push_back(pt);

    pt = new MapPoint;
    pt->iRailLine = Chengdu_Chongqing; 
    pt->iKM = 252;
    pt->iLon = 104.075530;
    pt->iLat = 30.699484;
    pt->iDirect = KUpLine;
    aPointList->push_back(pt);

    pt = new MapPoint;
    pt->iRailLine = Chengdu_Chongqing; 
    pt->iKM = 253;
    pt->iLon = 104.086526;
    pt->iLat = 30.699484;
    pt->iDirect = KUpLine;
    aPointList->push_back(pt);

    pt = new MapPoint;
    pt->iRailLine = Chengdu_Chongqing; 
    pt->iKM = 254;
    pt->iLon = 104.097521;
    pt->iLat = 30.699484;
    pt->iDirect = KUpLine;
    aPointList->push_back(pt);
    return KErrNone;
}


int SetOrgPoint( int aOrgID, int aCmd, const MapPoint* aPoint )
{
    switch (aCmd)
    {
    case CMD_POINT_ADD:
        break;
    case CMD_POINT_MODIFY:
        break;
    case CMD_POINT_DELETE:
        break;
    default:
        break;
    }
    return KErrNone;
}

int GetOrgLine(int iOrgID, const vector<MapPoint*>& aPointList, vector<LineInfo*>* aLineList)
{
    LineInfo *line = new LineInfo;
    line->iLineID = 1;
    line->iLineName = _T("�ɾ�1��");
    line->iStartKm = (aPointList)[0]->iKM;
    line->iStartNo = KFirstDay;
    line->iLineKmLonLat.push_back(aPointList[0]);
    line->iLineKmTime.push_back(100);
    line->iLineKmLonLat.push_back(aPointList[1]);
    line->iLineKmTime.push_back(100);
    line->iLineKmLonLat.push_back(aPointList[2]);
    line->iLineKmTime.push_back(100);
    line->iLineKmLonLat.push_back(aPointList[3]);
    line->iLineKmTime.push_back(100);
    aLineList->push_back(line);

    line = new LineInfo;
    line->iLineID = 2;
    line->iLineName = _T("�ɾ�2��");
    line->iStartKm = aPointList[1]->iKM;
    line->iStartNo = KThirdDay;
    line->iLineKmLonLat.push_back(aPointList[1]);
    line->iLineKmTime.push_back(0);
    line->iLineKmLonLat.push_back(aPointList[3]);
    line->iLineKmTime.push_back(0);
    aLineList->push_back(line);
    return KErrNone;
}

int SetOrgLine( int aOrgID, int aCmd, const LineInfo* aLine )
{
    return KErrNone;
}

int GetOrgStaff(int aOrgID, vector<StaffInfo*>* aStaffList)
{
    StaffInfo* staff = new StaffInfo;
    staff->iID = 1;
    staff->iOrgID = aOrgID;
    staff->iPassword = _T("111");
    staff->iLoginPermission = TRUE;
    staff->iName = _T("����");
    aStaffList->push_back(staff);

    staff = new StaffInfo;
    staff->iID = 2;
    staff->iOrgID = aOrgID;
    staff->iPassword = _T("");
    staff->iLoginPermission = FALSE;
    staff->iName = _T("����");
    aStaffList->push_back(staff);

    staff = new StaffInfo;
    staff->iID = 3;
    staff->iOrgID = aOrgID;
    staff->iName = _T("����");
    staff->iPassword = _T("");
    staff->iLoginPermission = FALSE;
    aStaffList->push_back(staff);
    return KErrNone;
}

int SetOrgStaff( int aOrgID, int aCmd, const StaffInfo* aStaff )
{
    return KErrNone;
}

int GetCalendarSchedule(int aOrgID, const vector<StaffInfo*>* aStaffList, CalendarSchedule* aSchedule)
{
    //for (size_t i=0; i<aLineList.size(); i++)
    //{
    //     aSchedule->iDateSchedule.push_back(aLineList[i]);
    //}
    aSchedule->iCaledarID = 1;
    aSchedule->iOrgID = aOrgID;
    aSchedule->iStartDay = 1288915200;
    aSchedule->iPeriods = 3;
    return KErrNone;
}

int SetCalendarSchedule(int aOrgID, const CalendarSchedule* aSchedule)
{
    return KErrNone;
}


int GetEmergencyTask( int aOrgID, vector<EmergencyTaskInfo*>* m_Emergency )
{
    return KErrNone;
}

int SetEmergencyTask( int aOrgID, int aCmd, const EmergencyTaskInfo* aEmergencyTask )
{
    return KErrNone;
}

int GetOrgDevice( int aOrgID, vector<DeviceInfo*>* aDeviceList )
{
    return KErrNone;
}

int SetOrgDevice( int aOrgID, int aCmd, const DeviceInfo* aDeviceList )
{
    return KErrNone;
}

int GetStaffCurrentTrack(time_t aDate, RecordStaff* aStaff)
{
    //Get the data by aStaff->iStaff->iID

    aStaff->iRecordLon.push_back(104.064631);
    aStaff->iRecordLat.push_back(30.698965);

    aStaff->iRecordLon.push_back(104.075630);
    aStaff->iRecordLat.push_back(30.699584);

    aStaff->iRecordLon.push_back(104.086426);
    aStaff->iRecordLat.push_back(30.699584);

    return KErrNone;
}

int GetStaffScheduleTrack(int aStaffID, time_t aDate, vector<double>* aRecordLon, vector<double>* aRecordLat )
{
    return KErrNone;
}

int GetPictureInfo( int aOrgID, time_t aStartDate, time_t aEndDate, vector<PictureInfo*>* aPictureList )
{
    PictureInfo* picture = new PictureInfo;
    picture->iPicID = 1;
    picture->iPicName = _T("516823.jpg");
    picture->iShootingTime = GetCurrentTime();
    picture->iErrorType = 0;

    aPictureList->push_back(picture);
    return KErrNone;
}

int SavePictureToDirect( int aOrgID, const PictureInfo* aPicture, CString aToDirect )
{
    return KErrNone;
}
