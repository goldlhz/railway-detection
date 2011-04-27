
#pragma once
#include <vector>
#include <atltypes.h>
#include <atlstr.h>

using namespace std;

enum DevState
{
	KRunning = 1,
	KClosed = 2
};

enum LineDirect
{
	KDownLine = 1,
	KUpLine = 2
};

typedef struct _MapPoint
{
	double iKM;
	double iLon;
	double iLat;
	LineDirect iDirect;
}MapPoint;

typedef struct _Line	//线路
{
	int iLineID;
	CString iLineName;
	int iOrgID;
	double iStartKm;
	vector<MapPoint> iUplineKmLonLat;	//上行每公里处经纬度
	vector<MapPoint> iDownlineKmLonLat;	//下行每公里处经纬度
}LineInfo;

typedef struct _Device	//设备
{
	int iDevID;
	int iOrgID;
	DevState iDevState;
	double iCurrentLon;
	double iCurrentLat;
}DeviceInfo;

typedef struct _Arrange	//排班表
{
	int iArrangeID;
	CString iArrangeName;
	int iWorkerID;
	DeviceInfo* iDevice;
	LineInfo* iLine;
	vector<int> iUplineKmTime;	//上行每公里处时间, 与iLine->iUplineKmLonLat对应
	vector<int> iDownlineKmTime;	//下行每公里处时间
}ArrangeLine;

typedef struct _OrgObj	//机构
{
	int iOrgID;
	CString iOrgName;
	struct _Org* iParentOrg;
	vector<struct _Org*> iChildOrg;
	vector<DeviceInfo*> iDevice;
	vector<LineInfo*> iLine;
}OrganizationInfo;

