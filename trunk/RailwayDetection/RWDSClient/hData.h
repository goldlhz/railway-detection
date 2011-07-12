#ifndef _hdata_ 
#define _hdata_
#include <list>
using namespace std;
#pragma  pack(1)
template<typename Data_Body_Type>
struct Pack_Shell_Type
{		// 数据长度		后续数据区长度,不含结尾标识
	Data_Body_Type gDataBodyPack;
};// 数据区		依赖消息的不同数据内容


typedef struct 
{
	unsigned char  nMsgNumber;	
	unsigned int   nBodyLength;
}BaseStruct;

typedef struct
{
	unsigned  int result;//0 成功 1 失败
}BaseResult;

//登录请求
typedef struct
{
	char Name[30];
	char Pwd[12];
}LoginRerquest;

typedef Pack_Shell_Type<LoginRerquest>   tUserLogin;
//登录返回
typedef struct
{
	unsigned  int iResult; // 0表成，1失败
	unsigned  int Power1;
	unsigned  int Power2;
	unsigned  int Power3;
	unsigned  int Orgid;
}LoginRerquestResult;
typedef Pack_Shell_Type<LoginRerquestResult>   tLoginRerquestResultn;
//机构添加
typedef struct
{
	char OrgName[30];
	char Address[20];
	unsigned   int  UpOrg;
	unsigned  int iLevel;
	unsigned  int LineId;
	unsigned  int startid;
	unsigned  int EndID;
}AddOrg;
//机构添加返回
typedef struct
{
	unsigned  int iResult;//添加结果 0成功  1失败
}AddOrgResult;

//机构修改
typedef struct
{
	unsigned  int OrgId;
	char OrgName[30];
	char Address[20];
	unsigned  int  UpOrg;
	unsigned  int iLevel;
	unsigned  int LineId;
	unsigned  int startid;
	unsigned  int EndID;
}EditOrg;

typedef struct
{
	unsigned  int iResult;//添加结果 0成功  1 失败
}EditOrgResult;

//机构删除
typedef struct
{
	unsigned  int OrgId;
}DelOrg;

typedef struct
{
	unsigned  int iResult;//添加结果 0成功  1 失败
	unsigned  int reson;  //失败返回 1 包含下属机构 2 机构下属有人 3 未知错误
}DelOrgResult;

//机构列表获取
typedef struct
{
	unsigned  int OrgId; // 0 代表全部机构信息
}RequestOrg;

typedef struct
{
	//unsigned  int iMsg;//消息号
	unsigned  int iTotle;//一共多少包
	unsigned  int iHasRecv;//已经发送报数
	unsigned  int OrgId;
	char OrgName[30];
	char Address[20];
	unsigned  int  UpOrg;
	unsigned  int iLevel;
	unsigned  int LineId;
	unsigned  int startid;
	unsigned  int EndID;
}RequestOrgResult;
typedef list <RequestOrgResult> lOrg;
typedef lOrg::iterator iterOrg;
//人员添加
typedef struct
{
	char Oper[20];
	char Pwd[32];
	char name[20];
	char Tel[20];
	unsigned int orgid;
	unsigned int PowerGroup;
	unsigned int pda1;
	unsigned int pda2;
	unsigned int UserState;// 是否允许登录（1、允许 2 不允许）
}AddUser;

typedef struct
{
	unsigned  int result;//0 成功 1 用户名已经存在 2 未知错误
}AddUserResult;
//人员修改
typedef struct
{
	char Oper[20];
	char name[20];
	char Tel[20];
	unsigned int orgid;
	unsigned int PowerGroup;
	unsigned int pda1;
	unsigned int pda2;
	unsigned int UserState;// 是否允许登录（1、允许 2 不允许）
}EditUser;

typedef struct
{
	unsigned  int result;//0 成功 1 失败
}EditUserResult;

//人员删除
typedef struct
{
	char Oper[20];
}DelUser;

