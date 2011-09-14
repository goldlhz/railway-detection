#include "StdAfx.h"
#include <string>
#include "AccessBaseData.h"
using namespace std;

CAccessBaseData::CAccessBaseData(void)
{
	m_pDatabase = NULL;
}

CAccessBaseData::~CAccessBaseData(void)
{
}

bool CAccessBaseData::InitAccesser(CADODatabase* pDatabase)
{
	ASSERT(pDatabase);
	if(pDatabase)
	{
		this->m_pDatabase = pDatabase;
		return true;
	}
	return false;
}

bool CAccessBaseData::UploadGPSPack(const GPS_UpLoad_Pack& gpsUpPack, 
	 GPS_DownLoad_Pack& gpsDownPack)
{
	if(m_pDatabase)
	{
		bool bErrorCode = false;
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into t_gps(G_time, G_j, G_w, G_tel) values('%s', %.7f, %.7f, '%s')",
			gpsUpPack.gDataBodyPack.strTime.c_str(), 
			gpsUpPack.gDataBodyPack.nLongitude, 
			gpsUpPack.gDataBodyPack.nLatitude,
			gpsUpPack.gDataBodyPack.strTerminalNumber.c_str());

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				bErrorCode = false;
				goto ErrorDeal;
			}
		}

		bErrorCode = true;
		if(!m_pDatabase->Execute(m_strBuffer))
		{
			bErrorCode = false;
			goto ErrorDeal;
		}

ErrorDeal:
		gpsDownPack.nBeginIdentify = gpsUpPack.nBeginIdentify;
		gpsDownPack.nMsgNumber = gpsUpPack.nMsgNumber;

		gpsDownPack.gDataBodyPack.nTerminalIdentifyLength = gpsUpPack.gDataBodyPack.nTerminalIdentifyLength;
		gpsDownPack.gDataBodyPack.strTerminalNumber = gpsUpPack.gDataBodyPack.strTerminalNumber;
		gpsDownPack.gDataBodyPack.strTime = gpsUpPack.gDataBodyPack.strTime;

		gpsDownPack.nBodyLength = 8 + gpsUpPack.gDataBodyPack.nTerminalIdentifyLength;
		return bErrorCode;
	}
	
	return false;
}

bool CAccessBaseData::UpLoadGPSPICPack(const GPSPIC_Pack& gpsPicUpLoadPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into t_pic(pname, tel, itype, dtime) values('%s', '%s', %d, '%s')",
			gpsPicUpLoadPack.strPicName.c_str(), 
			gpsPicUpLoadPack.strTel.c_str(), 
			gpsPicUpLoadPack.nType,
			gpsPicUpLoadPack.strTime.c_str());

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return false;
			}
		}

		if(m_pDatabase->Execute(m_strBuffer))
		{
			return true;
		}
	}
	return false;
}

bool CAccessBaseData::UpLoadGPSVEDIOPack(const Vedio_Pack& gpsVedioUpLoadPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into t_vedio(V_name, V_Telno, V_timer, V_type) values('%s', '%s', '%s', %d)",
			gpsVedioUpLoadPack.strName .c_str(), 
			gpsVedioUpLoadPack.strTel.c_str(), 
			gpsVedioUpLoadPack.strTime.c_str(),
			gpsVedioUpLoadPack.nType);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return false;
			}
		}

		if(m_pDatabase->Execute(m_strBuffer))
		{
			return true;
		}
	}
	return false;
}

bool CAccessBaseData::UploadLoginPack(const Login_UpLoad_Pack& loginUpPack, 
	Login_DownLoad_Pack& loginDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);

		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from t_user where User_State = 1 and User_Oper = '%s' and User_Pawd = '%s'",
			loginUpPack.gDataBodyPack.strName.c_str(),
			loginUpPack.gDataBodyPack.strPassword.c_str());

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(m_adoRecordSet.Open(m_pDatabase->GetActiveConnection(), m_strBuffer))
		{
			if(m_adoRecordSet.GetRecordCount() <= 0)
			{
				goto ErrorDeal;
			}

			//int nUserID = 0;
			int nOrgID = 0;
			int nPowerGroup = 0;

			//m_adoRecordSet.GetFieldValue("User_Id", nUserID);
			m_adoRecordSet.GetFieldValue("User_Org", nOrgID);
			m_adoRecordSet.GetFieldValue("User_PGroup", nPowerGroup);

			loginDownPack.gDataBodyPack.nOrgID = nOrgID;
			loginDownPack.gDataBodyPack.nUser_PGroup = nPowerGroup;
			loginDownPack.gDataBodyPack.nPower2 = 0;
			loginDownPack.gDataBodyPack.nPower3 = 0;

			nErrorCode = 0;
			//memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
			//sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from T_PowerGroup where Power_Id=%d", nPowerGroup);

			//if(m_adoRecordSet.Open(m_pDatabase->GetActiveConnection(), m_strBuffer))
			//{
			//	if(m_adoRecordSet.GetRecordCount() <= 0)
			//	{
			//		goto ErrorDeal;
			//	}

			//	//m_adoRecordSet.GetFieldValue("Group1", (unsigned long&)loginDownPack.gDataBodyPack.nPower1);

			//	m_adoRecordSet.GetFieldValue("Group2", (unsigned long&)loginDownPack.gDataBodyPack.nPower2);
			//	m_adoRecordSet.GetFieldValue("Group3", (unsigned long&)loginDownPack.gDataBodyPack.nPower3);

			//	string strTime = GetCurrentTimeByFormat();

			//	memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);

			//	sprintf_s(m_strBuffer, INPUTSQLBUFFERS, 
			//		"update T_User set User_LoginTimes = User_LoginTimes + 1, \
			//		User_LastLoginTimes = '%s' where User_Id = %d",
			//		strTime.c_str(),
			//		nUserID);

			//	if(!m_pDatabase->Execute(m_strBuffer))
			//	{
			//		goto ErrorDeal;
			//	}

			//	nErrorCode = 0;
			//}
		}

ErrorDeal:
		loginDownPack.nBeginIdentify = loginUpPack.nBeginIdentify;
		loginDownPack.nMsgNumber = loginUpPack.nMsgNumber;

		loginDownPack.gDataBodyPack.nResult = nErrorCode;

		loginDownPack.nBodyLength = 20;
		return true;		
	}
	return false;
}

bool CAccessBaseData::UploadAddOrgPack(const AddOrg_UpLoad_Pack& addOrgUpPack, 
	AddOrg_DownLoad_Pack& addOrgDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert t_org(org_name, org_upid, org_add, org_lev, org_lineid, org_startid, org_endid)	values('%s', %d, '%s', %d, %d, %d, %d)",
			addOrgUpPack.gDataBodyPack.strOrgName.c_str(),
			addOrgUpPack.gDataBodyPack.nUpOrg,
			addOrgUpPack.gDataBodyPack.strAddress.c_str(),
			addOrgUpPack.gDataBodyPack.nLevel,
			addOrgUpPack.gDataBodyPack.nLineId,
			addOrgUpPack.gDataBodyPack.nStartid,
			addOrgUpPack.gDataBodyPack.nEndID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			goto ErrorDeal;
		}

		nErrorCode = 0;
