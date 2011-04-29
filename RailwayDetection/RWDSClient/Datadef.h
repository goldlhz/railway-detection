
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

typedef struct _Line	//��·
{
	int iLineID;
	CString iLineName;
	int iOrgID;
	double iStartKm;
	vector<MapPoint*> iLineKmLonLat;	//������ÿ���ﴦ��γ��
	//vector<MapPoint> iDownlineKmLonLat;	//����ÿ���ﴦ��γ��
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

