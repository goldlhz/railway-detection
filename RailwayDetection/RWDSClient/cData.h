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
	//��ȡѲ���
	int GetPointList(Sgps const sValue,lGps *llist);
	//��ȡ��·����
	int GetLineName(GetLinesName const sValue,lLineName *llist);
	//Ѳ��ʱ�����
	int SetxjTime(xj const sValue);
	//Ѳ��ʱ�����
	int SetMangLine(MangLine const sValue);
	//�Ű����
	int	SetpxPb(pb const sValue);
	//�Ű����
	int	Setpx(PbLine const sValue);
	//�Ű���Ա����lele
	int SetpxOrder(pbpp const sValue);
	//��������
	int Setjjrw(jjrw const sValue);
	//������Ա
	int Setjjry(jjry const sValue);
	//��ȡ������Ч�б�
	int GetOrgPoint(int const sValue,lOrgLine *llist);
	//��ȡ�����İ���Ϣ
	int GetOrgpInfo(OrgPxInfo const sValue,GetOrgPxInfo *llist);
	//��ȡͼƬ�б�
	int GetOrgPic(GetPic const sValue,lPicList *llist);
	//��ȡ�����¶ȱ���
	int GetOrgMonthPx(getorgpx const sValue,lOrgMonth *llist);
	//��ȡͼƬ
	int getPic(const Getrealpic sValue);
	//������
	int setPoint(const PointMang sValue);
	//��ȡ����������
	int GetOrgLine(const Orglines sValue,lallOrgLine *llist);
	//��ȡ��·��ʱ��
	int rGetLineTime(const rLinePointTime sValue,lrLinePointTimeResult *llist);
	//��ȡ�Ű���Ա
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

