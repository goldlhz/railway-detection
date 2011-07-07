#include "StdAfx.h"
#include <algorithm>
#include "base64.h"
#include "IOCPDefine.h"
#include "CommonFunction.h"
#include "DataPackParse.h"

using namespace std;

CDataPackParse::CDataPackParse(void)
	:m_strFind1("\r\n"),
	m_strFind2(4, (char)(0xff))
{
	m_vecGPSLine.reserve(16);
}

CDataPackParse::~CDataPackParse(void)
{
}

unsigned char CDataPackParse::ParsePackMsgNum(const char* pDataBuffer)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if('P' == *((char*)pDataBuffer))
		{
			return (unsigned char)(0x02);
		}
		else if(PACK_IDENTIFY == *((unsigned int*)pDataBuffer))
		{
			return *((unsigned char*)(pDataBuffer + 4));
		}
	}

	return '\0';
}

bool CDataPackParse::PackGPSUpParse(const char* pDataBuffer, 
	GPS_UpLoad_Pack& gpsUpLoadPack, 
	DWORD& dNumberOfBytes, 
	string& strGPSContext, 
	string& strTel)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_bufferGPSPack1, 0x00, GPS_BUFFER_SIZE);
		memset(m_bufferGPSPack2, 0x00, GPS_BUFFER_SIZE);

		int nPackLength = 0;
		int nFlag = ParseGPSPackHttpShell(pDataBuffer, m_bufferGPSPack1, strGPSContext, dNumberOfBytes, strTel);
		if(0 == nFlag)
		{
			int nBeginIndex;

			m_strSrcBase64 = m_bufferGPSPack1;
			m_strDestBase64 = base64_decode(m_strSrcBase64);
			nBeginIndex = m_strDestBase64.find(m_strFind2, 0);

			if(std::string::npos != nBeginIndex)
			{
				memcpy(m_bufferGPSPack2, m_strDestBase64.c_str() + nBeginIndex, 
					m_strDestBase64.length() - nBeginIndex);

				if(ParseDataPackShell(m_bufferGPSPack2, gpsUpLoadPack))
				{
					if(GPS_PACK == gpsUpLoadPack.nMsgNumber)
					{
						if(ParseGPSPackData(m_bufferGPSPack2 + 7, gpsUpLoadPack))
							return true;
					}
				}
			}
		}
		else if(1 == nFlag)
		{
			return true;
		}
	}

	return false;
}

bool CDataPackParse::PackGPSUpPicParse(string& strDestGPSContext, string& strDestGPSPicContext)
{
	int nSite;
	unsigned int nPicLength;

	nSite = ParseGPSPicDataSite(strDestGPSContext);
	if(-1 != nSite)
	{
		nPicLength = *(unsigned int*)(&strDestGPSContext[nSite]);
		strDestGPSPicContext.append(&strDestGPSContext[nSite + 4], nPicLength);

		return true;
	}

	return false;
}

int CDataPackParse::PackGPSDownBuild(char* pDataBuffer, const GPS_DownLoad_Pack& gpsDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		static string strGPSBack = "HTTP/1.1 200 OK\r\nAccept-Ranges: bytes\r\nCmd: Response\r\nContent-Length: 0\r\nConnection: Keep-Alive\r\n\r\n";

		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		memcpy(pDataBuffer, strGPSBack.c_str(), strGPSBack.length());
		return strGPSBack.length();

	}
	return 0;
}

void CDataPackParse::FillGPSFailPack(GPS_DownLoad_Pack& gpsDownLoadPack)
{
}

