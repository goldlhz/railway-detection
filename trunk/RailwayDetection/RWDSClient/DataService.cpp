#include "StdAfx.h"
#include "DataService.h"
#include "ErrorDefine.h"
#include "CmdDefine.h"
#include "cData.h"

#define TESTCODE

int VerifyLogin( CString& aLoginAccount, CString& aLoginPassword, int* orgID, PermissionGroup *pPower)
{
    ///////////////////////////////////////////////////
#ifdef TESTCODE
    *orgID = 1;
    //pPower->iBasical = 0x0FFF;
    pPower->iBasical = 3967;
    pPower->iOperate = 1;
    pPower->iReportForm = 1;
    return KErrNone;
#endif
    ///////////////////////////////////////////////////
    cData *cd = new cData();
	char  *UserName = (LPSTR)(LPCTSTR)aLoginAccount; 
	char  *PassWord = (LPSTR)(LPCTSTR)aLoginPassword;
    //aLoginAccount.GetBuffer();
	int iOrgid,p1,p2,p3;
	int iResult = cd->UserLog(aLoginAccount.GetBuffer(),aLoginPassword.GetBuffer(),&iOrgid,&p1,&p2,&p3);
	if(iResult == 0)
	{
		*orgID = iOrgid;
		pPower->iBasical = p1;
		pPower->iOperate = p2;
		pPower->iReportForm = p3;
        if (iOrgid <= 0)
        {
            iResult = KErrNotFound;
        }
	}
    aLoginAccount.ReleaseBuffer();
    aLoginPassword.ReleaseBuffer();
	return iResult;
}

int GetRailLine( vector<RailLine*>* aRailLineList )
{
    RailLine* root = new RailLine;
    root->iRailID = 0;
    root->iRailName = _T("未知线路");
    aRailLineList->push_back(root);
    ///////////////////////////////////////////////////
#ifdef TESTCODE
    RailLine* rail = new RailLine;
    rail->iRailID = 1;
    rail->iRailName = _T("宝成线");
    aRailLineList->push_back(rail);

    rail = new RailLine;
    rail->iRailID = 2;
    rail->iRailName = _T("成昆线");
    aRailLineList->push_back(rail);

    rail = new RailLine;
    rail->iRailID = 3;
    rail->iRailName = _T("成渝线");
    aRailLineList->push_back(rail);

    rail = new RailLine;
    rail->iRailID = 4;
    rail->iRailName = _T("成东线");
    aRailLineList->push_back(rail);

    rail = new RailLine;
    rail->iRailID = 5;
    rail->iRailName = _T("成西线");
    aRailLineList->push_back(rail);
    
#endif
    ///////////////////////////////////////////////////
    return KErrNone;
}

int SetRailLine( int aCmd, const RailLine* aRailLine )
{
    ///////////////////////////////////////////////////
#ifdef TESTCODE
    return KErrNone;
#endif
    ///////////////////////////////////////////////////
    return KErrNone;

//#define CMD_RAIL_ADD 0x43
//#define CMD_RAIL_MODIFY 0x44
//#define CMD_RAIL_DELETE 0x45
}

RailLine* GetRailLineByID(vector<RailLine*>& aRailLineList, int aRailID )
{
    RailLine* retValue = aRailLineList[0];
    for (size_t i=0; i<aRailLineList.size(); i++)
    {
        if (aRailID == aRailLineList[i]->iRailID)
        {
            retValue = aRailLineList[i];
            break;
        }
    }
    return retValue;
}

RailLine* GetRailLineByName(vector<RailLine*>& aRailLineList, CString aRailName )
{
    RailLine* retValue = aRailLineList[0];
    for (size_t i=0; i<aRailLineList.size(); i++)
    {
        if (aRailName == aRailLineList[i]->iRailName)
        {
            retValue = aRailLineList[i];
            break;
        }
    }
    return retValue;
}

int GetOrgTree(const int OrgId, vector<OrganizationInfo*>* a_OrgTree)
{
    ///////////////////////////////////////////////////
    #ifdef TESTCODE
    OrganizationInfo* org = new OrganizationInfo;
    org->iOrgName = _T("Admin");
    org->iParentOrg = NULL;
    org->iParentID = 0;
    org->iOrgID = 1;
    org->iBoundaryRail = 1;
    org->iChildID.push_back(2);
    org->iChildID.push_back(3);
    a_OrgTree->push_back(org);

    org = new OrganizationInfo;
    org->iOrgName = _T("Child1");
    org->iParentOrg = (*a_OrgTree)[0];
    org->iParentID = org->iParentOrg->iOrgID;
    (*a_OrgTree)[0]->iChildOrg.push_back(org);
    org->iOrgID = 2;
    org->iBoundaryRail = 2;
    org->iChildID.push_back(4);
    a_OrgTree->push_back(org);

    org = new OrganizationInfo;
    org->iOrgName = _T("Child2");
    org->iParentOrg = (*a_OrgTree)[0];
    org->iParentID = org->iParentOrg->iOrgID;
    (*a_OrgTree)[0]->iChildOrg.push_back(org);
    org->iBoundaryRail = 3;
    org->iOrgID = 3;
    org->iChildID.push_back(5);
    a_OrgTree->push_back(org);
    return KErrNone;
#endif
    ///////////////////////////////////////////////////
	
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
        org->iBoundaryRail      = iter->LineId ;
        org->iBoundaryStartKM = iter->startid ;
        org->iBoundaryEndKM = iter->EndID;
        a_OrgTree->push_back(org);
    }

    for(vector<OrganizationInfo*>::iterator iterOrg = a_OrgTree->begin() ; iterOrg != a_OrgTree->end() ; iterOrg++)
    {
        CherkOrgType(a_OrgTree,*iterOrg);
    }
    delete cd;

    return KErrNone;
}

