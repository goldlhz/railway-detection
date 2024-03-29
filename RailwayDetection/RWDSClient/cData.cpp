#include "StdAfx.h"
#include "TCPTransmit.h"
#include "cData.h"
#include "hXJ.h"
#include <shlwapi.h>



const int port = 9527 ;
const char* destIp = "182.143.211.1\0";
cData::cData(void)
{
	cs = new CTCPClient();
	iSocketState = 0;
	//CString str;
	char ip[30];
	memset(ip,0,30);

    CString modDir;
    GetModuleFileName(NULL, modDir.GetBuffer(MAX_PATH), MAX_PATH); 
    modDir.ReleaseBuffer();
    int pos;   
    pos = modDir.ReverseFind('\\');   
    modDir = modDir.Left(pos);
    CString filename = modDir + _T("\\sys.ini");
	::GetPrivateProfileString("ip","host","127.0.0.1",ip,30,filename); 
	iSocketState = cs->Open(port,ip);
}
//select datename(dw,'2011-1-4 1:2:3') 取星期几
int cData::UserLog(char * UserName,char *UserPwd,int *Orgid,int *p1,int *p2,int *p3)
{
	int itsr = sizeof(AddOrg);
	if(NoSocket > iSocketState)
	{
		return -1;
	}
	
	char cTempBuf[1024];
	memset(cTempBuf,0,1024);

	char *pDataBuffer = new char[11+sizeof(LoginRerquest)];
	memset(pDataBuffer,0,11+sizeof(LoginRerquest));
	tUserLogin tUl ;
	memset(&tUl,0,sizeof(tUl));
	int its = strlen(UserPwd);

	LoginRerquest lqust;
	memset(&lqust,0,sizeof(LoginRerquest));
	memcpy(lqust.Pwd,UserPwd, strlen(UserPwd));//sizeof(UserPwd));
	memcpy(lqust.Name,UserName,strlen(UserName));
	BaseStruct bs;
	memset(&bs,0,sizeof(bs));
	bs.nBodyLength =  sizeof(LoginRerquest);
	bs.nMsgNumber = dUser_Login;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&lqust,sizeof(lqust));
	memcpy(cTempBuf, (char*)pDataBuffer, 53);

	int iResult = cs->Write(pDataBuffer,11 + sizeof(LoginRerquest));
	if(iResult < iSocketState)
	{
		return -1;
	}
	memset(cTempBuf,0,sizeof(cTempBuf));
	int it = 11 + sizeof(LoginRerquestResult);
	Sleep(5);
	LoginRerquestResult lr;
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);
	
	if(iResult < 0)
	{
		return -1;
	}

	if(DepressPacket(lr,dUser_Login,cTempBuf))
	{
	*Orgid = lr.Orgid ;
	*p1   = lr.Power1;
	*p2 = lr.Power2;
	*p3 = lr.Power3;
	}
	else 
		lr.iResult = 1;
	delete pDataBuffer;
	return lr.iResult;
}

cData::~cData(void)
{
}
//////////////////////////////////////////////////////////////////////////
int cData::AddOrgs(AddOrg *aOrg)
{
	char pDataBuffer[11 + sizeof(AddOrg)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(AddOrg);
	bs.nMsgNumber = ADDORG_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,aOrg,sizeof(AddOrg));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(AddOrg));
	if(iResult < iSocketState)
	{
		return -1;
	}

	AddOrgResult lr;
	int it = 11 + sizeof(AddOrgResult);
	char cTempBuf[11 + sizeof(AddOrgResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}

	if(DepressPacket(lr,ADDORG_PACK,cTempBuf))
	{
		lr.iResult = 0;
	}else
	{
		lr.iResult = 1;
	}

	return lr.iResult;
}

