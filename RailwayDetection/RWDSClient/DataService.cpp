#include "StdAfx.h"
#include "DataService.h"
#include "ErrorDefine.h"
#include "CmdDefine.h"
#include "cData.h"


int VerifyLogin( const CString& aLoginAccount, const CString& aLoginPassword, int* orgID, Permission *pPower)
{
    cData *cd = new cData();
	char  *UserName = (LPSTR)(LPCTSTR)aLoginAccount; 
	char  *PassWord = (LPSTR)(LPCTSTR)aLoginPassword;
	int iOrgid,p1,p2,p3;
	int iResult = cd->UserLog(UserName,PassWord,&iOrgid,&p1,&p2,&p3);
	if(iResult > 0)
	{
		*orgID = iOrgid;
		pPower->iBasical = p1;
		pPower->iOperate = p2;
		pPower->iReportForm = p3;
	}
	return iResult;
}



int GetOrgTree(const int& OrgId, vector<OrganizationInfo*>* a_OrgTree)
{
	
	cData *cd = new cData();
	lOrg pOrg;
	int iResult = cd->GetOrgList(OrgId,&pOrg);
	if(iResult < 1)
	{
		return -1;
	}
	for(iterOrg iter = pOrg.begin() ; iter !=pOrg.end() ;iter++)
	{
		OrganizationInfo* org = new OrganizationInfo;
		org->iOrgName=      iter->OrgName;
		org->iOrgAddress =  iter->Address;
		org->iParentID = iter->UpOrg ;
		org->iOrgLevel =    iter->iLevel;
		org->iParentOrg =   NULL;
		org->iOrgID =      iter->OrgId;
		org->iBoundaryRail      = (RailLine)iter->LineId ;
		org->iBoundaryStartKM = iter->startid ;
		a_OrgTree->push_back(org);
	}

	for(vector<OrganizationInfo*>::iterator iterOrg = a_OrgTree->begin() ; iterOrg != a_OrgTree->end() ; a_OrgTree++)
	{
		CherkOrgType(a_OrgTree,*iterOrg);
	}
	delete cd;

    return KErrNone;
}

void CherkOrgType(vector<OrganizationInfo*>* a_OrgTree,OrganizationInfo* OrgInfo)
{
	for(vector<OrganizationInfo*>::iterator iterOrg = a_OrgTree->begin() ; iterOrg != a_OrgTree->end() ; a_OrgTree++)
	{
		//获取下属机构
		if(OrgInfo->iOrgID == (*iterOrg)->iParentID)
		{
			OrgInfo->iChildID.push_back((*iterOrg)->iOrgID);
		}
		//获取上级机构
		if(OrgInfo->iParentID ==(*iterOrg)->iOrgID )
		{
			OrgInfo->iParentOrg = *iterOrg;
			(*iterOrg)->iChildOrg.push_back(OrgInfo);
		}
	}
	return;
}

int SetOrganization(int aCmd, const OrganizationInfo* aOrganization )
{
	int iResult;
	cData *cd = new cData();
	switch(aCmd)
	{
	case CMD_ORG_ADD:
		{

		
		AddOrg aOrg;
		memset(&aOrg,0,sizeof(AddOrg));
		aOrg.UpOrg = aOrganization->iParentID ;
		aOrg.iLevel = aOrganization->iOrgLevel ;
		aOrg.LineId = aOrganization->iBoundaryRail;
		aOrg.startid = aOrganization->iBoundaryStartKM;
		aOrg.EndID = aOrganization->iBoundaryEndKM;
		char *pTemp = (LPSTR)(LPCTSTR)aOrganization->iOrgAddress;
		memcpy(&aOrg.Address,pTemp,sizeof(pTemp));

		pTemp = (LPSTR)(LPCTSTR)aOrganization->iOrgName;
		memcpy(&aOrg.OrgName,pTemp,sizeof(pTemp));
		iResult = cd->AddOrgs(&aOrg);
		break;
		}
	case CMD_ORG_MODIFY:
		{
			EditOrg eOrg;
			memset(&eOrg,0,sizeof(EditOrg));
			char *pTemp = (LPSTR)(LPCTSTR)aOrganization->iOrgAddress;
			memcpy(&eOrg.Address,pTemp,sizeof(pTemp));

			pTemp = (LPSTR)(LPCTSTR)aOrganization->iOrgName;
			memcpy(&eOrg.OrgName,pTemp,sizeof(pTemp));

			eOrg.UpOrg = aOrganization->iParentID ;
			eOrg.iLevel = aOrganization->iOrgLevel ;
			eOrg.LineId = aOrganization->iBoundaryRail;
			eOrg.startid = aOrganization->iBoundaryStartKM;
			eOrg.EndID = aOrganization->iBoundaryEndKM;
			eOrg.OrgId = aOrganization->iOrgID ;
			iResult = cd->EditOrgs(&eOrg);
			break;
		}
	case CMD_ORG_DELETE:
		DelOrg dOrg;
		dOrg.OrgId = aOrganization->iOrgID;
		iResult = cd->DelOrgs(&dOrg);
		break;
	default:
		break;
	}
    return KErrNone;
}

