#include "StdAfx.h"
#include "DataListControl.h"
#include "ErrorDefine.h"


int DeleteOrgListElement( vector<OrganizationInfo*>* aOrgList )
{
    aOrgList->clear();
    return KErrNone;
}

int DeletePointListElement( vector<MapPoint*>* aMapPointList )
{
    aMapPointList->clear();
    return KErrNone;
}

int DeleteLineListElement( vector<LineInfo*>* aLineList )
{
    aLineList->clear();
    return KErrNone;
}

int DeleteStaffListElement( vector<StaffInfo*>* aStaffList )
{
    aStaffList->clear();
    return KErrNone;
}

int DeleteEmergencyListElement( vector<EmergencyTaskInfo*>* aEmergencyList )
{
    aEmergencyList->clear();
    return KErrNone;
}

int DeleteDeviceListElement( vector<DeviceInfo*>* aDeviceList )
{
    aDeviceList->clear();
    return KErrNone;
}

int DeleteReportListElement( vector<ReportInfo*>* aReportList )
{
    aReportList->clear();
    return KErrNone;
}