ErrorDeal:
		addOrgDownPack.nBeginIdentify = addOrgUpPack.nBeginIdentify;
		addOrgDownPack.nMsgNumber = addOrgUpPack.nMsgNumber;

		addOrgDownPack.gDataBodyPack.nResult = nErrorCode;

		addOrgDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UploadModifyOrgPack(const ModifyOrg_UpLoad_Pack& modifyOrgUpPack, 
	ModifyOrg_DownLoad_Pack& modifyOrgDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);

		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from T_Org where Org_Id = %d",
			modifyOrgUpPack.gDataBodyPack.nOrgID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(m_adoRecordSet.Open(m_pDatabase->GetActiveConnection(), m_strBuffer))
		{
			if(m_adoRecordSet.GetRecordCount() <= 0)
			{
				goto ErrorDeal;
			}
		}

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);

		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update T_Org set Org_name = '%s', Org_Add = '%s', org_lineid = %d, org_startid = %d, org_endid = %d where Org_Id = %d",
			modifyOrgUpPack.gDataBodyPack.strOrgName.c_str(),
			modifyOrgUpPack.gDataBodyPack.strAddress.c_str(),
			modifyOrgUpPack.gDataBodyPack.nLineId,
			modifyOrgUpPack.gDataBodyPack.nStartid,
			modifyOrgUpPack.gDataBodyPack.nEndID,
			modifyOrgUpPack.gDataBodyPack.nOrgID);

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			goto ErrorDeal;
		}

		nErrorCode = 0;
ErrorDeal:
		modifyOrgDownPack.nBeginIdentify = modifyOrgUpPack.nBeginIdentify;
		modifyOrgDownPack.nMsgNumber = modifyOrgUpPack.nMsgNumber;

		modifyOrgDownPack.gDataBodyPack.nResult = nErrorCode;

		modifyOrgDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UploadDeleteOrgPack(const DeleteOrg_UpLoad_Pack& deleteOrgUpPack, 
	DeleteOrg_DownLoad_Pack& deleteOrgDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 3;

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from t_org where Org_Upid=%d",
			deleteOrgUpPack.gDataBodyPack.nOrgID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(m_adoRecordSet.Open(m_pDatabase->GetActiveConnection(), m_strBuffer))
		{
			if(m_adoRecordSet.GetRecordCount() > 0)
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}
		}

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "delete t_org where Org_Id=%d",
			deleteOrgUpPack.gDataBodyPack.nOrgID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			goto ErrorDeal;
		}

		nErrorCode = 0;
ErrorDeal:
		deleteOrgDownPack.nBeginIdentify = deleteOrgUpPack.nBeginIdentify;
		deleteOrgDownPack.nMsgNumber = deleteOrgUpPack.nMsgNumber;

		deleteOrgDownPack.gDataBodyPack.nResult = nErrorCode;

		deleteOrgDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

CADORecordset* CAccessBaseData::UploadAskOrgListPack(const AskOrgList_UpLoad_Pack& askOrgListUpPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);

		if(askOrgListUpPack.gDataBodyPack.nOrgID == 0)
		{
			sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from t_org order by org_lev asc");
		}
		else
		{
			sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from t_org where dbo.cherkorg(%d, org_id) = 1 order by org_lev asc",
				askOrgListUpPack.gDataBodyPack.nOrgID);
		}

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}

bool CAccessBaseData::UploadAddWorkerPack(const AddWorker_UpLoad_Pack& addWorkerUpPack, 
	AddWorker_DownLoad_Pack& addWorkerDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into T_User(User_Oper, User_Pawd, User_Name, User_Org, \
			User_Tel, User_PGroup, User_Pda1, User_Pda2, User_State, User_LoginTimes) \
			values('%s', '%s', '%s', %d, '%s', %d, %d, %d, %d, 0)",
			addWorkerUpPack.gDataBodyPack.strOper.c_str(),
			addWorkerUpPack.gDataBodyPack.strPassword.c_str(),
			addWorkerUpPack.gDataBodyPack.strName.c_str(),
			addWorkerUpPack.gDataBodyPack.nOrgID,
			addWorkerUpPack.gDataBodyPack.strTel.c_str(),
			addWorkerUpPack.gDataBodyPack.nPowerGroup,
			addWorkerUpPack.gDataBodyPack.nPda1,
			addWorkerUpPack.gDataBodyPack.nPda2,
			addWorkerUpPack.gDataBodyPack.nWorkerState);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			goto ErrorDeal;
		}

		nErrorCode = 0;
ErrorDeal:
		addWorkerDownPack.nBeginIdentify = addWorkerUpPack.nBeginIdentify;
		addWorkerDownPack.nMsgNumber = addWorkerUpPack.nMsgNumber;

		addWorkerDownPack.gDataBodyPack.nResult = nErrorCode;

		addWorkerDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UploadModifyWorkerPack(const ModifyWorker_UpLoad_Pack& modifyWorkerUpPack, 
	ModifyWorker_DownLoad_Pack& modifyWorkerDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);

		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from T_User where User_Oper = '%s'",
			modifyWorkerUpPack.gDataBodyPack.strOper.c_str());

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(m_adoRecordSet.Open(m_pDatabase->GetActiveConnection(), m_strBuffer))
		{
			if(m_adoRecordSet.GetRecordCount() <= 0)
			{
				goto ErrorDeal;
			}
		}

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);

		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update T_User set User_Name = '%s', User_Org = '%d', \
			User_Tel = '%s', User_PGroup = %d, User_Pda1 = %d, User_Pda2 = %d, User_State = %d where User_Oper = '%s'",
			modifyWorkerUpPack.gDataBodyPack.strName.c_str(),
			modifyWorkerUpPack.gDataBodyPack.nOrgID,
			modifyWorkerUpPack.gDataBodyPack.strTel.c_str(),
			modifyWorkerUpPack.gDataBodyPack.nPowerGroup,
			modifyWorkerUpPack.gDataBodyPack.nPda1,
			modifyWorkerUpPack.gDataBodyPack.nPda2,
			modifyWorkerUpPack.gDataBodyPack.nWorkerState,
			modifyWorkerUpPack.gDataBodyPack.strOper.c_str());

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			goto ErrorDeal;
		}

		nErrorCode = 0;
ErrorDeal:
		modifyWorkerDownPack.nBeginIdentify = modifyWorkerUpPack.nBeginIdentify;
		modifyWorkerDownPack.nMsgNumber = modifyWorkerUpPack.nMsgNumber;

		modifyWorkerDownPack.gDataBodyPack.nResult = nErrorCode;

		modifyWorkerDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UploadDeleteWorkerPack(const DeleteWorker_UpLoad_Pack& deleteWorkerUpPack, 
	DeleteWorker_DownLoad_Pack& deleteWorkerDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "delete T_User where User_Oper = '%s'",
			deleteWorkerUpPack.gDataBodyPack.strOper.c_str());

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			goto ErrorDeal;
		}

		nErrorCode = 0;
ErrorDeal:
		deleteWorkerDownPack.nBeginIdentify = deleteWorkerUpPack.nBeginIdentify;
		deleteWorkerDownPack.nMsgNumber = deleteWorkerUpPack.nMsgNumber;

		deleteWorkerDownPack.gDataBodyPack.nResult = nErrorCode;

		deleteWorkerDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

CADORecordset* CAccessBaseData::UploadAskWorkerListPack(const AskWorkerList_UpLoad_Pack& askWorkerListUpPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		switch(askWorkerListUpPack.gDataBodyPack.nType)
		{
		case 1:
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from T_User where User_Org = %d",
					askWorkerListUpPack.gDataBodyPack.nOrgID);
			}
			break;

		case 2:
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from T_User where User_Org in \
					(select Org_Id from T_Org where Org_Id = %d) or User_Org = %d",
					askWorkerListUpPack.gDataBodyPack.nOrgID,
					askWorkerListUpPack.gDataBodyPack.nOrgID);
			}
			break;

		default:
			return NULL;
		}
				
		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}

