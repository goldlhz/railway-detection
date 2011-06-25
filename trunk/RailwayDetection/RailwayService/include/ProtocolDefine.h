#pragma once
#include <string>
#include <vector>
using namespace std;

template<typename Data_Body_Type>
struct Pack_Shell_Type
{
	unsigned int   nBeginIdentify;			// ��ʼ��ʶ
	unsigned int   nMsgNumber;				// ��Ϣ��		��Ϣ��ʶ
	unsigned int   nBodyLength;				// ���ݳ���		��������������,������β��ʶ
	Data_Body_Type gDataBodyPack;			// ������		������Ϣ�Ĳ�ͬ��������
	unsigned int   nEndIdentify;			// ������ʶ
};

// GPSͨѶ  msgnum=0x02
struct GPS_UpLoad_Type
{
	unsigned int nTerminalIdentifyLength;	// ��ʶ����		�ն�Ψһ��ʶ�ĳ���
	string       strTerminalNumber;			// �ն˱�ʶ		�ն˱�ʶ���ֻ�����
	double		 nLongitude;				// ����			��λǧ���֮һ��.  ��ֵ=����*10000000����������,������������
	double		 nLatitude;					// γ��			��λǧ���֮һ��.  ��ֵ=γ��*10000000��������γ,����������γ
	unsigned int nSpeed;					// �ٶ�			��λ:km/h(����/Сʱ) ����
	unsigned int nDirection;				// ����			��ֵ=������Ϊ0��,˳ʱ��ƫ�ƵĶ���/2
	string       strTime;					// ʱ��			
											//  2�ֽ���
											//	1�ֽ���
											//	1�ֽ���
											//	1�ֽ�Сʱ
											//	1�ֽڷ�
											//	1�ֽ���

											//	D9 07 0B 1B 0B 20 30
											//	2009��11��27��11��32��48��
	unsigned int nState;					// ״̬			0������ͨ.��ʱ������,�ɹ�ͨ.
	unsigned int nMileage;					// ���			��λ��,����.   �޴�0
	unsigned int nOilMass;					// ����			��λ��,����.   �޴�0
	unsigned int nIsLocate;					// �Ƿ�λ		0����GPSģ�鶨λ ����������λ
};

struct GPS_DownLoad_Type
{
	unsigned int nTerminalIdentifyLength;	// ��ʶ����		�ն�Ψһ��ʶ�ĳ���
	string       strTerminalNumber;			// �ն˱�ʶ		�ն˱�ʶ���ֻ�����
	string		 strTime;					// ʱ��			
											//  2�ֽ���
											//	1�ֽ���
											//	1�ֽ���
											//	1�ֽ�Сʱ
											//	1�ֽڷ�
											//	1�ֽ���
};
typedef Pack_Shell_Type<GPS_UpLoad_Type>   GPS_UpLoad_Pack;
typedef Pack_Shell_Type<GPS_DownLoad_Type> GPS_DownLoad_Pack;

struct GPSPIC_Pack
{
	string strPicName;
	string strTel;
	unsigned int nType;
	string strTime;
};

// LoginͨѶ msgnum=0x0a
struct Login_Upload_Type
{
	string		 strName;					// �û���
	string		 strPassword;				// ����
};

struct Login_DownLoad_Type
{
	unsigned int nResult;					// 0�ɹ���1ʧ��
	unsigned int nPower1;
	unsigned int nPower2;
	unsigned int nPower3;
	unsigned int nOrgID;					// ������
};
typedef Pack_Shell_Type<Login_Upload_Type>   Login_UpLoad_Pack;
typedef Pack_Shell_Type<Login_DownLoad_Type> Login_DownLoad_Pack;

// ��ӻ��� msgnum=0x0b
struct AddOrg_Upload_Type
{
	string		 strOrgName;
	string		 strAddress;
	unsigned int nUpOrg;
	unsigned int nLevel;
	unsigned int nLineId;
	unsigned int nStartid;
	unsigned int nEndID;
};

struct AddOrg_Download_Type
{
	unsigned int nResult;					// 0�ɹ���1ʧ��
};
typedef Pack_Shell_Type<AddOrg_Upload_Type>   AddOrg_UpLoad_Pack;
typedef Pack_Shell_Type<AddOrg_Download_Type> AddOrg_DownLoad_Pack;

