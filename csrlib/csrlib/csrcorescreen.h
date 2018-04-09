#ifndef _CSR_CORE_SCREEN_H_
#define _CSR_CORE_SCREEN_H_

#include <iostream>
#include <windows.h>
#include <atlstr.h>
#include <atlimage.h>

//截屏函数返回保存文件的路径
int screencut(std::string strPath);

int SaveBMPToFile(HBITMAP  hBitmap, CString szfilename, int x, int y);

// 搜索 SaveBmpToFile
int SaveBmpToFile(const char *filename, BITMAPINFO *info, UINT *bits);

BOOL  SaveBmp(HBITMAP  hBitmap, CString  FileName);


#endif