bool CAccessBaseData::UploadAddPowerGroupPack(const AddPowerGroup_UpLoad_Pack& addPowerGroupUpPack, 
	AddPowerGroup_DownLoad_Pack& addPowerGroupDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into T_PowerGroup(Power_name, Group1, Group2, Group3) values ('%s', %d, %d, %d)",
			addPowerGroupUpPack.gDataBodyPack.strPowerName.c_str(),
			addPowerGroupUpPack.gDataBodyPack.iPow1,
			addPowerGroupUpPack.gDataBodyPack.iPow2,
			addPowerGroupUpPack.gDataBodyPack.iPow3);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			goto ErrorDeal;
		}

		nErrorCode = 0;
ErrorDeal:
		addPowerGroupDownPack.nBeginIdentify = addPowerGroupUpPack.nBeginIdentify;
		addPowerGroupDownPack.nMsgNumber = addPowerGroupUpPack.nMsgNumber;

		addPowerGroupDownPack.gDataBodyPack.nResult = nErrorCode;

		addPowerGroupDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UploadModifyPowerGroupPack(const ModifyPowerGroup_UpLoad_Pack& modifyPowerGroupUpPack, 
	ModifyPowerGroup_DownLoad_Pack& modifyPowerGroupDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);

		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from T_PowerGroup where Power_Id = %d",
			modifyPowerGroupUpPack.gDataBodyPack.nPowerID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(m_adoRecordSet.Open(m_pDatabase->GetActiveConnection(), m_strBuffer))
		{
			if(m_adoRecordSet.GetRecordCount() <= 0)
			{
				goto ErrorDeal;
			}
		}

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);

		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update T_PowerGroup set Power_name = '%s', Group1 = %d, Group2 = %d, Group3 = %d where Power_Id = %d",
			modifyPowerGroupUpPack.gDataBodyPack.strPowerName.c_str(),
			modifyPowerGroupUpPack.gDataBodyPack.nPow1,
			modifyPowerGroupUpPack.gDataBodyPack.nPow2,
			modifyPowerGroupUpPack.gDataBodyPack.nPow3,
			modifyPowerGroupUpPack.gDataBodyPack.nPowerID);

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			goto ErrorDeal;
		}

		nErrorCode = 0;
ErrorDeal:
		modifyPowerGroupDownPack.nBeginIdentify = modifyPowerGroupUpPack.nBeginIdentify;
		modifyPowerGroupDownPack.nMsgNumber = modifyPowerGroupUpPack.nMsgNumber;

		modifyPowerGroupDownPack.gDataBodyPack.nResult = nErrorCode;

		modifyPowerGroupDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UploadDeletePowerGroupPack(const DeletePowerGroup_UpLoad_Pack& deletePowerGroupUpPack, 
	DeletePowerGroup_DownLoad_Pack& deletePowerGroupDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "delete T_PowerGroup where Power_Id = %d",
			deletePowerGroupUpPack.gDataBodyPack.nPowerID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			goto ErrorDeal;
		}

		nErrorCode = 0;
ErrorDeal:
		deletePowerGroupDownPack.nBeginIdentify = deletePowerGroupUpPack.nBeginIdentify;
		deletePowerGroupDownPack.nMsgNumber = deletePowerGroupUpPack.nMsgNumber;

		deletePowerGroupDownPack.gDataBodyPack.nResult = nErrorCode;

		deletePowerGroupDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

CADORecordset* CAccessBaseData::UploadAskPowerGroupListPack(const AskPowerGroupList_UpLoad_Pack& askPowerGroupListUpPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);

		if(askPowerGroupListUpPack.gDataBodyPack.nPowerID == 0)
		{
			sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from T_PowerGroup");
		}
		else
		{
			sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from T_PowerGroup where Power_Id = %d",
				askPowerGroupListUpPack.gDataBodyPack.nPowerID);
		}

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}

bool CAccessBaseData::UploadAddDevicePack(const AddDevice_UpLoad_Pack& addDeviceUpPack, 
	AddDevice_DownLoad_Pack& addDeviceDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into T_Device( Device_Pid, Device_Org, Device_Type)	values ('%s', %d, %d)",
			addDeviceUpPack.gDataBodyPack.strDevicePID.c_str(),
			addDeviceUpPack.gDataBodyPack.nDeviceOrg,
			addDeviceUpPack.gDataBodyPack.nDeviceType);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			goto ErrorDeal;
		}

		nErrorCode = 0;
ErrorDeal:
		addDeviceDownPack.nBeginIdentify = addDeviceUpPack.nBeginIdentify;
		addDeviceDownPack.nMsgNumber = addDeviceUpPack.nMsgNumber;

		addDeviceDownPack.gDataBodyPack.nResult = nErrorCode;

		addDeviceDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UploadModifyDevicePack(const ModifyDevice_UpLoad_Pack& modifyDeviceUpPack, 
	ModifyDevice_DownLoad_Pack& modifyDeviceDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);

		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from T_Device where Device_id = %d",
			modifyDeviceUpPack.gDataBodyPack.nDeviceID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(m_adoRecordSet.Open(m_pDatabase->GetActiveConnection(), m_strBuffer))
		{
			if(m_adoRecordSet.GetRecordCount() <= 0)
			{
				goto ErrorDeal;
			}
		}

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);

		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update T_Device SET Device_Pid = '%s', Device_Org = %d, Device_Type = %d WHERE Device_id = %d",
			modifyDeviceUpPack.gDataBodyPack.strDevicePID.c_str(),
			modifyDeviceUpPack.gDataBodyPack.nDeviceOrg,
			modifyDeviceUpPack.gDataBodyPack.nDeviceType,
			modifyDeviceUpPack.gDataBodyPack.nDeviceID);

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			goto ErrorDeal;
		}

		nErrorCode = 0;
ErrorDeal:
		modifyDeviceDownPack.nBeginIdentify = modifyDeviceUpPack.nBeginIdentify;
		modifyDeviceDownPack.nMsgNumber = modifyDeviceUpPack.nMsgNumber;

		modifyDeviceDownPack.gDataBodyPack.nResult = nErrorCode;

		modifyDeviceDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UploadDeleteDevicePack(const DeleteDevice_UpLoad_Pack& deleteDeviceUpPack, 
	DeleteDevice_DownLoad_Pack& deleteDeviceDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "delete T_Device where Device_id=%d",
			deleteDeviceUpPack.gDataBodyPack.nDeviceID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			goto ErrorDeal;
		}

		nErrorCode = 0;
ErrorDeal:
		deleteDeviceDownPack.nBeginIdentify = deleteDeviceUpPack.nBeginIdentify;
		deleteDeviceDownPack.nMsgNumber = deleteDeviceUpPack.nMsgNumber;

		deleteDeviceDownPack.gDataBodyPack.nResult = nErrorCode;

		deleteDeviceDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

CADORecordset* CAccessBaseData::UploadAskDeviceListPack(const AskDeviceList_Upload_Pack& askDeviceListUpPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		switch(askDeviceListUpPack.gDataBodyPack.nType)
		{
		case 0:
			{
				switch(askDeviceListUpPack.gDataBodyPack.nDeivceType)
				{
				case 2:
					{
						sprintf_s(m_strBuffer, INPUTSQLBUFFERS, 
							"select * from T_Device \
							where  Device_Org in \
							(select Org_Id from T_Org where Org_Upid = %d) or Device_Org = %d",
							askDeviceListUpPack.gDataBodyPack.nOrgID,
							askDeviceListUpPack.gDataBodyPack.nOrgID);
					}
					break;

				default:
					{
						sprintf_s(m_strBuffer, INPUTSQLBUFFERS, 
							"select * from T_Device \
							where Device_Type = %d and Device_Org in \
							(select Org_Id from T_Org where Org_Upid = %d) or Device_Org = %d",
							askDeviceListUpPack.gDataBodyPack.nDeivceType,
							askDeviceListUpPack.gDataBodyPack.nOrgID,
							askDeviceListUpPack.gDataBodyPack.nOrgID);
					}
					break;
				}
			}
			break;

		case 1:
			{
				switch(askDeviceListUpPack.gDataBodyPack.nDeivceType)
				{
				case 2:
					{
						sprintf_s(m_strBuffer, INPUTSQLBUFFERS, 
							"select * from T_Device \
							where  Device_Org = %d",
							askDeviceListUpPack.gDataBodyPack.nOrgID);
					}
					break;

				default:
					{
						sprintf_s(m_strBuffer, INPUTSQLBUFFERS, 
							"select * from T_Device \
							where Device_Type = %d and Device_Org =  %d)",
							askDeviceListUpPack.gDataBodyPack.nDeivceType,
							askDeviceListUpPack.gDataBodyPack.nOrgID);
					}
					break;
				}
			}
			break;

		default:
			return NULL;
		}

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}

