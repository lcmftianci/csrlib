#include "csrcopy.h"

bool CopyFileToDest(std::string lpSrc, std::string lpDes, bool bIsExit = false, bool bmkDir = false)
{
	WIN32_FIND_DATA FileData;
	HANDLE          hSearch;
	DWORD           dwAttrs;
	TCHAR           szNewPath[MAX_PATH];

	BOOL            fFinished = FALSE;

	// Create a new directory. 

	CString strDes = lpDes.c_str();
	if (bmkDir)
	{
		if (!CreateDirectory(strDes, NULL))
			return false;
	}

	// Start searching for text files in the current directory. 
	CString strSrc = lpSrc.c_str();
	strSrc += "*.*";
	hSearch = FindFirstFile(strSrc, &FileData);
	if (hSearch == INVALID_HANDLE_VALUE)
		return false;

	// Copy each .TXT file to the new directory 
	// and change it to read only, if not already. 
	while (!fFinished)
	{
		StringCchPrintf(szNewPath, sizeof(szNewPath) / sizeof(szNewPath[0]), TEXT("%s\\%s"), strDes, FileData.cFileName);

		if (CopyFile(FileData.cFileName, szNewPath, FALSE))
		{
			dwAttrs = GetFileAttributes(FileData.cFileName);
			if (dwAttrs == INVALID_FILE_ATTRIBUTES) return false;

			if (!(dwAttrs & FILE_ATTRIBUTE_READONLY))
			{
				SetFileAttributes(szNewPath,
					dwAttrs | FILE_ATTRIBUTE_READONLY);
			}
		}
		else
		{
			printf("Could not copy file.\n");
			return false;
		}

		if (!FindNextFile(hSearch, &FileData))
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
			{
				_tprintf(TEXT("Copied *.txt to %s\n"), strDes);
				fFinished = TRUE;
			}
			else
			{
				printf("Could not find next file.\n");
				return false;
			}
		}
	}

	// Close the search handle. 
	FindClose(hSearch);
	return true;
}

