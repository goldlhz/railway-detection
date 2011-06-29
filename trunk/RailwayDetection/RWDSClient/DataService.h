#pragma once
#include "Datadef.h"

//��¼��֤
int VerifyLogin( CString& aLoginAccount, CString& aLoginPassword, int* orgID, Permission *pPower);

//��ȡ��¼��Ȩ��
int GetLoginerPermission(const CString& aLoginAccount);

//��ȡ/������֯�ṹ
int GetOrgTree(const int& OrgId, vector<OrganizationInfo*>* a_OrgTree);
void CherkOrgType(vector<OrganizationInfo*>* a_OrgTree,OrganizationInfo* OrgInfo);
int SetOrganization(int aCmd, const OrganizationInfo* aOrganization);

//��ȡ/���û���·�����е�
int GetOrgPoint(int aOrgID, vector<MapPoint*>* aPointList);
int SetOrgPoint(int aOrgID, int aCmd, const MapPoint* aPoint);

//��ȡ/���û���·��
int GetOrgLine(int aOrgID, const vector<MapPoint*>& aPointList, vector<LineInfo*>* aLineList);
int SetOrgLine(int aOrgID, int aCmd, const LineInfo* aLine);

//��ȡ/����Ա����Ϣ
int GetOrgStaff(int aOrgID, vector<StaffInfo*>* aStaffList);
int SetOrgStaff(int aOrgID, int aCmd, const StaffInfo* aStaff);

//��ȡ/�����Ű��
int GetCalendarSchedule(int aOrgID, const vector<StaffInfo*>* ListStaff, CalendarSchedule* aSchedule);
int SetCalendarSchedule(int aOrgID, const CalendarSchedule* aSchedule);

//��ȡ/���ý�������
int GetEmergencyTask(int aOrgID, vector<EmergencyTaskInfo*>* aEmergency);
int SetEmergencyTask(int aOrgID, int aCmd, const EmergencyTaskInfo* aEmergencyTask);
//��ȡ����������Ա�б�
//��ȡ/�����豸
int GetOrgDevice(int aOrgID, vector<DeviceInfo*>* aDeviceList);
int SetOrgDevice(int aOrgID, int aCmd, const DeviceInfo* aDeviceList);

//��ȡԱ��Ѳ���¼
int GetStaffCurrentTrack(time_t aDate, RecordStaff* aStaff);

//��ȡԱ��Ԥ��Ѳ��·��
int GetStaffScheduleTrack(int aStaffID, time_t aDate, vector<double>* aRecordLon, vector<double>* aRecordLat);
//��ȡͼƬ��Ϣ
int GetPictureInfo(int aOrgID, time_t aStartDate, time_t aEndDate, vector<PictureInfo*>* aPictureList);
int SavePictureToDirect(int aOrgID, const PictureInfo* aPicture, CString aToDirect);
CString Time2Strings(time_t sec);
time_t Time2Strings1(CString sec);
time_t Time2Strings2(CString sec);
void StringToChar(char *p,CString sVal);
void StringRelease(CString sVAL);