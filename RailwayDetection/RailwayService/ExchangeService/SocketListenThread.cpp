#include "StdAfx.h"
#include "SocketListenThread.h"
#include "CommonFunction.h"
#include "GobalConfig.h"
#include "PoolManager.h"


CSocketListenThread::CSocketListenThread(void)
{
	m_hJudgeEvent[0] = NULL;									// acceptex�����¼�
	m_hJudgeEvent[1] = NULL;									// ���յ�һ�������¼�
	m_hCompletionPort = NULL;
}

CSocketListenThread::~CSocketListenThread(void)
{
	ReleaseAllSource();
}

unsigned int CSocketListenThread::ExecuteThread()
{
	ASSERT(m_hJudgeEvent[0]);
	ASSERT(m_hJudgeEvent[1]);

	int nErrorCode = 0;
	int nPostCount = 0;

	CGobalConfig::POSTACCEPTMODE PostMode = CGobalConfig::GetGobalConfig().GetPostMode();
	if(CGobalConfig::POST_BYKERNAL == PostMode)
		nPostCount = CCommonFunction::GetCurrentKernalCount() * CGobalConfig::GetGobalConfig().GetPostTiems();		
	else
		nPostCount = CGobalConfig::GetGobalConfig().GetPostMaxSize();

	//for (int nIndex = 0; nIndex < nPostCount; ++nIndex)
	for (int nIndex = 0; nIndex < 1; ++nIndex)
	{
		if(!PostAcceptExMSG())
		{
			ReleaseAllSource();
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), ��һ����Ͷ��AcceptEx��Ϣ����,���������߳��˳�"));
			return 0;
		}
	}

	while(!m_bIsTerminate)
	{
		static int nIndex = 0;
		nIndex = ::WSAWaitForMultipleEvents(JUDGEHANDLESIZE, m_hJudgeEvent, FALSE, 60 * 1000, FALSE);

		if(WSA_WAIT_FAILED == nIndex)
		{
			ReleaseAllSource();
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), ���������߳��е�WSAWaitForMultipleEvents()���س���,�߳��˳�"));
			return 0;
		}	

		if(WSA_WAIT_TIMEOUT == nIndex)
		{
			// ���Դ˼��һЩ����,�����Ȳ���
			continue;
		}

		nIndex = nIndex - WAIT_OBJECT_0;			
		if(nIndex >= 0)
		{
			static WSANETWORKEVENTS networkEvent;
			switch (nIndex)
			{
			case 0:
				{
					::WSAEnumNetworkEvents(m_BaseSocket.GetSocket(), m_hJudgeEvent[0], &networkEvent);
					if(networkEvent.lNetworkEvents & FD_ACCEPT)
					{
						static int nCount;
						for (nCount = 0; nCount < nPostCount; ++nCount)
						{
							if(!PostAcceptExMSG())
							{
								DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), AcceptEx����ʱ��Ͷ��AcceptEx����,���������߳���Ϣ2�����Ͷ��"));
								SleepEx(100, TRUE);
							}
						}
					}
				}
				break;
			case 1:
				{
					if(!PostAcceptExMSG())
					{
						DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), ���յ������Ӻ�������AcceptExͶ��ʱ����"));
						SleepEx(100, TRUE);
					}
				}
				break;
			default: 
				{
					WriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), WSAWaitForMultipleEvents()������δ֪����Ϣ,����Ϊ:%d"), nIndex + WAIT_OBJECT_0);
				}
				break;
			}
		}
	}

	return 0;
}

bool CSocketListenThread::InitListenThreadInstance()
{
	int nPort = CGobalConfig::GetGobalConfig().GetServerPort();

	if(!CreateIOCPHandle())
	{
		// ����IOCP��Ҫ�õľ������
		return false;
	}
	DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), CreateIOCPHandle()���óɹ�"));

	if(!CreateServerSocket(nPort))
	{
		// ����Socket����
		CloseIOCPHandle();
		return false;
	}
	DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), CreateServerSocket()���óɹ�"));

	if(!m_MemoryPoolManager.InitMemoryPool())
	{
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), ��ʼ���ڴ�ش���"));

		CloseServerSocket();
		CloseIOCPHandle();

		return false;
	}

	DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), ��ʼ���ڴ�سɹ�"));

	if(!m_SocketPoolManager.InitSocketPool())
	{
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), ��ʼ���׽��ֳش���"));

		CloseServerSocket();
		CloseIOCPHandle();

		return false;
	}
				
	DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), ��ʼ���׽��ֳسɹ�"));

	HANDLE hHandle = CreateIoCompletionPort((HANDLE)m_BaseSocket.GetSocket(), m_hCompletionPort, (ULONG_PTR)NULL, NULL);
	if(hHandle)
	{
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), �󶨼����׽��ֵ���ɶ˿�ʱ�ɹ�"));

		if(SOCKET_ERROR == WSAEventSelect(m_BaseSocket.GetSocket(), m_hJudgeEvent[0], FD_ACCEPT))
		{
			int nErrorCode = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), ע��FD_ACCEPT�¼�����, �������:%d"), nErrorCode);
		}
		else
		{
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), ע��FD_ACCEPT�¼��ɹ�"));

			if(CreateServerThreadPool())
			{
				DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), ��ʼ�������߳����"));
				return true;
			}
		}
	}
	else
	{
		int nErrorCode = GetLastError();
		WriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), �󶨼����׽��ֵ���ɶ˿�ʱ����,�������:%d"), nErrorCode);
	}
	return false;
}