bool CDataPackParse::PackLoginUpParse(const char* pDataBuffer, Login_UpLoad_Pack& loginUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, loginUpLoadPack))
		{
			if(LOGIN_PACK == loginUpLoadPack.nMsgNumber)
			{
				if(ParseLoginPackData(pDataBuffer + 7, loginUpLoadPack))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool CDataPackParse::PackLoginDownBuild(char* pDataBuffer, const Login_DownLoad_Pack& loginDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer,loginDownLoadPack, LOGIN_PACK))
		{
			if(BuildLoginPackData(pDataBuffer + 7, loginDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillLoginFailPack(Login_DownLoad_Pack& loginDownLoadPack)
{
	loginDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	loginDownLoadPack.nMsgNumber = LOGIN_PACK;
	loginDownLoadPack.nBodyLength = 20;

	loginDownLoadPack.gDataBodyPack.nResult = 1;
	loginDownLoadPack.gDataBodyPack.nPower1 = 0;
	loginDownLoadPack.gDataBodyPack.nPower2 = 0;
	loginDownLoadPack.gDataBodyPack.nPower3 = 0;
	loginDownLoadPack.gDataBodyPack.nOrgID = 0;
}

bool CDataPackParse::PackAddOrgUpParse(const char* pDataBuffer, AddOrg_UpLoad_Pack& addOrgUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, addOrgUpLoadPack))
		{
			if(ADDORG_PACK == addOrgUpLoadPack.nMsgNumber)
			{
				if(ParseAddOrgPackData(pDataBuffer + 7, addOrgUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackAddOrgDownBuild(char* pDataBuffer, const AddOrg_DownLoad_Pack& addOrgDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer,addOrgDownLoadPack, ADDORG_PACK))
		{
			if(BuildAddOrgPackData(pDataBuffer + 7, addOrgDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillAddOrgFailPack(AddOrg_DownLoad_Pack& addOrgDownLoadPack)
{
	addOrgDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	addOrgDownLoadPack.nMsgNumber = ADDORG_PACK;
	addOrgDownLoadPack.nBodyLength = 4;

	addOrgDownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackModifyOrgUpParse(const char* pDataBuffer, ModifyOrg_UpLoad_Pack& modifyOrgUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, modifyOrgUpLoadPack))
		{
			if(MODIFYORG_PACK == modifyOrgUpLoadPack.nMsgNumber)
			{
				if(ParseModifyOrgPackData(pDataBuffer + 7, modifyOrgUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackModifyOrgDownBuild(char* pDataBuffer, const ModifyOrg_DownLoad_Pack& modifyOrgDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer,modifyOrgDownLoadPack, MODIFYORG_PACK))
		{
			if(BuildModifyOrgPackData(pDataBuffer + 7, modifyOrgDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillModifyOrgFailPack(ModifyOrg_DownLoad_Pack& modifyOrgDownLoadPack)
{
	modifyOrgDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	modifyOrgDownLoadPack.nMsgNumber = MODIFYORG_PACK;
	modifyOrgDownLoadPack.nBodyLength = 4;

	modifyOrgDownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackDeleteOrgUpParse(const char* pDataBuffer, DeleteOrg_UpLoad_Pack& deleteOrgUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, deleteOrgUpLoadPack))
		{
			if(DELETEORG_PACK == deleteOrgUpLoadPack.nMsgNumber)
			{
				if(ParseDeleteOrgPackData(pDataBuffer + 7, deleteOrgUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackDeleteOrgDownBuild(char* pDataBuffer, const DeleteOrg_DownLoad_Pack& deleteOrgDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer,deleteOrgDownLoadPack, DELETEORG_PACK))
		{
			if(BuildDeleteOrgPackData(pDataBuffer + 7, deleteOrgDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillDeleteOrgFailPack(DeleteOrg_DownLoad_Pack& deleteOrgDownLoadPack)
{
	deleteOrgDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	deleteOrgDownLoadPack.nMsgNumber = DELETEORG_PACK;
	deleteOrgDownLoadPack.nBodyLength = 8;

	deleteOrgDownLoadPack.gDataBodyPack.nResult = 1;
	deleteOrgDownLoadPack.gDataBodyPack.nReson = 3;
}

bool CDataPackParse::PackAskOrgListUpParse(const char* pDataBuffer, AskOrgList_UpLoad_Pack& askOrgListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, askOrgListUpLoadPack))
		{
			if(ASKORGLIST_PACK == askOrgListUpLoadPack.nMsgNumber)
			{
				if(ParseAskOrgListPackData(pDataBuffer + 7, askOrgListUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackAskOrgListDownBuild(char* pDataBuffer, const AskOrgList_DownLoad_Pack& askOrgListDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer,askOrgListDownLoadPack, ASKORGLIST_PACK))
		{
			if(BuildAskOrgListPackData(pDataBuffer + 7, askOrgListDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillAskOrgListFailPack(AskOrgList_DownLoad_Pack& askOrgListDownLoadPack)
{
	askOrgListDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	askOrgListDownLoadPack.nMsgNumber = ASKORGLIST_PACK;
	askOrgListDownLoadPack.nBodyLength = 82;

	askOrgListDownLoadPack.gDataBodyPack.nTitlePack = 0;
	askOrgListDownLoadPack.gDataBodyPack.nHadSendPack = 0;
	askOrgListDownLoadPack.gDataBodyPack.nOrgID = 0;
	askOrgListDownLoadPack.gDataBodyPack.strOrgName = "";
	askOrgListDownLoadPack.gDataBodyPack.strAddress = "";
	askOrgListDownLoadPack.gDataBodyPack.nUpOrgID = 0;
	askOrgListDownLoadPack.gDataBodyPack.nLevel = 0;

	return;
}

bool CDataPackParse::PackAddWorkerUpParse(const char* pDataBuffer, AddWorker_UpLoad_Pack& addWorkerUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, addWorkerUpLoadPack))
		{
			if(ADDWORKER_PACK == addWorkerUpLoadPack.nMsgNumber)
			{
				if(ParseAddWorkerPackData(pDataBuffer + 7, addWorkerUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackAddWorkerDownBuild(char* pDataBuffer, const AddWorker_DownLoad_Pack& addWorkerDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer,addWorkerDownLoadPack, ADDWORKER_PACK))
		{
			if(BuildAddWorkertPackData(pDataBuffer + 7, addWorkerDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillAddWorkerFailPack(AddWorker_DownLoad_Pack& addWorkerDownLoadPack)
{
	addWorkerDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	addWorkerDownLoadPack.nMsgNumber = ADDWORKER_PACK;
	addWorkerDownLoadPack.nBodyLength = 4;

	addWorkerDownLoadPack.gDataBodyPack.nResult = 2;
}

bool CDataPackParse::PackModifyWorkerUpParse(const char* pDataBuffer, ModifyWorker_UpLoad_Pack& modifyWorkerUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, modifyWorkerUpLoadPack))
		{
			if(MODIFYWORKER_PACK == modifyWorkerUpLoadPack.nMsgNumber)
			{
				if(ParseModifyWorkerPackData(pDataBuffer + 7, modifyWorkerUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackModifyWorkerDownBuild(char* pDataBuffer, ModifyWorker_DownLoad_Pack& modifyWorkerDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer,modifyWorkerDownLoadPack, MODIFYWORKER_PACK))
		{
			if(BuildModifyWorkertPackData(pDataBuffer + 7, modifyWorkerDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillModifyWorkerFailPack(ModifyWorker_DownLoad_Pack& modifyWorkerDownLoadPack)
{
	modifyWorkerDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	modifyWorkerDownLoadPack.nMsgNumber = MODIFYWORKER_PACK;
	modifyWorkerDownLoadPack.nBodyLength = 4;

	modifyWorkerDownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackDeleteWorkerUpParse(const char* pDataBuffer, DeleteWorker_UpLoad_Pack& deleteWorkerUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, deleteWorkerUpLoadPack))
		{
			if(DELETEWORKER_PACK == deleteWorkerUpLoadPack.nMsgNumber)
			{
				if(ParseDeleteWorkerPackData(pDataBuffer + 7, deleteWorkerUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackDeleteWorkerDownBuild(char* pDataBuffer, DeleteWorker_DownLoad_Pack& deleteWorkerDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer,deleteWorkerDownLoadPack, DELETEWORKER_PACK))
		{
			if(BuildDeleteWorkerPackData(pDataBuffer + 7, deleteWorkerDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillDeleteWorkerFailPack(DeleteWorker_DownLoad_Pack& deleteWorkerDownLoadPack)
{
	deleteWorkerDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	deleteWorkerDownLoadPack.nMsgNumber = DELETEWORKER_PACK;
	deleteWorkerDownLoadPack.nBodyLength = 4;

	deleteWorkerDownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackAskWorkerListUpParse(const char* pDataBuffer, AskWorkerList_UpLoad_Pack& askWorkerListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, askWorkerListUpLoadPack))
		{
			if(ASKWORKERLIST_PACK == askWorkerListUpLoadPack.nMsgNumber)
			{
				if(ParseAskWorkerListPackData(pDataBuffer + 7, askWorkerListUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackAskWorkerListDownBuild(char* pDataBuffer, AskWorkerList_DownLoad_Pack& askWorkerListDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer,askWorkerListDownLoadPack, ASKWORKERLIST_PACK))
		{
			if(BuildAskWorkerListPackData(pDataBuffer + 7, askWorkerListDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillAskWorkerListFailPack(AskWorkerList_DownLoad_Pack& askWorkerListDownLoadPack)
{
	askWorkerListDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	askWorkerListDownLoadPack.nMsgNumber = ASKWORKERLIST_PACK;
	askWorkerListDownLoadPack.nBodyLength = 88;

	askWorkerListDownLoadPack.gDataBodyPack.nTitlePack = 0;
	askWorkerListDownLoadPack.gDataBodyPack.nHadSendPack = 0;
	askWorkerListDownLoadPack.gDataBodyPack.strOper = "";
	askWorkerListDownLoadPack.gDataBodyPack.strName = "";
	askWorkerListDownLoadPack.gDataBodyPack.strTel = "";
	askWorkerListDownLoadPack.gDataBodyPack.nOrgID = 0;
	askWorkerListDownLoadPack.gDataBodyPack.nPowerGroup = 0;
	askWorkerListDownLoadPack.gDataBodyPack.nPda1 = 0;
	askWorkerListDownLoadPack.gDataBodyPack.nPda2 = 0;
	askWorkerListDownLoadPack.gDataBodyPack.nWorkerState = 0;

	return;
}

bool CDataPackParse::PackAddPowerGroupUpParse(const char* pDataBuffer, AddPowerGroup_UpLoad_Pack& addPowerGroupUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, addPowerGroupUpLoadPack))
		{
			if(ADDPOWERGROUP_PACK == addPowerGroupUpLoadPack.nMsgNumber)
			{
				if(ParseAddPowerGroupPackData(pDataBuffer + 7, addPowerGroupUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackAddPowerGroupDownBuild(char* pDataBuffer, AddPowerGroup_DownLoad_Pack& addPowerGroupDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer,addPowerGroupDownLoadPack, ADDPOWERGROUP_PACK))
		{
			if(BuildAddPowerGroupPackData(pDataBuffer + 7, addPowerGroupDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillAddPowerGroupFailPack(AddPowerGroup_DownLoad_Pack& addPowerGroupDownLoadPack)
{
	addPowerGroupDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	addPowerGroupDownLoadPack.nMsgNumber = ADDPOWERGROUP_PACK;
	addPowerGroupDownLoadPack.nBodyLength = 4;

	addPowerGroupDownLoadPack.gDataBodyPack.nResult = 1 ;
}

bool CDataPackParse::PackModifyPowerGroupUpParse(const char* pDataBuffer, ModifyPowerGroup_UpLoad_Pack& modifyPowerGroupUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, modifyPowerGroupUpLoadPack))
		{
			if(MODIFYPOWERGROUP_PACK == modifyPowerGroupUpLoadPack.nMsgNumber)
			{
				if(ParseModifyPowerGroupPackData(pDataBuffer + 7, modifyPowerGroupUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackModifyPowerGroupDownBuild(char* pDataBuffer, ModifyPowerGroup_DownLoad_Pack& modifyPowerGroupDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, modifyPowerGroupDownLoadPack, MODIFYPOWERGROUP_PACK))
		{
			if(BuildModifyPowerGroupPackData(pDataBuffer + 7, modifyPowerGroupDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillModifyPowerGroupFailPack(ModifyPowerGroup_DownLoad_Pack& modifyPowerGroupDownLoadPack)
{
	modifyPowerGroupDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	modifyPowerGroupDownLoadPack.nMsgNumber = MODIFYPOWERGROUP_PACK;
	modifyPowerGroupDownLoadPack.nBodyLength = 4;

	modifyPowerGroupDownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackDeletePowerGroupUpParse(const char* pDataBuffer, DeletePowerGroup_UpLoad_Pack& deletePowerGroupUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, deletePowerGroupUpLoadPack))
		{
			if(DELETEPOWERGROUP_PACK == deletePowerGroupUpLoadPack.nMsgNumber)
			{
				if(ParseDeletePowerGroupPackData(pDataBuffer + 7, deletePowerGroupUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackDeletePowerGroupDownBuild(char* pDataBuffer, DeletePowerGroup_DownLoad_Pack& deletePowerGroupDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, deletePowerGroupDownLoadPack, DELETEPOWERGROUP_PACK))
		{
			if(BuildDeletePowerGroupPackData(pDataBuffer + 7, deletePowerGroupDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillDeletePowerGroupFailPack(DeletePowerGroup_DownLoad_Pack& deletePowerGroupDownLoadPack)
{
	deletePowerGroupDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	deletePowerGroupDownLoadPack.nMsgNumber = DELETEPOWERGROUP_PACK;
	deletePowerGroupDownLoadPack.nBodyLength = 4;

	deletePowerGroupDownLoadPack.gDataBodyPack.nResult = 2;
}

bool CDataPackParse::PackAskPowerGroupListUpParse(const char* pDataBuffer, AskPowerGroupList_UpLoad_Pack& askPowerGroupListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, askPowerGroupListUpLoadPack))
		{
			if(ASKPOWERGROUPLIST_PACK == askPowerGroupListUpLoadPack.nMsgNumber)
			{
				if(ParseAskPowerGroupListPackData(pDataBuffer + 7, askPowerGroupListUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackAskPowerGroupListDownBuild(char* pDataBuffer, AskPowerGroupList_DownLoad_Pack& askPowerGroupDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, askPowerGroupDownLoadPack, ASKPOWERGROUPLIST_PACK))
		{
			if(BuildAskPowerGroupListPackData(pDataBuffer + 7, askPowerGroupDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillAskPowerGroupListFailPack(AskPowerGroupList_DownLoad_Pack& askPowerGroupListDownLoadPack)
{
	askPowerGroupListDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	askPowerGroupListDownLoadPack.nMsgNumber = ASKPOWERGROUPLIST_PACK;
	askPowerGroupListDownLoadPack.nBodyLength = 44;

	askPowerGroupListDownLoadPack.gDataBodyPack.nTitlePack = 0;
	askPowerGroupListDownLoadPack.gDataBodyPack.nHadSendPack = 0;
	askPowerGroupListDownLoadPack.gDataBodyPack.nPowerID = 0;
	askPowerGroupListDownLoadPack.gDataBodyPack.strPowerName = "";
	askPowerGroupListDownLoadPack.gDataBodyPack.nPow1 = 0;
	askPowerGroupListDownLoadPack.gDataBodyPack.nPow2 = 0;
	askPowerGroupListDownLoadPack.gDataBodyPack.nPow3 = 0;
}

bool CDataPackParse::PackAddDeviceUpParse(const char* pDataBuffer, AddDevice_UpLoad_Pack& addDeviceUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, addDeviceUpLoadPack))
		{
			if(ADDDEVICE_PACK == addDeviceUpLoadPack.nMsgNumber)
			{
				if(ParseAddDevicePackData(pDataBuffer + 7, addDeviceUpLoadPack))
					return true;
			}
		}
	}
	return false;
}
bool CDataPackParse::PackAddDeviceDownBuild(char* pDataBuffer, AddDevice_DownLoad_Pack& addDeviceDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, addDeviceDownLoadPack, ADDDEVICE_PACK))
		{
			if(BuildAddDevicePackData(pDataBuffer + 7, addDeviceDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillAddDeviceFailPack(AddDevice_DownLoad_Pack& addDeviceDownLoadPack)
{
	addDeviceDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	addDeviceDownLoadPack.nMsgNumber = ADDDEVICE_PACK;
	addDeviceDownLoadPack.nBodyLength = 4;

	addDeviceDownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackModifyDeviceUpParse(const char* pDataBuffer, ModifyDevice_UpLoad_Pack& modifyDeviceUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, modifyDeviceUpLoadPack))
		{
			if(MODIFYDEVICE_PACK == modifyDeviceUpLoadPack.nMsgNumber)
			{
				if(ParseModifyDevicePackData(pDataBuffer + 7, modifyDeviceUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackModifyDeviceDownBuild(char* pDataBuffer, ModifyDevice_DownLoad_Pack& modifyDeviceDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, modifyDeviceDownLoadPack, MODIFYDEVICE_PACK))
		{
			if(BuildModifyDevicePackData(pDataBuffer + 7, modifyDeviceDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillModifyDeviceFailPack(ModifyDevice_DownLoad_Pack& modifyDeviceDownLoadPack)
{
	modifyDeviceDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	modifyDeviceDownLoadPack.nMsgNumber = MODIFYDEVICE_PACK;
	modifyDeviceDownLoadPack.nBodyLength = 4;

	modifyDeviceDownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackDeleteDeviceUpParse(const char* pDataBuffer, DeleteDevice_UpLoad_Pack& deleteDeviceUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, deleteDeviceUpLoadPack))
		{
			if(DELETEDEVICE_PACK == deleteDeviceUpLoadPack.nMsgNumber)
			{
				if(ParseDeleteDevicePackData(pDataBuffer + 7, deleteDeviceUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackDeleteDeviceDownBuild(char* pDataBuffer, DeleteDevice_DownLoad_Pack& deleteDeviceDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, deleteDeviceDownLoadPack, DELETEDEVICE_PACK))
		{
			if(BuildDeleteDevicePackData(pDataBuffer + 7, deleteDeviceDownLoadPack))
				return true;
		}
	}

	return false;
}

void CDataPackParse::FillDeleteDeviceFailPack(DeleteDevice_DownLoad_Pack& deleteDeviceDownLoadPack)
{
	deleteDeviceDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	deleteDeviceDownLoadPack.nMsgNumber = DELETEDEVICE_PACK;
	deleteDeviceDownLoadPack.nBodyLength = 4;

	deleteDeviceDownLoadPack.gDataBodyPack.nResult = 2;
}

bool CDataPackParse::PackAskDeviceListUpParse(const char* pDataBuffer, AskDeviceList_Upload_Pack& askDeviceListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, askDeviceListUpLoadPack))
		{
			if(ASKDEIVCELIST_PACK == askDeviceListUpLoadPack.nMsgNumber)
			{
				if(ParseAskDeviceListPackData(pDataBuffer + 7, askDeviceListUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackAskDeviceListDownBuild(char* pDataBuffer, AskDeviceList_Download_Pack& askDeviceListDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, askDeviceListDownLoadPack, ASKDEIVCELIST_PACK))
		{
			if(BuildAskDeviceListPackData(pDataBuffer + 7, askDeviceListDownLoadPack))
				return true;
		}
	}
	return false;
}

void CDataPackParse::FillAskDeviceListFailPack(AskDeviceList_Download_Pack& askDeviceListDownLoadPack)
{
	askDeviceListDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	askDeviceListDownLoadPack.nMsgNumber = ASKDEIVCELIST_PACK;
	askDeviceListDownLoadPack.nBodyLength = 40;

	askDeviceListDownLoadPack.gDataBodyPack.nTitlePack = 0;
	askDeviceListDownLoadPack.gDataBodyPack.nHadSendPack = 0;
	askDeviceListDownLoadPack.gDataBodyPack.nDeviceID = 0;
	askDeviceListDownLoadPack.gDataBodyPack.strDevicePID = "";
	askDeviceListDownLoadPack.gDataBodyPack.nDeviceOrg = 0;
	askDeviceListDownLoadPack.gDataBodyPack.nDeviceType = 0;
}

bool CDataPackParse::PackGetGPSPointUpParse(const char* pDataBuffer, GetGPSPoint_Upload_Pack& getGPSPointUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, getGPSPointUpLoadPack))
		{
			if(GETGPSPOINT_PACK == getGPSPointUpLoadPack.nMsgNumber)
			{
				if(ParseGetGPSPointPackData(pDataBuffer + 7, getGPSPointUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackGetGPSPointDownBuild(char* pDataBuffer, GetGPSPoint_Download_Pack& getGPGPointDowndLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, getGPGPointDowndLoadPack, GETGPSPOINT_PACK))
		{
			if(BuildGetGPSPointPackData(pDataBuffer + 7, getGPGPointDowndLoadPack))
				return true;
		}
	}
	return false;
}

void CDataPackParse::FillGetGPSPointFailPack(GetGPSPoint_Download_Pack& getGPGPointDowndLoadPack)
{
	getGPGPointDowndLoadPack.nBeginIdentify = PACK_IDENTIFY;
	getGPGPointDowndLoadPack.nMsgNumber = GETGPSPOINT_PACK;
	getGPGPointDowndLoadPack.nBodyLength =98;

	getGPGPointDowndLoadPack.gDataBodyPack.nTotlePack = 0;
	getGPGPointDowndLoadPack.gDataBodyPack.nHadSendPack = 0;
	getGPGPointDowndLoadPack.gDataBodyPack.vecGPS.clear();
}

bool CDataPackParse::PackGetRailWayNameUpParse(const char* pDataBuffer, GetRailWayName_Upload_Pack& getRailWayTableNameUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, getRailWayTableNameUpLoadPack))
		{
			if(GETRAILWAYTABLENAME_PACK == getRailWayTableNameUpLoadPack.nMsgNumber)
			{
				if(ParseGetRailWayNamePackData(pDataBuffer + 7, getRailWayTableNameUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackGetRailWayNameDownBuild(char* pDataBuffer, GetRailWayName_Download_Pack& getRailWayTableNameDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, getRailWayTableNameDownLoadPack, GETRAILWAYTABLENAME_PACK))
		{
			if(BuildGetRailWayNamePackData(pDataBuffer + 7, getRailWayTableNameDownLoadPack))
				return true;
		}
	}
	return false;
}

void CDataPackParse::FillGetRailWayNameFailPack(GetRailWayName_Download_Pack& getRailWayTableNameDownLoadPack)
{
	getRailWayTableNameDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	getRailWayTableNameDownLoadPack.nMsgNumber = GETRAILWAYTABLENAME_PACK;
	getRailWayTableNameDownLoadPack.nBodyLength =508;

	getRailWayTableNameDownLoadPack.gDataBodyPack.nTotlePack = 0;
	getRailWayTableNameDownLoadPack.gDataBodyPack.nHadSendPack = 0;
	getRailWayTableNameDownLoadPack.gDataBodyPack.strnName = "";
}

bool CDataPackParse::PackSettingLineUpParse(const char* pDataBuffer, SettingLine_Upload_Pack& settingLineUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, settingLineUpLoadPack))
		{
			if(SETTINGLINE_PACK == settingLineUpLoadPack.nMsgNumber)
			{
				if(ParseSettingLinePackData(pDataBuffer + 7, settingLineUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackSettingLineDownBuild(char* pDataBuffer, SettingLine_Download_Pack& settingLineDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, settingLineDownLoadPack, SETTINGLINE_PACK))
		{
			if(BuildSettingLinePackData(pDataBuffer + 7, settingLineDownLoadPack))
				return true;
		}
	}
	return false;
}

void CDataPackParse::FillSettingLineFailPack(SettingLine_Download_Pack& settingLineDownLoadPack)
{
	settingLineDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	settingLineDownLoadPack.nMsgNumber = SETTINGLINE_PACK;
	settingLineDownLoadPack.nBodyLength = 4;

	settingLineDownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackSettingPollingTimeUpParse(const char* pDataBuffer, SettingPollingTime_Upload_Pack& settingPollingTimeUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, settingPollingTimeUpLoadPack))
		{
			if(SETTINGPOLLINGTIME_PACK == settingPollingTimeUpLoadPack.nMsgNumber)
			{
				if(ParseSettingPollingTimePackData(pDataBuffer + 7, settingPollingTimeUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackSettingPollingTimeDownBuild(char* pDataBuffer, SettingPollingTime_Download_Pack& settingPollingTimeDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, settingPollingTimeDownLoadPack, SETTINGPOLLINGTIME_PACK))
		{
			if(BuildSettingPollingTimePackData(pDataBuffer + 7, settingPollingTimeDownLoadPack))
				return true;
		}
	}
	return false;
}

void CDataPackParse::FillSettingPollingTimeFailPack(SettingPollingTime_Download_Pack& settingPollingTimeDownLoadPack)
{
	settingPollingTimeDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	settingPollingTimeDownLoadPack.nMsgNumber = SETTINGPOLLINGTIME_PACK;
	settingPollingTimeDownLoadPack.nBodyLength = 4;

	settingPollingTimeDownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackSettingSchedule5UpParse(const char* pDataBuffer, SettingSchedule5_Upload_Pack& setingSchedule5UpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, setingSchedule5UpLoadPack))
		{
			if(SETTINGSCHEDULE5_PACK == setingSchedule5UpLoadPack.nMsgNumber)
			{
				if(ParseSettingSchedule5PackData(pDataBuffer + 7, setingSchedule5UpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackSettingSchedule5DownBuild(char* pDataBuffer, SettingSchedule5_Download_Pack& settingSchdule5DownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, settingSchdule5DownLoadPack, SETTINGSCHEDULE5_PACK))
		{
			if(BuildSettingSchedule5PackData(pDataBuffer + 7, settingSchdule5DownLoadPack))
				return true;
		}
	}
	return false;
}

void CDataPackParse::FillSettingSchedule5FailPack(SettingSchedule5_Download_Pack& settingSchdule5DownLoadPack)
{
	settingSchdule5DownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	settingSchdule5DownLoadPack.nMsgNumber = SETTINGSCHEDULE5_PACK;
	settingSchdule5DownLoadPack.nBodyLength = 4;

	settingSchdule5DownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackSettingSchedule6UpParse(const char* pDataBuffer, SettingSchedule6_Upload_Pack& setingSchedule6UpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, setingSchedule6UpLoadPack))
		{
			if(SETTINGSCHEDULE6_PACK == setingSchedule6UpLoadPack.nMsgNumber)
			{
				if(ParseSettingSchedule6PackData(pDataBuffer + 7, setingSchedule6UpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackSettingSchedule6DownBuild(char* pDataBuffer, SettingSchedule6_Download_Pack& settingSchdule6DownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, settingSchdule6DownLoadPack, SETTINGSCHEDULE6_PACK))
		{
			if(BuildSettingSchedule6PackData(pDataBuffer + 7, settingSchdule6DownLoadPack))
				return true;
		}
	}
	return false;
}

void CDataPackParse::FillSettingSchedule6FailPack(SettingSchedule6_Download_Pack& settingSchdule6DownLoadPack)
{
	settingSchdule6DownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	settingSchdule6DownLoadPack.nMsgNumber = SETTINGSCHEDULE6_PACK;
	settingSchdule6DownLoadPack.nBodyLength = 4;

	settingSchdule6DownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackSettingScheduleWorkerUpParse(const char* pDataBuffer, SettingScheduleWorker_Upload_Pack& settingScheduleWorkerUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, settingScheduleWorkerUpLoadPack))
		{
			if(SETTINGSCHEDULEWORKER_PACK == settingScheduleWorkerUpLoadPack.nMsgNumber)
			{
				if(ParseSettingScheduleWorkerPackData(pDataBuffer + 7, settingScheduleWorkerUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackSettingScheduleWorkerDownBuild(char* pDataBuffer, SettingScheduleWorker_Download_Pack& settingScheduleWorkerDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, settingScheduleWorkerDownLoadPack, SETTINGSCHEDULEWORKER_PACK))
		{
			if(BuildSettingScheduleWorkerPackData(pDataBuffer + 7, settingScheduleWorkerDownLoadPack))
				return true;
		}
	}
	return false;
}

void CDataPackParse::FillSettingScheduleWorkerFailPack(SettingScheduleWorker_Download_Pack& settingScheduleWorkerDownLoadPack)
{
	settingScheduleWorkerDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	settingScheduleWorkerDownLoadPack.nMsgNumber = SETTINGSCHEDULEWORKER_PACK;
	settingScheduleWorkerDownLoadPack.nBodyLength = 4;

	settingScheduleWorkerDownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackUrgencyReleaseUpParse(const char* pDataBuffer, UrgencyRelease_Upload_Pack& urgencyReleaseUpLoadPack)

{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, urgencyReleaseUpLoadPack))
		{
			if(URGENCYRELEASE_PACK == urgencyReleaseUpLoadPack.nMsgNumber)
			{
				if(ParseUrgencyReleasePackData(pDataBuffer + 7, urgencyReleaseUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackUrgencyReleaseDownBuild(char* pDataBuffer, UrgencyRelease_Download_Pack& urgencyReleaseDownloadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, urgencyReleaseDownloadPack, URGENCYRELEASE_PACK))
		{
			if(BuildUrgencyReleasePackData(pDataBuffer + 7, urgencyReleaseDownloadPack))
				return true;
		}
	}
	return false;
}

void CDataPackParse::FillUrgencyReleaseFailPack(UrgencyRelease_Download_Pack& urgencyReleaseDownLoadPack)
{
	urgencyReleaseDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	urgencyReleaseDownLoadPack.nMsgNumber = URGENCYRELEASE_PACK;
	urgencyReleaseDownLoadPack.nBodyLength = 4;

	urgencyReleaseDownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackUrgencyWorkerUpParse(const char* pDataBuffer, UrgencyWorker_Upload_Pack& urgencyWorkerUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, urgencyWorkerUpLoadPack))
		{
			if(URGENCYWORKER_PACK == urgencyWorkerUpLoadPack.nMsgNumber)
			{
				if(ParseUrgencyWorkerPackData(pDataBuffer + 7, urgencyWorkerUpLoadPack))
					return true;
			}
		}
	}
	return false;
}

bool CDataPackParse::PackUrgencyWorkerDownBuild(char* pDataBuffer, UrgencyWorker_Download_Pack& urgencyWorkerDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, urgencyWorkerDownLoadPack, URGENCYWORKER_PACK))
		{
			if(BuildUrgencyWorkerPackData(pDataBuffer + 7, urgencyWorkerDownLoadPack))
				return true;
		}
	}
	return false;
}

void CDataPackParse::FillUrgencyWorkerFailPack(UrgencyWorker_Download_Pack& urgencyWorkerDownLoadPack)
{
	urgencyWorkerDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	urgencyWorkerDownLoadPack.nMsgNumber = URGENCYWORKER_PACK;
	urgencyWorkerDownLoadPack.nBodyLength = 4;

	urgencyWorkerDownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackGetLinePointUpPack(const char* pDataBuffer, GetLinePoint_Upload_Pack& getLinePointUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, getLinePointUpLoadPack))
		{
			if(GETLINEPOINT_PACK == getLinePointUpLoadPack.nMsgNumber)
			{
				if(ParseGetLinePointPackData(pDataBuffer + 7, getLinePointUpLoadPack))
					return true;
			}
		}
	}
	return false;
}
bool CDataPackParse::PackGetLinePointDownBuild(char* pDataBuffer, GetLinePoint_Download_Pack& getLinePointDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, getLinePointDownLoadPack, GETLINEPOINT_PACK))
		{
			if(BuildGetLinePointPackData(pDataBuffer + 7, getLinePointDownLoadPack))
				return true;
		}
	}
	return false;
}
void CDataPackParse::FillGetLinePointFailPack(GetLinePoint_Download_Pack& getLinePointDownLoadPack)
{
	getLinePointDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	getLinePointDownLoadPack.nMsgNumber = GETLINEPOINT_PACK;
	getLinePointDownLoadPack.nBodyLength = 32;

	getLinePointDownLoadPack.gDataBodyPack.nTotalPacket = 0;
	getLinePointDownLoadPack.gDataBodyPack.nCurrentPacket = 0;
	getLinePointDownLoadPack.gDataBodyPack.fJDU = 0;
	getLinePointDownLoadPack.gDataBodyPack.fWDU = 0;
	getLinePointDownLoadPack.gDataBodyPack.fGls = 0;
	getLinePointDownLoadPack.gDataBodyPack.nState = 0;
	getLinePointDownLoadPack.gDataBodyPack.nLineID = 0;
}

bool CDataPackParse::PackGetPicListPackUpPack(const char* pDataBuffer, GetPicList_Upload_Pack& getPicListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, getPicListUpLoadPack))
		{
			if(GETPICLIST_PACK == getPicListUpLoadPack.nMsgNumber)
			{
				if(ParseGetPicListPackData(pDataBuffer + 7, getPicListUpLoadPack))
					return true;
			}
		}
	}
	return false;
}
bool CDataPackParse::PackGetPicListPackDownBuild(char* pDataBuffer, GetPicList_Download_Pack& getPicListDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, getPicListDownLoadPack, GETPICLIST_PACK))
		{
			if(BuildGetPicListPackData(pDataBuffer + 7, getPicListDownLoadPack))
				return true;
		}
	}
	return false;
}
void CDataPackParse::FillGetPicListPackFailPack(GetPicList_Download_Pack& getPicListDownLoadPack)
{
	getPicListDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	getPicListDownLoadPack.nMsgNumber = GETPICLIST_PACK;
	getPicListDownLoadPack.nBodyLength = 82;

	getPicListDownLoadPack.gDataBodyPack.nTotalPacket = 0;
	getPicListDownLoadPack.gDataBodyPack.nCurrentPacket = 0;
	getPicListDownLoadPack.gDataBodyPack.strName = "";
	getPicListDownLoadPack.gDataBodyPack.strTime = "";
	getPicListDownLoadPack.gDataBodyPack.nType = 0;
	getPicListDownLoadPack.gDataBodyPack.strTel = "";
}

bool CDataPackParse::PackGetUrgencyMissionListUpPack(const char* pDataBuffer, GetUrgencyMissionList_Upload_Pack& getUrgencyMissionListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, getUrgencyMissionListUpLoadPack))
		{
			if(GETURGENCYMISSIONLIST_PACK == getUrgencyMissionListUpLoadPack.nMsgNumber)
			{
				if(ParseGetUrgencyMissionListPackData(pDataBuffer + 7, getUrgencyMissionListUpLoadPack))
					return true;
			}
		}
	}
	return false;
}
bool CDataPackParse::PackGetUrgencyMissionListDownBuild(char* pDataBuffer, GetUrgencyMissionList_Download_Pack& getUrgencyMissionListDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, getUrgencyMissionListDownLoadPack, GETURGENCYMISSIONLIST_PACK))
		{
			if(BuildGetUrgencyMissionListPackData(pDataBuffer + 7, getUrgencyMissionListDownLoadPack))
				return true;
		}
	}
	return false;
}
void CDataPackParse::FillGetUrgencyMissionListFailPack(GetUrgencyMissionList_Download_Pack& getUrgencyMissionListDownLoadPack)
{
	getUrgencyMissionListDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	getUrgencyMissionListDownLoadPack.nMsgNumber = GETURGENCYMISSIONLIST_PACK;
	getUrgencyMissionListDownLoadPack.nBodyLength = 118;

	getUrgencyMissionListDownLoadPack.gDataBodyPack.nTtlePacket = 0;
	getUrgencyMissionListDownLoadPack.gDataBodyPack.nCurrentPacket = 0;
	getUrgencyMissionListDownLoadPack.gDataBodyPack.strSTime = "";
	getUrgencyMissionListDownLoadPack.gDataBodyPack.strETime = "";
	getUrgencyMissionListDownLoadPack.gDataBodyPack.strRWName= "";
	getUrgencyMissionListDownLoadPack.gDataBodyPack.nState = 0;
	getUrgencyMissionListDownLoadPack.gDataBodyPack.nID = 0;
	getUrgencyMissionListDownLoadPack.gDataBodyPack.nLineID = 0;
	getUrgencyMissionListDownLoadPack.gDataBodyPack.nSPoint = 0;
	getUrgencyMissionListDownLoadPack.gDataBodyPack.nEPoint = 0;
}

bool CDataPackParse::PackGetUrgencyWorkerListUpPack(const char* pDataBuffer, GetUrgencyWorkerList_Upload_Pack& getUrgencyWorkerListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, getUrgencyWorkerListUpLoadPack))
		{
			if(GETURGENCYWORKERLIST_PACK == getUrgencyWorkerListUpLoadPack.nMsgNumber)
			{
				if(ParseGetUrgencyWorkerListPackData(pDataBuffer + 7, getUrgencyWorkerListUpLoadPack))
					return true;
			}
		}
	}
	return false;
}
bool CDataPackParse::PackGetUrgencyWorkerListDownBuild(char* pDataBuffer, GetUrgencyWorkerList_Download_Pack& getUrgencyWorkerListDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, getUrgencyWorkerListDownLoadPack, GETURGENCYWORKERLIST_PACK))
		{
			if(BuildGetUrgencyWorkerListPackData(pDataBuffer + 7, getUrgencyWorkerListDownLoadPack))
				return true;
		}
	}
	return false;
}
void CDataPackParse::FillGetUrgencyWorkerListFailPack(GetUrgencyWorkerList_Download_Pack& getUrgencyWorkerListDownLoadPack)
{
	getUrgencyWorkerListDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	getUrgencyWorkerListDownLoadPack.nMsgNumber = GETURGENCYWORKERLIST_PACK;
	getUrgencyWorkerListDownLoadPack.nBodyLength = 137;

	getUrgencyWorkerListDownLoadPack.gDataBodyPack.nTtlePacket = 0;
	getUrgencyWorkerListDownLoadPack.gDataBodyPack.nCurrentPacket = 0;
	getUrgencyWorkerListDownLoadPack.gDataBodyPack.nJj_long = 0;
	getUrgencyWorkerListDownLoadPack.gDataBodyPack.strJj_Pid = "";
	getUrgencyWorkerListDownLoadPack.gDataBodyPack.nJj_OrgID = 0;
	getUrgencyWorkerListDownLoadPack.gDataBodyPack.strJj_Name = "";
	getUrgencyWorkerListDownLoadPack.gDataBodyPack.strJj_Time ="";
	getUrgencyWorkerListDownLoadPack.gDataBodyPack.nJj_long = 0;
}

bool CDataPackParse::PackGetPicDataUpPack(const char* pDataBuffer, GetPicData_Upload_Pack& getPicDataUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, getPicDataUpLoadPack))
		{
			if(GETPICDATA_PACK == getPicDataUpLoadPack.nMsgNumber)
			{
				if(ParseGetPicDataPackData(pDataBuffer + 7, getPicDataUpLoadPack))
					return true;
			}
		}
	}
	return false;
}
bool CDataPackParse::PackGetPicDataDownBuild(char* pDataBuffer, GetPicData_Download_Pack& getPicDataDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, getPicDataDownLoadPack, GETPICDATA_PACK))
		{
			if(BuildGetPicDataPackData(pDataBuffer + 7, getPicDataDownLoadPack))
				return true;
		}
	}
	return false;
}
void CDataPackParse::FillGetPicDataFilePack(GetPicData_Download_Pack& getPicDataDownLoadPack)
{
	getPicDataDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	getPicDataDownLoadPack.nMsgNumber = GETPICDATA_PACK;
	getPicDataDownLoadPack.nBodyLength = 1024*3+12;

	getPicDataDownLoadPack.gDataBodyPack.nTtlePacket = 0;
	getPicDataDownLoadPack.gDataBodyPack.nCurrentPacket = 0;
	getPicDataDownLoadPack.gDataBodyPack.nPagesize = 0;
}

bool CDataPackParse::PackGetOrgSchedueListUpPack(const char* pDataBuffer, GetOrgSchedueList_Upload_Pack& getOrgSchedueListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		if(ParseDataPackShell(pDataBuffer, getOrgSchedueListUpLoadPack))
		{
			if(GETORGSCHEDUELIST_PACK == getOrgSchedueListUpLoadPack.nMsgNumber)
			{
				if(ParseGetOrgSchedueListPackData(pDataBuffer + 7, getOrgSchedueListUpLoadPack))
					return true;
			}
		}
	}
	return false;
}
bool CDataPackParse::PackGetOrgSchedueListDownBuild(char* pDataBuffer, GetOrgSchedueList_Download_Pack& getOrgSchedueListDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
		if(BuildDataPackShell(pDataBuffer, getOrgSchedueListDownLoadPack, GETORGSCHEDUELIST_PACK))
		{
			if(BuildGetOrgSchedueListPackData(pDataBuffer + 7, getOrgSchedueListDownLoadPack))
				return true;
		}
	}
	return false;
}
void CDataPackParse::FillGetOrgSchedueListFailPack(GetOrgSchedueList_Download_Pack& getOrgSchedueListDownLoadPack)
{
	getOrgSchedueListDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	getOrgSchedueListDownLoadPack.nMsgNumber = GETORGSCHEDUELIST_PACK;
	getOrgSchedueListDownLoadPack.nBodyLength = 84;

	getOrgSchedueListDownLoadPack.gDataBodyPack.nTotlePacket = 0;
	getOrgSchedueListDownLoadPack.gDataBodyPack.nCurrentPacket = 0;
	getOrgSchedueListDownLoadPack.gDataBodyPack.strUserID = "";
	getOrgSchedueListDownLoadPack.gDataBodyPack.strDates = "";
	getOrgSchedueListDownLoadPack.gDataBodyPack.strXJ = "";
	getOrgSchedueListDownLoadPack.gDataBodyPack.nSGetPoint = 0;
	getOrgSchedueListDownLoadPack.gDataBodyPack.nCPoint = 0;
	getOrgSchedueListDownLoadPack.gDataBodyPack.nLPoint = 0;
}

bool CDataPackParse::PackGetOrgSchedueInfoUpPack(const char* pDataBuffer, GetOrgSchedueInfo_Upload_Pack& getOrgSchedueInfoUpLoadPack)
{
	if(ParseDataPackShell(pDataBuffer, getOrgSchedueInfoUpLoadPack))
	{
		if(GETORGSCHEDUEINFO_PACK == getOrgSchedueInfoUpLoadPack.nMsgNumber)
		{
			if(ParseGetOrgSchedueInfoPackData(pDataBuffer + 7, getOrgSchedueInfoUpLoadPack))
				return true;
		}
	}
	return false;
}

bool CDataPackParse::PackGetOrgSchedueInfoDownBuild(char* pDataBuffer, GetOrgSchedueInfo_Download_Pack& getOrgSchedueInfoDownLoadPack)
{
	memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
	if(BuildDataPackShell(pDataBuffer, getOrgSchedueInfoDownLoadPack, GETORGSCHEDUEINFO_PACK))
	{
		if(BuildGetOrgSchedueInfoPackData(pDataBuffer + 7, getOrgSchedueInfoDownLoadPack))
			return true;
	}
	return false;
}

void CDataPackParse::FillGetOrgSchedueInfoFailPack(GetOrgSchedueInfo_Download_Pack& getOrgSchedueInfoDownLoadPack)
{
	getOrgSchedueInfoDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	getOrgSchedueInfoDownLoadPack.nMsgNumber = GETORGSCHEDUEINFO_PACK;
	getOrgSchedueInfoDownLoadPack.nBodyLength = 78;

	getOrgSchedueInfoDownLoadPack.gDataBodyPack.nID = 0;
	getOrgSchedueInfoDownLoadPack.gDataBodyPack.nCYC = 0;
	getOrgSchedueInfoDownLoadPack.gDataBodyPack.strTime = "";
	getOrgSchedueInfoDownLoadPack.gDataBodyPack.strContent = "";
}

bool CDataPackParse::PackSetPointUpPack(const char* pDataBuffer, SetPoint_Upload_Pack& setPointUpLoadPack)
{
	if(ParseDataPackShell(pDataBuffer, setPointUpLoadPack))
	{
		if(SETPOINT_PACK == setPointUpLoadPack.nMsgNumber)
		{
			if(ParseSetPointPackData(pDataBuffer + 7, setPointUpLoadPack))
				return true;
		}
	}
	return false;
}
bool CDataPackParse::PackSetPointDownBuild(char* pDataBuffer, SetPoint_Download_Pack& setPointDownLoadPack)
{
	memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
	if(BuildDataPackShell(pDataBuffer, setPointDownLoadPack, SETPOINT_PACK))
	{
		if(BuildSetPointPackData(pDataBuffer + 7, setPointDownLoadPack))
			return true;
	}
	return false;
}
void CDataPackParse::FillSetPointFailPack(SetPoint_Download_Pack& setPointDownLoadPack)
{
	setPointDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	setPointDownLoadPack.nMsgNumber = SETPOINT_PACK;
	setPointDownLoadPack.nBodyLength = 42;

	setPointDownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackGetOrgLineUpPack(const char* pDataBuffer, GetOrgLine_Upload_Pack& getOrgLineUpLoadPack)
{
	if(ParseDataPackShell(pDataBuffer, getOrgLineUpLoadPack))
	{
		if(GETORGLINE_PACK == getOrgLineUpLoadPack.nMsgNumber)
		{
			if(ParseGetOrgLinePackData(pDataBuffer + 7, getOrgLineUpLoadPack))
				return true;
		}
	}
	return false;
}

bool CDataPackParse::PackGetOrgLineDownBuild(char* pDataBuffer, GetOrgLine_Download_Pack& getOrgLineDownLoadPack)
{
	memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
	if(BuildDataPackShell(pDataBuffer, getOrgLineDownLoadPack, GETORGLINE_PACK))
	{
		if(BuildGetOrgLinePackData(pDataBuffer + 7, getOrgLineDownLoadPack))
			return true;
	}
	return false;
}

void CDataPackParse::FillGetOrgLineFailPack(GetOrgLine_Download_Pack& getOrgLineDownLoadPack)
{
	getOrgLineDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	getOrgLineDownLoadPack.nMsgNumber = GETORGLINE_PACK;
	getOrgLineDownLoadPack.nBodyLength = 46;

	getOrgLineDownLoadPack.gDataBodyPack.nCurrentPacket = 0;
	getOrgLineDownLoadPack.gDataBodyPack.nTtotlePacket = 0;
	getOrgLineDownLoadPack.gDataBodyPack.nLineDay = 0;
	getOrgLineDownLoadPack.gDataBodyPack.nLineID = 0;
	getOrgLineDownLoadPack.gDataBodyPack.strLineName = "";
}

bool CDataPackParse::PackGetLinePointTimeUpPack(const char* pDataBuffer, GetLinePointTime_Upload_Pack& getLinePointTimeUpLoadPack)
{
	if(ParseDataPackShell(pDataBuffer, getLinePointTimeUpLoadPack))
	{
		if(GETLINEPOINTTIME_PACK == getLinePointTimeUpLoadPack.nMsgNumber)
		{
			if(ParseGetLinePointTimePackData(pDataBuffer + 7, getLinePointTimeUpLoadPack))
				return true;
		}
	}
	return false;
}

bool CDataPackParse::PackGetLinePointTimeDownBuild(char* pDataBuffer, GetLinePointTime_Download_Pack& getLinePointTimeDownLoadPack)
{
	memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
	if(BuildDataPackShell(pDataBuffer, getLinePointTimeDownLoadPack, GETLINEPOINTTIME_PACK))
	{
		if(BuildGetLinePointTimePackData(pDataBuffer + 7, getLinePointTimeDownLoadPack))
			return true;
	}
	return false;
}

void CDataPackParse::FillGetLinePointTimeFailPack(GetLinePointTime_Download_Pack& getLinePointTimeDownLoadPack)
{
	getLinePointTimeDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	getLinePointTimeDownLoadPack.nMsgNumber = GETLINEPOINTTIME_PACK;
	getLinePointTimeDownLoadPack.nBodyLength = 48;

	getLinePointTimeDownLoadPack.gDataBodyPack.nTotlePacket = 0;
	getLinePointTimeDownLoadPack.gDataBodyPack.nCurrentPacket = 0;
	getLinePointTimeDownLoadPack.gDataBodyPack.fIKM = 0;
	getLinePointTimeDownLoadPack.gDataBodyPack.fILat = 0;
	getLinePointTimeDownLoadPack.gDataBodyPack.fILon = 0;
	getLinePointTimeDownLoadPack.gDataBodyPack.nIDirect = 0;
	getLinePointTimeDownLoadPack.gDataBodyPack.nPointID = 0;
	getLinePointTimeDownLoadPack.gDataBodyPack.strTime = "";
}

bool CDataPackParse::PackGetOrgSchWorkerUpPack(const char* pDataBuffer, GetOrgSchWorker_Upload_Pack& getOrgSchWorkerUpLoadPack)
{
	if(ParseDataPackShell(pDataBuffer, getOrgSchWorkerUpLoadPack))
	{
		if(GETORGSCHWORKER_PACK == getOrgSchWorkerUpLoadPack.nMsgNumber)
		{
			if(ParseGetOrgSchWorkerPackData(pDataBuffer + 7, getOrgSchWorkerUpLoadPack))
				return true;
		}
	}
	return false;
}

bool CDataPackParse::PackGetOrgSchWorkerDownBuild(char* pDataBuffer, GetOrgSchWorker_Download_Pack& getOrgSchWorkerDownLoadPack)
{
	memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
	if(BuildDataPackShell(pDataBuffer, getOrgSchWorkerDownLoadPack, GETORGSCHWORKER_PACK))
	{
		if(BuildGetOrgSchWorkerPackData(pDataBuffer + 7, getOrgSchWorkerDownLoadPack))
			return true;
	}
	return false;
}

void CDataPackParse::FillGetOrgSchWorkerFailPack(GetOrgSchWorker_Download_Pack& getOrgSchWorkerDownLoadPack)
{
	getOrgSchWorkerDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	getOrgSchWorkerDownLoadPack.nMsgNumber = GETORGSCHWORKER_PACK;
	getOrgSchWorkerDownLoadPack.nBodyLength = 88;

	getOrgSchWorkerDownLoadPack.gDataBodyPack.nTotle = 0;
	getOrgSchWorkerDownLoadPack.gDataBodyPack.nHasRecv = 0;
	getOrgSchWorkerDownLoadPack.gDataBodyPack.strOper = "";
	getOrgSchWorkerDownLoadPack.gDataBodyPack.strName = "";
	getOrgSchWorkerDownLoadPack.gDataBodyPack.strTel = "";
	getOrgSchWorkerDownLoadPack.gDataBodyPack.nOrgID = 0;
	getOrgSchWorkerDownLoadPack.gDataBodyPack.nPowerGroup = 0;
	getOrgSchWorkerDownLoadPack.gDataBodyPack.nPda1 = 0;
	getOrgSchWorkerDownLoadPack.gDataBodyPack.nPda2 = 0;
	getOrgSchWorkerDownLoadPack.gDataBodyPack.nWorkerState = 0;
}

bool CDataPackParse::PackWorkerPollQueryUpPack(const char* pDataBuffer, WorkerPollQuery_Upload_Pack& workerPollQureyUpLoadPack)
{
	if(ParseDataPackShell(pDataBuffer, workerPollQureyUpLoadPack))
	{
		if(WORKERPOLLQUERY_PACK == workerPollQureyUpLoadPack.nMsgNumber)
		{
			if(ParseWorkerPollQueryPackData(pDataBuffer + 7, workerPollQureyUpLoadPack))
				return true;
		}
	}
	return false;
}
bool CDataPackParse::PackWorkerPollQueryDownBuild(char* pDataBuffer, WorkerPollQuery_Download_Pack& workerPollQureyDownLoadPack)
{
	memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
	if(BuildDataPackShell(pDataBuffer, workerPollQureyDownLoadPack, WORKERPOLLQUERY_PACK))
	{
		if(BuildWorkerPollQueryPackData(pDataBuffer + 7, workerPollQureyDownLoadPack))
			return true;
	}
	return false;
}
void CDataPackParse::FillWorkerPollQueryFailPack(WorkerPollQuery_Download_Pack& workerPollQureyDownLoadPack)
{
	workerPollQureyDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	workerPollQureyDownLoadPack.nMsgNumber = WORKERPOLLQUERY_PACK;
	workerPollQureyDownLoadPack.nBodyLength = 36;

	workerPollQureyDownLoadPack.gDataBodyPack.nTotlePacket = 0;
	workerPollQureyDownLoadPack.gDataBodyPack.nCurrentPacket = 0;
	workerPollQureyDownLoadPack.gDataBodyPack.fJUD = 0;
	workerPollQureyDownLoadPack.gDataBodyPack.fWDU = 0;
	workerPollQureyDownLoadPack.gDataBodyPack.strTime = "";
}

bool CDataPackParse::PackUrgencyMissionDeleteUpPack(const char* pDataBuffer, UrgencyMissionDelete_Upload_Pack& urgencyMissionDeleteUpLoadPack)
{
	if(ParseDataPackShell(pDataBuffer, urgencyMissionDeleteUpLoadPack))
	{
		if(URGENCYMISSIONDELETE_PACK == urgencyMissionDeleteUpLoadPack.nMsgNumber)
		{
			if(ParseUrgencyMissionDeletePackData(pDataBuffer + 7, urgencyMissionDeleteUpLoadPack))
				return true;
		}
	}
	return false;
}
bool CDataPackParse::PackUrgencyMissionDeleteDownBuild(char* pDataBuffer, UrgencyMissionDelete_Download_Pack& urgencyMissionDeleteDownLoadPack)
{
	memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
	if(BuildDataPackShell(pDataBuffer, urgencyMissionDeleteDownLoadPack, URGENCYMISSIONDELETE_PACK))
	{
		if(BuildUrgencyMissionDeletePackData(pDataBuffer + 7, urgencyMissionDeleteDownLoadPack))
			return true;
	}
	return false;
}
void CDataPackParse::FillUrgencyMissionDeleteFailPack(UrgencyMissionDelete_Download_Pack& urgencyMissionDeleteDownLoadPack)
{
	urgencyMissionDeleteDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	urgencyMissionDeleteDownLoadPack.nMsgNumber = URGENCYMISSIONDELETE_PACK;
	urgencyMissionDeleteDownLoadPack.nBodyLength = 4;

	urgencyMissionDeleteDownLoadPack.gDataBodyPack.nResult = 1;
}

bool CDataPackParse::PackWorkerPollUpPack(const char* pDataBuffer, WorkerPoll_Upload_Pack& workerPollUpLoadPack)
{
	if(ParseDataPackShell(pDataBuffer, workerPollUpLoadPack))
	{
		if(WORKERPOLL_PACK == workerPollUpLoadPack.nMsgNumber)
		{
			if(ParseWorkerPollPackData(pDataBuffer + 7, workerPollUpLoadPack))
				return true;
		}
	}
	return false;
}
bool CDataPackParse::PackWorkerPollDownBuild(char* pDataBuffer, WorkerPoll_Download_Pack& workerPollDownLoadPack)
{
	memset(pDataBuffer, 0x00, BUFFER_SIZE_TO_CLIENT);
	if(BuildDataPackShell(pDataBuffer, workerPollDownLoadPack, WORKERPOLL_PACK))
	{
		if(BuildWorkerPollPackData(pDataBuffer + 7, workerPollDownLoadPack))
			return true;
	}
	return false;
}
void CDataPackParse::FillWorkerPollFailPack(WorkerPoll_Download_Pack& workerPollDownLoadPack)
{
	workerPollDownLoadPack.nBeginIdentify = PACK_IDENTIFY;
	workerPollDownLoadPack.nMsgNumber = WORKERPOLL_PACK;
	workerPollDownLoadPack.nBodyLength = 96;

	workerPollDownLoadPack.gDataBodyPack.nTotlePacket = 0;
	workerPollDownLoadPack.gDataBodyPack.nCurrentPacket = 0;
	workerPollDownLoadPack.gDataBodyPack.nPointID = 0;
	workerPollDownLoadPack.gDataBodyPack.strArrtime = "";
	workerPollDownLoadPack.gDataBodyPack.strDate = "";
	workerPollDownLoadPack.gDataBodyPack.strPID = "";
	workerPollDownLoadPack.gDataBodyPack.strRealTime = "";
	workerPollDownLoadPack.gDataBodyPack.nPointState = 0;
}

int CDataPackParse::ParseGPSPackHttpShell(const char* pDataBuffer, char* outBuffer, 
	string& strGPSContext, DWORD& nPackLength, string& strTel)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		int nOffBegin = 0, nOffEnd = 0;
		m_strGPSPack = pDataBuffer;
		m_vecGPSLine.clear();

		for(int nIndex = 0; nIndex < 11; ++nIndex)
		{
			nOffEnd = m_strGPSPack.find(m_strFind1, nOffBegin);
			if(string::npos == nOffEnd)
			{
				if(0 != nOffBegin)
					m_vecGPSLine.push_back(m_strGPSPack.substr(nOffBegin));
				break;
			}

			m_vecGPSLine.push_back(m_strGPSPack.substr(nOffBegin, nOffEnd - nOffBegin));
			nOffBegin = nOffEnd +  m_strFind1.length();
		}

		if(m_vecGPSLine.size() >= 11)
		{
			nPackLength = GetContextLength(m_vecGPSLine);
			strTel = GetContextTel(m_vecGPSLine);

			if(0 != nPackLength)
			{
				if(nPackLength <= m_vecGPSLine[10].length())
				{
					memcpy(outBuffer, m_vecGPSLine[10].c_str(), m_vecGPSLine[10].length());
					return 0;
				}
				else
				{
					strGPSContext = m_vecGPSLine[10].c_str();
					return 1;
				}
			}
		}
	}
	return -1;
}

template<typename T>
bool CDataPackParse::ParseDataPackShell(const char* pDataBuffer, T& dataUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		dataUpLoadPack.nBeginIdentify   = *((unsigned int*)pDataBuffer);
		dataUpLoadPack.nMsgNumber       = *((unsigned char*)(pDataBuffer + 4));
		dataUpLoadPack.nBodyLength      = *((unsigned short*)(pDataBuffer + 5));

		if(PACK_IDENTIFY == dataUpLoadPack.nBeginIdentify)
			return true;
	}

	return false;
}

template<typename T>
bool CDataPackParse::BuildDataPackShell(char* pDataBuffer, const T& dataDownLoadPack, unsigned char cMsgNumber)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)pDataBuffer) = PACK_IDENTIFY;
		*((unsigned char*)(pDataBuffer + 4)) = cMsgNumber;
		*((unsigned short*)(pDataBuffer + 5)) = dataDownLoadPack.nBodyLength;
		*((unsigned int*)(pDataBuffer + dataDownLoadPack.nBodyLength + 7)) = PACK_IDENTIFY;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseGPSPackData(const char* pDataBuffer, GPS_UpLoad_Pack& gpsUpLoadPack)
{
#define TERM_IDENT_LENGTH							128
#define TIME_BUFFER_LENGTH							32
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		double nSite;
		unsigned int noffset;
		char  strTerminalNumber[TERM_IDENT_LENGTH];
		char  strTime[TIME_BUFFER_LENGTH]; 

		noffset = *((unsigned char*)pDataBuffer);
		memset(strTerminalNumber, 0x00, TERM_IDENT_LENGTH);
		memset(strTime, 0x00, TIME_BUFFER_LENGTH);

		gpsUpLoadPack.gDataBodyPack.nTerminalIdentifyLength = noffset;
		memcpy(strTerminalNumber, pDataBuffer + 1, noffset);
		gpsUpLoadPack.gDataBodyPack.strTerminalNumber = strTerminalNumber;
		nSite = *((unsigned int*)(pDataBuffer + 1 + noffset));
		gpsUpLoadPack.gDataBodyPack.nLongitude = nSite / 10000000;
		nSite = *((unsigned int*)(pDataBuffer + 5 + noffset));
		gpsUpLoadPack.gDataBodyPack.nLatitude = nSite / 10000000;
		gpsUpLoadPack.gDataBodyPack.nSpeed = *((unsigned short*)(pDataBuffer + 9 + noffset));
		gpsUpLoadPack.gDataBodyPack.nDirection = *((unsigned char*)(pDataBuffer + 11 + noffset));

		sprintf_s(strTime, TIME_BUFFER_LENGTH, "%.04d-%.02d-%.02d %.02d:%.02d:%.02d",
			(*(unsigned short*)(pDataBuffer + 12 + noffset)),
			(*(unsigned char*)(pDataBuffer + 14 + noffset)),
			(*(unsigned char*)(pDataBuffer + 15 + noffset)),
			(*(unsigned char*)(pDataBuffer + 16 + noffset)),
			(*(unsigned char*)(pDataBuffer + 17 + noffset)),
			(*(unsigned char*)(pDataBuffer + 18 + noffset)));

		gpsUpLoadPack.gDataBodyPack.strTime = strTime;
		gpsUpLoadPack.gDataBodyPack.nState = *((unsigned char*)(pDataBuffer + 19 + noffset));
		gpsUpLoadPack.gDataBodyPack.nMileage = *((unsigned int*)(pDataBuffer + 20 + noffset));
		gpsUpLoadPack.gDataBodyPack.nOilMass = *((unsigned int*)(pDataBuffer + 24 + noffset));
		gpsUpLoadPack.gDataBodyPack.nIsLocate = *((unsigned char*)(pDataBuffer + 28 + noffset));

		return true;
	}

	return false;
}

bool CDataPackParse::BuildGPSPackData(char* pDataBuffer, const GPS_DownLoad_Pack& gpsDownLoadPack)
{
#define TERM_IDENT_LENGTH							128
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned char*)(pDataBuffer)) = gpsDownLoadPack.gDataBodyPack.nTerminalIdentifyLength;
		memcpy(pDataBuffer + 1, gpsDownLoadPack.gDataBodyPack.strTerminalNumber.c_str(), gpsDownLoadPack.gDataBodyPack.nTerminalIdentifyLength);
		memcpy(pDataBuffer + 1 + gpsDownLoadPack.gDataBodyPack.nTerminalIdentifyLength, gpsDownLoadPack.gDataBodyPack.strTime.c_str(), 7);
		*((unsigned int*)(pDataBuffer + 7 + gpsDownLoadPack.nBodyLength)) = PACK_IDENTIFY;

		return true;
	}
	return false;
}


bool CDataPackParse::ParseLoginPackData(const char* pDataBuffer, Login_UpLoad_Pack& loginUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer2, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer, 30);
		memcpy(m_tempBuffer2, pDataBuffer + 30, 12);

		loginUpLoadPack.gDataBodyPack.strName = m_tempBuffer1;
		loginUpLoadPack.gDataBodyPack.strPassword = m_tempBuffer2;

		return true;
	}
	return false;
}

bool CDataPackParse::BuildLoginPackData(char* pDataBuffer, const Login_DownLoad_Pack& loginDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = loginDownLoadPack.gDataBodyPack.nResult;
		*((unsigned int*)(pDataBuffer + 4)) = loginDownLoadPack.gDataBodyPack.nPower1;
		*((unsigned int*)(pDataBuffer + 8)) = loginDownLoadPack.gDataBodyPack.nPower2;
		*((unsigned int*)(pDataBuffer + 12)) = loginDownLoadPack.gDataBodyPack.nPower3;
		*((unsigned int*)(pDataBuffer + 16)) = loginDownLoadPack.gDataBodyPack.nOrgID;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseAddOrgPackData(const char* pDataBuffer, AddOrg_UpLoad_Pack& addOrgUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer2, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer, 30);
		memcpy(m_tempBuffer2, pDataBuffer + 30, 20);

		addOrgUpLoadPack.gDataBodyPack.strOrgName = m_tempBuffer1;
		addOrgUpLoadPack.gDataBodyPack.strAddress = m_tempBuffer2;

		addOrgUpLoadPack.gDataBodyPack.nUpOrg = *((unsigned int*)(pDataBuffer + 50)); 
		addOrgUpLoadPack.gDataBodyPack.nLevel = *((unsigned int*)(pDataBuffer + 54));
		addOrgUpLoadPack.gDataBodyPack.nLineId = *((unsigned int*)(pDataBuffer + 58));
		addOrgUpLoadPack.gDataBodyPack.nStartid= *((unsigned int*)(pDataBuffer + 62));
		addOrgUpLoadPack.gDataBodyPack.nEndID  = *((unsigned int*)(pDataBuffer + 66));

		return true;

	}
	return false;
}

bool CDataPackParse::BuildAddOrgPackData(char* pDataBuffer, const AddOrg_DownLoad_Pack& addOrgDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = addOrgDownLoadPack.gDataBodyPack.nResult;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseModifyOrgPackData(const char* pDataBuffer, ModifyOrg_UpLoad_Pack& modifyOrgUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer2, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer + 4, 30);
		memcpy(m_tempBuffer2, pDataBuffer + 34, 20);

		modifyOrgUpLoadPack.gDataBodyPack.strOrgName = m_tempBuffer1;
		modifyOrgUpLoadPack.gDataBodyPack.strAddress = m_tempBuffer2;

		modifyOrgUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer));
		modifyOrgUpLoadPack.gDataBodyPack.nUpOrgID = *((unsigned int*)(pDataBuffer + 54));
		modifyOrgUpLoadPack.gDataBodyPack.nLevel = *((unsigned int*)(pDataBuffer + 58));
		modifyOrgUpLoadPack.gDataBodyPack.nLineId = *((unsigned int*)(pDataBuffer + 62));
		modifyOrgUpLoadPack.gDataBodyPack.nStartid= *((unsigned int*)(pDataBuffer + 66));
		modifyOrgUpLoadPack.gDataBodyPack.nEndID  = *((unsigned int*)(pDataBuffer + 70));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildModifyOrgPackData(char* pDataBuffer, const ModifyOrg_DownLoad_Pack& modifyOrgDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = modifyOrgDownLoadPack.gDataBodyPack.nResult;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseDeleteOrgPackData(const char* pDataBuffer, DeleteOrg_UpLoad_Pack& deleteOrgUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		deleteOrgUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer));

		return true;
	}
	return false;
}
bool CDataPackParse::BuildDeleteOrgPackData(char* pDataBuffer, const DeleteOrg_DownLoad_Pack& deleteOrgDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = deleteOrgDownLoadPack.gDataBodyPack.nResult;
		*((unsigned int*)(pDataBuffer + 4)) = deleteOrgDownLoadPack.gDataBodyPack.nReson;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseAskOrgListPackData(const char* pDataBuffer, AskOrgList_UpLoad_Pack& askOrgListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		askOrgListUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildAskOrgListPackData(char* pDataBuffer, const AskOrgList_DownLoad_Pack& askOrgListDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = askOrgListDownLoadPack.gDataBodyPack.nTitlePack;
		*((unsigned int*)(pDataBuffer + 4)) = askOrgListDownLoadPack.gDataBodyPack.nHadSendPack;
		*((unsigned int*)(pDataBuffer + 8)) = askOrgListDownLoadPack.gDataBodyPack.nOrgID;

		memcpy(pDataBuffer + 12, askOrgListDownLoadPack.gDataBodyPack.strOrgName.c_str(), 
			askOrgListDownLoadPack.gDataBodyPack.strOrgName.length());
		memcpy(pDataBuffer + 42, askOrgListDownLoadPack.gDataBodyPack.strAddress.c_str(),
			askOrgListDownLoadPack.gDataBodyPack.strAddress.length());

		*((unsigned int*)(pDataBuffer + 62)) = askOrgListDownLoadPack.gDataBodyPack.nUpOrgID;
		*((unsigned int*)(pDataBuffer + 66)) = askOrgListDownLoadPack.gDataBodyPack.nLevel;
		*((unsigned int*)(pDataBuffer + 70)) = askOrgListDownLoadPack.gDataBodyPack.nLineId;
		*((unsigned int*)(pDataBuffer + 74)) = askOrgListDownLoadPack.gDataBodyPack.nStartid;
		*((unsigned int*)(pDataBuffer + 78)) = askOrgListDownLoadPack.gDataBodyPack.nEndID;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseAddWorkerPackData(const char* pDataBuffer, AddWorker_UpLoad_Pack& addWorkerUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer2, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer3, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer4, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer, 20);
		memcpy(m_tempBuffer2, pDataBuffer + 20, 32);
		memcpy(m_tempBuffer3, pDataBuffer + 52, 20);
		memcpy(m_tempBuffer4, pDataBuffer + 72, 20);

		addWorkerUpLoadPack.gDataBodyPack.strOper = m_tempBuffer1;
		addWorkerUpLoadPack.gDataBodyPack.strPassword = m_tempBuffer2;
		addWorkerUpLoadPack.gDataBodyPack.strName = m_tempBuffer3;
		addWorkerUpLoadPack.gDataBodyPack.strTel = m_tempBuffer4;
		addWorkerUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer + 92));
		addWorkerUpLoadPack.gDataBodyPack.nPowerGroup = *((unsigned int*)(pDataBuffer + 96));
		addWorkerUpLoadPack.gDataBodyPack.nPda1 = *((unsigned int*)(pDataBuffer + 100));
		addWorkerUpLoadPack.gDataBodyPack.nPda2 = *((unsigned int*)(pDataBuffer + 104));
		addWorkerUpLoadPack.gDataBodyPack.nWorkerState = *((unsigned int*)(pDataBuffer + 108));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildAddWorkertPackData(char* pDataBuffer, const AddWorker_DownLoad_Pack& addWorkerDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = addWorkerDownLoadPack.gDataBodyPack.nResult;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseModifyWorkerPackData(const char* pDataBuffer, ModifyWorker_UpLoad_Pack& modifyWorkerUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer2, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer3, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer, 20);
		memcpy(m_tempBuffer2, pDataBuffer + 20, 20);
		memcpy(m_tempBuffer3, pDataBuffer + 40, 20);

		modifyWorkerUpLoadPack.gDataBodyPack.strOper = m_tempBuffer1;
		modifyWorkerUpLoadPack.gDataBodyPack.strName = m_tempBuffer2;
		modifyWorkerUpLoadPack.gDataBodyPack.strTel = m_tempBuffer3;

		modifyWorkerUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer + 60));
		modifyWorkerUpLoadPack.gDataBodyPack.nPowerGroup = *((unsigned int*)(pDataBuffer + 64));
		modifyWorkerUpLoadPack.gDataBodyPack.nPda1 = *((unsigned int*)(pDataBuffer + 68));
		modifyWorkerUpLoadPack.gDataBodyPack.nPda2 = *((unsigned int*)(pDataBuffer + 72));
		modifyWorkerUpLoadPack.gDataBodyPack.nWorkerState = *((unsigned int*)(pDataBuffer + 76));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildModifyWorkertPackData(char* pDataBuffer, const ModifyWorker_DownLoad_Pack& modifyWorkerDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = modifyWorkerDownLoadPack.gDataBodyPack.nResult;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseDeleteWorkerPackData(const char* pDataBuffer, DeleteWorker_UpLoad_Pack& deleteWorkerUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer, 20);

		deleteWorkerUpLoadPack.gDataBodyPack.strOper = m_tempBuffer1;

		return true;
	}
	return false;
}

bool CDataPackParse::BuildDeleteWorkerPackData(char* pDataBuffer, const DeleteWorker_DownLoad_Pack& deleteWorkerDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = deleteWorkerDownLoadPack.gDataBodyPack.nResult;

		return true;
	}
	return false;
}
bool CDataPackParse::ParseAskWorkerListPackData(const char* pDataBuffer, AskWorkerList_UpLoad_Pack& askWorkerListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		askWorkerListUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer));
		askWorkerListUpLoadPack.gDataBodyPack.nType = *((unsigned int*)(pDataBuffer + 4));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildAskWorkerListPackData(char* pDataBuffer, const AskWorkerList_DownLoad_Pack& askWorkerListDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = askWorkerListDownLoadPack.gDataBodyPack.nTitlePack;
		*((unsigned int*)(pDataBuffer + 4)) = askWorkerListDownLoadPack.gDataBodyPack.nHadSendPack;

		memcpy(pDataBuffer + 8, askWorkerListDownLoadPack.gDataBodyPack.strOper.c_str(),
			askWorkerListDownLoadPack.gDataBodyPack.strOper.length());
		memcpy(pDataBuffer + 28, askWorkerListDownLoadPack.gDataBodyPack.strName.c_str(),
			askWorkerListDownLoadPack.gDataBodyPack.strName.length());
		memcpy(pDataBuffer + 48, askWorkerListDownLoadPack.gDataBodyPack.strTel.c_str(),
			askWorkerListDownLoadPack.gDataBodyPack.strTel.length());

		*((unsigned int*)(pDataBuffer + 68)) = askWorkerListDownLoadPack.gDataBodyPack.nOrgID;
		*((unsigned int*)(pDataBuffer + 72)) = askWorkerListDownLoadPack.gDataBodyPack.nPowerGroup;
		*((unsigned int*)(pDataBuffer + 76)) = askWorkerListDownLoadPack.gDataBodyPack.nPda1;
		*((unsigned int*)(pDataBuffer + 80)) = askWorkerListDownLoadPack.gDataBodyPack.nPda2;
		*((unsigned int*)(pDataBuffer + 84)) = askWorkerListDownLoadPack.gDataBodyPack.nWorkerState;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseAddPowerGroupPackData(const char* pDataBuffer, AddPowerGroup_UpLoad_Pack& addPowerGroupUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer, 20);

		addPowerGroupUpLoadPack.gDataBodyPack.strPowerName = m_tempBuffer1;
		addPowerGroupUpLoadPack.gDataBodyPack.iPow1 = *((unsigned int*)(pDataBuffer + 20));
		addPowerGroupUpLoadPack.gDataBodyPack.iPow2 = *((unsigned int*)(pDataBuffer + 24));
		addPowerGroupUpLoadPack.gDataBodyPack.iPow3 = *((unsigned int*)(pDataBuffer + 28));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildAddPowerGroupPackData(char* pDataBuffer, const AddPowerGroup_DownLoad_Pack& addPowerGroupDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = addPowerGroupDownLoadPack.gDataBodyPack.nResult;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseModifyPowerGroupPackData(const char* pDataBuffer, ModifyPowerGroup_UpLoad_Pack& modifyPowerGroupUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer + 4, 20);

		modifyPowerGroupUpLoadPack.gDataBodyPack.nPowerID = *((unsigned int*)(pDataBuffer));
		modifyPowerGroupUpLoadPack.gDataBodyPack.strPowerName = m_tempBuffer1;
		modifyPowerGroupUpLoadPack.gDataBodyPack.nPow1 = *((unsigned int*)(pDataBuffer + 24));
		modifyPowerGroupUpLoadPack.gDataBodyPack.nPow2 = *((unsigned int*)(pDataBuffer + 28));
		modifyPowerGroupUpLoadPack.gDataBodyPack.nPow3 = *((unsigned int*)(pDataBuffer + 32));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildModifyPowerGroupPackData(char* pDataBuffer, const ModifyPowerGroup_DownLoad_Pack& modifyPowerGroupDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = modifyPowerGroupDownLoadPack.gDataBodyPack.nResult;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseDeletePowerGroupPackData(const char* pDataBuffer, DeletePowerGroup_UpLoad_Pack& deletePowerGroupUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		deletePowerGroupUpLoadPack.gDataBodyPack.nPowerID = *((unsigned int*)(pDataBuffer));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildDeletePowerGroupPackData(char* pDataBuffer, const DeletePowerGroup_DownLoad_Pack& deletePowerGroupDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = deletePowerGroupDownLoadPack.gDataBodyPack.nResult;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseAskPowerGroupListPackData(const char* pDataBuffer, AskPowerGroupList_UpLoad_Pack& askPowerGroupListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		askPowerGroupListUpLoadPack.gDataBodyPack.nPowerID = *((unsigned int*)(pDataBuffer));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildAskPowerGroupListPackData(char* pDataBuffer, const AskPowerGroupList_DownLoad_Pack& askPowerGroupListDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = askPowerGroupListDownLoadPack.gDataBodyPack.nTitlePack;
		*((unsigned int*)(pDataBuffer + 4)) = askPowerGroupListDownLoadPack.gDataBodyPack.nHadSendPack;
		*((unsigned int*)(pDataBuffer + 8)) = askPowerGroupListDownLoadPack.gDataBodyPack.nPowerID;

		memcpy(pDataBuffer + 12, askPowerGroupListDownLoadPack.gDataBodyPack.strPowerName.c_str(),
		askPowerGroupListDownLoadPack.gDataBodyPack.strPowerName.length());
		
		*((unsigned int*)(pDataBuffer + 32)) = askPowerGroupListDownLoadPack.gDataBodyPack.nPow1;
		*((unsigned int*)(pDataBuffer + 36)) = askPowerGroupListDownLoadPack.gDataBodyPack.nPow2;
		*((unsigned int*)(pDataBuffer + 40)) = askPowerGroupListDownLoadPack.gDataBodyPack.nPow3;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseAddDevicePackData(const char* pDataBuffer, AddDevice_UpLoad_Pack& addDeviceUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer, 20);

		addDeviceUpLoadPack.gDataBodyPack.strDevicePID = m_tempBuffer1;
		addDeviceUpLoadPack.gDataBodyPack.nDeviceOrg = *((unsigned int*)(pDataBuffer + 20));
		addDeviceUpLoadPack.gDataBodyPack.nDeviceType = *((unsigned int*)(pDataBuffer + 24));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildAddDevicePackData(char* pDataBuffer, const AddDevice_DownLoad_Pack& addDeviceDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = addDeviceDownLoadPack.gDataBodyPack.nResult;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseModifyDevicePackData(const char* pDataBuffer, ModifyDevice_UpLoad_Pack& modifyDeviceUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer + 4, 20);

		modifyDeviceUpLoadPack.gDataBodyPack.nDeviceID = *((unsigned int*)(pDataBuffer));
		modifyDeviceUpLoadPack.gDataBodyPack.strDevicePID = m_tempBuffer1;
		modifyDeviceUpLoadPack.gDataBodyPack.nDeviceOrg = *((unsigned int*)(pDataBuffer + 24));
		modifyDeviceUpLoadPack.gDataBodyPack.nDeviceType = *((unsigned int*)(pDataBuffer + 28));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildModifyDevicePackData(char* pDataBuffer, const ModifyDevice_DownLoad_Pack& modifyDeviceDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = modifyDeviceDownLoadPack.gDataBodyPack.nResult;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseDeleteDevicePackData(const char* pDataBuffer, DeleteDevice_UpLoad_Pack& deleteDeviceUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		deleteDeviceUpLoadPack.gDataBodyPack.nDeviceID = *((unsigned int*)(pDataBuffer));
		return true;
	}
	return false;
}

bool CDataPackParse::BuildDeleteDevicePackData(char* pDataBuffer, const DeleteDevice_DownLoad_Pack& deleteDeviceDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = deleteDeviceDownLoadPack.gDataBodyPack.nResult;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseAskDeviceListPackData(const char* pDataBuffer, AskDeviceList_Upload_Pack& askDeviceListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		askDeviceListUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer));
		askDeviceListUpLoadPack.gDataBodyPack.nType = *((unsigned int*)(pDataBuffer + 4));
		askDeviceListUpLoadPack.gDataBodyPack.nDeivceType = *((unsigned int*)(pDataBuffer + 8));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildAskDeviceListPackData(char* pDataBuffer, const AskDeviceList_Download_Pack& askDeviceListeDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = askDeviceListeDownLoadPack.gDataBodyPack.nTitlePack;
		*((unsigned int*)(pDataBuffer + 4)) = askDeviceListeDownLoadPack.gDataBodyPack.nHadSendPack;
		*((unsigned int*)(pDataBuffer + 8)) = askDeviceListeDownLoadPack.gDataBodyPack.nDeviceID;
		
		memcpy(pDataBuffer + 12, askDeviceListeDownLoadPack.gDataBodyPack.strDevicePID.c_str(),
			askDeviceListeDownLoadPack.gDataBodyPack.strDevicePID.length());

		*((unsigned int*)(pDataBuffer + 32)) = askDeviceListeDownLoadPack.gDataBodyPack.nDeviceOrg;
		*((unsigned int*)(pDataBuffer + 36)) = askDeviceListeDownLoadPack.gDataBodyPack.nDeviceType;

		return true;
	}
	return false;
}


bool CDataPackParse::ParseGetGPSPointPackData(const char* pDataBuffer, GetGPSPoint_Upload_Pack& getGPSPointUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer2, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer3, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer, 20);
		memcpy(m_tempBuffer2, pDataBuffer + 20, 20);
		memcpy(m_tempBuffer3, pDataBuffer + 40, 20);

		getGPSPointUpLoadPack.gDataBodyPack.strTel = m_tempBuffer1;
		getGPSPointUpLoadPack.gDataBodyPack.strTime = m_tempBuffer2;
		getGPSPointUpLoadPack.gDataBodyPack.strEndTime = m_tempBuffer3;
		getGPSPointUpLoadPack.gDataBodyPack.nType = *((unsigned int*)(pDataBuffer + 60));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildGetGPSPointPackData(char* pDataBuffer, const GetGPSPoint_Download_Pack& getGPSPointDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = getGPSPointDownLoadPack.gDataBodyPack.nTotlePack;
		*((unsigned int*)(pDataBuffer + 4)) = getGPSPointDownLoadPack.gDataBodyPack.nHadSendPack;

		int nIndex;
		int nBegin;		
		std::vector<SGPS>::const_iterator iterSGPS;

		for(nIndex = 0, iterSGPS = getGPSPointDownLoadPack.gDataBodyPack.vecGPS.begin();
			iterSGPS != getGPSPointDownLoadPack.gDataBodyPack.vecGPS.end() && nIndex < 10; ++iterSGPS, ++nIndex)
		{
			nBegin = 8 + nIndex * 90;

			memcpy(pDataBuffer + nBegin, iterSGPS->strTime.c_str(), iterSGPS->strTime.length());
			memcpy(pDataBuffer + nBegin + 30, iterSGPS->strJDU.c_str(), iterSGPS->strJDU.length());
			memcpy(pDataBuffer + nBegin + 50, iterSGPS->strWDU.c_str(), iterSGPS->strWDU.length());
			memcpy(pDataBuffer + nBegin + 70, iterSGPS->strTel.c_str(), iterSGPS->strTel.length());
		}

		return true;
	}
	return false;
}


bool CDataPackParse::ParseGetRailWayNamePackData(const char* pDataBuffer, GetRailWayName_Upload_Pack& getRailWayNameUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		getRailWayNameUpLoadPack.gDataBodyPack.nLineID = *((unsigned int*)(pDataBuffer));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildGetRailWayNamePackData(char* pDataBuffer, const GetRailWayName_Download_Pack& getRailWayNameDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = getRailWayNameDownLoadPack.gDataBodyPack.nTotlePack;
		*((unsigned int*)(pDataBuffer + 4)) = getRailWayNameDownLoadPack.gDataBodyPack.nHadSendPack;
		memcpy(pDataBuffer + 8, getRailWayNameDownLoadPack.gDataBodyPack.strnName.c_str(),
			getRailWayNameDownLoadPack.gDataBodyPack.strnName.length());

		return true;
	}
	return false;
}


bool CDataPackParse::ParseSettingLinePackData(const char* pDataBuffer, SettingLine_Upload_Pack& settingLineUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);
		memcpy(m_tempBuffer1, pDataBuffer + 8, 30);

		settingLineUpLoadPack.gDataBodyPack.nType = *((unsigned int*)(pDataBuffer));
		settingLineUpLoadPack.gDataBodyPack.nID = *((unsigned int*)(pDataBuffer + 4));
		settingLineUpLoadPack.gDataBodyPack.strName = m_tempBuffer1;
		settingLineUpLoadPack.gDataBodyPack.nDay = *((unsigned int*)(pDataBuffer + 38));
		settingLineUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer + 42));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildSettingLinePackData(char* pDataBuffer, const SettingLine_Download_Pack& settingLineDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = settingLineDownLoadPack.gDataBodyPack.nResult;
		return true;
	}
	return false;
}


bool CDataPackParse::ParseSettingPollingTimePackData(const char* pDataBuffer, SettingPollingTime_Upload_Pack& settingPollingTimeUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);
		memcpy(m_tempBuffer1, pDataBuffer + 16, 20);

		settingPollingTimeUpLoadPack.gDataBodyPack.nType = *((unsigned int*)(pDataBuffer));
		settingPollingTimeUpLoadPack.gDataBodyPack.nID = *((unsigned int*)(pDataBuffer + 4));
		settingPollingTimeUpLoadPack.gDataBodyPack.nLineID = *((unsigned int*)(pDataBuffer + 8));
		settingPollingTimeUpLoadPack.gDataBodyPack.nPointID = *((unsigned int*)(pDataBuffer + 12));
		settingPollingTimeUpLoadPack.gDataBodyPack.strTime = m_tempBuffer1;

		return true;
	}
	return false;
}

bool CDataPackParse::BuildSettingPollingTimePackData(char* pDataBuffer, const SettingPollingTime_Download_Pack& settingPollingTimeDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = settingPollingTimeDownLoadPack.gDataBodyPack.nResult;

		return true;
	}
	return false;
}


bool CDataPackParse::ParseSettingSchedule5PackData(const char* pDataBuffer, SettingSchedule5_Upload_Pack& settingSchedule5UpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer2, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer + 16, 20);
		memcpy(m_tempBuffer2, pDataBuffer + 36, 60);

		settingSchedule5UpLoadPack.gDataBodyPack.nType = *((unsigned int*)(pDataBuffer));
		settingSchedule5UpLoadPack.gDataBodyPack.nID = *((unsigned int*)(pDataBuffer + 4));
		settingSchedule5UpLoadPack.gDataBodyPack.nCYC = *((unsigned int*)(pDataBuffer + 8));
		settingSchedule5UpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer + 12));
		settingSchedule5UpLoadPack.gDataBodyPack.strStart = m_tempBuffer1;
		settingSchedule5UpLoadPack.gDataBodyPack.strContent = m_tempBuffer2;

		return true;
	}
	return false;
}

bool CDataPackParse::BuildSettingSchedule5PackData(char* pDataBuffer, const SettingSchedule5_Download_Pack& settingSchedule5DownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = settingSchedule5DownLoadPack.gDataBodyPack.nResult;
		return true;
	}
	return false;
}


bool CDataPackParse::ParseSettingSchedule6PackData(const char* pDataBuffer, SettingSchedule6_Upload_Pack& settingSchedule6UpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		settingSchedule6UpLoadPack.gDataBodyPack.nType = *((unsigned int*)(pDataBuffer));
		settingSchedule6UpLoadPack.gDataBodyPack.nPL_ID = *((unsigned int*)(pDataBuffer + 4));
		settingSchedule6UpLoadPack.gDataBodyPack.nPL_PBID = *((unsigned int*)(pDataBuffer + 8));
		settingSchedule6UpLoadPack.gDataBodyPack.nPL_LineID = *((unsigned int*)(pDataBuffer + 12));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildSettingSchedule6PackData(char* pDataBuffer, const SettingSchedule6_Download_Pack& settingSchedule6DownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = settingSchedule6DownLoadPack.gDataBodyPack.nResult;
		return true;
	}
	return false;
}


bool CDataPackParse::ParseSettingScheduleWorkerPackData(const char* pDataBuffer, SettingScheduleWorker_Upload_Pack& settingScheduleWorkerUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);
		memcpy(m_tempBuffer1, pDataBuffer + 8, 20);

		settingScheduleWorkerUpLoadPack.gDataBodyPack.nType = *((unsigned int*)(pDataBuffer));
		settingScheduleWorkerUpLoadPack.gDataBodyPack.nPBID = *((unsigned int*)(pDataBuffer + 4));
		settingScheduleWorkerUpLoadPack.gDataBodyPack.strRYID = m_tempBuffer1;
		settingScheduleWorkerUpLoadPack.gDataBodyPack.nPX = *((unsigned int*)(pDataBuffer + 28));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildSettingScheduleWorkerPackData(char* pDataBuffer, const SettingScheduleWorker_Download_Pack& settingScheduleWorkerDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = settingScheduleWorkerDownLoadPack.gDataBodyPack.nResult;
		return true;
	}
	return false;
}


bool CDataPackParse::ParseUrgencyReleasePackData(const char* pDataBuffer, UrgencyRelease_Upload_Pack& urgencyReleaseUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer2, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer3, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer + 12, 25);
		memcpy(m_tempBuffer2, pDataBuffer + 37, 25);
		memcpy(m_tempBuffer3, pDataBuffer + 62, 40);

		urgencyReleaseUpLoadPack.gDataBodyPack.nType = *((unsigned int*)(pDataBuffer));
		urgencyReleaseUpLoadPack.gDataBodyPack.nStartPointID = *((unsigned int*)(pDataBuffer + 4));
		urgencyReleaseUpLoadPack.gDataBodyPack.nEndPointID = *((unsigned int*)(pDataBuffer + 8));
		urgencyReleaseUpLoadPack.gDataBodyPack.strStartTime = m_tempBuffer1;
		urgencyReleaseUpLoadPack.gDataBodyPack.strEndTime = m_tempBuffer2;
		urgencyReleaseUpLoadPack.gDataBodyPack.strRWName = m_tempBuffer3;
		urgencyReleaseUpLoadPack.gDataBodyPack.nStartPointID = *((unsigned int*)(pDataBuffer + 102));
		urgencyReleaseUpLoadPack.gDataBodyPack.nLineID = *((unsigned int*)(pDataBuffer + 106));
		urgencyReleaseUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer + 110));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildUrgencyReleasePackData(char* pDataBuffer, const UrgencyRelease_Download_Pack& urgencyReleaseDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = urgencyReleaseDownLoadPack.gDataBodyPack.nResult;
		return true;
	}
	return false;
}

bool CDataPackParse::ParseUrgencyWorkerPackData(const char* pDataBuffer, UrgencyWorker_Upload_Pack& urgencyWorkerUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		urgencyWorkerUpLoadPack.gDataBodyPack.nType = *((unsigned int*)(pDataBuffer));
		urgencyWorkerUpLoadPack.gDataBodyPack.nJJ_ID = *((unsigned int*)(pDataBuffer + 4));
		urgencyWorkerUpLoadPack.gDataBodyPack.nJJ_PID = *((unsigned int*)(pDataBuffer + 8));
		urgencyWorkerUpLoadPack.gDataBodyPack.nJJ_PDevice = *((unsigned int*)(pDataBuffer + 12));


		return true;
	}
	return false;
}

bool CDataPackParse::BuildUrgencyWorkerPackData(char* pDataBuffer, const UrgencyWorker_Download_Pack& urgencyWorkerDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = urgencyWorkerDownLoadPack.gDataBodyPack.nResult;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseGetLinePointPackData(const char* pDataBuffer, GetLinePoint_Upload_Pack& getLinePointUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		getLinePointUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer));

		return true;
	}
	return false;
}
bool CDataPackParse::BuildGetLinePointPackData(char* pDataBuffer, const GetLinePoint_Download_Pack& getLinePointDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = getLinePointDownLoadPack.gDataBodyPack.nTotalPacket;
		*((unsigned int*)(pDataBuffer + 4)) = getLinePointDownLoadPack.gDataBodyPack.nCurrentPacket;
		*((unsigned int*)(pDataBuffer + 8)) = getLinePointDownLoadPack.gDataBodyPack.nPointID;
		*((float*)(pDataBuffer + 12)) = getLinePointDownLoadPack.gDataBodyPack.fJDU;
		*((float*)(pDataBuffer + 16)) = getLinePointDownLoadPack.gDataBodyPack.fWDU;
		*((float*)(pDataBuffer + 20)) = getLinePointDownLoadPack.gDataBodyPack.fGls;
		*((unsigned int*)(pDataBuffer + 24)) = getLinePointDownLoadPack.gDataBodyPack.nState;
		*((unsigned int*)(pDataBuffer + 28)) = getLinePointDownLoadPack.gDataBodyPack.nLineID;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseGetPicListPackData(const char* pDataBuffer, GetPicList_Upload_Pack& getPicListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer2, 0x00, TEMP_BUFFER_SIZE);

		getPicListUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer));
		memcpy(m_tempBuffer1, pDataBuffer + 4, 20);
		memcpy(m_tempBuffer2, pDataBuffer + 24, 20);
		getPicListUpLoadPack.gDataBodyPack.strSTime = m_tempBuffer1;
		getPicListUpLoadPack.gDataBodyPack.strETime = m_tempBuffer2;

		return true;
	}
	return false;
}
bool CDataPackParse::BuildGetPicListPackData(char* pDataBuffer, const GetPicList_Download_Pack& getPicListDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = getPicListDownLoadPack.gDataBodyPack.nTotalPacket;
		*((unsigned int*)(pDataBuffer + 4)) = getPicListDownLoadPack.gDataBodyPack.nCurrentPacket;
		memcpy(pDataBuffer + 8, getPicListDownLoadPack.gDataBodyPack.strName.c_str(), getPicListDownLoadPack.gDataBodyPack.strName.length());
		memcpy(pDataBuffer + 38, getPicListDownLoadPack.gDataBodyPack.strTime.c_str(), getPicListDownLoadPack.gDataBodyPack.strTime.length());
		*((unsigned int*)(pDataBuffer + 58)) = getPicListDownLoadPack.gDataBodyPack.nType; 
		memcpy(pDataBuffer + 62, getPicListDownLoadPack.gDataBodyPack.strTel.c_str(), getPicListDownLoadPack.gDataBodyPack.strTel.length());

		return true;
	}
	return false;
}

bool CDataPackParse::ParseGetUrgencyMissionListPackData(const char* pDataBuffer, GetUrgencyMissionList_Upload_Pack& getUrgencyMissionListDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		getUrgencyMissionListDownLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer));

		return true;
	}
	return false;
}
bool CDataPackParse::BuildGetUrgencyMissionListPackData(char* pDataBuffer, const GetUrgencyMissionList_Download_Pack& getUrgencyMissionListDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = getUrgencyMissionListDownLoadPack.gDataBodyPack.nTtlePacket;
		*((unsigned int*)(pDataBuffer + 4)) = getUrgencyMissionListDownLoadPack.gDataBodyPack.nCurrentPacket;
		memcpy(pDataBuffer + 8, getUrgencyMissionListDownLoadPack.gDataBodyPack.strSTime.c_str(), getUrgencyMissionListDownLoadPack.gDataBodyPack.strSTime.length());
		memcpy(pDataBuffer + 38, getUrgencyMissionListDownLoadPack.gDataBodyPack.strETime.c_str(), getUrgencyMissionListDownLoadPack.gDataBodyPack.strETime.length());
		*((unsigned int*)(pDataBuffer + 58)) = getUrgencyMissionListDownLoadPack.gDataBodyPack.nState;
		*((unsigned int*)(pDataBuffer + 62)) = getUrgencyMissionListDownLoadPack.gDataBodyPack.nID;
		*((unsigned int*)(pDataBuffer + 66)) = getUrgencyMissionListDownLoadPack.gDataBodyPack.nLineID;
		*((unsigned int*)(pDataBuffer + 70)) = getUrgencyMissionListDownLoadPack.gDataBodyPack.nSPoint;
		*((unsigned int*)(pDataBuffer + 74)) = getUrgencyMissionListDownLoadPack.gDataBodyPack.nEPoint;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseGetUrgencyWorkerListPackData(const char* pDataBuffer, GetUrgencyWorkerList_Upload_Pack& getUrgencyWorkerListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		getUrgencyWorkerListUpLoadPack.gDataBodyPack.nJJID = *((unsigned int*)(pDataBuffer));

		return true;
	}
	return false;
}
bool CDataPackParse::BuildGetUrgencyWorkerListPackData(char* pDataBuffer, const GetUrgencyWorkerList_Download_Pack& getUrgencyWorkerListDownLooadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = getUrgencyWorkerListDownLooadPack.gDataBodyPack.nTtlePacket;
		*((unsigned int*)(pDataBuffer + 4)) = getUrgencyWorkerListDownLooadPack.gDataBodyPack.nCurrentPacket;
		memcpy(pDataBuffer + 8, getUrgencyWorkerListDownLooadPack.gDataBodyPack.strJj_Pid.c_str(), getUrgencyWorkerListDownLooadPack.gDataBodyPack.strJj_Pid.length());
		memcpy(pDataBuffer + 28, getUrgencyWorkerListDownLooadPack.gDataBodyPack.strJj_Name.c_str(), getUrgencyWorkerListDownLooadPack.gDataBodyPack.strJj_Name.length());
		*((unsigned int*)(pDataBuffer + 68)) = getUrgencyWorkerListDownLooadPack.gDataBodyPack.nJj_OrgID;
		memcpy(pDataBuffer + 72, getUrgencyWorkerListDownLooadPack.gDataBodyPack.strJj_Time.c_str(), getUrgencyWorkerListDownLooadPack.gDataBodyPack.strJj_Time.length());
		*((unsigned int*)(pDataBuffer + 97)) = getUrgencyWorkerListDownLooadPack.gDataBodyPack.nJj_long;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseGetPicDataPackData(const char* pDataBuffer, GetPicData_Upload_Pack& getPicDataUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer, 30);
		getPicDataUpLoadPack.gDataBodyPack.strPicName = m_tempBuffer1;

		return true;
	}
	return false;
}
bool CDataPackParse::BuildGetPicDataPackData(char* pDataBuffer, const GetPicData_Download_Pack& getPicDataDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = getPicDataDownLoadPack.gDataBodyPack.nTtlePacket;
		*((unsigned int*)(pDataBuffer + 4)) = getPicDataDownLoadPack.gDataBodyPack.nCurrentPacket;
		*((unsigned int*)(pDataBuffer + 8)) = getPicDataDownLoadPack.gDataBodyPack.nPagesize;

		memcpy(pDataBuffer + 12, getPicDataDownLoadPack.gDataBodyPack.picBuffer, 1024*3);

		return true;
	}
	return false;
}

bool CDataPackParse::ParseGetOrgSchedueListPackData(const char* pDataBuffer, GetOrgSchedueList_Upload_Pack& getOrgSchedueListUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		getOrgSchedueListUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer));
		getOrgSchedueListUpLoadPack.gDataBodyPack.nYears = *((unsigned int*)(pDataBuffer + 4));
		getOrgSchedueListUpLoadPack.gDataBodyPack.nMonths= *((unsigned int*)(pDataBuffer + 8));

		return true;
	}
	return false;
}
bool CDataPackParse::BuildGetOrgSchedueListPackData(char* pDataBuffer, const GetOrgSchedueList_Download_Pack& getOrgShcedueListDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = getOrgShcedueListDownLoadPack.gDataBodyPack.nTotlePacket;
		*((unsigned int*)(pDataBuffer + 4)) = getOrgShcedueListDownLoadPack.gDataBodyPack.nCurrentPacket;

		memcpy(pDataBuffer + 8, getOrgShcedueListDownLoadPack.gDataBodyPack.strUserID.c_str(), getOrgShcedueListDownLoadPack.gDataBodyPack.strUserID.length());
		memcpy(pDataBuffer + 28, getOrgShcedueListDownLoadPack.gDataBodyPack.strDates.c_str(), getOrgShcedueListDownLoadPack.gDataBodyPack.strDates.length());
		memcpy(pDataBuffer + 48, getOrgShcedueListDownLoadPack.gDataBodyPack.strXJ.c_str(), getOrgShcedueListDownLoadPack.gDataBodyPack.strXJ.length());

		*((unsigned int*)(pDataBuffer + 68)) = getOrgShcedueListDownLoadPack.gDataBodyPack.nSTotlePoint;
		*((unsigned int*)(pDataBuffer + 72)) = getOrgShcedueListDownLoadPack.gDataBodyPack.nSGetPoint;
		*((unsigned int*)(pDataBuffer + 76)) = getOrgShcedueListDownLoadPack.gDataBodyPack.nCPoint;
		*((unsigned int*)(pDataBuffer + 80)) = getOrgShcedueListDownLoadPack.gDataBodyPack.nLPoint;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseGetOrgSchedueInfoPackData(const char* pDataBuffer, GetOrgSchedueInfo_Upload_Pack& getOrgSchedueInfoUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		getOrgSchedueInfoUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer));

		return true;
	}
	return false;
}
bool CDataPackParse::BuildGetOrgSchedueInfoPackData(char* pDataBuffer, const GetOrgSchedueInfo_Download_Pack& getOrgSchedueInfoDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = getOrgSchedueInfoDownLoadPack.gDataBodyPack.nID;
		*((unsigned int*)(pDataBuffer + 4)) = getOrgSchedueInfoDownLoadPack.gDataBodyPack.nCYC;

		memcpy(pDataBuffer + 8, getOrgSchedueInfoDownLoadPack.gDataBodyPack.strTime.c_str(), getOrgSchedueInfoDownLoadPack.gDataBodyPack.strTime.length());
		memcpy(pDataBuffer + 28, getOrgSchedueInfoDownLoadPack.gDataBodyPack.strContent.c_str(), getOrgSchedueInfoDownLoadPack.gDataBodyPack.strContent.length());

		return true;
	}
	return false;
}

