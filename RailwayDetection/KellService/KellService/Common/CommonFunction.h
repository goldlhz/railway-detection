#pragma once
class CCommonFunction
{
private:	
	CCommonFunction(void);											// �����࣬������ʵ����
public:
	static int GetKernalCount();									// ��ȡ��ǰ������CPU�˸���
	static int GetBackLogCount();									// ��õ�ǰ�ʺϵļ������г���
	static string GetLogPluginPath(HMODULE hInstance);				// ��õ�ǰ·��
	static string IntToString(int n);								// ����ת�ַ���
	static unsigned int  StringToInt(string& strData);
	static string GetCurrentTimeByFormat();

};