// �޸Ļ��� msgnum=0x0c
struct ModifyOrg_Upload_Type
{
	unsigned int nOrgID;
	string		 strOrgName;
	string		 strAddress;
	unsigned int nUpOrgID;
	unsigned int nLevel;
	unsigned int nLineId;
	unsigned int nStartid;
	unsigned int nEndID;
};

struct ModifyOrg_Download_Type
{
	unsigned int nResult;					// 0�ɹ���1ʧ��
};
typedef Pack_Shell_Type<ModifyOrg_Upload_Type>   ModifyOrg_UpLoad_Pack;
typedef Pack_Shell_Type<ModifyOrg_Download_Type> ModifyOrg_DownLoad_Pack;


// ɾ������ msgnum=0x0d
struct DeleteOrg_Upload_Type
{
	unsigned int nOrgID;
};

struct DeleteOrg_Download_Type
{
	unsigned int nResult;					// 0�ɹ���1ʧ��
	unsigned int nReson;					// ʧ�ܷ��� 1 ������������ 2 ������������ 3 δ֪����
};
typedef Pack_Shell_Type<DeleteOrg_Upload_Type>   DeleteOrg_UpLoad_Pack;
typedef Pack_Shell_Type<DeleteOrg_Download_Type> DeleteOrg_DownLoad_Pack;


// ��������б� msgnum=0x0e
struct AskOrgList_Upload_Type
{
	unsigned int nOrgID;					// 0��ʾȫ��������Ϣ
};

struct AskOrgList_Download_Type
{
	unsigned int nTitlePack;
	unsigned int nHadSendPack;
	unsigned int nOrgID;
	string		 strOrgName;
	string		 strAddress;
	unsigned int nUpOrgID;
	unsigned int nLevel;
	unsigned int nLineId;
	unsigned int nStartid;
	unsigned int nEndID;
};
typedef Pack_Shell_Type<AskOrgList_Upload_Type>   AskOrgList_UpLoad_Pack;
typedef Pack_Shell_Type<AskOrgList_Download_Type> AskOrgList_DownLoad_Pack;


// �����Ա msgnum=0x0f
struct AddWorker_Upload_Type
{
	string		 strOper;
	string		 strPassword;
	string		 strName;
	string	     strTel;
	unsigned int nOrgID;
	unsigned int nPowerGroup;
	unsigned int nPda1;
	unsigned int nPda2;
	unsigned int nWorkerState;				// �Ƿ������¼��1������ 2 ������
};

struct AddWorker_Download_Type
{
	unsigned int nResult;					// 0�ɹ���1ʧ��2δ֪����
};
typedef Pack_Shell_Type<AddWorker_Upload_Type>   AddWorker_UpLoad_Pack;
typedef Pack_Shell_Type<AddWorker_Download_Type> AddWorker_DownLoad_Pack;


// �޸���Ա msgnum=0x10
struct ModifyWorker_Upload_Type
{
	string		 strOper;
	string		 strName;
	string		 strTel;
	unsigned int nOrgID;
	unsigned int nPowerGroup;
	unsigned int nPda1;
	unsigned int nPda2;
	unsigned int nWorkerState;				// �Ƿ������¼��1������ 2 ������
};

struct ModifyWorker_Download_Type
{
	unsigned int nResult;					// 0�ɹ���1ʧ��
};
typedef Pack_Shell_Type<ModifyWorker_Upload_Type>   ModifyWorker_UpLoad_Pack;
typedef Pack_Shell_Type<ModifyWorker_Download_Type> ModifyWorker_DownLoad_Pack;

// ɾ����Ա msgnum=0x11
struct DeleteWorker_Upload_Type
{
	string		 strOper;
};

struct DeleteWorker_Download_Type
{
	unsigned int nResult;					// 0�ɹ���1ʧ��
};
typedef Pack_Shell_Type<DeleteWorker_Upload_Type>   DeleteWorker_UpLoad_Pack;
typedef Pack_Shell_Type<DeleteWorker_Download_Type> DeleteWorker_DownLoad_Pack;


// ��ȡ��Ա�б� msgnum=0x12
struct AskWorkerList_Upload_Type
{
	unsigned int nOrgID;
	unsigned int nType;						// 1 ����������Ա�б� 2 �����ͻ�������ȫ����Ա�б�
};

