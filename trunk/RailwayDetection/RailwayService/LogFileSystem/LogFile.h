#pragma once
#include <string>
using namespace std;

// 给出一个绝对路径，创建这个路径
tstring CreateLogDirectory(tstring dirPath, const SYSTEMTIME & createTime);

// 给出一个文件名，创建这个文件
bool CreateLogFile(const tstring & fileName, FILE *& file);

// 给出一个以NULL结束的字符串，将这个字符串写入到文件
DWORD WriteLogFile(const tstring & logInfomation, FILE * file);

// 关闭日志文件
void CloseAllFile();

// 生成月份字符串
tstring BuildMonthString(const SYSTEMTIME & createTime);

// 生成日期字符串
tstring BuildDayString(const SYSTEMTIME & createTime);

// 生成小时字符串
tstring BuildHourString(const SYSTEMTIME & createTime);

// 生成时间字符串
tstring BuildTimeString(const SYSTEMTIME & createTime);

// 生成错误字符串
tstring GetErrorString(DWORD err);

// 生成日志记录
tstring BuildLogLine(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & infoStr, const tstring & file, const tstring & line);

// 读取配置文件
void ReadLogerIniFile(tstring strPluginPath);

// 写全量的日志文件
bool WriteFullLog(DWORD errorLevel, const SYSTEMTIME & createTime, tstring infoStr, const tstring & file, const tstring & line, const tstring & filepath);
bool WriteFullLogEx(DWORD errorLevel, const SYSTEMTIME & createTime, tstring infoStr, const tstring & file, const tstring & line, const tstring & filepath, const tstring & fileName);
// 写独立的日志文件
bool WriteAloneLog(DWORD errorLevel, const SYSTEMTIME & createTime, tstring infoStr, const tstring & file, const tstring & line, const tstring & filepath);

// 跟据文件名在数组中找文件指针
FILE * FindHandleByFileName(const tstring & fileName);