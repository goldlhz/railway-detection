#include "StdAfx.h"
#include "CommonFunction.h"
#include "BaseSocket.h"
#include <MSWSock.h>

CBaseSocket::CBaseSocket(void)
{
	m_scSocket = SOCKET_ERROR;
	m_lpfnAcceptEx = NULL;
	m_lpfnGetAcceptExSockaddrs = NULL;
}

CBaseSocket::~CBaseSocket(void)
{
	CloseSocket();
}

BOOL CBaseSocket::AcceptEx(
						   SOCKET sListenSocket,
						   SOCKET sAcceptSocket,
						   PVOID lpOutputBuffer,
						   DWORD dwReceiveDataLength,
						   DWORD dwLocalAddressLength,
						   DWORD dwRemoteAddressLength,
						   LPDWORD lpdwBytesReceived,
						   LPOVERLAPPED lpOverlapped
						   )
{
	if(m_lpfnAcceptEx)
	{
		return m_lpfnAcceptEx(
			sListenSocket,
			sAcceptSocket,
			lpOutputBuffer,
			dwReceiveDataLength,
			dwLocalAddressLength,
			dwRemoteAddressLength,
			lpdwBytesReceived,
			lpOverlapped
			);
	}

	return FALSE;
}

bool CBaseSocket::GetAcceptExSockaddrs(
									   PVOID lpOutputBuffer,
									   DWORD dwReceiveDataLength,
									   DWORD dwLocalAddressLength,
									   DWORD dwRemoteAddressLength,
									   LPSOCKADDR* LocalSockaddr,
									   LPINT LocalSockaddrLength,
									   LPSOCKADDR* RemoteSockaddr,
									   LPINT RemoteSockaddrLength
									   )
{
	if(m_lpfnGetAcceptExSockaddrs)
	{
		m_lpfnGetAcceptExSockaddrs(
			lpOutputBuffer,
			dwReceiveDataLength,
			dwLocalAddressLength,
			dwRemoteAddressLength,
			LocalSockaddr,
			LocalSockaddrLength,
			RemoteSockaddr,
			RemoteSockaddrLength
			);

		return true;
	}
	return false;
}

bool CBaseSocket::CreateListenSocket(int nPort)
{
	if(CreateSocketInstance())
	{
		if(!BindListenSocket(nPort))
		{
			CloseSocket();
			return false;
		}

		if(!GetSocket2FunPointer())
		{
			CloseSocket();
			return false;
		}
		
		DoWriteLogInfo(LOG_INFO, _T("CBaseSocket::CreateListenSocket(), �ɹ���ɷ�����׽��ֵĴ���"));
		return true;
	}

	return false;
}

bool CBaseSocket::CloseSocket()
{
	if(SOCKET_ERROR != m_scSocket)
	{
		int nBack = 0;
		shutdown(m_scSocket,  SD_BOTH);
		nBack = closesocket(m_scSocket);
		m_scSocket = SOCKET_ERROR;

		return (nBack == 0) ? true : false;
	}

	return true;
}

void CBaseSocket::GetServerAddr(struct sockaddr_in * pscinTcpAddr)
{
	if(pscinTcpAddr)
	{
		memcpy(pscinTcpAddr, &m_scinTcpAddr, sizeof(struct sockaddr_in));
	}
}

bool CBaseSocket::InitSocketLib()
{
	int nBack = 0;
	WSADATA wsaOutData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	memset(&wsaOutData, 0x00, sizeof(WSADATA));

	nBack = WSAStartup(wVersionRequested, &wsaOutData);
	if(0 == nBack)
	{
		DoWriteLogInfo(LOG_INFO, _T("CBaseSocket::InitSocketLib(), ��ʼ��SOCKET��ɹ�"));
		return true;
	}
	else
	{
		int nErrorCode = WSAGetLastError();
		WriteLogInfo(LOG_INFO, _T("CBaseSocket::InitSocketLib(), ��ʼ��SOCKET������������:%d"), nErrorCode);
		return false;
	}
}

bool CBaseSocket::UnInitSocketLib()
{
	int nBack = 0;
	nBack = WSACleanup();

	return (nBack == 0) ? true : false;
}

