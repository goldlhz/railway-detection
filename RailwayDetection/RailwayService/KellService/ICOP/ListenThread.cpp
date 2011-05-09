#include "StdAfx.h"
#include "ListenThread.h"
#include "GobalConfig.h"
#include "CommonFunction.h"

#define ACCEPTEX_FAIL_WAIT_TIME									1000

CListenThread::CListenThread(void)
{
	m_hJudgeEvent[0] = NULL;									// acceptex�����¼�
	m_hJudgeEvent[1] = NULL;									// ���յ�һ�����ӵ���
	m_hCompletionPort = NULL;
	m_pGobalConfig = NULL;
}

CListenThread::~CListenThread(void)
{
	ReleaseAllSource();
}

bool CListenThread::InitListenThread(CGobalConfig * pGobalConfig)
{
	ASSERT(pGobalConfig);

	m_pGobalConfig = pGobalConfig;
	if(m_pGobalConfig)
	{
		int nListenPort = m_pGobalConfig->GetListenPort();

		if(!CreateIOCPHandle())
		{
			// ����IOCP�ܹ���Ҫ�õľ������
			return false;
		}
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThread(), CreateIOCPHandle()���óɹ�"));

		if(!CreateListenSocket(nListenPort))
		{
			// �����׽��ִ���ʧ��
			CloseIOCPHandle();
			return false;
		}
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThread(), CreateListenSocket()���óɹ�"));

		if(!m_MemoryPool.InitMemoryPool(m_pGobalConfig))
		{
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThread(), ��ʼ���ڴ�ش���"));

			CloseListenSocket();
			CloseIOCPHandle();

			return false;
		}
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThread(), ��ʼ���ڴ�سɹ�"));

		if(!m_SocketPool.InitSocketPool(m_pGobalConfig))
		{
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThread(), ��ʼ���׽��ֳش���"));

			CloseListenSocket();
			CloseIOCPHandle();

			return false;
		}
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThread(), ��ʼ���׽��ֳسɹ�"));

		if(SOCKET_ERROR == WSAEventSelect(m_sListenSocket.GetSocket(), m_hJudgeEvent[0], FD_ACCEPT))
		{
			int nErrorCode = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), ע��FD_ACCEPT�¼�����, �������:%d"), nErrorCode);

			CloseListenSocket();
			CloseIOCPHandle();

			return false;
		}
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), ע��FD_ACCEPT�¼��ɹ�"));


		HANDLE hHandle = CreateIoCompletionPort((HANDLE)m_sListenSocket.GetSocket(), m_hCompletionPort, (ULONG_PTR)NULL, NULL);
		if(!hHandle)
		{
			int nErrorCode = GetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), �󶨼����׽��ֵ���ɶ˿�ʱ����,�������:%d"), nErrorCode);

			CloseListenSocket();
			CloseIOCPHandle();

			return false;
		}
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), �󶨼����׽��ֵ���ɶ˿�ʱ�ɹ�"));

		if(CreateWorkThreadPool())
		{
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::InitListenThreadInstance(), ��ʼ�������߳����"));
			return true;
		}

		CloseListenSocket();
		CloseIOCPHandle();
		return false;

	}

	return false;
}
void CListenThread::UnInitListenThread()
{
	ReleaseAllSource();
	SafeTerminate();
}
unsigned int CListenThread::ExecuteThread()
{
	ASSERT(m_hJudgeEvent[0]);
	ASSERT(m_hJudgeEvent[1]);
	ASSERT(m_pGobalConfig);

	if(m_pGobalConfig)
	{
		int nErrorCode = 0;
		int nPostCount = 0;
		CGobalConfig::POSTACCEPTMODE PostMode;

		PostMode = m_pGobalConfig->GetPostMode();

		if(CGobalConfig::POST_BYKERNAL == PostMode)
			nPostCount = CCommonFunction::GetKernalCount() * m_pGobalConfig->GetPostTiems();		
		else
			nPostCount = m_pGobalConfig->GetPostMaxSize();

		for (int nEventIndex = 0; nEventIndex < nPostCount; ++nEventIndex)
		{
			if(!PostAcceptExMSG())
			{
				ReleaseAllSource();
				DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), ��һ����Ͷ��AcceptEx��Ϣ����,���������߳��˳�"));
				return 0;
			}
		}

		int nEventIndex = 0;
		WSANETWORKEVENTS networkEvent;

		while(!m_bIsTerminate)
		{			
			nEventIndex = ::WSAWaitForMultipleEvents(JUDGEHANDLESIZE, m_hJudgeEvent, FALSE, 60 * 1000, FALSE);

			if(WSA_WAIT_FAILED == nEventIndex)
			{
				nErrorCode = WSAGetLastError();
				ReleaseAllSource();
				WriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), ���������߳��е�WSAWaitForMultipleEvents()���س���,�߳��˳�:%d"), nErrorCode);
				return 0;
			}	

			if(WSA_WAIT_TIMEOUT == nEventIndex)
			{
				// ���Դ˼��һЩ����,�����Ȳ���
				continue;
			}

			nEventIndex = nEventIndex - WAIT_OBJECT_0;			
			if(nEventIndex >= 0)
			{
				switch (nEventIndex)
				{
				case 0:
					{
						::WSAEnumNetworkEvents(m_sListenSocket.GetSocket(), m_hJudgeEvent[0], &networkEvent);
						if(networkEvent.lNetworkEvents & FD_ACCEPT)
						{
							static int nCount;
							for (nCount = 0; nCount < nPostCount; ++nCount)
							{
								if(!PostAcceptExMSG())
								{
									DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), AcceptEx����ʱ��Ͷ��AcceptEx����,���������߳���Ϣ1�����Ͷ��"));
									SleepEx(ACCEPTEX_FAIL_WAIT_TIME, TRUE);
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
							SleepEx(ACCEPTEX_FAIL_WAIT_TIME, TRUE);
						}
					}
					break;
				default: 
					{
						WriteLogInfo(LOG_INFO, _T("CSocketListenThread::ExecuteThread(), WSAWaitForMultipleEvents()������δ֪����Ϣ,����Ϊ:%d"), nEventIndex + WAIT_OBJECT_0);
					}
					break;
				}
			}
		}
	}

	return 0;
}

