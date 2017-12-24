#include "csrscreencututils.h"

static CString GetTempName(CString strPath)
{
	const int nMin = 0;
	const int nMax = 999;
	const CString strBase(_T("ScreenCap"));
	static int nLastKnown = nMin;

	// count up sequentially to make sure we take the next available
	// slot
	if (strPath.Right(1) != "\\")
		strPath += '\\';

	bool bFound = false;
	CString strPathName;
	while (!bFound) {
		if (nLastKnown > nMax)
			break;
		strPathName = strPath + strBase;
		strPathName.AppendFormat(_T("%03.3d.png"), nLastKnown++);
		HANDLE hFile = ::CreateFile(strPathName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE) {
			::CloseHandle(hFile);
			bFound = TRUE;
		}
	}
	if (!bFound)
		strPathName.Empty();
	return(strPathName);
}

void CaptureScreen(CaptureData* lpData)
{
	if (!lpData)
		return;
	BOOL bStat;
	CImage image;
	HWND hWnd;
	HDC hDc;
	CRect rect;
	if (lpData->bCaptureFullScreen) {
		hWnd = GetDesktopWindow();
	}
	else {
		hWnd = GetActiveWindow();
	}
	if (hWnd == NULL)
		return;
	GetWindowRect(hWnd, rect);
	hDc = GetDC(hWnd);
	int nBPP = GetDeviceCaps(hDc, BITSPIXEL) * GetDeviceCaps(hDc, PLANES);
	if (nBPP < 24)
		nBPP = 24;
	bStat = image.Create(rect.Width(), rect.Height(), nBPP);
	//ASSERT(bStat);
	if (!bStat)
		return;
	CImageDC imageDC(image);
	::BitBlt(imageDC, 0, 0, rect.Width(), rect.Height(), hDc, 0, 0, SRCCOPY);
	CString strTempName = GetTempName(CString(lpData->szCapturePath));
	HRESULT hr = image.Save(strTempName);
	if (FAILED(hr)) {
		return;
	}
	strncpy_s(lpData->szCaptureFilename, MAX_PATH, CT2A(::PathFindFileName(strTempName)), _TRUNCATE);
}

BOOL ConfigureCapture(HWND hWnd, CaptureData* lpData)
{
	if (!lpData)
		return(FALSE);
	BOOL bStat = FALSE;
	USES_CONVERSION;
	bStat = TRUE;
	lpData->bCaptureFullScreen = 1;
	strncpy_s(lpData->szCapturePath, MAX_PATH, /*CT2A(*/"D:\\vsproj\\CommonRes"/*)*/, _TRUNCATE);
	*lpData->szCaptureFilename = '\0';      
	return(bStat);
}