CADORecordset* CAccessBaseData::UpLoadGetGPSPointPack(const GetGPSPoint_Upload_Pack& getGPSPointUpPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "Select * from t_gps where G_tel = '%s' and '%s' > G_time and '%s' < G_time",
			getGPSPointUpPack.gDataBodyPack.strTel.c_str(),
			getGPSPointUpPack.gDataBodyPack.strEndTime.c_str(),
			getGPSPointUpPack.gDataBodyPack.strTime.c_str());

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}

CADORecordset* CAccessBaseData::UpLoadGetRailWayNamePack(const GetRailWayName_Upload_Pack& getRailWayTableNameUpPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		if(getRailWayTableNameUpPack.gDataBodyPack.nLineID == 0)
		{
			sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from  T_TLName");
		}
		else
		{
			sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from  T_TLName where TL_id=%d",
				getRailWayTableNameUpPack.gDataBodyPack.nLineID);
		}

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}

bool CAccessBaseData::UpLoadSettingLinePack(const SettingLine_Upload_Pack& settingLineUpPack,
	SettingLine_Download_Pack& settingLineDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		switch(settingLineUpPack.gDataBodyPack.nType)
		{
		case 0:			// add
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into T_Line(Line_Name, Line_day, Line_org) values('%s', %d, %d)",
					settingLineUpPack.gDataBodyPack.strName.c_str(),
					settingLineUpPack.gDataBodyPack.nDay,
					settingLineUpPack.gDataBodyPack.nOrgID);
				nErrorCode = 0;
			}
			break;

		case 1:			// edit
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update T_Line set Line_Name = '%s', Line_day = %d where Line_id = %d",
					settingLineUpPack.gDataBodyPack.strName.c_str(),
					settingLineUpPack.gDataBodyPack.nDay,
					settingLineUpPack.gDataBodyPack.nID);
				nErrorCode = 0;
			}
			break;

		case 2:			// del
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "delete T_Line where Line_id = %d",
					settingLineUpPack.gDataBodyPack.nID);
				nErrorCode = 0;
			}
			break;

		default:
			nErrorCode = 1;
			break;
		}

		if(nErrorCode == 0)
		{
			if(!m_pDatabase->IsOpen())
			{
				if(!m_pDatabase->Open())
				{
					nErrorCode = 1;
					goto ErrorDeal;
				}
			}

			if(!m_pDatabase->Execute(m_strBuffer))
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}
		}

ErrorDeal:
		settingLineDownPack.nBeginIdentify = settingLineDownPack.nBeginIdentify;
		settingLineDownPack.nMsgNumber = settingLineDownPack.nMsgNumber;
		
		settingLineDownPack.gDataBodyPack.nResult = nErrorCode;
		
		settingLineDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UpLoadSettingPollingTimePack(const SettingPollingTime_Upload_Pack& settingPollingTimeUpPack,
	SettingPollingTime_Download_Pack& settingPollingTimeDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		switch(settingPollingTimeUpPack.gDataBodyPack.nType)
		{
		case 0:			// add
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into T_LineTimes(Lt_LineId, Lt_PointId, Lt_PointTimes) values(%d, %d, '%s')",
					settingPollingTimeUpPack.gDataBodyPack.nLineID,
					settingPollingTimeUpPack.gDataBodyPack.nPointID,
					settingPollingTimeUpPack.gDataBodyPack.strTime.c_str());
				nErrorCode = 0;
			}
			break;

		case 1:			// edit
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update T_LineTimes set Lt_LineId = %d, Lt_PointId = %d, Lt_PointTimes = '%s' where LT_id = %d",
					settingPollingTimeUpPack.gDataBodyPack.nLineID,
					settingPollingTimeUpPack.gDataBodyPack.nPointID,
					settingPollingTimeUpPack.gDataBodyPack.strTime.c_str(),
					settingPollingTimeUpPack.gDataBodyPack.nID);
				nErrorCode = 0;
			}
			break;

		case 2:			// del
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "delete T_LineTimes where LT_id = %d",
					settingPollingTimeUpPack.gDataBodyPack.nID);
				nErrorCode = 0;
			}
			break;

		default:
			nErrorCode = 1;
			break;
		}

		if(nErrorCode == 0)
		{
			if(!m_pDatabase->IsOpen())
			{
				if(!m_pDatabase->Open())
				{
					nErrorCode = 1;
					goto ErrorDeal;
				}
			}

			if(!m_pDatabase->Execute(m_strBuffer))
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}
		}

ErrorDeal:
		settingPollingTimeDownPack.nBeginIdentify = settingPollingTimeUpPack.nBeginIdentify;
		settingPollingTimeDownPack.nMsgNumber = settingPollingTimeUpPack.nMsgNumber;

		settingPollingTimeDownPack.gDataBodyPack.nResult = nErrorCode;

		settingPollingTimeDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UpLoadSettingSchedule5Pack(const SettingSchedule5_Upload_Pack& settingSchedule5UpPack,
	SettingSchedule5_Download_Pack& settingSchedule5DownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		switch(settingSchedule5UpPack.gDataBodyPack.nType)
		{
		case 0:			// add
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into T_PB(PB_star, Pb_Cyc, Pb_Content, pb_org) values('%s', %d, '%s', %d)",
					settingSchedule5UpPack.gDataBodyPack.strStart.c_str(),
					settingSchedule5UpPack.gDataBodyPack.nCYC,
					settingSchedule5UpPack.gDataBodyPack.strContent.c_str(),
					settingSchedule5UpPack.gDataBodyPack.nOrgID);
				nErrorCode = 0;
			}
			break;

		case 1:			// edit
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update T_PB set PB_star = '%s', Pb_Cyc = %d, Pb_Content = '%s', pb_org = %d where PB_id = %d",
					settingSchedule5UpPack.gDataBodyPack.strStart.c_str(),
					settingSchedule5UpPack.gDataBodyPack.nCYC,
					settingSchedule5UpPack.gDataBodyPack.strContent.c_str(),
					settingSchedule5UpPack.gDataBodyPack.nOrgID,
					settingSchedule5UpPack.gDataBodyPack.nID);
				nErrorCode = 0;
			}
			break;

		case 2:			// del
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "delete T_PB where PB_id = %d",
					settingSchedule5UpPack.gDataBodyPack.nID);
				nErrorCode = 0;
			}
			break;

		default:
			nErrorCode = 1;
			break;
		}

		if(nErrorCode == 0)
		{
			if(!m_pDatabase->IsOpen())
			{
				if(!m_pDatabase->Open())
				{
					nErrorCode = 1;
					goto ErrorDeal;
				}
			}

			if(!m_pDatabase->Execute(m_strBuffer))
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}
		}

