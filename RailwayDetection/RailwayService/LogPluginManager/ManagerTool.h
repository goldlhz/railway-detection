#pragma once

	//////////////////////////////////////////////////////////////////////////
	// �����ļ��ṹ
	//////////////////////////////////////////////////////////////////////////
typedef struct _GlobalConfigInfo
{
	int WriteLevelMask;						// ��Ҫ�鵵�ĵȼ�����,default:31
	int CreateFullLoger;					// �Ƿ�����ȫ������־�ļ�,default:1
	int CreateAloneLoger;					// �Ƿ񴴽������ȼ�����־�ļ�,default:0
}GlobalConfigInfo;

	//////////////////////////////////////////////////////////////////////////
	// ����ں�����ָ�����ͣ�������־�������˳ʵ�������������͵ĺ���
	// �Թ��������
	//////////////////////////////////////////////////////////////////////////
typedef	bool	(*InitPlugin_Type)(const tstring &, const tstring &);
typedef bool	(*UnInitPlugin_Type)();
typedef bool	(*WriteLogInfo_Type)(DWORD , const SYSTEMTIME &, const tstring &, const tstring &, const tstring &);
typedef bool	(*WriteLogInfo_TypeEx)(DWORD , const SYSTEMTIME &, const tstring &, const tstring &, const tstring &, const tstring &);
	//////////////////////////////////////////////////////////////////////////
	// �������ָ��ṹ
	//////////////////////////////////////////////////////////////////////////
typedef struct _PluginPointer
{
	InitPlugin_Type		InitPlugin;
	UnInitPlugin_Type	UnInitPlugin;
	WriteLogInfo_Type	WriteLogInfo;
	WriteLogInfo_TypeEx	WriteLogInfoEx;
}PluginPointer;
	
	//////////////////////////////////////////////////////////////////////////
	// ��־��Ϣ�ṹ��
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
	// ���Ͷ���
	//////////////////////////////////////////////////////////////////////////
typedef deque <LogInfo>			DeqLogInfo;
typedef vector<PluginPointer>	VecPluginPointer;
typedef vector<HMODULE>			VecHModule;


	// ���ܣ�	��ȡ��־������ڵľ���·��
	// �÷���	ֱ�ӵ���
	// ����ֵ������ɹ���������ľ���·����ʧ�ܷ���_T("")
tstring GetLogPluginPath();

	// ���ܣ�	ö���ļ�ϵͳ���Ѿ���װ�õ����
	// �÷���	�����������Ŀ¼�ľ���·����һ���ַ������ݵ�����
	// ����ֵ������湦�򷵻�true����Ϊfalse��ö�ٵ���������·��������ַ���������
bool    EnumInstallPlugin(tstring strPluginPath, vector<tstring> &verPlugin);

	// ���ܣ�	��������ڵ�Ŀ¼�ַ���������ļ��������һ���γɾ���·��
	// �÷���	
	// ����ֵ��
void    BuildPluginFileName(tstring strPluginPath, vector<tstring> &verPlugin);

	// ���ܣ�	�������
	// �÷���	
	// ����ֵ��
int		LoadPlugin(const tstring & strPluginPath, const tstring & moduleName, const vector<tstring> & vecPlugin);

	// ���ܣ�	ѭ��д����־
	// �÷���	
	// ����ֵ��
DWORD   LoopWriteLog(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & strInfo, const tstring & strFile, const tstring & strLine);
DWORD   LoopWriteLogEx(DWORD errorLevel, const SYSTEMTIME & createTime, const tstring & strInfo, const tstring & strFile, const tstring & strLine, const tstring & fileName);	
	// ���ܣ�	ж����Դ
	// �÷���	
	// ����ֵ��
void	ReleasePlugin();

	// ���ܣ�	�������ַ���ת��
	// �÷���	
	// ����ֵ��
tstring IntToTString(int data);
	
	// ���ܣ�	��������ͬ����־���е��¼�
	// �÷���	
	// ����ֵ��
bool	CreateSynLogEvent();

	// ���ܣ�	�ر�����ͬ����־���е��¼�
	// �÷���	
	// ����ֵ��
bool	CloseSynLogEvent();

	// ���ܣ�	����־���������־ˢ�µ���־��
	// �÷���	
	// ����ֵ��
bool    FlashLogQueue();

	// ���ܣ�	����д��־���߳�
	// �÷���	
	// ����ֵ��
bool	CreateWriteThread();

	// ���ܣ�	����д��־���߳�
	// �÷���	
	// ����ֵ��
bool	CloseWriteThread();

	// ���ܣ�	�̺߳���
	// �÷���	
	// ����ֵ��
unsigned __stdcall WriteLogThreadFunc(void * pArguments);

	// ���ܣ�	����־д�뵽������
	// �÷���	
	// ����ֵ��
bool	WriteLogToDeque(DWORD errorLevel, const tstring & strInfo, const tstring & strFile, const tstring & strLine);

	// ���ܣ�	��ȡ�����ļ���Ϣ
	// �÷���	
	// ����ֵ��
void 	ReadLogerIniFile(tstring strPluginPath);

	// ���ܣ�	���ָ���ȼ�����־�Ƿ���Ҫ�鵵
	// �÷���	
	// ����ֵ��
bool 	IsErrorLevelLog(DWORD errorLevel);

	// ���ܣ�	������ʱ��Ϊ�������ַ���
	// �÷���	
	// ����ֵ��
tstring BuildTimeString(int nTimes);