#pragma once
#include "Datadef.h"

//��ȡ��֯�ṹ
int GetOrgTree(const CString& aLoginCount, vector<OrganizationInfo*>* aOrgTree);

//��ȡ����·�����е�
int GetOrgPoint(int aOrgID, vector<MapPoint*>* aPointList);
int SetOrgPoint(int aOrgID, int aCmd, const MapPoint* aPoint);

//��ȡ����·��
int GetOrgLine(int aOrgID, const vector<MapPoint*>* aPointList, vector<LineInfo*>* aLineList);

//��ȡԱ����Ϣ
int GetOrgStaff(int aOrgID, vector<StaffInfo*>* aStaffList);

//��ȡԱ��Ѳ���¼
int GetStaffCurrentTrack(RecordStaff* aStaff, time_t aDate);
