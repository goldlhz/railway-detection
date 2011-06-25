#pragma once
#include <string>
#include <vector>
#include "ProtocolDefine.h"

#define GPS_BUFFER_SIZE						4096
#define TEMP_BUFFER_SIZE					64
using namespace std;
class CDataPackParse
{
public:
	CDataPackParse(void);
	virtual ~CDataPackParse(void);

public:
	unsigned char ParsePackMsgNum(const char* pDataBuffer);								// 解析包的消息号

	// 缺省认为buffer的长度为1024
	bool PackGPSUpParse(const char* pDataBuffer, GPS_UpLoad_Pack& gpsUpLoadPack, 
		DWORD& dNumberOfBytes, string& strGPSContext, string& strTel);	// 解析上行数据包
	bool PackGPSUpPicParse(string& strDestGPSContext, string& strDestGPSPicContext);
	
	int  PackGPSDownBuild(char* pDataBuffer, const GPS_DownLoad_Pack& gpsDownLoadPack);	// 格式化下行数据包
	void FillGPSFailPack(GPS_DownLoad_Pack& gpsDownLoadPack);							// 填写失败时的返回包

	bool PackLoginUpParse(const char* pDataBuffer, Login_UpLoad_Pack& loginUpLoadPack);
	bool PackLoginDownBuild(char* pDataBuffer, const Login_DownLoad_Pack& loginDownLoadPack);
	void FillLoginFailPack(Login_DownLoad_Pack& loginDownLoadPack);

	bool PackAddOrgUpParse(const char* pDataBuffer, AddOrg_UpLoad_Pack& addOrgUpLoadPack);
	bool PackAddOrgDownBuild(char* pDataBuffer, const AddOrg_DownLoad_Pack& addOrgDownLoadPack);
	void FillAddOrgFailPack(AddOrg_DownLoad_Pack& addOrgDownLoadPack);

	bool PackModifyOrgUpParse(const char* pDataBuffer, ModifyOrg_UpLoad_Pack& modifyOrgUpLoadPack);
	bool PackModifyOrgDownBuild(char* pDataBuffer, const ModifyOrg_DownLoad_Pack& modifyOrgDownLoadPack);
	void FillModifyOrgFailPack(ModifyOrg_DownLoad_Pack& modifyOrgDownLoadPack);

	bool PackDeleteOrgUpParse(const char* pDataBuffer, DeleteOrg_UpLoad_Pack& deleteOrgUpLoadPack);
	bool PackDeleteOrgDownBuild(char* pDataBuffer, const DeleteOrg_DownLoad_Pack& deleteOrgDownLoadPack);
	void FillDeleteOrgFailPack(DeleteOrg_DownLoad_Pack& deleteOrgDownLoadPack);

	bool PackAskOrgListUpParse(const char* pDataBuffer, AskOrgList_UpLoad_Pack& askOrgListUpLoadPack);
	bool PackAskOrgListDownBuild(char* pDataBuffer, const AskOrgList_DownLoad_Pack& askOrgListDownLoadPack);
	void FillAskOrgListFailPack(AskOrgList_DownLoad_Pack& askOrgListDownLoadPack);

	bool PackAddWorkerUpParse(const char* pDataBuffer, AddWorker_UpLoad_Pack& addWorkerUpLoadPack);
	bool PackAddWorkerDownBuild(char* pDataBuffer, const AddWorker_DownLoad_Pack& addWorkerDownLoadPack);
	void FillAddWorkerFailPack(AddWorker_DownLoad_Pack& addWorkerDownLoadPack);

	bool PackModifyWorkerUpParse(const char* pDataBuffer, ModifyWorker_UpLoad_Pack& modifyWorkerUpLoadPack);
	bool PackModifyWorkerDownBuild(char* pDataBuffer, ModifyWorker_DownLoad_Pack& modifyWorkerDownLoadPack);
	void FillModifyWorkerFailPack(ModifyWorker_DownLoad_Pack& modifyWorkerDownLoadPack);