bool CDataPackParse::ParseSetPointPackData(const char* pDataBuffer, SetPoint_Upload_Pack& setPointUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		setPointUpLoadPack.gDataBodyPack.nIType = *((unsigned int*)(pDataBuffer));
		setPointUpLoadPack.gDataBodyPack.nPointId = *((unsigned int*)(pDataBuffer + 4));
		setPointUpLoadPack.gDataBodyPack.nIRailLine = *((unsigned int*)(pDataBuffer + 8));

		setPointUpLoadPack.gDataBodyPack.dIKM = *((unsigned int*)(pDataBuffer + 12));
		setPointUpLoadPack.gDataBodyPack.dILon = *((unsigned int*)(pDataBuffer + 20));
		setPointUpLoadPack.gDataBodyPack.dILat = *((unsigned int*)(pDataBuffer + 28));
		setPointUpLoadPack.gDataBodyPack.nIDirect = *((int*)(pDataBuffer + 32));

		return true;
	}
	return false;
}

bool CDataPackParse::BuildSetPointPackData(char* pDataBuffer, const SetPoint_Download_Pack& setPointDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = setPointDownLoadPack.gDataBodyPack.nResult;
		return true;
	}
	return false;
}

bool CDataPackParse::ParseGetOrgLinePackData(const char* pDataBuffer, GetOrgLine_Upload_Pack& getOrgLineUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		getOrgLineUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer));
		return true;
	}
	return false;
}

