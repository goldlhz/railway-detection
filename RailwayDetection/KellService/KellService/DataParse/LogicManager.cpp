#include "StdAfx.h"
#include "CommonFunction.h"
#include "base64.h"
#include "GobalConfig.h"
#include "LogicManager.h"

typedef int (*FPDealSendData)(LPOverKeyPire pKeyOverPire, void* parameter);
typedef int (*FPDealRecvData)(LPOverKeyPire pKeyOverPire, void* parameter, string& strContext);

CLogicManager::CLogicManager(void)
{
}

CLogicManager::~CLogicManager(void)
{
}

int CLogicManager::SetLogicData(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunSendData,
	void* pFunRecvData,
	CGobalConfig* pGobalConfig)
{
	ASSERT(pKeyOverPire);
	ASSERT(pDatabase);
		
	int   nBackCode = -1;
	if(pKeyOverPire && pDatabase)
	{
		char* pcBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;
		char  cMsgNum = m_DataPackPares.ParsePackMsgNum(pcBuffer);

		switch(cMsgNum)
		{
		case GPS_PACK:
			nBackCode = DealGPSPack(dNumberOfBytes, 
				pKeyOverPire, 
				pDatabase,
				pWorkThread, 
				pFunSendData,
				pFunRecvData,
				pGobalConfig);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case LOGIN_PACK:
			nBackCode = DealLoginPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case ADDORG_PACK:
			nBackCode = DealAddOrgPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case MODIFYORG_PACK:
			nBackCode = DealModifyOrgPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case DELETEORG_PACK:
			nBackCode = DealDeleteOrgPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case ASKORGLIST_PACK:
			nBackCode = DealAskOrgListPack(dNumberOfBytes, 
				pKeyOverPire, 
				pDatabase,
				pWorkThread, 
				pFunSendData);
			break;

		case ADDWORKER_PACK:
			nBackCode = DealAddWorkerPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case MODIFYWORKER_PACK:
			nBackCode = DealModifyWorkerPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case DELETEWORKER_PACK:
			nBackCode = DealDeleteWorkerPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;
			
		case ASKWORKERLIST_PACK:
			nBackCode = DealAskWorkerListPack(dNumberOfBytes, 
				pKeyOverPire, 
				pDatabase,
				pWorkThread, 
				pFunSendData);
			break;

		case ADDPOWERGROUP_PACK:
			nBackCode = DealAddPowerGroupPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case MODIFYPOWERGROUP_PACK:
			nBackCode = DealModifyPowerGroupPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case DELETEPOWERGROUP_PACK:
			nBackCode = DealDeletePowerGroupPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case ASKPOWERGROUPLIST_PACK:
			nBackCode = DealAskPowerGroupListPack(dNumberOfBytes, 
				pKeyOverPire, 
				pDatabase,
				pWorkThread, 
				pFunSendData);
			break;

		case ADDDEVICE_PACK:
			nBackCode = DealAddDevicePack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case MODIFYDEVICE_PACK:
			nBackCode = DealModifyDevicePack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case DELETEDEVICE_PACK:
			nBackCode = DealDeleteDevicePack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case ASKDEIVCELIST_PACK:
			nBackCode = DealAskDeviceListPack(dNumberOfBytes, 
				pKeyOverPire, 
				pDatabase,
				pWorkThread, 
				pFunSendData);
			break;

		case GETGPSPOINT_PACK:
			nBackCode = DealGetGPSPointPack(dNumberOfBytes, 
				pKeyOverPire, 
				pDatabase,
				pWorkThread, 
				pFunSendData);
			break;

		case GETRAILWAYTABLENAME_PACK:
			nBackCode = DealGetRailWayNamePack(dNumberOfBytes, 
				pKeyOverPire, 
				pDatabase,
				pWorkThread, 
				pFunSendData);
			break;

		case SETTINGLINE_PACK:
			nBackCode = DealSettingLinePack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case SETTINGPOLLINGTIME_PACK:
			nBackCode = DealSettingPollingTimePack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case SETTINGSCHEDULE5_PACK:
			nBackCode = DealSettingSchedule5Pack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case SETTINGSCHEDULE6_PACK:
			nBackCode = DealSettingSchedule6Pack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case SETTINGSCHEDULEWORKER_PACK:
			nBackCode = DealSettingScheduleWorkerPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case URGENCYRELEASE_PACK:
			nBackCode = DealUrgencyReleasePack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case URGENCYWORKER_PACK:
			nBackCode = DealUrgencyWorkerPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case GETLINEPOINT_PACK:
			nBackCode = DealGetLinePointPack(dNumberOfBytes, pKeyOverPire, pDatabase, pWorkThread, pFunSendData);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case GETPICLIST_PACK:
			nBackCode = DealGetPicListPack(dNumberOfBytes, pKeyOverPire, pDatabase, pWorkThread, pFunSendData);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case GETURGENCYMISSIONLIST_PACK:
			nBackCode = DealGetUrgencyMissionListPack(dNumberOfBytes, pKeyOverPire, pDatabase, pWorkThread, pFunSendData);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case GETURGENCYWORKERLIST_PACK:
			nBackCode = DealGetUrgencyWorkerListPack(dNumberOfBytes, pKeyOverPire, pDatabase, pWorkThread, pFunSendData);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case GETPICDATA_PACK:
			nBackCode = DealGetPicDataPack(dNumberOfBytes, pKeyOverPire, pDatabase, pWorkThread, pFunSendData, pGobalConfig);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case GETORGSCHEDUELIST_PACK:
			nBackCode = DealGetSchedueListPack(dNumberOfBytes, pKeyOverPire, pDatabase, pWorkThread, pFunSendData);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case GETORGSCHEDUEINFO_PACK:
			nBackCode = DealGetSchedueInfoPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case SETPOINT_PACK:
			nBackCode = DealSetPointPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case GETORGLINE_PACK:
			nBackCode =  DealGetOrgLinePack(dNumberOfBytes, 
				pKeyOverPire, 
				pDatabase,
				pWorkThread,
				pFunSendData);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case GETLINEPOINTTIME_PACK:
			nBackCode =  DealGetLinePointTimePack(dNumberOfBytes, 
				pKeyOverPire, 
				pDatabase,
				pWorkThread,
				pFunSendData);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case GETORGSCHWORKER_PACK:
			nBackCode =  DealGetOrgSchWorkerPack(dNumberOfBytes, 
				pKeyOverPire, 
				pDatabase,
				pWorkThread,
				pFunSendData);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case WORKERPOLLQUERY_PACK:
			nBackCode =  DealWorkerPollQueryPack(dNumberOfBytes, 
				pKeyOverPire, 
				pDatabase,
				pWorkThread,
				pFunSendData);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case URGENCYMISSIONDELETE_PACK:
			nBackCode = DealUrgencyMissionDeletePack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case WORKERPOLL_PACK:
			nBackCode =  DealWorkerPollPack(dNumberOfBytes, 
				pKeyOverPire, 
				pDatabase,
				pWorkThread,
				pFunSendData);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case SYSSETTING_PACK:
			nBackCode = DealSysSettingPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case SYSSETTINGGET_PACK:
			nBackCode = DealSysSettingGetPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case MODIFYPASS_PACK:
			nBackCode = DealModifyPassPack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case WORKERWARNPOINT_PACK:
			nBackCode = DealWorkWramPointPack(dNumberOfBytes, 
				pKeyOverPire, 
				pDatabase,
				pWorkThread,
				pFunSendData);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;

		case OPTELINE_PACK:
			nBackCode = DealOpteLinePack(dNumberOfBytes, pcBuffer, pDatabase);
			pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBackCode;
			break;
		}
	}
	return nBackCode;
}

