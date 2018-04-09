#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <vector>

#include "filenameio.h"

#pragma comment(lib, "User32.lib")

void DisplayErrorBox(LPTSTR lpszFunction);

void ReverseFile(WIN32_FIND_DATA *tffd, LPTSTR strDir, HANDLE hFind, LARGE_INTEGER filesize);

void FindFileInDir(char* rootDir, char* strRet);

void PrintFileName(char* m_pFileDirectory);

BOOL  DirectoryList(LPCSTR Path);

int _tmain(int argc, TCHAR *argv[])
{
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize = {0};
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	char szRet[100];

	// If the directory is not specified as a command-line argument,
	// print usage.

	if (argc != 2)
	{
		_tprintf(TEXT("\nUsage: %s <directory name>\n"), argv[0]);
		return (-1);
	}

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.

	StringCchLength(argv[1], MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3))
	{
		_tprintf(TEXT("\nDirectory path is too long.\n"));
		return (-1);
	}

	_tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, argv[1]);

	DirectoryList(szDir);

	//PrintFileName(szDir);

	//FindFileInDir(szDir, szRet);

#if 0
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.
	hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
		return dwError;
	}


	// List all the files in the directory with some info about them.
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
		}
	} while (FindNextFile(hFind, &ffd) != 0);
#endif

	//ReverseFile(&ffd, szDir, hFind, filesize);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
	}

	FindClose(hFind);
	system("pause");
	return dwError;
}

//判断是否是点号
BOOL IsDots(LPTSTR strFile)
{
	if (strcmp(strFile, ".") == 0 || strcmp(strFile, "..") == 0)
		return TRUE;
	return FALSE;
}

//遍历文件夹删除指定后缀名文件
void ReverseFile(WIN32_FIND_DATA *tffd, LPTSTR strDir, HANDLE hFind, LARGE_INTEGER filesize)
{
	if (hFind == INVALID_HANDLE_VALUE)
	{
		TCHAR strTmp[MAX_PATH];
		StringCchCopy(strTmp, MAX_PATH, strDir);
		StringCchCat(strTmp, MAX_PATH, "\\*");
		hFind = FindFirstFile(strTmp, tffd);
	}
#if 0
	else
	{
		TCHAR strTmp[MAX_PATH];
		StringCchCopy(strTmp, MAX_PATH, strDir);
		//StringCchCat(strTmp, MAX_PATH, "\\*");
		if (FindNextFile(hFind, tffd) == 0)
			return;
	}
#endif

	do
	{
		if (tffd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			/*if (tffd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
				continue;*/

			if(IsDots(tffd->cFileName))
				continue;

			_tprintf(TEXT("  %s   <DIR>\n"), tffd->cFileName);
			StringCchCat(strDir, MAX_PATH, TEXT("\\"));
			StringCchCat(strDir, MAX_PATH, tffd->cFileName);
			//StringCchCat(strDir, MAX_PATH, TEXT("\\*"));
			ReverseFile(tffd, strDir, hFind, filesize);
		}
		else
		{
			filesize.LowPart = tffd->nFileSizeLow;
			filesize.HighPart = tffd->nFileSizeHigh;
			_tprintf(TEXT("  %s   %ld bytes\n"), tffd->cFileName, filesize.QuadPart);
		}
	} while (FindNextFile(hFind, tffd) != 0);
}


void DisplayErrorBox(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and clean up

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

void FindFileInDir(char* rootDir, char* strRet)
{
	char fname[MAX_PATH];
	ZeroMemory(fname, MAX_PATH);
	WIN32_FIND_DATA fd;
	ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));
	HANDLE hSearch;
	char filePathName[256];
	char tmpPath[256];
	ZeroMemory(filePathName, 256);
	ZeroMemory(tmpPath, 256);
	strcpy(filePathName, rootDir);
	BOOL bSearchFinished = FALSE;
	if (filePathName[strlen(filePathName) - 1] != '//')
	{
		strcat(filePathName, "//");
	}

	strcat(filePathName, "*");

	hSearch = FindFirstFile(filePathName, &fd);

	//Is directory
	if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)&& strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, ".."))
	{
		strcpy(tmpPath, rootDir);
		strcat(tmpPath, fd.cFileName);
		printf("%s\n", fd.cFileName);
		FindFileInDir(tmpPath, strRet);
	}
	else   if (strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, ".."))
	{
		sprintf(fname, "%-50.50s", fd.cFileName);
		printf("%s\n", fd.cFileName);
		strcat(strRet + strRet[strlen(strRet)], fname);
	}
	while (!bSearchFinished)
	{
		if (FindNextFile(hSearch, &fd))
		{
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, ".."))
			{
				strcpy(tmpPath, rootDir);
				strcat(tmpPath, fd.cFileName);
				printf("%s\n", fd.cFileName);
				FindFileInDir(tmpPath, strRet);
			}

			else   if (strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, ".."))
			{
				sprintf(fname, "%-50.50s", fd.cFileName);
				printf("%s\n", fd.cFileName);
				strcat(strRet + strRet[strlen(strRet)], fname);
			}
		}
		else
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)          //Normal Finished
			{
				bSearchFinished = TRUE;
			}
			else
				bSearchFinished = TRUE;     //Terminate Search
		}
	}
	FindClose(hSearch);
}


void PrintFileName(char* m_pFileDirectory)
{
	char pLogFileDirectory[MAX_PATH] = { 0 };
	strcpy_s(pLogFileDirectory, MAX_PATH, m_pFileDirectory);
	int iLen = strlen(pLogFileDirectory);
	pLogFileDirectory[iLen] = '*';
	pLogFileDirectory[iLen + 1] = 0;

	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(pLogFileDirectory, &findFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		int iErrno = GetLastError();
		return;
	}

	BOOL bRet = FALSE;
	for (;;)
	{
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//目录  
			if (!strcmp(findFileData.cFileName, ".") || !strcmp(findFileData.cFileName, ".."))
			{
				//.或..  
			}
			else
			{
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
				{
					//系统目录  
				}
				else
				{
					if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
					{
						//隐藏目录  
					}
					else
					{
						//普通目录  
						printf("%s\n", findFileData.cFileName);
					}
				}
			}
		}
		else
		{
			//文件  
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
			{
				//系统文件  
			}
			else
			{
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				{
					//隐藏文件  
				}
				else
				{
					//普通文件  
					printf("%s\n", findFileData.cFileName);
				}
			}
		}

		bRet = FindNextFileA(hFind, &findFileData);
		if (!bRet)
			break;
	}

	FindClose(hFind);
}



//遍历文件夹
BOOL  DirectoryList(LPCSTR Path)
{
	WIN32_FIND_DATA FindData;
	HANDLE hError;
	char FilePathName[MAX_PATH];
	// 构造路径
	char FullPathName[MAX_PATH];
	strcpy(FilePathName, Path);
	strcat(FilePathName, "\\*.*");
	hError = FindFirstFile(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		printf("搜索失败!");
		return 0;
	}
	while (::FindNextFile(hError, &FindData))
	{
		// 过虑.和..
		if (strcmp(FindData.cFileName, ".") == 0 || strcmp(FindData.cFileName, "..") == 0)
		{
			continue;
		}
		// 构造完整路径
		wsprintf(FullPathName, "%s\\%s", Path, FindData.cFileName);
		//FileCount++;
		// 输出本级的文件
		printf("%s\n", FullPathName);

		if (!CsrCmpExt(FullPathName, ".pdb"))
		{
			DeleteFile(FullPathName);
			printf("已删除\n");
		}

		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			DirectoryList(FullPathName);
		}
	}
	return 0;
}