bool CDataPackParse::BuildGetOrgLinePackData(char* pDataBuffer, const GetOrgLine_Download_Pack& getOrgLineDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = getOrgLineDownLoadPack.gDataBodyPack.nTtotlePacket;
		*((unsigned int*)(pDataBuffer + 4)) = getOrgLineDownLoadPack.gDataBodyPack.nCurrentPacket;
		*((unsigned int*)(pDataBuffer + 8)) = getOrgLineDownLoadPack.gDataBodyPack.nLineID;
		*((unsigned int*)(pDataBuffer + 12)) = getOrgLineDownLoadPack.gDataBodyPack.nLineDay;
		memcpy(pDataBuffer + 16, getOrgLineDownLoadPack.gDataBodyPack.strLineName.c_str(), getOrgLineDownLoadPack.gDataBodyPack.strLineName.length());

		return true;
	}
	return false;
}

bool CDataPackParse::ParseGetLinePointTimePackData(const char* pDataBuffer, GetLinePointTime_Upload_Pack& getLinePointTimeUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		getLinePointTimeUpLoadPack.gDataBodyPack.nLineID = *((unsigned int*)(pDataBuffer));
		return true;
	}
	return false;
}

bool CDataPackParse::BuildGetLinePointTimePackData(char* pDataBuffer, const GetLinePointTime_Download_Pack& getLinePointTimeDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = getLinePointTimeDownLoadPack.gDataBodyPack.nTotlePacket;
		*((unsigned int*)(pDataBuffer + 4)) = getLinePointTimeDownLoadPack.gDataBodyPack.nCurrentPacket;
		*((float*)(pDataBuffer + 8)) = getLinePointTimeDownLoadPack.gDataBodyPack.fIKM;
		*((float*)(pDataBuffer + 12)) = getLinePointTimeDownLoadPack.gDataBodyPack.fILon;
		*((float*)(pDataBuffer + 16)) = getLinePointTimeDownLoadPack.gDataBodyPack.fILat;
		*((unsigned int*)(pDataBuffer + 20)) = getLinePointTimeDownLoadPack.gDataBodyPack.nIDirect;
		*((unsigned int*)(pDataBuffer + 24)) = getLinePointTimeDownLoadPack.gDataBodyPack.nPointID;
		memcpy(pDataBuffer + 28, getLinePointTimeDownLoadPack.gDataBodyPack.strTime.c_str(), getLinePointTimeDownLoadPack.gDataBodyPack.strTime.length());

		return true;
	}
	return false;
}

