
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
}Permission;

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
	RailLine iRailLine;//��·��
	double iKM;//���ﴦ
	double iLon;
	double iLat;
	LineDirect iDirect;//������
	int   iPointId;//��id �������ݿ�����
}MapPoint;

struct _Staff;

typedef struct _Line	//��·
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
	int iOrgID;//��������
	double iStartKm;
	LineStartNo iStartNo;//��ʼ����
	vector<MapPoint*> iLineKmLonLat;	//ÿ���ﴦ��γ��
	vector<time_t> iLineKmTime;			//ÿ���ﴦʱ��
    CString iLineRemark;//��ע
}LineInfo;

typedef struct _Device	//�豸
{
    _Device()
    {
        iDevID = 0;
        iOrgID = 0;
        iDeviceType = KPicture;
    }
    int iDevID;
    int iOrgID;//��������
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
    //vector<LineInfo*> iArrangeLine;//Ѳ��·��
    BOOL iLoginPermission;
    DeviceInfo* iTakeDevice;
    //Permission iPermission;
	int Pda1;
	int Pda2;//�豸id��
	int iPermissionGroup;
}StaffInfo;

typedef struct _Calendar  //�Ű��
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
	time_t iStartDay;//��ʼ����
	int iPeriods;//����
	vector<StaffInfo*> iScheduleStaff;
	CString iLineRemark;//��ע
}CalendarSchedule;

typedef struct _Emergency
{
    int iTaskID;
    CString iTaskName;
    MapPoint* iBeginKm;//��ʼ��
    MapPoint* iEndKm;//�յ�
    time_t iBeginTime;//��ʼ��ʱ��
    time_t iEndTime;//�յ㴦ʱ��
    EmergencyStatus iStatus;//0������1����
    CString iEmergencyRemark;
    vector<StaffInfo*> iAppointStaff;
}EmergencyTaskInfo;

typedef struct _OrgObj	//����
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
	int iOrgID;
    int iOrgLevel;
	CString iOrgName;
    CString iOrgAddress;
    int iParentID;
	struct _OrgObj* iParentOrg;//�ϼ�����
	vector<int> iChildID;
	vector<struct _OrgObj*> iChildOrg;//ֱ���¼�����
    RailLine iBoundaryRail;
    double iBoundaryStartKM;
    double iBoundaryEndKM;
    vector<MapPoint*> iMapPoint;
    vector<StaffInfo*> iStaff;//�û�������Ա
	vector<DeviceInfo*> iDevice;//�û���ӵ�е��豸
	vector<LineInfo*> iLine;//�û������õ�·�ߣ�����Ϊ���һ������ΪNULL
    CalendarSchedule* iCalendar;//�û������Ű��
    vector<EmergencyTaskInfo*> iEmergency;
    BOOL iDataSet;
}OrganizationInfo;


typedef struct _RecordStaff//Ա����Ѳ���¼
{
    _RecordStaff()
    {
    iStaff = NULL;
    }
    StaffInfo* iStaff;
    vector<double> iRecordLon;
    vector<double> iRecordLat;
}RecordStaff;

typedef struct _PictureInfo//ͼƬ��Ϣ
{
	int iPicID;
	CString iPicName;
	CString iShootingTime;
	int iErrorType;
}PictureInfo;


