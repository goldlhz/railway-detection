#pragma once
#include <WinSock2.h>
#include <MSWSock.h>

#define BASESOCKET_SENDBUF_SIZE			65535
#define BASESOCKET_REVCBUF_SIZE			65535

class CBaseSocket
{
public:
	CBaseSocket(void);
	virtual ~CBaseSocket(void);

	BOOL AcceptEx(
		SOCKET sListenSocket,
		SOCKET sAcceptSocket,
		PVOID lpOutputBuffer,
		DWORD dwReceiveDataLength,
		DWORD dwLocalAddressLength,
		DWORD dwRemoteAddressLength,
		LPDWORD lpdwBytesReceived,
		LPOVERLAPPED lpOverlapped
		);

	bool GetAcceptExSockaddrs(
		PVOID lpOutputBuffer,
		DWORD dwReceiveDataLength,
		DWORD dwLocalAddressLength,
		DWORD dwRemoteAddressLength,
		LPSOCKADDR* LocalSockaddr,
		LPINT LocalSockaddrLength,
		LPSOCKADDR* RemoteSockaddr,
		LPINT RemoteSockaddrLength
		);

	bool CreateListenSocket(int nPort);								// 创建一个套接字
	bool CloseSocket();												// 关闭套接字
	SOCKET GetSocket(){return m_scSocket;};							// 获取当前套接字
	void GetServerAddr(struct sockaddr_in * pscinTcpAddr);			// 获取服务套接字的地址信息

	static bool InitSocketLib();									// 初始化套接字库
	static bool UnInitSocketLib();									// 卸载套接字库

private:
	bool CreateSocketInstance();									// 创建重叠模式的套接字实例
	bool BindListenSocket(int nPort);								// 邦定操作
	bool GetExFunPointer();											// 获取扩展指针
	bool GetAcceptExPointer();										// 获取WinSocket2里的AcceptEx
	bool GetAcceptExSockaddrsPointer();								// 获取WinSocket2里的AcceptExSockaddrs

	SOCKET m_scSocket;												// 当前套接字
	struct sockaddr_in m_scinTcpAddr;								// 服务端地址信息
	LPFN_ACCEPTEX m_lpfnAcceptEx;									// AcceptEx函数指针
	LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockaddrs;			// GetAcceptExSocketAddrs函数指针
};
