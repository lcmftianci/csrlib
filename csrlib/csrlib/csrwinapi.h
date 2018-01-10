#include <windows>

std::string GetTime(int args)
{
     SYSTEMTIME      stLocal, stUTC, stUTC2;  
    FILETIME        ftLocal, ftUTC, ft;  
    ULARGE_INTEGER  uli;  
    
    char szTime = {0};
  
    GetLocalTime(&stLocal);  
    GetSystemTime(&stUTC);  
    sprintf(szTime, "Local System Time(YYYY-MM-DD HH:MM:SS): %d-%d-%d %d:%d:%d/n", stLocal.wYear, stLocal.wMonth,  
        stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);  
    sprintf(szTime, "UTC System Time  (YYYY-MM-DD HH:MM:SS): %d-%d-%d %d:%d:%d/n", stUTC.wYear, stUTC.wMonth,  
        stUTC.wDay, stUTC.wHour, stUTC.wMinute, stUTC.wSecond);  
  
    SystemTimeToFileTime(&stLocal, &ftLocal);  
    uli.LowPart = ftLocal.dwLowDateTime;  
    uli.HighPart = ftLocal.dwHighDateTime;  
    sprintf(szTime, "Local File Time: %llu/n", uli.QuadPart);  
  
    LocalFileTimeToFileTime(&ftLocal, &ftUTC);  
    uli.LowPart = ftUTC.dwLowDateTime;  
    uli.HighPart = ftUTC.dwHighDateTime;  
    sprintf(szTime, "UTC File Time: %llu/n", uli.QuadPart);  
  
    FileTimeToSystemTime(&ftUTC, &stUTC2);  
    sprintf(szTime, "UTC System Time2 (YYYY-MM-DD HH:MM:SS): %d-%d-%d %d:%d:%d/n", stUTC2.wYear, stUTC2.wMonth,  
        stUTC2.wDay, stUTC2.wHour, stUTC2.wMinute, stUTC2.wSecond);  
  
  return string(szTime);
}