struct AskWorkerList_Download_Type
{
	unsigned int nTitlePack;
	unsigned int nHadSendPack;
	string		 strOper;
	string       strName;
	string		 strTel;
	unsigned int nOrgID;
	unsigned int nPowerGroup;
	unsigned int nPda1;
	unsigned int nPda2;
	unsigned int nWorkerState;				// �Ƿ������¼��1������ 2 ������
};
typedef Pack_Shell_Type<AskWorkerList_Upload_Type>   AskWorkerList_UpLoad_Pack;
typedef Pack_Shell_Type<AskWorkerList_Download_Type> AskWorkerList_DownLoad_Pack;


// ���Ȩ���� msgnum=0x13
struct AddPowerGroup_Upload_Type
{
	string		 strPowerName;
	unsigned int iPow1;
	unsigned int iPow2;
	unsigned int iPow3;
};

struct AddPowerGroup_Download_Type
{
	unsigned int nResult;					//0 �ɹ� 1 ʧ��
};
typedef Pack_Shell_Type<AddPowerGroup_Upload_Type>   AddPowerGroup_UpLoad_Pack;
typedef Pack_Shell_Type<AddPowerGroup_Download_Type> AddPowerGroup_DownLoad_Pack;


// �޸�Ȩ���� msgnum=0x14
struct ModifyPowerGroup_Upload_Type
{
	unsigned int nPowerID;
	string		 strPowerName;
	unsigned int nPow1;
	unsigned int nPow2;
	unsigned int nPow3;
};
struct ModifyPowerGroup_Download_Type
{
	unsigned int nResult;					// 0 �ɹ� 1 ʧ��
};
typedef Pack_Shell_Type<ModifyPowerGroup_Upload_Type>   ModifyPowerGroup_UpLoad_Pack;
typedef Pack_Shell_Type<ModifyPowerGroup_Download_Type> ModifyPowerGroup_DownLoad_Pack;


// ɾ��Ȩ���� msgnum=0x15
struct DeletePowerGroup_Upload_Type
{
	unsigned int nPowerID;
};

struct DeletePowerGroup_Download_Type
{
	unsigned int nResult;					//0 �ɹ� 1 Ȩ����������Ա 2 δ֪����
};
typedef Pack_Shell_Type<DeletePowerGroup_Upload_Type>   DeletePowerGroup_UpLoad_Pack;
typedef Pack_Shell_Type<DeletePowerGroup_Download_Type> DeletePowerGroup_DownLoad_Pack;

// ��ȡȨ�����б� msgnum=0x16
struct AskPowerGroupList_Upload_Type
{
	unsigned int nPowerID;					// 0 Ϊ��ȡȫ��Ȩ���� ����Ϊ��ȡĳ��Ȩ���� 
};

struct AskPowerGroupList_Download_Type
{
	unsigned int nTitlePack;
	unsigned int nHadSendPack;
	unsigned int nPowerID;
	string		 strPowerName;
	unsigned int nPow1;
	unsigned int nPow2;
	unsigned int nPow3;
};
typedef Pack_Shell_Type<AskPowerGroupList_Upload_Type>   AskPowerGroupList_UpLoad_Pack;
typedef Pack_Shell_Type<AskPowerGroupList_Download_Type> AskPowerGroupList_DownLoad_Pack;


// ����豸 msgnu=0x17
struct AddDevice_Upload_Type
{
	string		 strDevicePID;
	unsigned int nDeviceOrg;
	unsigned int nDeviceType;
};

struct AddDevice_Download_Type
{
	unsigned  int nResult;					//0 �ɹ�  1 ʧ�� 
};
typedef Pack_Shell_Type<AddDevice_Upload_Type>   AddDevice_UpLoad_Pack;
typedef Pack_Shell_Type<AddDevice_Download_Type> AddDevice_DownLoad_Pack;

// �޸��豸 msgnum=0x18
struct ModifyDevice_Upload_Type
{
	unsigned int nDeviceID;
	string		 strDevicePID;
	unsigned int nDeviceOrg;
	unsigned int nDeviceType;
};

