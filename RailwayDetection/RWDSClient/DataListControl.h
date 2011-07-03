#pragma once
#include <vector>
#include "Datadef.h"
#include <stdio.h>
#include <afx.h>

#define ENABLETRACE

#ifdef ENABLETRACE
#define PRINTTRACE(a) \
    {\
    CFile logFile;\
    if(!PathFileExists(_T("log.txt")))\
    {\
        logFile.Open(_T("log.txt"), CFile::modeCreate | CFile::modeWrite);\
    }\
    else\
    {\
        logFile.Open(_T("log.txt"), CFile::modeWrite);\
        logFile.SeekToEnd();\
    }\
    logFile.Write(a, strlen(a));\
    logFile.Write(_T("\r\n"), 2);\
    logFile.Close();\
    }

#else
#define PRINTTRACE(a)
#endif

int DeleteOrgListElement(vector<OrganizationInfo*>* aOrgList);

int DeletePointListElement(vector<MapPoint*>* aMapPointList);

int DeleteLineListElement(vector<LineInfo*>* aLineList);

int DeleteStaffListElement(vector<StaffInfo*>* aStaffList);

int DeleteEmergencyListElement(vector<EmergencyTaskInfo*>* aEmergencyList);

int DeleteDeviceListElement(vector<DeviceInfo*>* aDeviceList);

int DeleteReportListElement(vector<ReportInfo*>* aReportList);
