#pragma once
#include "IOCPDefine.h"

class CGobalConfig;
class CMemoryPool
{
public:
	CMemoryPool(void);
	virtual ~CMemoryPool(void);

public:
	LPOverKeyPire GetNewOverKeyPire();										// 从池中得到一个Pire
	void ReleaseOverKeyPire(LPOverKeyPire pPire);							// 释放一个Pire到池中
	unsigned int GetUsedMemoryPoolSize(){return m_vecUsedOverKeyPire.size();};
	unsigned int GetPrepMemoryPoolSize(){return m_vecPrepOverKeyPire.size();};
	bool InitMemoryPool(CGobalConfig * pGobalConfig);						// 初始化内存池
	void ClearMemoryPool();													// 清空Pire池
	void RecycleMemory();													// 回收多余的池资源
private:
	bool MemoryPoolAutoManage();											// 扫描池，并作内存池自动管理工作
	bool IncrementMemoryPoolSize(unsigned int nCurrentSize);				// 增大池大小
	bool DecrescenceMemoryPoolSize(unsigned int nCurrentSize);				// 减小池大小

	typedef list<LPOverKeyPire> CListOverKeyPire;							// 定义类型
	CGobalConfig    *m_pGobalConfig;										// 全局配置
	HANDLE			 m_hEvent;
	CCriticalSection m_scMemoryCriticalSection;								// 内存池保护临界区
	CListOverKeyPire m_vecUsedOverKeyPire;									// 已经在使用的完成重叠对
	CListOverKeyPire m_vecPrepOverKeyPire;									// 没有使用的完成重叠对
};