bool CBaseSocket::CreateSocketInstance()
{
	if(SOCKET_ERROR == m_scSocket)
	{
		m_scSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if(INVALID_SOCKET != m_scSocket)
		{
			int  nErrorCode = 0;
			int  nRevcBufSize = BASESOCKET_REVCBUF_SIZE;
			int  nSendBufSize = BASESOCKET_SENDBUF_SIZE;
			BOOL bOpt = FALSE;

			DoWriteLogInfo(LOG_DEBUG, _T("CBaseSocket::CreateSocketInstance(), ���������׽��ֳɹ�"));
			nErrorCode = setsockopt(m_scSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&nRevcBufSize, sizeof(int));
			if(SOCKET_ERROR  == nErrorCode)
			{
				nErrorCode = WSAGetLastError();
				WriteLogInfo(LOG_INFO, _T("CBaseSocket::CreateSocketInstance(), ���ü����׽��ֽ��ջ����Сʱ����,�������:%d"), nErrorCode);
				
				closesocket(m_scSocket);
				return false;
			}

			DoWriteLogInfo(LOG_DEBUG, _T("CBaseSocket::CreateSocketInstance(), �޸ļ����׽��ֽ��ջ����С�ɹ�"));
			nErrorCode = setsockopt(m_scSocket, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendBufSize, sizeof(int));
			if(SOCKET_ERROR  == nErrorCode)
			{
				nErrorCode = WSAGetLastError();
				WriteLogInfo(LOG_INFO, _T("CBaseSocket::CreateSocketInstance(), ���ü����׽��ַ��ͻ����Сʱ����,�������:%d"), nErrorCode);
			
				closesocket(m_scSocket);
				return false;
			}

			DoWriteLogInfo(LOG_DEBUG, _T("CBaseSocket::CreateSocketInstance(), �޸ļ����׽��ַ��ͻ����С�ɹ�"));
			nErrorCode = setsockopt(m_scSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&bOpt, sizeof(BOOL));
			if(SOCKET_ERROR  == nErrorCode)
			{
				nErrorCode = WSAGetLastError();
				WriteLogInfo(LOG_INFO, _T("CBaseSocket::CreateSocketInstance(), ���ü����׽���Ϊȡ���ӳ�ģʽʱ����,�������:%d"), nErrorCode);
			
				closesocket(m_scSocket);
				return false;
			}

			DoWriteLogInfo(LOG_DEBUG, _T("CBaseSocket::CreateSocketInstance(), ����TCP_NODELAY�׽���ѡ��ɹ�"));
			return true;
		}

		int nErrorCode = WSAGetLastError();
		WriteLogInfo(LOG_INFO, _T("CBaseSocket::CreateSocketInstance(), ���������׽��ֳ����������:%d"), nErrorCode);

		return false;
	}

	return true;
}

bool CBaseSocket::BindListenSocket(int nPort)
{
	if(SOCKET_ERROR != m_scSocket)
	{
		int nErrorCode = 0;

		m_scinTcpAddr.sin_family = AF_INET;
		m_scinTcpAddr.sin_port   = htons(nPort);
		m_scinTcpAddr.sin_addr.s_addr = htonl(INADDR_ANY);

		nErrorCode = bind(m_scSocket, (SOCKADDR *)&m_scinTcpAddr, sizeof(m_scinTcpAddr));
		if(0 == nErrorCode)
		{
			WriteLogInfo(LOG_DEBUG, _T("CBaseSocket::BindListenSocket(%d), ������׽��ֳɹ�"), nPort);

			int nBackLog = CCommonFunction::GetBackLogCount();
			nErrorCode = listen(m_scSocket, nBackLog);
			if(0 == nErrorCode)
			{
				WriteLogInfo(LOG_DEBUG, _T("CBaseSocket::BindListenSocket(%d), �����׽�����Ϊ����״̬�ɹ�"), nPort);
				return true;
			}

			nErrorCode = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CBaseSocket::BindListenSocket(%d), �����׽�����Ϊ����״̬ʱ�����������:%d"), nPort, nErrorCode);

			return false;
		}

		nErrorCode = WSAGetLastError();
		WriteLogInfo(LOG_INFO, _T("CBaseSocket::BindListenSocket(%d), ������׽��ֳ����������:%d"), nPort, nErrorCode);
	}

	return false;
}

bool CBaseSocket::GetSocket2FunPointer()
{
	if(!GetAcceptExPointer())
	{
		return false;
	}

	if(!GetAcceptExSockaddrsPointer())
	{
		return false;
	}

	return true;
}

bool CBaseSocket::GetAcceptExPointer()
{
	if(SOCKET_ERROR != m_scSocket)
	{
		int   nErrorCode = 0;
		DWORD dwBytes = 0;
		GUID  GuidAcceptEx = WSAID_ACCEPTEX;

		nErrorCode = WSAIoctl(
			m_scSocket, 
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&GuidAcceptEx,   
			sizeof (GuidAcceptEx),
			&m_lpfnAcceptEx, 
			sizeof (m_lpfnAcceptEx), 
			&dwBytes, 
			NULL, 
			NULL);

		if(SOCKET_ERROR == nErrorCode)
		{
			int nErrorCode = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CBaseSocket::GetAcceptExPointer(), ��̬��ȡAcceptExָ������������:%d"), nErrorCode);

			return false;
		}
		else
		{
			DoWriteLogInfo(LOG_DEBUG, _T("CBaseSocket::GetAcceptExPointer(), ��̬��ȡAcceptExָ��ɹ�"));
			return true;
		}
	}
	return false;
}

bool CBaseSocket::GetAcceptExSockaddrsPointer()
{
	if(SOCKET_ERROR != m_scSocket)
	{
		int   nErrorCode = 0;
		DWORD dwBytes = 0;
		GUID  GuidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;

		nErrorCode = WSAIoctl(
			m_scSocket,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&GuidGetAcceptExSockaddrs,
			sizeof(GuidGetAcceptExSockaddrs),
			&m_lpfnGetAcceptExSockaddrs,
			sizeof(m_lpfnGetAcceptExSockaddrs),
			&dwBytes,
			NULL,
			NULL
			);

		if(SOCKET_ERROR == nErrorCode)
		{
			int nErrorCode = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CBaseSocket::GetAcceptExSockaddrsPointer(), ��̬��ȡGetAcceptExSockaddrsָ������������:%d"), nErrorCode);

			return false;
		}
		else
		{
			DoWriteLogInfo(LOG_DEBUG, _T("CBaseSocket::GetAcceptExSockaddrsPointer(), ��̬��ȡGetAcceptExSockaddrsָ��ɹ�"));
			return true;
		}
	}
	return false;
}