#include "csrlog.h"
#include "csrfilesystem.h"

HANDLE hOutputHandle;

/*
AllocConsole();
freopen("CONOUT$", "w", stdout);
std::cout << "This is a test info" << std::endl;
BOOL AllocConsole(void);
FILE *freopen(const char *path, const char *mode, FILE *stream);
GetConsoleScreenBufferInfo 　　// 检索窗口大小，屏幕缓冲区大小及颜色属性
SetConsoleWindowInfo 　　　　　 // 改变控制台窗口大小
SetConsoleScreenBufferSize　　 // 改变控制台屏幕缓冲区大小
SetConsoleTextAttribute　　    // 设置颜色属性
SetConsoleTitle　　　　　　　　  // 设置控制台窗口标题
GetConsoleTitle　　　　　　　　  // 获取控制台窗口标题
*/


bool InitLogSys()
{
	AllocConsole();//为进程创造一个新的控制台
	//hOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);//获得控制台输出句柄
	//freopen("CONOUT$", "w", stdout);
	//获得系统时间
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	//获得程序目录
	std::string strLogFilePath = GetModlePath();
	char chBuf[200];
	sprintf(chBuf, "%04d-%02d-%02d-%02d-%02d-%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	strLogFilePath += chBuf;
	strLogFilePath += ".txt";
	if (freopen(strLogFilePath.c_str(), "wt", stdout) == NULL)
		return false;
	return true;
}

void LogWrite(std::string strLog)
{
	DWORD nRet = 0;
	//TCHAR buf[100] = { 0 };//用来输出字符的缓冲区
	//lstrcpy(buf, _T("Hello"));
	CString cstrLog = strLog.c_str();
	//WriteConsole(hOutputHandle, strLog.c_str(), lstrlen(cstrLog), &nRet, NULL);
	//std::cout << cstrLog << std::endl;
	printf("%s", strLog.c_str());
	std::cout << "---csrlog.end" << std::endl;
}

void EndLogSys()
{
	FreeConsole();
}