struct ModifyDevice_Download_Type
{
	unsigned int nResult;					//0 �ɹ� 1 ʧ��
};
typedef Pack_Shell_Type<ModifyDevice_Upload_Type>   ModifyDevice_UpLoad_Pack;
typedef Pack_Shell_Type<ModifyDevice_Download_Type> ModifyDevice_DownLoad_Pack;


// ɾ���豸 msgnum=0x19
struct DeleteDevice_Upload_Type
{
	unsigned int nDeviceID;
};

struct DeleteDevice_Download_Type
{
	unsigned int nResult;					//0 �ɹ� 1 Ȩ����������Ա 2 δ֪����
};
typedef Pack_Shell_Type<DeleteDevice_Upload_Type>   DeleteDevice_UpLoad_Pack;
typedef Pack_Shell_Type<DeleteDevice_Download_Type> DeleteDevice_DownLoad_Pack;


// �豸�б��ȡ msgnum=0x1a
struct AskDeviceList_Upload_Type
{
	unsigned int nOrgID;
	unsigned int nType;						// 0��ȡ��������������ȫ���豸��1��ȡ��������
	unsigned int nDeivceType;				// 0ͼƬ�豸��1��Ƶ�豸��2ȫ��
};

struct AskDeviceList_Download_Type
{
	unsigned int nTitlePack;
	unsigned int nHadSendPack;
	unsigned int nDeviceID;
	string		 strDevicePID;
	unsigned int nDeviceOrg;
	unsigned int nDeviceType;
};
typedef Pack_Shell_Type<AskDeviceList_Upload_Type>   AskDeviceList_Upload_Pack;
typedef Pack_Shell_Type<AskDeviceList_Download_Type> AskDeviceList_Download_Pack;


// GPS���ȡ msgnum=0x1b
struct GetGPSPoint_Upload_Type
{
	string		 strTel;
	string		 strTime;
	string		 strEndTime;
	unsigned int nType;						// 0 lineid  1 pointid
};

struct SGPS 
{
	string		 strTime;
	string		 strJDU;
	string		 strWDU;
	string		 strTel;
};
struct GetGPSPoint_Download_Type
{
	unsigned int nTotlePack;
	unsigned int nHadSendPack;
	std::vector<SGPS> vecGPS;
};
typedef Pack_Shell_Type<GetGPSPoint_Upload_Type>   GetGPSPoint_Upload_Pack;
typedef Pack_Shell_Type<GetGPSPoint_Download_Type> GetGPSPoint_Download_Pack;


// ��ȡ��·���� msgnum=0x1c
struct GetRailWayTableName_Upload_Type
{
	unsigned int nLineID;				// 0 ��ʾȫ��
};

struct GetRailWayTableName_Download_Type
{
	unsigned int nTotlePack;
	unsigned int nHadSendPack;
	string		 strnName;
};
typedef Pack_Shell_Type<GetRailWayTableName_Upload_Type>   GetRailWayName_Upload_Pack;
typedef Pack_Shell_Type<GetRailWayTableName_Download_Type> GetRailWayName_Download_Pack;


// ��·���� msgnum=0x1d
struct SettingLine_Upload_Type 
{
	unsigned int nType;					// 0 add, 1 edit, 2 delete
	unsigned int nID;
	string		 strName;
	unsigned int nDay;
	unsigned int nOrgID;
};

struct SettingLine_Download_Type
{
	unsigned int nResult;				// 0 ok, 1 fail
};
typedef Pack_Shell_Type<SettingLine_Upload_Type>   SettingLine_Upload_Pack;
typedef Pack_Shell_Type<SettingLine_Download_Type> SettingLine_Download_Pack;


// Ѳ��ʱ������ msgnum=0x1e
struct SettingPollingTime_Upload_Type
{
	unsigned int nType;					// 0 add, 1 edit, 2 delete
	unsigned int nID;
	unsigned int nLineID;
	unsigned int nPointID;
	string		 strTime;
};

struct SettingPollingTime_Download_Type 
{
	unsigned int nResult;				// 0 ok, 1 fail
};
typedef Pack_Shell_Type<SettingPollingTime_Upload_Type>   SettingPollingTime_Upload_Pack;
typedef Pack_Shell_Type<SettingPollingTime_Download_Type> SettingPollingTime_Download_Pack;


