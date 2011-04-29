
#pragma once
#include <vector>
#include <atltypes.h>
#include <atlstr.h>

using namespace std;

extern const CString RailLineName[];
extern const CString DirectName[];
extern const CString StrKm;

#define ENCODERAILWAYFULLNAME(aStr, aRailLine, aKm, aLineDirect)\
	aStr.Format(RailLineName[aRailLine]+_T("%.0f")+StrKm+DirectName[aLineDirect], aKm);

#define DECODERAILWAYFULLNAME(aStr, aRailLine, aKm, aLineDirect)\
	aStrLine.Delete(aStrLine.GetLength()-StrKm.GetLength(), StrKm.GetLength());\
	for (int i=0; i<RailLineName->GetLength(); i++)\
	{\
		if(aStrLine.Find(RailLineName[i]) == 0)\
		{\
			aRailLine = static_cast<RailLine>(i);\
			aStrLine.Delete(0, RailLineName[i].GetLength());\
			aKm = _ttoi(aStrLine);\
			break;\
		}\
	};

enum RailLine
{
	Baoji_Chengdu = 0,
	Chengdu_Kunming,
	Chengdu_Chongqing
};


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
	RailLine iRailLine;
	double iKM;
	double iLon;
	double iLat;
	LineDirect iDirect;
	vector<int> iArriveTime;
}MapPoint;

typedef struct _Line	//线路
{
	int iLineID;
	CString iLineName;
	int iOrgID;
	double iStartKm;
	vector<MapPoint*> iLineKmLonLat;	//上下行每公里处经纬度
	//vector<MapPoint> iDownlineKmLonLat;	//下行每公里处经纬度
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

