#pragma once
class CCommonFunction
{
private:	
	CCommonFunction(void);											// 工具类，不允许实例化
public:
	static int GetKernalCount();									// 读取当前机器的CPU核个数
	static int GetBackLogCount();									// 获得当前适合的监听队列长度
	static string GetLogPluginPath(HMODULE hInstance);				// 获得当前路径
	static string IntToString(int n);								// 整数转字符串
	static unsigned int  StringToInt(string& strData);
	static string GetCurrentTimeByFormat();

};

