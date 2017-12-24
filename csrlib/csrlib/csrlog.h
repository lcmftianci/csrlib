#ifndef _CSR_LOG_H_
#define _CSR_LOG_H_

#include "comheader.h"

//日志系统
bool InitLogSys();

void LogWrite(std::string strLog = "");

void EndLogSys();



#endif // _CSR_LOG_H_