// �Ű����� msgnum=0x1f
struct SettingSchedule5_Upload_Type
{
	unsigned int nType;						// 0 add, 1 edit, 2 delete
	unsigned int nID;
	unsigned int nCYC;
	unsigned int nOrgID;
	string		 strStart;
	string		 strContent; 
};

struct SettingSchedule5_Download_Type 
{
	unsigned int nResult;				// 0 ok, 1 fail
};
typedef Pack_Shell_Type<SettingSchedule5_Upload_Type>   SettingSchedule5_Upload_Pack;
typedef Pack_Shell_Type<SettingSchedule5_Download_Type> SettingSchedule5_Download_Pack;


// �Ű����� msgnum=0x20
struct SettingSchedule6_Upload_Type
{
	unsigned int nType;
	unsigned int nPL_ID;
	unsigned int nPL_PBID;
	unsigned int nPL_LineID;
};

struct SettingSchedule6_Download_Type 
{
	unsigned int nResult;				// 0 ok, 1 fail
};
typedef Pack_Shell_Type<SettingSchedule6_Upload_Type>   SettingSchedule6_Upload_Pack;
typedef Pack_Shell_Type<SettingSchedule6_Download_Type> SettingSchedule6_Download_Pack;



// �Ű���Ա���� msgnum=0x21
struct SettingScheduleWorker_Upload_Type
{
	 unsigned int nType;
	 unsigned int nPBID;
	 string       strRYID;
	 unsigned int nPX;
};

struct SettingScheduleWorker_Download_Type
{
	unsigned int nResult;				// 0 ok, 1 fail
};
typedef Pack_Shell_Type<SettingScheduleWorker_Upload_Type>   SettingScheduleWorker_Upload_Pack;
typedef Pack_Shell_Type<SettingScheduleWorker_Download_Type> SettingScheduleWorker_Download_Pack;

// �������񷢲� msgnum=0x22
struct UrgencyRelease_Upload_Type
{
	unsigned int nType;
	unsigned int nID;
	unsigned int nStartPointID;
	unsigned int nEndPointID;
	string		 strStartTime;
	string		 strEndTime;
	unsigned int nState;
	unsigned int nLineID;
	unsigned int nOrgID;
};

struct UrgencyRelease_Download_Type 
{
	unsigned int nResult;				// 0 ok, 1 fail
};
typedef Pack_Shell_Type<UrgencyRelease_Upload_Type>   UrgencyRelease_Upload_Pack;
typedef Pack_Shell_Type<UrgencyRelease_Download_Type> UrgencyRelease_Download_Pack;

// ����������Ա msgnum=0x23
struct UrgencyWorker_Upload_Type 
{
	unsigned int nType;
	unsigned int nID;
	unsigned int nJJ_ID;
	unsigned int nJJ_PID;
	unsigned int nJJ_PDevice;
};

struct UrgencyWorker_Download_Type 
{
	unsigned int nResult;				// 0 ok, 1 fail
};
typedef Pack_Shell_Type<UrgencyWorker_Upload_Type>   UrgencyWorker_Upload_Pack;
typedef Pack_Shell_Type<UrgencyWorker_Download_Type> UrgencyWorker_Download_Pack;

// ��ȡ��·�� msgnum=0x25
struct GetLinePoint_Upload_Type
{
	unsigned int nOrgID;
};

struct GetLinePoint_Download_Type
{
	unsigned int nTotalPacket;
	unsigned int nCurrentPacket;
	unsigned int nPointID;
	float		 fJDU;
	float		 fWDU;
	float		 fGls;
	unsigned int nState;
	unsigned int nLineID;
};
typedef Pack_Shell_Type<GetLinePoint_Upload_Type>   GetLinePoint_Upload_Pack;
typedef Pack_Shell_Type<GetLinePoint_Download_Type> GetLinePoint_Download_Pack;

// ��ȡͼƬ msgnum=0x26
struct GetPicList_Upload_Type
{
	unsigned int nOrgID;
	string		 strSTime;
	string		 strETime;
};

struct GetPicList_Download_Type
{
	unsigned int nTotalPacket;
	unsigned int nCurrentPacket;
	string		 strName;
	string		 strTime;
	unsigned int nType;
	string		 strTel;
};
typedef Pack_Shell_Type<GetPicList_Upload_Type>   GetPicList_Upload_Pack;
typedef Pack_Shell_Type<GetPicList_Download_Type> GetPicList_Download_Pack;

