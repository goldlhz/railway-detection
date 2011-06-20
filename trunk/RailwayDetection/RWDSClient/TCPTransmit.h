#pragma once
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
/*
typedef struct _TCP_KEEPALIVE 
{
	u_long onoff;
	u_long keepalivetime;
	u_long keepaliveinterval;
}TCP_KeepAlive;*/
#define SIO_KEEPALIVE_VALS _WSAIOW(IOC_VENDOR,4)

class  CSocketServer
{
public:
	CSocketServer(void);
	virtual ~CSocketServer(void);
public:
	//virtual int Write(char *lpSendBuf, int iBufLen, char *lpDestIP=INADDR_ANY, int iDestPort=0) = 0;
	//virtual int Read(char *lpReadBuf, int iBufLen, char *lpFromIP=INADDR_ANY, int iFromPort=0) = 0;
	//virtual int Open(int iPort, int iListen=10, char *lpIP=INADDR_ANY) = 0;
public:
	virtual void Close();
	virtual int WaitForData(DWORD dwWaitTimeOut);
	SOCKET GetSocket();
protected:
	bool m_bOpen;
	int m_iPort;
	SOCKET m_sSocket;
};

class CTCPClient : public CSocketServer
{
public:
	CTCPClient(void);
	virtual ~CTCPClient(void);
public:
	int Open(int iPort, char *lpIP);
	int Read(char *lpReadBuf, int iBufLen);
	int Write(char *lpSendBuf, int iBufLen);
};

class CTCPServer : public CTCPClient
{
public:
	CTCPServer(void);
	~CTCPServer(void);
public:
	int SetSocketServer(SOCKET server);
	int SetClientIPPort(char* IP, int Port);
	void GetClientIPPort(char* IP, int *Port);
	void Close();
private:
	char m_lpClientIP[16];
	int m_iClientPort;
};
class CTCPService :	public CSocketServer
{
public:
	CTCPService(void);
	~CTCPService(void);
public:
	int Open(int iPort, int iListen);
	int AcceptSocket(CTCPServer *cAcceptTCP, DWORD dwTimeout);
private:
	int m_iListen;
};