typedef struct
{
	unsigned  int result;//0 成功 1 失败
}DelUserResult;

//人员列表
typedef struct
{
	unsigned   int Orgid;
	unsigned   int Type;//1 机构下属人员列表 2 机构和机构下属全部人员列表
}RequestUserList;

typedef struct
{
	unsigned  int iTotle;//一共多少包
	unsigned  int iHasRecv;//已经发送报数
	char Oper[20];
	char name[20];
	char Tel[20];
	unsigned int orgid;
	unsigned int PowerGroup;
	unsigned int pda1;
	unsigned int pda2;
	unsigned int UserState;// 是否允许登录（1、允许 2 不允许）
}RequestUserListResult;
typedef list <RequestUserListResult> lUser;
typedef lUser::iterator iterUser;
//权限组添加
typedef struct
{
	char PowerName[20];
	unsigned  int iPow1;
	unsigned  int iPow2;
	unsigned  int iPow3;
}AddPower;

typedef struct
{
	unsigned  int result;//0 成功 1 失败
}AddPowerResult;

//权限组修改
typedef struct
{
	unsigned  int PowerId;
	char PowerName[20];
	unsigned  int iPow1;
	unsigned  int iPow2;
	unsigned  int iPow3;
}EditPower;

typedef struct
{
	unsigned  int result;//0 成功 1 失败
}EditPowerResult;

//权限组删除
typedef struct
{
	unsigned  int PowerId;
}DelGPower;

typedef struct
{
	unsigned  int result;//0 成功 1 权限组下有人员 2 未知错误
}DelGPowerResult;

///权限组获取
typedef struct
{
unsigned  int PowerId;// 0 为获取全部权限组 其他为获取某个权限组 
}RequestPowerList;

typedef struct
{
	unsigned  int iTotle;//一共多少包
	unsigned  int iHasRecv;//已经发送报数
	unsigned  int PowerId;
	char PowerName[20];
	unsigned  int iPow1;
	unsigned  int iPow2;
	unsigned  int iPow3;
}RequestPowerListResult;
typedef list <RequestPowerListResult> lPower;
typedef lPower::iterator iterPower;

//设备添加
typedef struct
{
	char DevicePid[20];
	unsigned  int DeviceOrg;
	unsigned  int DeviceType;
}AddDevice;

typedef struct
{
	unsigned  int result;//0 成功  1 失败 
}AddDeviceResult;

//设备修改
typedef struct
{
	unsigned  int DeviceId;
	char DevicePid[20];
	unsigned  int DeviceOrg;
	unsigned  int DeviceType;
}EditDevice;

typedef struct
{
	unsigned  int result;//0 成功 1 失败
}EditDeviceResult;
//删除设备
typedef struct
{
	unsigned  int DeviceId;
}DelDevice;

typedef struct
{
	unsigned  int result;//0 成功 1 有人正在使用 2 未知错误
}DelDeviceResult;

//获取设备列表
typedef struct
{
	unsigned  int Orgid;//
	unsigned  int type;//0获取机构和下属机构全部设备 1 获取机构下属 
	unsigned  int Devicetype;//0 图片设备 1 视频设备 2 全部 
}RequestDeviceList;

typedef struct
{
	unsigned  int iTotle;//一共多少包
	unsigned  int iHasRecv;//已经发送报数
	unsigned  int DeviceId;
	char DevicePid[20];
	unsigned  int DeviceOrg;
	unsigned  int DeviceType;
}RequestDeviceListResult;
typedef list <RequestDeviceListResult> lDevice;
typedef lDevice::iterator iterDevice;

 ////获取图片列表
typedef struct
{
	unsigned  int Orgid;
	char   stime[20];
	char   etime[20];
}GetPic;