bool CDataPackParse::ParseGetOrgSchWorkerPackData(const char* pDataBuffer, GetOrgSchWorker_Upload_Pack& getOrgSchWorkerUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		getOrgSchWorkerUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer));
		return true;
	}
	return false;
}

bool CDataPackParse::BuildGetOrgSchWorkerPackData(char* pDataBuffer, const GetOrgSchWorker_Download_Pack& getOrgSchWorkerDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = getOrgSchWorkerDownLoadPack.gDataBodyPack.nTotle;
		*((unsigned int*)(pDataBuffer + 4)) = getOrgSchWorkerDownLoadPack.gDataBodyPack.nHasRecv;
		memcpy(pDataBuffer + 8, getOrgSchWorkerDownLoadPack.gDataBodyPack.strOper.c_str(), getOrgSchWorkerDownLoadPack.gDataBodyPack.strOper.length());
		memcpy(pDataBuffer + 28, getOrgSchWorkerDownLoadPack.gDataBodyPack.strName.c_str(), getOrgSchWorkerDownLoadPack.gDataBodyPack.strName.length());
		memcpy(pDataBuffer + 48, getOrgSchWorkerDownLoadPack.gDataBodyPack.strTel.c_str(), getOrgSchWorkerDownLoadPack.gDataBodyPack.strTel.length());
		*((unsigned int*)(pDataBuffer + 68)) = getOrgSchWorkerDownLoadPack.gDataBodyPack.nOrgID;
		*((unsigned int*)(pDataBuffer + 72)) = getOrgSchWorkerDownLoadPack.gDataBodyPack.nPowerGroup;
		*((unsigned int*)(pDataBuffer + 76)) = getOrgSchWorkerDownLoadPack.gDataBodyPack.nPda1;
		*((unsigned int*)(pDataBuffer + 80)) = getOrgSchWorkerDownLoadPack.gDataBodyPack.nPda2;
		*((unsigned int*)(pDataBuffer + 84)) = getOrgSchWorkerDownLoadPack.gDataBodyPack.nWorkerState;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseWorkerPollQueryPackData(const char* pDataBuffer, WorkerPollQuery_Upload_Pack& workerPollQueryUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer2, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer, 20);
		memcpy(m_tempBuffer2, pDataBuffer + 20, 20);

		workerPollQueryUpLoadPack.gDataBodyPack.strOper = m_tempBuffer1;
		workerPollQueryUpLoadPack.gDataBodyPack.strTime = m_tempBuffer2;

		return true;
	}
	return false;
}

