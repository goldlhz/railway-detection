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
	CloseLocalSocket();
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

bool CBaseSocket::CreateLocalSocket(int nPort)
{
	if(CreateSocketInstance())
	{
		if(!BindListenSocket(nPort))
		{
			CloseLocalSocket();
			return false;
		}

		if(!GetSocket2ExFun())
		{
			CloseLocalSocket();
			return false;
		}
		
		DoWriteLogInfo(LOG_INFO, _T("CBaseSocket::CreateListenSocket(), 成功完成服务端套接字的创建"));
		return true;
	}

	return false;
}

bool CBaseSocket::CloseLocalSocket()
{
	if(SOCKET_ERROR != m_scSocket)
	{
		int nErrorCode = 0;
		shutdown(m_scSocket,  SD_BOTH);
		nErrorCode = closesocket(m_scSocket);
		m_scSocket = SOCKET_ERROR;

		return (nErrorCode == 0) ? true : false;
	}

	return true;
}

void CBaseSocket::GetServerAddr(sockaddr_in * pscinTcpAddr)
{
	if(pscinTcpAddr)
	{
		memcpy(pscinTcpAddr, &m_tcpAddr, sizeof(sockaddr_in));
	}
}

bool CBaseSocket::InitSocketLib()
{
	int nErrorCode = 0;
	WSADATA wsaOutData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	ZeroMemory(&wsaOutData, sizeof(WSADATA));

	nErrorCode = WSAStartup(wVersionRequested, &wsaOutData);
	if(0 == nErrorCode)
	{
		DoWriteLogInfo(LOG_INFO, _T("CBaseSocket::InitSocketLib(), 初始化SOCKET库成功"));
		return true;
	}
	else
	{
		nErrorCode = WSAGetLastError();
		WriteLogInfo(LOG_INFO, _T("CBaseSocket::InitSocketLib(), 初始化SOCKET库出错，错误代码:%d"), nErrorCode);
		return false;
	}
}

bool CBaseSocket::UnInitSocketLib()
{
	int nErrorCode = 0;
	nErrorCode = WSACleanup();

	return (nErrorCode == 0) ? true : false;
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

			DoWriteLogInfo(LOG_DEBUG, _T("CBaseSocket::CreateSocketInstance(), 创建基本套接字成功"));
			nErrorCode = setsockopt(m_scSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&nRevcBufSize, sizeof(int));
			if(SOCKET_ERROR  == nErrorCode)
			{
				nErrorCode = WSAGetLastError();
				WriteLogInfo(LOG_INFO, _T("CBaseSocket::CreateSocketInstance(), 设置监听套接字接收缓冲大小时出错,错误代码:%d"), nErrorCode);
				
				closesocket(m_scSocket);
				return false;
			}

			DoWriteLogInfo(LOG_DEBUG, _T("CBaseSocket::CreateSocketInstance(), 修改监听套接字接收缓冲大小成功"));
			nErrorCode = setsockopt(m_scSocket, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendBufSize, sizeof(int));
			if(SOCKET_ERROR  == nErrorCode)
			{
				nErrorCode = WSAGetLastError();
				WriteLogInfo(LOG_INFO, _T("CBaseSocket::CreateSocketInstance(), 设置监听套接字发送缓冲大小时出错,错误代码:%d"), nErrorCode);
			
				closesocket(m_scSocket);
				return false;
			}

			DoWriteLogInfo(LOG_DEBUG, _T("CBaseSocket::CreateSocketInstance(), 修改监听套接字发送缓冲大小成功"));
			nErrorCode = setsockopt(m_scSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&bOpt, sizeof(BOOL));
			if(SOCKET_ERROR  == nErrorCode)
			{
				nErrorCode = WSAGetLastError();
				WriteLogInfo(LOG_INFO, _T("CBaseSocket::CreateSocketInstance(), 设置监听套接字为取消延迟模式时出错,错误代码:%d"), nErrorCode);
			
				closesocket(m_scSocket);
				return false;
			}

			DoWriteLogInfo(LOG_DEBUG, _T("CBaseSocket::CreateSocketInstance(), 设置TCP_NODELAY套接字选项成功"));
			return true;
		}

		int nErrorCode = WSAGetLastError();
		WriteLogInfo(LOG_INFO, _T("CBaseSocket::CreateSocketInstance(), 创建监听套接字出错，错误代码:%d"), nErrorCode);

		return false;
	}

	return true;
}

bool CBaseSocket::BindListenSocket(int nPort)
{
	if(SOCKET_ERROR != m_scSocket)
	{
		int nErrorCode = 0;

		m_tcpAddr.sin_family = AF_INET;
		m_tcpAddr.sin_port   = htons(nPort);
		m_tcpAddr.sin_addr.s_addr = htonl(INADDR_ANY);

		nErrorCode = bind(m_scSocket, (SOCKADDR *)&m_tcpAddr, sizeof(m_tcpAddr));
		if(0 == nErrorCode)
		{
			WriteLogInfo(LOG_DEBUG, _T("CBaseSocket::BindListenSocket(%d), 邦定监听套接字成功"), nPort);

			int nBackLog = CCommonFunction::GetBackLogCount();
			nErrorCode = listen(m_scSocket, nBackLog);
			if(0 == nErrorCode)
			{
				WriteLogInfo(LOG_DEBUG, _T("CBaseSocket::BindListenSocket(%d), 监听套接字置为监听状态成功"), nPort);
				return true;
			}

			nErrorCode = WSAGetLastError();
			WriteLogInfo(LOG_INFO, _T("CBaseSocket::BindListenSocket(%d), 监听套接字置为监听状态时出错，错误代码:%d"), nPort, nErrorCode);

			return false;
		}

		nErrorCode = WSAGetLastError();
		WriteLogInfo(LOG_INFO, _T("CBaseSocket::BindListenSocket(%d), 邦定监听套接字出错，错误代码:%d"), nPort, nErrorCode);
	}

	return false;
}

bool CBaseSocket::GetSocket2ExFun()
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
			WriteLogInfo(LOG_INFO, _T("CBaseSocket::GetAcceptExPointer(), 动态获取AcceptEx指针出错，错误代码:%d"), nErrorCode);

			return false;
		}
		else
		{
			DoWriteLogInfo(LOG_DEBUG, _T("CBaseSocket::GetAcceptExPointer(), 动态获取AcceptEx指针成功"));
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
			WriteLogInfo(LOG_INFO, _T("CBaseSocket::GetAcceptExSockaddrsPointer(), 动态获取GetAcceptExSockaddrs指针出错，错误代码:%d"), nErrorCode);

			return false;
		}
		else
		{
			DoWriteLogInfo(LOG_DEBUG, _T("CBaseSocket::GetAcceptExSockaddrsPointer(), 动态获取GetAcceptExSockaddrs指针成功"));
			return true;
		}
	}
	return false;
}