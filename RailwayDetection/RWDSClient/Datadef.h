
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
	RailLine iRailLine;//��·��
	double iKM;//���ﴦ
	double iLon;
	double iLat;
	LineDirect iDirect;//������
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
    vector<struct _Staff*> iArrangeStaff;   //����������ҪѲ���Ա��
    CString iLineRemark;//��ע
}LineInfo;

typedef struct _Staff
{
    int iID;
    int iOrgID;
    CString iName;
    CString iPassword;
    vector<LineInfo*> iArrangeLine;//Ѳ��·��
    BOOL iLoginPermission;
}StaffInfo;

typedef struct _Device	//�豸
{
	int iDevID;
	int iOrgID;//��������
	DevState iDevState;
	double iCurrentLon;//��ǰ����
	double iCurrentLat;//��ǰγ��
}DeviceInfo;

typedef struct _Calendar  //�Ű��
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
	time_t iStartDay;//��ʼ����
	int iPeriods;//����
	vector<LineInfo*>* iDateSchedule;//���Ű��µ�·�߼�
}CalendarSchedule;

typedef struct _OrgObj	//����
{
	int iOrgID;
	CString iOrgName;
	struct _OrgObj* iParentOrg;//�ϼ�����
	vector<int> iChildID;
	vector<struct _OrgObj*> iChildOrg;//ֱ���¼�����
	vector<DeviceInfo*> iDevice;//�û���ӵ�е��豸
	vector<LineInfo*> iLine;//�û������õ�·�ߣ�����Ϊ���һ������ΪNULL
}OrganizationInfo;

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
