#include "StdAfx.h"
#include "DataService.h"
#include "ErrorDefine.h"
#include "CmdDefine.h"
#include "cData.h"


int VerifyLogin( CString& aLoginAccount, CString& aLoginPassword, int* orgID, Permission *pPower)
{
    ///////////////////////////////////////////////////
    *orgID = 1;
    pPower->iBasical = 1;
    pPower->iOperate = 1;
    pPower->iReportForm = 1;
    return KErrNone;
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
	}
    aLoginAccount.ReleaseBuffer();
    aLoginPassword.ReleaseBuffer();
	return iResult;
}



int GetOrgTree(const int& OrgId, vector<OrganizationInfo*>* a_OrgTree)
{
    ///////////////////////////////////////////////////
    OrganizationInfo* org = new OrganizationInfo;
    org->iOrgName = _T("Admin");
    org->iParentOrg = NULL;
    org->iParentID = 0;
    org->iOrgID = 1;
    org->iBoundaryRail = Chengdu_Kunming;
    org->iChildID.push_back(2);
    org->iChildID.push_back(3);
    a_OrgTree->push_back(org);

    org = new OrganizationInfo;
    org->iOrgName = _T("Child1");
    org->iParentOrg = (*a_OrgTree)[0];
    org->iParentID = org->iParentOrg->iOrgID;
    (*a_OrgTree)[0]->iChildOrg.push_back(org);
    org->iOrgID = 2;
    org->iBoundaryRail = Chengdu_Chongqing;
    org->iChildID.push_back(4);
    a_OrgTree->push_back(org);

    org = new OrganizationInfo;
    org->iOrgName = _T("Child2");
    org->iParentOrg = (*a_OrgTree)[0];
    org->iParentID = org->iParentOrg->iOrgID;
    (*a_OrgTree)[0]->iChildOrg.push_back(org);
    org->iBoundaryRail = Baoji_Chengdu;
    org->iOrgID = 3;
    org->iChildID.push_back(5);
    a_OrgTree->push_back(org);
    return KErrNone;
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
		org->iBoundaryRail      = (RailLine)iter->LineId ;
		org->iBoundaryStartKM = iter->startid ;
		a_OrgTree->push_back(org);
	}

    //for (int i=0; i<a_OrgTree->size(); i++)
    //{
    //    CherkOrgType(a_OrgTree,a_OrgTree[i]);
    //}

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
		//char *pTemp = (LPSTR)(LPCTSTR)aOrganization->iOrgAddress;
		//memcpy(&aOrg.Address,pTemp,sizeof(pTemp));
		//aOrg.Address = aOrganization->iOrgAddress.GetBuffer();
		////pTemp = (LPSTR)(LPCTSTR)aOrganization->iOrgName;
		////memcpy(&aOrg.OrgName,pTemp,sizeof(pTemp));
		//aOrg.OrgName = aOrganization->iOrgName.GetBuffer();
		iResult = cd->AddOrgs(&aOrg);
		//aOrganization->iOrgAddress.ReleaseBuffer();
		//aOrganization->iOrgName.ReleaseBuffer();
		//aOrganization->iOrgAddress.ReleaseBuffer();
		cTEMP.ReleaseBuffer();
		break;
		}
	case CMD_ORG_MODIFY:
		{
			EditOrg eOrg;
			memset(&eOrg,0,sizeof(EditOrg));
			//char *pTemp = (LPSTR)(LPCTSTR)aOrganization->iOrgAddress;
			//memcpy(&eOrg.Address,pTemp,sizeof(pTemp));
			//eOrg.Address = aOrganization->iOrgAddress.GetBuffer();
			////pTemp = (LPSTR)(LPCTSTR)aOrganization->iOrgName;
			////memcpy(&eOrg.OrgName,pTemp,sizeof(pTemp));
			//eOrg.OrgName = aOrganization->iOrgName.GetBuffer();
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
			
			//aOrganization->iOrgName.ReleaseBuffer();
			//aOrganization->iOrgAddress.ReleaseBuffer();
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
int GetOrgPoint(int aOrgID, vector<MapPoint*>* aPointList)
{
    ///////////////////////////////////////////////////
    //MapPoint *pt = new MapPoint;
    //pt->iRailLine = Chengdu_Chongqing; 
    //pt->iKM = 251;
    //pt->iLon = 104.064531;
    //pt->iLat = 30.699965;
    //pt->iDirect = KUpLine;
    //aPointList->push_back(pt);

    //pt = new MapPoint;
    //pt->iRailLine = Chengdu_Chongqing; 
    //pt->iKM = 252;
    //pt->iLon = 104.075530;
    //pt->iLat = 30.699484;
    //pt->iDirect = KUpLine;
    //aPointList->push_back(pt);

    //pt = new MapPoint;
    //pt->iRailLine = Chengdu_Chongqing; 
    //pt->iKM = 253;
    //pt->iLon = 104.086526;
    //pt->iLat = 30.699484;
    //pt->iDirect = KUpLine;
    //aPointList->push_back(pt);

    //pt = new MapPoint;
    //pt->iRailLine = Chengdu_Chongqing; 
    //pt->iKM = 254;
    //pt->iLon = 104.097521;
    //pt->iLat = 30.699484;
    //pt->iDirect = KUpLine;
    //aPointList->push_back(pt);
    //return KErrNone;
    ///////////////////////////////////////////////////

	cData *cd = new cData();
	lOrgLine llist;
	if(cd->GetOrgPoint(aOrgID,&llist) > 0)
	{
		for(iterOrgLine iter = llist.begin() ; iter != llist.end() ; iter++)
		{
			MapPoint *pt = new MapPoint;
			pt->iRailLine = (RailLine)iter->lineid;
			pt->iKM = iter->gls;
			pt->iLat = iter->jdu;
			pt->iLon = iter->wdu;
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
    //return KErrNone;
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
			pPoint.iRailLine = aPoint->iRailLine;
			pPoint.itype = 0;//tianjia
			iResult=cd->setPoint(pPoint);
			break;
		}
    case CMD_POINT_MODIFY:
		{
			pPoint.iDirect = (int)aPoint->iDirect;
			pPoint.iKM = aPoint->iKM;
			pPoint.iLat = aPoint->iLat;
			pPoint.iLon = aPoint->iLon;
			pPoint.iRailLine = aPoint->iRailLine;
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
	if(iResult==1)
	{
		return KErrNone;
	}else
		return ResultOk;
}

int GetOrgLine(int aOrgID, const vector<MapPoint*>& aPointList, vector<LineInfo*>* aLineList)
{
    ///////////////////////////////////////////////////
    //LineInfo *line = new LineInfo;
    //line->iLineID = 1;
    //line->iLineName = _T("成局1段");
    //line->iStartKm = (aPointList)[0]->iKM;
    //line->iStartNo = KFirstDay;
    //line->iLineKmLonLat.push_back(aPointList[0]);
    //line->iLineKmTime.push_back(100);
    //line->iLineKmLonLat.push_back(aPointList[1]);
    //line->iLineKmTime.push_back(100);
    //line->iLineKmLonLat.push_back(aPointList[2]);
    //line->iLineKmTime.push_back(100);
    //line->iLineKmLonLat.push_back(aPointList[3]);
    //line->iLineKmTime.push_back(100);
    //aLineList->push_back(line);

    //line = new LineInfo;
    //line->iLineID = 2;
    //line->iLineName = _T("成局2段");
    //line->iStartKm = aPointList[1]->iKM;
    //line->iStartNo = KThirdDay;
    //line->iLineKmLonLat.push_back(aPointList[1]);
    //line->iLineKmTime.push_back(0);
    //line->iLineKmLonLat.push_back(aPointList[3]);
    //line->iLineKmTime.push_back(0);
    //aLineList->push_back(line);
    //return KErrNone;
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

int GetOrgStaff(int aOrgID, vector<StaffInfo*>* aStaffList)
{

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
			staff->Pda1 = iter->pda1;
			staff->Pda2 = iter->pda2;
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
    //return KErrNone;
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
			aUser.pda1 = aStaff->Pda1 ;
			aUser.pda2 = aStaff->Pda2;
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
			/*		char *pTemp = (LPSTR)(LPCTSTR)aStaff->iName;
			memcpy(&eUser.name,pTemp,sizeof(pTemp));

			pTemp = (LPSTR)(LPCTSTR)aStaff->iID;
			memcpy(&eUser.Oper,pTemp,sizeof(pTemp));*/
			//eUser.name = aStaff->iName.GetBuffer();
			//eUser.Oper = aStaff->iID.GetBuffer();
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
			eUser.pda1 = aStaff->Pda1 ;
			eUser.pda2 = aStaff->Pda2;
			eUser.PowerGroup = aStaff->iPermissionGroup;
			int iLogin = 1;
			if(aStaff->iLoginPermission)iLogin = 1;
			else iLogin = 0;
			eUser.UserState = iLogin;
			iResult = cd->EditUsers(&eUser);
			//aStaff->iID.ReleaseBuffer();
			//aStaff->iName.ReleaseBuffer();
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

			//pTemp = (LPSTR)(LPCTSTR)aStaff->iID;
			//memcpy(&dUser.Oper,pTemp,sizeof(pTemp));
			//dUser.Oper = aStaff->iID.GetBuffer();
			iResult = cd->DelUsers(&dUser);
			//aStaff->iID.ReleaseBuffer();
			break;
		}
	}
	return iResult;
}
//获取排版下 实际线路ID 
int GetCalendarSchedule(int aOrgID, const vector<StaffInfo*>* ListStaff, CalendarSchedule* aSchedule)
{
    ///////////////////////////////////////////////////
 //   aSchedule->iCaledarID = 1;
 //   aSchedule->iOrgID = aOrgID;
 //   aSchedule->iStartDay = 1288915200;
 //   aSchedule->iPeriods = 3;
	//return KErrNone;
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
	 //return KErrNone;
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
    return KErrNone;
}

int SetEmergencyTask( int aOrgID, int aCmd, const EmergencyTaskInfo* aEmergencyTask )
{
   // aEmergencyTask->
	return KErrNone;
}

int GetOrgDevice( int aOrgID, vector<DeviceInfo*>* aDeviceList )
{
    ///////////////////////////////////////////////////
    //return KErrNone;
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
    //return KErrNone;
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
    ///////////////////////////////////////////////////
    //return KErrNone;
    ///////////////////////////////////////////////////
	CString sStart = _T("2011-1-1");//Time2Strings(aStartDate);
	CString eTimes = _T("2011-11-1");//Time2Strings(aEndDate);
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
		pInfo->iErrorType = iter->itype ;
		//pInfo.iPicID = iter->
		pInfo->iPicName = iter->name;
		pInfo->iShootingTime = iter->time;
		aPictureList->push_back(pInfo);
	}
	return 0;
}

int SavePictureToDirect( int aOrgID, const PictureInfo* aPicture, CString aToDirect )
{
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
	CTime time(i1,i2,i3,0,0,0); 
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