typedef struct
{
	unsigned  int totlePacket;
	unsigned  int CurrentPacket;
	char  name[30];
	char time[20];
	unsigned int itype;
	char tel[20];
}picResult;
typedef list <picResult> lPicList;
typedef lPicList::iterator iterPic;
//图像获取
typedef struct
{
	char pname[30];
}Getrealpic;

typedef struct
{
	unsigned  int totlePacket;
	unsigned  int CurrentPacket;
	unsigned  int Pagesize;
	char param [1024];
}realpiclist;
//获取某天巡检
typedef struct
{
	char oper[20];
	char time[20];
}UserGps;
typedef struct
{
	unsigned  int totlePacket;
	unsigned  int CurrentPacket;
	float jdu;
	float wdu;
	char time[20];
}OrgLineResults;
typedef list <OrgLineResults> lOrgLineResult;
typedef lOrgLineResult::iterator IterOrgLineResult;

/////////////////////////////
//人员巡检
typedef struct
{
	char oper[20];
	char date[20];
}ryxj1;

typedef struct
{
unsigned  int totlePacket;
unsigned  int CurrentPacket;
unsigned  int Pointid;
char date[20];
char sArrtime[20];
char realTime[20];
char pid[20];
unsigned  int pointstate;
}ryxj1result;
typedef list<ryxj1result> lryxj1result;
typedef lryxj1result::iterator Iterryxj1result;

////紧急任务人员删除
typedef struct
{
	unsigned  int iTaskId;
}jjDel;

typedef struct
{
	unsigned  int iresult;
}jDleRersult;
//紧急任务列表
typedef struct
{
unsigned  int Orgid;
}Getjj;

typedef struct
{
	unsigned  int totlePacket;
	unsigned  int CurrentPacket;
	char stime[30];
	char etime[20];
	char  rwName[40];
	unsigned  int state;
	unsigned  int id;////////
	unsigned  int lineid;
	unsigned  int spoint;
	unsigned  int epoint;
}jjListresult;
typedef list<jjListresult> ljjListresult;
typedef ljjListresult::iterator IterljjListresult;

////
typedef struct
{
unsigned  int jjid;
}Getjjry;


	//unsigned  int nTtlePacket;
	//unsigned  int nCurrentPacket;
	//string		  strJj_Pid;
	//string		  strJj_Name;
	//unsigned  int nJj_OrgID;
	//string		  strJj_Time;
	//unsigned  int nJj_long

typedef struct
{
unsigned  int totlePacket;
unsigned  int CurrentPacket;
char Jj_Pid[20];
char		  strJj_Name[40];
unsigned  int nJj_OrgID;
char Jj_time[25];
unsigned  int Jj_long;
}jjryListresult;
typedef list<jjryListresult> ljjryListresult;
typedef ljjryListresult::iterator IterlljjryListresult;
//////////////////////
//////////////////////////////////////////////////////////////////////////
///
/// 数据包类型宏定义
//////////////////////////////////////////////////////////////////////////
#define PACK_IDENTIFY					0XFFFFFFFF

#define GPS_PACK						0X02
#define dUser_Login						0X0A
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

#define GETORGPOINT_PACK				0X25
#define GETPICLIST_PACK				    0X26
#define GETORGPXINFO_PACK				0X2B
#define GETORGMONTHPX_PACK				0X2a
#define GETpic_PACK				        0X29
#define SetPoint_PACK				    0X2C
#define GETORGLINE                      0x2d
#define GETLINETIME                     0x2e
#define GETORGPBSTAFF                   0x2f
#define GETGPSDATE_PACKET               0x30
#define DELALLJJRY_PACKET               0x31
#define XJMX_PACKET                     0x32

#define GETJJRWLIST_PACKET              0x27
#define GETJJRWRYLIST_PACKET            0x28
#define DELETERY_PACKET                 0x31

#define NoSocket                        0x00
#define NoData                          0x01
#define ErrSocket                       -1
#define ErrListCount                    -2
#define WaitListDataTime                0x02
#define ResultIsOk                      0x01
#endif