	bool PackDeleteWorkerUpParse(const char* pDataBuffer, DeleteWorker_UpLoad_Pack& deleteWorkerUpLoadPack);
	bool PackDeleteWorkerDownBuild(char* pDataBuffer, DeleteWorker_DownLoad_Pack& deleteWorkerDownLoadPack);
	void FillDeleteWorkerFailPack(DeleteWorker_DownLoad_Pack& deleteWorkerDownLoadPack);

	bool PackAskWorkerListUpParse(const char* pDataBuffer, AskWorkerList_UpLoad_Pack& askWorkerListUpLoadPack);
	bool PackAskWorkerListDownBuild(char* pDataBuffer, AskWorkerList_DownLoad_Pack& askWorkerListDownLoadPack);
	void FillAskWorkerListFailPack(AskWorkerList_DownLoad_Pack& askWorkerListDownLoadPack);

	bool PackAddPowerGroupUpParse(const char* pDataBuffer, AddPowerGroup_UpLoad_Pack& addPowerGroupUpLoadPack);
	bool PackAddPowerGroupDownBuild(char* pDataBuffer, AddPowerGroup_DownLoad_Pack& addPowerGroupDownLoadPack);
	void FillAddPowerGroupFailPack(AddPowerGroup_DownLoad_Pack& addPowerGroupDownLoadPack);

	bool PackModifyPowerGroupUpParse(const char* pDataBuffer, ModifyPowerGroup_UpLoad_Pack& modifyPowerGroupUpLoadPack);
	bool PackModifyPowerGroupDownBuild(char* pDataBuffer, ModifyPowerGroup_DownLoad_Pack& modifyPowerGroupDownLoadPack);
	void FillModifyPowerGroupFailPack(ModifyPowerGroup_DownLoad_Pack& modifyPowerGroupDownLoadPack);

	bool PackDeletePowerGroupUpParse(const char* pDataBuffer, DeletePowerGroup_UpLoad_Pack& deletePowerGroupUpLoadPack);
	bool PackDeletePowerGroupDownBuild(char* pDataBuffer, DeletePowerGroup_DownLoad_Pack& deletePowerGroupDownLoadPack);
	void FillDeletePowerGroupFailPack(DeletePowerGroup_DownLoad_Pack& deletePowerGroupDownLoadPack);

	bool PackAskPowerGroupListUpParse(const char* pDataBuffer, AskPowerGroupList_UpLoad_Pack& askPowerGroupListUpLoadPack);
	bool PackAskPowerGroupListDownBuild(char* pDataBuffer, AskPowerGroupList_DownLoad_Pack& askPowerGroupDownLoadPack);
	void FillAskPowerGroupListFailPack(AskPowerGroupList_DownLoad_Pack& askPowerGroupListDownLoadPack);

	bool PackAddDeviceUpParse(const char* pDataBuffer, AddDevice_UpLoad_Pack& addDeviceUpLoadPack);
	bool PackAddDeviceDownBuild(char* pDataBuffer, AddDevice_DownLoad_Pack& addDeviceDownLoadPack);
	void FillAddDeviceFailPack(AddDevice_DownLoad_Pack& addDeviceDownLoadPack);

	bool PackModifyDeviceUpParse(const char* pDataBuffer, ModifyDevice_UpLoad_Pack& modifyDeviceUpLoadPack);
	bool PackModifyDeviceDownBuild(char* pDataBuffer, ModifyDevice_DownLoad_Pack& modifyDeviceDownLoadPack);
	void FillModifyDeviceFailPack(ModifyDevice_DownLoad_Pack& modifyDeviceDownLoadPack);

	bool PackDeleteDeviceUpParse(const char* pDataBuffer, DeleteDevice_UpLoad_Pack& deleteDeviceUpLoadPack);
	bool PackDeleteDeviceDownBuild(char* pDataBuffer, DeleteDevice_DownLoad_Pack& deleteDeviceDownLoadPack);
	void FillDeleteDeviceFailPack(DeleteDevice_DownLoad_Pack& deleteDeviceDownLoadPack);