//获取机构有效巡检范围 比如 成昆铁路 1- 8K出
int GetOrgPoint(int iOrgID, vector<MapPoint*>* aPointList)
{

	cData *cd = new cData();
	lOrgLine llist;
	if(cd->GetOrgPoint(iOrgID,&llist) > 0)
	{
		for(iterOrgLine iter = llist.begin() ; iter != llist.end() ; iter++)
		{
			MapPoint *pt = new MapPoint;
			pt->iRailLine = (RailLine)iter->lineid;
			pt->iKM = iter->gls;
			pt->iLat = iter->jdu;
			pt->iLon = iter->wdu;
	
			if(iter->iState == 1)
				pt->iDirect = KDownLine;
			else
				pt->iDirect = KUpLine;

			aPointList->push_back(pt);
			delete pt;
		}
		return ResultOk;
	}else
	{
		return KErrNone;
	}
    
}


int SetOrgPoint( int aOrgID, int aCmd, const MapPoint* aPoint )
{
    switch (aCmd)
    {
    case CMD_POINT_ADD:
        break;
    case CMD_POINT_MODIFY:
        break;
    case CMD_POINT_DELETE:
        break;
    default:
        break;
    }

    return KErrNone;
}

int GetOrgLine(int iOrgID, const vector<MapPoint*>& aPointList, vector<LineInfo*>* aLineList)
{
    LineInfo *line = new LineInfo;
    line->iLineID = 1;
    line->iLineName = _T("成局1段");
    line->iStartKm = (aPointList)[0]->iKM;
    line->iStartNo = KFirstDay;
    line->iLineKmLonLat.push_back(aPointList[0]);
    line->iLineKmTime.push_back(100);
    line->iLineKmLonLat.push_back(aPointList[1]);
    line->iLineKmTime.push_back(100);
    line->iLineKmLonLat.push_back(aPointList[2]);
    line->iLineKmTime.push_back(100);
    line->iLineKmLonLat.push_back(aPointList[3]);
    line->iLineKmTime.push_back(100);
    aLineList->push_back(line);

    line = new LineInfo;
    line->iLineID = 2;
    line->iLineName = _T("成局2段");
    line->iStartKm = aPointList[1]->iKM;
    line->iStartNo = KThirdDay;
    line->iLineKmLonLat.push_back(aPointList[1]);
    line->iLineKmTime.push_back(0);
    line->iLineKmLonLat.push_back(aPointList[3]);
    line->iLineKmTime.push_back(0);
    aLineList->push_back(line);
    return KErrNone;
	
}

int SetOrgLine( int aOrgID, int aCmd, const LineInfo* aLine )
{
    //aLine->
	//aLine->
	return KErrNone;
}

int GetOrgStaff(int aOrgID, vector<StaffInfo*>* aStaffList)
{
    
	cData *cd = new cData();
	lUser lPoint;
	if(cd->GetUserList(aOrgID,1,&lPoint) > 0)
	{
		for(iterUser iter = lPoint.begin() ; iter != lPoint.end() ; iter++)
		{
			StaffInfo* staff = new StaffInfo;
			staff->iID = iter->Oper;
			staff->iOrgID = iter->orgid;
			staff->iPassword = _T("");
			staff->iLoginPermission = TRUE;
			staff->iName = iter->name;
			staff->Pda1 = iter->pda1;
			staff->Pda2 = iter->pda2;
			staff->PowerId = iter->PowerGroup ;
			aStaffList->push_back(staff);
		}
		return ResultOk;
	}

    return KErrNone;
}

