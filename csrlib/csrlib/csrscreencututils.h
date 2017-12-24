#ifndef _CSR_SCREEN_CUT_UTILS_H_
#define _CSR_SCREEN_CUT_UTILS_H_

#include "comheader.h"

struct CaptureData
{
	bool    bCaptureFullScreen;
	char	szCapturePath[1024]; // no unicode path support
	char	szCaptureFilename[1024];
};

void CaptureScreen(CaptureData* lpData);

BOOL ConfigureCapture(HWND hWnd, CaptureData* lpData);


#endif // _CSR_SCREEN_CUT_UTILS_H_
