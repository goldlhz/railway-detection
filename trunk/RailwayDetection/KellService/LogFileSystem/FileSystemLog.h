/**************************************************************************
 *	���ƣ������־ϵͳ
 *	���ܣ����ô�����ɼ�¼������־��Ϣ
 *	ʹ�÷�����
 *	���ߣ����
 *	���ڣ�2010.10.25
 **************************************************************************/
#include <string>
using namespace std;

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
	// ���ܣ�	��ʼ�������������Ҫ����Դ
	// �÷���	ֱ�ӵ��ú����ȴ����ؽ��
	// ������  filePathΪ������ڵ�Ŀ¼��·��
	// ����ֵ�������ʼ���ɹ����򷵻�true�����򷵻�false
	//
	LOGMOUDLE bool InitPlugin(const tstring & filePath, const tstring & moduleName);

	// ���ܣ�	ж���������ʹ�õ���Դ
	// �÷���	ֱ�ӵ��ú����ȴ����ؽ��
	// ����ֵ�����ж�سɹ����򷵻�true�����򷵻�false
	//
	LOGMOUDLE bool UnInitPlugin();

	// ���ܣ�  ͨ��ָ���������д��־
	// �÷���	ֱ�ӵ��ú�������������־��Ϣ
	// ����ֵ�����д����־�ɹ����򷵻��㣬���򷵻�false
	//
	LOGMOUDLE bool WriteLogInfo(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & infoStr, const tstring & file, const tstring & line);
	LOGMOUDLE bool WriteLogInfoEx(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & infoStr, const tstring & file, const tstring & line, const tstring & fileName);

};