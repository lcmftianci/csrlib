#include "csrfilesystem.h"

//获取文件完整路径
std::string GetModleFullPath()
{
	char lpBuf[MAX_PATH];
	wchar_t *lpstr1 = new wchar_t[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, lpstr1);

	//get modle name
	CString strCurPath;
	GetModuleFileName(NULL, strCurPath.GetBuffer(MAX_PATH), MAX_PATH);

	CString strBuf = lpstr1;
	strBuf += "\\";
	strBuf += strCurPath;

	USES_CONVERSION;
	return W2A(strBuf.GetBuffer());
}

//获取文件路径不包括文件名称
std::string GetModlePath()
{
	char lpBuf[MAX_PATH];
	CString strBuf;
	wchar_t *lpstr1 = new wchar_t[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, lpstr1);
	
	USES_CONVERSION;
	strBuf = lpstr1;
	strBuf += "\\";
	return W2A(strBuf.GetBuffer());
}

//获取文件名称
std::string GetModleName()
{
	//get modle name
	CString strCurPath;
	GetModuleFileName(NULL, strCurPath.GetBuffer(MAX_PATH), MAX_PATH);

	USES_CONVERSION;
	return W2A(strCurPath.GetBuffer());
}
#if 1
//遍历文件夹
 void TraverseDir(WCHAR* strDir)
{
	WIN32_FIND_DATA wfd = { 0 };
	WCHAR szText[256] = { 0 };
	wsprintf(szText, L"%s/*.*", strDir);
	HANDLE hFind = FindFirstFileW(szText, &wfd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("Invalid file handle. Error is %u \n", GetLastError());
		return;
	}
	if((wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
	//if (wfd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
	{
		wprintf(L"%s\n", wfd.cFileName);
	}
	else if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY&& _tcscmp(wfd.cFileName, L".") != 0&& _tcscmp(wfd.cFileName, L".") != 0)
	{
		wprintf(L"[%s]\n", wfd.cFileName);
		WCHAR szTextr[256] = { 0 };
		//strcat(szTextr, strDir);
		wsprintf(szText, L"%s/%s", strDir, wfd.cFileName);
		//sprintf(szTextr, "/%s", wfd.cFileName);
		TraverseDir(szTextr);
	}
	while (FindNextFile(hFind, &wfd))
	{
		if (wfd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{
			wprintf(L"%s\n", wfd.cFileName);
		}
		else if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && _tcscmp(wfd.cFileName, _T(".")) != 0 && _tcscmp(wfd.cFileName, _T("..")) != 0)
		{
			wprintf(L"[%s]\n", wfd.cFileName);
			WCHAR szTexts[256] = { 0 };
			wcscat(szTexts, strDir);
			wsprintf(szTexts, L"/%s", wfd.cFileName);
			TraverseDir(szTexts);
		}
	}
}
#endif
//来自 shlwapi.dll 中的 Windows API 系列：
/*
// 扩展名相关操作
PathAddExtension
PathFindExtension
PathRemoveExtension
PathRenameExtension
// 附加路径或组合两个路径
PathAppend
PathCombine
// 判断是否根目录或文件夹
PathIsRoot
PathIsDirectory
// 文件是否存在
PathFileExists
// 文件名是否合法
PathIsFileSpec
// 获取或移除命令行参数
PathGetArgs
PathRemoveArgs
// 文件名是否匹配通配符
PathMatchSpec
// 得到一个文件夹相对于另一文件夹的相对路径
PathRelativePathTo
// 查找文件名
PathFindFileName
// 移除文件名
PathRemoveFileSpec
*/
BOOL CALLBACK ShowFoundFiles(LPCTSTR pszFile, LPARAM lParam)
{
	_tprintf(TEXT("%s\n"), pszFile);
	// 返回 TRUE 继续查找，返回 FALSE 终止。
	return TRUE;
}

BOOL WINAPI FindAllFilesRecursively(LPCTSTR pszRootPath,FINDFILESPROC ffproc,LPARAM lpParam)
{
	TCHAR szFileFind[MAX_PATH] = { 0 };
	if (PathCombine(szFileFind, pszRootPath, TEXT("*.*")) == NULL)
	{
		SetLastError(ERROR_INVALID_NAME);
		return FALSE;
	}
	WIN32_FIND_DATA wfd = { 0 };
	HANDLE hFindFile = FindFirstFile(szFileFind, &wfd);
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		SetLastError(ERROR_INVALID_NAME);
		return FALSE;
	}
	__try {
		do {
			if ((lstrcmpi(wfd.cFileName, TEXT(".")) != 0) &&
				(lstrcmpi(wfd.cFileName, TEXT("..")) != 0))
			{
				BOOL fResult = FALSE;
				TCHAR szNextPath[MAX_PATH] = { 0 };
				if (PathCombine(szNextPath, pszRootPath, wfd.cFileName) == NULL)
				{
					return FALSE;
				}
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					fResult = FindAllFilesRecursively(szNextPath, ffproc, lpParam);
				}
				else
				{
					fResult = ffproc(szNextPath, lpParam);
				}
				if (!fResult)
				{
					return FALSE;
				}
			}
		} while (FindNextFile(hFindFile, &wfd));
	}
	__finally {
		FindClose(hFindFile);
	}
	return TRUE;
}


//获得所有盘符
void GetAllDrive(std::vector<std::string>& allDri)
{
	WCHAR Drive[MAX_PATH] = { 0 };
	int i = 0;
	GetLogicalDriveStrings(MAX_PATH, Drive);
	char strDri[10];
	while (Drive[i - 1] != '\0' || Drive[i] != '\0')
	{
		sprintf(strDri, "%s", &Drive[i]);
		i += 2;
		allDri.push_back(std::string(strDri));
	}
}

#if 0
void list_current_dir()
{
	WIN32_FIND_DATA FileData;
	HANDLE hSearch = NULL;
	BOOL finished = FALSE;

	printf("%d\n", sizeof(FILETIME));
	printf("%d\n", sizeof(WIN32_FIND_DATA));

	hSearch = FindFirstFile("*", &FileData);  // find for all files
	if (hSearch == INVALID_HANDLE_VALUE)
		return;   // No file found

	while (!finished)
	{
		//puts(FileData.cFileName);    // print file name
		printf("%s\n", FileData.cFileName);
		finished = !FindNextFile(hSearch, &FileData);   // find next file
	}
	FindClose(hSearch);  // finish searching
}
#endif

void FindFiles(CString &strPath)
{
	WIN32_FIND_DATA findData;

	CString strTemp = strPath;
	strTemp.Format(_T("%s\\*.*"), strPath);//查找指定目录下的直接的所有文件和目录

	HANDLE hFile = FindFirstFile(strTemp, &findData);
	while (hFile != INVALID_HANDLE_VALUE)
	{
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//如果是目录
		{
			if (findData.cFileName[0] != _T('.'))//排除.和..文件夹
			{

				strTemp = strPath + CString("\\") + findData.cFileName;//获得完整路径
				FindFiles(strTemp);//递归查找当前目录的文件
			}
		}
		else
		{
			strTemp = strPath + CString("\\") + findData.cFileName;
		}

		if (!FindNextFile(hFile, &findData))
		{
			break;
		}
	}
}