//////////////////////////////////////////////////////////////////////////
int cData::EditOrgs(EditOrg *aOrg)
{
	char pDataBuffer[11 + sizeof(EditOrg)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(EditOrg);
	bs.nMsgNumber = MODIFYORG_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,aOrg,sizeof(EditOrg));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(EditOrg));
	if(iResult < iSocketState)
	{
		return -1;
	}

	EditOrgResult lr;
	int it = 11 + sizeof(EditOrgResult);
	char cTempBuf[11 + sizeof(EditOrgResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	//DepressPacket(lr,MODIFYORG_PACK,cTempBuf);
	if(DepressPacket(lr,MODIFYORG_PACK,cTempBuf))
	{
		lr.iResult = 0;
	}else
	{
		iResult = 1;
	}
	return lr.iResult;
}

int cData::DelOrgs(DelOrg *aOrg)
{
	char pDataBuffer[11 + sizeof(DelOrg)];

	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(DelOrg);
	bs.nMsgNumber = DELETEORG_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,aOrg,sizeof(DelOrg));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(DelOrg));
	if(iResult < iSocketState)
	{
		return -1;
	}

	DelOrgResult lr;
	int it = 11 + sizeof(DelOrgResult);
	char cTempBuf[11 + sizeof(DelOrgResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	//DepressPacket(lr,DELETEORG_PACK,cTempBuf);
		if(DepressPacket(lr,DELETEORG_PACK,cTempBuf))
	{
		lr.iResult = 0;
	}else
	{
		iResult = 1;
	}
	return lr.iResult;
}

int cData::AddUsers(AddUser *aUser)
{
	char pDataBuffer[11 + sizeof(AddUser)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(AddUser);
	bs.nMsgNumber = ADDWORKER_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,aUser,sizeof(AddUser));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(AddUser));
	if(iResult < iSocketState)
	{
		return -1;
	}

	AddUserResult lr;
	int it = 11 + sizeof(AddUserResult);
	char cTempBuf[11 + sizeof(AddUserResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	//DepressPacket(lr,ADDWORKER_PACK,cTempBuf);
	if(DepressPacket(lr,ADDWORKER_PACK,cTempBuf))
	{
		lr.result = 0;
	}else
	{
		lr.result = 1;
	}
	return lr.result;
}

int cData::EditUsers(EditUser *aUser)
{
	char pDataBuffer[11 + sizeof(EditUser)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(EditUser);
	bs.nMsgNumber = MODIFYWORKER_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,aUser,sizeof(EditUser));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(EditUser));
	if(iResult < iSocketState)
	{
		return -1;
	}

	AddUserResult lr;
	int it = 11 + sizeof(AddUserResult);
	char cTempBuf[11 + sizeof(AddUserResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
//	DepressPacket(lr,MODIFYWORKER_PACK,cTempBuf);

			if(DepressPacket(lr,MODIFYWORKER_PACK,cTempBuf))
	{
		lr.result = 0;
	}else
	{
		lr.result = 1;
	}
	return lr.result;
}

int cData::DelUsers(DelUser *aUser)
{
	char pDataBuffer[11 + sizeof(DelUser)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(DelUser);
	bs.nMsgNumber = DELETEWORKER_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,aUser,sizeof(DelUser));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(DelUser));
	if(iResult < iSocketState)
	{
		return -1;
	}

	AddUserResult lr;
	int it = 11 + sizeof(AddUserResult);
	char cTempBuf[11 + sizeof(AddUserResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	//DepressPacket(lr,DELETEWORKER_PACK,cTempBuf);
				if(DepressPacket(lr,DELETEWORKER_PACK,cTempBuf))
	{
		lr.result = 0;
	}else
	{
		lr.result = 1;
	}
	return lr.result;
}

int cData::AddPowers(AddPower *aPower)
{
	char pDataBuffer[11 + sizeof(AddPower)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(AddPower);
	bs.nMsgNumber = ADDPOWERGROUP_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,aPower,sizeof(AddPower));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(AddPower));
	if(iResult < iSocketState)
	{
		return -1;
	}

	AddUserResult lr;
	int it = 11 + sizeof(AddUserResult);
	char cTempBuf[11 + sizeof(AddUserResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	//DepressPacket(lr,ADDPOWERGROUP_PACK	,cTempBuf);
	if(DepressPacket(lr,ADDPOWERGROUP_PACK,cTempBuf))
	{
		lr.result = 0;
	}else
	{
		lr.result = 1;
	}

	return lr.result;
}
int cData::EditPowers(EditPower *aPower)
{
	char pDataBuffer[11 + sizeof(EditPower)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(EditPower);
	bs.nMsgNumber = MODIFYPOWERGROUP_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,aPower,sizeof(EditPower));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(EditPower));
	if(iResult < iSocketState)
	{
		return -1;
	}

	AddUserResult lr;
	int it = 11 + sizeof(AddUserResult);
	char cTempBuf[11 + sizeof(AddUserResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	//DepressPacket(lr,MODIFYPOWERGROUP_PACK	,cTempBuf);
	if(DepressPacket(lr,MODIFYPOWERGROUP_PACK,cTempBuf))
	{
		lr.result = 0;
	}else
	{
		lr.result = 1;
	}
	return lr.result;
}
int cData::DelPowers(DelGPower *aPower)
{
	char pDataBuffer[11 + sizeof(DelGPower)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(DelGPower);
	bs.nMsgNumber = DELETEPOWERGROUP_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,aPower,sizeof(DelGPower));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(DelGPower));
	if(iResult < iSocketState)
	{
		return -1;
	}

	AddUserResult lr;
	int it = 11 + sizeof(AddUserResult);
	char cTempBuf[11 + sizeof(AddUserResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
//	DepressPacket(lr,DELETEPOWERGROUP_PACK	,cTempBuf);
	if(DepressPacket(lr,DELETEPOWERGROUP_PACK,cTempBuf))
	{
		lr.result = 0;
	}else
	{
		lr.result = 1;
	}
	return lr.result;
}
int cData::AddDevices(AddDevice *aPower)
{
	char pDataBuffer[11 + sizeof(AddDevice)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(AddDevice);
	bs.nMsgNumber = ADDDEVICE_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,aPower,sizeof(AddDevice));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(AddDevice));
	if(iResult < iSocketState)
	{
		return -1;
	}

	AddUserResult lr;
	int it = 11 + sizeof(AddUserResult);
	char cTempBuf[11 + sizeof(AddUserResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
//	DepressPacket(lr,ADDDEVICE_PACK	,cTempBuf);
	if(DepressPacket(lr,ADDDEVICE_PACK,cTempBuf))
	{
		lr.result = 0;
	}else
	{
		lr.result = 1;
	}
	return lr.result;
}
int cData::EditDevices(EditDevice *aPower)
{
	char pDataBuffer[11 + sizeof(EditDevice)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(EditDevice);
	bs.nMsgNumber = MODIFYDEVICE_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,aPower,sizeof(EditDevice));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(EditDevice));
	if(iResult < iSocketState)
	{
		return -1;
	}

	BaseResult lr;
	int it = 11 + sizeof(BaseResult);
	char cTempBuf[11 + sizeof(BaseResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	//DepressPacket(lr,MODIFYDEVICE_PACK	,cTempBuf);
	if(DepressPacket(lr,MODIFYDEVICE_PACK,cTempBuf))
	{
		lr.result = 0;
	}else
	{
		lr.result = 1;
	}
	return lr.result;
}

int cData::DelDevices(DelDevice *aPower)
{
	char pDataBuffer[11 + sizeof(DelDevice)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(DelDevice);
	bs.nMsgNumber = DELETEDEVICE_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,aPower,sizeof(DelDevice));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(DelDevice));
	if(iResult < iSocketState)
	{
		return -1;
	}

	BaseResult lr;
	int it = 11 + sizeof(BaseResult);
	char cTempBuf[11 + sizeof(BaseResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
//	DepressPacket(lr,DELETEDEVICE_PACK	,cTempBuf);
if(DepressPacket(lr,DELETEDEVICE_PACK,cTempBuf))
	{
		lr.result = 0;
	}else
	{
		lr.result = 1;
	}
	return lr.result;
}
int cData::GetOrgList(int Orgid,lOrg *plOrg)
{
	plOrg->clear();
	
	char pDataBuffer[11 + sizeof(int)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(int);
	bs.nMsgNumber = ASKORGLIST_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&Orgid,sizeof(int));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(int));
	if(iResult < iSocketState)
	{
		return -1;
	}
	int iTotleCount = 0;
	while(1)
	{
	 	RequestOrgResult lr;	
		int it = 11 + sizeof(RequestOrgResult);
		char cTempBuf[11 + sizeof(RequestOrgResult)];
		memset(cTempBuf,0,it);
		Sleep(5);
		iResult = cs->Read(cTempBuf,it);

	    if(iResult < 1)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(plOrg->size() == 35)
		{
			int i = 0;
			i++;
		}

		if(DepressPacket(lr,ASKORGLIST_PACK,cTempBuf))
		{
			plOrg->push_back(lr);
			iTotleCount = lr.iTotle ;
			if(lr.iTotle == lr.iHasRecv)
			{
				break;
			}
		}
	}
	if(plOrg->size() != iTotleCount)
	{
		return -2;
	}

	return 1;
}

int cData::GetUserList(int Orgid, int iType,lUser *lPoint)
{
	RequestUserList ru;
	ru.Orgid = Orgid;
	ru.Type = iType;
	char pDataBuffer[11 + sizeof(RequestUserList)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(RequestUserList);
	bs.nMsgNumber = ASKWORKERLIST_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&ru,sizeof(RequestUserList));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(RequestUserList));
	if(iResult < iSocketState)
	{
		return -1;
	}
	int iTotleCount = 0;
	while(1)
	{
		
		RequestUserListResult lr;
		int it = 11 + sizeof(RequestUserListResult);
		char cTempBuf[11 + sizeof(RequestUserListResult)];
		memset(cTempBuf,0,it);
		Sleep(5);
		iResult = cs->Read(cTempBuf,it);

		if(iResult < 1)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,ASKWORKERLIST_PACK,cTempBuf))
		{
			lPoint->push_back(lr);
			iTotleCount = lr.iTotle ;
			if(lr.iTotle == lr.iHasRecv)
				break;
		}
	}
	if(lPoint->size() != iTotleCount)
	{
		return -2;
	}
	return 1;
}

int cData::GetDeviceList(RequestDeviceList rs,lDevice *lPoint)
{

	char pDataBuffer[11 + sizeof(RequestDeviceList)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(RequestDeviceList);
	bs.nMsgNumber = ASKDEIVCELIST_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&rs,sizeof(RequestDeviceList));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(RequestDeviceList));
	if(iResult < iSocketState)
	{
		return -1;
	}
	RequestDeviceListResult lr;

	int iTotleCount = 0;
	lPoint->clear();
	while(1)
	{
		Sleep(5);

		int it = 11 + sizeof(RequestDeviceListResult);
		char cTempBuf[11 + sizeof(RequestDeviceListResult)];
		memset(cTempBuf,0,it);

		iResult = cs->Read(cTempBuf,it);

		if(iResult < 1)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,ASKDEIVCELIST_PACK,cTempBuf))
		{
			lPoint->push_back(lr);
			iTotleCount = lr.iTotle ;
			if(lr.iTotle == lr.iHasRecv)
				break;
		}
	}
	if(lPoint->size() != iTotleCount)
	{
		return -2;
	}
	return 1;
}
int cData::GetPowerList(RequestPowerList rs,lPower *lPoint)
{
	char pDataBuffer[11 + sizeof(RequestPowerList)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(RequestPowerList);
	bs.nMsgNumber = ASKPOWERGROUPLIST_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&rs,sizeof(RequestPowerList));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(RequestPowerList));
	if(iResult < iSocketState)
	{
		return ErrSocket;
	}
	RequestPowerListResult lr;
	int iTotleCount = 0;
	while(1)
	{
		Sleep(WaitListDataTime);

		int it = 11 + sizeof(RequestPowerListResult);
		char cTempBuf[11 + sizeof(RequestPowerListResult)];
		memset(cTempBuf,0,it);

		iResult = cs->Read(cTempBuf,it);

		if(iResult < NoData)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,ASKORGLIST_PACK,cTempBuf))
		{
			lPoint->push_back(lr);
			iTotleCount = lr.iTotle ;
			if(lr.iTotle == lr.iHasRecv)
				break;
		}
	}
	if(lPoint->size() != iTotleCount)
	{
		return ErrListCount;
	}
	return 1;
}

