#ifndef _TIME_STRAMP_H_
#define _TIME_STRAMP_H_


#include <windows.h>
#include <stdio.h>
#include <atlstr.h>

extern FILE *g_fpLog;

class timestramp {
private:
	LARGE_INTEGER m_litmp;
	_int64 QPart2;
	_int64 QPart1;
	double dfMinus, dfFreq, dfTim;
public:
	timestramp() {
		QueryPerformanceFrequency(&m_litmp);
		dfFreq = (double)m_litmp.QuadPart;
		QueryPerformanceCounter(&m_litmp);
		QPart1 = m_litmp.QuadPart;
	}

	~timestramp() {
		QueryPerformanceCounter(&m_litmp);
		QPart2 = m_litmp.QuadPart;
		dfMinus = (double)(QPart2 - QPart1);
		dfTim = dfMinus / dfFreq * 1000;

		//ÏÔÊ¾Ê±¼ä
		CString msg4 = "用时:", msg3, msg5 = "ms";
		msg3.Format("%10.9f", dfTim);
		CString st = msg4 + msg3 + msg5;

		fprintf(g_fpLog, "%s\n", st);
	}
};

#endif