ErrorDeal:
		settingSchedule5DownPack.nBeginIdentify = settingSchedule5UpPack.nBeginIdentify;
		settingSchedule5DownPack.nMsgNumber = settingSchedule5UpPack.nMsgNumber;

		settingSchedule5DownPack.gDataBodyPack.nResult = nErrorCode;

		settingSchedule5DownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UpLoadSettingSchedule6Pack(const SettingSchedule6_Upload_Pack& settingSchedule6UpPack,
	SettingSchedule6_Download_Pack& settingSchedule6DownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		switch(settingSchedule6UpPack.gDataBodyPack.nType)
		{
		case 0:			// add
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into T_PbLine(PL_PBID, PL_Lineid) values(%d, %d)",
					settingSchedule6UpPack.gDataBodyPack.nPL_PBID,
					settingSchedule6UpPack.gDataBodyPack.nPL_LineID);
				nErrorCode = 0;
			}
			break;

		case 1:			// edit
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update T_PbLine set PL_PBID = %d, PL_Lineid = %d where PL_Id = %d",
					settingSchedule6UpPack.gDataBodyPack.nPL_LineID,
					settingSchedule6UpPack.gDataBodyPack.nPL_LineID,
					settingSchedule6UpPack.gDataBodyPack.nPL_ID);
				nErrorCode = 0;
			}
			break;

		case 2:			// del
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "delete T_PbLine where PL_Id = %d",
					settingSchedule6UpPack.gDataBodyPack.nPL_ID);
				nErrorCode = 0;
			}
			break;

		default:
			nErrorCode = 1;
			break;
		}

		if(nErrorCode == 0)
		{
			if(!m_pDatabase->IsOpen())
			{
				if(!m_pDatabase->Open())
				{
					nErrorCode = 1;
					goto ErrorDeal;
				}
			}

			if(!m_pDatabase->Execute(m_strBuffer))
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}
		}

ErrorDeal:
		settingSchedule6DownPack.nBeginIdentify = settingSchedule6UpPack.nBeginIdentify;
		settingSchedule6DownPack.nMsgNumber = settingSchedule6UpPack.nMsgNumber;

		settingSchedule6DownPack.gDataBodyPack.nResult = nErrorCode;

		settingSchedule6DownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UpLoadSettingScheduleWorkerPack(const SettingScheduleWorker_Upload_Pack& settingScheduleWorkerUpPack,
	SettingScheduleWorker_Download_Pack& settingScheduleWorkerDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		switch(settingScheduleWorkerUpPack.gDataBodyPack.nType)
		{
		case 0:			// add
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into T_PbPerson(pp_BID, Pp_pid, Pp_sx) values(%d, '%s', %d)",
					settingScheduleWorkerUpPack.gDataBodyPack.nPBID,
					settingScheduleWorkerUpPack.gDataBodyPack.strRYID.c_str(),
					settingScheduleWorkerUpPack.gDataBodyPack.nPX);
				nErrorCode = 0;
			}
			break;

		case 1:			// edit
			{
				//sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update T_PbPerson set pp_BID = %d, Pp_pid = %d, Pp_sx = %d where Pp_Id = %d",
				//	settingScheduleWorkerUpPack.gDataBodyPack.nPBID,
				//	settingScheduleWorkerUpPack.gDataBodyPack.strRYID,
				//	settingScheduleWorkerUpPack.gDataBodyPack.nPX,
				//	settingScheduleWorkerUpPack.gDataBodyPack.nID);
				nErrorCode = 1;
			}
			break;

		case 2:			// del
			{
				//sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "delete T_PbPerson where Pp_Id = %d",
				//	settingScheduleWorkerUpPack.gDataBodyPack.nID);
				nErrorCode = 1;
			}
			break;

		default:
			nErrorCode = 1;
			break;
		}

		if(nErrorCode == 0)
		{
			if(!m_pDatabase->IsOpen())
			{
				if(!m_pDatabase->Open())
				{
					nErrorCode = 1;
					goto ErrorDeal;
				}
			}

			if(!m_pDatabase->Execute(m_strBuffer))
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}
		}

ErrorDeal:
		settingScheduleWorkerDownPack.nBeginIdentify = settingScheduleWorkerUpPack.nBeginIdentify;
		settingScheduleWorkerDownPack.nMsgNumber = settingScheduleWorkerUpPack.nMsgNumber;

		settingScheduleWorkerDownPack.gDataBodyPack.nResult = nErrorCode;

		settingScheduleWorkerDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UpLoadUrgencyReleasePack(const UrgencyRelease_Upload_Pack& urgencyReleaseUpPack,
	UrgencyRelease_Download_Pack& urgencyReleaseDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		switch(urgencyReleaseUpPack.gDataBodyPack.nType)
		{
		case 0:			// add
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into T_JJ(Jj_StartP, Jj_Endp, Jj_StartD, Jj_endd, JJ_State, Jj_lineid, jj_org, jj_name) \
														values(%d, %d, '%s', '%s', %d, %d, %d, '%s')",
					urgencyReleaseUpPack.gDataBodyPack.nStartPointID,
					urgencyReleaseUpPack.gDataBodyPack.nEndPointID,
					urgencyReleaseUpPack.gDataBodyPack.strStartTime.c_str(),
					urgencyReleaseUpPack.gDataBodyPack.strEndTime.c_str(),
					urgencyReleaseUpPack.gDataBodyPack.nState,
					urgencyReleaseUpPack.gDataBodyPack.nLineID,
					urgencyReleaseUpPack.gDataBodyPack.nOrgID,
					urgencyReleaseUpPack.gDataBodyPack.strRWName.c_str());
				nErrorCode = 0;
			}
			break;

		case 1:			// edit
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update T_JJ set Jj_StartP = %d, Jj_Endp = %d, Jj_StartD ='%s', Jj_endd = '%s', JJ_State = %d, Jj_lineid = %d, jj_org  = %d, jj_name = '%s' \
														where Jj_Id = %d",
					urgencyReleaseUpPack.gDataBodyPack.nStartPointID,
					urgencyReleaseUpPack.gDataBodyPack.nEndPointID,
					urgencyReleaseUpPack.gDataBodyPack.strStartTime.c_str(),
					urgencyReleaseUpPack.gDataBodyPack.strEndTime.c_str(),
					urgencyReleaseUpPack.gDataBodyPack.nState,
					urgencyReleaseUpPack.gDataBodyPack.nLineID,
					urgencyReleaseUpPack.gDataBodyPack.nOrgID,
					urgencyReleaseUpPack.gDataBodyPack.strRWName.c_str(),
					urgencyReleaseUpPack.gDataBodyPack.nID);
				nErrorCode = 0;
			}
			break;

		case 2:			// del
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "delete T_JJ where Jj_Id = %d",
					urgencyReleaseUpPack.gDataBodyPack.nID);
				nErrorCode = 0;
			}
			break;

		default:
			nErrorCode = 1;
			break;
		}

		if(nErrorCode == 0)
		{
			if(!m_pDatabase->IsOpen())
			{
				if(!m_pDatabase->Open())
				{
					nErrorCode = 1;
					goto ErrorDeal;
				}
			}

			if(!m_pDatabase->Execute(m_strBuffer))
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}

			if(0 == urgencyReleaseUpPack.gDataBodyPack.nType)
			{
				memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "SELECT MAX(Jj_Id) AS Expr1 FROM T_JJ");

				if(m_adoRecordSet.Open(m_pDatabase->GetActiveConnection(), m_strBuffer))
				{
					m_adoRecordSet.GetFieldValue("Expr1", nErrorCode);
					goto ErrorDeal;
				}
				nErrorCode = 0;
			}
		}

