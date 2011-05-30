#pragma once
#include "Datadef.h"

//获取组织结构
int GetOrgTree(const CString& aLoginCount, vector<OrganizationInfo*>* aOrgTree);

//获取机构路线所有点
int GetOrgPoint(int aOrgID, vector<MapPoint*>* aPointList);
int SetOrgPoint(int aOrgID, int aCmd, const MapPoint* aPoint);

//获取机构路线
int GetOrgLine(int aOrgID, const vector<MapPoint*>* aPointList, vector<LineInfo*>* aLineList);

//获取员工信息
int GetOrgStaff(int aOrgID, vector<StaffInfo*>* aStaffList);

//获取员工巡查记录
int GetStaffCurrentTrack(RecordStaff* aStaff, time_t aDate);
