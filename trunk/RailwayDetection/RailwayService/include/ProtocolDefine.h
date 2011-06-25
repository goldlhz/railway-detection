#pragma once
#include <string>
#include <vector>
using namespace std;

template<typename Data_Body_Type>
struct Pack_Shell_Type
{
	unsigned int   nBeginIdentify;			// 开始标识
	unsigned int   nMsgNumber;				// 消息号		消息标识
	unsigned int   nBodyLength;				// 数据长度		后续数据区长度,不含结尾标识
	Data_Body_Type gDataBodyPack;			// 数据区		依赖消息的不同数据内容
	unsigned int   nEndIdentify;			// 结束标识
};

// GPS通讯  msgnum=0x02
struct GPS_UpLoad_Type
{
	unsigned int nTerminalIdentifyLength;	// 标识长度		终端唯一标识的长度
	string       strTerminalNumber;			// 终端标识		终端标识即手机号码
	double		 nLongitude;				// 经度			单位千万分之一度.  该值=经度*10000000正数代表东经,负数代表西经
	double		 nLatitude;					// 纬度			单位千万分之一度.  该值=纬度*10000000正数代表北纬,负数代表南纬
	unsigned int nSpeed;					// 速度			单位:km/h(公里/小时) 整数
	unsigned int nDirection;				// 方向			该值=以正北为0度,顺时针偏移的度数/2
	string       strTime;					// 时间			
											//  2字节年
											//	1字节月
											//	1字节日
											//	1字节小时
											//	1字节分
											//	1字节秒

											//	D9 07 0B 1B 0B 20 30
											//	2009年11月27号11点32分48秒
	unsigned int nState;					// 状态			0代表普通.暂时无其它,可沟通.
	unsigned int nMileage;					// 里程			单位米,整数.   无传0
	unsigned int nOilMass;					// 油量			单位升,整数.   无传0
	unsigned int nIsLocate;					// 是否定位		0代表GPS模块定位 其它代表不定位
};

struct GPS_DownLoad_Type
{
	unsigned int nTerminalIdentifyLength;	// 标识长度		终端唯一标识的长度
	string       strTerminalNumber;			// 终端标识		终端标识即手机号码
	string		 strTime;					// 时间			
											//  2字节年
											//	1字节月
											//	1字节日
											//	1字节小时
											//	1字节分
											//	1字节秒
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

// Login通讯 msgnum=0x0a
struct Login_Upload_Type
{
	string		 strName;					// 用户名
	string		 strPassword;				// 密码
};

struct Login_DownLoad_Type
{
	unsigned int nResult;					// 0成功或1失败
	unsigned int nPower1;
	unsigned int nPower2;
	unsigned int nPower3;
	unsigned int nOrgID;					// 机构号
};
typedef Pack_Shell_Type<Login_Upload_Type>   Login_UpLoad_Pack;
typedef Pack_Shell_Type<Login_DownLoad_Type> Login_DownLoad_Pack;

// 添加机构 msgnum=0x0b
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
	unsigned int nResult;					// 0成功或1失败
};
typedef Pack_Shell_Type<AddOrg_Upload_Type>   AddOrg_UpLoad_Pack;
typedef Pack_Shell_Type<AddOrg_Download_Type> AddOrg_DownLoad_Pack;

// 修改机构 msgnum=0x0c
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
	unsigned int nResult;					// 0成功或1失败
};
typedef Pack_Shell_Type<ModifyOrg_Upload_Type>   ModifyOrg_UpLoad_Pack;
typedef Pack_Shell_Type<ModifyOrg_Download_Type> ModifyOrg_DownLoad_Pack;


// 删除机构 msgnum=0x0d
struct DeleteOrg_Upload_Type
{
	unsigned int nOrgID;
};

struct DeleteOrg_Download_Type
{
	unsigned int nResult;					// 0成功或1失败
	unsigned int nReson;					// 失败返回 1 包含下属机构 2 机构下属有人 3 未知错误
};
typedef Pack_Shell_Type<DeleteOrg_Upload_Type>   DeleteOrg_UpLoad_Pack;
typedef Pack_Shell_Type<DeleteOrg_Download_Type> DeleteOrg_DownLoad_Pack;


// 请求机构列表 msgnum=0x0e
struct AskOrgList_Upload_Type
{
	unsigned int nOrgID;					// 0表示全部机构信息
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


// 添加人员 msgnum=0x0f
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
	unsigned int nWorkerState;				// 是否允许登录（1、允许 2 不允许）
};

