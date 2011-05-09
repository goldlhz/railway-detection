#pragma once
#include "IOCPDefine.h"

class CGobalConfig;
class CMemoryPool
{
public:
	CMemoryPool(void);
	virtual ~CMemoryPool(void);

public:
	LPOverKeyPire GetNewOverKeyPire();										// �ӳ��еõ�һ��Pire
	void ReleaseOverKeyPire(LPOverKeyPire pPire);							// �ͷ�һ��Pire������
	unsigned int GetUsedMemoryPoolSize(){return m_vecUsedOverKeyPire.size();};
	unsigned int GetPrepMemoryPoolSize(){return m_vecPrepOverKeyPire.size();};
	bool InitMemoryPool(CGobalConfig * pGobalConfig);						// ��ʼ���ڴ��
	void ClearMemoryPool();													// ���Pire��
	void RecycleMemory();													// ���ն���ĳ���Դ
private:
	bool MemoryPoolAutoManage();											// ɨ��أ������ڴ���Զ�������
	bool IncrementMemoryPoolSize(unsigned int nCurrentSize);				// ����ش�С
	bool DecrescenceMemoryPoolSize(unsigned int nCurrentSize);				// ��С�ش�С

	typedef list<LPOverKeyPire> CListOverKeyPire;							// ��������
	CGobalConfig    *m_pGobalConfig;										// ȫ������
	HANDLE			 m_hEvent;
	CCriticalSection m_scMemoryCriticalSection;								// �ڴ�ر����ٽ���
	CListOverKeyPire m_vecUsedOverKeyPire;									// �Ѿ���ʹ�õ�����ص���
	CListOverKeyPire m_vecPrepOverKeyPire;									// û��ʹ�õ�����ص���
};