void CherkOrgType(vector<OrganizationInfo*>* a_OrgTree,OrganizationInfo* OrgInfo)
{
	for(vector<OrganizationInfo*>::iterator iterOrg = a_OrgTree->begin() ; iterOrg != a_OrgTree->end() ; iterOrg++)
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
		char *p1;
		CString cTEMP = aOrganization->iOrgAddress;
		p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
		memcpy(&aOrg.Address,p1,cTEMP.GetLength());
		cTEMP.ReleaseBuffer();
		cTEMP = aOrganization->iOrgName;
		p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
		memcpy(&aOrg.OrgName,p1,cTEMP.GetLength());
		cTEMP.ReleaseBuffer();

		iResult = cd->AddOrgs(&aOrg);

		cTEMP.ReleaseBuffer();
		break;
		}
	case CMD_ORG_MODIFY:
		{
			EditOrg eOrg;
			memset(&eOrg,0,sizeof(EditOrg));

			eOrg.UpOrg = aOrganization->iParentID ;
			eOrg.iLevel = aOrganization->iOrgLevel ;
			eOrg.LineId = aOrganization->iBoundaryRail;
			eOrg.startid = aOrganization->iBoundaryStartKM;
			eOrg.EndID = aOrganization->iBoundaryEndKM;
			eOrg.OrgId = aOrganization->iOrgID ;
			char *p1;
			CString cTEMP = aOrganization->iOrgAddress;
			p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
			memcpy(&eOrg.Address,p1,cTEMP.GetLength());
			cTEMP.ReleaseBuffer();

			cTEMP = aOrganization->iOrgName;
			p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
			memcpy(&eOrg.OrgName,p1,cTEMP.GetLength());
			cTEMP.ReleaseBuffer();

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
int GetOrgPoint(int aOrgID, vector<RailLine*>& aRailLineList, vector<MapPoint*>* aPointList)
{
    ///////////////////////////////////////////////////
    #ifdef TESTCODE
    MapPoint *pt = new MapPoint;
    pt->iRailLine = aRailLineList[1]; 
    pt->iKM = 251;
    pt->iLon = 104.064531;
    pt->iLat = 30.699965;
    pt->iDirect = KUpLine;
    aPointList->push_back(pt);

    pt = new MapPoint;
    pt->iRailLine = aRailLineList[1]; 
    pt->iKM = 252;
    pt->iLon = 104.075530;
    pt->iLat = 30.699484;
    pt->iDirect = KUpLine;
    aPointList->push_back(pt);

    pt = new MapPoint;
    pt->iRailLine = aRailLineList[1]; 
    pt->iKM = 253;
    pt->iLon = 104.086526;
    pt->iLat = 30.699484;
    pt->iDirect = KUpLine;
    aPointList->push_back(pt);

    pt = new MapPoint;
    pt->iRailLine = aRailLineList[1]; 
    pt->iKM = 254;
    pt->iLon = 104.097521;
    pt->iLat = 30.699484;
    pt->iDirect = KUpLine;
    aPointList->push_back(pt);
    return KErrNone;
#endif
    ///////////////////////////////////////////////////

	cData *cd = new cData();
	lOrgLine llist;
	if(cd->GetOrgPoint(aOrgID,&llist) > 0)
	{
        
		for(iterOrgLine iter = llist.begin() ; iter != llist.end() ; iter++)
		{
			MapPoint *pt = new MapPoint;
			pt->iRailLine = GetRailLineByID(aRailLineList, iter->lineid);
			pt->iKM = iter->gls;
			pt->iLat = iter->wdu;
			pt->iLon = iter->jdu;
			pt->iPointId = iter->iPoint;
	
			if(iter->iState == 1)
				pt->iDirect = KDownLine;
			else
				pt->iDirect = KUpLine;

			aPointList->push_back(pt);
			//delete pt;
		}
		return ResultOk;
	}else
	{
		return KErrNone;
	}
    
}


int SetOrgPoint( int aOrgID, int aCmd, const MapPoint* aPoint )
{
    ///////////////////////////////////////////////////
#ifdef TESTCODE
    return KErrNone;
#endif
    ///////////////////////////////////////////////////
    cData *cd = new cData();
	int iResult = 0;
	PointMang pPoint;
	switch (aCmd)
    {
    case CMD_POINT_ADD:
		{
			pPoint.iDirect = (int)aPoint->iDirect;
			pPoint.iKM = aPoint->iKM;
			pPoint.iLat = aPoint->iLat;
			pPoint.iLon = aPoint->iLon;
			pPoint.iRailLine = aPoint->iRailLine->iRailID;
			pPoint.itype = 0;//tianjia
            pPoint.PointId = aPoint->iPointId;
			iResult=cd->setPoint(pPoint);
			break;
		}
    case CMD_POINT_MODIFY:
		{
			pPoint.iDirect = (int)aPoint->iDirect;
			pPoint.iKM = aPoint->iKM;
			pPoint.iLat = aPoint->iLat;
			pPoint.iLon = aPoint->iLon;
			pPoint.iRailLine = aPoint->iRailLine->iRailID;
			pPoint.itype = 1;//tianjia
			pPoint.PointId = aPoint->iPointId;
			iResult=cd->setPoint(pPoint);
        break;
		}
    case CMD_POINT_DELETE:
        {
			pPoint.PointId = aPoint->iPointId;
			pPoint.itype = 2;//tianjia
			iResult=cd->setPoint(pPoint);
			break;
		}
    default:
        break;
    }
	delete cd;
    return iResult;//设置正常，返回点ID
}

int GetOrgLine(int aOrgID, const vector<MapPoint*>& aPointList, vector<LineInfo*>* aLineList)
{
    ///////////////////////////////////////////////////
#ifdef TESTCODE
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
#endif
    ///////////////////////////////////////////////////
    aLineList->clear();
	lallOrgLine llist ;
	Orglines ol;
	ol.orgid = aOrgID;
	cData *cd = new cData();
	if(1 == cd->GetOrgLine(ol,&llist))
	{
		for(riterOrgLine iter = llist.begin() ; iter != llist.end() ; iter++)
		{
			LineInfo *line = new LineInfo;
			//memset(line,0,sizeof(LineInfo));
			line->iLineID = iter->lineid;
			line->iLineName = iter->line;
			line->iStartNo = (LineStartNo)iter->lindday;
			//int rGetLineTime(const rLinePointTime sValue,lrLinePointTimeResult *llist);
			rLinePointTime sValue;
			sValue.lineid = line->iLineID ; //
			line->iOrgID = aOrgID;
			lrLinePointTimeResult llists ;
			if(0 == cd->rGetLineTime(sValue,&llists))
			{
				int i = llists.size();
				for(iterrLinePointTime it = llists.begin() ; it!= llists.end() ;it++)
				{
					MapPoint *mp;
					//mp->iDirect = (LineDirect)it->iDirect;
					//mp->iKM = it->iKM;
					//mp->iLat = it->iLat;
					//mp->iLon = it->iLon;
					//mp->iRailLine = (RailLine)sValue.lineid;
					//mp->PointId = it->PointId;
					
					bool find= false;
					for(size_t i=0; i<aPointList.size(); i++)
					{
						
						if(aPointList[i]->iPointId == it->PointId)
						{
						find = true;
						mp = aPointList[i];
						break;
						}
					}
					if(find)
					{
						line->iLineKmLonLat.push_back(mp);
						line->iLineKmTime.push_back(Time2Strings1(it->time));
					}
					//delete mp;
				}
				
			}
			aLineList->push_back(line);
			//delete line;
		}
	}
	
	return 1;
	
}

int SetOrgLine( int aOrgID, int aCmd, const LineInfo* aLine )
{
    ///////////////////////////////////////////////////
#ifdef TESTCODE
    return KErrNone;
#endif
    ///////////////////////////////////////////////////
//#define CMD_LINE_ADD 0x33
//#define CMD_LINE_MODIFY 0x34
//#define CMD_LINE_DELETE 0x35
	cData *cd = new cData();
	MangLine Ml;
	int iResult = 0;
	memset(&Ml,0,sizeof(MangLine));
	switch(aCmd)
	{
	case CMD_LINE_ADD:
		{
			Ml.type = 0;
			Ml.day = aLine->iStartNo;
			Ml.orgid = aLine->iOrgID;
			//char *pTemp = (LPSTR)(LPCTSTR)aLine->iLineName;
			//memcpy(&Ml.cName,pTemp,sizeof(pTemp));
			//Ml.cName = aLine->iLineName.GetBuffer();

			CString cTEMP = aLine->iLineName;
			char *p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
			memcpy(&Ml.cName,p1,cTEMP.GetLength());
			cTEMP.ReleaseBuffer();

			iResult = cd->SetMangLine(Ml);
			break;
		}
	case CMD_LINE_MODIFY:
		{
			Ml.type = 1;
			Ml.day = aLine->iStartNo;
			Ml.orgid = aLine->iOrgID;
			Ml.id = aLine->iLineID;
			//char *pTemp = (LPSTR)(LPCTSTR)aLine->iLineName;
			//memcpy(&Ml.cName,pTemp,sizeof(pTemp));
			//Ml.cName = aLine->iLineName.GetBuffer();

			CString cTEMP = aLine->iLineName;
			char *p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
			memcpy(&Ml.cName,p1,cTEMP.GetLength());
			cTEMP.ReleaseBuffer();

			iResult = cd->SetMangLine(Ml);
			if(iResult == 0) //增加点和时间
			{	
				Sleep(2);
				int iPointSize = aLine->iLineKmLonLat.size();
				for(int i = 0;i < iPointSize ;i++)
				{//
					int iPoint = aLine->iLineKmLonLat[i]->iPointId;
					time_t sec = aLine->iLineKmTime[i];
					int iHour,iMin;
					CTime t(sec);
					iHour = t.GetHour();
					iMin = t.GetMinute();
					CString cs;
					cs.Format(_T("%d:%d"),iHour,iMin);
					xj pb;
					memset(&pb,0,sizeof(xj));
					pb.type = 0;
					pb.lineid = aLine->iLineID;
					pb.pointed = iPoint;
					//char *pTemp = (LPSTR)(LPCTSTR)cs;
					//Time2Strings1
					CString cTEMP = cs;
					char *p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
					memcpy(&pb.time,p1,cTEMP.GetLength());
					cTEMP.ReleaseBuffer();

					//memcpy(&pb.time,pTemp,sizeof(pTemp));
					
					cd->SetxjTime(pb);
				}
				//;
			}
			break;
		}
	case CMD_LINE_DELETE:
		{
			Ml.type = 2;
			Ml.id = aLine->iLineID;
			iResult = cd->SetMangLine(Ml);
			break;
		}
	}
    delete cd;
	if(iResult==1)
	{
		return KErrNone;
	}else
		return ResultOk;
}

int GetOrgStaff(int aOrgID, const vector<DeviceInfo*>& aDeviceList, vector<StaffInfo*>* aStaffList)
{
    ///////////////////////////////////////////////////
#ifdef TESTCODE
    StaffInfo* staff = new StaffInfo;
    staff->iID = _T("1");
    staff->iOrgID = aOrgID;
    staff->iPassword = _T("111");
    staff->iLoginPermission = TRUE;
    staff->iName = _T("张三");
    staff->iPermissionGroup = 0;
    if (aDeviceList.size() > 0)
    {
        staff->iTakeDevice = aDeviceList[0];
    }
    
    aStaffList->push_back(staff);

    staff = new StaffInfo;
    staff->iID = _T("2");;
    staff->iOrgID = aOrgID;
    staff->iPassword = _T("");
    staff->iLoginPermission = FALSE;
    staff->iName = _T("李四");
    staff->iPermissionGroup = 0;
    if (aDeviceList.size() > 0)
    {
        staff->iTakeDevice = aDeviceList[0];
    }
    aStaffList->push_back(staff);

    staff = new StaffInfo;
    staff->iID = _T("3");;
    staff->iOrgID = aOrgID;
    staff->iName = _T("王五");
    staff->iPassword = _T("");
    staff->iLoginPermission = FALSE;
    staff->iPermissionGroup = 0;
    if (aDeviceList.size() > 0)
    {
        staff->iTakeDevice = aDeviceList[0];
    }
    aStaffList->push_back(staff);
    return KErrNone;
#endif
    ///////////////////////////////////////////////////
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
            for (size_t i=0; i<aDeviceList.size(); i++)
            {
                if (aDeviceList[i]->iDevID == iter->pda1)
                {
                    staff->iTakeDevice = aDeviceList[i];
                    break;
                }
            }
			staff->iPermissionGroup = iter->PowerGroup ;
			aStaffList->push_back(staff);
		}
		return ResultOk;
	}

    return KErrNone;
}

