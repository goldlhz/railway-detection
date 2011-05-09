#pragma once
#include <string>
using namespace std;

template<typename Data_Body_Type>
struct Pack_GPSTransfers
{
	unsigned int   nBeginIdentify;			// 开始标识
	unsigned int   nMsgNumber;				// 消息号		消息标识
	unsigned int   nBodyLength;				// 数据长度		后续数据区长度,不含结尾标识
	Data_Body_Type gGPSBodyPack;			// 数据区		依赖消息的不同数据内容
	unsigned int   nEndIdentify;			// 结束标识
};

struct GPS_Body_UpLoad_Type
{
	unsigned int nTerminalIdentifyLength;	// 标识长度		终端唯一标识的长度
	string       strTerminalNumber;			// 终端标识		终端标识即手机号码
	signed   int nLongitude;				// 经度			单位千万分之一度.  该值=经度*10000000正数代表东经,负数代表西经
	signed   int nLatitude;					// 纬度			单位千万分之一度.  该值=纬度*10000000正数代表北纬,负数代表南纬
	unsigned int nSpeed;					// 速度			单位:km/h(公里/小时) 整数
	unsigned int nDirection;				// 方向			该值=以正北为0度,顺时针偏移的度数/2
	string       strTime;					// 时间			
											//  2字节年
											//	1字节月
											//	1字节日
											//	1字节小时
											//	1字节分
											//	1字节秒

											//	D9 07 0B 1B 0B 20 30
											//	2009年11月27号11点32分48秒
	unsigned int nState;					// 状态			0代表普通.暂时无其它,可沟通.
	unsigned int nMileage;					// 里程			单位米,整数.   无传0
	unsigned int nOilMass;					// 油量			单位升,整数.   无传0
	unsigned int nIsLocate;					// 是否定位		0代表GPS模块定位 其它代表不定位
};

struct GPS_Body_DownLoad_Type
{
	unsigned int nTerminalIdentifyLength;	// 标识长度		终端唯一标识的长度
	string       strTerminalNumber;			// 终端标识		终端标识即手机号码
	string		 strTime;					// 时间			
											//  2字节年
											//	1字节月
											//	1字节日
											//	1字节小时
											//	1字节分
											//	1字节秒
};

typedef Pack_GPSTransfers<GPS_Body_UpLoad_Type>   GPS_UpLoad_Pack;
typedef Pack_GPSTransfers<GPS_Body_DownLoad_Type> GPS_DownLoad_Pack;