////////////////////////////////////////////////////
int cData::GetPointList(Sgps const sValue,lGps *llist)
{
	char pDataBuffer[11 + sizeof(Sgps)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(Sgps);
	bs.nMsgNumber = GETGPSPOINT_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(Sgps));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(Sgps));
	if(iResult < iSocketState)
	{
		return ErrSocket;
	}
	GetGpsResult lr;
	int iTotleCount = 0;
	while(1)
	{
		Sleep(WaitListDataTime);

		int it = 11 + sizeof(GetGpsResult);
		char cTempBuf[11 + sizeof(GetGpsResult)];
		memset(cTempBuf,0,it);

		iResult = cs->Read(cTempBuf,it);

		if(iResult < NoData)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,GETGPSPOINT_PACK,cTempBuf))
		{
			llist->push_back(lr);
			iTotleCount = lr.iTotle ;
			if(lr.iTotle == lr.HasSendPacket)
				break;
		}
	}
	if(llist->size() != iTotleCount)
	{
		return ErrListCount;
	}
	return ResultIsOk;
}
//获取铁路名称
int cData::GetLineName(GetLinesName const sValue,lLineName *llist)
{
	char pDataBuffer[11 + sizeof(GetLinesName)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(GetLinesName);
	bs.nMsgNumber = GETRAILWAYTABLENAME_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(GetLinesName));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(GetLinesName));
	if(iResult < iSocketState)
	{
		return ErrSocket;
	}
	GetLineNameResult lr;
	int iTotleCount = 0;
	while(1)
	{
		Sleep(WaitListDataTime);

		int it = 11 + sizeof(GetLineNameResult);
		char cTempBuf[11 + sizeof(GetLineNameResult)];
		memset(cTempBuf,0,it);

		iResult = cs->Read(cTempBuf,it);

		if(iResult < NoData)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,GETRAILWAYTABLENAME_PACK,cTempBuf))
		{
			llist->push_back(lr);
			iTotleCount = lr.iTotle ;
			if(lr.iTotle == lr.HasSendPacket)
				break;
		}
	}
	if(llist->size() != iTotleCount)
	{
		return ErrListCount;
	}
	return ResultIsOk;
}
//巡检时间设计
int cData::SetxjTime(xj const sValue)
{
	char pDataBuffer[11 + sizeof(xj)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(xj);
	bs.nMsgNumber = SETTINGPOLLINGTIME_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(xj));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(xj));
	if(iResult < iSocketState)
	{
		return -1;
	}

	BaseResult lr;
	int it = 11 + sizeof(BaseResult);
	char cTempBuf[11 + sizeof(BaseResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	if(DepressPacket(lr,SETTINGPOLLINGTIME_PACK	,cTempBuf))
		return lr.result;

	return 1;
}
//巡检时间设计
int cData::SetMangLine(MangLine const sValue)
{
	char pDataBuffer[11 + sizeof(MangLine)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(MangLine);
	bs.nMsgNumber = SETTINGLINE_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(MangLine));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(MangLine));
	if(iResult < iSocketState)
	{
		return -1;
	}
	Sleep(2);
	BaseResult lr;
	int it = 11 + sizeof(BaseResult);
	char cTempBuf[11 + sizeof(BaseResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	if(DepressPacket(lr,SETTINGLINE_PACK,cTempBuf))

	return lr.result;
	else
		return -1;
}
//排班设计le
int	cData::SetpxPb(pb const sValue)
{
	char pDataBuffer[11 + sizeof(pb)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(pb);
	bs.nMsgNumber = SETTINGSCHEDULE5_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(pb));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(pb));
	if(iResult < iSocketState)
	{
		return -1;
	}
	BaseResult lr;
	int it = 11 + sizeof(BaseResult);
	char cTempBuf[11 + sizeof(BaseResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

		if(iResult < 0)
		{
			return -1;
		}
		DepressPacket(lr,SETTINGSCHEDULE5_PACK,cTempBuf);

		return lr.result;
}
//排班设计lele
int	cData::Setpx(PbLine const sValue)
{
	char pDataBuffer[11 + sizeof(PbLine)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(PbLine);
	bs.nMsgNumber = SETTINGSCHEDULE6_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(PbLine));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(PbLine));
	if(iResult < iSocketState)
	{
		return -1;
	}

	BaseResult lr;
	int it = 11 + sizeof(BaseResult);
	char cTempBuf[11 + sizeof(BaseResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

		if(iResult < 0)
		{
			return -1;
		}
		DepressPacket(lr,SETTINGSCHEDULE6_PACK,cTempBuf);

		return lr.result;
}
////排班人员排序
int cData::SetpxOrder(pbpp const sValue)
{
	char pDataBuffer[11 + sizeof(pbpp)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(pbpp);
	bs.nMsgNumber = SETTINGSCHEDULEWORKER_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(pbpp));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(pbpp));
	if(iResult < iSocketState)
	{
		return -1;
	}

	BaseResult lr;
	int it = 11 + sizeof(BaseResult);
	char cTempBuf[11 + sizeof(BaseResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	DepressPacket(lr,SETTINGSCHEDULEWORKER_PACK,cTempBuf);

	return lr.result;
}

////紧急任务
int cData::Setjjrw(jjrw const sValue)
{
	char pDataBuffer[11 + sizeof(jjrw)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(jjrw);
	bs.nMsgNumber = URGENCYRELEASE_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(jjrw));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(jjrw));
	if(iResult < iSocketState)
	{
		return -1;
	}

	BaseResult lr;
	int it = 11 + sizeof(BaseResult);
	char cTempBuf[11 + sizeof(BaseResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	DepressPacket(lr,URGENCYRELEASE_PACK,cTempBuf);

	return lr.result;
}
////紧急人员
int cData::Setjjry(jjry const sValue)
{
	char pDataBuffer[11 + sizeof(jjry)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(jjry);
	bs.nMsgNumber = URGENCYWORKER_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(jjry));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(jjry));
	if(iResult < iSocketState)
	{
		return -1;
	}

	BaseResult lr;
	int it = 11 + sizeof(BaseResult);
	char cTempBuf[11 + sizeof(BaseResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	DepressPacket(lr,URGENCYWORKER_PACK,cTempBuf);

	return lr.result;
}
//获取机构有效点
int cData::GetOrgPoint(int const sValue,lOrgLine *llist)
{
	char pDataBuffer[11 + sizeof(int)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(int);
	bs.nMsgNumber = GETORGPOINT_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(int));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(int));
	if(iResult < iSocketState)
	{
		return ErrSocket;
	}
	OrgLineResult lr;
	int iTotleCount = 0;
	while(1)
	{
		Sleep(WaitListDataTime);

		int it = 11 + sizeof(OrgLineResult);
		char cTempBuf[11 + sizeof(OrgLineResult)];
		memset(cTempBuf,0,it);

		iResult = cs->Read(cTempBuf,it);

		if(iResult < NoData)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,GETORGPOINT_PACK,cTempBuf))
		{
			llist->push_back(lr);
			iTotleCount = lr.iTotle ;
			if(lr.iTotle == lr.iHasRecv)
				break;
		}
	}
	if(llist->size() != iTotleCount)
	{
		return ErrListCount;
	}
	return 1;
}

int cData::GetOrgpInfo(OrgPxInfo const sValue,GetOrgPxInfo *llist)
{
	char pDataBuffer[11 + sizeof(OrgPxInfo)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(OrgPxInfo);
	bs.nMsgNumber = GETORGPXINFO_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(OrgPxInfo));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(OrgPxInfo));
	if(iResult < iSocketState)
	{
		return -1;
	}
	Sleep(2);
	GetOrgPxInfo lr;
	int it = 11 + sizeof(GetOrgPxInfo);
	char cTempBuf[11 + sizeof(GetOrgPxInfo)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(GetOrgPxInfo));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	DepressPacket(lr,GETORGPXINFO_PACK,cTempBuf);
	memcpy(llist,&lr,sizeof(GetOrgPxInfo));
	return 1;
}

//////////////////////////////////////////////////////////////////////////
int cData::GetOrgPic(GetPic const sValue,lPicList *llist)
{
	char pDataBuffer[11 + sizeof(GetPic)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(GetPic);
	bs.nMsgNumber = GETPICLIST_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(GetPic));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(GetPic));
	if(iResult < iSocketState)
	{
		return ErrSocket;
	}
	picResult lr;
	int iTotleCount = 0;
	llist->clear();
	while(1)
	{
		
		Sleep(WaitListDataTime);
		int it = 11 + sizeof(picResult);
		char cTempBuf[11 + sizeof(picResult)];
		memset(cTempBuf,0,it);

		iResult = cs->Read(cTempBuf,it);

		if(iResult < NoData)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,GETPICLIST_PACK,cTempBuf))
		{
			llist->push_back(lr);
			iTotleCount = lr.totlePacket ;
			if(lr.CurrentPacket == lr.totlePacket)
			{
				break;
			}
		}
		else
		{
			return 0;
			break;
		}
	}
	if(llist->size() != iTotleCount)
	{
		return ErrListCount;
	}
	return 1;
}