int  CLogicManager::DealGPSPack(DWORD dNumberOfBytes, 
	 LPOverKeyPire pKeyOverPire, 
	 CADODatabase* pDatabase,
	 void* pWorkThread,
	 void* pFunSendData,
	 void* pFunRecvData,
	 CGobalConfig* pGobalConfig)
{
	// 即使发上来的包完全错的
	// 我也要返回一个包回去
	int nFlag = 1;
	DWORD nBodyLength;
	string strGPSContext;
	string strTel;

	FPDealRecvData DealRecvData = (FPDealRecvData)pFunRecvData;
	FPDealSendData DealSendData = (FPDealSendData)pFunSendData;
	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;

	if(m_DataPackPares.PackGPSUpParse(pBuffer, m_gpsUpPack, nBodyLength, strGPSContext, strTel))
	{
		if(strGPSContext.empty())
		{
			if(m_AccessBaseData.InitAccesser(pDatabase))
			{
				m_AccessBaseData.UploadGPSPack(m_gpsUpPack, m_gpsDownPack);
			}
		}
		else
		{
			while(strGPSContext.length() < nBodyLength)
			{
				nFlag = DealRecvData(pKeyOverPire, pWorkThread, strGPSContext);
				if(nFlag != 0)
					break;
			}

			if(0 == nFlag)
			{
				int    nType = 0;
				string strDestGPSContext;
				string strDestGPSPicContext;
				strDestGPSContext = base64_decode(strGPSContext);

				if(m_DataPackPares.PackGPSUpPicParse(strDestGPSContext, strDestGPSPicContext, nType))
				{
					if(m_AccessBaseData.InitAccesser(pDatabase))
					{
						string strPicName;
						GPSPIC_Pack gpsPicUpLoadPack;

						FillPicStoreStruct(gpsPicUpLoadPack, strTel, nType);
						strPicName = pGobalConfig->GetPicFilePath() + "\\" + gpsPicUpLoadPack.strPicName.substr(0, 8);//gpsPicUpLoadPack.strTime;

						if(BuildPicDir(strPicName))
						{
							if(m_AccessBaseData.UpLoadGPSPICPack(gpsPicUpLoadPack))
							{
								strPicName = strPicName + "\\" + gpsPicUpLoadPack.strPicName;
								FILE* fPic = fopen(strPicName.c_str(), "wb");
								if(fPic)
								{
									fwrite(strDestGPSPicContext.c_str(), strDestGPSPicContext.length(), 1, fPic);
									fclose(fPic);
								}
							}
						}
					}
				}
			}
		}
	}

	nBodyLength = m_DataPackPares.PackGPSDownBuild(pBuffer, m_gpsDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = nBodyLength;
	DealSendData(pKeyOverPire, pWorkThread);

	return -2;
}

int  CLogicManager::DealLoginPack(DWORD dNumberOfBytes, 
	 char  * pBuffer, 
	 CADODatabase* pDatabase)
{
	// 即使发上来的包完全错的
	// 我也要返回一个包回去
	if(m_DataPackPares.PackLoginUpParse(pBuffer, m_loginUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UploadLoginPack(m_loginUpPack, m_loginDownPack);
			m_DataPackPares.PackLoginDownBuild(pBuffer, m_loginDownPack);

			return m_loginDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillLoginFailPack(m_loginDownPack);
	m_DataPackPares.PackLoginDownBuild(pBuffer, m_loginDownPack);
	return m_loginDownPack.nBodyLength + 11;
}

int  CLogicManager::DealAddOrgPack(DWORD dNumberOfBytes, 
	 char  * pBuffer, 
	 CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackAddOrgUpParse(pBuffer, m_addOrgUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UploadAddOrgPack(m_addOrgUpPack, m_addOrgDownPack);
			m_DataPackPares.PackAddOrgDownBuild(pBuffer, m_addOrgDownPack);
			return m_addOrgDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillAddOrgFailPack(m_addOrgDownPack);
	m_DataPackPares.PackAddOrgDownBuild(pBuffer, m_addOrgDownPack);
	return m_addOrgDownPack.nBodyLength + 11;
}

int  CLogicManager::DealModifyOrgPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackModifyOrgUpParse(pBuffer, m_modifyOrgUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UploadModifyOrgPack(m_modifyOrgUpPack, m_modifyOrgDownPack);
			m_DataPackPares.PackModifyOrgDownBuild(pBuffer, m_modifyOrgDownPack);

			return m_modifyOrgDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillModifyOrgFailPack(m_modifyOrgDownPack);
	m_DataPackPares.PackModifyOrgDownBuild(pBuffer, m_modifyOrgDownPack);

	return m_modifyOrgDownPack.nBodyLength + 11;
}

int  CLogicManager::DealDeleteOrgPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackDeleteOrgUpParse(pBuffer, m_deleteOrgUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UploadDeleteOrgPack(m_deleteOrgUpPack, m_deleteOrgDownPack);
			m_DataPackPares.PackDeleteOrgDownBuild(pBuffer, m_deleteOrgDownPack);

			return m_deleteOrgDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillDeleteOrgFailPack(m_deleteOrgDownPack);
	m_DataPackPares.PackDeleteOrgDownBuild(pBuffer, m_deleteOrgDownPack);

	return m_deleteOrgDownPack.nBodyLength + 11;
}

int  CLogicManager::DealAskOrgListPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;
	if(m_DataPackPares.PackAskOrgListUpParse(pBuffer, m_askOrgListUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UploadAskOrgListPack(m_askOrgListUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_askOrgListDownPack.gDataBodyPack.nTitlePack = nTitleRecord;
						m_askOrgListDownPack.gDataBodyPack.nHadSendPack = nHadSendRecord;

						pRecordset->GetFieldValue("Org_Id", nTemp);
						m_askOrgListDownPack.gDataBodyPack.nOrgID = nTemp;

						pRecordset->GetFieldValue("Org_name", strTemp);
						m_askOrgListDownPack.gDataBodyPack.strOrgName = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("Org_Add", strTemp);
						m_askOrgListDownPack.gDataBodyPack.strAddress = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("Org_Upid", nTemp);
						m_askOrgListDownPack.gDataBodyPack.nUpOrgID = nTemp;

						pRecordset->GetFieldValue("Org_Lev", nTemp);
						m_askOrgListDownPack.gDataBodyPack.nLevel = nTemp;

						pRecordset->GetFieldValue("org_lineid", nTemp);
						m_askOrgListDownPack.gDataBodyPack.nLineId = nTemp;

						pRecordset->GetFieldValue("org_startid", nTemp);
						m_askOrgListDownPack.gDataBodyPack.nStartid = nTemp;

						pRecordset->GetFieldValue("org_endid", nTemp);
						m_askOrgListDownPack.gDataBodyPack.nEndID = nTemp;

						m_askOrgListDownPack.nBodyLength = 82;
						m_DataPackPares.PackAskOrgListDownBuild(pBuffer, m_askOrgListDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_askOrgListDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}

	m_DataPackPares.FillAskOrgListFailPack(m_askOrgListDownPack);
	m_DataPackPares.PackAskOrgListDownBuild(pBuffer, m_askOrgListDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_askOrgListDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}

int  CLogicManager::DealAddWorkerPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackAddWorkerUpParse(pBuffer, m_addWorkerUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UploadAddWorkerPack(m_addWorkerUpPack, m_addWorkerDownPack);
			m_DataPackPares.PackAddWorkerDownBuild(pBuffer, m_addWorkerDownPack);

			return m_addWorkerDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillAddWorkerFailPack(m_addWorkerDownPack);
	m_DataPackPares.PackAddWorkerDownBuild(pBuffer, m_addWorkerDownPack);

	return m_addWorkerDownPack.nBodyLength + 11;
}

int  CLogicManager::DealModifyWorkerPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackModifyWorkerUpParse(pBuffer, m_modifyWorkerUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UploadModifyWorkerPack(m_modifyWorkerUpPack, m_modifyWorkerDownPack);
			m_DataPackPares.PackModifyWorkerDownBuild(pBuffer, m_modifyWorkerDownPack);

			return m_modifyWorkerDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillModifyWorkerFailPack(m_modifyWorkerDownPack);
	m_DataPackPares.PackModifyWorkerDownBuild(pBuffer, m_modifyWorkerDownPack);

	return m_modifyWorkerDownPack.nBodyLength + 11;
}

int  CLogicManager::DealDeleteWorkerPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackDeleteWorkerUpParse(pBuffer, m_deleteWorkerUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UploadDeleteWorkerPack(m_deleteWorkerUpPack, m_deleteWorkerDownPack);
			m_DataPackPares.PackDeleteWorkerDownBuild(pBuffer, m_deleteWorkerDownPack);

			return m_deleteWorkerDownPack.nBodyLength + 11;
		}
	}
	
	m_DataPackPares.FillDeleteWorkerFailPack(m_deleteWorkerDownPack);
	m_DataPackPares.PackDeleteWorkerDownBuild(pBuffer, m_deleteWorkerDownPack);

	return m_deleteWorkerDownPack.nBodyLength + 11;
}

int  CLogicManager::DealAskWorkerListPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;
	if(m_DataPackPares.PackAskWorkerListUpParse(pBuffer, m_askWorkerListUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UploadAskWorkerListPack(m_askWorkerListUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_askWorkerListDownPack.gDataBodyPack.nTitlePack = nTitleRecord;
						m_askWorkerListDownPack.gDataBodyPack.nHadSendPack = nHadSendRecord;

						pRecordset->GetFieldValue("User_Oper", strTemp);
						m_askWorkerListDownPack.gDataBodyPack.strOper = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("User_Name", strTemp);
						m_askWorkerListDownPack.gDataBodyPack.strName = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("User_Org", nTemp);
						m_askWorkerListDownPack.gDataBodyPack.nOrgID = nTemp;

						pRecordset->GetFieldValue("User_PGroup", nTemp);
						m_askWorkerListDownPack.gDataBodyPack.nPowerGroup = nTemp;

						pRecordset->GetFieldValue("User_Pda1", nTemp);
						m_askWorkerListDownPack.gDataBodyPack.nPda1 = nTemp;

						pRecordset->GetFieldValue("User_Pda2", nTemp);
						m_askWorkerListDownPack.gDataBodyPack.nPda2 = nTemp;

						pRecordset->GetFieldValue("User_State", nTemp);
						m_askWorkerListDownPack.gDataBodyPack.nWorkerState = nTemp;

						m_askWorkerListDownPack.nBodyLength = 88;
						m_DataPackPares.PackAskWorkerListDownBuild(pBuffer, m_askWorkerListDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_askWorkerListDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}

					return 0;
				}
			}
		}
	}

	m_DataPackPares.FillAskWorkerListFailPack(m_askWorkerListDownPack);
	m_DataPackPares.PackAskWorkerListDownBuild(pBuffer, m_askWorkerListDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_askWorkerListDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}

int  CLogicManager::DealAddPowerGroupPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackAddPowerGroupUpParse(pBuffer, m_addPowerGroupUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UploadAddPowerGroupPack(m_addPowerGroupUpPack, m_addPowerGroupDownPack);
			m_DataPackPares.PackAddPowerGroupDownBuild(pBuffer, m_addPowerGroupDownPack);

			return m_addPowerGroupDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillAddPowerGroupFailPack(m_addPowerGroupDownPack);
	m_DataPackPares.PackAddPowerGroupDownBuild(pBuffer, m_addPowerGroupDownPack);

	return m_addPowerGroupDownPack.nBodyLength + 11;
}

int  CLogicManager::DealModifyPowerGroupPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackModifyPowerGroupUpParse(pBuffer, m_modifyPowerGroupUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UploadModifyPowerGroupPack(m_modifyPowerGroupUpPack, m_modifyPowerGroupDownPack);
			m_DataPackPares.PackModifyPowerGroupDownBuild(pBuffer, m_modifyPowerGroupDownPack);

			return m_modifyPowerGroupDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillModifyPowerGroupFailPack(m_modifyPowerGroupDownPack);
	m_DataPackPares.PackModifyPowerGroupDownBuild(pBuffer, m_modifyPowerGroupDownPack);

	return m_modifyPowerGroupDownPack.nBodyLength + 11;
}

int  CLogicManager::DealDeletePowerGroupPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackDeletePowerGroupUpParse(pBuffer, m_deletePowerGroupUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UploadDeletePowerGroupPack(m_deletePowerGroupUpPack, m_deletePowerGroupDownPack);
			m_DataPackPares.PackDeletePowerGroupDownBuild(pBuffer, m_deletePowerGroupDownPack);

			return m_deletePowerGroupDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillDeletePowerGroupFailPack(m_deletePowerGroupDownPack);
	m_DataPackPares.PackDeletePowerGroupDownBuild(pBuffer, m_deletePowerGroupDownPack);

	return m_deletePowerGroupDownPack.nBodyLength + 11;
}

int  CLogicManager::DealAskPowerGroupListPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;
	if(m_DataPackPares.PackAskPowerGroupListUpParse(pBuffer, m_askPowerGroupListUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UploadAskPowerGroupListPack(m_askPowerGroupListUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_askPowerGroupListDownPack.gDataBodyPack.nTitlePack = nTitleRecord;
						m_askPowerGroupListDownPack.gDataBodyPack.nHadSendPack = nHadSendRecord;

						pRecordset->GetFieldValue("Power_Id", nTemp);
						m_askPowerGroupListDownPack.gDataBodyPack.nPowerID = nTemp;

						pRecordset->GetFieldValue("Power_name", strTemp);
						m_askPowerGroupListDownPack.gDataBodyPack.strPowerName = strTemp.GetBuffer();
						strTemp.LockBuffer();
						
						pRecordset->GetFieldValue("Group1", nTemp);
						m_askPowerGroupListDownPack.gDataBodyPack.nPow1 = nTemp;

						pRecordset->GetFieldValue("Group2", nTemp);
						m_askPowerGroupListDownPack.gDataBodyPack.nPow2 = nTemp;

						pRecordset->GetFieldValue("Group3", nTemp);
						m_askPowerGroupListDownPack.gDataBodyPack.nPow3 = nTemp;

						m_askPowerGroupListDownPack.nBodyLength = 44;
						m_DataPackPares.PackAskPowerGroupListDownBuild(pBuffer, m_askPowerGroupListDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_askPowerGroupListDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}

	m_DataPackPares.FillAskPowerGroupListFailPack(m_askPowerGroupListDownPack);
	m_DataPackPares.PackAskPowerGroupListDownBuild(pBuffer, m_askPowerGroupListDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_askPowerGroupListDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}

int  CLogicManager::DealAddDevicePack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackAddDeviceUpParse(pBuffer, m_addDeviceUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UploadAddDevicePack(m_addDeviceUpPack, m_addDeviceDownPack);
			m_DataPackPares.PackAddDeviceDownBuild(pBuffer, m_addDeviceDownPack);

			return m_addDeviceDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillAddDeviceFailPack(m_addDeviceDownPack);
	m_DataPackPares.PackAddDeviceDownBuild(pBuffer, m_addDeviceDownPack);

	return m_addDeviceDownPack.nBodyLength + 11;
}

int  CLogicManager::DealModifyDevicePack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackModifyDeviceUpParse(pBuffer, m_modifyDeviceUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UploadModifyDevicePack(m_modifyDeviceUpPack, m_modifyDeviceDownPack);
			m_DataPackPares.PackModifyDeviceDownBuild(pBuffer, m_modifyDeviceDownPack);

			return m_modifyDeviceDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillModifyDeviceFailPack(m_modifyDeviceDownPack);
	m_DataPackPares.PackModifyDeviceDownBuild(pBuffer, m_modifyDeviceDownPack);

	return m_modifyDeviceDownPack.nBodyLength + 11;
}

int  CLogicManager::DealDeleteDevicePack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackDeleteDeviceUpParse(pBuffer, m_deleteDeviceUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UploadDeleteDevicePack(m_deleteDeviceUpPack, m_deleteDeviceDownPack);
			m_DataPackPares.PackDeleteDeviceDownBuild(pBuffer, m_deleteDeviceDownPack);

			return m_deleteDeviceDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillModifyDeviceFailPack(m_modifyDeviceDownPack);
	m_DataPackPares.PackDeleteDeviceDownBuild(pBuffer, m_deleteDeviceDownPack);

	return m_deleteDeviceDownPack.nBodyLength + 11;
}


int  CLogicManager::DealAskDeviceListPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;
	if(m_DataPackPares.PackAskDeviceListUpParse(pBuffer, m_askDeviceUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UploadAskDeviceListPack(m_askDeviceUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;
				
					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_askDeviceDownPack.gDataBodyPack.nTitlePack = nTitleRecord;
						m_askDeviceDownPack.gDataBodyPack.nHadSendPack = nHadSendRecord;

						pRecordset->GetFieldValue("Device_id", nTemp);
						m_askDeviceDownPack.gDataBodyPack.nDeviceID = nTemp;

						pRecordset->GetFieldValue("Device_Pid", strTemp);
						m_askDeviceDownPack.gDataBodyPack.strDevicePID = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("Device_Org", nTemp);
						m_askDeviceDownPack.gDataBodyPack.nDeviceOrg = nTemp;

						pRecordset->GetFieldValue("Device_Type", nTemp);
						m_askDeviceDownPack.gDataBodyPack.nDeviceType = nTemp;

						m_askDeviceDownPack.nBodyLength = 40;
						m_DataPackPares.PackAskDeviceListDownBuild(pBuffer, m_askDeviceDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_askDeviceDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}

	m_DataPackPares.FillAskDeviceListFailPack(m_askDeviceDownPack);
	m_DataPackPares.PackAskDeviceListDownBuild(pBuffer, m_askDeviceDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_askOrgListDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}

int  CLogicManager::DealGetGPSPointPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;

	if(m_DataPackPares.PackGetGPSPointUpParse(pBuffer, m_getGPSPointUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UpLoadGetGPSPointPack(m_getGPSPointUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord = 0;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;

					pRecordset->MoveFirst();

					for (int nIndex0 = 0; nIndex0 < (int)(nTitleRecord / 10); ++nIndex0)
					{
						m_getGPSPointDownPack.gDataBodyPack.vecGPS.clear();						
						m_getGPSPointDownPack.gDataBodyPack.nTotlePack = nTitleRecord;
						m_getGPSPointDownPack.gDataBodyPack.nHadSendPack = nHadSendRecord;

						nHadSendRecord += 10;
						for (int nIndex1 = 0; nIndex1 < 10 ; ++nIndex1, pRecordset->MoveNext())
						{
							if(!pRecordset->IsEOF())
							{
								SGPS sgps;

								pRecordset->GetFieldValue("G_time", strTemp);
								sgps.strTime = strTemp.GetBuffer();
								strTemp.LockBuffer();

								pRecordset->GetFieldValue("G_tel", strTemp);
								sgps.strTel = strTemp.GetBuffer();
								strTemp.LockBuffer();

								pRecordset->GetFieldValue("G_j", nTemp);
								sgps.strJDU = CCommonFunction::IntToString(nTemp * 10000000);

								pRecordset->GetFieldValue("G_w", nTemp);
								sgps.strWDU = CCommonFunction::IntToString(nTemp * 10000000);

								m_getGPSPointDownPack.gDataBodyPack.vecGPS.push_back(sgps);
							}
						}

						m_getGPSPointDownPack.nBodyLength = 98;
						m_DataPackPares.PackGetGPSPointDownBuild(pBuffer, m_getGPSPointDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getGPSPointDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}

					if(!pRecordset->IsEOF())
					{
						m_getGPSPointDownPack.gDataBodyPack.vecGPS.clear();						
						m_getGPSPointDownPack.gDataBodyPack.nTotlePack = nTitleRecord;
						m_getGPSPointDownPack.gDataBodyPack.nHadSendPack = nHadSendRecord;

						while (!pRecordset->IsEOF())
						{
							SGPS sgps;

							pRecordset->GetFieldValue("G_time", strTemp);
							sgps.strTime = strTemp.GetBuffer();
							strTemp.LockBuffer();

							pRecordset->GetFieldValue("G_tel", strTemp);
							sgps.strTel = strTemp.GetBuffer();
							strTemp.LockBuffer();

							pRecordset->GetFieldValue("G_j", nTemp);
							sgps.strJDU = CCommonFunction::IntToString(nTemp * 10000000);

							pRecordset->GetFieldValue("G_w", nTemp);
							sgps.strWDU = CCommonFunction::IntToString(nTemp * 10000000);

							m_getGPSPointDownPack.gDataBodyPack.vecGPS.push_back(sgps);
						}

						m_getGPSPointDownPack.nBodyLength = 98;
						m_DataPackPares.PackGetGPSPointDownBuild(pBuffer, m_getGPSPointDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getGPSPointDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}	
					return 0;
				}
			}
		}
	}
	m_DataPackPares.FillGetGPSPointFailPack(m_getGPSPointDownPack);
	m_DataPackPares.PackGetGPSPointDownBuild(pBuffer, m_getGPSPointDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_askOrgListDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}


int  CLogicManager::DealGetRailWayNamePack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;
	if(m_DataPackPares.PackGetRailWayNameUpParse(pBuffer, m_getRailWayNameUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UpLoadGetRailWayNamePack(m_getRailWayNameUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_getRailWayNameDownPack.gDataBodyPack.nTotlePack = nTitleRecord;
						m_getRailWayNameDownPack.gDataBodyPack.nHadSendPack = nHadSendRecord;
						
						pRecordset->GetFieldValue("TL_id", nTemp);
						m_getRailWayNameDownPack.gDataBodyPack.nID = nTemp;

						pRecordset->GetFieldValue("TL_Name", strTemp);
						m_getRailWayNameDownPack.gDataBodyPack.strnName = strTemp.GetBuffer();
						strTemp.LockBuffer();

						m_getRailWayNameDownPack.nBodyLength = 32;
						m_DataPackPares.PackGetRailWayNameDownBuild(pBuffer, m_getRailWayNameDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getRailWayNameDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}

	m_DataPackPares.FillGetRailWayNameFailPack(m_getRailWayNameDownPack);
	m_DataPackPares.PackGetRailWayNameDownBuild(pBuffer, m_getRailWayNameDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getRailWayNameDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}


int  CLogicManager::DealSettingLinePack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackSettingLineUpParse(pBuffer, m_settingLinkUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UpLoadSettingLinePack(m_settingLinkUpPack, m_settingLinkDownPack);
			m_DataPackPares.PackSettingLineDownBuild(pBuffer, m_settingLinkDownPack);

			return m_settingLinkDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillSettingLineFailPack(m_settingLinkDownPack);
	m_DataPackPares.PackSettingLineDownBuild(pBuffer, m_settingLinkDownPack);

	return m_settingLinkDownPack.nBodyLength + 11;
}


int  CLogicManager::DealSettingPollingTimePack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackSettingPollingTimeUpParse(pBuffer, m_settingPollingTimeUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UpLoadSettingPollingTimePack(m_settingPollingTimeUpPack, m_settingPollingTimeDownPack);
			m_DataPackPares.PackSettingPollingTimeDownBuild(pBuffer, m_settingPollingTimeDownPack);

			return m_settingPollingTimeDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillSettingPollingTimeFailPack(m_settingPollingTimeDownPack);
	m_DataPackPares.PackSettingPollingTimeDownBuild(pBuffer, m_settingPollingTimeDownPack);

	return m_settingPollingTimeDownPack.nBodyLength + 11;
}


int  CLogicManager::DealSettingSchedule5Pack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackSettingSchedule5UpParse(pBuffer, m_settingSchedule5UpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UpLoadSettingSchedule5Pack(m_settingSchedule5UpPack, m_settingSchedule5DownPack);
			m_DataPackPares.PackSettingSchedule5DownBuild(pBuffer, m_settingSchedule5DownPack);

			return m_settingSchedule5DownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillSettingSchedule5FailPack(m_settingSchedule5DownPack);
	m_DataPackPares.PackSettingSchedule5DownBuild(pBuffer, m_settingSchedule5DownPack);

	return m_settingSchedule5DownPack.nBodyLength + 11;
}


int  CLogicManager::DealSettingSchedule6Pack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackSettingSchedule6UpParse(pBuffer, m_settingSchedule6UpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UpLoadSettingSchedule6Pack(m_settingSchedule6UpPack, m_settingSchedule6DownPack);
			m_DataPackPares.PackSettingSchedule6DownBuild(pBuffer, m_settingSchedule6DownPack);

			return m_settingSchedule6DownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillSettingSchedule6FailPack(m_settingSchedule6DownPack);
	m_DataPackPares.PackSettingSchedule6DownBuild(pBuffer, m_settingSchedule6DownPack);

	return m_settingSchedule6DownPack.nBodyLength + 11;
}


int  CLogicManager::DealSettingScheduleWorkerPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackSettingScheduleWorkerUpParse(pBuffer, m_settingScheduleWorkerUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UpLoadSettingScheduleWorkerPack(m_settingScheduleWorkerUpPack, m_settingScheduleWorkerDownPack);
			m_DataPackPares.PackSettingScheduleWorkerDownBuild(pBuffer, m_settingScheduleWorkerDownPack);

			return m_settingScheduleWorkerDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillSettingScheduleWorkerFailPack(m_settingScheduleWorkerDownPack);
	m_DataPackPares.PackSettingScheduleWorkerDownBuild(pBuffer, m_settingScheduleWorkerDownPack);

	return m_settingScheduleWorkerDownPack.nBodyLength + 11;
}

int  CLogicManager::DealUrgencyReleasePack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackUrgencyReleaseUpParse(pBuffer, m_urgencyReleaseUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UpLoadUrgencyReleasePack(m_urgencyReleaseUpPack, m_urgencyReleaseDownPack);
			m_DataPackPares.PackUrgencyReleaseDownBuild(pBuffer, m_urgencyReleaseDownPack);

			return m_urgencyReleaseDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillUrgencyReleaseFailPack(m_urgencyReleaseDownPack);
	m_DataPackPares.PackUrgencyReleaseDownBuild(pBuffer, m_urgencyReleaseDownPack);

	return m_urgencyReleaseDownPack.nBodyLength + 11;
}


int  CLogicManager::DealUrgencyWorkerPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackUrgencyWorkerUpParse(pBuffer, m_urgencyWorkerUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UpLoadUrgencyWorkerPack(m_urgencyWorkerUpPack, m_urgencyWorkerDownPack);
			m_DataPackPares.PackUrgencyWorkerDownBuild(pBuffer, m_urgencyWorkerDownPack);

			return m_urgencyWorkerDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillUrgencyWorkerFailPack(m_urgencyWorkerDownPack);
	m_DataPackPares.PackUrgencyWorkerDownBuild(pBuffer, m_urgencyWorkerDownPack);

	return m_urgencyWorkerDownPack.nBodyLength + 11;
}

int  CLogicManager::DealGetLinePointPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;

	if(m_DataPackPares.PackGetLinePointUpPack(pBuffer, m_getLinePointUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UploadGetLinePointPack(m_getLinePointUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					double	dTemp;
					int     nTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_getLinePointDownPack.gDataBodyPack.nTotalPacket = nTitleRecord;
						m_getLinePointDownPack.gDataBodyPack.nCurrentPacket = nHadSendRecord;

						pRecordset->GetFieldValue("Pit_id", nTemp);
						m_getLinePointDownPack.gDataBodyPack.nPointID = nTemp;

						pRecordset->GetFieldValue("Pit_j", dTemp);
						m_getLinePointDownPack.gDataBodyPack.fJDU = (float)dTemp;

						pRecordset->GetFieldValue("Pit_w", dTemp);
						m_getLinePointDownPack.gDataBodyPack.fWDU = (float)dTemp;

						pRecordset->GetFieldValue("Pit_Dis", dTemp);
						m_getLinePointDownPack.gDataBodyPack.fGls = (float)dTemp;

						pRecordset->GetFieldValue("Pit_SX", nTemp);
						m_getLinePointDownPack.gDataBodyPack.nState = nTemp;

						pRecordset->GetFieldValue("Pit_TLid", nTemp);
						m_getLinePointDownPack.gDataBodyPack.nLineID = nTemp;

						m_getLinePointDownPack.nBodyLength = 32;
						m_DataPackPares.PackGetLinePointDownBuild(pBuffer, m_getLinePointDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getLinePointDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}


	m_DataPackPares.FillGetLinePointFailPack(m_getLinePointDownPack);
	m_DataPackPares.PackGetLinePointDownBuild(pBuffer, m_getLinePointDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_askOrgListDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}

int  CLogicManager::DealGetPicListPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;

	if(m_DataPackPares.PackGetPicListPackUpPack(pBuffer, m_getPicListUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UpLoadGetPicListPack(m_getPicListUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_getPicListDownPack.gDataBodyPack.nTotalPacket = nTitleRecord;
						m_getPicListDownPack.gDataBodyPack.nCurrentPacket = nHadSendRecord;

						pRecordset->GetFieldValue("pname", strTemp);
						m_getPicListDownPack.gDataBodyPack.strName = strTemp;
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("dtime", strTemp);
						m_getPicListDownPack.gDataBodyPack.strTime = strTemp;
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("itype", nTemp);
						m_getPicListDownPack.gDataBodyPack.nType = nTemp;

						pRecordset->GetFieldValue("tel", strTemp);
						m_getPicListDownPack.gDataBodyPack.strTel = strTemp;
						strTemp.LockBuffer();

						m_getPicListDownPack.nBodyLength = 82;
						m_DataPackPares.PackGetPicListPackDownBuild(pBuffer, m_getPicListDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getPicListDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}

	m_DataPackPares.FillGetPicListPackFailPack(m_getPicListDownPack);
	m_DataPackPares.PackGetPicListPackDownBuild(pBuffer, m_getPicListDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getPicListDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}

int  CLogicManager::DealGetUrgencyMissionListPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;

	if(m_DataPackPares.PackGetUrgencyMissionListUpPack(pBuffer, m_getUrgencyMissionListUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UpLoadGetUrgencyMissionListPack(m_getUrgencyMissionListUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_getUrgencyMissionListDownPack.gDataBodyPack.nTtlePacket = nTitleRecord;
						m_getUrgencyMissionListDownPack.gDataBodyPack.nCurrentPacket = nHadSendRecord;

						pRecordset->GetFieldValue("Jj_StartD", strTemp);
						m_getUrgencyMissionListDownPack.gDataBodyPack.strSTime = strTemp;
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("Jj_endd", strTemp);
						m_getUrgencyMissionListDownPack.gDataBodyPack.strETime = strTemp;
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("jj_name", strTemp);
						m_getUrgencyMissionListDownPack.gDataBodyPack.strRWName = strTemp;
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("JJ_State", nTemp);
						m_getUrgencyMissionListDownPack.gDataBodyPack.nState = nTemp;

						pRecordset->GetFieldValue("Jj_Id", nTemp);
						m_getUrgencyMissionListDownPack.gDataBodyPack.nID = nTemp;

						pRecordset->GetFieldValue("Jj_lineid", nTemp);
						m_getUrgencyMissionListDownPack.gDataBodyPack.nLineID = nTemp;

						pRecordset->GetFieldValue("Jj_StartP", strTemp);
						m_getUrgencyMissionListDownPack.gDataBodyPack.nSPoint = nTemp;

						pRecordset->GetFieldValue("Jj_Endp", nTemp);
						m_getUrgencyMissionListDownPack.gDataBodyPack.nEPoint = nTemp;

						m_getUrgencyMissionListDownPack.nBodyLength = 118;
						m_DataPackPares.PackGetUrgencyMissionListDownBuild(pBuffer, m_getUrgencyMissionListDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getUrgencyMissionListDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}

	m_DataPackPares.FillGetUrgencyMissionListFailPack(m_getUrgencyMissionListDownPack);
	m_DataPackPares.PackGetUrgencyMissionListDownBuild(pBuffer, m_getUrgencyMissionListDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getUrgencyMissionListDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}

int  CLogicManager::DealGetUrgencyWorkerListPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;

	if(m_DataPackPares.PackGetUrgencyWorkerListUpPack(pBuffer, m_getUrgencyWorkerListUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UploadGetUrgencyWorkerListPack(m_getUrgencyWorkerListUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_getUrgencyWorkerListDownPack.gDataBodyPack.nTtlePacket = nTitleRecord;
						m_getUrgencyWorkerListDownPack.gDataBodyPack.nCurrentPacket = nHadSendRecord;

						pRecordset->GetFieldValue("Jj_Pid", strTemp);
						m_getUrgencyWorkerListDownPack.gDataBodyPack.strJj_Pid = strTemp;
						strTemp.LockBuffer();
						
						pRecordset->GetFieldValue("username", strTemp);
						m_getUrgencyWorkerListDownPack.gDataBodyPack.strJj_Name = strTemp;
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("userOrg", nTemp);
						m_getUrgencyWorkerListDownPack.gDataBodyPack.nJj_OrgID = nTemp;
												
						pRecordset->GetFieldValue("Jj_time", strTemp);
						m_getUrgencyWorkerListDownPack.gDataBodyPack.strJj_Time = strTemp;
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("Jj_long", nTemp);
						m_getUrgencyWorkerListDownPack.gDataBodyPack.nJj_long = nTemp;

						m_getUrgencyWorkerListDownPack.nBodyLength = 101;
						m_DataPackPares.PackGetUrgencyWorkerListDownBuild(pBuffer, m_getUrgencyWorkerListDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getUrgencyWorkerListDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}

	m_DataPackPares.FillGetUrgencyWorkerListFailPack(m_getUrgencyWorkerListDownPack);
	m_DataPackPares.PackGetUrgencyWorkerListDownBuild(pBuffer, m_getUrgencyWorkerListDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getUrgencyWorkerListDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}

int  CLogicManager::DealGetPicDataPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData,
	CGobalConfig* pGobalConfig)
{
	GPSPIC_Pack gpsPicPack;
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;

	if(m_DataPackPares.PackGetPicDataUpPack(pBuffer, m_getPicDataUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			if(m_AccessBaseData.UpLoadGetPicDataPack(m_getPicDataUpPack, gpsPicPack))
			{
				FILE * fPic = NULL;
				string strPicnName;

				strPicnName = pGobalConfig->GetPicFilePath() + "\\" + gpsPicPack.strPicName.substr(0, 8) + "\\" + gpsPicPack.strPicName;
				fPic = fopen(strPicnName.c_str(), "rb");
				if(fPic)
				{
					long lFileLength;
					const int nBufferLength = 1024;

					fseek(fPic, 0, SEEK_END);
					lFileLength = ftell(fPic);

					if(lFileLength > 0)
					{
						int nTotalPack;
						int nCurrentPack;
						int nReadCount;
						int nTemp;

						if((lFileLength % nBufferLength) == 0)
							nTotalPack = lFileLength / (nBufferLength);
						else
							nTotalPack = lFileLength / (nBufferLength) + 1;

						fseek(fPic, 0, SEEK_SET);
						for (nCurrentPack = 1; nCurrentPack <= nTotalPack ; ++nCurrentPack )
						{
							m_getPicDataDownPack.gDataBodyPack.nTtlePacket = nTotalPack;
							m_getPicDataDownPack.gDataBodyPack.nCurrentPacket = nCurrentPack;
							m_getPicDataDownPack.gDataBodyPack.nPagesize = lFileLength;

							memset(m_getPicDataDownPack.gDataBodyPack.picBuffer, 0x00, nBufferLength);

							nReadCount = fread(m_getPicDataDownPack.gDataBodyPack.picBuffer, 1, nBufferLength, fPic);
							if(nReadCount > 0)
							{
								m_getPicDataDownPack.nBodyLength = nBufferLength + 12;
								m_DataPackPares.PackGetPicDataDownBuild(pBuffer, m_getPicDataDownPack);
								pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getPicDataDownPack.nBodyLength + 11;

								nTemp = DealSendData(pKeyOverPire, pWorkThread);
								if(0 != nTemp)
								{
									fclose(fPic);
									return -2;
								}
							}
						}
						fclose(fPic);
						return 0;
					}
					fclose(fPic);
				}
			}
		}
	}

	m_DataPackPares.FillGetPicDataFilePack(m_getPicDataDownPack);
	m_DataPackPares.PackGetPicDataDownBuild(pBuffer, m_getPicDataDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getPicDataDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}


int  CLogicManager::DealGetSchedueListPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;

	if(m_DataPackPares.PackGetOrgSchedueListUpPack(pBuffer, m_getOrgSchedueListUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UpLoadGetOrgSchedueListPack(m_getOrgSchedueListUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_getOrgSchedueListDownPack.gDataBodyPack.nTotlePacket = nTitleRecord;
						m_getOrgSchedueListDownPack.gDataBodyPack.nCurrentPacket = nHadSendRecord;
						
						pRecordset->GetFieldValue("r", strTemp);
						m_getOrgSchedueListDownPack.gDataBodyPack.strUserID = strTemp;
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("r2", strTemp);
						m_getOrgSchedueListDownPack.gDataBodyPack.strDates = strTemp;
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("r3", strTemp);
						m_getOrgSchedueListDownPack.gDataBodyPack.strXJ = strTemp;
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("r4", nTemp);
						m_getOrgSchedueListDownPack.gDataBodyPack.nSTotlePoint = nTemp;

						pRecordset->GetFieldValue("r5", nTemp);
						m_getOrgSchedueListDownPack.gDataBodyPack.nSGetPoint = nTemp;

						pRecordset->GetFieldValue("r6", nTemp);
						m_getOrgSchedueListDownPack.gDataBodyPack.nCPoint = nTemp;

						pRecordset->GetFieldValue("r7", nTemp);
						m_getOrgSchedueListDownPack.gDataBodyPack.nLPoint = nTemp;

						m_getOrgSchedueListDownPack.nBodyLength = 84;
						m_DataPackPares.PackGetOrgSchedueListDownBuild(pBuffer, m_getOrgSchedueListDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getOrgSchedueListDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}

	m_DataPackPares.FillGetOrgSchedueListFailPack(m_getOrgSchedueListDownPack);
	m_DataPackPares.PackGetOrgSchedueListDownBuild(pBuffer, m_getOrgSchedueListDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getUrgencyMissionListDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}


int  CLogicManager::DealGetSchedueInfoPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackGetOrgSchedueInfoUpPack(pBuffer, m_getOrgSchedueInfoUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UpLoadGetOrgSchedueInfoPack(m_getOrgSchedueInfoUpPack, m_getOrgSchedueInfoDownPack);
			m_DataPackPares.PackGetOrgSchedueInfoDownBuild(pBuffer, m_getOrgSchedueInfoDownPack);

			return m_getOrgSchedueInfoDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillGetOrgSchedueInfoFailPack(m_getOrgSchedueInfoDownPack);
	m_DataPackPares.PackGetOrgSchedueInfoDownBuild(pBuffer, m_getOrgSchedueInfoDownPack);

	return m_getOrgSchedueInfoDownPack.nBodyLength + 11;
}

int  CLogicManager::DealSetPointPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackSetPointUpPack(pBuffer, m_setPointUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UpLoadSetPointPack(m_setPointUpPack, m_setPointDownPack);
			m_DataPackPares.PackSetPointDownBuild(pBuffer, m_setPointDownPack);

			return m_setPointDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillSetPointFailPack(m_setPointDownPack);
	m_DataPackPares.PackSetPointDownBuild(pBuffer, m_setPointDownPack);

	return m_setPointDownPack.nBodyLength + 11;
}

int  CLogicManager::DealGetOrgLinePack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;

	if(m_DataPackPares.PackGetOrgLineUpPack(pBuffer, m_getOrgLineUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UploadGetOrgLinePack(m_getOrgLineUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_getOrgLineDownPack.gDataBodyPack.nTtotlePacket = nTitleRecord;
						m_getOrgLineDownPack.gDataBodyPack.nCurrentPacket = nHadSendRecord;

						pRecordset->GetFieldValue(_T("line_id"), nTemp);
						m_getOrgLineDownPack.gDataBodyPack.nLineID = nTemp;

						pRecordset->GetFieldValue(_T("line_day"), nTemp);
						m_getOrgLineDownPack.gDataBodyPack.nLineDay = nTemp;

						pRecordset->GetFieldValue(_T("line_name"), strTemp);
						m_getOrgLineDownPack.gDataBodyPack.strLineName = strTemp;

						m_getOrgLineDownPack.nBodyLength = 46;
						m_DataPackPares.PackGetOrgLineDownBuild(pBuffer, m_getOrgLineDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getOrgLineDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}
	m_DataPackPares.FillGetOrgLineFailPack(m_getOrgLineDownPack);
	m_DataPackPares.PackGetOrgLineDownBuild(pBuffer, m_getOrgLineDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getOrgLineDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}
int  CLogicManager::DealGetLinePointTimePack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;

	if(m_DataPackPares.PackGetLinePointTimeUpPack(pBuffer, m_getLinePointTimeUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UpLoadGetLinePointTimePack(m_getLinePointTimeUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;
					double  dTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_getLinePointTimeDownPack.gDataBodyPack.nTotlePacket = nTitleRecord;
						m_getLinePointTimeDownPack.gDataBodyPack.nCurrentPacket = nHadSendRecord;
						
						pRecordset->GetFieldValue(_T("pit_dis"), dTemp);
						m_getLinePointTimeDownPack.gDataBodyPack.fIKM = (float)dTemp;

						pRecordset->GetFieldValue(_T("pit_j"), dTemp);
						m_getLinePointTimeDownPack.gDataBodyPack.fILon = (float)dTemp;

						pRecordset->GetFieldValue(_T("pit_w"), dTemp);
						m_getLinePointTimeDownPack.gDataBodyPack.fILat = (float)dTemp;

						pRecordset->GetFieldValue(_T("pit_sx"), nTemp);
						m_getLinePointTimeDownPack.gDataBodyPack.nIDirect = nTemp;

						pRecordset->GetFieldValue(_T("lt_pointid"), nTemp);
						m_getLinePointTimeDownPack.gDataBodyPack.nPointID = nTemp;

						pRecordset->GetFieldValue(_T("Lt_PointTimes"), strTemp);
						m_getLinePointTimeDownPack.gDataBodyPack.strTime = strTemp.GetBuffer();
						strTemp.LockBuffer();

						m_getLinePointTimeDownPack.nBodyLength = 48;
						m_DataPackPares.PackGetLinePointTimeDownBuild(pBuffer, m_getLinePointTimeDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getLinePointTimeDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}
	m_DataPackPares.FillGetLinePointTimeFailPack(m_getLinePointTimeDownPack);
	m_DataPackPares.PackGetLinePointTimeDownBuild(pBuffer, m_getLinePointTimeDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getLinePointTimeDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}
int  CLogicManager::DealGetOrgSchWorkerPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;

	if(m_DataPackPares.PackGetOrgSchWorkerUpPack(pBuffer, m_getOrgSchWorkerUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UpLoadGetOrgSchWorkerPack(m_getOrgSchWorkerUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_getOrgSchWorkerDownPack.gDataBodyPack.nTotle = nTitleRecord;
						m_getOrgSchWorkerDownPack.gDataBodyPack.nHasRecv = nHadSendRecord;

						pRecordset->GetFieldValue(_T("User_Oper"), strTemp);
						m_getOrgSchWorkerDownPack.gDataBodyPack.strOper = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue(_T("User_Name"), strTemp);
						m_getOrgSchWorkerDownPack.gDataBodyPack.strName = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue(_T("User_Tel"), strTemp);
						m_getOrgSchWorkerDownPack.gDataBodyPack.strTel = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue(_T("User_Org"), nTemp);
						m_getOrgSchWorkerDownPack.gDataBodyPack.nOrgID = nTemp;

						pRecordset->GetFieldValue(_T("User_PGroup"), nTemp);
						m_getOrgSchWorkerDownPack.gDataBodyPack.nPowerGroup = nTemp;

						pRecordset->GetFieldValue(_T("User_Pda1"), nTemp);
						m_getOrgSchWorkerDownPack.gDataBodyPack.nPda1 = nTemp;

						pRecordset->GetFieldValue(_T("User_Pda2"), nTemp);
						m_getOrgSchWorkerDownPack.gDataBodyPack.nPda2 = nTemp;

						pRecordset->GetFieldValue(_T("User_State"), nTemp);
						m_getOrgSchWorkerDownPack.gDataBodyPack.nWorkerState = nTemp;

						m_getOrgSchWorkerDownPack.nBodyLength = 88;
						m_DataPackPares.PackGetOrgSchWorkerDownBuild(pBuffer, m_getOrgSchWorkerDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getOrgSchWorkerDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}
	m_DataPackPares.FillGetOrgSchWorkerFailPack(m_getOrgSchWorkerDownPack);
	m_DataPackPares.PackGetOrgSchWorkerDownBuild(pBuffer, m_getOrgSchWorkerDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_getOrgSchWorkerDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}

int  CLogicManager::DealWorkerPollQueryPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;

	if(m_DataPackPares.PackWorkerPollQueryUpPack(pBuffer, m_workerPollQueryUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UploadWorkerPollQureyPack(m_workerPollQueryUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;
					double  dTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_workerPollQueryDownPack.gDataBodyPack.nTotlePacket = nTitleRecord;
						m_workerPollQueryDownPack.gDataBodyPack.nCurrentPacket = nHadSendRecord;

						pRecordset->GetFieldValue(_T("G_j"), dTemp);
						m_workerPollQueryDownPack.gDataBodyPack.fJUD = (float)dTemp;

						pRecordset->GetFieldValue(_T("G_w"), dTemp);
						m_workerPollQueryDownPack.gDataBodyPack.fWDU = (float)dTemp;

						pRecordset->GetFieldValue(_T("G_time"), strTemp);
						m_workerPollQueryDownPack.gDataBodyPack.strTime = strTemp.GetBuffer();
						strTemp.LockBuffer();
						
						m_workerPollQueryDownPack.nBodyLength = 36;
						m_DataPackPares.PackWorkerPollQueryDownBuild(pBuffer, m_workerPollQueryDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_workerPollQueryDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}
	m_DataPackPares.FillWorkerPollQueryFailPack(m_workerPollQueryDownPack);
	m_DataPackPares.PackWorkerPollQueryDownBuild(pBuffer, m_workerPollQueryDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_workerPollQueryDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}

int  CLogicManager::DealUrgencyMissionDeletePack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackUrgencyMissionDeleteUpPack(pBuffer, m_urgencyMissionDeleteUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UpLoadUrgencyMissionDeletePack(m_urgencyMissionDeleteUpPack, m_urgencyMissionDeleteDownPack);
			m_DataPackPares.PackUrgencyMissionDeleteDownBuild(pBuffer, m_urgencyMissionDeleteDownPack);
			return m_urgencyMissionDeleteDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillUrgencyMissionDeleteFailPack(m_urgencyMissionDeleteDownPack);
	m_DataPackPares.PackUrgencyMissionDeleteDownBuild(pBuffer, m_urgencyMissionDeleteDownPack);
	return m_urgencyMissionDeleteDownPack.nBodyLength + 11;
}

int  CLogicManager::DealWorkerPollPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;

	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;

	if(m_DataPackPares.PackWorkerPollUpPack(pBuffer, m_workerPollUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UploadWorkerPollPack(m_workerPollUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_workerPollDownPack.gDataBodyPack.nTotlePacket = nTitleRecord;
						m_workerPollDownPack.gDataBodyPack.nCurrentPacket = nHadSendRecord;

						pRecordset->GetFieldValue(_T("R_pointid"), nTemp);
						m_workerPollDownPack.gDataBodyPack.nPointID = nTemp;

						pRecordset->GetFieldValue(_T("r_pxdate"), strTemp);
						m_workerPollDownPack.gDataBodyPack.strDate = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue(_T("R_arrTime"), strTemp);
						m_workerPollDownPack.gDataBodyPack.strArrtime = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue(_T("R_realtime"), strTemp);
						m_workerPollDownPack.gDataBodyPack.strRealTime = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue(_T("R_pid"), strTemp);
						m_workerPollDownPack.gDataBodyPack.strPID = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue(_T("P_state"), nTemp);
						m_workerPollDownPack.gDataBodyPack.nPointState = nTemp;


						m_workerPollDownPack.nBodyLength = 96;
						m_DataPackPares.PackWorkerPollDownBuild(pBuffer, m_workerPollDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_workerPollDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}
	m_DataPackPares.FillWorkerPollFailPack(m_workerPollDownPack);
	m_DataPackPares.PackWorkerPollDownBuild(pBuffer, m_workerPollDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_workerPollDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}

int  CLogicManager::DealSysSettingPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackSysSettingUpPack(pBuffer, m_sysSettingUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UpLoadSysSettingPack(m_sysSettingUpPack, m_sysSettingDownPack);
			m_DataPackPares.PackSysSettingDownBuild(pBuffer, m_sysSettingDownPack);

			return m_sysSettingDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillSysSettingFailPack(m_sysSettingDownPack);
	m_DataPackPares.PackSysSettingDownBuild(pBuffer, m_sysSettingDownPack);

	return m_sysSettingDownPack.nBodyLength + 11;
}

int  CLogicManager::DealSysSettingGetPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackSysSettingGetUpPack(pBuffer, m_sysSettingGetUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UpLoadSysSettingGetPack(m_sysSettingGetUpPack, m_sysSettingGetDownPack);
			m_DataPackPares.PackSysSettingGetDownBuild(pBuffer, m_sysSettingGetDownPack);

			return m_sysSettingGetDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillSysSettingGetFailPack(m_sysSettingGetDownPack);
	m_DataPackPares.PackSysSettingGetDownBuild(pBuffer, m_sysSettingGetDownPack);

	return m_sysSettingGetDownPack.nBodyLength + 11;
}

int  CLogicManager::DealModifyPassPack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackModifyPassUpPack(pBuffer, m_sysModifyPasswordUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UpLoadModifyPassPack(m_sysModifyPasswordUpPack, m_sysModifyPasswordDownPack);
			m_DataPackPares.PackModifyPassDownBuild(pBuffer, m_sysModifyPasswordDownPack);

			return m_sysModifyPasswordDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillModifyPassFailPack(m_sysModifyPasswordDownPack);
	m_DataPackPares.PackModifyPassDownBuild(pBuffer, m_sysModifyPasswordDownPack);

	return m_sysModifyPasswordDownPack.nBodyLength + 11;
}

int  CLogicManager::DealWorkWramPointPack(DWORD dNumberOfBytes, 
	LPOverKeyPire pKeyOverPire, 
	CADODatabase* pDatabase,
	void* pWorkThread,
	void* pFunDealSendData)
{
	FPDealSendData DealSendData = (FPDealSendData)pFunDealSendData;
	char * pBuffer = pKeyOverPire->pireOverLappedex.wsaBuffer;

	if(m_DataPackPares.PackWorkerWramPointUpPack(pBuffer, m_workWramPointUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			auto_ptr<CADORecordset> pRecordset(m_AccessBaseData.UpLoadWorkWramPointPack(m_workWramPointUpPack));
			if(pRecordset.get())
			{
				DWORD nHadSendRecord;
				DWORD nTitleRecord = pRecordset->GetRecordCount();

				if(nTitleRecord > 0)
				{
					CString strTemp;
					int     nTemp;
					double  dTemp;

					pRecordset->MoveFirst();
					for (nHadSendRecord = 1; !pRecordset->IsEOF(); ++nHadSendRecord, pRecordset->MoveNext())
					{
						m_workWramPointDownPack.gDataBodyPack.nTotlePacket = nTitleRecord;
						m_workWramPointDownPack.gDataBodyPack.nCurrentPacket = nHadSendRecord;

						pRecordset->GetFieldValue("tl_name", strTemp);
						m_workWramPointDownPack.gDataBodyPack.strLineName = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("pit_dis", dTemp);
						m_workWramPointDownPack.gDataBodyPack.fDirect = (float)dTemp;

						pRecordset->GetFieldValue("r_userid", strTemp);
						m_workWramPointDownPack.gDataBodyPack.strUserId = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("r_arrtime", strTemp);
						m_workWramPointDownPack.gDataBodyPack.strSTime = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("r_realtime", strTemp);
						m_workWramPointDownPack.gDataBodyPack.strRealTime = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("r_pxdate", strTemp);
						m_workWramPointDownPack.gDataBodyPack.strData = strTemp.GetBuffer();
						strTemp.LockBuffer();

						pRecordset->GetFieldValue("p_state", nTemp);
						m_workWramPointDownPack.gDataBodyPack.nPointState = nTemp;

						m_workWramPointDownPack.nBodyLength = 186;
						m_DataPackPares.PackWorkerWramPointDownBuild(pBuffer, m_workWramPointDownPack);
						pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_workWramPointDownPack.nBodyLength + 11;

						nTemp = DealSendData(pKeyOverPire, pWorkThread);
						if(0 != nTemp)
							return -2;
					}
					return 0;
				}
			}
		}
	}
	m_DataPackPares.FillWorkerWramPointFailPack(m_workWramPointDownPack);
	m_DataPackPares.PackWorkerWramPointDownBuild(pBuffer, m_workWramPointDownPack);
	pKeyOverPire->pireOverLappedex.wsaWSABUF.len = m_workWramPointDownPack.nBodyLength + 11;

	DealSendData(pKeyOverPire, pWorkThread);
	return 0;
}

int  CLogicManager::DealOpteLinePack(DWORD dNumberOfBytes, 
	char  * pBuffer, 
	CADODatabase* pDatabase)
{
	if(m_DataPackPares.PackOpteLineUpPack(pBuffer, m_opteLineUpPack))
	{
		if(m_AccessBaseData.InitAccesser(pDatabase))
		{
			m_AccessBaseData.UpLoadOpteLinePack(m_opteLineUpPack, m_opteLineDownPack);
			m_DataPackPares.PackOpteLineDownBuild(pBuffer, m_opteLineDownPack);

			return m_opteLineDownPack.nBodyLength + 11;
		}
	}

	m_DataPackPares.FillOpteLineFailPack(m_opteLineDownPack);
	m_DataPackPares.PackOpteLineDownBuild(pBuffer, m_opteLineDownPack);

	return m_opteLineDownPack.nBodyLength + 11;
}

void CLogicManager::FillPicStoreStruct(GPSPIC_Pack& gpsPicUpLoadPack, string strTel, int nType)
{
	gpsPicUpLoadPack.strPicName = CCommonFunction::GetCurrentTimeByFormat() + ".jpg";
	gpsPicUpLoadPack.strTime = gpsPicUpLoadPack.strPicName.substr(0, 4) + "-" + 
		gpsPicUpLoadPack.strPicName.substr(4, 2) + "-" +
		gpsPicUpLoadPack.strPicName.substr(6, 2) + " " +
		gpsPicUpLoadPack.strPicName.substr(8, 2) + ":" +
		gpsPicUpLoadPack.strPicName.substr(10, 2) +":" +
		gpsPicUpLoadPack.strPicName.substr(12, 2);
	gpsPicUpLoadPack.strTel = strTel;
	gpsPicUpLoadPack.nType = nType;
}

bool CLogicManager::BuildPicDir(string strDir)
{
	if(!CreateDirectory(strDir.c_str(), NULL))
	{
		if(GetLastError() == ERROR_ALREADY_EXISTS)
			return true;
		else
			return false;
	}
	return true;
}