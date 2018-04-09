#ifndef _RAD_MIRROR_H_
#define _RAD_MIRROR_H_

#include "comheader.h"

class RDPCapture
{
	int mw;
	int mh;
	int mx0;
	int my0;
	DEVMODE myDM;
	DISPLAY_DEVICE myDev;
	HDC m_driverDC;
	//CDC m_cdc;
	HDC m_hdc;
	BITMAPINFO	m_BmpInfo;
	HBITMAP	m_Bitmap;
	HBITMAP	Old_bitmap;
	DEVMODE oldDM;
public:
	RDPCapture() {}
	RDPCapture(DISPLAY_DEVICE dev, DEVMODE dm)
	{
		myDev = dev;
		myDM = dm;
		oldDM = dm;
		m_driverDC = NULL;
	}
	~RDPCapture()
	{
		//SelectObject(m_cdc, Old_bitmap);
		SelectObject(m_hdc, Old_bitmap);
		DeleteObject(m_Bitmap);
		//m_cdc.DeleteDC();
		DeleteDC(m_hdc);
		if (m_driverDC != NULL) DeleteDC(m_driverDC);
		oldDM.dmDeviceName[0] = 0;
		ChangeDisplaySettingsEx(myDev.DeviceName, &oldDM, 0, 0, 0);
	}
	virtual bool Init(int x0, int y0, int width, int height)
	{
		mx0 = x0;
		my0 = y0;
		mw = (width + 3) & 0xFFFC;
		mh = height;

		DEVMODE dm;
		dm = myDM;
		WORD drvExtraSaved = dm.dmDriverExtra;
		memset(&dm, 0, sizeof(DEVMODE));
		dm.dmSize = sizeof(DEVMODE);
		dm.dmDriverExtra = drvExtraSaved;
		dm.dmPelsWidth = 2048;
		dm.dmPelsHeight = 1280;
		dm.dmBitsPerPel = 24;
		dm.dmPosition.x = 0;
		dm.dmPosition.y = 0;
		dm.dmDeviceName[0] = '\0';
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_POSITION;
		if (ChangeDisplaySettingsEx(myDev.DeviceName, &dm, 0, CDS_UPDATEREGISTRY, 0))
		{
			ChangeDisplaySettingsEx(myDev.DeviceName, &dm, 0, 0, 0);
		}
		//------------------------------------------------
		ZeroMemory(&m_BmpInfo, sizeof(BITMAPINFO));
		m_BmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		m_BmpInfo.bmiHeader.biBitCount = 24;
		m_BmpInfo.bmiHeader.biCompression = BI_RGB;
		m_BmpInfo.bmiHeader.biPlanes = 1;
		m_BmpInfo.bmiHeader.biWidth = mw;
		m_BmpInfo.bmiHeader.biHeight = -mh;
		m_BmpInfo.bmiHeader.biSizeImage = mw*mh * 3;
		HDC Top = ::GetDC(GetDesktopWindow());
		//m_cdc.CreateCompatibleDC(NULL);//兼容设备上下文环境
		CreateCompatibleDC(m_hdc);
		m_Bitmap = CreateCompatibleBitmap(Top, mw, mh);//Bitmap,画布
		//Old_bitmap = (HBITMAP)SelectObject(m_cdc, m_Bitmap);//画布与设备上下文环境关联
		Old_bitmap = (HBITMAP)SelectObject(m_hdc, m_Bitmap);//画布与设备上下文环境关联
		::ReleaseDC(GetDesktopWindow(), Top);
		m_driverDC = CreateDC(myDev.DeviceName, 0, 0, 0);
		return true;
	};
	virtual bool GetData(unsigned char *buf)
	{
		//BitBlt(m_cdc, 0, 0, mw, mh, m_driverDC, mx0, my0, SRCCOPY | CAPTUREBLT);
		BitBlt(m_hdc, 0, 0, mw, mh, m_driverDC, mx0, my0, SRCCOPY | CAPTUREBLT);
		//GetDIBits(m_cdc, m_Bitmap, 0, mh, buf, &m_BmpInfo, DIB_RGB_COLORS);
		GetDIBits(m_hdc, m_Bitmap, 0, mh, buf, &m_BmpInfo, DIB_RGB_COLORS);
		return true;
	};
};

int Detect_MirrorDriver(std::vector<DISPLAY_DEVICE>& devices, std::map<int, DEVMODE>& settings);

#endif