//获取机构月度报表
int cData::GetOrgMonthPx(getorgpx const sValue,lOrgMonth *llist)
{
	char pDataBuffer[11 + sizeof(getorgpx)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(getorgpx);
	bs.nMsgNumber = GETORGMONTHPX_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(getorgpx));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(getorgpx));
	if(iResult < iSocketState)
	{
		return ErrSocket;
	}
	getorgpxlistresult lr;
	int iTotleCount = 0;
	llist->clear();
	while(1)
	{
		Sleep(WaitListDataTime);

		int it = 11 + sizeof(getorgpxlistresult);
		char cTempBuf[11 + sizeof(getorgpxlistresult)];
		memset(cTempBuf,0,it);

		iResult = cs->Read(cTempBuf,it);

		if(iResult < NoData)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,GETORGMONTHPX_PACK,cTempBuf))
		{
			llist->push_back(lr);
			iTotleCount = lr.totlePacket;
			if(lr.totlePacket == lr.CurrentPacket)
				break;
		}
	}
	if(llist->size() != iTotleCount)
	{
		return ErrListCount;
	}
	return 1;
}

int cData::getPic(const Getrealpic sValue, CString aToDirect)
{
	string picBuf;
	char pDataBuffer[11 + sizeof(Getrealpic)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(Getrealpic);
	bs.nMsgNumber = GETpic_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(Getrealpic));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(Getrealpic));
	if(iResult < iSocketState)
	{
		return ErrSocket;
	}
	realpiclist lr;
	int iTotleCount = 0;
    picBuf = "";
	char p[1024*100*3];
	memset(p,0,1024*100*3);
	int i =0;
	bool bResult = false;
	while(1)
	{
		//Sleep(5);
		string sTemp;
		int it = 11 + sizeof(realpiclist);
		char cTempBuf[11 + sizeof(realpiclist)];
		memset(cTempBuf,0,it);

		iResult = cs->Read(cTempBuf,it);

		if(iResult < NoData)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,GETpic_PACK,cTempBuf))
		{
			memcpy(p+(i *1024),lr.param,1024);
			i++;
			
			
			iTotleCount = lr.totlePacket;
			if(lr.CurrentPacket == lr.totlePacket)
			{

                if(!PathFileExists(aToDirect))
                {
                    CFile logFile;
                    logFile.Open(aToDirect, CFile::modeCreate | CFile::modeWrite);
                    logFile.Write(p,lr.Pagesize);
					bResult = true;
                }

				break;
			}
		}else
		{
			return bResult;
		}
	}
	//sTemp = p;
	return bResult;
}