bool CDataPackParse::BuildWorkerPollQueryPackData(char* pDataBuffer, const WorkerPollQuery_Download_Pack& workerPollQueryDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = workerPollQueryDownLoadPack.gDataBodyPack.nTotlePacket;
		*((unsigned int*)(pDataBuffer + 4)) = workerPollQueryDownLoadPack.gDataBodyPack.nCurrentPacket;
		*((float*)(pDataBuffer + 8)) = workerPollQueryDownLoadPack.gDataBodyPack.fJUD;
		*((float*)(pDataBuffer + 12)) = workerPollQueryDownLoadPack.gDataBodyPack.fWDU;
		memcpy(pDataBuffer + 16, workerPollQueryDownLoadPack.gDataBodyPack.strTime.c_str(), 
			workerPollQueryDownLoadPack.gDataBodyPack.strTime.length());

		return true;
	}
	return false;
}

bool CDataPackParse::ParseUrgencyMissionDeletePackData(const char* pDataBuffer, UrgencyMissionDelete_Upload_Pack& urgencyMissionDeleteUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		urgencyMissionDeleteUpLoadPack.gDataBodyPack.nOrgID = *((unsigned int*)(pDataBuffer));
		return true;
	}
	return false;
}

bool CDataPackParse::BuildUrgencyMissionDeletePackData(char* pDataBuffer, const UrgencyMissionDelete_Download_Pack& urgencyMissionDeleteDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = urgencyMissionDeleteDownLoadPack.gDataBodyPack.nResult;

		return true;
	}
	return false;
}

