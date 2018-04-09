#ifndef _CSR_FILE_SYSTEM_H_
#define _CSR_FILE_SYSTEM_H_

#include "comheader.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

typedef BOOL(CALLBACK *FINDFILESPROC)(LPCTSTR, LPARAM);

BOOL CALLBACK ShowFoundFiles(LPCTSTR pszFile, LPARAM lParam);

BOOL WINAPI FindAllFilesRecursively(LPCTSTR, FINDFILESPROC, LPARAM);

//获取文件路径不包括文件名称
std::string GetModlePath();

//获取文件完整路径
std::string GetModleFullPath();

//获取文件名称
std::string GetModleName();

//获取所有盘符
void GetAllDrive(std::vector<std::string>& allDri);

//遍历文件夹
void TraverseDir(WCHAR* strDir);

void FindFiles(CString &strPath);

#endif // _CSR_FILE_SYSTEM_H_