int cData::setPoint(const PointMang sValue)
{
	char pDataBuffer[11 + sizeof(PointMang)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(PointMang);
	bs.nMsgNumber = SetPoint_PACK;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(PointMang));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(PointMang));
	if(iResult < iSocketState)
	{
		return -1;
	}

	pointResult lr;
	int it = 11 + sizeof(pointResult);
	char cTempBuf[11 + sizeof(pointResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	DepressPacket(lr,SetPoint_PACK,cTempBuf);

	return lr.iResult;
}

int cData::GetOrgLine(const Orglines sValue,lallOrgLine *llist)
{
	char pDataBuffer[11 + sizeof(Orglines)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(Orglines);
	bs.nMsgNumber = GETORGLINE;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(Orglines));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(Orglines));
	if(iResult < iSocketState)
	{
		return ErrSocket;
	}
	allOrgLineResult lr;
	int iTotleCount = 0;
	llist->clear();
	while(1)
	{
		Sleep(WaitListDataTime);

		int it = 11 + sizeof(allOrgLineResult);
		char cTempBuf[11 + sizeof(allOrgLineResult)];
		memset(cTempBuf,0,it);

		iResult = cs->Read(cTempBuf,it);

		if(iResult < NoData)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,GETORGLINE,cTempBuf))
		{
			llist->push_back(lr);
			iTotleCount = lr.totlePacket;
			if(lr.CurrentPacket == lr.totlePacket)
				break;
		}
		
	}

	if(lr.totlePacket >0 && lr.totlePacket == iTotleCount)
	return 1;
	else
		return 0;
}