	bool PackAskDeviceListUpParse(const char* pDataBuffer, AskDeviceList_Upload_Pack& askDeviceListUpLoadPack);
	bool PackAskDeviceListDownBuild(char* pDataBuffer, AskDeviceList_Download_Pack& askDeviceListDownLoadPack);
	void FillAskDeviceListFailPack(AskDeviceList_Download_Pack& askDeviceListDownLoadPack);

	bool PackGetGPSPointUpParse(const char* pDataBuffer, GetGPSPoint_Upload_Pack& getGPSPointUpLoadPack);
	bool PackGetGPSPointDownBuild(char* pDataBuffer, GetGPSPoint_Download_Pack& getGPGPointDowndLoadPack);
	void FillGetGPSPointFailPack(GetGPSPoint_Download_Pack& getGPGPointDowndLoadPack);

	bool PackGetRailWayNameUpParse(const char* pDataBuffer, GetRailWayName_Upload_Pack& getRailWayTableNameUpLoadPack);
	bool PackGetRailWayNameDownBuild(char* pDataBuffer, GetRailWayName_Download_Pack& getRailWayTableNameDownLoadPack);
	void FillGetRailWayNameFailPack(GetRailWayName_Download_Pack& getRailWayTableNameDownLoadPack);

	bool PackSettingLineUpParse(const char* pDataBuffer, SettingLine_Upload_Pack& settingLineUpLoadPack);
	bool PackSettingLineDownBuild(char* pDataBuffer, SettingLine_Download_Pack& settingLineDownLoadPack);
	void FillSettingLineFailPack(SettingLine_Download_Pack& settingLineDownLoadPack);

	bool PackSettingPollingTimeUpParse(const char* pDataBuffer, SettingPollingTime_Upload_Pack& settingPollingTimeUpLoadPack);
	bool PackSettingPollingTimeDownBuild(char* pDataBuffer, SettingPollingTime_Download_Pack& settingPollingTimeDownLoadPack);
	void FillSettingPollingTimeFailPack(SettingPollingTime_Download_Pack& settingPollingTimeDownLoadPack);

	bool PackSettingSchedule5UpParse(const char* pDataBuffer, SettingSchedule5_Upload_Pack& setingSchedule5UpLoadPack);
	bool PackSettingSchedule5DownBuild(char* pDataBuffer, SettingSchedule5_Download_Pack& settingSchdule5DownLoadPack);
	void FillSettingSchedule5FailPack(SettingSchedule5_Download_Pack& settingSchdule5DownLoadPack);

	bool PackSettingSchedule6UpParse(const char* pDataBuffer, SettingSchedule6_Upload_Pack& setingSchedule6UpLoadPack);
	bool PackSettingSchedule6DownBuild(char* pDataBuffer, SettingSchedule6_Download_Pack& settingSchdule6DownLoadPack);
	void FillSettingSchedule6FailPack(SettingSchedule6_Download_Pack& settingSchdule6DownLoadPack);

	bool PackSettingScheduleWorkerUpParse(const char* pDataBuffer, SettingScheduleWorker_Upload_Pack& settingScheduleWorkerUpLoadPack);
	bool PackSettingScheduleWorkerDownBuild(char* pDataBuffer, SettingScheduleWorker_Download_Pack& settingScheduleWorkerDownLoadPack);
	void FillSettingScheduleWorkerFailPack(SettingScheduleWorker_Download_Pack& settingScheduleWorkerDownLoadPack);

	bool PackUrgencyReleaseUpParse(const char* pDataBuffer, UrgencyRelease_Upload_Pack& urgencyReleaseUpLoadPack);
	bool PackUrgencyReleaseDownBuild(char* pDataBuffer, UrgencyRelease_Download_Pack& urgencyReleaseDownloadPack);
	void FillUrgencyReleaseFailPack(UrgencyRelease_Download_Pack& urgencyReleaseDownLoadPack);

	bool PackUrgencyWorkerUpParse(const char* pDataBuffer, UrgencyWorker_Upload_Pack& urgencyWorkerUpLoadPack);
	bool PackUrgencyWorkerDownBuild(char* pDataBuffer, UrgencyWorker_Download_Pack& urgencyWorkerDownLoadPack);
	void FillUrgencyWorkerFailPack(UrgencyWorker_Download_Pack& urgencyWorkerDownLoadPack);
	
