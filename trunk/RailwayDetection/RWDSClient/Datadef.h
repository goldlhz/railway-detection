
#pragma once
#include <vector>
#include <atltypes.h>
#include <atlstr.h>
#include <sys\timeb.h>

using namespace std;

extern const CString RailLineName[];
extern const int RailLineNameCount;
extern const CString DirectName[];
extern const int DirectNameCount;
extern const CString StrKm;
extern const CString StrStartNo[];
extern const int StrStartNoCount;
extern const CString StrClientCaption;

#define ENCODERAILWAYFULLNAME(aStr, aRailLine, aKm, aLineDirect)\
	aStr.Format(RailLineName[aRailLine]+_T("%.2f")+StrKm+DirectName[aLineDirect], aKm);

#define DECODERAILWAYFULLNAME(aStr, aRailLine, aKm, aLineDirect)\
	aStrLine.Delete(aStrLine.GetLength()-StrKm.GetLength(), StrKm.GetLength());\
	for (int i=0; i<RailLineNameCount; i++)\
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

enum LineStartNo
{
	KUndefine=-1,
	KFirstDay=0,
	KSecondDay,
	KThirdDay,
	KFourthDay,
	KFifthDay,
	KSixthDay,
	KSeventhDay
};

enum EmergencyStatus
{
    KNormal=0,
    KFinished
};

typedef struct _MapPoint
{
	RailLine iRailLine;
	double iKM;
	double iLon;
	double iLat;
	LineDirect iDirect;
	//vector<int> iArriveTime;
}MapPoint;

struct _Staff;

typedef struct _Line	//��·
{
	int iLineID;
	CString iLineName;
	int iOrgID;
	double iStartKm;
	LineStartNo iStartNo;//��ʼ����
	vector<MapPoint*> iLineKmLonLat;	//ÿ���ﴦ��γ��
	vector<time_t> iLineKmTime;			//ÿ���ﴦʱ��
    vector<struct _Staff*> iArrangeStaff;   //����������ҪѲ���Ա��
    CString iLineRemark;
}LineInfo;

typedef struct _Staff
{
    int iID;
    int iOrgID;
    CString iName;
    CString iPassword;
    vector<LineInfo*> iArrangeLine;
    BOOL iLoginPermission;
}StaffInfo;

typedef struct _Device	//�豸
{
	int iDevID;
	int iOrgID;
	DevState iDevState;
	double iCurrentLon;
	double iCurrentLat;
}DeviceInfo;

typedef struct _Calendar  //�Ű��
{
	int iCaledarID;
	int iOrgID;
	time_t iStartDay;
	int iPeriods;
	vector<LineInfo*>* iDateSchedule;
}CalendarSchedule;

typedef struct _OrgObj	//����
{
	int iOrgID;
	CString iOrgName;
	struct _OrgObj* iParentOrg;
	vector<int> iChildID;
	vector<struct _OrgObj*> iChildOrg;
	vector<DeviceInfo*> iDevice;
	vector<LineInfo*> iLine;
}OrganizationInfo;

typedef struct _Emergency
{
    int iTaskID;
    MapPoint* i_BeginPoint;
    MapPoint* i_EndPoint;
    time_t iBeginTime;
    time_t iEndTime;
    EmergencyStatus iStatus;
}EmergencyTaskInfo;