ErrorDeal:
		urgencyReleaseDownPack.nBeginIdentify = urgencyReleaseUpPack.nBeginIdentify;
		urgencyReleaseDownPack.nMsgNumber = urgencyReleaseUpPack.nMsgNumber;

		urgencyReleaseDownPack.gDataBodyPack.nResult = nErrorCode;

		urgencyReleaseDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UpLoadUrgencyWorkerPack(const UrgencyWorker_Upload_Pack& urgencyWorkerUpPack,
	UrgencyWorker_Download_Pack& urgencyWorkerDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		switch(urgencyWorkerUpPack.gDataBodyPack.nType)
		{
		case 0:			// add
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into t_jjry(Jj_id, Jj_Pid, Jj_pDevice) values(%d, '%s', %d)",
					urgencyWorkerUpPack.gDataBodyPack.nJJ_ID,
					urgencyWorkerUpPack.gDataBodyPack.strJJ_PID.c_str(),
					urgencyWorkerUpPack.gDataBodyPack.nJJ_PDevice);
				nErrorCode = 0;
			}
			break;

		//case 1:			// edit
		//	{
		//		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update t_jjry set Jj_id = %d, Jj_Pid = '%s', Jj_pDevice = %d where Jy_Id = %d",
		//			urgencyWorkerUpPack.gDataBodyPack.nJJ_ID,
		//			urgencyWorkerUpPack.gDataBodyPack.strJJ_PID,
		//			urgencyWorkerUpPack.gDataBodyPack.nJJ_PDevice,
		//			urgencyWorkerUpPack.gDataBodyPack.nID);
		//		nErrorCode = 0;
		//	}
		//	break;

		case 1:			// del
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "delete t_jjry where Jj_Pid = '%s'",
					urgencyWorkerUpPack.gDataBodyPack.strJJ_PID.c_str());
				nErrorCode = 0;
			}
			break;

		default:
			nErrorCode = 1;
			break;
		}

		if(nErrorCode == 0)
		{
			if(!m_pDatabase->IsOpen())
			{
				if(!m_pDatabase->Open())
				{
					nErrorCode = 1;
					goto ErrorDeal;
				}
			}

			if(!m_pDatabase->Execute(m_strBuffer))
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}
		}

ErrorDeal:
		urgencyWorkerDownPack.nBeginIdentify = urgencyWorkerUpPack.nBeginIdentify;
		urgencyWorkerDownPack.nMsgNumber = urgencyWorkerUpPack.nMsgNumber;

		urgencyWorkerDownPack.gDataBodyPack.nResult = nErrorCode;

		urgencyWorkerDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

CADORecordset* CAccessBaseData::UploadGetLinePointPack(const GetLinePoint_Upload_Pack& askDeviceListUpPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from t_org where org_id = %d",
			askDeviceListUpPack.gDataBodyPack.nOrgID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		if(m_adoRecordSet.Open(m_pDatabase->GetActiveConnection(), m_strBuffer))
		{
			if(m_adoRecordSet.GetRecordCount() <= 0)
			{
				return NULL;
			}

			int nLineID, nStartID, nEndID;

			m_adoRecordSet.MoveFirst();
			m_adoRecordSet.GetFieldValue("org_lineid", nLineID);
			m_adoRecordSet.GetFieldValue("org_startid", nStartID);
			m_adoRecordSet.GetFieldValue("org_endid", nEndID);

			memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
			sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from T_Point where Pit_TLid = %d and Pit_Dis between %d and %d",
				nLineID,
				nStartID, 
				nEndID);

			CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
			if(pRecordset->Open(m_strBuffer))
			{
				return pRecordset;
			}
		}
	}
	return NULL;
}

CADORecordset* CAccessBaseData::UpLoadGetPicListPack(const GetPicList_Upload_Pack& getPicListUpPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from t_pic where tel in (select Device_Pid from t_device where Device_Org = %d) and datediff(d,dtime,'%s') > 0 and datediff(d,'%s',dtime) >= 0",
			getPicListUpPack.gDataBodyPack.nOrgID,
			getPicListUpPack.gDataBodyPack.strETime.c_str(),
			getPicListUpPack.gDataBodyPack.strSTime.c_str());

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}

CADORecordset* CAccessBaseData::UpLoadGetUrgencyMissionListPack(const GetUrgencyMissionList_Upload_Pack& getRailWayNameUpPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from t_jj where jj_org = %d",
			getRailWayNameUpPack.gDataBodyPack.nOrgID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}

CADORecordset* CAccessBaseData::UploadGetUrgencyWorkerListPack(const GetUrgencyWorkerList_Upload_Pack& askDeviceListUpPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select *, (select top 1 [user_name] from t_user where t_user.user_oper = t_jjry.jj_pid) as username, (select top 1 User_Org from t_user where t_user.user_oper = t_jjry.jj_pid) as userOrg from t_jjry where jj_id = %d",
			askDeviceListUpPack.gDataBodyPack.nJJID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}

bool CAccessBaseData::UpLoadGetPicDataPack(const GetPicData_Upload_Pack& getPicDataUpPack,
	GPSPIC_Pack& gpsPicPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from t_pic where pname = '%s'",
			getPicDataUpPack.gDataBodyPack.strPicName.c_str());

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return false;
			}
		}

		if(m_adoRecordSet.Open(m_pDatabase->GetActiveConnection(), m_strBuffer))
		{
			if(m_adoRecordSet.GetRecordCount() <= 0)
			{
				return false;
			}

			CString strTemp;

			m_adoRecordSet.MoveFirst();

			m_adoRecordSet.GetFieldValue("pname", strTemp);
			gpsPicPack.strPicName = strTemp.GetBuffer();
			strTemp.LockBuffer();

			m_adoRecordSet.GetFieldValue("dtime", strTemp);
			gpsPicPack.strTime = strTemp.GetBuffer();
			strTemp.LockBuffer();

			return true;
		}
	}
	return false;
}

CADORecordset* CAccessBaseData::UpLoadGetOrgSchedueListPack(const GetOrgSchedueList_Upload_Pack& getOrgSchedueListUpPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;
		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}
		
 		CADORecordset *padoRecordset = new CADORecordset(m_pDatabase);;
		CADOCommand adoCommand(m_pDatabase, _T("dbo.tj"));

		CADOParameter orgidPar(adOpenDynamic, sizeof(int), adParamInput, _T("@orgid"));
		CADOParameter yearPar(adOpenDynamic, sizeof(int), adParamInput, _T("@year"));
		CADOParameter monthPar(adOpenDynamic, sizeof(int), adParamInput, _T("@month"));

		orgidPar.SetValue((long)getOrgSchedueListUpPack.gDataBodyPack.nOrgID);
		yearPar.SetValue((long)getOrgSchedueListUpPack.gDataBodyPack.nYears);
		monthPar.SetValue((long)getOrgSchedueListUpPack.gDataBodyPack.nMonths);

		adoCommand.AddParameter(&orgidPar);
		adoCommand.AddParameter(&yearPar);
		adoCommand.AddParameter(&monthPar);

		if(padoRecordset->Execute(&adoCommand))
		{
			return padoRecordset;
		}
	}
	return NULL;
}

bool CAccessBaseData::UpLoadGetOrgSchedueInfoPack(const GetOrgSchedueInfo_Upload_Pack& getOrgSchedueInfoUpPack,
	GetOrgSchedueInfo_Download_Pack& getOrgSchedueInfoDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select  top 1 * from t_pb where PB_org = %d",
			getOrgSchedueInfoUpPack.gDataBodyPack.nOrgID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				goto ErrorDeal;
			}
		}

		if(m_adoRecordSet.Open(m_pDatabase->GetActiveConnection(), m_strBuffer))
		{
			if(m_adoRecordSet.GetRecordCount() <= 0)
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}

			int     nTemp;
			CString strTemp;

			m_adoRecordSet.MoveFirst();

			m_adoRecordSet.GetFieldValue("PB_id", nTemp);
			getOrgSchedueInfoDownPack.gDataBodyPack.nID = nTemp;

			m_adoRecordSet.GetFieldValue("Pb_Cyc", nTemp);
			getOrgSchedueInfoDownPack.gDataBodyPack.nCYC = nTemp;

			m_adoRecordSet.GetFieldValue("PB_star", strTemp);
			getOrgSchedueInfoDownPack.gDataBodyPack.strTime = strTemp.GetBuffer();
			strTemp.LockBuffer();

			m_adoRecordSet.GetFieldValue("Pb_Content", strTemp);
			getOrgSchedueInfoDownPack.gDataBodyPack.strContent = strTemp.GetBuffer();
			strTemp.LockBuffer();

			getOrgSchedueInfoDownPack.nBodyLength = 78;
			return true;
		}