	bool PackGetLinePointUpPack(const char* pDataBuffer, GetLinePoint_Upload_Pack& getLinePointUpLoadPack);
	bool PackGetLinePointDownBuild(char* pDataBuffer, GetLinePoint_Download_Pack& getLinePointDownLoadPack);
	void FillGetLinePointFailPack(GetLinePoint_Download_Pack& getLinePointDownLoadPack);

	bool PackGetPicListPackUpPack(const char* pDataBuffer, GetPicList_Upload_Pack& getPicListUpLoadPack);
	bool PackGetPicListPackDownBuild(char* pDataBuffer, GetPicList_Download_Pack& getPicListDownLoadPack);
	void FillGetPicListPackFailPack(GetPicList_Download_Pack& getPicListDownLoadPack);

	bool PackGetUrgencyMissionListUpPack(const char* pDataBuffer, GetUrgencyMissionList_Upload_Pack& getUrgencyMissionListUpLoadPack);
	bool PackGetUrgencyMissionListDownBuild(char* pDataBuffer, GetUrgencyMissionList_Download_Pack& getUrgencyMissionListDownLoadPack);
	void FillGetUrgencyMissionListFailPack(GetUrgencyMissionList_Download_Pack& getUrgencyMissionListDownLoadPack);

	bool PackGetUrgencyWorkerListUpPack(const char* pDataBuffer, GetUrgencyWorkerList_Upload_Pack& getUrgencyWorkerListUpLoadPack);
	bool PackGetUrgencyWorkerListDownBuild(char* pDataBuffer, GetUrgencyWorkerList_Download_Pack& getUrgencyWorkerListDownLoadPack);
	void FillGetUrgencyWorkerListFailPack(GetUrgencyWorkerList_Download_Pack& getUrgencyWorkerListDownLoadPack);

	bool PackGetPicDataUpPack(const char* pDataBuffer, GetPicData_Upload_Pack& getPicDataUpLoadPack);
	bool PackGetPicDataDownBuild(char* pDataBuffer, GetPicData_Download_Pack& getPicDataDownLoadPack);
	void FillGetPicDataFilePack(GetPicData_Download_Pack& getPicDataDownLoadPack);

	bool PackGetOrgSchedueListUpPack(const char* pDataBuffer, GetOrgSchedueList_Upload_Pack& getOrgSchedueListUpLoadPack);
	bool PackGetOrgSchedueListDownBuild(char* pDataBuffer, GetOrgSchedueList_Download_Pack& getOrgSchedueListDownLoadPack);
	void FillGetOrgSchedueListFailPack(GetOrgSchedueList_Download_Pack& getOrgSchedueListDownLoadPack);

	bool PackGetOrgSchedueInfoUpPack(const char* pDataBuffer, GetOrgSchedueInfo_Upload_Pack& getOrgSchedueInfoUpLoadPack);
	bool PackGetOrgSchedueInfoDownBuild(char* pDataBuffer, GetOrgSchedueInfo_Download_Pack& getOrgSchedueInfoDownLoadPack);
	void FillGetOrgSchedueInfoFailPack(GetOrgSchedueInfo_Download_Pack& getOrgSchedueInfoDownLoadPack);
	
	bool PackSetPointUpPack(const char* pDataBuffer, SetPoint_Upload_Pack& setPointUpLoadPack);
	bool PackSetPointDownBuild(char* pDataBuffer, SetPoint_Download_Pack& setPointDownLoadPack);
	void FillSetPointFailPack(SetPoint_Download_Pack& setPointDownLoadPack);

	bool PackGetOrgLineUpPack(const char* pDataBuffer, GetOrgLine_Upload_Pack& getOrgLineUpLoadPack);
	bool PackGetOrgLineDownBuild(char* pDataBuffer, GetOrgLine_Download_Pack& getOrgLineDownLoadPack);
	void FillGetOrgLineFailPack(GetOrgLine_Download_Pack& getOrgLineDownLoadPack);
	
