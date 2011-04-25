#pragma once

class CGobalConfig
{
private:
	CGobalConfig(void);
public:
	enum POSTACCEPTMODE{POST_BYKERNAL = 0, POST_BYCONFIG = 1,};

	int GetMemoryDownLine(){return m_nMemoryDownLine;};
	int GetMemoryPoolSize(){return m_nMemoryPoolSize;};
	int GetMemoryRecycleSize(){return m_nMemoryRecycleSize;};
	
	POSTACCEPTMODE GetPostMode(){return m_nPostMode;};
	int GetPostTiems(){return m_nPostTimes;};
	int GetPostMaxSize(){return m_nPostMaxSize;};

	int GetSocketDownLine(){return m_nSocketDownLine;};
	int GetSocketPoolSize(){return m_nSocketPoolSize;};
	int GetSocketRecycleSize(){return m_nSocketRecycleSize;};

	int GetServerPort(){return m_nServerPort;};

	bool InitGoblaConfig(HMODULE hInstance);

	static CGobalConfig& GetGobalConfig(){return m_gobalconfig;};

private:
	int m_nMemoryDownLine;
	int m_nMemoryPoolSize;
	int m_nMemoryRecycleSize;
	
	POSTACCEPTMODE m_nPostMode;
	int m_nPostTimes;
	int m_nPostMaxSize;

	int m_nSocketDownLine;
	int m_nSocketPoolSize;
	int m_nSocketRecycleSize;

	int m_nServerPort;

	static CGobalConfig m_gobalconfig;
};
