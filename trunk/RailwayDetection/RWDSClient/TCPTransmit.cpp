#include "StdAfx.h"
#include "TCPTransmit.h"

CSocketServer::CSocketServer(void)
	:m_bOpen(false), m_iPort(0), m_sSocket(NULL)
{
}

CSocketServer::~CSocketServer(void)
{
}

void CSocketServer::Close()
{
	if(!m_bOpen)
		return;
	closesocket(m_sSocket);
	m_sSocket = NULL;
	WSACleanup();
	m_bOpen = false;
}
int CSocketServer::WaitForData(DWORD dwWaitTimeOut)
{
	struct timeval tm;
	tm.tv_sec = dwWaitTimeOut / 1000;
	tm.tv_usec = dwWaitTimeOut % 1000;
	struct fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m_sSocket, &fds);
	int ret = select(0, &fds, NULL, NULL, &tm);
	if(ret > 0)
	{
		if(!FD_ISSET(m_sSocket, &fds))
			ret = -2;
	}
	return ret;
}
SOCKET CSocketServer::GetSocket()
{
	return m_sSocket;
}

CTCPClient::CTCPClient(void)
{
}

CTCPClient::~CTCPClient(void)
{
}
int CTCPClient::Open(int iPort, char *lpIP)
{
	int ret = -1;
	if(m_bOpen)
		return 1;
	m_iPort = iPort;
	WORD wVersion = MAKEWORD(2, 0);
	WSADATA WSAData;
	if(WSAStartup(wVersion, &WSAData) != 0)
	{
		return ret;
	}
	m_sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_sSocket == INVALID_SOCKET)
	{
		WSACleanup();
		return ret;
	}
	
	m_bOpen = true;
	struct sockaddr_in addr;
	addr.sin_port = htons(m_iPort);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(lpIP);
	int OutTime = 60;
	setsockopt(m_sSocket,SOL_SOCKET,SO_RCVTIMEO,reinterpret_cast<char*>(&OutTime),sizeof(OutTime));
	if(connect(m_sSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr))  == SOCKET_ERROR)
	{
		Close();
		return SOCKET_ERROR;
	}
	return 0;
}
int CTCPClient::Read(char *lpReadBuf, int iBufLen)
{
	if(!m_bOpen)
		return -1;
	if(iBufLen < 1)
		return 0;
	int rlen = 0;
	rlen = WaitForData(500);
	if(rlen > 0)
	{
		rlen = recv(m_sSocket, lpReadBuf, iBufLen, 0);
		if(rlen <= 0)
		{
			int ty = GetLastError();
			rlen = -ty;
			Close();
			
		}
	}
	else if(rlen == SOCKET_ERROR)
	{
		int ty = GetLastError();
		rlen = -ty;
		Close();
	}
	else if(rlen == 0)
	{
		
		rlen = recv(m_sSocket, lpReadBuf, iBufLen, 0);
		int xx = GetLastError();
		if(rlen < 0)
		{
			rlen = 0;
			if (xx != WSAETIMEDOUT)
			{
				Close();
				rlen = -xx;
			}
		}
	}
	return rlen;
}

int CTCPClient::Write(char *lpSendBuf, int iBufLen)
{
	if(!m_bOpen)
		return -1;
	int written = 0;
	while(written < iBufLen)
	{
		int slen = send(m_sSocket, lpSendBuf + written, iBufLen - written, 0);
		if(slen == SOCKET_ERROR)
		{
			int ty = GetLastError();
			written = -ty;
			Close();
			break;
		}
		written += slen;
		Sleep(5);
	}
	return written;
}

CTCPServer::CTCPServer(void)
	:m_iClientPort(0)
{
	memset(m_lpClientIP, '\0', sizeof(m_lpClientIP));
}

CTCPServer::~CTCPServer(void)
{
	try
	{
		Close();
	}
	catch(...)
	{

	}
}
int CTCPServer::SetSocketServer(SOCKET server)
{
	m_sSocket = server;
	m_bOpen = true;
	return 0;
}
int CTCPServer::SetClientIPPort(char *IP, int Port)
{
	strcpy_s(m_lpClientIP, sizeof(m_lpClientIP), IP);
	m_iClientPort = Port;
	return 0;
}
void CTCPServer::GetClientIPPort(char* IP, int *Port)
{
	strcpy_s(IP, 16, m_lpClientIP);
	*Port = m_iClientPort;
}

void CTCPServer::Close()
{
	if(!m_bOpen)
		return;
	closesocket(m_sSocket);
	m_sSocket = NULL;
	m_bOpen = false;
}

CTCPService::CTCPService(void)
:m_iListen(5)
{
}

CTCPService::~CTCPService(void)
{
}
int CTCPService::Open(int iPort, int iListen)
{
	if(m_bOpen)
		return 1;
	m_iPort = iPort;
	WORD wVersion = MAKEWORD(2, 0);
	WSADATA WSAData;
	if(WSAStartup(wVersion, &WSAData) != 0)
	{
		return -1;
	}
	m_sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_sSocket == INVALID_SOCKET)
	{
		WSACleanup();
		return -1;
	}
	LONG nZero = 60*1024;
	setsockopt(m_sSocket,SOL_SOCKET,SO_SNDBUF,(const char *)(&nZero),sizeof(LONG));
	m_bOpen = true;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(iPort);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int OutTime = 50;
	setsockopt(m_sSocket,SOL_SOCKET,SO_RCVTIMEO,reinterpret_cast<char*>(&OutTime),sizeof(OutTime));
	if(bind(m_sSocket, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
	{
		Close();
		return -1;
	}
	if(listen(m_sSocket, iListen) == SOCKET_ERROR)
	{
		Close();
		return -1;
	}
	return 0;
}
int CTCPService::AcceptSocket(CTCPServer *cAcceptTCP, DWORD dwTimeout)
{
	struct timeval timeout;
	timeout.tv_sec = dwTimeout / 1000;
	timeout.tv_usec = dwTimeout % 1000;
	fd_set fd;
	FD_ZERO(&fd);
	FD_SET(m_sSocket, &fd);
	int ret = select(0, &fd, NULL, NULL, &timeout);
	if(ret  == SOCKET_ERROR)
	{
		DWORD err = GetLastError();
		return -1;
	}
	else if(ret > 0)
	{
		struct sockaddr_in cAddr;
		int cLen = sizeof(struct sockaddr_in);
		SOCKET asocket = accept(m_sSocket, reinterpret_cast<struct sockaddr*>(&cAddr), &cLen);
		//SOCKET asocket = accept(m_sSocket, NULL, NULL);
		if(asocket == INVALID_SOCKET)
		{
			//DWORD rtt = GetLastError();
			return -1;
		}
		//设置服务对象
		cAcceptTCP->SetSocketServer(asocket);
		cAcceptTCP->SetClientIPPort(inet_ntoa(cAddr.sin_addr), ntohs(cAddr.sin_port));
	}
	return ret;
}