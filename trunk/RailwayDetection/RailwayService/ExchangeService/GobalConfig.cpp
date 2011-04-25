#include "StdAfx.h"
#include "GobalConfig.h"
#include "CommonFunction.h"

CGobalConfig CGobalConfig::m_gobalconfig;
CGobalConfig::CGobalConfig(void)
{
	m_nMemoryDownLine = 10;
	m_nMemoryPoolSize = 2000;
	m_nMemoryRecycleSize = 3000;

	m_nPostMode = POST_BYKERNAL;
	m_nPostTimes = 100;
	m_nPostMaxSize = 1000;

	m_nSocketDownLine = 10;
	m_nSocketPoolSize = 4400;
	m_nSocketRecycleSize = 5400;

	m_nServerPort = 9527;
}

bool CGobalConfig::InitGoblaConfig(HMODULE hInstance)
{
	int nTemp = 0;
	wstring strIniPath = CCommonFunction::GetLogPluginPath(hInstance);
	
	if(strIniPath.empty())
		strIniPath = _T(".\\System.ini");

	strIniPath += _T("\\System.ini");

	// 内存池配置
	m_nMemoryDownLine = GetPrivateProfileInt(_T("MemoryPool"), _T("MemoryDownLine"), m_nMemoryDownLine, strIniPath.c_str());
	m_nMemoryPoolSize = GetPrivateProfileInt(_T("MemoryPool"), _T("MemoryPoolSize"), m_nMemoryPoolSize, strIniPath.c_str());
	m_nMemoryRecycleSize = GetPrivateProfileInt(_T("MemoryPool"), _T("MemoryRecycleSize"), m_nMemoryRecycleSize, strIniPath.c_str());

	// AcceptEX投递方式
	nTemp = GetPrivateProfileInt(_T("AcceptExPool"), _T("PostMode"), 0, strIniPath.c_str());
	if(nTemp == 0)
		m_nPostMode = POST_BYKERNAL;
	else
		m_nPostMode = POST_BYCONFIG;
	m_nPostTimes = GetPrivateProfileInt(_T("AcceptExPool"), _T("PostTimes"), m_nPostTimes, strIniPath.c_str());
	m_nPostMaxSize = GetPrivateProfileInt(_T("AcceptExPool"), _T("PostMaxSize"), m_nPostMaxSize, strIniPath.c_str());

	// SocketPool配置
	m_nSocketDownLine = GetPrivateProfileInt(_T("SocketPool"), _T("SocketDownLine"), m_nSocketDownLine, strIniPath.c_str());
	m_nSocketPoolSize = GetPrivateProfileInt(_T("SocketPool"), _T("SocketPoolSize"), m_nSocketPoolSize, strIniPath.c_str());
	m_nSocketRecycleSize = GetPrivateProfileInt(_T("SocketPool"), _T("SocketRecycleSize"), m_nSocketRecycleSize, strIniPath.c_str());

	// 服务端口配
	m_nServerPort = GetPrivateProfileInt(_T("Server"), _T("ServerPort"), m_nServerPort, strIniPath.c_str());

	return true;
}
