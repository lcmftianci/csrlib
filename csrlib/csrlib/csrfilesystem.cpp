#include "csrfilesystem.h"

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

std::string GetModleName()
{
	//get modle name
	CString strCurPath;
	GetModuleFileName(NULL, strCurPath.GetBuffer(MAX_PATH), MAX_PATH);

	USES_CONVERSION;
	return W2A(strCurPath.GetBuffer());
}