	bool PackGetLinePointTimeUpPack(const char* pDataBuffer, GetLinePointTime_Upload_Pack& getLinePointTimeUpLoadPack);
	bool PackGetLinePointTimeDownBuild(char* pDataBuffer, GetLinePointTime_Download_Pack& getLinePointTimeDownLoadPack);
	void FillGetLinePointTimeFailPack(GetLinePointTime_Download_Pack& getLinePointTimeDownLoadPack);

	bool PackGetOrgSchWorkerUpPack(const char* pDataBuffer, GetOrgSchWorker_Upload_Pack& getOrgSchWorkerUpLoadPack);
	bool PackGetOrgSchWorkerDownBuild(char* pDataBuffer, GetOrgSchWorker_Download_Pack& getOrgSchWorkerDownLoadPack);
	void FillGetOrgSchWorkerFailPack(GetOrgSchWorker_Download_Pack& getOrgSchWorkerDownLoadPack);

	bool PackWorkerPollQueryUpPack(const char* pDataBuffer, WorkerPollQuery_Upload_Pack& workerPollQureyUpLoadPack);
	bool PackWorkerPollQueryDownBuild(char* pDataBuffer, WorkerPollQuery_Download_Pack& workerPollQureyDownLoadPack);
	void FillWorkerPollQueryFailPack(WorkerPollQuery_Download_Pack& workerPollQureyDownLoadPack);

	bool PackUrgencyMissionDeleteUpPack(const char* pDataBuffer, UrgencyMissionDelete_Upload_Pack& urgencyMissionDeleteUpLoadPack);
	bool PackUrgencyMissionDeleteDownBuild(char* pDataBuffer, UrgencyMissionDelete_Download_Pack& urgencyMissionDeleteDownLoadPack);
	void FillUrgencyMissionDeleteFailPack(UrgencyMissionDelete_Download_Pack& urgencyMissionDeleteDownLoadPack);

	bool PackWorkerPollUpPack(const char* pDataBuffer, WorkerPoll_Upload_Pack& workerPollUpLoadPack);
	bool PackWorkerPollDownBuild(char* pDataBuffer, WorkerPoll_Download_Pack& workerPollDownLoadPack);
	void FillWorkerPollFailPack(WorkerPoll_Download_Pack& workerPollDownLoadPack);
private:
	int ParseGPSPackHttpShell(const char* pDataBuffer, char* outBuffer, 
		string& strGPSContext, DWORD& nPackLength, string& strTel);

	template<typename T>
	bool ParseDataPackShell(const char* pDataBuffer, T& dataUpLoadPack);				// 数据指针指向数据包外壳开始位置	
	template<typename T>
	bool BuildDataPackShell(char* pDataBuffer, const T& gpsDownLoadPack, 
		unsigned char cMsgNumber);														// 数据指针指向数据包外壳开始位置

	bool ParseGPSPackData(const char* pDataBuffer, GPS_UpLoad_Pack& gpsUpLoadPack);		// 数据指针指向数据包中数据区开始位置
	bool BuildGPSPackData(char* pDataBuffer, const GPS_DownLoad_Pack& gpsDownLoadPack);	// 数据指针指向数据包中数据区开始位置

	bool ParseLoginPackData(const char* pDataBuffer, Login_UpLoad_Pack& loginUpLoadPack);
	bool BuildLoginPackData(char* pDataBuffer, const Login_DownLoad_Pack& loginDownLoadPack);

	bool ParseAddOrgPackData(const char* pDataBuffer, AddOrg_UpLoad_Pack& addOrgUpLoadPack);
	bool BuildAddOrgPackData(char* pDataBuffer, const AddOrg_DownLoad_Pack& addOrgDownLoadPack);

	bool ParseModifyOrgPackData(const char* pDataBuffer, ModifyOrg_UpLoad_Pack& modifyOrgUpLoadPack);
	bool BuildModifyOrgPackData(char* pDataBuffer, const ModifyOrg_DownLoad_Pack& modifyOrgDownLoadPack);

	bool ParseDeleteOrgPackData(const char* pDataBuffer, DeleteOrg_UpLoad_Pack& deleteOrgUpLoadPack);
	bool BuildDeleteOrgPackData(char* pDataBuffer, const DeleteOrg_DownLoad_Pack& deleteOrgDownLoadPack);

