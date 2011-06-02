
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
    _MapPoint()
    {
        iRailLine = Baoji_Chengdu;
        iKM = 0.0;
        iLon = 0.0;
        iLat = 0.0;
        iDirect = KDownLine;
    }
	RailLine iRailLine;//铁路线
	double iKM;//公里处
	double iLon;
	double iLat;
	LineDirect iDirect;//上下行
}MapPoint;

struct _Staff;

typedef struct _Line	//线路
{
    _Line()
    {
        iLineID = 0;
        iLineName = _T("");
        iOrgID = 0;
        iStartKm = 0.0;
        iStartNo = KFirstDay;
        iLineRemark = _T("");
    }
	int iLineID;
	CString iLineName;
	int iOrgID;//所属机构
	double iStartKm;
	LineStartNo iStartNo;//开始天数
	vector<MapPoint*> iLineKmLonLat;	//每公里处经纬度
	vector<time_t> iLineKmTime;			//每公里处时间
    vector<struct _Staff*> iArrangeStaff;   //该线上所有要巡查的员工
    CString iLineRemark;//备注
}LineInfo;

typedef struct _Staff
{
    int iID;
    int iOrgID;
    CString iName;
    CString iPassword;
    vector<LineInfo*> iArrangeLine;//巡查路线
    BOOL iLoginPermission;
}StaffInfo;

typedef struct _Device	//设备
{
	int iDevID;
	int iOrgID;//所属机构
	DevState iDevState;
	double iCurrentLon;//当前经度
	double iCurrentLat;//当前纬度
}DeviceInfo;

typedef struct _Calendar  //排班表
{
    _Calendar()
    {
        iCaledarID = 0;
        iOrgID = 0;
        iStartDay = 0;
        iPeriods = 0;
        iDateSchedule = NULL;
    }
	int iCaledarID;
	int iOrgID;
	time_t iStartDay;//开始日期
	int iPeriods;//周期
	vector<LineInfo*>* iDateSchedule;//该排班下的路线集
}CalendarSchedule;

typedef struct _OrgObj	//机构
{
	int iOrgID;
	CString iOrgName;
	struct _OrgObj* iParentOrg;//上级机构
	vector<int> iChildID;
	vector<struct _OrgObj*> iChildOrg;//直接下级机构
	vector<DeviceInfo*> iDevice;//该机构拥有的设备
	vector<LineInfo*> iLine;//该机构配置的路线，若不为最后一级，则为NULL
}OrganizationInfo;

typedef struct _Emergency
{
    int iTaskID;
    CString iTaskName;
    MapPoint* iBeginKm;//开始处
    MapPoint* iEndKm;//终点
    time_t iBeginTime;//开始处时间
    time_t iEndTime;//终点处时间
    EmergencyStatus iStatus;//0正常，1结束
    CString iEmergencyRemark;
    vector<StaffInfo*> iAppointStaff;
}EmergencyTaskInfo;

typedef struct _RecordStaff//员工所巡查记录
{
    _RecordStaff()
    {
    iStaff = NULL;
    }
    StaffInfo* iStaff;
    vector<double> iRecordLon;
    vector<double> iRecordLat;
}RecordStaff;