int SetOrgStaff( int aOrgID, int aCmd, const StaffInfo* aStaff )
{
    ///////////////////////////////////////////////////
#ifdef TESTCODE
    return KErrNone;
#endif
    ///////////////////////////////////////////////////
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
			//char *pTemp = (LPSTR)(LPCTSTR)aStaff->iName;
			//memcpy(&aUser.name,pTemp,sizeof(pTemp));

			//pTemp = (LPSTR)(LPCTSTR)aStaff->iID;
			//memcpy(&aUser.Oper,pTemp,sizeof(pTemp));
			//aUser.name = aStaff->iName.GetBuffer();
			//aUser.Oper = aStaff->iID.GetBuffer();
			char *p1;
			CString cTEMP = aStaff->iID;
			p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
			memcpy(&aUser.Oper,p1,cTEMP.GetLength());
			cTEMP.ReleaseBuffer();

			cTEMP = aStaff->iName;
			p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
			memcpy(&aUser.name,p1,cTEMP.GetLength());
			cTEMP.ReleaseBuffer();

			aUser.orgid = aStaff->iOrgID;
			aUser.pda1 = 0;//aStaff->iTakeDevice->iDevID;
			//aUser.pda2 = aStaff->iTakeDevice->iDevID;
			aUser.PowerGroup = aStaff->iPermissionGroup;
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
			char *p1;
			CString cTEMP = aStaff->iID;
			p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
			memcpy(&eUser.Oper,p1,cTEMP.GetLength());
			cTEMP.ReleaseBuffer();

			cTEMP = aStaff->iName;
			p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
			memcpy(&eUser.name,p1,cTEMP.GetLength());
			cTEMP.ReleaseBuffer();

			eUser.orgid = aStaff->iOrgID;
			eUser.pda1 = 0;
            if(aStaff->iTakeDevice)
                eUser.pda1 = aStaff->iTakeDevice->iDevID;
			eUser.PowerGroup = aStaff->iPermissionGroup;
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
			char *p1 ;
			
			CString cTEMP = aStaff->iID;
			p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
			memcpy(&dUser.Oper,p1,cTEMP.GetLength());
			cTEMP.ReleaseBuffer();

			iResult = cd->DelUsers(&dUser);
			//aStaff->iID.ReleaseBuffer();
			break;
		}
	}
	return iResult;
}

