#ifndef _CSR_CURL_DOWNLOAD_H_
#define _CSR_CURL_DOWNLOAD_H_

/*
利用curl下载文件
*/
#include <stdlib.h>

//写文件回调函数
typedef size_t(*WriteFileCallBack)(void* contents, size_t size, size_t nmem, void *usrp);

//下载管理类
class CsrCurlDownload {
public:
	CsrCurlDownload();
	~CsrCurlDownload();

	int Process(const char* url, WriteFileCallBack callback_fun, void *pParams);
};


#endif  //_CURL_DOWNLOAD_H_
