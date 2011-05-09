#pragma once
#include "ProtocolDefine.h"

class CDataPackParse
{
public:
	CDataPackParse(void);
	virtual ~CDataPackParse(void);

public:
	// 缺省认为buffer的长度为1024
	bool PackParse(char* pDataBuffer, GPS_UpLoad_Pack& gpsUpLoadPack);
	bool PackBuild(char* pDataBuffer, const GPS_DownLoad_Pack& gpsDownLoadPack);

private:

};

