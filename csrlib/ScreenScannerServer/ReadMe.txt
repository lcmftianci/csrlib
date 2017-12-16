================================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : ScreenScannerServer Project Overview
===============================================================================

The application wizard has created this ScreenScannerServer application for
you.  This application not only demonstrates the basics of using the Microsoft
Foundation Classes but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your ScreenScannerServer application.

ScreenScannerServer.vcxproj
    This is the main project file for VC++ projects generated using an application wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    application wizard.

ScreenScannerServer.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the assciation between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

ScreenScannerServer.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CScreenScannerServerApp application class.

ScreenScannerServer.cpp
    This is the main application source file that contains the application
    class CScreenScannerServerApp.

ScreenScannerServer.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++. Your project resources are in 1033.

res\ScreenScannerServer.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file ScreenScannerServer.rc.

res\ScreenScannerServer.rc2
    This file contains resources that are not edited by Microsoft
    Visual C++. You should place all resources not editable by
    the resource editor in this file.


/////////////////////////////////////////////////////////////////////////////

The application wizard creates one dialog class:

ScreenScannerServerDlg.h, ScreenScannerServerDlg.cpp - the dialog
    These files contain your CScreenScannerServerDlg class.  This class defines
    the behavior of your application's main dialog.  The dialog's template is
    in ScreenScannerServer.rc, which can be edited in Microsoft Visual C++.

/////////////////////////////////////////////////////////////////////////////

Help Support:

hlp\ScreenScannerServer.hhp
    This file is a help project file. It contains the data needed to
    compile the help files into a .chm file.

hlp\ScreenScannerServer.hhc
    This file lists the contents of the help project.

hlp\ScreenScannerServer.hhk
    This file contains an index of the help topics.

hlp\afxcore.htm
    This file contains the standard help topics for standard MFC
    commands and screen objects. Add your own help topics to this file.

hlp\afxprint.htm
    This file contains the help topics for the printing commands.

makehtmlhelp.bat
    This file is used by the build system to compile the help files.

hlp\Images\*.gif
    These are bitmap files required by the standard help file topics for
    Microsoft Foundation Class Library standard commands.


/////////////////////////////////////////////////////////////////////////////

Other Features:

ActiveX Controls
    The application includes support to use ActiveX controls.

Printing and Print Preview support
    The application wizard has generated code to handle the print, print setup, and print preview
    commands by calling member functions in the CView class from the MFC library.

/////////////////////////////////////////////////////////////////////////////

Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named ScreenScannerServer.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

ScreenScannerServer.manifest
	Application manifest files are used by Windows XP to describe an applications
	dependency on specific versions of Side-by-Side assemblies. The loader uses this
	information to load the appropriate assembly from the assembly cache or private
	from the application. The Application manifest  maybe included for redistribution
	as an external .manifest file that is installed in the same folder as the application
	executable or it may be included in the executable in the form of a resource.
/////////////////////////////////////////////////////////////////////////////

Other notes:

The application wizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, you will need
to redistribute the MFC DLLs. If your application is in a language
other than the operating system's locale, you will also have to
redistribute the corresponding localized resources MFC100XXX.DLL.
For more information on both of these topics, please see the section on
redistributing Visual C++ applications in MSDN documentation.

/////////////////////////////////////////////////////////////////////////////

//
位图文件解析


位图文件头-----位图信息段-----位图调色板（可有）------位图数据（RGB像素）或者索引数据

位图文件头
WORD    bfType;   说明文件的类型，该值必需是0x4D42，也就是字符'BM'，否则表示根本不是BMP
DWORD   bfSize;   说明该位图文件的大小，用字节为单位
WORD    bfReserved1;   保留，必须设置为0
WORD    bfReserved2;   保留，必须设置为0
DWORD   bfOffBits;   说明从文件头开始到实际的图象数据之间的字节的偏移量。这个参数是非常有用的，因为位图信息头和调色板的长度会根据不同情况而变化，所以你可以用这个偏移值迅速的从文件中读取到位数据。


