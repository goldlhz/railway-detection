#pragma once
#include <string>
using namespace std;

// ����һ������·�����������·��
tstring CreateLogDirectory(tstring dirPath, const SYSTEMTIME & createTime);

// ����һ���ļ�������������ļ�
bool CreateLogFile(const tstring & fileName, FILE *& file);

// ����һ����NULL�������ַ�����������ַ���д�뵽�ļ�
DWORD WriteLogFile(const tstring & logInfomation, FILE * file);

// �ر���־�ļ�
void CloseAllFile();

// �����·��ַ���
tstring BuildMonthString(const SYSTEMTIME & createTime);

// ���������ַ���
tstring BuildDayString(const SYSTEMTIME & createTime);

// ����Сʱ�ַ���
tstring BuildHourString(const SYSTEMTIME & createTime);

// ����ʱ���ַ���
tstring BuildTimeString(const SYSTEMTIME & createTime);

// ���ɴ����ַ���
tstring GetErrorString(DWORD err);

// ������־��¼
tstring BuildLogLine(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & infoStr, const tstring & file, const tstring & line);

// ��ȡ�����ļ�
void ReadLogerIniFile(tstring strPluginPath);

// дȫ������־�ļ�
bool WriteFullLog(DWORD errorLevel, const SYSTEMTIME & createTime, tstring infoStr, const tstring & file, const tstring & line, const tstring & filepath);
bool WriteFullLogEx(DWORD errorLevel, const SYSTEMTIME & createTime, tstring infoStr, const tstring & file, const tstring & line, const tstring & filepath, const tstring & fileName);
// д��������־�ļ�
bool WriteAloneLog(DWORD errorLevel, const SYSTEMTIME & createTime, tstring infoStr, const tstring & file, const tstring & line, const tstring & filepath);

// �����ļ��������������ļ�ָ��
FILE * FindHandleByFileName(const tstring & fileName);