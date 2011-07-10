#include "StdAfx.h"
#include "MemoryPool.h"
#include "GobalConfig.h"

CMemoryPool::CMemoryPool()
{
	m_pGobalConfig = NULL;
	m_hEvent = NULL; 
	m_vecPrepOverKeyPire.clear();
	m_vecUsedOverKeyPire.clear();
}

CMemoryPool::~CMemoryPool()
{
	ClearMemoryPool();
}

LPOverKeyPire CMemoryPool::GetNewOverKeyPire()
{
	LPOverKeyPire pOverKeyPire = NULL;
	CListOverKeyPire::iterator iterBack;

	m_scMemoryCriticalSection.Lock();

	if(MemoryPoolAutoManage())
	{
		iterBack = m_vecPrepOverKeyPire.begin();
		pOverKeyPire = (LPOverKeyPire)(*iterBack);

		m_vecPrepOverKeyPire.erase(iterBack);
		m_vecUsedOverKeyPire.push_back(pOverKeyPire);
	}

	m_scMemoryCriticalSection.Unlock();
	return pOverKeyPire;
}

void CMemoryPool::ReleaseOverKeyPire(LPOverKeyPire pPire)
{
	if(pPire)
	{
		CListOverKeyPire::iterator iterTemp;
		m_scMemoryCriticalSection.Lock();

		for (iterTemp = m_vecUsedOverKeyPire.begin(); iterTemp != m_vecUsedOverKeyPire.end(); ++iterTemp)
		{
			if(pPire == (LPOverKeyPire)(*iterTemp))
			{
				m_vecUsedOverKeyPire.erase(iterTemp);
				m_vecPrepOverKeyPire.push_back(pPire);
				break;
			}
		}

		m_scMemoryCriticalSection.Unlock();
	}
}

bool CMemoryPool::InitMemoryPool(CGobalConfig * pGobalConfig)
{
	ASSERT(pGobalConfig);
	if(!pGobalConfig)
		return false;

	m_pGobalConfig = pGobalConfig;

	unsigned int nMemoryPoolSize = pGobalConfig->GetMemoryPoolSize();;
	unsigned int nPrepMemoryPoolSize = m_vecPrepOverKeyPire.size();

	if(nPrepMemoryPoolSize < nMemoryPoolSize)
	{
		LPOverKeyPire pPire;
		for(unsigned int nIndex = nPrepMemoryPoolSize; nIndex < nMemoryPoolSize; ++nIndex)
		{
			pPire = (LPOverKeyPire)::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(OverKeyPire));
			if(pPire)
			{
				m_vecPrepOverKeyPire.push_back(pPire);
				continue;
			}
			return false;
		}
	}
	return true;
}

void CMemoryPool::ClearMemoryPool()
{
	CListOverKeyPire::iterator iterTemp;
	m_scMemoryCriticalSection.Lock();

	if(!m_vecUsedOverKeyPire.empty())
	{
		for (iterTemp = m_vecUsedOverKeyPire.begin(); iterTemp != m_vecUsedOverKeyPire.end(); ++iterTemp)
		{
			::HeapFree(::GetProcessHeap(), 0, (LPVOID)(*iterTemp));
		}

		m_vecUsedOverKeyPire.clear();
	}

	if(!m_vecPrepOverKeyPire.empty())
	{
		for (iterTemp = m_vecPrepOverKeyPire.begin(); iterTemp != m_vecPrepOverKeyPire.end(); ++iterTemp)
		{
			::HeapFree(::GetProcessHeap(), 0, (LPVOID)(*iterTemp));
		}

		m_vecPrepOverKeyPire.clear();
	}

	m_scMemoryCriticalSection.Unlock();
}

void CMemoryPool::RecycleMemory()
{
	unsigned int nMemoryPoolSize = 0;
	m_scMemoryCriticalSection.Lock();

	nMemoryPoolSize = m_vecPrepOverKeyPire.size();
	if(nMemoryPoolSize > m_pGobalConfig->GetMemoryRecycleSize())
	{
		DecrescenceMemoryPoolSize(nMemoryPoolSize);
	}

	m_scMemoryCriticalSection.Unlock();
}

bool CMemoryPool::MemoryPoolAutoManage()
{
	ASSERT(m_pGobalConfig);
	if(!m_pGobalConfig)
		return false;

	unsigned int nPrepOverKeyPireSize = m_vecPrepOverKeyPire.size();
	if(nPrepOverKeyPireSize < m_pGobalConfig->GetMemoryDownLine())
	{
		return IncrementMemoryPoolSize(nPrepOverKeyPireSize);
	}
	else if(nPrepOverKeyPireSize > m_pGobalConfig->GetMemoryRecycleSize())
	{
		return DecrescenceMemoryPoolSize(nPrepOverKeyPireSize);
	}
	return true;
}

bool CMemoryPool::IncrementMemoryPoolSize(unsigned int nCurrentSize)
{
	ASSERT(m_pGobalConfig);
	if(!m_pGobalConfig)
		return false;

	if(nCurrentSize >= m_pGobalConfig->GetMemoryPoolSize())
		return true;

	LPOverKeyPire pPire;
	for( ; nCurrentSize < m_pGobalConfig->GetMemoryPoolSize(); ++nCurrentSize)
	{
		pPire = (LPOverKeyPire)::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(OverKeyPire));
		if(pPire)
		{
			m_vecPrepOverKeyPire.push_back(pPire);
			continue;
		}
		return false;
	}
	return true;
}

bool CMemoryPool::DecrescenceMemoryPoolSize(unsigned int nCurrentSize)
{
	ASSERT(m_pGobalConfig);
	if(!m_pGobalConfig)
		return false;

	unsigned int nMemorySize = m_pGobalConfig->GetMemoryPoolSize();
	if(nCurrentSize > nMemorySize)
	{
		CListOverKeyPire::iterator iterPire;

		for (; nCurrentSize > nMemorySize; --nCurrentSize)
		{
			iterPire = m_vecPrepOverKeyPire.end();
			--iterPire;

			::HeapFree(::GetProcessHeap(), 0, (LPVOID)(*iterPire));
			m_vecPrepOverKeyPire.erase(iterPire);
		}
	}
	return true;
}
