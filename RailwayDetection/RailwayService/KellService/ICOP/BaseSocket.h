#pragma once
#include <WinSock2.h>
#include <MSWSock.h>

#define BASESOCKET_SENDBUF_SIZE			65536
#define BASESOCKET_REVCBUF_SIZE			65536

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

	bool CreateLocalSocket(int nPort);								// ����һ���׽���
	bool CloseLocalSocket();										// �ر��׽���
	SOCKET GetSocket(){return m_scSocket;};							// ��ȡ��ǰ�׽���
	void GetServerAddr(sockaddr_in * pscinTcpAddr);					// ��ȡ�����׽��ֵĵ�ַ��Ϣ

	static bool InitSocketLib();									// ��ʼ���׽��ֿ�
	static bool UnInitSocketLib();									// ж���׽��ֿ�

private:
	bool CreateSocketInstance();									// �����ص�ģʽ���׽���ʵ��
	bool BindListenSocket(int nPort);								// �����
	bool GetSocket2ExFun();											// ��ȡ��չָ��
	bool GetAcceptExPointer();										// ��ȡWinSocket2���AcceptEx
	bool GetAcceptExSockaddrsPointer();								// ��ȡWinSocket2���AcceptExSockaddrs

	SOCKET		m_scSocket;											// ��ǰ�׽���
	sockaddr_in m_tcpAddr;											// ����˵�ַ��Ϣ
	LPFN_ACCEPTEX m_lpfnAcceptEx;									// AcceptEx����ָ��
	LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockaddrs;			// GetAcceptExSocketAddrs����ָ��
};
