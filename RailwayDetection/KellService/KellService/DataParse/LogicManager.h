#pragma once
#include "IOCPDefine.h"
#include "DataPackParse.h"
#include "AccessBaseData.h"

// 数据逻辑处理
class CADODatabase;
class CGobalConfig;
class CLogicManager
{
public:
	CLogicManager(void);
	virtual ~CLogicManager(void);

public:
	// 返回值小于等于0时表示数据包无效,不应当返回一个数据包
	// 返回值等于0表求数据包是请求列表的,在回调中已经处理
	// 不再需要返回数据包
	// 返回值大于0时表示数据包有效
	int SetLogicData(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunSendData,
		void* pFunRecvData,
		CGobalConfig* pGobalConfig);

private:
	int  DealGPSPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunSendData,
		void* pFunRecvData,
		CGobalConfig* pGobalConfig);

	int  DealLoginPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealAddOrgPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealModifyOrgPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealDeleteOrgPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealAskOrgListPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);

	int  DealAddWorkerPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealModifyWorkerPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealDeleteWorkerPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealAskWorkerListPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);

	int  DealAddPowerGroupPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealModifyPowerGroupPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealDeletePowerGroupPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealAskPowerGroupListPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);

	int  DealAddDevicePack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealModifyDevicePack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealDeleteDevicePack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealAskDeviceListPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);

	int  DealGetGPSPointPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);

	int  DealGetRailWayNamePack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealSettingLinePack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealSettingPollingTimePack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealSettingSchedule5Pack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealSettingSchedule6Pack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealSettingScheduleWorkerPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealUrgencyReleasePack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealUrgencyWorkerPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealGetLinePointPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);

	int  DealGetPicListPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);

	int  DealGetUrgencyMissionListPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);

	int  DealGetUrgencyWorkerListPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);

	int  DealGetPicDataPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData,
		CGobalConfig* pGobalConfig);

	int  DealGetSchedueListPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);

	int  DealGetSchedueInfoPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealSetPointPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealGetOrgLinePack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);
	int  DealGetLinePointTimePack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);
	int  DealGetOrgSchWorkerPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);

	int  DealWorkerPollQueryPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);

	int  DealUrgencyMissionDeletePack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealWorkerPollPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);

	int  DealSysSettingPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealSysSettingGetPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealModifyPassPack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);

	int  DealWorkWramPointPack(DWORD dNumberOfBytes, 
		LPOverKeyPire pKeyOverPire, 
		CADODatabase* pDatabase,
		void* pWorkThread,
		void* pFunDealSendData);

	int  DealOpteLinePack(DWORD dNumberOfBytes, 
		char  * pBuffer, 
		CADODatabase* pDatabase);
private:
	void FillPicStoreStruct(GPSPIC_Pack& gpsPicUpLoadPack, string strTel, int nType);
	bool BuildPicDir(string strDir);

	CDataPackParse			m_DataPackPares;
	CAccessBaseData			m_AccessBaseData;

