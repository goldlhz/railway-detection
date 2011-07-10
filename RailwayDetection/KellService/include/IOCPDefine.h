#pragma once
#include <WinSock2.h>

#define LISTEN_SOCKET_BUFFER_SIZE				65536
#define BUFFER_SIZE_TO_CLIENT					4096									// 定义了socket缓冲区大小
#define BUFFER_SIZE_TO_ACCEPT					((sizeof (sockaddr_in) + 16) * 2)
#define ADDRESS_LENGTH							(sizeof(sockaddr_in) + 16)
// 完成操作类型
enum OptType													
{
	CT_REVC = 0,																		// 接收到数据
	CT_SEND = 1,																		// 发成数据完成
	CT_ACCP = 2,																		// 接收到连接
};

// 重叠结构
typedef struct OVERLAPPEDEX
{
	OVERLAPPED	wsaOverlapped;
	WSABUF		wsaWSABUF;
	CHAR		wsaBuffer[BUFFER_SIZE_TO_CLIENT];
	CHAR		wsaAcceptBuffer[BUFFER_SIZE_TO_ACCEPT];
	OptType		wsaOptType;
	SOCKET      wsaClientSocket;
}*LPOVERLAPPEDEX;

// 完成键
typedef struct CompKey
{
	SOCKET      keyOptSocket;
	sockaddr_in keyClientAddr;
	SYSTEMTIME  keyLinkTime;
}*LPCompKey;

typedef struct OverKeyPire
{
	OVERLAPPEDEX pireOverLappedex;
	CompKey      pireCompletionKey;
}*LPOverKeyPire;
