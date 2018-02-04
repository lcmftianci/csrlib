#include "QtScreenShared.h"
#include <windows.h>
#include <atlstr.h>

QtScreenShared::QtScreenShared(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

//获取执行时间

void GetThunderTime()
{
	LARGE_INTEGER litmp;
	_int64 QPart1, QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;

	//3 在判断结束位置 读取运行时间

	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;
	dfMinus = (double)(QPart2 - QPart1);
	dfTim = dfMinus / dfFreq * 1000;
	//显示时间
	CString msg4 = "时间：", msg3, msg5 = "毫秒";
	msg3.Format("%10.9f", dfTim);
	CString st = msg4 + msg3 + msg5;
}


