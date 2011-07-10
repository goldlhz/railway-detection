/**************************************************************************
 *	名称：睿明日志系统
 *	功能：调用此组件可记录程序日志信息
 *	使用方法：按顺序调用函数，先初始化，再使用，不使用就可以将它释放
 *              应当在程序一开始就将插件管理器进行初始化，程序关闭之前
 *				再将其释放
 *	作者：李科
 *	日期：2010.10.25
 **************************************************************************/
#include <string>
using namespace std;

#ifdef UNICODE
	#define WIDEN2(x) L ## x
	#define WIDEN(x) WIDEN2(x)
	#define __TFILE__ WIDEN(__FILE__)
#else
	#define WIDEN2(x) x
	#define WIDEN(x) WIDEN2(x)
	#define __TFILE__ WIDEN(__FILE__)
#endif

// 定义导入导出宏
#ifndef		REMINELOGPLUGIN
#define		LOGMOUDLE	__declspec(dllimport)
#else
#define		LOGMOUDLE	__declspec(dllexport)
#endif

// 定义错误等级
#define LOG_EMERG			1						// 紧急		系统不可用
#define LOG_CRIT			2						// 严重		危急信息，如硬件出错
#define LOG_ERR				3						// 错误		错误信息
#define LOG_INFO			4						// 信息		通知信息
#define LOG_DEBUG			5						// 调试		调试信息，包括正常的使用信息

extern "C"
{
	// 功能：  初始化日志组件
	// 用法：	从参数中传递需要使用的日志组件的掩码 
	// 返回值：如果成功返回为零，如果全部出错返回-1，如果部份出错返回1
	//
	LOGMOUDLE DWORD __stdcall InitLogPluginManagerA(char* moduleName);
	LOGMOUDLE DWORD __stdcall InitLogPluginManagerW(wchar_t* moduleName);
	// 功能：  通过指定的组件，写日志
	// 用法：	直接调用函数，并传递日志信息
	// 返回值：成功，则返回零，返回-1表示全部出错，返回1表示部分出错
	//			低等级日志是异步方式写入，因此总是返回成功的，高等级日志是同步写入的，有可能返回错误
	LOGMOUDLE DWORD __stdcall WriteLogInfoBaseA(DWORD errorLevel, const char* file, int line, const char* fmt, ...);
	LOGMOUDLE DWORD __stdcall WriteLogInfoBaseW(DWORD errorLevel, const wchar_t* file, int line, const wchar_t* fmt, ...);
	// 功能：  通过指定的组件，写日志
	// 用法：	直接调用函数，并传递日志信息
	// 返回值：成功，则返回零，返回-1表示全部出错，返回1表示部分出错
	//			低等级日志是异步方式写入，因此总是返回成功的，高等级日志是同步写入的，有可能返回错误
	LOGMOUDLE DWORD __stdcall DoWriteLogInfoBaseA(DWORD errorLevel, const char* infoStr, const char* file, int line);
	LOGMOUDLE DWORD __stdcall DoWriteLogInfoBaseW(DWORD errorLevel, const wchar_t* infoStr, const wchar_t* file, int line);
	// 功能：  通过指定的组件，写日志
	// 用法：  直接调用函数，并传递日志信息，参数fileName表示单独生成的文件的名称参数，如果fileName为空，则函数等同于直接调用WriteLogInfoBase
	// 返回值：成功，则返回零，返回-1表示全部出错，返回1表示部分出错
	//			低等级日志是异步方式写入，因此总是返回成功的，高等级日志是同步写入的，有可能返回错误
	LOGMOUDLE DWORD __stdcall WriteLogInfoBaseExA(DWORD errorLevel, const char* file, int line, const char * fileName, const char* fmt, ...);
	LOGMOUDLE DWORD __stdcall WriteLogInfoBaseExW(DWORD errorLevel, const wchar_t* file, int line, const wchar_t * fileName, const wchar_t* fmt, ...);
	// 功能：  通过指定的组件，写日志
	// 用法：	直接调用函数，并传递日志信息，参数fileName表示单独生成的文件的名称参数，如果fileName为空，则函数等同于直接调用DoWriteLogInfoBase
	// 返回值：成功，则返回零，返回-1表示全部出错，返回1表示部分出错
	//			低等级日志是异步方式写入，因此总是返回成功的，高等级日志是同步写入的，有可能返回错误
	//			
	LOGMOUDLE DWORD __stdcall DoWriteLogInfoBaseExA(DWORD errorLevel, const char* infoStr, const char* file, int line, const char * fileName);
	LOGMOUDLE DWORD __stdcall DoWriteLogInfoBaseExW(DWORD errorLevel, const wchar_t* infoStr, const wchar_t* file, int line, const wchar_t * fileName);
	// 功能：  卸载日志组件
	// 用法：	直接调用
	// 返回值：总是返回0
	//
	LOGMOUDLE DWORD __stdcall UnInitLogPluginManager();
};

#ifdef UNICODE
	#define InitLogPluginManager(moduleName)		InitLogPluginManagerW(moduleName)

	#define WriteLogInfo(errorLevel, fmt, ...)		WriteLogInfoBaseW(errorLevel, __TFILE__, __LINE__, fmt, __VA_ARGS__)
	#define DoWriteLogInfo(errorLevel, infoStr)		DoWriteLogInfoBaseW(errorLevel, infoStr, __TFILE__, __LINE__)
	
	#define WriteLogInfoEx(errorLevel, fileName, fmt, ...)		WriteLogInfoBaseExW(errorLevel, __TFILE__, __LINE__, fileName, fmt, __VA_ARGS__)
	#define DoWriteLogInfoEx(errorLevel, infoStr, fileName)		DoWriteLogInfoBaseExW(errorLevel, infoStr, __TFILE__, __LINE__, fileName)
#else
	#define InitLogPluginManager(moduleName)		InitLogPluginManagerA(moduleName)

	#define WriteLogInfo(errorLevel, fmt, ...)		WriteLogInfoBaseA(errorLevel, __TFILE__, __LINE__, fmt, __VA_ARGS__)
	#define DoWriteLogInfo(errorLevel, infoStr)		DoWriteLogInfoBaseA(errorLevel, infoStr, __TFILE__, __LINE__)
	
	#define WriteLogInfoEx(errorLevel, fileName, fmt, ...)		WriteLogInfoBaseExA(errorLevel, __TFILE__, __LINE__, fileName, fmt, __VA_ARGS__)
	#define DoWriteLogInfoEx(errorLevel, infoStr, fileName)		DoWriteLogInfoBaseExA(errorLevel, infoStr, __TFILE__, __LINE__, fileName)
#endif