int SetOrgStaff( int aOrgID, int aCmd, const StaffInfo* aStaff )
{
   // aStaff->
	/*
	#define CMD_STAFF_ADD 0x36
	#define CMD_STAFF_MODIFY 0x37
	#define CMD_STAFF_DELETE 0x38
	*/
	int iResult = 0;
	cData *cd = new cData();
	switch(aCmd)
	{
	case CMD_STAFF_ADD:
		{
			AddUser aUser;
			memset(&aUser,0,sizeof(AddUser));
			char *pTemp = (LPSTR)(LPCTSTR)aStaff->iName;
			memcpy(&aUser.name,pTemp,sizeof(pTemp));

			pTemp = (LPSTR)(LPCTSTR)aStaff->iID;
			memcpy(&aUser.Oper,pTemp,sizeof(pTemp));
	
			aUser.orgid = aStaff->iOrgID;
			aUser.pda1 = aStaff->Pda1 ;
			aUser.pda2 = aStaff->Pda2;
			aUser.PowerGroup = aStaff->PowerId;
			int iLogin = 1;
			if(aStaff->iLoginPermission)iLogin = 1;
			else iLogin = 0;
			aUser.UserState = iLogin;
			iResult = cd->AddUsers(&aUser);
			break;
		}
	case CMD_STAFF_MODIFY:
		{
			EditUser eUser;
			memset(&eUser,0,sizeof(EditUser));
			char *pTemp = (LPSTR)(LPCTSTR)aStaff->iName;
			memcpy(&eUser.name,pTemp,sizeof(pTemp));

			pTemp = (LPSTR)(LPCTSTR)aStaff->iID;
			memcpy(&eUser.Oper,pTemp,sizeof(pTemp));

			eUser.orgid = aStaff->iOrgID;
			eUser.pda1 = aStaff->Pda1 ;
			eUser.pda2 = aStaff->Pda2;
			eUser.PowerGroup = aStaff->PowerId;
			int iLogin = 1;
			if(aStaff->iLoginPermission)iLogin = 1;
			else iLogin = 0;
			eUser.UserState = iLogin;
			iResult = cd->EditUsers(&eUser);
			break;
		}
	case CMD_STAFF_DELETE:
		{
			DelUser dUser;
			memset(&dUser,0,sizeof(DelUser));
			char *pTemp ;

			pTemp = (LPSTR)(LPCTSTR)aStaff->iID;
			memcpy(&dUser.Oper,pTemp,sizeof(pTemp));
			iResult = cd->DelUsers(&dUser);
			break;
		}
	}
	return iResult;
}
//获取排版下 实际线路ID 
int GetCalendarSchedule(int aOrgID, const vector<StaffInfo*>* ListStaff, CalendarSchedule* aSchedule)
{

	cData *cd = new cData();
	OrgPxInfo oInfo ;
	oInfo.Orgid = aOrgID;
	GetOrgPxInfo *llist;
	cd->GetOrgpInfo(oInfo,llist);
	aSchedule->iCaledarID = llist->id ;
	aSchedule->iPeriods = llist->cyc;
	delete cd;

    return KErrNone;
}
//
int SetCalendarSchedule(int aOrgID, const CalendarSchedule* aSchedule/*, const <StaffInfo*>*ListStaff*/)
{
	//aSchedule->iDateSchedule[0]->iArrangeStaff
	//aSchedule->iDateSchedule
	//aSchedule->iDateSchedule
    return KErrNone;
}


int GetEmergencyTask( int aOrgID, vector<EmergencyTaskInfo*>* m_EmergencyList )
{
    return KErrNone;
}

int SetEmergencyTask( int aOrgID, int aCmd, const EmergencyTaskInfo* aEmergencyTask )
{
   // aEmergencyTask->
	return KErrNone;
}