ErrorDeal:
		getOrgSchedueInfoDownPack.nBeginIdentify = getOrgSchedueInfoUpPack.nBeginIdentify;
		getOrgSchedueInfoDownPack.nMsgNumber = getOrgSchedueInfoUpPack.nMsgNumber;

		getOrgSchedueInfoDownPack.gDataBodyPack.nID = 0;
		getOrgSchedueInfoDownPack.gDataBodyPack.nCYC = 0;
		getOrgSchedueInfoDownPack.gDataBodyPack.strContent = "";
		getOrgSchedueInfoDownPack.gDataBodyPack.strTime = "";

		getOrgSchedueInfoDownPack.nBodyLength = 78;
		return true;
	}
	return false;
}

bool CAccessBaseData::UpLoadSetPointPack(const SetPoint_Upload_Pack& setPointUpPack,
	SetPoint_Download_Pack& setPointDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;

		switch(setPointUpPack.gDataBodyPack.nIType)
		{
		case 0:
			{
				memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from T_Point where Pit_TLid = %d and Pit_Dis = %f  and Pit_SX = %d",
					setPointUpPack.gDataBodyPack.nIRailLine,
					setPointUpPack.gDataBodyPack.dIKM,
					setPointUpPack.gDataBodyPack.nIDirect);

				if(!m_pDatabase->IsOpen())
				{
					if(!m_pDatabase->Open())
					{
						goto ErrorDeal;
					}
				}

				if(m_adoRecordSet.Open(m_pDatabase->GetActiveConnection(), m_strBuffer))
				{
					if(m_adoRecordSet.GetRecordCount() > 0)
					{
						nErrorCode = 1;
						goto ErrorDeal;
					}

					memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
					sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into t_point (pit_j, pit_w, pit_tlid, pit_sx, pit_dis) values(%f, %f, %d, %d, %f)",
						setPointUpPack.gDataBodyPack.dILon,
						setPointUpPack.gDataBodyPack.dILat,
						setPointUpPack.gDataBodyPack.nIRailLine,
						setPointUpPack.gDataBodyPack.nIDirect,
						setPointUpPack.gDataBodyPack.dIKM);

					if(!m_pDatabase->Execute(m_strBuffer))
					{
						nErrorCode = 1;
						goto ErrorDeal;
					}

					nErrorCode = 0;
				}
			}
			break;
		case 1:
			{
				memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update t_point set pit_j = %f, pit_w = %f, pit_tlid = %d, pit_sx = %d, pit_dis = %f where Pit_id = %d",
					setPointUpPack.gDataBodyPack.dILon,
					setPointUpPack.gDataBodyPack.dILat,
					setPointUpPack.gDataBodyPack.nIRailLine,
					setPointUpPack.gDataBodyPack.nIDirect,
					setPointUpPack.gDataBodyPack.dIKM,
					setPointUpPack.gDataBodyPack.nPointId);

				if(!m_pDatabase->Execute(m_strBuffer))
				{
					nErrorCode = 1;
					goto ErrorDeal;
				}

				nErrorCode = 0;

			}
			break;
		case 2:
			{
				memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "delete t_point where Pit_id = %d",
					setPointUpPack.gDataBodyPack.nPointId);

				if(!m_pDatabase->Execute(m_strBuffer))
				{
					nErrorCode = 1;
					goto ErrorDeal;
				}

				nErrorCode = 0;
			}
			break;
		}
ErrorDeal:
		setPointDownPack.nBeginIdentify = setPointUpPack.nBeginIdentify;
		setPointDownPack.nMsgNumber = setPointUpPack.nMsgNumber;

		setPointDownPack.gDataBodyPack.nResult = nErrorCode;
		setPointDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

CADORecordset* CAccessBaseData::UploadGetOrgLinePack(const GetOrgLine_Upload_Pack& getOrgLineUpPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select line_id,line_name,line_day from t_line where line_org = %d",
			getOrgLineUpPack.gDataBodyPack.nOrgID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}

CADORecordset* CAccessBaseData::UpLoadGetLinePointTimePack(const GetLinePointTime_Upload_Pack& getLinePointTimeUpPack){
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select pit_dis,pit_j,pit_w,pit_sx,lt_pointid,Lt_PointTimes from v_pLineTime where Lt_LineId = %d",
			getLinePointTimeUpPack.gDataBodyPack.nLineID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}

CADORecordset* CAccessBaseData::UpLoadGetOrgSchWorkerPack(const GetOrgSchWorker_Upload_Pack& getOrgSchWorkerUpPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from v_user where User_Org = %d order by Pp_sx asc",
			getOrgSchWorkerUpPack.gDataBodyPack.nOrgID);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}

CADORecordset* CAccessBaseData::UploadWorkerPollQureyPack(const WorkerPollQuery_Upload_Pack& workerPollQuerUpPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "SELECT * FROM T_GPS WHERE datediff(d,G_TIME,'%s')=0 and dbo.cherkPda('%s',g_tel)=1",
			workerPollQuerUpPack.gDataBodyPack.strTime.c_str(),
			workerPollQuerUpPack.gDataBodyPack.strOper.c_str());

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}

bool CAccessBaseData::UpLoadUrgencyMissionDeletePack(const UrgencyMissionDelete_Upload_Pack& urgencyMissionDeleteUpPack,
	UrgencyMissionDelete_Download_Pack& urgencyMissionDeleteDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "Delete from t_jjry where jj_id = %d",
			urgencyMissionDeleteUpPack.gDataBodyPack.nJJRW);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}
		}

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			nErrorCode = 1;
			goto ErrorDeal;
		}
		nErrorCode = 0;

ErrorDeal:
		urgencyMissionDeleteDownPack.nBeginIdentify = urgencyMissionDeleteUpPack.nBeginIdentify;
		urgencyMissionDeleteDownPack.nMsgNumber = urgencyMissionDeleteUpPack.nMsgNumber;
		urgencyMissionDeleteDownPack.gDataBodyPack.nResult = nErrorCode;

		urgencyMissionDeleteDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

CADORecordset* CAccessBaseData::UploadWorkerPollPack(const WorkerPoll_Upload_Pack& workerPollUpPack)
{
	if(m_pDatabase)
	{
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "Select R_pointid,r_pxdate,R_arrTime,R_realtime,R_pid,P_state from T_realPb where R_Userid ='%s' and r_pxdate = '%s'",
			workerPollUpPack.gDataBodyPack.strOper.c_str(),
			workerPollUpPack.gDataBodyPack.strDate.c_str());

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}


