#pragma once
#include "adoConnect.h"
#include "ProtocolDefine.h"


#define INPUTSQLBUFFERS                                    1024

class DATABASEINTERFACE_API CAccessBaseData
{
public:
	CAccessBaseData(void);
	virtual ~CAccessBaseData(void);

public:
	bool InitAccesser(CADODatabase* pDatabase);
		
	bool UploadGPSPack(const GPS_UpLoad_Pack& gpsUpPack, 
		GPS_DownLoad_Pack& gpsDownPack);

	bool UpLoadGPSPICPack(const GPSPIC_Pack& gpsPicUpLoadPack); 

	bool UploadLoginPack(const Login_UpLoad_Pack& loginUpPack, 
		Login_DownLoad_Pack& loginDownPack);

	bool UploadAddOrgPack(const AddOrg_UpLoad_Pack& addOrgUpPack, 
		AddOrg_DownLoad_Pack& addOrgDownPack);

	bool UploadModifyOrgPack(const ModifyOrg_UpLoad_Pack& modifyOrgUpPack, 
		ModifyOrg_DownLoad_Pack& modifyOrgDownPack);

	bool UploadDeleteOrgPack(const DeleteOrg_UpLoad_Pack& deleteOrgUpPack, 
		DeleteOrg_DownLoad_Pack& deleteOrgDownPack);

	CADORecordset* UploadAskOrgListPack(const AskOrgList_UpLoad_Pack& askOrgListUpPack);

	bool UploadAddWorkerPack(const AddWorker_UpLoad_Pack& addWorkerUpPack, 
		AddWorker_DownLoad_Pack& addWorkerDownPack);

	bool UploadModifyWorkerPack(const ModifyWorker_UpLoad_Pack& modifyWorkerUpPack, 
		ModifyWorker_DownLoad_Pack& modifyWorkerDownPack);

	bool UploadDeleteWorkerPack(const DeleteWorker_UpLoad_Pack& deleteWorkerUpPack, 
		DeleteWorker_DownLoad_Pack& deleteWorkerDownPack);

	CADORecordset* UploadAskWorkerListPack(const AskWorkerList_UpLoad_Pack& askWorkerListUpPack);

	bool UploadAddPowerGroupPack(const AddPowerGroup_UpLoad_Pack& addPowerGroupUpPack, 
		AddPowerGroup_DownLoad_Pack& addPowerGroupDownPack);

	bool UploadModifyPowerGroupPack(const ModifyPowerGroup_UpLoad_Pack& modifyPowerGroupUpPack, 
		ModifyPowerGroup_DownLoad_Pack& modifyPowerGroupDownPack);

	bool UploadDeletePowerGroupPack(const DeletePowerGroup_UpLoad_Pack& deletePowerGroupUpPack, 
		DeletePowerGroup_DownLoad_Pack& deletePowerGroupDownPack);

	CADORecordset* UploadAskPowerGroupListPack(const AskPowerGroupList_UpLoad_Pack& askPowerGroupListUpPack);

	bool UploadAddDevicePack(const AddDevice_UpLoad_Pack& addDeviceUpPack, 
		AddDevice_DownLoad_Pack& addDeviceDownPack);

	bool UploadModifyDevicePack(const ModifyDevice_UpLoad_Pack& modifyDeviceUpPack, 
		ModifyDevice_DownLoad_Pack& modifyDeviceDownPack);

	bool UploadDeleteDevicePack(const DeleteDevice_UpLoad_Pack& deleteDeviceUpPack, 
		DeleteDevice_DownLoad_Pack& deleteDeviceDownPack);

	CADORecordset* UploadAskDeviceListPack(const AskDeviceList_Upload_Pack& askDeviceListUpPack);

	CADORecordset* UpLoadGetGPSPointPack(const GetGPSPoint_Upload_Pack& getGPSPointUpPack);