int SetStaffPassword(int aOrgID, CString aStaffID, CString aPassword)
{
    ///////////////////////////////////////////////////
#ifdef TESTCODE
    return KErrNone;
#endif
    ///////////////////////////////////////////////////
}


//获取排版下 实际线路ID 
int GetCalendarSchedule(int aOrgID, const vector<StaffInfo*>* ListStaff, CalendarSchedule* aSchedule)
{
    ///////////////////////////////////////////////////
#ifdef TESTCODE
    aSchedule->iCaledarID = 1;
    aSchedule->iOrgID = aOrgID;
    aSchedule->iStartDay = 1288915200;
    aSchedule->iPeriods = 3;
	return KErrNone;
#endif
    ///////////////////////////////////////////////////
	cData *cd = new cData();
	OrgPxInfo oInfo ;
	oInfo.Orgid = aOrgID;
	GetOrgPxInfo llist ;
	memset(&llist,0,sizeof(GetOrgPxInfo));
	cd->GetOrgpInfo(oInfo,&llist);
	aSchedule->iCaledarID = llist.id ;
	aSchedule->iOrgID = aOrgID;
	aSchedule->iPeriods = llist.cyc;
	aSchedule->iScheduleRemark = llist.content;
	aSchedule->iStartDay = Time2Strings2(llist.stime);
	rOrgPB pb;
	pb.orgid = aOrgID;
	lUser lPoint;
	if(cd->rGetOPb(pb,&lPoint) > 0)
	{
		for(iterUser iter = lPoint.begin() ; iter != lPoint.end() ; iter++)
		{
			StaffInfo* staff;// = new StaffInfo;
			BOOL find = FALSE;
			for (size_t i=0; i<ListStaff->size(); i++)
			{
				staff = (*ListStaff)[i];
				if (staff->iID.Compare(iter->Oper) == 0)
				{
					find = TRUE;
					break;
				}
			}
			if (find)
			{
				aSchedule->iScheduleStaff.push_back(staff);
			}
		}
		return ResultOk;
	}

	//int cData::rGetOPb(const rOrgPB sValue,lUser *lPoint)
	delete cd;

    return KErrNone;
}
//
int SetCalendarSchedule(int aOrgID, const CalendarSchedule* aSchedule/*, const <StaffInfo*>*ListStaff*/)
{
    ///////////////////////////////////////////////////
#ifdef TESTCODE
	 return KErrNone;
#endif
    ///////////////////////////////////////////////////
	cData *cd = new cData();
	pb *p = new pb();
	memset(p,0,sizeof(pb));
	p->cyc = aSchedule->iPeriods;
	p->id = aSchedule->iCaledarID;
	p->orgid = aSchedule->iOrgID;
	p->type = 1;

	char *p2 ;
	CString cTEMP1 = aSchedule->iScheduleRemark;
	p2 = (char*)cTEMP1.GetBuffer(cTEMP1.GetLength());
	memcpy(p->content,p2,cTEMP1.GetLength());
	cTEMP1.ReleaseBuffer();
	cd->SetpxPb(*p);

	int iOrder = 1;

	for(vector<StaffInfo*>::iterator iter = ((CalendarSchedule*)aSchedule)->iScheduleStaff.begin();iter != aSchedule->iScheduleStaff.end();iter++)
	{
		pbpp pp;
		memset(&pp,0,sizeof(pbpp));
		pp.ppid = aSchedule->iCaledarID;
		pp.px = iOrder;
		char *p1 ;
		CString cTEMP = (*iter)->iID;
		p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
		memcpy(pp.ryid,p1,cTEMP.GetLength());
		cTEMP.ReleaseBuffer();


		cd->SetpxOrder(pp);
		(*iter)->iID.ReleaseBuffer();
		iOrder++;
	}

	delete cd;
    return KErrNone;
}