	bool ParseAskOrgListPackData(const char* pDataBuffer, AskOrgList_UpLoad_Pack& askOrgListUpLoadPack);
	bool BuildAskOrgListPackData(char* pDataBuffer, const AskOrgList_DownLoad_Pack& askOrgListDownLoadPack);

	bool ParseAddWorkerPackData(const char* pDataBuffer, AddWorker_UpLoad_Pack& addWorkerUpLoadPack);
	bool BuildAddWorkertPackData(char* pDataBuffer, const AddWorker_DownLoad_Pack& addWorkerDownLoadPack);

	bool ParseModifyWorkerPackData(const char* pDataBuffer, ModifyWorker_UpLoad_Pack& modifyWorkerUpLoadPack);
	bool BuildModifyWorkertPackData(char* pDataBuffer, const ModifyWorker_DownLoad_Pack& modifyWorkerDownLoadPack);

	bool ParseDeleteWorkerPackData(const char* pDataBuffer, DeleteWorker_UpLoad_Pack& deleteWorkerUpLoadPack);
	bool BuildDeleteWorkerPackData(char* pDataBuffer, const DeleteWorker_DownLoad_Pack& deleteWorkerDownLoadPack);

	bool ParseAskWorkerListPackData(const char* pDataBuffer, AskWorkerList_UpLoad_Pack& askWorkerListUpLoadPack);
	bool BuildAskWorkerListPackData(char* pDataBuffer, const AskWorkerList_DownLoad_Pack& askWorkerListDownLoadPack);

	bool ParseAddPowerGroupPackData(const char* pDataBuffer, AddPowerGroup_UpLoad_Pack& addPowerGroupUpLoadPack);
	bool BuildAddPowerGroupPackData(char* pDataBuffer, const AddPowerGroup_DownLoad_Pack& addPowerGroupDownLoadPack);

	bool ParseModifyPowerGroupPackData(const char* pDataBuffer, ModifyPowerGroup_UpLoad_Pack& modifyPowerGroupUpLoadPack);
	bool BuildModifyPowerGroupPackData(char* pDataBuffer, const ModifyPowerGroup_DownLoad_Pack& modifyPowerGroupDownLoadPack);

	bool ParseDeletePowerGroupPackData(const char* pDataBuffer, DeletePowerGroup_UpLoad_Pack& deletePowerGroupUpLoadPack);
	bool BuildDeletePowerGroupPackData(char* pDataBuffer, const DeletePowerGroup_DownLoad_Pack& deletePowerGroupDownLoadPack);

	bool ParseAskPowerGroupListPackData(const char* pDataBuffer, AskPowerGroupList_UpLoad_Pack& askPowerGroupListUpLoadPack);
	bool BuildAskPowerGroupListPackData(char* pDataBuffer, const AskPowerGroupList_DownLoad_Pack& askPowerGroupListDownLoadPack);

	bool ParseAddDevicePackData(const char* pDataBuffer, AddDevice_UpLoad_Pack& addDeviceUpLoadPack);
	bool BuildAddDevicePackData(char* pDataBuffer, const AddDevice_DownLoad_Pack& addDeviceDownLoadPack);

	bool ParseModifyDevicePackData(const char* pDataBuffer, ModifyDevice_UpLoad_Pack& modifyDeviceUpLoadPack);
	bool BuildModifyDevicePackData(char* pDataBuffer, const ModifyDevice_DownLoad_Pack& modifyDeviceDownLoadPack);

	bool ParseDeleteDevicePackData(const char* pDataBuffer, DeleteDevice_UpLoad_Pack& deleteDeviceUpLoadPack);
	bool BuildDeleteDevicePackData(char* pDataBuffer, const DeleteDevice_DownLoad_Pack& deleteDeviceDownLoadPack);

	bool ParseAskDeviceListPackData(const char* pDataBuffer, AskDeviceList_Upload_Pack& askDeviceListUpLoadPack);
	bool BuildAskDeviceListPackData(char* pDataBuffer, const AskDeviceList_Download_Pack& askDeviceListeDownLoadPack);
	