struct AddWorker_Download_Type
{
	unsigned int nResult;					// 0成功或1失败2未知错误
};
typedef Pack_Shell_Type<AddWorker_Upload_Type>   AddWorker_UpLoad_Pack;
typedef Pack_Shell_Type<AddWorker_Download_Type> AddWorker_DownLoad_Pack;


// 修改人员 msgnum=0x10
struct ModifyWorker_Upload_Type
{
	string		 strOper;
	string		 strName;
	string		 strTel;
	unsigned int nOrgID;
	unsigned int nPowerGroup;
	unsigned int nPda1;
	unsigned int nPda2;
	unsigned int nWorkerState;				// 是否允许登录（1、允许 2 不允许）
};

struct ModifyWorker_Download_Type
{
	unsigned int nResult;					// 0成功或1失败
};
typedef Pack_Shell_Type<ModifyWorker_Upload_Type>   ModifyWorker_UpLoad_Pack;
typedef Pack_Shell_Type<ModifyWorker_Download_Type> ModifyWorker_DownLoad_Pack;

// 删除人员 msgnum=0x11
struct DeleteWorker_Upload_Type
{
	string		 strOper;
};

struct DeleteWorker_Download_Type
{
	unsigned int nResult;					// 0成功或1失败
};
typedef Pack_Shell_Type<DeleteWorker_Upload_Type>   DeleteWorker_UpLoad_Pack;
typedef Pack_Shell_Type<DeleteWorker_Download_Type> DeleteWorker_DownLoad_Pack;


// 获取人员列表 msgnum=0x12
struct AskWorkerList_Upload_Type
{
	unsigned int nOrgID;
	unsigned int nType;						// 1 机构下属人员列表 2 机构和机构下属全部人员列表
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
	unsigned int nWorkerState;				// 是否允许登录（1、允许 2 不允许）
};
typedef Pack_Shell_Type<AskWorkerList_Upload_Type>   AskWorkerList_UpLoad_Pack;
typedef Pack_Shell_Type<AskWorkerList_Download_Type> AskWorkerList_DownLoad_Pack;


// 添加权限组 msgnum=0x13
struct AddPowerGroup_Upload_Type
{
	string		 strPowerName;
	unsigned int iPow1;
	unsigned int iPow2;
	unsigned int iPow3;
};

struct AddPowerGroup_Download_Type
{
	unsigned int nResult;					//0 成功 1 失败
};
typedef Pack_Shell_Type<AddPowerGroup_Upload_Type>   AddPowerGroup_UpLoad_Pack;
typedef Pack_Shell_Type<AddPowerGroup_Download_Type> AddPowerGroup_DownLoad_Pack;


// 修改权限组 msgnum=0x14
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
	unsigned int nResult;					// 0 成功 1 失败
};
typedef Pack_Shell_Type<ModifyPowerGroup_Upload_Type>   ModifyPowerGroup_UpLoad_Pack;
typedef Pack_Shell_Type<ModifyPowerGroup_Download_Type> ModifyPowerGroup_DownLoad_Pack;


// 删除权限组 msgnum=0x15
struct DeletePowerGroup_Upload_Type
{
	unsigned int nPowerID;
};

struct DeletePowerGroup_Download_Type
{
	unsigned int nResult;					//0 成功 1 权限组下有人员 2 未知错误
};
typedef Pack_Shell_Type<DeletePowerGroup_Upload_Type>   DeletePowerGroup_UpLoad_Pack;
typedef Pack_Shell_Type<DeletePowerGroup_Download_Type> DeletePowerGroup_DownLoad_Pack;