void CSocketListenThread::UnInitListenThreadInstance()
{
	ReleaseAllSource();
	SafeTerminate();
}

bool CSocketListenThread::CreateIOCPHandle()
{
	if(!m_hCompletionPort)
	{
		m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if(!m_hCompletionPort)
		{
			int nErrorCode = GetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), ������ɶ˿ڳ����������:%d"), nErrorCode);
			return false;
		}

		DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateIOCPHandle(), ������ɶ˿ڳɹ�"));

		m_hJudgeEvent[0] = WSACreateEvent();
		if(WSA_INVALID_EVENT == m_hJudgeEvent[0])
		{
			int nErrorCode = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), �����¼�ѡ��ģ�͵��¼������������:%d"), nErrorCode);

			CloseHandle(m_hCompletionPort);
	
			return false;
		}
		
		DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateIOCPHandle(), �����¼�ѡ��ģ�͵��¼��ɹ�"));

		m_hJudgeEvent[1] = CreateEvent(NULL, FALSE, FALSE, _T("Global\\Notify_Accept_Link"));
		if(NULL == m_hJudgeEvent[1])
		{
			int nErrorCode = GetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), ����֪ͨ���յ�һ�������¼������������:%d"), nErrorCode);

			CloseHandle(m_hJudgeEvent[0]);
			CloseHandle(m_hCompletionPort);

			return false;
		}

		DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateIOCPHandle(), ����֪ͨ���յ�һ�������¼��ɹ�"));
	}
	return true;
}

bool CSocketListenThread::CreateServerSocket(int nPort)
{
	if(m_hCompletionPort)
	{
		if(m_BaseSocket.CreateListenSocket(nPort))
		{
			DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateServerSocket(), �����������׽��ֳɹ�"));
			return true;
		}

		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateServerSocket(), �����������׽���ʧ��"));
	}
	return false;
}

bool CSocketListenThread::CreateServerThreadPool()
{
	if(m_hCompletionPort)
	{
		if(m_ServerThreadPool.CreateServerThreads(m_hCompletionPort, 
			m_hJudgeEvent[1], 
			&m_BaseSocket, 
			&m_SocketPoolManager, 
			&m_MemoryPoolManager))

		{
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateServerThreadPool(), �����������̳߳سɹ�"));
			return true;
		}

		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateServerThreadPool(), �����������̳߳�ʧ��"));
	}
	return false;
}

bool CSocketListenThread::CloseServerThreadPool()
{
	return m_ServerThreadPool.CloseServerThreads();
}

bool CSocketListenThread::CloseServerSocket()
{
	return m_BaseSocket.CloseSocket();
}

bool CSocketListenThread::CloseIOCPHandle()
{
	if(m_hCompletionPort)
	{
		CloseHandle(m_hJudgeEvent[1]);
		m_hJudgeEvent[1] = NULL;

		CloseHandle(m_hJudgeEvent[0]);
		m_hJudgeEvent[0] = NULL;

		CloseHandle(m_hCompletionPort);
		m_hCompletionPort = NULL;

		DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CloseIOCPHandle(), �ر���ɶ˿�"));
	}
	return true;
}

bool CSocketListenThread::PostAcceptExMSG()
{
	// ���� 
	ASSERT(m_hCompletionPort);

	BOOL   bTempTest = FALSE;
	DWORD  dReceviedSize = 0;
	SOCKET scSocket = INVALID_SOCKET;
	KeyOverPire * pKeyOverPire = NULL;

	scSocket     = m_SocketPoolManager.GetNewSocket();
	pKeyOverPire = m_MemoryPoolManager.GetNewKeyOverPire();

	if(scSocket != INVALID_SOCKET && pKeyOverPire != NULL)
	{
		pKeyOverPire->pireCompletionKey.keySocket = m_BaseSocket.GetSocket();

		pKeyOverPire->pireOverLappedex.wsaWSABuf.len = BUFFER_SIZE_TO_SOCKET;
		pKeyOverPire->pireOverLappedex.wsaWSABuf.buf = pKeyOverPire->pireOverLappedex.wsaBuffer;

		pKeyOverPire->pireOverLappedex.wsaClientSocket = scSocket;
		pKeyOverPire->pireOverLappedex.wsaOptType = CT_ACCP;

		bTempTest = m_BaseSocket.AcceptEx(
			m_BaseSocket.GetSocket(), 
			scSocket,
			pKeyOverPire->pireOverLappedex.wsaAcceptBuffer,
			0,
			ADDRESS_LENGTH,
			ADDRESS_LENGTH,
			&(dReceviedSize),
			&(pKeyOverPire->pireOverLappedex.wsaOverlapped)
			);

		if(!bTempTest)
		{
			int nErrorCode = WSAGetLastError();
			if(ERROR_IO_PENDING == nErrorCode)
				return true;

			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::PostAcceptEx(), �����׽�����Ϊ����״̬ʱ�����������:%d"), nErrorCode);
			return false;
		}

		return true;
	}

	DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::PostAcceptEx(), �ع����������޷��������ʹ�õ���Դ"));
	return false;
}

void CSocketListenThread::ReleaseAllSource()
{
	CloseServerThreadPool();
	CloseServerSocket();
	CloseIOCPHandle();
}