	bool ParseGetGPSPointPackData(const char* pDataBuffer, GetGPSPoint_Upload_Pack& getGPSPointUpLoadPack);
	bool BuildGetGPSPointPackData(char* pDataBuffer, const GetGPSPoint_Download_Pack& getGPSPointDownLoadPack);

	bool ParseGetRailWayNamePackData(const char* pDataBuffer, GetRailWayName_Upload_Pack& getRailWayTableNameUpLoadPack);
	bool BuildGetRailWayNamePackData(char* pDataBuffer, const GetRailWayName_Download_Pack& getRailWayTableNameDownLoadPack);

	bool ParseSettingLinePackData(const char* pDataBuffer, SettingLine_Upload_Pack& settingLineUpLoadPack);
	bool BuildSettingLinePackData(char* pDataBuffer, const SettingLine_Download_Pack& settingLineDownLoadPack);

	bool ParseSettingPollingTimePackData(const char* pDataBuffer, SettingPollingTime_Upload_Pack& settingPollingTimeUpLoadPack);
	bool BuildSettingPollingTimePackData(char* pDataBuffer, const SettingPollingTime_Download_Pack& settingPollingTimeDownLoadPack);

	bool ParseSettingSchedule5PackData(const char* pDataBuffer, SettingSchedule5_Upload_Pack& settingSchedule5UpLoadPack);
	bool BuildSettingSchedule5PackData(char* pDataBuffer, const SettingSchedule5_Download_Pack& settingSchedule5DownLoadPack);

	bool ParseSettingSchedule6PackData(const char* pDataBuffer, SettingSchedule6_Upload_Pack& settingSchedule6UpLoadPack);
	bool BuildSettingSchedule6PackData(char* pDataBuffer, const SettingSchedule6_Download_Pack& settingSchedule6DownLoadPack);

	bool ParseSettingScheduleWorkerPackData(const char* pDataBuffer, SettingScheduleWorker_Upload_Pack& settingScheduleWorkerUpLoadPack);
	bool BuildSettingScheduleWorkerPackData(char* pDataBuffer, const SettingScheduleWorker_Download_Pack& settingScheduleWorkerDownLoadPack);

	bool ParseUrgencyReleasePackData(const char* pDataBuffer, UrgencyRelease_Upload_Pack& urgencyReleaseUpLoadPack);
	bool BuildUrgencyReleasePackData(char* pDataBuffer, const UrgencyRelease_Download_Pack& urgencyReleaseDownLoadPack);

	bool ParseUrgencyWorkerPackData(const char* pDataBuffer, UrgencyWorker_Upload_Pack& urgencyWorkerUpLoadPack);
	bool BuildUrgencyWorkerPackData(char* pDataBuffer, const UrgencyWorker_Download_Pack& urgencyWorkerDownLoadPack);

	bool ParseGetLinePointPackData(const char* pDataBuffer, GetLinePoint_Upload_Pack& getLinePointUpLoadPack);
	bool BuildGetLinePointPackData(char* pDataBuffer, const GetLinePoint_Download_Pack& getLinePointDownLoadPack);

	bool ParseGetPicListPackData(const char* pDataBuffer, GetPicList_Upload_Pack& getPicListUpLoadPack);
	bool BuildGetPicListPackData(char* pDataBuffer, const GetPicList_Download_Pack& getPicListDownLoadPack);

	bool ParseGetUrgencyMissionListPackData(const char* pDataBuffer, GetUrgencyMissionList_Upload_Pack& getUrgencyMissionListDownLoadPack);
	bool BuildGetUrgencyMissionListPackData(char* pDataBuffer, const GetUrgencyMissionList_Download_Pack& getUrgencyMissionListDownLoadPack);

	bool ParseGetUrgencyWorkerListPackData(const char* pDataBuffer, GetUrgencyWorkerList_Upload_Pack& getUrgencyWorkerListUpLoadPack);
	bool BuildGetUrgencyWorkerListPackData(char* pDataBuffer, const GetUrgencyWorkerList_Download_Pack& getUrgencyWorkerListDownLooadPack);