// 获取权限组列表 msgnum=0x16
struct AskPowerGroupList_Upload_Type
{
	unsigned int nPowerID;					// 0 为获取全部权限组 其他为获取某个权限组 
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


// 添加设备 msgnu=0x17
struct AddDevice_Upload_Type
{
	string		 strDevicePID;
	unsigned int nDeviceOrg;
	unsigned int nDeviceType;
};

struct AddDevice_Download_Type
{
	unsigned  int nResult;					//0 成功  1 失败 
};
typedef Pack_Shell_Type<AddDevice_Upload_Type>   AddDevice_UpLoad_Pack;
typedef Pack_Shell_Type<AddDevice_Download_Type> AddDevice_DownLoad_Pack;

// 修改设备 msgnum=0x18
struct ModifyDevice_Upload_Type
{
	unsigned int nDeviceID;
	string		 strDevicePID;
	unsigned int nDeviceOrg;
	unsigned int nDeviceType;
};

struct ModifyDevice_Download_Type
{
	unsigned int nResult;					//0 成功 1 失败
};
typedef Pack_Shell_Type<ModifyDevice_Upload_Type>   ModifyDevice_UpLoad_Pack;
typedef Pack_Shell_Type<ModifyDevice_Download_Type> ModifyDevice_DownLoad_Pack;


// 删除设备 msgnum=0x19
struct DeleteDevice_Upload_Type
{
	unsigned int nDeviceID;
};

struct DeleteDevice_Download_Type
{
	unsigned int nResult;					//0 成功 1 权限组下有人员 2 未知错误
};
typedef Pack_Shell_Type<DeleteDevice_Upload_Type>   DeleteDevice_UpLoad_Pack;
typedef Pack_Shell_Type<DeleteDevice_Download_Type> DeleteDevice_DownLoad_Pack;


// 设备列表获取 msgnum=0x1a
struct AskDeviceList_Upload_Type
{
	unsigned int nOrgID;
	unsigned int nType;						// 0获取机构和下属机构全部设备，1获取机构下属
	unsigned int nDeivceType;				// 0图片设备，1视频设备，2全部
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


// GPS点获取 msgnum=0x1b
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


// 获取铁路表名 msgnum=0x1c
struct GetRailWayTableName_Upload_Type
{
	unsigned int nLineID;				// 0 表示全部
};

struct GetRailWayTableName_Download_Type
{
	unsigned int nTotlePack;
	unsigned int nHadSendPack;
	string		 strnName;
};
typedef Pack_Shell_Type<GetRailWayTableName_Upload_Type>   GetRailWayName_Upload_Pack;
typedef Pack_Shell_Type<GetRailWayTableName_Download_Type> GetRailWayName_Download_Pack;


// 线路设置 msgnum=0x1d
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


// 巡检时间设置 msgnum=0x1e
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


// 排班设置 msgnum=0x1f
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


// 排班设置 msgnum=0x20
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



// 排班人员排序 msgnum=0x21
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

// 紧急任务发布 msgnum=0x22
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

// 紧急任务人员 msgnum=0x23
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

// 获取线路点 msgnum=0x25
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

// 获取图片 msgnum=0x26
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

// 获取紧急任务列表 msgnum = 0x27
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

// 获取紧急任务人员列表 msgnum=0x28
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

// 图像获取 msgnum=0x29
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

// 机构排版列表 msgnum=0x2a
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

// 机构排版信息 msgnum=0x2b
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

// 点设置msgnum=0x2c
struct SetPoint_Upload_Type
{
	unsigned  int nIType;//itype; // 0 添加 1 修改 2 删除
	unsigned  int nPointId;//修改、删除时有效
	unsigned  int nIRailLine;//线路id
	double		  dIKM;//公里处
	double		  dILon;//经度
	double		  dILat;//纬度
	int			  nIDirect;//上下行标志
};

struct SetPoint_Download_Type
{
	unsigned  int nResult;//修改结果 0成功 1失败
};
typedef Pack_Shell_Type<SetPoint_Upload_Type> SetPoint_Upload_Pack;
typedef Pack_Shell_Type<SetPoint_Download_Type> SetPoint_Download_Pack;

// 获取机构线路msgnum=0x2d
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

// 获取线路的点时间msgnum=0x2e
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

// 获取机构下已排版人员msgnum=0x2f
struct GetOrgSchWorker_Upload_Type
{
	unsigned  int nOrgID;
};
struct GetOrgSchWorker_Download_Type
{
	unsigned  int nTotle;//一共多少包
	unsigned  int nHasRecv;//已经发送报数
	string		  strOper;
	string		  strName;
	string		  strTel;
	unsigned int  nOrgID;
	unsigned int  nPowerGroup;
	unsigned int  nPda1;
	unsigned int  nPda2;
	unsigned int  nWorkerState;// 是否允许登录（1、允许 2 不允许）
};
typedef Pack_Shell_Type<GetOrgSchWorker_Upload_Type> GetOrgSchWorker_Upload_Pack;
typedef Pack_Shell_Type<GetOrgSchWorker_Download_Type> GetOrgSchWorker_Download_Pack;
//////////////////////////////////////////////////////////////////////////
///
/// 数据包类型宏定义
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