#pragma once
#include "Datadef.h"

//登录验证
int VerifyLogin(const CString& aLoginAccount, const CString& aLoginPassword);

//获取登录者权限
int GetLoginerPermission(const CString& aLoginAccount);

//获取/设置组织结构
int GetOrgTree(const CString& aLoginCount, vector<OrganizationInfo*>* aOrgTree);
int SetOrganization(int aCmd, const OrganizationInfo* aOrganization);

//获取/设置机构路线所有点
int GetOrgPoint(int aOrgID, vector<MapPoint*>* aPointList);
int SetOrgPoint(int aOrgID, int aCmd, const MapPoint* aPoint);

//获取/设置机构路线
int GetOrgLine(int aOrgID, const vector<MapPoint*>& aPointList, vector<LineInfo*>* aLineList);
int SetOrgLine(int aOrgID, int aCmd, const LineInfo* aLine);

//获取/设置员工信息
int GetOrgStaff(int aOrgID, vector<StaffInfo*>* aStaffList);
int SetOrgStaff(int aOrgID, int aCmd, const StaffInfo* aStaff);

//获取/设置排班表
int GetCalendarSchedule(int aOrgID, const vector<StaffInfo*>* aStaffList, CalendarSchedule* aSchedule);
int SetCalendarSchedule(int aOrgID, const CalendarSchedule* aSchedule);

//获取/设置紧急任务
int GetEmergencyTask(int aOrgID, vector<EmergencyTaskInfo*>* aEmergency);
int SetEmergencyTask(int aOrgID, int aCmd, const EmergencyTaskInfo* aEmergencyTask);

//获取/设置设备
int GetOrgDevice(int aOrgID, vector<DeviceInfo*>* aDeviceList);
int SetOrgDevice(int aOrgID, int aCmd, const DeviceInfo* aDeviceList);

//获取员工巡查记录
int GetStaffCurrentTrack(time_t aDate, RecordStaff* aStaff);

//获取员工预设巡查路线
int GetStaffScheduleTrack(int aStaffID, time_t aDate, vector<double>* aRecordLon, vector<double>* aRecordLat);

//获取图片信息
int GetPictureInfo(int aOrgID, time_t aStartDate, time_t aEndDate, vector<PictureInfo*>* aPictureList);
int SavePictureToDirect(int aOrgID, const PictureInfo* aPicture, CString aToDirect);
