#pragma once
#include <WinSock2.h>


#define BUFFER_SIZE_TO_SOCKET					1024*4									// 定义了socket缓冲区大小
#define BUFFER_SIZE_TO_ACCEPT					((sizeof (sockaddr_in) + 16) * 2)
#define ADDRESS_LENGTH							(sizeof(sockaddr_in) + 16)
// 完成操作类型
enum CompletionType													
{
	CT_UNKOWN=0,																		// 未知类型
	CT_PREPREVC=1,																		// 数据到来
	CT_REVC = 2,																		// 接收到数据
	CT_SEND = 3,																		// 发成数据完成
	CT_ACCP = 4,																		// 接收到连接
};

// 完成键
typedef struct _CompletionKey
{
	SOCKET      keySocket;
	sockaddr_in keyClientAddr;
	SYSTEMTIME  keyLinkTime;
}CompletionKey;

// 重叠结构
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
	// 工具类，不允许实例化
	CCommonFunction(void);
public:
	static int GetCurrentKernalCount();								// 读取当前机器的CPU核个数
	static int GetBackLogCount();									// 获得当前适合的监听队列长度
};