bool CAccessBaseData::UpLoadSysSettingPack(const SysSetting_Upload_Pack& sysSettingUpPack,
	SysSetting_Download_Pack& sysSettingDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update t_sys set dataBak=%d, bak=%d, bak1=%d, WarnTime=%f, FBak=%f, cData='%s', cData1='%s' where id=1",
			sysSettingUpPack.gDataBodyPack.nDataBak,
			sysSettingUpPack.gDataBodyPack.nBak,
			sysSettingUpPack.gDataBodyPack.nBak1,
			sysSettingUpPack.gDataBodyPack.fWarnTime,
			sysSettingUpPack.gDataBodyPack.fBak, 
			sysSettingUpPack.gDataBodyPack.strData.c_str(),
			sysSettingUpPack.gDataBodyPack.strData1.c_str());

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}
		}

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			nErrorCode = 1;
			goto ErrorDeal;
		}
		if(m_pDatabase->GetRecordsAffected() > 0)
			nErrorCode = 0;
		else
			nErrorCode = 1;

ErrorDeal:
		sysSettingDownPack.nBeginIdentify = sysSettingUpPack.nBeginIdentify;
		sysSettingDownPack.nMsgNumber = sysSettingUpPack.nMsgNumber;
		sysSettingDownPack.gDataBodyPack.nResult = nErrorCode;

		sysSettingDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

bool CAccessBaseData::UpLoadSysSettingGetPack(const SysSettingGet_Upload_Pack& sysSettingGetUpPack,
	SysSettingGet_Download_Pack& sysSettingGetDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select * from t_sys");

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}
		}

		if(m_adoRecordSet.Open(m_pDatabase->GetActiveConnection(), m_strBuffer))
		{
			if(m_adoRecordSet.GetRecordCount() <= 0)
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}

			int     nTemp;
			double  dTemp;
			CString strTemp;//dataBak, bak, bak1, WarnTime, FBak, cData, cData1

			m_adoRecordSet.MoveFirst();

			m_adoRecordSet.GetFieldValue("dataBak", nTemp);
			sysSettingGetDownPack.gDataBodyPack.nDataBak = nTemp;

			m_adoRecordSet.GetFieldValue("bak", nTemp);
			sysSettingGetDownPack.gDataBodyPack.nBak = nTemp;

			m_adoRecordSet.GetFieldValue("bak1", nTemp);
			sysSettingGetDownPack.gDataBodyPack.nBak1 = nTemp;

			m_adoRecordSet.GetFieldValue("WarnTime", dTemp);
			sysSettingGetDownPack.gDataBodyPack.fWarnTime = (float)dTemp;

			m_adoRecordSet.GetFieldValue("FBak", dTemp);
			sysSettingGetDownPack.gDataBodyPack.fBak = (float)dTemp;

			m_adoRecordSet.GetFieldValue("cData", strTemp);
			sysSettingGetDownPack.gDataBodyPack.strData = strTemp.GetBuffer();
			strTemp.LockBuffer();

			m_adoRecordSet.GetFieldValue("cData1", strTemp);
			sysSettingGetDownPack.gDataBodyPack.strData1 = strTemp.GetBuffer();
			strTemp.LockBuffer();

			sysSettingGetDownPack.nBodyLength = 120;
			return true;
		}

ErrorDeal:
		sysSettingGetDownPack.nBeginIdentify = sysSettingGetUpPack.nBeginIdentify;
		sysSettingGetDownPack.nMsgNumber = sysSettingGetUpPack.nMsgNumber;

		sysSettingGetDownPack.gDataBodyPack.nDataBak = 0;
		sysSettingGetDownPack.gDataBodyPack.nBak = 0;
		sysSettingGetDownPack.gDataBodyPack.nBak1 = 0;
		sysSettingGetDownPack.gDataBodyPack.fWarnTime = 0;
		sysSettingGetDownPack.gDataBodyPack.fBak = 0;
		sysSettingGetDownPack.gDataBodyPack.strData = "";
		sysSettingGetDownPack.gDataBodyPack.strData1 = "";

		sysSettingGetDownPack.nBodyLength = 120;
		return true;
	}
	return false;
}

bool CAccessBaseData::UpLoadModifyPassPack(const ModifyPassword_Upload_Pack& modifyPassUpPack,
	ModifyPassword_Download_Pack& modifyPassDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update t_user set User_Pawd='%s' where User_Oper='%s'",
			modifyPassUpPack.gDataBodyPack.strPassword.c_str(),
			modifyPassUpPack.gDataBodyPack.strOper.c_str());

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}
		}

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			nErrorCode = 1;
			goto ErrorDeal;
		}

		if(m_pDatabase->GetRecordsAffected() > 0)
			nErrorCode = 0;
		else
			nErrorCode = 1;

ErrorDeal:
		modifyPassDownPack.nBeginIdentify = modifyPassUpPack.nBeginIdentify;
		modifyPassDownPack.nMsgNumber = modifyPassUpPack.nMsgNumber;
		modifyPassDownPack.gDataBodyPack.nResult = nErrorCode;

		modifyPassDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}

CADORecordset* CAccessBaseData::UpLoadWorkWramPointPack(const WorkWramPoint_Upload_Pack& workWramPointUpPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;

		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "select tl_name,pit_dis,r_userid,r_arrtime,r_realtime,r_pxdate,p_state from v_userPointLine where r_orgid = %d and year(r_pxdate) = %d and month(r_pxdate)=%d",
			workWramPointUpPack.gDataBodyPack.nOrgID,
			workWramPointUpPack.gDataBodyPack.nYear,
			workWramPointUpPack.gDataBodyPack.nMonth);

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				return NULL;
			}
		}

		CADORecordset* pRecordset = new CADORecordset(m_pDatabase);
		if(pRecordset->Open(m_strBuffer))
		{
			return pRecordset;
		}
	}
	return NULL;
}


bool CAccessBaseData::UpLoadOpteLinePack(const OpteLine_Upload_Pack& opteLineUpPack,
	OpteLine_Download_Pack& opteLineDownPack)
{
	if(m_pDatabase)
	{
		int nErrorCode = 1;
		memset(m_strBuffer, 0x00, INPUTSQLBUFFERS);
		switch(opteLineUpPack.gDataBodyPack.nType)
		{
		case 0:
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "insert into t_tlname(TL_Name) values('%s')",
					opteLineUpPack.gDataBodyPack.strName.c_str());
			}
			break;
		case 1:
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "update t_tlname set TL_Name='%s' where TL_id=%d",
					opteLineUpPack.gDataBodyPack.strName.c_str(),
					opteLineUpPack.gDataBodyPack.nID);
			}
			break;
		case 2:
			{
				sprintf_s(m_strBuffer, INPUTSQLBUFFERS, "delete t_tlname where TL_id=%d",
					opteLineUpPack.gDataBodyPack.nID);
			}
			break;
		default:
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}
			break;
		}

		if(!m_pDatabase->IsOpen())
		{
			if(!m_pDatabase->Open())
			{
				nErrorCode = 1;
				goto ErrorDeal;
			}
		}

		if(!m_pDatabase->Execute(m_strBuffer))
		{
			nErrorCode = 1;
			goto ErrorDeal;
		}
		if(m_pDatabase->GetRecordsAffected() > 0)
			nErrorCode = 0;
		else
			nErrorCode = 1;

ErrorDeal:
		opteLineDownPack.nBeginIdentify = opteLineUpPack.nBeginIdentify;
		opteLineDownPack.nMsgNumber = opteLineUpPack.nMsgNumber;
		opteLineDownPack.gDataBodyPack.nResult = nErrorCode;

		opteLineDownPack.nBodyLength = 4;
		return true;
	}
	return false;
}


string CAccessBaseData::GetCurrentTimeByFormat()
{
#define  TIMEBUFFER	32

	SYSTEMTIME systime;
	char strTimeBuffer[TIMEBUFFER] = {0};

	GetLocalTime(&systime);
	sprintf_s(strTimeBuffer, TIMEBUFFER, "%.04d-%.02d-%.02d %.02d:%.02d:%.02d",//"%.04d%.02d%.02d%.02d%.02d%.02d",
		systime.wYear,
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond);

	return strTimeBuffer;
}