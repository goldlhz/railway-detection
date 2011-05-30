#include "StdAfx.h"
#include "DataService.h"
#include "ErrorDefine.h"
#include "CmdDefine.h"


int GetOrgTree(const CString& aLoginCount, vector<OrganizationInfo*>* a_OrgTree)
{
    OrganizationInfo* org = new OrganizationInfo;
    org->iOrgName = _T("Admin");
    org->iParentOrg = NULL;
    org->iOrgID = 1;
    org->iChildID.push_back(2);
    org->iChildID.push_back(3);
    a_OrgTree->push_back(org);

    org = new OrganizationInfo;
    org->iOrgName = _T("Child1");
    org->iParentOrg = (*a_OrgTree)[0];
    (*a_OrgTree)[0]->iChildOrg.push_back(org);
    org->iOrgID = 2;
    org->iChildID.push_back(4);
    a_OrgTree->push_back(org);

    org = new OrganizationInfo;
    org->iOrgName = _T("Child2");
    org->iParentOrg = (*a_OrgTree)[0];
    (*a_OrgTree)[0]->iChildOrg.push_back(org);
    org->iOrgID = 3;
    org->iChildID.push_back(5);
    a_OrgTree->push_back(org);
    return KErrNone;
}

int GetOrgPoint(int iOrgID, vector<MapPoint*>* aPointList)
{
    if(iOrgID == 1)
    {//不是最后一级机构
        return kErrNotLastOrg;
    }
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

int GetOrgLine(int iOrgID, const vector<MapPoint*>* aPointList, vector<LineInfo*>* aLineList)
{
    if(iOrgID == 1)
    {//不是最后一级机构
        return kErrNotLastOrg;
    }
    LineInfo *line = new LineInfo;
    line->iLineID = 1;
    line->iLineName = _T("成局1段");
    line->iStartKm = (*aPointList)[0]->iKM;
    line->iStartNo = KFirstDay;
    line->iLineKmLonLat.push_back((*aPointList)[0]);
    line->iLineKmTime.push_back(100);
    line->iLineKmLonLat.push_back((*aPointList)[1]);
    line->iLineKmTime.push_back(100);
    line->iLineKmLonLat.push_back((*aPointList)[2]);
    line->iLineKmTime.push_back(100);
    line->iLineKmLonLat.push_back((*aPointList)[3]);
    line->iLineKmTime.push_back(100);
    aLineList->push_back(line);

    line = new LineInfo;
    line->iLineID = 2;
    line->iLineName = _T("成局2段");
    line->iStartKm = (*aPointList)[1]->iKM;
    line->iStartNo = KThirdDay;
    line->iLineKmLonLat.push_back((*aPointList)[1]);
    line->iLineKmTime.push_back(0);
    line->iLineKmLonLat.push_back((*aPointList)[3]);
    line->iLineKmTime.push_back(0);
    aLineList->push_back(line);
    return KErrNone;
}


int GetOrgStaff(int iOrgID, vector<StaffInfo*>* aStaffList)
{
    if(iOrgID == 1)
    {//不是最后一级机构
        return kErrNotLastOrg;
    }
    StaffInfo* staff = new StaffInfo;
    staff->iID = 1;
    staff->iOrgID = 1;
    staff->iPassword = _T("111");
    staff->iLoginPermission = TRUE;
    staff->iName = _T("张三");
    aStaffList->push_back(staff);

    staff = new StaffInfo;
    staff->iID = 2;
    staff->iOrgID = 1;
    staff->iPassword = _T("");
    staff->iLoginPermission = FALSE;
    staff->iName = _T("李四");
    aStaffList->push_back(staff);

    staff = new StaffInfo;
    staff->iID = 3;
    staff->iOrgID = 1;
    staff->iName = _T("王五");
    staff->iPassword = _T("");
    staff->iLoginPermission = FALSE;
    aStaffList->push_back(staff);
    return KErrNone;
}

int GetStaffCurrentTrack(RecordStaff* aStaff, time_t aDate)
{

    aStaff->iRecordLon.push_back(104.064631);
    aStaff->iRecordLat.push_back(30.698965);

    aStaff->iRecordLon.push_back(104.075630);
    aStaff->iRecordLat.push_back(30.699584);

    aStaff->iRecordLon.push_back(104.086426);
    aStaff->iRecordLat.push_back(30.699584);

    return KErrNone;
}