int GetEmergencyTask( int aOrgID, vector<EmergencyTaskInfo*>* m_EmergencyList )
{
	ljjListresult lPoint;
	cData *cs = new cData();
	cs->GetJJRWList(aOrgID,&lPoint);
	for(IterljjListresult iter = lPoint.begin();iter != lPoint.end(); iter++)
	{
		EmergencyTaskInfo  *eInfo = new EmergencyTaskInfo;
		eInfo->iBeginKm = (int)iter->spoint;
		eInfo->iEndKm = (int)iter->epoint;

		eInfo->iBeginTime = Time2Strings2(iter->stime);
		eInfo->iEndTime = Time2Strings2(iter->etime);

		eInfo->iStatus = (EmergencyStatus)iter->state;
		eInfo->iTaskID = iter->id;

		eInfo->iRailLine = iter->lineid;
		eInfo->iTaskName = iter->rwName;
		eInfo->iOrgId = aOrgID;
		ljjryListresult lPoint;
		cs->GetJJry(iter->id,&lPoint);
		for(IterlljjryListresult iter = lPoint.begin(); iter != lPoint.end();iter++)
		{
			eInfo->iStaffID.push_back(iter->Jj_Pid);
			eInfo->iStaffOrgID.push_back(iter->nJj_OrgID);
			eInfo->iStaffName.push_back(iter->strJj_Name);
		}
		m_EmergencyList->push_back(eInfo);
	}
	return KErrNone;
}

int SetEmergencyTask( int aOrgID, int aCmd, const EmergencyTaskInfo* aEmergencyTask )
{
   // aEmergencyTask->
    int iResult = 0;
	if( CMD_EMERGENCY_MODIFY== aCmd )//紧急任务休息
	{
		if(aEmergencyTask->iStatus == KFinished)
		{
			return 0;
		}
        cData *cd = new cData();
        jjrw *sVAL= new jjrw();
        memset(sVAL,0,sizeof(jjrw));
        sVAL->type = 1;
        CString timeTemp = Time2Strings(aEmergencyTask->iEndTime);
        char *p1 = (char*)timeTemp.GetBuffer(timeTemp.GetLength());
        memcpy(sVAL->endTime,p1,timeTemp.GetLength());
        timeTemp.ReleaseBuffer();

        CString timeTemp1 = Time2Strings(aEmergencyTask->iBeginTime);
        p1 = (char*)timeTemp1.GetBuffer(timeTemp1.GetLength());
        memcpy(sVAL->startTime,p1,timeTemp1.GetLength());
        timeTemp1.ReleaseBuffer();

        sVAL->endPointId = aEmergencyTask->iBeginKm;
        sVAL->lineid = (int)aEmergencyTask->iRailLine;

        sVAL->orgid = aOrgID;
        timeTemp = aEmergencyTask->iTaskName;
        p1 = (char*)timeTemp.GetBuffer(timeTemp.GetLength());
        memcpy(sVAL->rwName,p1,timeTemp.GetLength());
        timeTemp.ReleaseBuffer();

        sVAL->startPointId = aEmergencyTask->iBeginKm;
        sVAL->endPointId = aEmergencyTask->iEndKm;
        sVAL->state = 0;
        sVAL->iTaskId = aEmergencyTask->iTaskID ;
        iResult = cd->Setjjrw(*sVAL);
        delete cd;
	}

	if( CMD_EMERGENCY_ADD == aCmd )//紧急任务天加
	{
		if(aEmergencyTask->iStatus == KFinished)
		{
			return 0;
		}
        cData *cd = new cData();
        jjrw *sVAL= new jjrw();
        memset(sVAL,0,sizeof(jjrw));
        sVAL->type = 0;
        CString timeTemp = Time2Strings(aEmergencyTask->iEndTime);
        char *p1 = (char*)timeTemp.GetBuffer(timeTemp.GetLength());
        memcpy(sVAL->endTime,p1,timeTemp.GetLength());
        timeTemp.ReleaseBuffer();

        timeTemp = Time2Strings(aEmergencyTask->iBeginTime);
        p1 = (char*)timeTemp.GetBuffer(timeTemp.GetLength());
        memcpy(sVAL->startTime,p1,timeTemp.GetLength());
        timeTemp.ReleaseBuffer();

        sVAL->endPointId = aEmergencyTask->iBeginKm;
        sVAL->lineid = (int)aEmergencyTask->iRailLine;

        sVAL->orgid = aOrgID;
        timeTemp = aEmergencyTask->iTaskName;
        p1 = (char*)timeTemp.GetBuffer(timeTemp.GetLength());
        memcpy(sVAL->rwName,p1,timeTemp.GetLength());
        timeTemp.ReleaseBuffer();

        sVAL->startPointId = aEmergencyTask->iBeginKm;
        sVAL->endPointId = aEmergencyTask->iEndKm;
        sVAL->state = 0;
        iResult = cd->Setjjrw(*sVAL);
        delete cd;
	}


	if(CMD_EMERGENCY_MODIFYSTAFF == aCmd)
	{
        //GetXjRymx
        cData *cs = new cData();
        jjDel jVal;
        jVal.iTaskId = aEmergencyTask->iTaskID;
        cs->DelXjRymx(jVal);
        for(size_t i =0;i < aEmergencyTask->iStaffID.size(); i++)
        {
            jjry  sValue;
            memset(&sValue,0,sizeof(jjry));
            sValue.type = 0;
            sValue.Jj_id = aEmergencyTask->iTaskID;

            CString cTemp = aEmergencyTask->iStaffID[i];
            char *p1 = (char*)cTemp.GetBuffer(cTemp.GetLength());
            memcpy(&sValue.Jj_Pid,p1,cTemp.GetLength());
            cTemp.ReleaseBuffer();
            cs->Setjjry(sValue);

        }
		return 1;
	}
	return iResult;
}