// ��ȡ���������б� msgnum = 0x27
struct GetUrgencyMissionList_Upload_Type
{
	unsigned int nOrgID;
};

struct GetUrgencyMissionList_Download_Type
{
	unsigned  int nTtlePacket;
	unsigned  int nCurrentPacket;
	string		  strSTime;
	string		  strETime;
	unsigned  int nState;
	unsigned  int nID;
	unsigned  int nLineID;
	unsigned  int nSPoint;
	unsigned  int nEPoint;
};
typedef Pack_Shell_Type<GetUrgencyMissionList_Upload_Type> GetUrgencyMissionList_Upload_Pack;
typedef Pack_Shell_Type<GetUrgencyMissionList_Download_Type> GetUrgencyMissionList_Download_Pack;

// ��ȡ����������Ա�б� msgnum=0x28
struct GetUrgencyWorkerList_Upload_Type
{
	unsigned int nJJID;
};

struct GetUrgencyWorkerList_Download_Type
{
	unsigned  int nTtlePacket;
	unsigned  int nCurrentPacket;
	string		  strJj_Pid;
	string		  strJj_Time;
	unsigned  int nJj_long;
};
typedef Pack_Shell_Type<GetUrgencyWorkerList_Upload_Type> GetUrgencyWorkerList_Upload_Pack;
typedef Pack_Shell_Type<GetUrgencyWorkerList_Download_Type> GetUrgencyWorkerList_Download_Pack;

// ͼ���ȡ msgnum=0x29
struct GetPicData_Upload_Type
{
	string		strPicName;
};

struct GetPicData_Download_Type
{
	unsigned  int nTtlePacket;
	unsigned  int nCurrentPacket;
	unsigned  int nPagesize;
	char		  picBuffer[1024*3];
};
typedef Pack_Shell_Type<GetPicData_Upload_Type> GetPicData_Upload_Pack;
typedef Pack_Shell_Type<GetPicData_Download_Type> GetPicData_Download_Pack;

// �����Ű��б� msgnum=0x2a
struct GetOrgSchedueList_Upload_Type
{
	unsigned  int nOrgID;
	unsigned  int nYears;
	unsigned  int nMonths;
};

struct GetOrgSchedueList_Download_Type
{
	string		strUserID;
	string		strDates;
	string		strXJ;
	unsigned  int nSTotlePoint;
	unsigned  int nSGetPoint;
	unsigned  int nCPoint;
	unsigned  int nLPoint;
};
typedef Pack_Shell_Type<GetOrgSchedueList_Upload_Type> GetOrgSchedueList_Upload_Pack;
typedef Pack_Shell_Type<GetOrgSchedueList_Download_Type> GetOrgSchedueList_Download_Pack;

// �����Ű���Ϣ msgnum=0x2b
struct GetOrgSchedueInfo_Upload_Type
{
	unsigned int nOrgID;
};

struct GetOrgSchedueInfo_Download_Type
{
	unsigned int nID;
	unsigned int nCYC;
	string   strTime;
	string   strContent;
};
typedef Pack_Shell_Type<GetOrgSchedueInfo_Upload_Type> GetOrgSchedueInfo_Upload_Pack;
typedef Pack_Shell_Type<GetOrgSchedueInfo_Download_Type> GetOrgSchedueInfo_Download_Pack;

// ������msgnum=0x2c
struct SetPoint_Upload_Type
{
	unsigned  int nIType;//itype; // 0 ��� 1 �޸� 2 ɾ��
	unsigned  int nPointId;//�޸ġ�ɾ��ʱ��Ч
	unsigned  int nIRailLine;//��·id
	double		  dIKM;//���ﴦ
	double		  dILon;//����
	double		  dILat;//γ��
	int			  nIDirect;//�����б�־
};

struct SetPoint_Download_Type
{
	unsigned  int nResult;//�޸Ľ�� 0�ɹ� 1ʧ��
};
typedef Pack_Shell_Type<SetPoint_Upload_Type> SetPoint_Upload_Pack;
typedef Pack_Shell_Type<SetPoint_Download_Type> SetPoint_Download_Pack;

// ��ȡ������·msgnum=0x2d
struct GetOrgLine_Upload_Type
{
	unsigned  int nOrgID;
};

