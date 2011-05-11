#include "StdAfx.h"
#include "DataPackParse.h"

CDataPackParse::CDataPackParse(void)
{
}

CDataPackParse::~CDataPackParse(void)
{
}

bool CDataPackParse::PackUpLoadParse(const char* pDataBuffer, GPS_UpLoad_Pack& gpsUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParsePackShell(pDataBuffer, gpsUpLoadPack))
		{
			if(0x02 == gpsUpLoadPack.nMsgNumber)
			{
				if(ParsePackData(pDataBuffer + 7, gpsUpLoadPack))
					return true;
			}
		}
	}

	return false;
}

bool CDataPackParse::PackDownBuild(char* pDataBuffer, const GPS_DownLoad_Pack& gpsDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(BuildPackShell(pDataBuffer,gpsDownLoadPack))
		{
			if(BuildPackData(pDataBuffer + 7, gpsDownLoadPack))
				return true;
		}
	}
	return false;
}

bool CDataPackParse::ParsePackShell(const char* pDataBuffer, GPS_UpLoad_Pack& gpsUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		gpsUpLoadPack.nBeginIdentify   = *((unsigned int*)pDataBuffer);
		gpsUpLoadPack.nMsgNumber       = *((unsigned char*)(pDataBuffer + 4));
		gpsUpLoadPack.nBodyLength      = *((unsigned short*)(pDataBuffer + 5));

		if(0xffffffff == gpsUpLoadPack.nBeginIdentify)
			return true;
	}

	return false;
}

bool CDataPackParse::ParsePackData(const char* pDataBuffer, GPS_UpLoad_Pack& gpsUpLoadPack)
{
#define TERM_IDENT_LENGTH							128
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		static unsigned int   noffset;
		static          char  strTerminalNumber[TERM_IDENT_LENGTH];
		static          char  strTime[8]; 

		noffset = *((unsigned char*)pDataBuffer);
		memset(strTerminalNumber, 0x00, TERM_IDENT_LENGTH);
		memset(strTime, 0x00, 8);

		gpsUpLoadPack.gGPSBodyPack.nTerminalIdentifyLength = noffset;
		memcpy(strTerminalNumber, pDataBuffer + 1, noffset);
		gpsUpLoadPack.gGPSBodyPack.strTerminalNumber = strTerminalNumber;
		gpsUpLoadPack.gGPSBodyPack.nLongitude = *((unsigned int*)(pDataBuffer + 1 + noffset));
		gpsUpLoadPack.gGPSBodyPack.nLatitude = *((unsigned int*)(pDataBuffer + 5 + noffset));
		gpsUpLoadPack.gGPSBodyPack.nSpeed = *((unsigned short*)(pDataBuffer + 9 + noffset));
		gpsUpLoadPack.gGPSBodyPack.nDirection = *((unsigned char*)(pDataBuffer + 11 + noffset));
		memcpy(strTime, pDataBuffer + 12 + noffset, 7);
		gpsUpLoadPack.gGPSBodyPack.strTime = strTime;
		gpsUpLoadPack.gGPSBodyPack.nState = *((unsigned char*)(pDataBuffer + 19 + noffset));
		gpsUpLoadPack.gGPSBodyPack.nMileage = *((unsigned int*)(pDataBuffer + 20 + noffset));
		gpsUpLoadPack.gGPSBodyPack.nOilMass = *((unsigned int*)(pDataBuffer + 24 + noffset));
		gpsUpLoadPack.gGPSBodyPack.nIsLocate = *((unsigned char*)(pDataBuffer + 28 + noffset));

		return true;
	}

	return false;
}

bool CDataPackParse::BuildPackShell(char* pDataBuffer, const GPS_DownLoad_Pack& gpsDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)pDataBuffer) = 0xffffffff;
		*((unsigned char*)(pDataBuffer + 1)) = 0x02;
		*((unsigned short*)(pDataBuffer + 5)) = gpsDownLoadPack.nBodyLength;

		return true;
	}
	return false;
}

bool CDataPackParse::BuildPackData(char* pDataBuffer, const GPS_DownLoad_Pack& gpsDownLoadPack)
{
#define TERM_IDENT_LENGTH							128
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		static unsigned int   noffset;
		static          char  strTerminalNumber[TERM_IDENT_LENGTH];
		static          char  strTime[8]; 

		memset(strTerminalNumber, 0x00, TERM_IDENT_LENGTH);
		memset(strTime, 0x00, 8);

		*((unsigned char*)(pDataBuffer)) = gpsDownLoadPack.gGPSBodyPack.nTerminalIdentifyLength;
		memcpy(pDataBuffer + 1, gpsDownLoadPack.gGPSBodyPack.strTerminalNumber.c_str(), gpsDownLoadPack.gGPSBodyPack.nTerminalIdentifyLength);
		memcpy(pDataBuffer + 1 + gpsDownLoadPack.gGPSBodyPack.nTerminalIdentifyLength, gpsDownLoadPack.gGPSBodyPack.strTime.c_str(), 7);
		*((unsigned int*)(pDataBuffer + 7 + gpsDownLoadPack.nBodyLength)) = 0xffffffff;

		return true;
	}
	return false;
}