//获取线路点时间
int cData::rGetLineTime(const rLinePointTime sValue,lrLinePointTimeResult *llist)
{
	char pDataBuffer[11 + sizeof(rLinePointTime)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(rLinePointTime);
	bs.nMsgNumber = GETLINETIME;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(rLinePointTime));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(rLinePointTime));
	if(iResult < iSocketState)
	{
		return ErrSocket;
	}
	rLinePointTimeResult lr;
	int iTotleCount = 0;
	llist->clear();
	while(1)
	{
		Sleep(WaitListDataTime);

		int it = 11 + sizeof(rLinePointTimeResult);
		char cTempBuf[11 + sizeof(rLinePointTimeResult)];
		memset(cTempBuf,0,it);

		iResult = cs->Read(cTempBuf,it);

		if(iResult < NoData)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,GETLINETIME,cTempBuf))
		{
			llist->push_back(lr);
			iTotleCount = lr.totlePacket;
			if(lr.totlePacket == lr.CurrentPacket)
				break;
		}
	}
	return 0;
}
int cData::rGetOPb(const rOrgPB sValue,lUser *lPoint)
{

	char pDataBuffer[11 + sizeof(rOrgPB)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(rOrgPB);
	bs.nMsgNumber = GETORGPBSTAFF;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(rOrgPB));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(rOrgPB));
	if(iResult < iSocketState)
	{
		return -1;
	}
	int iTotleCount = 0;
	lPoint->clear();
	while(1)
	{

		RequestUserListResult lr;
		int it = 11 + sizeof(RequestUserListResult);
		char cTempBuf[11 + sizeof(RequestUserListResult)];
		memset(cTempBuf,0,it);
		Sleep(2);
		iResult = cs->Read(cTempBuf,it);

		if(iResult < 1)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,GETORGPBSTAFF,cTempBuf))
		{
			lPoint->push_back(lr);
			iTotleCount = lr.iTotle ;
			if(lr.iTotle == lr.iHasRecv)
				break;
		}
	}
	if(lPoint->size() != iTotleCount)
	{
		return -2;
	}
	return 1;
}