	CADORecordset* UpLoadGetRailWayNamePack(const GetRailWayName_Upload_Pack& getRailWayNameUpPack);

	bool UpLoadSettingLinePack(const SettingLine_Upload_Pack& settingLineUpPack,
		SettingLine_Download_Pack& settingLineDownPack);

	bool UpLoadSettingPollingTimePack(const SettingPollingTime_Upload_Pack& settingPollingTimeUpPack,
		SettingPollingTime_Download_Pack& settingPollingTimeDownPack);

	bool UpLoadSettingSchedule5Pack(const SettingSchedule5_Upload_Pack& settingSchedule5UpPack,
		SettingSchedule5_Download_Pack& settingSchedule5DownPack);

	bool UpLoadSettingSchedule6Pack(const SettingSchedule6_Upload_Pack& settingSchedule6UpPack,
		SettingSchedule6_Download_Pack& settingSchedule6DownPack);

	bool UpLoadSettingScheduleWorkerPack(const SettingScheduleWorker_Upload_Pack& settingScheduleWorkerUpPack,
		SettingScheduleWorker_Download_Pack& settingScheduleWorkerDownPack);

	bool UpLoadUrgencyReleasePack(const UrgencyRelease_Upload_Pack& urgencyReleaseUpPack,
		UrgencyRelease_Download_Pack& urgencyReleaseDownPack);

	bool UpLoadUrgencyWorkerPack(const UrgencyWorker_Upload_Pack& urgencyWorkerUpPack,
		UrgencyWorker_Download_Pack& urgencyWorkerDownPack);

	CADORecordset* UploadGetLinePointPack(const GetLinePoint_Upload_Pack& askDeviceListUpPack);

	CADORecordset* UpLoadGetPicListPack(const GetPicList_Upload_Pack& getPicListUpPack);

	CADORecordset* UpLoadGetUrgencyMissionListPack(const GetUrgencyMissionList_Upload_Pack& getRailWayNameUpPack);

	CADORecordset* UploadGetUrgencyWorkerListPack(const GetUrgencyWorkerList_Upload_Pack& askDeviceListUpPack);

	bool UpLoadGetPicDataPack(const GetPicData_Upload_Pack& getPicDataUpPack,
		GPSPIC_Pack& gpsPicPack);

	CADORecordset* UpLoadGetOrgSchedueListPack(const GetOrgSchedueList_Upload_Pack& getOrgSchedueListUpPack);

	bool UpLoadGetOrgSchedueInfoPack(const GetOrgSchedueInfo_Upload_Pack& getOrgSchedueInfoUpPack,
		GetOrgSchedueInfo_Download_Pack& getOrgSchedueInfoDownPack);

	bool UpLoadSetPointPack(const SetPoint_Upload_Pack& setPointUpPack,
		SetPoint_Download_Pack& setPointDownPack);

	CADORecordset* UploadGetOrgLinePack(const GetOrgLine_Upload_Pack& getOrgLineUpPack);

	CADORecordset* UpLoadGetLinePointTimePack(const GetLinePointTime_Upload_Pack& getLinePointTimeUpPack);

	CADORecordset* UpLoadGetOrgSchWorkerPack(const GetOrgSchWorker_Upload_Pack& getOrgSchWorkerUpPack);

	CADORecordset* UploadWorkerPollQureyPack(const WorkerPoll_Upload_Pack& workerPollUpPack);

	bool UpLoadUrgencyMissionDeletePack(const UrgencyMissionDelete_Upload_Pack& urgencyMissionDeleteUpPack,
		UrgencyMissionDelete_Download_Pack& urgencyMissionDeleteDownPack);

	CADORecordset* UploadWorkerPollPack(const WorkerPoll_Upload_Pack& workerPollUpPack);

private:
	string GetCurrentTimeByFormat();

private:

	char          m_strBuffer[INPUTSQLBUFFERS];
	CADODatabase* m_pDatabase;
	CADORecordset m_adoRecordSet;
};

