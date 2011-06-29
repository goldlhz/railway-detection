#pragma once
#include "hData.h"
#include "hXj.h"
class CTCPClient;
class cData
{
public:
	cData(void);
	virtual ~cData(void);
	int UserLog(char * UserName,char *UserPwd,int *Orgid,int *p1,int *p2,int *p3);
	int AddOrgs(AddOrg *aOrg);
	int EditOrgs(EditOrg *aOrg);
	int DelOrgs(DelOrg *aOrg);
	int AddUsers(AddUser *aUser);
	int EditUsers(EditUser *aUser);
	int DelUsers(DelUser *aUser);
	int AddPowers(AddPower *aPower);
	int EditPowers(EditPower *aPower);
	int DelPowers(DelGPower *aPower);
	int AddDevices(AddDevice *aPower);
	int EditDevices(EditDevice *aPower);
	int DelDevices(DelDevice *aPower);
	int GetOrgList(int Orgid,lOrg *plOrg);
	int GetUserList(int Orgid, int iType,lUser *lPoint);
	int GetDeviceList(RequestDeviceList rs,lDevice *lPoint);
	int GetPowerList(RequestPowerList rs,lPower *lPoint);
	//获取巡检点
	int GetPointList(Sgps const sValue,lGps *llist);
	//获取铁路名称
	int GetLineName(GetLinesName const sValue,lLineName *llist);
	//巡检时间设计
	int SetxjTime(xj const sValue);
	//巡检时间设计
	int SetMangLine(MangLine const sValue);
	//排班设计
	int	SetpxPb(pb const sValue);
	//排班设计
	int	Setpx(PbLine const sValue);
	//排班人员排序lele
	int SetpxOrder(pbpp const sValue);
	//紧急任务
	int Setjjrw(jjrw const sValue);
	//紧急人员
	int Setjjry(jjry const sValue);
	//获取机构有效列表
	int GetOrgPoint(int const sValue,lOrgLine *llist);
	//获取机构拍板信息
	int GetOrgpInfo(OrgPxInfo const sValue,GetOrgPxInfo *llist);
	//获取图片列表
	int GetOrgPic(GetPic const sValue,lPicList *llist);
	//获取机构月度报表
	int GetOrgMonthPx(getorgpx const sValue,lOrgMonth *llist);
	//获取图片
	int getPic(const Getrealpic sValue);
	//点设置
	int setPoint(const PointMang sValue);
	//获取机构下属线
	int GetOrgLine(const Orglines sValue,lallOrgLine *llist);
	//获取线路点时间
	int rGetLineTime(const rLinePointTime sValue,lrLinePointTimeResult *llist);
	//获取排版人员
	int rGetOPb(const rOrgPB sValue,lUser *lPoint);
private:
	CTCPClient *cs;
	void CreateConn();
	void CloseConn();
	void WriteBug(const char *buf,const int iLength) const;
	void readBug(char  *buf, int* iLength) ;
	int iSocketState;

	bool BuildDataPackShell(char* pDataBuffer, BaseStruct* gpsDownLoadPack);
	template<typename T>
	bool DepressPacket(T& dataDownLoadPack, unsigned char cMsgNumber,const char* buf);

	bool BuildLoginPackData(char* pDataBuffer, const tUserLogin& loginDownLoadPack);
	
};