bool CDataPackParse::ParseWorkerPollPackData(const char* pDataBuffer, WorkerPoll_Upload_Pack& workerPollUpLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		memset(m_tempBuffer1, 0x00, TEMP_BUFFER_SIZE);
		memset(m_tempBuffer2, 0x00, TEMP_BUFFER_SIZE);

		memcpy(m_tempBuffer1, pDataBuffer, 20);
		memcpy(m_tempBuffer2, pDataBuffer + 20, 20);

		workerPollUpLoadPack.gDataBodyPack.strOper = m_tempBuffer1;
		workerPollUpLoadPack.gDataBodyPack.strDate = m_tempBuffer2;

		return true;
	}
	return false;
}

bool CDataPackParse::BuildWorkerPollPackData(char* pDataBuffer, const WorkerPoll_Download_Pack& workerPollDownLoadPack)
{
	ASSERT(pDataBuffer);
	if(pDataBuffer)
	{
		*((unsigned int*)(pDataBuffer)) = workerPollDownLoadPack.gDataBodyPack.nTotlePacket;
		*((unsigned int*)(pDataBuffer + 4)) = workerPollDownLoadPack.gDataBodyPack.nCurrentPacket;
		*((unsigned int*)(pDataBuffer + 8)) = workerPollDownLoadPack.gDataBodyPack.nPointID;
		memcpy(pDataBuffer + 12, workerPollDownLoadPack.gDataBodyPack.strDate.c_str(), workerPollDownLoadPack.gDataBodyPack.strDate.length());
		memcpy(pDataBuffer + 32, workerPollDownLoadPack.gDataBodyPack.strArrtime.c_str(), workerPollDownLoadPack.gDataBodyPack.strArrtime.length());
		memcpy(pDataBuffer + 52, workerPollDownLoadPack.gDataBodyPack.strRealTime.c_str(), workerPollDownLoadPack.gDataBodyPack.strRealTime.length());
		memcpy(pDataBuffer + 72, workerPollDownLoadPack.gDataBodyPack.strPID.c_str(), workerPollDownLoadPack.gDataBodyPack.strPID.length());
		*((unsigned int*)(pDataBuffer + 92)) = workerPollDownLoadPack.gDataBodyPack.nPointState;

		return true;
	}
	return false;
}