int GetEmergencyLogs( int aTaskID, vector<EmergencyLogs*>* aEmergencyStaff )
{
    ///////////////////////////////////////////////////
#ifdef TESTCODE
    EmergencyLogs* logs = new EmergencyLogs;
    logs->iTaskID = aTaskID;
    logs->iStaffID = _T("123123");
    logs->iStaffOrgID = 1;
    logs->iTotalKM = 10.25;
    logs->iTotalTime = _T("10223");
    aEmergencyStaff->push_back(logs);
    return KErrNone;
#endif
    ///////////////////////////////////////////////////
		ljjryListresult lPoint;
	cData *cd = new cData();
	cd->GetJJry(aTaskID,&lPoint);
	for(IterlljjryListresult iter = lPoint.begin(); iter != lPoint.end();iter++)
	{
		EmergencyLogs *eNew = new EmergencyLogs();
		eNew->iStaffID = iter->Jj_Pid;
		eNew->iStaffOrgID = iter->nJj_OrgID;
		eNew->iTaskID = aTaskID;
		eNew->iTotalTime = iter->Jj_time;
		eNew->iTotalKM = iter->Jj_long;
		aEmergencyStaff->push_back(eNew);
	}
    return KErrNone;
}

int GetOrgDevice( int aOrgID, vector<DeviceInfo*>* aDeviceList )
{
    ///////////////////////////////////////////////////
#ifdef TESTCODE
    DeviceInfo* device = new DeviceInfo;
    device->iDeviceType = KPicture;
    device->iOrgID = aOrgID;
    device->iDevID = 1;
    device->iPhoneNum = _T("18602812345");
    aDeviceList->push_back(device);
    return KErrNone;
#endif
    ///////////////////////////////////////////////////
	cData *cd = new cData();
	//if(cd->iSocketState < 0)
	RequestDeviceList rRequest;
	rRequest.Devicetype = 2;
	rRequest.Orgid = aOrgID;
	rRequest.type = 1;
	lDevice lPoint ;
	if(cd->GetDeviceList(rRequest,&lPoint) < 0)
	{
		return KErrNone;
	}
	aDeviceList->clear();
	for(iterDevice iter = lPoint.begin() ; iter !=lPoint.end();iter++)
	{
		DeviceInfo *dInfo = new DeviceInfo;
		//memset(dInfo,0,sizeof(DeviceInfo));
		dInfo->iDeviceType = (DeviceType)iter->DeviceType; 
		dInfo->iOrgID = aOrgID;
		dInfo->iDevID = iter->DeviceId;

		//char *pTemp ;
		//pTemp = (LPSTR)(LPCTSTR)iter->DevicePid;
		//memcpy(&dInfo.iPhoneNum,pTemp,sizeof(pTemp));
		dInfo->iPhoneNum = iter->DevicePid;
		aDeviceList->push_back(dInfo);
	}
	return ResultOk;
}

