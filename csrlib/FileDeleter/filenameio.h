#pragma once

#include <stdio.h>
#include <windows.h>

//获取文件后缀
char* CsrGetFileExt(char* strFile)
{
	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	printf("Path created with _makepath: %s\n\n", strFile);
	_splitpath(strFile, drive, dir, fname, ext);

	return ext;
}


//判断文件后缀是否符合要求
int CsrCmpExt(char* strFile, char* strExt)
{
	return strcmp(CsrGetFileExt(strFile), strExt);
}