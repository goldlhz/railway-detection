#pragma once
#include "ProtocolDefine.h"

class CDataPackParse
{
public:
	CDataPackParse(void);
	virtual ~CDataPackParse(void);

public:
	// 缺省认为buffer的长度为1024
	bool PackUpLoadParse(const char* pDataBuffer, GPS_UpLoad_Pack& gpsUpLoadPack);		// 解析上行数据包
	bool PackDownBuild(char* pDataBuffer, const GPS_DownLoad_Pack& gpsDownLoadPack);	// 格式化下行数据包

private:
	bool ParsePackShell(const char* pDataBuffer, GPS_UpLoad_Pack& gpsUpLoadPack);		// 数据指针指向数据包外壳开始位置
	bool ParsePackData(const char* pDataBuffer, GPS_UpLoad_Pack& gpsUpLoadPack);		// 数据指针指向数据包中数据区开始位置

	bool BuildPackShell(char* pDataBuffer, const GPS_DownLoad_Pack& gpsDownLoadPack);	// 数据指针指向数据包外壳开始位置
	bool BuildPackData(char* pDataBuffer, const GPS_DownLoad_Pack& gpsDownLoadPack);	// 数据指针指向数据包中数据区开始位置

};