int SetOrgDevice( int aOrgID, int aCmd, const DeviceInfo* aDeviceList )
{
    ///////////////////////////////////////////////////
#ifdef TESTCODE
    return KErrNone;
#endif
    ///////////////////////////////////////////////////
//#define CMD_DEVICE_ADD 0x40
//#define CMD_DEVICE_MODIFY 0x41
//#define CMD_DEVICE_DELETE 0x42
	int iResult;
	cData *cd = new cData();
	switch(aCmd)
	{
	case CMD_DEVICE_ADD:
		{
			AddDevice *aDevice = new AddDevice;
			memset(aDevice,0,sizeof(AddDevice));
			aDevice->DeviceOrg = aDeviceList->iOrgID;
			aDevice->DeviceType = aDeviceList->iDeviceType;

			CString cTEMP = aDeviceList->iPhoneNum;
			char *p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
			memcpy(&aDevice->DevicePid,p1,cTEMP.GetLength());
			cTEMP.ReleaseBuffer();

			//char *pTemp = (LPSTR)(LPCTSTR)aDeviceList->iPhoneNum;
			//memcpy(&aDevice->DevicePid,pTemp,sizeof(pTemp));
			iResult = cd->AddDevices(aDevice);
			break;
		}
	case CMD_DEVICE_MODIFY:
		{
			EditDevice *aDevice = new EditDevice;
			memset(aDevice,0,sizeof(EditDevice));
			aDevice->DeviceOrg = aDeviceList->iOrgID;
			aDevice->DeviceType = aDeviceList->iDeviceType;
			aDevice->DeviceId =aDeviceList->iDevID; 
			//char *pTemp = (LPSTR)(LPCTSTR)aDeviceList->iPhoneNum;
			//memcpy(&aDevice->DevicePid,pTemp,sizeof(pTemp));

			CString cTEMP = aDeviceList->iPhoneNum;
			char *p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
			memcpy(&aDevice->DevicePid,p1,cTEMP.GetLength());
			cTEMP.ReleaseBuffer();

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

int GetOrgMonthPx(GetOrgPxEx const sValue,vector<getorgpxlistresultEx*> *Value)
{
	cData *cd = new cData();
//int cData::GetOrgMonthPx(getorgpx const sValue,lOrgMonth *llist)
//{
	getorgpx sValues;
	lOrgMonth llist;

	sValues.months = sValue.months;
	sValues.orgid = sValue.orgid;
	sValues.yesrs = sValue.yesrs;
	if(cd->GetOrgMonthPx(sValues,&llist) == 1)
	{
		for (iterOrgMonth iter = llist.begin() ; iter != llist.end();iter++)
		{
            getorgpxlistresultEx *ex = new getorgpxlistresultEx();
            ex->dates = iter->dates;
            ex->xj = iter->xj;
            ex->sGetPoint = iter->sGetPoint;
            ex->sTotlePoint =  iter->sTotlePoint;
            ex->cPoint =  iter->cPoint;
            ex->lPoint =  iter->lPoint;
            ex->userid =  iter->userid;
            Value->push_back(ex);
		}
	}
	return 0;
}

int GetPictureInfo( int aOrgID, time_t aStartDate, time_t aEndDate, vector<PictureInfo*>* aPictureList )
{
    ///////////////////////////////////////////////////
#ifdef TESTCODE
    PictureInfo *pInfo = new PictureInfo;
    //pInfo.iPicID = iter->
    pInfo->iPicName = _T("516823.jpg");
    pInfo->iShootingTime = _T("2011-7-1");
    pInfo->iErrorType =	1;
    aPictureList->push_back(pInfo);
    return KErrNone;
#endif
    ///////////////////////////////////////////////////
	CString sStart = Time2Strings(aStartDate);//_T("2011-1-1");//
	CString eTimes = Time2Strings(aEndDate);//_T("2011-11-1");//
	GetPic pc;
	memset(&pc,0,sizeof(GetPic));
	pc.Orgid = aOrgID;

	CString cTEMP = sStart;
	char *p1 = (char*)cTEMP.GetBuffer(cTEMP.GetLength());
	memcpy(&pc.stime,p1,cTEMP.GetLength());
	cTEMP.ReleaseBuffer();

	CString cTEMP1 = eTimes;
	char *p2 = (char*)cTEMP1.GetBuffer(cTEMP1.GetLength());
	memcpy(&pc.etime,p2,cTEMP1.GetLength());
	cTEMP1.ReleaseBuffer();

	lPicList llist ;
	cData *cd = new cData();
	if(cd->GetOrgPic(pc,&llist) != 1)
	{
		return 1;
	}
	for(iterPic iter = llist.begin();iter != llist.end() ;iter++)
	{
		PictureInfo *pInfo = new PictureInfo;
		//memset(pInfo,0,sizeof(PictureInfo));
		//pInfo.iPicID = iter->
		pInfo->iPicName = iter->name;
		pInfo->iShootingTime = iter->time;
		pInfo->iErrorType =	iter->itype;
		aPictureList->push_back(pInfo);
	}
	return 0;
}

int GetOrgXl()
{
	//int cData::GetXjRymx(const ryxj1 value,lryxj1result *lPoint)
	ryxj1 user;
	memset(&user,0,sizeof(UserGps));
	strcpy_s(user.oper,_T("1234"));
	strcpy_s(user.date,_T("2011-7-1"));
	cData *cd = new cData();
	lryxj1result lPoint;
	cd->GetXjRymx(user,&lPoint);
	return 1;
}
//
int GetOrgMonthxl()
{
	//int PGPSDayData(const UserGps value,lOrgLineResult *lPoint);
	UserGps user;
	memset(&user,0,sizeof(UserGps));
	strcpy_s(user.oper,_T("admin"));
	strcpy_s(user.time,_T("2011-7-1"));
	cData *cd = new cData();
	lOrgLineResult lPoint;
	cd->PGPSDayData(user,&lPoint);;
	return 1;
}

int SavePictureToDirect( int aOrgID, const PictureInfo* aPicture, CString aToDirect )
{
    Getrealpic cs;
    memset(&cs,0,sizeof(Getrealpic));
 
    CString cTEMP1 = aPicture->iPicName;
    char *p2 = (char*)cTEMP1.GetBuffer(cTEMP1.GetLength());
    memcpy(&cs.pname,p2,cTEMP1.GetLength());
    cTEMP1.ReleaseBuffer();

	cData *cd = new cData();
	if(cd->getPic((const Getrealpic)cs,aToDirect))
		return ResultOk;
	else
		return KErrNone;

}

int GetStaffScheduleTrack(CString aStaffID, time_t aDate, RecordStaff* aRecord )
{//流水
    /////////////////////////////////////////////////////
#ifdef TESTCODE
    aRecord->iStaffID = aStaffID;
    aRecord->iArrivedTime.push_back(_T("9:10"));
    aRecord->iRecordLon.push_back(104.064631);
    aRecord->iRecordLat.push_back(30.698965);
    //
    aRecord->iArrivedTime.push_back(_T("9:15"));
    aRecord->iRecordLon.push_back(104.075630);
    aRecord->iRecordLat.push_back(30.699584);
    //
    aRecord->iArrivedTime.push_back(_T("9:28"));
    aRecord->iRecordLon.push_back(104.086426);
    aRecord->iRecordLat.push_back(30.699584);
    return KErrNone;
#endif
    /////////////////////////////////////////////////////

    aRecord->iStaffID = aStaffID;
	//int PGPSDayData(const UserGps value,lOrgLineResult *lPoint);
	char UserName[20];
	memset(UserName,0,20);
	UserGps user;
	memset(&user,0,sizeof(UserGps));
    CString cTEMP1 = aStaffID;
    char *p2 = (char*)cTEMP1.GetBuffer(cTEMP1.GetLength());
    memcpy(UserName,p2,cTEMP1.GetLength());
    cTEMP1.ReleaseBuffer();
	memcpy(user.oper,UserName,20);

	CString dates = Time2Strings(aDate);
    char *p3 = (char*)dates.GetBuffer(dates.GetLength());
    memcpy(user.time,p3,dates.GetLength());
	dates.ReleaseBuffer();

	cData *cd = new cData();
	lOrgLineResult lPoint;
	cd->PGPSDayData(user,&lPoint);
////OrgLineResults;
//typedef list <OrgLineResults> lOrgLineResult;
//typedef lOrgLineResult::iterator IterOrgLineResult;
	aRecord->iStaffID = aStaffID;
	for(IterOrgLineResult iter = lPoint.begin() ;iter !=lPoint.end(); iter++ )
	{
		aRecord->iRecordLat.push_back(iter->wdu) ;
		aRecord->iRecordLon.push_back(iter->jdu);
		aRecord->iArrivedTime.push_back(iter->time);
	}
    return KErrNone;
}

int GetReportDetail(CString aStaffID, CString aTime, ReportDetail* aReportList)
{//明细
    /////////////////////////////////////////////////////
#ifdef TESTCODE
    aReportList->iDay = aTime;
    aReportList->iPlanArrivedTime.push_back(_T("9:00"));
    aReportList->iActualArrivedTime.push_back(_T("9:05"));
    aReportList->iState.push_back(KPointNormal);
    aReportList->iPlanArrivedTime.push_back(_T("9:10"));
    aReportList->iActualArrivedTime.push_back(_T("9:11"));
    aReportList->iState.push_back(KPointUnArrived);
    return KErrNone;
#endif
    /////////////////////////////////////////////////////
	ryxj1 user;
	memset(&user,0,sizeof(ryxj1));

    CString cTEMP1 = aStaffID;
    char *p2 = (char*)cTEMP1.GetBuffer(cTEMP1.GetLength());
    memcpy(user.oper,p2,cTEMP1.GetLength());
    cTEMP1.ReleaseBuffer();

	CString dates = aTime;
    char *p3 = (char*)dates.GetBuffer(dates.GetLength());
	memcpy(user.date,p3,dates.GetLength());

	cData *cd = new cData();
	lryxj1result lPoint;
	cd->GetXjRymx(user,&lPoint);
	for(Iterryxj1result iter = lPoint.begin();iter != lPoint.end();iter++)
	{
		aReportList->iActualArrivedTime.push_back(iter->realTime);
		aReportList->iPlanArrivedTime.push_back(iter->sArrtime);
		aReportList->iDay = iter->date;
		aReportList->iState.push_back((PointState)iter->pointstate);
	}

	return KErrNone;
}

int GetReportInfoList( int aOrgID, int aYear, int aMonth, vector<ReportInfo*>* aReportList )
{//月报表
    /////////////////////////////////////////////////////
#ifdef TESTCODE
    ReportInfo* report = new ReportInfo;
    report->iOrgID = aOrgID;
    report->iStaffName = _T("Admin");
    report->iReportDay = _T("2011-1-1");
    report->iPlanArrived = 50;
    report->iActualArrived = 40;
    report->iAbnormal = 10;
    aReportList->push_back(report);

    report = new ReportInfo;
    report->iOrgID = aOrgID;
    report->iStaffName = _T("admin2");
    report->iReportDay = _T("2011-1-1");
    report->iPlanArrived = 60;
    report->iActualArrived = 40;
    report->iAbnormal = 20;
    aReportList->push_back(report);

    return KErrNone;
#endif
    /////////////////////////////////////////////////////
	cData *cd = new cData();
	getorgpx sValues;
	lOrgMonth llist;

	sValues.months = aMonth;
	sValues.orgid = aOrgID;
	sValues.yesrs = aYear;
	if(cd->GetOrgMonthPx(sValues,&llist) == 1)
	{
		for (iterOrgMonth iter = llist.begin() ; iter != llist.end();iter++)
		{
		ReportInfo* report = new ReportInfo;
		report->iOrgID = aOrgID;
		report->iStaffName = iter->userid;
		report->iReportDay = iter->dates;
		report->iWeekDay = iter->xj;
		report->iPlanArrived = iter->sTotlePoint;
		report->iActualArrived = iter->sGetPoint;
		report->iAbnormal = iter->cPoint;
		report->iUnArrived = iter->lPoint;
		aReportList->push_back(report);

		}
	}	

    return KErrNone;
}

time_t Time2Strings1(CString sec)
{
	int iHour,iMonth;
	sscanf_s((const char*)sec.GetBuffer(),"%d:%d",&iHour,&iMonth);
	CTime time(2010,1,1,iHour,iMonth,0); 
	time_t time2=time.GetTime();
	sec.ReleaseBuffer();
	return time2;
}
time_t Time2Strings2(CString sec)
{
	if(sec == "")
		return 0;
	int i1,i2,i3;
	sscanf_s((const char*)sec.GetBuffer(),"%d-%d-%d",&i1,&i2,&i3);
	CTime time(i1,i2,i3,0,0,1); 
	time_t time2=time.GetTime();
	sec.ReleaseBuffer();
	return time2;
}

CString Time2Strings(time_t sec)
{
	if(sec==0){
		return _T("");
	}

	time_t tm = sec;

	CTime t(tm);
	int year = t.GetYear();
	int month = t.GetMonth();
	int day = t.GetDay();
	int hour = t.GetHour();
	int minute = t.GetMinute();
	int second = t.GetSecond();

	CString strYear;
	CString strMonth;
	CString strDay;
	CString strHour;
	CString strMinute;
	CString strSecond;

	int year2 = year%100;
	//strYear.Format("%d",year2);
	strYear.Format(_T("%d"),year);
	if((year2>=0)&&(year2<=9)){
		strYear = _T("0") + strYear;
	}

	strMonth.Format(_T("%d"),month);

	strDay.Format(_T("%d"),day);


	CString result = strYear+"-"+strMonth+"-"+strDay;



	return result;

}

void StringToChar(char *p,CString sVal)
{
	p =(char*)sVal.GetBuffer(sVal.GetLength());
	return;
}

void StringRelease(CString sVAL);