private:
	GPS_UpLoad_Pack			m_gpsUpPack;
	GPS_DownLoad_Pack		m_gpsDownPack;

	Login_UpLoad_Pack		m_loginUpPack;
	Login_DownLoad_Pack		m_loginDownPack;

	AddOrg_UpLoad_Pack		m_addOrgUpPack;
	AddOrg_DownLoad_Pack	m_addOrgDownPack;

	ModifyOrg_UpLoad_Pack	m_modifyOrgUpPack;
	ModifyOrg_DownLoad_Pack	m_modifyOrgDownPack;

	DeleteOrg_UpLoad_Pack	m_deleteOrgUpPack;
	DeleteOrg_DownLoad_Pack	m_deleteOrgDownPack;

	AskOrgList_UpLoad_Pack		m_askOrgListUpPack;
	AskOrgList_DownLoad_Pack	m_askOrgListDownPack;

	AddWorker_UpLoad_Pack		m_addWorkerUpPack;
	AddWorker_DownLoad_Pack		m_addWorkerDownPack;

	ModifyWorker_UpLoad_Pack	m_modifyWorkerUpPack;
	ModifyWorker_DownLoad_Pack	m_modifyWorkerDownPack;

	DeleteWorker_UpLoad_Pack	m_deleteWorkerUpPack;
	DeleteWorker_DownLoad_Pack	m_deleteWorkerDownPack;

	AskWorkerList_UpLoad_Pack		m_askWorkerListUpPack;
	AskWorkerList_DownLoad_Pack		m_askWorkerListDownPack;

	AddPowerGroup_UpLoad_Pack		m_addPowerGroupUpPack;
	AddPowerGroup_DownLoad_Pack		m_addPowerGroupDownPack;

	ModifyPowerGroup_UpLoad_Pack	m_modifyPowerGroupUpPack;
	ModifyPowerGroup_DownLoad_Pack	m_modifyPowerGroupDownPack;

	DeletePowerGroup_UpLoad_Pack	m_deletePowerGroupUpPack;
	DeletePowerGroup_DownLoad_Pack	m_deletePowerGroupDownPack;

	AskPowerGroupList_UpLoad_Pack	m_askPowerGroupListUpPack;
	AskPowerGroupList_DownLoad_Pack	m_askPowerGroupListDownPack;

	AddDevice_UpLoad_Pack				m_addDeviceUpPack;
	AddDevice_DownLoad_Pack				m_addDeviceDownPack;

	ModifyDevice_UpLoad_Pack			m_modifyDeviceUpPack;
	ModifyDevice_DownLoad_Pack			m_modifyDeviceDownPack;

	DeleteDevice_UpLoad_Pack			m_deleteDeviceUpPack;
	DeleteDevice_DownLoad_Pack			m_deleteDeviceDownPack;

	AskDeviceList_Upload_Pack			m_askDeviceUpPack;
	AskDeviceList_Download_Pack			m_askDeviceDownPack;

	GetGPSPoint_Upload_Pack				m_getGPSPointUpPack;
	GetGPSPoint_Download_Pack			m_getGPSPointDownPack;

	GetRailWayName_Upload_Pack			m_getRailWayNameUpPack;
	GetRailWayName_Download_Pack		m_getRailWayNameDownPack;		

	SettingLine_Upload_Pack				m_settingLinkUpPack;
	SettingLine_Download_Pack			m_settingLinkDownPack;

	SettingPollingTime_Upload_Pack		m_settingPollingTimeUpPack;
	SettingPollingTime_Download_Pack	m_settingPollingTimeDownPack;

	SettingSchedule5_Upload_Pack		m_settingSchedule5UpPack;
	SettingSchedule5_Download_Pack		m_settingSchedule5DownPack;

	SettingSchedule6_Upload_Pack		m_settingSchedule6UpPack;
	SettingSchedule6_Download_Pack		m_settingSchedule6DownPack;

	SettingScheduleWorker_Upload_Pack	m_settingScheduleWorkerUpPack;
	SettingScheduleWorker_Download_Pack	m_settingScheduleWorkerDownPack;

	UrgencyRelease_Upload_Pack			m_urgencyReleaseUpPack;
	UrgencyRelease_Download_Pack		m_urgencyReleaseDownPack;

	UrgencyWorker_Upload_Pack			m_urgencyWorkerUpPack;
	UrgencyWorker_Download_Pack			m_urgencyWorkerDownPack;

	GetLinePoint_Upload_Pack			m_getLinePointUpPack;
	GetLinePoint_Download_Pack			m_getLinePointDownPack;

	GetPicList_Upload_Pack				m_getPicListUpPack;
	GetPicList_Download_Pack			m_getPicListDownPack;

	GetUrgencyMissionList_Upload_Pack	m_getUrgencyMissionListUpPack;
	GetUrgencyMissionList_Download_Pack	m_getUrgencyMissionListDownPack;

	GetUrgencyWorkerList_Upload_Pack	m_getUrgencyWorkerListUpPack;
	GetUrgencyWorkerList_Download_Pack	m_getUrgencyWorkerListDownPack;

	GetPicData_Upload_Pack				m_getPicDataUpPack;	
	GetPicData_Download_Pack			m_getPicDataDownPack;
	
	GetOrgSchedueList_Upload_Pack		m_getOrgSchedueListUpPack;
	GetOrgSchedueList_Download_Pack		m_getOrgSchedueListDownPack;

	GetOrgSchedueInfo_Upload_Pack		m_getOrgSchedueInfoUpPack;
	GetOrgSchedueInfo_Download_Pack		m_getOrgSchedueInfoDownPack;

	SetPoint_Upload_Pack				m_setPointUpPack;
	SetPoint_Download_Pack				m_setPointDownPack;

	GetOrgLine_Upload_Pack				m_getOrgLineUpPack;
	GetOrgLine_Download_Pack			m_getOrgLineDownPack;

	GetLinePointTime_Upload_Pack		m_getLinePointTimeUpPack;
	GetLinePointTime_Download_Pack		m_getLinePointTimeDownPack;

	GetOrgSchWorker_Upload_Pack			m_getOrgSchWorkerUpPack;
	GetOrgSchWorker_Download_Pack		m_getOrgSchWorkerDownPack;
	
	WorkerPollQuery_Upload_Pack			m_workerPollQueryUpPack;
	WorkerPollQuery_Download_Pack		m_workerPollQueryDownPack;

	UrgencyMissionDelete_Upload_Pack	m_urgencyMissionDeleteUpPack;
	UrgencyMissionDelete_Download_Pack	m_urgencyMissionDeleteDownPack;

	WorkerPoll_Upload_Pack				m_workerPollUpPack;
	WorkerPoll_Download_Pack			m_workerPollDownPack;
	
	SysSetting_Upload_Pack				m_sysSettingUpPack;
	SysSetting_Download_Pack			m_sysSettingDownPack;

	SysSettingGet_Upload_Pack			m_sysSettingGetUpPack;
	SysSettingGet_Download_Pack			m_sysSettingGetDownPack;

	ModifyPassword_Upload_Pack			m_sysModifyPasswordUpPack;
	ModifyPassword_Download_Pack		m_sysModifyPasswordDownPack;

	WorkWramPoint_Upload_Pack			m_workWramPointUpPack;
	WorkWramPoint_Download_Pack			m_workWramPointDownPack;
	
	OpteLine_Upload_Pack				m_opteLineUpPack;
	OpteLine_Download_Pack				m_opteLineDownPack;
};

