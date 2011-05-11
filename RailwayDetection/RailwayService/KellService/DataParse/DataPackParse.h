#pragma once
#include "ProtocolDefine.h"

class CDataPackParse
{
public:
	CDataPackParse(void);
	virtual ~CDataPackParse(void);

public:
	// ȱʡ��Ϊbuffer�ĳ���Ϊ1024
	bool PackUpLoadParse(const char* pDataBuffer, GPS_UpLoad_Pack& gpsUpLoadPack);		// �����������ݰ�
	bool PackDownBuild(char* pDataBuffer, const GPS_DownLoad_Pack& gpsDownLoadPack);	// ��ʽ���������ݰ�

private:
	bool ParsePackShell(const char* pDataBuffer, GPS_UpLoad_Pack& gpsUpLoadPack);		// ����ָ��ָ�����ݰ���ǿ�ʼλ��
	bool ParsePackData(const char* pDataBuffer, GPS_UpLoad_Pack& gpsUpLoadPack);		// ����ָ��ָ�����ݰ�����������ʼλ��

	bool BuildPackShell(char* pDataBuffer, const GPS_DownLoad_Pack& gpsDownLoadPack);	// ����ָ��ָ�����ݰ���ǿ�ʼλ��
	bool BuildPackData(char* pDataBuffer, const GPS_DownLoad_Pack& gpsDownLoadPack);	// ����ָ��ָ�����ݰ�����������ʼλ��

};