struct GetOrgLine_Download_Type
{
	unsigned  int nTtotlePacket;
	unsigned  int nCurrentPacket;
	unsigned  int nLineID;
	unsigned  int nLineDay;
	string		  strLineName;
};
typedef Pack_Shell_Type<GetOrgLine_Upload_Type> GetOrgLine_Upload_Pack;
typedef Pack_Shell_Type<GetOrgLine_Download_Type> GetOrgLine_Download_Pack;

// ��ȡ��·�ĵ�ʱ��msgnum=0x2e
struct GetLinePointTime_Upload_Type
{
	unsigned  int nLineID;
};
struct GetLinePointTime_Download_Type
{
	unsigned  int nTotlePacket;
	unsigned  int nCurrentPacket;
	float		  fIKM;//
	float		  fILon;
	float		  fILat;
	int			  nIDirect;//
	int			  nPointID;
	string		  strTime;
};
typedef Pack_Shell_Type<GetLinePointTime_Upload_Type> GetLinePointTime_Upload_Pack;
typedef Pack_Shell_Type<GetLinePointTime_Download_Type> GetLinePointTime_Download_Pack;

// ��ȡ���������Ű���Աmsgnum=0x2f
struct GetOrgSchWorker_Upload_Type
{
	unsigned  int nOrgID;
};
struct GetOrgSchWorker_Download_Type
{
	unsigned  int nTotle;//һ�����ٰ�
	unsigned  int nHasRecv;//�Ѿ����ͱ���
	string		  strOper;
	string		  strName;
	string		  strTel;
	unsigned int  nOrgID;
	unsigned int  nPowerGroup;
	unsigned int  nPda1;
	unsigned int  nPda2;
	unsigned int  nWorkerState;// �Ƿ������¼��1������ 2 ������
};
typedef Pack_Shell_Type<GetOrgSchWorker_Upload_Type> GetOrgSchWorker_Upload_Pack;
typedef Pack_Shell_Type<GetOrgSchWorker_Download_Type> GetOrgSchWorker_Download_Pack;
//////////////////////////////////////////////////////////////////////////
///
/// ���ݰ����ͺ궨��
//////////////////////////////////////////////////////////////////////////
#define PACK_IDENTIFY					0XFFFFFFFF

#define GPS_PACK						0X02
#define LOGIN_PACK						0X0A
#define ADDORG_PACK						0X0B
#define MODIFYORG_PACK					0X0C
#define DELETEORG_PACK					0X0D
#define ASKORGLIST_PACK					0X0E
#define ADDWORKER_PACK					0X0F
#define MODIFYWORKER_PACK				0X10
#define DELETEWORKER_PACK				0X11
#define ASKWORKERLIST_PACK				0X12
#define ADDPOWERGROUP_PACK				0X13
#define MODIFYPOWERGROUP_PACK			0X14
#define DELETEPOWERGROUP_PACK			0X15
#define ASKPOWERGROUPLIST_PACK			0X16
#define ADDDEVICE_PACK					0X17
#define MODIFYDEVICE_PACK				0X18
#define DELETEDEVICE_PACK				0X19
#define ASKDEIVCELIST_PACK				0X1A
#define GETGPSPOINT_PACK				0X1B
#define GETRAILWAYTABLENAME_PACK		0X1C
#define SETTINGLINE_PACK				0X1D
#define SETTINGPOLLINGTIME_PACK			0X1E
#define SETTINGSCHEDULE5_PACK			0X1F
#define SETTINGSCHEDULE6_PACK			0X20
#define SETTINGSCHEDULEWORKER_PACK		0X21
#define URGENCYRELEASE_PACK				0X22
#define URGENCYWORKER_PACK				0X23
#define GETLINEPOINT_PACK				0X25
#define GETPICLIST_PACK					0X26
#define	GETURGENCYMISSIONLIST_PACK		0X27
#define GETURGENCYWORKERLIST_PACK		0X28
#define GETPICDATA_PACK					0X29
#define GETORGSCHEDUELIST_PACK			0X2A
#define GETORGSCHEDUEINFO_PACK			0X2B
#define SETPOINT_PACK					0X2C
#define GETORGLINE_PACK					0X2D
#define	GETLINEPOINTTIME_PACK			0X2E
#define GETORGSCHWORKER_PACK			0X2F