#pragma once
#include "Datadef.h"

//��¼��֤
int VerifyLogin( CString& aLoginAccount, CString& aLoginPassword, int* orgID, PermissionGroup *pPower);

//��ȡ��¼��Ȩ��
int GetLoginerPermission(const CString& aLoginAccount);

int GetSystemConfigure(SystemConfigure* aGetConfigure);
int SetSystemConfigure(const SystemConfigure* aGetConfigure);

//��ȡ/������·������
int GetRailLine(vector<RailLine*>* aRailLineList);
int SetRailLine(int aCmd, const RailLine* aRailLine);

//��·�߲���
RailLine* GetRailLineByID(vector<RailLine*>& aRailLineList, int aRailID);
RailLine* GetRailLineByName(vector<RailLine*>& aRailLineList, CString aRailName);

//��ȡ/������֯�ṹ
int GetOrgTree(const int aOrgID, vector<OrganizationInfo*>* a_OrgTree);
void CherkOrgType(vector<OrganizationInfo*>* a_OrgTree,OrganizationInfo* OrgInfo);
int SetOrganization(int aCmd, const OrganizationInfo* aOrganization);

//��ȡ/���û���·�����е�
int GetOrgPoint(int aOrgID, vector<RailLine*>& aRailLineList, vector<MapPoint*>* aPointList);
int SetOrgPoint(int aOrgID, int aCmd, const MapPoint* aPoint);

//��ȡ/���û���·��
int GetOrgLine(int aOrgID, const vector<MapPoint*>& aPointList, vector<LineInfo*>* aLineList);
int SetOrgLine(int aOrgID, int aCmd, const LineInfo* aLine);

//��ȡ/����Ա����Ϣ
int GetOrgStaff(int aOrgID, const vector<DeviceInfo*>& aDeviceList, vector<StaffInfo*>* aStaffList);
int SetOrgStaff(int aOrgID, int aCmd, const StaffInfo* aStaff);
int SetStaffPassword(int aOrgID, CString aStaffID, CString aPassword);

//��ȡ/�����Ű��
int GetCalendarSchedule(int aOrgID, const vector<StaffInfo*>* ListStaff, CalendarSchedule* aSchedule);
int SetCalendarSchedule(int aOrgID, const CalendarSchedule* aSchedule);

//��ȡ/���ý�������
int GetEmergencyTask(int aOrgID, vector<EmergencyTaskInfo*>* aEmergency);
int SetEmergencyTask(int aOrgID, int aCmd, const EmergencyTaskInfo* aEmergencyTask);

//ͳ�ƽ�������
int GetEmergencyLogs(int aTaskID, vector<EmergencyLogs*>* aEmergencyStaff);

//��ȡ����������Ա�б�
//��ȡ/�����豸
int GetOrgDevice(int aOrgID, vector<DeviceInfo*>* aDeviceList);
int SetOrgDevice(int aOrgID, int aCmd, const DeviceInfo* aDeviceList);

////��ȡԱ��Ѳ���¼
//int GetStaffCurrentTrack(time_t aDate, RecordStaff* aStaff);


//��ȡͼƬ��Ϣ
int GetPictureInfo(int aOrgID, time_t aStartDate, time_t aEndDate, vector<PictureInfo*>* aPictureList);
int SavePictureToDirect(int aOrgID, const PictureInfo* aPicture, CString aToDirect);

//��ȡԱ��Ѳ����ˮ
int GetStaffScheduleTrack(CString aStaffID, time_t aDate, RecordStaff* aRecord );
//��ȡ�����±���
int GetReportInfoList(int aOrgID, int aYear, int aMonth, vector<ReportInfo*>* aReportList);
//��ȡ����ϸ
int GetReportDetail(CString aStaffID, CString aTime, ReportDetail* aReportList);
//��ȡ�澯�±�
int GetAlarmByMonth(int aOrgID,int aYear, int aMonth, ReportDetail* aAlarmList);

int GetOrgMonthPx(GetOrgPxEx const sValue,vector<getorgpxlistresultEx*> *Value);
int GetOrgMonthxl();
int Getryxjmx();
int GetOrgXl();

CString Time2Strings(time_t sec);
time_t Time2Strings1(CString sec);
time_t Time2Strings2(CString sec);
void StringToChar(char *p,CString sVal);
void StringRelease(CString sVAL);