bool CListenThread::CreateIOCPHandle()
{
	if(!m_hCompletionPort)
	{
		int nErrorCode = 0;

		m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if(!m_hCompletionPort)
		{
			nErrorCode = GetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), ������ɶ˿ڳ����������:%d"), nErrorCode);
			return false;
		}
		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), ������ɶ˿ڳɹ�"));

		m_hJudgeEvent[0] = WSACreateEvent();
		if(WSA_INVALID_EVENT == m_hJudgeEvent[0])
		{
			nErrorCode = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), �����¼�ѡ��ģ�͵��¼������������:%d"), nErrorCode);
			CloseHandle(m_hCompletionPort);

			return false;
		}
		DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateIOCPHandle(), �����¼�ѡ��ģ�͵��¼��ɹ�"));

		m_hJudgeEvent[1] = CreateEvent(NULL, FALSE, FALSE, _T("Global\\Notify_Accept_Link"));
		if(NULL == m_hJudgeEvent[1])
		{
			nErrorCode = GetLastError();
			WriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateIOCPHandle(), ����֪ͨ���յ�һ�������¼������������:%d"), nErrorCode);

			CloseHandle(m_hJudgeEvent[0]);
			CloseHandle(m_hCompletionPort);

			return false;
		}
		DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateIOCPHandle(), ����֪ͨ���յ�һ�������¼��ɹ�"));
	}

	return true;
}

