/**************************************************************************
 *	名称：睿明日志系统
 *	功能：调用此组件可记录程序日志信息
 *	使用方法：
 *	作者：李科
 *	日期：2010.10.25
 **************************************************************************/
#include <string>
using namespace std;

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
	// 功能：	初始化组件及其所需要的资源
	// 用法：	直接调用函数等待返回结果
	// 参数：  filePath为组件所在的目录的路径
	// 返回值：如果初始化成功，则返回true，否则返回false
	//
	LOGMOUDLE bool InitPlugin(const tstring & filePath, const tstring & moduleName);

	// 功能：	卸载组件及其使用的资源
	// 用法：	直接调用函数等待返回结果
	// 返回值：如果卸载成功，则返回true，否则返回false
	//
	LOGMOUDLE bool UnInitPlugin();

	// 功能：  通过指定的组件，写日志
	// 用法：	直接调用函数，并传递日志信息
	// 返回值：如果写入日志成功，则返回零，否则返回false
	//
	LOGMOUDLE bool WriteLogInfo(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & infoStr, const tstring & file, const tstring & line);
	LOGMOUDLE bool WriteLogInfoEx(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & infoStr, const tstring & file, const tstring & line, const tstring & fileName);

};