///////////////////////////////////////////////////
int cData::PGPSDayData(const UserGps value,lOrgLineResult *lPoint)
{
	char pDataBuffer[11 + sizeof(UserGps)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(UserGps);
	bs.nMsgNumber = GETGPSDATE_PACKET;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&value,sizeof(UserGps));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(UserGps));
	if(iResult < iSocketState)
	{
		return -1;
	}
	int iTotleCount = 0;
	lPoint->clear();
	while(1)
	{

		OrgLineResults lr;
		int it = 11 + sizeof(OrgLineResults);
		char cTempBuf[11 + sizeof(OrgLineResults)];
		memset(cTempBuf,0,it);
		Sleep(2);
		iResult = cs->Read(cTempBuf,it);

		if(iResult < 1)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,GETGPSDATE_PACKET,cTempBuf))
		{
			lPoint->push_back(lr);
			iTotleCount = lr.totlePacket ;
			if(lr.CurrentPacket == lr.totlePacket)
				break;
		}
	}
	if(lPoint->size() != iTotleCount)
	{
		return -2;
	}
	return 1;
}

int cData::GetXjRymx(const ryxj1 value,lryxj1result *lPoint)
{
	char pDataBuffer[11 + sizeof(ryxj1)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(ryxj1);
	bs.nMsgNumber = XJMX_PACKET;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&value,sizeof(ryxj1));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(ryxj1));
	if(iResult < iSocketState)
	{
		return -1;
	}
	int iTotleCount = 0;
	lPoint->clear();
	while(1)
	{

		ryxj1result lr;
		int it = 11 + sizeof(ryxj1result);
		char cTempBuf[11 + sizeof(ryxj1result)];
		memset(cTempBuf,0,it);
		Sleep(2);
		iResult = cs->Read(cTempBuf,it);

		if(iResult < 1)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,XJMX_PACKET,cTempBuf))
		{
			lPoint->push_back(lr);
			iTotleCount = lr.totlePacket ;
		if(lr.CurrentPacket == lr.totlePacket)
				break;
		}
	}
	if(lPoint->size() != iTotleCount)
	{
		return -2;
	}
	return 1;
}

//删除全部 解决任务下面的人员
int cData::DelXjRymx(const jjDel value)
{
	char pDataBuffer[11 + sizeof(jjDel)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(jjDel);
	bs.nMsgNumber = DELALLJJRY_PACKET;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,(char *)&value,sizeof(jjDel));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(jjDel));
	if(iResult < iSocketState)
	{
		return -1;
	}

	BaseResult lr;
	int it = 11 + sizeof(BaseResult);
	char cTempBuf[11 + sizeof(BaseResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}
	if(DepressPacket(lr,DELALLJJRY_PACKET	,cTempBuf))
		return lr.result;
	return 1;

}

//获取紧急任务列表
int cData::GetJJRWList(const int orgid,ljjListresult *lPoint)
{
	
//	jjListresult;
//typedef list<jjListresult> ljjListresult;
//typedef ljjListresult::iterator IterljjListresult;
	char pDataBuffer[11 + sizeof(int)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(int);
	bs.nMsgNumber = GETJJRWLIST_PACKET;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&orgid,sizeof(int));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(int));
	if(iResult < iSocketState)
	{
		return -1;
	}
	int iTotleCount = 0;
	lPoint->clear();
	while(1)
	{

		jjListresult lr;
		int it = 11 + sizeof(jjListresult);
		char cTempBuf[11 + sizeof(jjListresult)];
		memset(cTempBuf,0,it);
		Sleep(2);
		iResult = cs->Read(cTempBuf,it);

		if(iResult < 1)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,GETJJRWLIST_PACKET,cTempBuf))
		{
			lPoint->push_back(lr);
			iTotleCount = lr.totlePacket ;
			if(lr.CurrentPacket == lr.totlePacket)
				break;
		}
	}
	if(lPoint->size() != iTotleCount)
	{
		return -2;
	}
	return 1;
}
	//获取紧急任务人员列表
int cData::GetJJry(const int rwid,ljjryListresult *lPoint)
{
//	jjryListresult;
//typedef list<jjryListresult> ljjryListresult;
//typedef ljjryListresult::iterator IterlljjryListresult;
	char pDataBuffer[11 + sizeof(int)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(int);
	bs.nMsgNumber = GETJJRWRYLIST_PACKET;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&rwid,sizeof(int));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(int));
	if(iResult < iSocketState)
	{
		return -1;
	}
	int iTotleCount = 0;
	lPoint->clear();
	while(1)
	{

		jjryListresult lr;
		int it = 11 + sizeof(jjryListresult);
		char cTempBuf[11 + sizeof(jjryListresult)];
		memset(cTempBuf,0,it);
		Sleep(2);
		iResult = cs->Read(cTempBuf,it);

		if(iResult < 1)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,GETJJRWRYLIST_PACKET,cTempBuf))
		{
			lPoint->push_back(lr);
			iTotleCount = lr.totlePacket ;
			if(lr.CurrentPacket == lr.totlePacket)
				break;
		}
	}
	if(lPoint->size() != iTotleCount)
	{
		return -2;
	}
	return 1;
}