位图信息段
DWORD  biSize;   说明BITMAPINFOHEADER结构所需要的字节数
LONG   biWidth;   说明图象的宽度，以象素为单位
LONG   biHeight;   说明图象的高度，以象素为单位。注：这个值除了用于描述图像的高度之外，它还有另一个用处，就是指明该图像是倒向的位图，还是正向的位图。如果该值是一个正数，说明图像是倒向的，即：数据的第一行其实是图像的最后一行，如果该值是一个负数，则说明图像是正向的。大多数的BMP文件都是倒向的位图，也就是时，高度值是一个正数。
WORD   biPlanes;   表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1
WORD   biBitCount   说明比特数/象素，其值为1、4、8、16、24、或32。
DWORD  biCompression;   说明图象数据压缩的类型，其中： BI_RGB：没有压缩 BI_RLE8：每个象素8比特的RLE压缩编码，压缩格式由2字节组成(重复象素计数和颜色索引)；BI_RLE4：每个象素4比特的RLE压缩编码，压缩格式由2字节组成 BI_BITFIELDS：每个象素的比特由指定的掩码决定。BI_JPEG：JPEG格式
DWORD  biSizeImage;   说明图象的大小，以字节为单位。当用BI_RGB格式时，可设置为0。
LONG   biXPelsPerMeter;   说明水平分辨率，用象素/米表示。
LONG   biYPelsPerMeter;   说明垂直分辨率，用象素/米表示。
DWORD  biClrUsed;   说明位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）。
DWORD  biClrImportant; 说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。

由于当前嵌入式设备使用的显示接口为RGB565格式,选择biBitCount为16的格式来存储显示屏数据.显示屏大小为800x600

#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <sys/ioctl.h>  
#include <sys/soundcard.h>  
#include <stdio.h>  
#include <unistd.h>  
#include <math.h>  
#include <string.h>  
#include <stdlib.h>  
  
static unsigned char sg_BHeader[] = {  
    0x42, 0x4D, 0x36, 0x58, 0x02, 0x00, 0x00, 0x00,  0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,   
    0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00,  0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x00, 0x00,   
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00  
};  
#define RGB565TO1555(rgb) ((unsigned short)((unsigned short)(rgb & 0x001f) | ((unsigned short)(rgb & 0xffc0) >> 1)))  
void SaveBMPFile(unsigned char *raw, char *filename)  
{  
    unsigned short *p = (unsigned short *)raw;  
    typedef unsigned int UINT;  
    typedef unsigned char UCHAR;  
    UINT m_Width = 800, m_Height = 480;  
    UINT i, j;  
    int bmp = open(filename, O_WRONLY | O_CREAT);  
    if(bmp < 0)  
        return;  
    sg_BHeader[0x02] = (UCHAR)(m_Width * m_Height * 2 + 0x36) & 0xff;  
    sg_BHeader[0x03] = (UCHAR)((m_Width * m_Height * 2 + 0x36) >> 8) & 0xff;  
    sg_BHeader[0x04] = (UCHAR)((m_Width * m_Height * 2 + 0x36) >> 16) & 0xff;  
    sg_BHeader[0x05] = (UCHAR)((m_Width * m_Height * 2 + 0x36) >> 24) & 0xff;  
    sg_BHeader[0x12] = (UCHAR)m_Width & 0xff;  
    sg_BHeader[0x13] = (UCHAR)(m_Width >> 8) & 0xff;  
    sg_BHeader[0x14] = (UCHAR)(m_Width >> 16) & 0xff;  
    sg_BHeader[0x15] = (UCHAR)(m_Width >> 24) & 0xff;  
    sg_BHeader[0x16] = (UCHAR)m_Height & 0xff;  
    sg_BHeader[0x17] = (UCHAR)(m_Height >> 8) & 0xff;  
    sg_BHeader[0x18] = (UCHAR)(m_Height >> 16) & 0xff;  
    sg_BHeader[0x19] = (UCHAR)(m_Height >> 24) & 0xff;  
    write(bmp, sg_BHeader, sizeof(sg_BHeader));  
    for(i = 0; i < m_Height; i++)  
    {  
        unsigned short *c = p + (m_Height - 1 - i) * m_Width;  
        unsigned short cc;  
        for(j = 0; j < m_Width; j++)  
        {  
            cc = RGB565TO1555(*(c + j));  
//            cc = *(c + j);  
            write(bmp, &cc, 2);  
        }  
    }  
    close(bmp);  
}  
int main(int argc, char *argv[])  
{  
    unsigned char buf[800*480*2];  
    char *filename;  
    int fb;  
    fb = open("/dev/fb0", O_RDONLY);  
    if(fb < 0)  
        exit(1);  
    if(argc == 2)  
        filename = argv[1];  
    else  
        exit(1);  
    printf("reading screen...\n");  
    read(fb, buf, 800*480*2);  
    close(fb);  
    printf("saving screen...\n");  
    SaveBMPFile(buf, filename);  
    printf("file %s created successfully\n", filename);  
    exit(0);  
}  