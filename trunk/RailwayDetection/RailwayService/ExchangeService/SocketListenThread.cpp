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
}

unsigned int CSocketListenThread::ExecuteThread()
{
	ASSERT(m_hJudgeEvent[0]);
	ASSERT(m_hJudgeEvent[1]);

	int nError = 0;
	int nPostCount = 0;

	CGobalConfig::POSTACCEPTMODE PostMode = CGobalConfig::GetGobalConfig().GetPostMode();
	if(CGobalConfig::POST_BYKERNAL == PostMode)
		nPostCount = CCommonFunction::GetCurrentKernalCount() * CGobalConfig::GetGobalConfig().GetPostTiems();		
	else
		nPostCount = CGobalConfig::GetGobalConfig().GetPostMaxSize();

	for (int nIndex = 0; nIndex < nPostCount; ++nIndex)
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
		int nIndex = ::WSAWaitForMultipleEvents(JUDGEHANDLESIZE, m_hJudgeEvent, FALSE, WSA_INFINITE, FALSE);

		if(WSA_WAIT_FAILED == nIndex)
		{
			ReleaseAllSource();
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), ���������߳��е�WSAWaitForMultipleEvents()���س���,�߳��˳�"));
			return 0;
		}	

		nIndex = nIndex - WAIT_OBJECT_0;			
		if(nIndex >= 0)
		{
			WSANETWORKEVENTS networkEvent;
			switch (nIndex)
			{
			case 0:
				{
					::WSAEnumNetworkEvents(m_BaseSocket.GetSocket(), m_hJudgeEvent[0], &networkEvent);
					if(networkEvent.lNetworkEvents & FD_ACCEPT)
					{
						for (int nIndex = 0; nIndex < nPostCount; ++nIndex)
						{
							if(!PostAcceptExMSG())
							{
								DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), AcceptEx����ʱ��Ͷ��AcceptEx����,���������߳���Ϣ2�����Ͷ��"));
								SleepEx(2000, TRUE);
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

	if(CreateIOCPHandle())
	{
		if(CreateServerSocket(nPort))
		{
			if(m_MemoryPoolManager.InitMemoryPool())
			{
				DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::InitListenThreadInstance(), ��ʼ���ڴ�سɹ�"));

				if(m_SocketPoolManager.InitSocketPool())
				{
					DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::InitListenThreadInstance(), ��ʼ���׽��ֳسɹ�"));

					HANDLE hHandle = CreateIoCompletionPort((HANDLE)m_BaseSocket.GetSocket(), m_hCompletionPort, (ULONG_PTR)NULL, NULL);
					if(hHandle)
					{
						DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::InitListenThreadInstance(), �󶨼����׽��ֵ���ɶ˿�ʱ�ɹ�"));

						if(SOCKET_ERROR == WSAEventSelect(m_BaseSocket.GetSocket(), m_hJudgeEvent[0], FD_ACCEPT))
						{
							int nError = WSAGetLastError();
							WriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), ע��FD_ACCEPT�¼�����, �������:%d"), nError);
						}
						else
						{
							DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::InitListenThreadInstance(), ע��FD_ACCEPT�¼��ɹ�"));

							if(CreateServerThreadPool())
							{
								DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::InitListenThreadInstance(), ��ʼ�������߳����"));
								return true;
							}
						}
					}
					else
					{
						int nError = GetLastError();
						WriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), �󶨼����׽��ֵ���ɶ˿�ʱ����,�������:%d"), nError);
					}
				}

				m_MemoryPoolManager.ClearMemoryPool();
			}
			CloseServerSocket();
		}
		CloseIOCPHandle();
	}

	DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), ��ʼ���ڴ�س���"));
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
		if(m_hCompletionPort)
		{
			DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateIOCPHandle(), ������ɶ˿ڳɹ�"));

			m_hJudgeEvent[0] = WSACreateEvent();
			if(WSA_INVALID_EVENT != m_hJudgeEvent[0])
			{
				DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateIOCPHandle(), ���������ڽ��������¼��ɹ�"));

				m_hJudgeEvent[1] = CreateEvent(NULL, FALSE, FALSE, _T("Global\\Notify_Accept_Link"));
				if(m_hJudgeEvent[1])
				{
					DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateIOCPHandle(), ����֪ͨ���յ�һ�������¼��ɹ�"));
					return true;
				}

				int nError = GetLastError();
				WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), ����֪ͨ���յ�һ�������¼������������:%d"), nError);
				return false;
			}

			int nError = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), ���������ڽ��������¼������������:%d"), nError);
			return false;
		}

		int nError = GetLastError();
		WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), ������ɶ˿ڳ����������:%d"), nError);
		return false;
	}
	return true;
}

bool CSocketListenThread::CreateServerSocket(int nPort)
{
	if(m_hCompletionPort)
	{
		if(m_BaseSocket.CreateListenSocket(nPort))
		{
			DoWriteLogInfo(LOG_DEBUG, _T("�����������׽��ֳɹ�"));
			return true;
		}

		DoWriteLogInfo(LOG_DEBUG, _T("�����������׽���ʧ��"));
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
			DoWriteLogInfo(LOG_DEBUG, _T("�����������̳߳سɹ�"));
			return true;
		}

		DoWriteLogInfo(LOG_DEBUG, _T("�����������̳߳�ʧ��"));
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
		pKeyOverPire->pireOverLappedex.wsaClientSocket = scSocket;
		pKeyOverPire->pireOverLappedex.wsaWSABuf.buf = (CHAR *)pKeyOverPire->pireOverLappedex.wsaBuffer;
		pKeyOverPire->pireOverLappedex.wsaOptType = CT_ACCP;

		bTempTest = m_BaseSocket.AcceptEx(
			m_BaseSocket.GetSocket(), 
			scSocket,
			pKeyOverPire->pireOverLappedex.wsaAcceptBuffer,
			0,
			sizeof(sockaddr_in) + 16,
			sizeof(sockaddr_in) + 16,
			&(dReceviedSize),
			&(pKeyOverPire->pireOverLappedex.wsaOverlapped)
			);

		if(!bTempTest)
		{
			int nError = WSAGetLastError();
			if(ERROR_IO_PENDING == nError)
				return true;

			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::PostAcceptEx(), �����׽�����Ϊ����״̬ʱ�����������:%d"), nError);
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