	bool ParseGetPicDataPackData(const char* pDataBuffer, GetPicData_Upload_Pack& getPicDataUpLoadPack);
	bool BuildGetPicDataPackData(char* pDataBuffer, const GetPicData_Download_Pack& getPicDataDownLoadPack);

	bool ParseGetOrgSchedueListPackData(const char* pDataBuffer, GetOrgSchedueList_Upload_Pack& getOrgSchedueListUpLoadPack);
	bool BuildGetOrgSchedueListPackData(char* pDataBuffer, const GetOrgSchedueList_Download_Pack& getOrgShcedueListDownLoadPack);

	bool ParseGetOrgSchedueInfoPackData(const char* pDataBuffer, GetOrgSchedueInfo_Upload_Pack& getOrgSchedueInfoUpLoadPack);
	bool BuildGetOrgSchedueInfoPackData(char* pDataBuffer, const GetOrgSchedueInfo_Download_Pack& getOrgSchedueInfoDownLoadPack);

	bool ParseSetPointPackData(const char* pDataBuffer, SetPoint_Upload_Pack& setPointUpLoadPack);
	bool BuildSetPointPackData(char* pDataBuffer, const SetPoint_Download_Pack& setPointDownLoadPack);

	bool ParseGetOrgLinePackData(const char* pDataBuffer, GetOrgLine_Upload_Pack& getOrgLineUpLoadPack);
	bool BuildGetOrgLinePackData(char* pDataBuffer, const GetOrgLine_Download_Pack& getOrgLineDownLoadPack);

	bool ParseGetLinePointTimePackData(const char* pDataBuffer, GetLinePointTime_Upload_Pack& getLinePointTimeUpLoadPack);
	bool BuildGetLinePointTimePackData(char* pDataBuffer, const GetLinePointTime_Download_Pack& getLinePointTimeDownLoadPack);

	bool ParseGetOrgSchWorkerPackData(const char* pDataBuffer, GetOrgSchWorker_Upload_Pack& getOrgSchWorkerUpLoadPack);
	bool BuildGetOrgSchWorkerPackData(char* pDataBuffer, const GetOrgSchWorker_Download_Pack& getOrgSchWorkerDownLoadPack);

	bool ParseWorkerPollQueryPackData(const char* pDataBuffer, WorkerPollQuery_Upload_Pack& workerPollQueryUpLoadPack);
	bool BuildWorkerPollQueryPackData(char* pDataBuffer, const WorkerPollQuery_Download_Pack& workerPollQueryDownLoadPack);

	bool ParseUrgencyMissionDeletePackData(const char* pDataBuffer, UrgencyMissionDelete_Upload_Pack& urgencyMissionDeleteUpLoadPack);
	bool BuildUrgencyMissionDeletePackData(char* pDataBuffer, const UrgencyMissionDelete_Download_Pack& urgencyMissionDeleteDownLoadPack);

	bool ParseWorkerPollPackData(const char* pDataBuffer, WorkerPoll_Upload_Pack& workerPollUpLoadPack);
	bool BuildWorkerPollPackData(char* pDataBuffer, const WorkerPoll_Download_Pack& workerPollDownLoadPack);
private: 
	unsigned int GetContextLength(vector<string>& vecGPSLine);
	string		 GetContextTel(vector<string>& vecGPSLine);
	int ParseGPSPicDataSite(string& strGPSContext);
	string         m_strFind1;													// 查找  \r\n
	string		   m_strFind2;													// 查找  0xffffffff
	string	       m_strSrcBase64;
	string         m_strDestBase64;
	string         m_strGPSPack;
	vector<string> m_vecGPSLine;
	char           m_bufferGPSPack1[GPS_BUFFER_SIZE];
	char           m_bufferGPSPack2[GPS_BUFFER_SIZE];

	char		   m_tempBuffer1[TEMP_BUFFER_SIZE];
	char		   m_tempBuffer2[TEMP_BUFFER_SIZE];
	char		   m_tempBuffer3[TEMP_BUFFER_SIZE];
	char		   m_tempBuffer4[TEMP_BUFFER_SIZE];
};