unsigned int CDataPackParse::GetContextLength(vector<string>& vecGPSLine)
{
	if(vecGPSLine.size() > 0)
	{
		for (vector<string>::iterator iterbegin = vecGPSLine.begin(); iterbegin != vecGPSLine.end(); ++iterbegin)
		{
			transform(iterbegin->begin(), iterbegin->end(), iterbegin->begin(), ::toupper);

			string strFind("CONTENT-LENGTH:");
			unsigned int nSite = iterbegin->find(strFind);
			if(nSite != string::npos)
			{
				string strLength = iterbegin->substr(strFind.length());

				return CCommonFunction::StringToInt(strLength);
			}
		}
	}

	return 0;
}

string	CDataPackParse::GetContextTel(vector<string>& vecGPSLine)
{
	string strTel;
	if(vecGPSLine.size() > 0)
	{
		for (vector<string>::iterator iterbegin = vecGPSLine.begin(); iterbegin != vecGPSLine.end(); ++iterbegin)
		{
			transform(iterbegin->begin(), iterbegin->end(), iterbegin->begin(), ::toupper);

			string strFind("X-UP-CALLING-LINE-ID:");
			unsigned int nSite = iterbegin->find(strFind);
			if(nSite != string::npos)
			{
				CString strLength(iterbegin->substr(strFind.length()).c_str());
				strLength.Trim();

				strTel = strLength.GetBuffer();
				strLength.LockBuffer();

				return strTel;
			}
		}
	}

	return strTel;
}

#define THINGTALK_MASK_MSGID			0X0001
#define THINGTALK_MASK_TID				0X0002
#define THINGTALK_MASK_MYNUMBER			0X0004
#define THINGTALK_MASK_TIME				0X0008
#define THINGTALK_MASK_WORKER_ID		0X0010
#define THINGTALK_MASK_SCANNER			0X0020
#define THINGTALK_MASK_PICTURE			0X0040
#define THINGTALK_MASK_TEXT				0X0080
#define THINGTALK_MASK_GPS				0X0100

#define THINGTALK_TID_MT_LOGIN ¡¡		0x01
#define THINGTALK_TID_MO_SCANNER		0x02
#define THINGTALK_TID_MO_GPS			0x03
#define THINGTALK_TID_MO_PICTURE		0x04
#define THINGTALK_TID_DISPLAY			0x05

int CDataPackParse::ParseGPSPicDataSite(string& strGPSContext)
{
	int nSite = 6;
	unsigned int nMask = *((unsigned int *)(&strGPSContext[0]));
	unsigned char cTID = *((unsigned char*)(&strGPSContext[5]));

	if(THINGTALK_TID_MO_PICTURE == cTID)
	{
		if(nMask & THINGTALK_MASK_MYNUMBER)
			nSite += 11;

		if(nMask & THINGTALK_MASK_TIME)
			nSite += 6;

		if(nMask & THINGTALK_MASK_WORKER_ID)
			nSite += 20;

		if(nMask & THINGTALK_MASK_GPS)
			nSite += 52;

		if(nMask & THINGTALK_MASK_SCANNER)
		{
			short nScannerLength = *((short *)(strGPSContext.c_str()[nSite]));
			nSite += nScannerLength;
		}

		return nSite;
	}
	return -1;
}