int cData::SetSys(const sysBak sValue)
{
	char pDataBuffer[11 + sizeof(sysBak)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(sysBak);
	bs.nMsgNumber = SYSSET_PACKET;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(sysBak));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(sysBak));
	if(iResult < iSocketState)
	{
		return -1;
	}

	BaseResult lr;
	int it = 11 + sizeof(BaseResult);
	char cTempBuf[11 + sizeof(BaseResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}

	if(DepressPacket(lr,SYSSET_PACKET,cTempBuf))
	{
		;
	}else
	{
		lr.result = 1;
	}

	return lr.result;

}
int cData::GetSys(sysBak* sValue)
{
	char pDataBuffer[11 ];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = 0;
	bs.nMsgNumber = SYSGET_PACKET;
	BuildDataPackShell(pDataBuffer, &bs);
	//memcpy(pDataBuffer+7,&sValue,sizeof(CPassRequest));

	int iResult = cs->Write(pDataBuffer,11);
	if(iResult < iSocketState)
	{
		return -1;
	}

	//BaseResult lr;
	int it = 11 + sizeof(sysBak);
	char cTempBuf[11 + sizeof(sysBak)];
	memset(cTempBuf,0,it);
	//memset(sValue,0,sizeof(sysBak));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}

	if(DepressPacket(*sValue,SYSGET_PACKET,cTempBuf))
	{
		return 0;
	}else
	{
		return 1;
	}

	return 0;
}
int cData::ChaengPassWord(CPassRequest* sValue)
{

	char pDataBuffer[11 + sizeof(CPassRequest)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(CPassRequest);
	bs.nMsgNumber = CHANGEPASSWORD_PACKET;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,sValue,sizeof(CPassRequest));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(CPassRequest));
	if(iResult < iSocketState)
	{
		return -1;
	}

	BaseResult lr;
	int it = 11 + sizeof(BaseResult);
	char cTempBuf[11 + sizeof(BaseResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}

	if(DepressPacket(lr,CHANGEPASSWORD_PACKET,cTempBuf))
	{
		;
	}else
	{
		lr.result = 1;
	}

	return lr.result;
}
int cData::LinesMang(lines* sValue)
{
	char pDataBuffer[11 + sizeof(lines)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(lines);
	bs.nMsgNumber = LINEMANGMENT;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,sValue,sizeof(lines));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(lines));
	if(iResult < iSocketState)
	{
		return -1;
	}

	BaseResult lr;
	int it = 11 + sizeof(BaseResult);
	char cTempBuf[11 + sizeof(BaseResult)];
	memset(cTempBuf,0,it);
	memset(&lr,0,sizeof(lr));
	iResult = cs->Read(cTempBuf,it);

	if(iResult < 0)
	{
		return -1;
	}

	if(DepressPacket(lr,LINEMANGMENT,cTempBuf))
	{
		;
	}else
	{
		lr.result = 1;
	}

	return lr.result;
}

int cData::GetWarnList(WarnRequest sValue,lWarnRequestReturn* warnValue)
{
	char pDataBuffer[11 + sizeof(WarnRequest)];
	memset(pDataBuffer,0,sizeof(pDataBuffer));
	BaseStruct bs;
	bs.nBodyLength = sizeof(WarnRequest);
	bs.nMsgNumber = ALLWARN_PACKET;
	BuildDataPackShell(pDataBuffer, &bs);
	memcpy(pDataBuffer+7,&sValue,sizeof(WarnRequest));

	int iResult = cs->Write(pDataBuffer,11 + sizeof(WarnRequest));
	if(iResult < iSocketState)
	{
		return -1;
	}
	int iTotleCount = 0;
	warnValue->clear();
	while(1)
	{

		WarnRequestReturn lr;
		int it = 11 + sizeof(WarnRequestReturn);
		char cTempBuf[11 + sizeof(WarnRequestReturn)];
		memset(cTempBuf,0,it);
		Sleep(2);
		iResult = cs->Read(cTempBuf,it);

		if(iResult < 1)
		{
			break;
		}
		memset(&lr,0,sizeof(lr));
		if(DepressPacket(lr,ALLWARN_PACKET,cTempBuf))
		{
			warnValue->push_back(lr);
			iTotleCount = lr.totlePacket ;
			if(lr.CurrentPacket == lr.totlePacket)
				break;
		}
	}
	if(warnValue->size() != iTotleCount)
	{
		return -2;
	}
	return 0;
}
////////////////////////////////////////////////////
template<typename T>
bool cData::DepressPacket(T& dataDownLoadPack, unsigned char cMsgNumber,const char* buf)
{
	ASSERT(buf);
	if(buf)
	{
		if(*((unsigned int*)buf) == PACK_IDENTIFY)
		{
			if(*((unsigned char*)(buf + 4)) == cMsgNumber)
			{
				unsigned short iLength = *((unsigned short*)(buf + 5));
				memcpy(&dataDownLoadPack,buf + 7 ,iLength);
				return true;
			}
		}
	}
	return false;
}

bool cData::BuildDataPackShell(char* pDataBuffer, BaseStruct* gpsDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)pDataBuffer) = PACK_IDENTIFY;
		*((unsigned char*)(pDataBuffer + 4)) = gpsDownLoadPack->nMsgNumber;
		*((unsigned short*)(pDataBuffer + 5)) = gpsDownLoadPack->nBodyLength;
		*((unsigned int*)(pDataBuffer + gpsDownLoadPack->nBodyLength + 7)) = PACK_IDENTIFY;

		return true;
	}
	return false;
}

bool cData::BuildLoginPackData(char* pDataBuffer, const tUserLogin& loginDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{

		memcpy(pDataBuffer,loginDownLoadPack.gDataBodyPack.Name,sizeof(loginDownLoadPack.gDataBodyPack.Name));
		memcpy(pDataBuffer + sizeof(loginDownLoadPack.gDataBodyPack.Name) ,loginDownLoadPack.gDataBodyPack.Pwd,sizeof(loginDownLoadPack.gDataBodyPack.Pwd));
		return true;
	}
	return false;
}

