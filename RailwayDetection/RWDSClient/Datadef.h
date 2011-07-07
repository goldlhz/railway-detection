
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
extern const CString strPictureErrorType[];
extern const int strPictureErrorTypeCount;
extern const CString strPointState[];
extern const int strPointStateCount;

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
	KUndefine=0,
	KFirstDay,
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

enum DeviceType
{
    KPicture = 1,
    KVideo
};

enum PictureErrorType
{
    KUnkownType = 0
};

enum PointState
{
    KPointNormal = 0
};

#define PERMISSIONSETPOINT 0x0001
#define PERMISSIONSETLINE 0x0002
#define PERMISSIONSETSTAFF 0x0004
#define PERMISSIONSCHEDULE 0x0008
#define PERMISSIONSETEMERGENCY 0x0010
#define PERMISSIONSETORG 0x0020
#define PERMISSIONSETDEVICE 0x0040
#define PERMISSIONSETPERMISSIONGROUP 0x0080
#define PERMISSIONVIEWPICTURE 0x0100
#define PERMISSIONVIEWVOICE 0x0200
#define PERMISSIONVIEWRECORDE 0x0400
#define PERMISSIONVIEWREPORT 0x0800

typedef struct _Single
{
    _Single()
    {
        iBasical = 0;
        iOperate = 0;
        iReportForm = 0;
    }
    int iBasical;
    int iOperate;
    int iReportForm;
}PermissionGroup;

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
	int   iPointId;//点id 对于数据库数据
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
    CString iLineRemark;//备注
}LineInfo;

typedef struct _Device	//设备
{
    _Device()
    {
        iDevID = 0;
        iOrgID = 0;
        iDeviceType = KPicture;
    }
    int iDevID;
    int iOrgID;//所属机构
    CString iPhoneNum;
    DeviceType iDeviceType;
}DeviceInfo;


typedef struct _Staff
{
    _Staff()
    {
       // iID = 0;
        iOrgID = 0;
        iLoginPermission = FALSE;
        iTakeDevice = NULL;
    }
    CString iID;
    int iOrgID;
    CString iName;
    CString iPassword;
    //vector<LineInfo*> iArrangeLine;//巡查路线
    BOOL iLoginPermission;
    DeviceInfo* iTakeDevice;    //所持设备
    //int iDevID;
    //Permission iPermission;
	//int Pda1;
	//int Pda2;
	int iPermissionGroup;
}StaffInfo;

typedef struct _Calendar  //排班表
{
    _Calendar()
    {
        iCaledarID = 0;
        iOrgID = 0;
        iStartDay = 0;
        iPeriods = 0;
    }
	int iCaledarID;
	int iOrgID;
	time_t iStartDay;//开始日期
	int iPeriods;//周期
	vector<StaffInfo*> iScheduleStaff;
	CString iScheduleRemark;//备注
}CalendarSchedule;

typedef struct _Emergency
{
    int iTaskID;
    CString iTaskName;
    RailLine iLineName;
    double iBeginKm;//开始处
    double iEndKm;//终点
    time_t iBeginTime;//开始处时间
    time_t iEndTime;//终点处时间
    EmergencyStatus iStatus;//0正常，1结束
    CString iEmergencyRemark;
    vector<int> iStaffOrgID;
    vector<CString> iStaffID;
    vector<CString> iStaffName;
}EmergencyTaskInfo;

typedef struct _EmergencyLogs//紧急任务的统计信息
{
    int iTaskID;//
    int iStaffOrgID;
    CString iStaffID;
    CString iStaffName;
    unsigned int iTotalTime;
    double iTotalKM;
}EmergencyLogs;

typedef struct _RecordStaff//员工所巡查流水
{
    _RecordStaff()
    {
        //iStaff = NULL;
    }
    //StaffInfo* iStaff;
    //vector<double>
    CString iStaffID;
    vector<double> iRecordLon;
    vector<double> iRecordLat;
    vector<CString> iArrivedTime;
}RecordStaff;

typedef struct _ReportDetail//日明细
{
    CString iDay;
    vector<CString> iPlanArrivedTime;
    vector<CString> iActualArrivedTime;
    vector<PointState> iState;
}ReportDetail;

typedef struct _ReportInfo//机构月报表
{
    int iOrgID;
    CString iStaffName;
    time_t iReportDay;
    int iPlanArrived;//计划到达数
    int iActualArrived;//实际到达数
    int iAbnormal;//异常数
    int iUnArrived;//未到达
}ReportInfo;

typedef struct _OrgObj	//机构
{
    inline _OrgObj()
    {
        iOrgID = 0;
        iOrgLevel = 0;
        iOrgName = _T("");
        iOrgAddress = _T("");
        iParentOrg = NULL;
        iDataSet = FALSE;
        iBoundaryStartKM = 0;
        iBoundaryEndKM = 0;
        iCalendar = new CalendarSchedule;
    }
    //机构基本信息
	int iOrgID;
    int iOrgLevel;
	CString iOrgName;
    CString iOrgAddress;
    int iParentID;
	struct _OrgObj* iParentOrg;//上级机构
	vector<int> iChildID;
	vector<struct _OrgObj*> iChildOrg;//直接下级机构
    RailLine iBoundaryRail;
    unsigned int iBoundaryStartKM;
    unsigned int iBoundaryEndKM;
    BOOL iDataSet;//机构基本信息是否已经获取

    //机构附属信息
    vector<MapPoint*> iMapPoint;
    vector<StaffInfo*> iStaff;//该机构的人员
	vector<DeviceInfo*> iDevice;//该机构拥有的设备
	vector<LineInfo*> iLine;//该机构配置的路线，若不为最后一级，则为NULL
    CalendarSchedule* iCalendar;//该机构的排班表
    vector<EmergencyTaskInfo*> iEmergency;
    vector<ReportInfo*> iReportInfo;
}OrganizationInfo;




typedef struct _PictureInfo//图片信息
{
	unsigned int iPicID;
	CString iPicName;
	CString iShootingTime;
	unsigned int iErrorType;
}PictureInfo;


///机构月报表
typedef struct
{
	unsigned  int orgid;
	unsigned  int yesrs;
	unsigned  int months;
}GetOrgPxEx;

typedef struct
{
	CString userid;
	CString dates;
	CString xj;
	unsigned  int sTotlePoint;//总点数
	unsigned  int sGetPoint;//到达点数 正常
	unsigned  int cPoint;//未正常到达点
	unsigned  int lPoint;//没有达到点
}getorgpxlistresultEx;
//typedef vector <getorgpxlistresult> lOrgMonth;
//typedef lOrgMonth::iterator iterOrgMonth;
