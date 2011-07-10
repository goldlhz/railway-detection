#pragma once

	//////////////////////////////////////////////////////////////////////////
	// 配置文件结构
	//////////////////////////////////////////////////////////////////////////
typedef struct _GlobalConfigInfo
{
	int WriteLevelMask;						// 需要归档的等级掩码,default:31
	int CreateFullLoger;					// 是否生成全量的日志文件,default:1
	int CreateAloneLoger;					// 是否创建单独等级的日志文件,default:0
}GlobalConfigInfo;

	//////////////////////////////////////////////////////////////////////////
	// 组件内函数的指针类型，所有日志组件都必顺实现下面三种类型的函数
	// 以供管理调用
	//////////////////////////////////////////////////////////////////////////
typedef	bool	(*InitPlugin_Type)(const tstring &, const tstring &);
typedef bool	(*UnInitPlugin_Type)();
typedef bool	(*WriteLogInfo_Type)(DWORD , const SYSTEMTIME &, const tstring &, const tstring &, const tstring &);
typedef bool	(*WriteLogInfo_TypeEx)(DWORD , const SYSTEMTIME &, const tstring &, const tstring &, const tstring &, const tstring &);
	//////////////////////////////////////////////////////////////////////////
	// 组件函数指针结构
	//////////////////////////////////////////////////////////////////////////
typedef struct _PluginPointer
{
	InitPlugin_Type		InitPlugin;
	UnInitPlugin_Type	UnInitPlugin;
	WriteLogInfo_Type	WriteLogInfo;
	WriteLogInfo_TypeEx	WriteLogInfoEx;
}PluginPointer;
	
	//////////////////////////////////////////////////////////////////////////
	// 日志信息结构体
	//////////////////////////////////////////////////////////////////////////
typedef struct _LogInfo
{
	DWORD	errorLevel;
	SYSTEMTIME createTime;
	tstring	strInfo;
	tstring strFile;
	tstring	strLine;
}LogInfo;

	//////////////////////////////////////////////////////////////////////////
	// 类型定义
	//////////////////////////////////////////////////////////////////////////
typedef deque <LogInfo>			DeqLogInfo;
typedef vector<PluginPointer>	VecPluginPointer;
typedef vector<HMODULE>			VecHModule;


	// 功能：	获取日志组件所在的绝对路径
	// 用法：	直接调用
	// 返回值：如果成功返回组件的绝对路径，失败返回_T("")
tstring GetLogPluginPath();

	// 功能：	枚举文件系统中已经安装好的组件
	// 用法：	传入组件所在目录的绝对路径和一个字符串数据的引用
	// 返回值：如果面功则返回true否则为false，枚举到的组件相对路径会放在字符串数组中
bool    EnumInstallPlugin(tstring strPluginPath, vector<tstring> &verPlugin);

	// 功能：	将组件所在的目录字符串与组件文件名组合在一起形成绝对路径
	// 用法：	
	// 返回值：
void    BuildPluginFileName(tstring strPluginPath, vector<tstring> &verPlugin);

	// 功能：	加载组件
	// 用法：	
	// 返回值：
int		LoadPlugin(const tstring & strPluginPath, const tstring & moduleName, const vector<tstring> & vecPlugin);

	// 功能：	循环写入日志
	// 用法：	
	// 返回值：
DWORD   LoopWriteLog(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & strInfo, const tstring & strFile, const tstring & strLine);
DWORD   LoopWriteLogEx(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & strInfo, const tstring & strFile, const tstring & strLine, const tstring & fileName);	
	// 功能：	卸载资源
	// 用法：	
	// 返回值：
void	ReleasePlugin();

	// 功能：	整数到字符串转换
	// 用法：	
	// 返回值：
tstring IntToTString(int data);
	
	// 功能：	创建用于同步日志队列的事件
	// 用法：	
	// 返回值：
bool	CreateSynLogEvent();

	// 功能：	关闭用于同步日志队列的事件
	// 用法：	
	// 返回值：
bool	CloseSynLogEvent();

	// 功能：	将日志队列里的日志刷新到日志中
	// 用法：	
	// 返回值：
bool    FlashLogQueue();

	// 功能：	创建写日志的线程
	// 用法：	
	// 返回值：
bool	CreateWriteThread();

	// 功能：	创建写日志的线程
	// 用法：	
	// 返回值：
bool	CloseWriteThread();

	// 功能：	线程函数
	// 用法：	
	// 返回值：
unsigned __stdcall WriteLogThreadFunc(void * pArguments);

	// 功能：	将日志写入到队列中
	// 用法：	
	// 返回值：
bool	WriteLogToDeque(DWORD errorLevel, const tstring & strInfo, const tstring & strFile, const tstring & strLine);

	// 功能：	读取配置文件信息
	// 用法：	
	// 返回值：
void 	ReadLogerIniFile(tstring strPluginPath);

	// 功能：	检测指定等级的日志是否需要归档
	// 用法：	
	// 返回值：
bool 	IsErrorLevelLog(DWORD errorLevel);

	// 功能：	构建以时间为条件的字符串
	// 用法：	
	// 返回值：
tstring BuildTimeString(int nTimes);