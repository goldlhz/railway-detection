#pragma once
class CCommonFunction
{
private:	
	CCommonFunction(void);											// �����࣬������ʵ����
public:
	static int GetKernalCount();									// ��ȡ��ǰ������CPU�˸���
	static int GetBackLogCount();									// ��õ�ǰ�ʺϵļ������г���
	static wstring GetLogPluginPath(HMODULE hInstance);				// ��õ�ǰ·��
};

