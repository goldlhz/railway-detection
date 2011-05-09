#pragma once

class CGobalConfig
{
public:
	CGobalConfig(void);
	~CGobalConfig(void);
	enum POSTACCEPTMODE{POST_BYKERNAL = 0, POST_BYCONFIG = 1,};

	unsigned int GetMemoryDownLine(){return m_nMemoryDownLine;};
	unsigned int GetMemoryPoolSize(){return m_nMemoryPoolSize;};
	unsigned int GetMemoryRecycleSize(){return m_nMemoryRecycleSize;};

	POSTACCEPTMODE GetPostMode(){return m_nPostMode;};
	unsigned int GetPostTiems(){return m_nPostTimes;};
	unsigned int GetPostMaxSize(){return m_nPostMaxSize;};

	unsigned int GetSocketDownLine(){return m_nSocketDownLine;};
	unsigned int GetSocketPoolSize(){return m_nSocketPoolSize;};
	unsigned int GetSocketRecycleSize(){return m_nSocketRecycleSize;};

	unsigned int GetListenPort(){return m_nServerPort;};

	bool InitGoblaConfig(HMODULE hInstance);

private:
	unsigned int m_nMemoryDownLine;
	unsigned int m_nMemoryPoolSize;
	unsigned int m_nMemoryRecycleSize;

	POSTACCEPTMODE m_nPostMode;
	unsigned int m_nPostTimes;
	unsigned int m_nPostMaxSize;

	unsigned int m_nSocketDownLine;
	unsigned int m_nSocketPoolSize;
	unsigned int m_nSocketRecycleSize;

	unsigned int m_nServerPort;
};

