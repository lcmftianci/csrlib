#ifndef _SCREEN_CAPTURE_H_
#define _SCREEN_CAPTURE_H_

#include <windows.h>

struct CaptureData
{
	bool    bCaptureFullScreen;
	char	szCapturePath[1024]; // no unicode path support
	char	szCaptureFilename[1024];
};

void CaptureScreen(CaptureData* lpData);

BOOL ConfigureCapture(HWND hWnd, CaptureData* lpData);


#endif // _SCREEN_CAPTURE_H_