int GetOrgDevice( int aOrgID, vector<DeviceInfo*>* aDeviceList )
{
	cData *cd = new cData();
	RequestDeviceList rRequest;
	rRequest.Devicetype = 2;
	rRequest.Orgid = aOrgID;
	rRequest.type = 1;
	lDevice *lPoint;
	if(cd->GetDeviceList(rRequest,lPoint) < 0)
	{
		return KErrNone;
	}
	aDeviceList->clear();
	for(iterDevice iter = lPoint->begin() ; iter !=lPoint->end();iter++)
	{
		DeviceInfo dInfo;
		memset(&dInfo,0,sizeof(DeviceInfo));
		dInfo.iDeviceType = (DeviceType)iter->DeviceType; 
		dInfo.iOrgID = aOrgID;
		dInfo.iDevID = iter->DeviceId;

		char *pTemp ;
		pTemp = (LPSTR)(LPCTSTR)iter->DevicePid;
		memcpy(&dInfo.iPhoneNum,pTemp,sizeof(pTemp));

		aDeviceList->push_back(&dInfo);
	}
	return ResultOk;
}

int SetOrgDevice( int aOrgID, int aCmd, const DeviceInfo* aDeviceList )
{
//#define CMD_DEVICE_ADD 0x40
//#define CMD_DEVICE_MODIFY 0x41
//#define CMD_DEVICE_DELETE 0x42
	int iResult;
	cData *cd = new cData();
	switch(aCmd)
	{
	case CMD_DEVICE_ADD:
		{
			AddDevice *aDevice;
			memset(aDevice,0,sizeof(AddDevice));
			aDevice->DeviceOrg = aDeviceList->iOrgID;
			aDevice->DeviceType = aDeviceList->iDeviceType;

			char *pTemp = (LPSTR)(LPCTSTR)aDeviceList->iPhoneNum;
			memcpy(&aDevice->DevicePid,pTemp,sizeof(pTemp));
			iResult = cd->AddDevices(aDevice);
			break;
		}
	case CMD_DEVICE_MODIFY:
		{
			EditDevice *aDevice;
			memset(aDevice,0,sizeof(EditDevice));
			aDevice->DeviceOrg = aDeviceList->iOrgID;
			aDevice->DeviceType = aDeviceList->iDeviceType;
			aDevice->DeviceId =aDeviceList->iDevID; 
			char *pTemp = (LPSTR)(LPCTSTR)aDeviceList->iPhoneNum;
			memcpy(&aDevice->DevicePid,pTemp,sizeof(pTemp));
			iResult = cd->EditDevices(aDevice);
			break;
		}
	case CMD_DEVICE_DELETE:
		{
			DelDevice dDevice;
			dDevice.DeviceId = aDeviceList->iDevID;
			iResult = cd->DelDevices(&dDevice);
			break;
		}
	default:
		break;
	}
	if(iResult < 0)
	{
		return KErrNone;
	}else
		return ResultOk;
	
}

int GetStaffCurrentTrack(time_t aDate, RecordStaff* aStaff)
{
    //Get the data by aStaff->iStaff->iID

    aStaff->iRecordLon.push_back(104.064631);
    aStaff->iRecordLat.push_back(30.698965);

    aStaff->iRecordLon.push_back(104.075630);
    aStaff->iRecordLat.push_back(30.699584);

    aStaff->iRecordLon.push_back(104.086426);
    aStaff->iRecordLat.push_back(30.699584);

    return KErrNone;
}

int GetStaffScheduleTrack(int aStaffID, time_t aDate, vector<double>* aRecordLon, vector<double>* aRecordLat )
{
    return KErrNone;
}

int GetPictureInfo( int aOrgID, time_t aStartDate, time_t aEndDate, vector<PictureInfo*>* aPictureList )
{
    PictureInfo* picture = new PictureInfo;
    picture->iPicID = 1;
    picture->iPicName = _T("516823.jpg");
    picture->iShootingTime = GetCurrentTime();
    picture->iErrorType = 0;

    aPictureList->push_back(picture);
    return KErrNone;
}

int SavePictureToDirect( int aOrgID, const PictureInfo* aPicture, CString aToDirect )
{
    return KErrNone;
}

