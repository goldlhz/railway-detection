/**************************************************************************
 *	���ƣ������־ϵͳ
 *	���ܣ����ô�����ɼ�¼������־��Ϣ
 *	ʹ�÷�������˳����ú������ȳ�ʼ������ʹ�ã���ʹ�þͿ��Խ����ͷ�
 *              Ӧ���ڳ���һ��ʼ�ͽ�������������г�ʼ��������ر�֮ǰ
 *				�ٽ����ͷ�
 *	���ߣ����
 *	���ڣ�2010.10.25
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

// ���嵼�뵼����
#ifndef		REMINELOGPLUGIN
#define		LOGMOUDLE	__declspec(dllimport)
#else
#define		LOGMOUDLE	__declspec(dllexport)
#endif

// �������ȼ�
#define LOG_EMERG			1						// ����		ϵͳ������
#define LOG_CRIT			2						// ����		Σ����Ϣ����Ӳ������
#define LOG_ERR				3						// ����		������Ϣ
#define LOG_INFO			4						// ��Ϣ		֪ͨ��Ϣ
#define LOG_DEBUG			5						// ����		������Ϣ������������ʹ����Ϣ

extern "C"
{
	// ���ܣ�  ��ʼ����־���
	// �÷���	�Ӳ����д�����Ҫʹ�õ���־��������� 
	// ����ֵ������ɹ�����Ϊ�㣬���ȫ��������-1��������ݳ�����1
	//
	LOGMOUDLE DWORD __stdcall InitLogPluginManagerA(char* moduleName);
	LOGMOUDLE DWORD __stdcall InitLogPluginManagerW(wchar_t* moduleName);
	// ���ܣ�  ͨ��ָ���������д��־
	// �÷���	ֱ�ӵ��ú�������������־��Ϣ
	// ����ֵ���ɹ����򷵻��㣬����-1��ʾȫ����������1��ʾ���ֳ���
	//			�͵ȼ���־���첽��ʽд�룬������Ƿ��سɹ��ģ��ߵȼ���־��ͬ��д��ģ��п��ܷ��ش���
	LOGMOUDLE DWORD __stdcall WriteLogInfoBaseA(DWORD errorLevel, const char* file, int line, const char* fmt, ...);
	LOGMOUDLE DWORD __stdcall WriteLogInfoBaseW(DWORD errorLevel, const wchar_t* file, int line, const wchar_t* fmt, ...);
	// ���ܣ�  ͨ��ָ���������д��־
	// �÷���	ֱ�ӵ��ú�������������־��Ϣ
	// ����ֵ���ɹ����򷵻��㣬����-1��ʾȫ����������1��ʾ���ֳ���
	//			�͵ȼ���־���첽��ʽд�룬������Ƿ��سɹ��ģ��ߵȼ���־��ͬ��д��ģ��п��ܷ��ش���
	LOGMOUDLE DWORD __stdcall DoWriteLogInfoBaseA(DWORD errorLevel, const char* infoStr, const char* file, int line);
	LOGMOUDLE DWORD __stdcall DoWriteLogInfoBaseW(DWORD errorLevel, const wchar_t* infoStr, const wchar_t* file, int line);
	// ���ܣ�  ͨ��ָ���������д��־
	// �÷���  ֱ�ӵ��ú�������������־��Ϣ������fileName��ʾ�������ɵ��ļ������Ʋ��������fileNameΪ�գ�������ͬ��ֱ�ӵ���WriteLogInfoBase
	// ����ֵ���ɹ����򷵻��㣬����-1��ʾȫ����������1��ʾ���ֳ���
	//			�͵ȼ���־���첽��ʽд�룬������Ƿ��سɹ��ģ��ߵȼ���־��ͬ��д��ģ��п��ܷ��ش���
	LOGMOUDLE DWORD __stdcall WriteLogInfoBaseExA(DWORD errorLevel, const char* file, int line, const char * fileName, const char* fmt, ...);
	LOGMOUDLE DWORD __stdcall WriteLogInfoBaseExW(DWORD errorLevel, const wchar_t* file, int line, const wchar_t * fileName, const wchar_t* fmt, ...);
	// ���ܣ�  ͨ��ָ���������д��־
	// �÷���	ֱ�ӵ��ú�������������־��Ϣ������fileName��ʾ�������ɵ��ļ������Ʋ��������fileNameΪ�գ�������ͬ��ֱ�ӵ���DoWriteLogInfoBase
	// ����ֵ���ɹ����򷵻��㣬����-1��ʾȫ����������1��ʾ���ֳ���
	//			�͵ȼ���־���첽��ʽд�룬������Ƿ��سɹ��ģ��ߵȼ���־��ͬ��д��ģ��п��ܷ��ش���
	//			
	LOGMOUDLE DWORD __stdcall DoWriteLogInfoBaseExA(DWORD errorLevel, const char* infoStr, const char* file, int line, const char * fileName);
	LOGMOUDLE DWORD __stdcall DoWriteLogInfoBaseExW(DWORD errorLevel, const wchar_t* infoStr, const wchar_t* file, int line, const wchar_t * fileName);
	// ���ܣ�  ж����־���
	// �÷���	ֱ�ӵ���
	// ����ֵ�����Ƿ���0
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