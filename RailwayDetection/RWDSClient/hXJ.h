#include "hData.h"

#ifndef _jXJ_
#define _jXJ_
#pragma  pack(1)
//gps点获取
typedef struct
{
	char  cTel[20];
	char  sTime[20];
	char sEndTime[20];
	unsigned   int type; // 0 lineid 1 pointid
}GetGps;

typedef struct
{
	char time[30];
	char jdu[20];
	char wdu[20];
	char tel[20];
}Sgps;

typedef struct
{
	unsigned  int iTotle;
	unsigned  int HasSendPacket;
	Sgps cGps[10]; //
}GetGpsResult;
typedef list <GetGpsResult> lGps;
typedef lGps::iterator iterGPS;

//获取铁路表名
typedef struct
{
unsigned   int LineId; //0 获取全部
}GetLinesName;

typedef struct
{
	unsigned  int iTotle;
	unsigned  int HasSendPacket;
	char  cName[500]; //x 名称和id号之间加“||”，名称之间采用“|||”
}GetLineNameResult;
typedef list <GetLineNameResult> lLineName;
typedef lLineName::iterator iterLineName;

//线路设置
typedef struct
{
	unsigned  int type;//0 add 1 edit 2 del
	unsigned  int id;
	char  cName[30];
	unsigned  int day;
	unsigned  int  orgid;
}MangLine;

typedef struct
{
	unsigned  int iResult;// 0 ok 1 err
}MangLineResult;

//巡检时间设计
typedef struct
{
	unsigned  int type;//0 add 1 edit 2 del
	unsigned  int id;
	unsigned  int lineid;
	unsigned  int pointed;
	char     time[20];
}xj;

typedef struct
{
	unsigned  int iResult;// 0 ok 1 err
}xjResult;

//排班设计
typedef struct
{
	unsigned  int type;//0 add 1 edit 2 del
	unsigned  int id;
	unsigned  int cyc;
	unsigned  int orgid;
	char     start[20];
	char     content[60];
}pb;

typedef struct
{
	unsigned  int iResult;// 0 ok 1 err
}pbResult;

//排班设计线路
typedef struct
{
	unsigned  int type;//0 add 1 edit 2 del
	unsigned  int PL_Id;
	unsigned  int PL_PBID;
	unsigned  int PL_Lineid;
} PbLine;

typedef struct
{
	unsigned  int iResult;// 0 ok 1 err
}PbLineResult;

//排班人员排序
typedef struct
{
	unsigned  int type;//0 add 1 edit 2 del
	unsigned  int ppid;
	char ryid[20];
	unsigned  int px;
}pbpp;

typedef struct
{
	unsigned  int iResult;// 0 ok 1 err
}pbppResult;
//紧急任务发布
typedef struct
{
	unsigned  int type;//0 add 1 edit 
	unsigned  int startPointId;
	unsigned  int endPointId;
	char  startTime[25];
	char  endTime[25];
	unsigned  int state;
	unsigned int lineid;
	unsigned  int orgid;
}jjrw;

typedef struct
{
	unsigned  int iResult;// 0 ok 1 err
}jjrwResult;


//紧急任务人员
typedef struct
{
	unsigned  int  type;//0 add 1 del 
	unsigned  int  Jj_id;
	char  Jj_Pid[20];
	unsigned  int  Jj_pDevice;
}jjry;

typedef struct
{
	unsigned  int iResult;// 0 ok 1 err
}jjryResult;

///机构有效路线范围
typedef struct
{
	unsigned  int Orgid;
}OrgLine;

typedef struct
{
	unsigned  int iTotle;//一共多少包
	unsigned  int iHasRecv;//已经发送报数
	float jdu;
	float wdu;
	float gls; //公里数
	int  iState;
	int lineid;
}OrgLineResult;
typedef list <OrgLineResult> lOrgLine;
typedef lOrgLine::iterator iterOrgLine;

///获取机构排班信息
typedef struct
{
	unsigned  int Orgid;
}OrgPxInfo;

typedef struct
{
	unsigned  int id;
	unsigned  int cyc;
	char stime[20];
	char content[50];
}GetOrgPxInfo;
///机构月报表
typedef struct
{
	unsigned  int orgid;
	unsigned  int yesrs;
	unsigned  int months;
}getorgpx;

typedef struct
{
	unsigned  int totlePacket;
	unsigned  int CurrentPacket;
	char userid[20];
	char dates[20];
	char xj[20];
	unsigned  int sTotlePoint;
	unsigned  int sGetPoint;
	unsigned  int cPoint;
	unsigned  int lPoint;
}getorgpxlistresult;
typedef list <getorgpxlistresult> lOrgMonth;
typedef lOrgMonth::iterator iterOrgMonth;
//图片信息获取
typedef struct
{
	char pname[30];
}Getrealpic;

typedef struct
{

	unsigned  int totlePacket;
	unsigned  int CurrentPacket;
	unsigned  int Pagesize;
	char param [1024*3];
}realpiclist;
///点设置
typedef struct
{
	unsigned  int itype; // 0 添加 1 修改 2 删除
	unsigned  int PointId;//修改、删除时有效
	unsigned  int iRailLine;//线路id
	double iKM;//公里处
	double iLon;//经度
	double iLat;//纬度
	int iDirect;//上下行标志
}PointMang;

typedef struct
{
	unsigned  int iResult;//修改结果 0成功 1失败
}pointResult;
//获取机构线路
typedef struct
{
	unsigned  int orgid;
}Orglines;

typedef struct
{
	unsigned  int totlePacket;
	unsigned  int CurrentPacket;
	unsigned  int lineid;
	unsigned  int lindday;
	char line[30];
}allOrgLineResult;
typedef list <allOrgLineResult> lallOrgLine;
typedef lallOrgLine::iterator riterOrgLine;

///获取机构线路下的点时间
typedef struct
{
	unsigned  int lineid;
}rLinePointTime;

typedef struct
{
	unsigned  int totlePacket;
	unsigned  int CurrentPacket;
	float iKM;//
	float iLon;
	float iLat;
	int iDirect;//
	int   PointId;
	char time[20];
}rLinePointTimeResult;
typedef list <rLinePointTimeResult> lrLinePointTimeResult;
typedef lrLinePointTimeResult::iterator iterrLinePointTime;
//请求机构下属排版人员列表
typedef struct
{
	unsigned  int orgid;
}rOrgPB;

#endif