#include "csrcorescreen.h"

#include <Gdiplusheaders.h>
#include <gdiplus.h>


int SaveBMPToFile(HBITMAP  hBitmap, CString szfilename, int x, int y)
{
	HDC     hDC;
	//当前分辨率下每象素所占字节数            
	int     iBits;
	//位图中每象素所占字节数            
	WORD     wBitCount;
	//定义调色板大小，     位图中像素字节大小     ，位图文件大小     ，     写入文件字节数                
	DWORD     dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	//位图属性结构                
	BITMAP     Bitmap;
	//位图文件头结构            
	BITMAPFILEHEADER     bmfHdr;
	//位图信息头结构                
	BITMAPINFOHEADER     bi;
	//指向位图信息头结构                    
	LPBITMAPINFOHEADER     lpbi;
	//定义文件，分配内存句柄，调色板句柄                
	HANDLE     fh, hDib, hPal, hOldPal = NULL;

	//计算位图文件每个像素所占字节数                
	hDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL)*GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else  if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else
		wBitCount = 24;

	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;

	dwBmBitsSize = ((Bitmap.bmWidth *wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	//为位图内容分配内存                
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	//     处理调色板                    
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	//     获取该调色板下新的像素值                
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
		(BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	//恢复调色板                    
	if (hOldPal)
	{
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	//创建位图文件                    
	fh = CreateFile(szfilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)         return     FALSE;

	//     设置位图文件头                
	bmfHdr.bfType = 0x4D42;     //     "BM"                
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//     写入位图文件头                
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//     写入位图文件其余内容                
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//清除                    
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return  TRUE;
}

//VC下把HBITMAP保存为bmp图片   
BOOL  SaveBmp(HBITMAP  hBitmap, CString  FileName)
{
	HDC hDC;
	//当前分辨率下每象素所占字节数         
	int iBits;
	//位图中每象素所占字节数         
	WORD wBitCount;
	//定义调色板大小，     位图中像素字节大小     ，位图文件大小     ，     写入文件字节数             
	DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	//位图属性结构             
	BITMAP Bitmap;
	//位图文件头结构         
	BITMAPFILEHEADER bmfHdr;
	//位图信息头结构             
	BITMAPINFOHEADER bi;
	//指向位图信息头结构                 
	LPBITMAPINFOHEADER lpbi;
	//定义文件，分配内存句柄，调色板句柄             
	HANDLE fh, hDib, hPal, hOldPal = NULL;

	int aa = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 288 * 352 * 24 / 8;

	//计算位图文件每个像素所占字节数             
	hDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL)     *     GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else  if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else
		wBitCount = 24;

	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = Bitmap.bmWidth * Bitmap.bmHeight * 3;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;

	dwBmBitsSize = ((Bitmap.bmWidth *wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	//为位图内容分配内存             
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	//     处理调色板                 
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	//     获取该调色板下新的像素值             
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
		(BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	//恢复调色板                 
	if (hOldPal)
	{
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	//创建位图文件                 
	fh = CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)         
		return  FALSE;

	//     设置位图文件头             
	bmfHdr.bfType = 0x4D42;     //"BM"             
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//     写入位图文件头             
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//     写入位图文件其余内容             
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize - 14, &dwWritten, NULL);
	//清除                 
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return  TRUE;
}

//截屏函数返回保存文件的路径
int screencut(std::string strPath)
{
	//获取屏幕DC
	HDC hDesktopDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	//内存DC
	HDC hMemoryDC = CreateCompatibleDC(hDesktopDC);
	//得到屏幕宽度
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	//根据屏幕DC创建屏幕位图
	HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, nScreenWidth, nScreenHeight);
	//将位图选择到内存DC中
	BitBlt(hMemoryDC, 0, 0, nScreenWidth, nScreenHeight, hDesktopDC, 0, 0, SRCCOPY);
	//得到鼠标位置
	POINT pt;
	GetCursorPos(&pt);
	//得到鼠标位置
	HCURSOR m_hcursor = LoadCursor(NULL, IDC_ARROW);
	DrawIconEx(hMemoryDC, pt.x, pt.y, m_hcursor, 0, 0, 0, NULL, DI_NORMAL | DI_COMPAT);
	SelectObject(hMemoryDC, hCaptureBitmap);
	//char str1[20] = "D:/123.bmp";
	//SaveBMPToFile(hCaptureBitmap, strPath.c_str(), pt.x, pt.y);
	SaveBmp(hCaptureBitmap, (CString)strPath.c_str());
	DeleteDC(hDesktopDC);
	DeleteDC(hMemoryDC);
	DeleteObject(hCaptureBitmap);
	return 0;
}


#if 0
BOOL SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
	CLSID pngClsid;

	Bitmap bmp(hBitmap, NULL);

	//获取BMP文件的编码方式(如果希望获取JPEG的编码方式，

	//那么参数一要设置为：_TEXT("image/jpeg")，其他支持的图片格式类似)
	int nResult = GetEncoderClsid(_TEXT("image/bmp"), &pngClsid);

	if (nResult >= 0)
	{
		//保存所截取的屏幕图片
		bmp.Save(lpszFileName, &pngClsid);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

//@brief 获取图片文件的编码方式，支持bmp、jpg、jpeg、gif、tiff和png等格式图片
//@date   1-13-2009  
//@param [in]  format 图片格式 值可以为以下几种
//@"image/bmp"
//@"image/jpeg"
//@"image/gif"
//@"image/tiff"
//@"image/png"
//@param [in]  pClsid
//@return  成功则返回值 >= 0，失败则返回值为-1
int GetEncoderClsid(const WCHAR* format, CLSID *pClsid)
{
	int nRet = -1;
	ImageCodecInfo* pCodecInfo = NULL;
	UINT nNum = 0, nSize = 0;
	GetImageEncodersSize(&nNum, &nSize);

	if (nSize<0)
	{
		return nRet;
	}

	pCodecInfo = new ImageCodecInfo[nSize];

	if (pCodecInfo == NULL)
	{
		return nRet;
	}

	GetImageEncoders(nNum, nSize, pCodecInfo);

	for (UINT i = 0; i<nNum;i++)
	{
		if (wcscmp(pCodecInfo[i].MimeType, format) == 0)
		{
			*pClsid = pCodecInfo[i].Clsid;
			nRet = i;
			delete[] pCodecInfo;
			return nRet;
		}
		else
		{
			continue;
		}
	}

	delete[] pCodecInfo;
	return nRet;
}
#endif

// 搜索 SaveBmpToFile
int SaveBmpToFile(const char *filename, BITMAPINFO *info, UINT *bits)
{
	FILE* fp;
	UINT size, //Size of file
		infosize, //Size of bitmap info
		bitsize;  //Size of bitmap pixels
	BITMAPFILEHEADER header;
	if ((fp = fopen(filename, "wb")) == NULL)
		return -1;
	if (info->bmiHeader.biSizeImage == 0)
		bitsize = (info->bmiHeader.biWidth *
			info->bmiHeader.biBitCount + 7) / 8 *
		abs(info->bmiHeader.biHeight);
	else
		bitsize = info->bmiHeader.biSizeImage;

	infosize = sizeof(BITMAPINFO);
	size = sizeof(BITMAPFILEHEADER) + infosize + bitsize;

	header.bfType = 'MB';
	header.bfSize = size;
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bfOffBits = sizeof(BITMAPFILEHEADER) + infosize;

	if (fwrite(&header, 1, sizeof(BITMAPFILEHEADER), fp) <
		sizeof(BITMAPFILEHEADER))
	{
		fclose(fp);
		return -1;
	}
}

#if 0
void ImageUtils()//灰度处理
{
	// TODO: Add your control notification handler code here
	CImage img;
	int Height, Width;
	int r, g, b;
	img.Load("C:\\Documents and Settings\\Administrator\\桌面\\p1.bmp");
	Height = img.GetHeight(); //获取图片的高度
	Width = img.GetWidth(); //获取图片的宽度
							/*申请动态的二维数组*/
	int **bmp;//**
	bmp = new int*[Height];
	for (int k = 0;k<Height;k++)
	{
		bmp[k] = new int[Width];
	}
	for (int i = 0;i<Width;i++)
	{/*将位图的像素值存入二维数组*/
		for (int j = 0;j<Height;j++)
		{
			COLORREF col = img.GetPixel(i, j);
			r = GetRValue(col);
			g = GetGValue(col);
			b = GetBValue(col);
			int rgb = int((r + g + b) / 3); //灰度处理
			bmp[j][i] = rgb;
		}
	}
	/*二维数组里存的像素，显示*/
	CDC *pDC;
	pDC = GetDC();
	for (int i = 0;i<Height;i++)
	{
		for (int j = 0;j<Width;j++)
		{
			pDC->SetPixel(j, i, RGB(bmp[i][j], bmp[i][j], bmp[i][j]));
		}
	}
	/*像素值写入data.txt文件*/
	FILE *fp = fopen("data.txt", "w");
	for (int i = 0;i<Height;i++)
	{
		for (int j = 0;j<Width;j++)
		{
			if (j == Width - 1)//?
			{
				fprintf(fp, "%d", bmp[i][j]);
			}
			else
				fprintf(fp, "%d ", bmp[i][j]);
		}
		fprintf(fp, "\n", 1);
	}
	fclose(fp);
	/*释放动态的二维数组*/
	for (int i = 0;i<Height;i++)
		delete[]bmp[i]; //以行为单位释放数值空间 
	delete[]bmp;
	///保存灰度图
	//CBitmap bmp;
	//img.BitBlt()
	//img.Save("C:\\Documents and Settings\\Administrator\\桌面\\p11.bmp");
}

#endif