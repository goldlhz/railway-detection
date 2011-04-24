#pragma once
#include <WinSock2.h>


#define BUFFER_SIZE_TO_SOCKET					1024*4									// ������socket��������С
#define BUFFER_SIZE_TO_ACCEPT					((sizeof (sockaddr_in) + 16) * 2)
#define ADDRESS_LENGTH							(sizeof(sockaddr_in) + 16)
// ��ɲ�������
enum CompletionType													
{
	CT_UNKOWN=0,																		// δ֪����
	CT_PREPREVC=1,																		// ���ݵ���
	CT_REVC = 2,																		// ���յ�����
	CT_SEND = 3,																		// �����������
	CT_ACCP = 4,																		// ���յ�����
};

// ��ɼ�
typedef struct _CompletionKey
{
	SOCKET      keySocket;
	sockaddr_in keyClientAddr;
	SYSTEMTIME  keyLinkTime;
}CompletionKey;

// �ص��ṹ
typedef struct _WsaOverLappedEX
{
	WSAOVERLAPPED  wsaOverlapped;
	CompletionType wsaOptType;
	SOCKET         wsaClientSocket;
	WSABUF		   wsaWSABuf;
	CHAR		   wsaBuffer[BUFFER_SIZE_TO_SOCKET];
	CHAR		   wsaAcceptBuffer[BUFFER_SIZE_TO_ACCEPT];
}WsaOverLappedEX;

typedef struct _KeyOverPire
{
	WsaOverLappedEX  pireOverLappedex;
	CompletionKey    pireCompletionKey;
}KeyOverPire;


class CCommonFunction
{
private:
	// �����࣬������ʵ����
	CCommonFunction(void);
public:
	static int GetCurrentKernalCount();								// ��ȡ��ǰ������CPU�˸���
	static int GetBackLogCount();									// ��õ�ǰ�ʺϵļ������г���
};
