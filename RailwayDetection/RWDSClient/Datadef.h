
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

typedef struct _Line	//��·
{
	int iLineID;
	CString iLineName;
	int iOrgID;
	double iStartKm;
	vector<MapPoint> iUplineKmLonLat;	//����ÿ���ﴦ��γ��
	vector<MapPoint> iDownlineKmLonLat;	//����ÿ���ﴦ��γ��
}LineInfo;

typedef struct _Device	//�豸
{
	int iDevID;
	int iOrgID;
	DevState iDevState;
	double iCurrentLon;
	double iCurrentLat;
}DeviceInfo;

typedef struct _Arrange	//�Ű��
{
	int iArrangeID;
	CString iArrangeName;
	int iWorkerID;
	DeviceInfo* iDevice;
	LineInfo* iLine;
	vector<int> iUplineKmTime;	//����ÿ���ﴦʱ��, ��iLine->iUplineKmLonLat��Ӧ
	vector<int> iDownlineKmTime;	//����ÿ���ﴦʱ��
}ArrangeLine;

typedef struct _OrgObj	//����
{
	int iOrgID;
	CString iOrgName;
	struct _Org* iParentOrg;
	vector<struct _Org*> iChildOrg;
	vector<DeviceInfo*> iDevice;
	vector<LineInfo*> iLine;
}OrganizationInfo;