bool CListenThread::CreateListenSocket(int nPort)
{
	ASSERT(m_hCompletionPort);

	if(nPort < 1024 || nPort > 65535)
	{
		DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateListenSocket(), �����ļ������õķ����������˿ڲ��ںϷ���Χ��"));
		return false;
	}

	if(m_hCompletionPort)
	{
		if(m_sListenSocket.CreateLocalSocket(nPort))
		{
			DoWriteLogInfo(LOG_DEBUG, _T("CSocketListenThread::CreateListenSocket(), �����������׽��ֳɹ�"));
			return true;
		}

		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateListenSocket(), �����������׽���ʧ��"));
	}
	return false;
}

bool CListenThread::CreateWorkThreadPool()
{
	if(m_hCompletionPort)
	{
		if(m_WorkThreadPool.CreateWorkThreads(
			m_hCompletionPort, 
			m_hJudgeEvent[1], 
			&m_sListenSocket, 
			&m_SocketPool, 
			&m_MemoryPool))

		{
			DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateWorkThreadPool(), �����������̳߳سɹ�"));
			return true;
		}

		DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::CreateWorkThreadPool(), �����������̳߳�ʧ��"));
	}
	return false;
}

bool CListenThread::CloseWorkThreadPool()
{
	return m_WorkThreadPool.CloseWorkThreads();
}

bool CListenThread::CloseListenSocket()
{
	return m_sListenSocket.CloseLocalSocket();
}

bool CListenThread::CloseIOCPHandle()
{
	ASSERT(m_hCompletionPort);

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

bool CListenThread::PostAcceptExMSG()
{
	BOOL   bTempTest = FALSE;
	DWORD  dReceviedSize = 0;
	SOCKET sClientSocket = INVALID_SOCKET;
	LPOverKeyPire pOverKeyPire = NULL;

	sClientSocket= m_SocketPool.GetNewSocket();
	pOverKeyPire = m_MemoryPool.GetNewOverKeyPire();

	if((INVALID_SOCKET != sClientSocket) && (NULL != pOverKeyPire))
	{
		pOverKeyPire->pireCompletionKey.keyOptSocket   = m_sListenSocket.GetSocket();
		pOverKeyPire->pireOverLappedex.wsaClientSocket = sClientSocket;
		pOverKeyPire->pireOverLappedex.wsaOptType      = CT_ACCP;

		bTempTest = m_sListenSocket.AcceptEx(
			m_sListenSocket.GetSocket(), 
			sClientSocket,
			pOverKeyPire->pireOverLappedex.wsaAcceptBuffer,
			0,
			ADDRESS_LENGTH,
			ADDRESS_LENGTH,
			&(dReceviedSize),
			(LPOVERLAPPED)pOverKeyPire
			);

		if(!bTempTest)
		{
			int nErrorCode = WSAGetLastError();
			if(ERROR_IO_PENDING == nErrorCode)
			{
				return true;
			}
			else if(WSAENOBUFS == nErrorCode)
			{
				TRACE(("accpetex ����һ����Դ�������:%d\n"), nErrorCode);
				WriteLogInfo(LOG_INFO, _T("CSocketListenThread::PostAcceptEx(), �����߳�Ͷ��AccpetEXʱϵͳ��Դ���㣬�������:%d"), nErrorCode);

				m_SocketPool.CloseSpecSocket(sClientSocket);
				m_MemoryPool.ReleaseOverKeyPire(pOverKeyPire);
				
				return false;
			}
			else
			{
				m_SocketPool.CloseSpecSocket(sClientSocket);
				m_MemoryPool.ReleaseOverKeyPire(pOverKeyPire);

				TRACE(("accpetex ����һ��δ֪����:%d\n"), nErrorCode);
				WriteLogInfo(LOG_INFO, _T("CSocketListenThread::PostAcceptEx(), �����߳�Ͷ��AccpetEXʱ�����������:%d"), nErrorCode);
				return false;
			}
		}

		return true;
	}

	DoWriteLogInfo(LOG_INFO, _T("CSocketListenThread::PostAcceptEx(), �ع����������޷��������ʹ�õ���Դ"));
	return false;
}

void CListenThread::ReleaseAllSource()
{
	CloseWorkThreadPool();
	CloseListenSocket();
	CloseIOCPHandle();
}
