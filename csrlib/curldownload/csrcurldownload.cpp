#include "csrcurldownload.h"
#include <curl/curl.h>

//#pragma comment(lib, "pthreadVC2.lib")
//#pragma comment(lib, "libcurl.lib")

//构造函�?
CsrCurlDownload::CsrCurlDownload()
{
	//初始化curl�?
	curl_global_init(CURL_GLOBAL_ALL);
}

//析构函数
CsrCurlDownload::~CsrCurlDownload()
{
	//析构�?
	curl_global_cleanup();
}

//下载线程
int CsrCurlDownload::Process(const char* url, WriteFileCallBack callback_fun, void *pParams)
{
	//初始化句�?
	CURL *curl_handle = curl_easy_init();
	int timeout = 20;

	//设置选项
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);						//下载地址
	//curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1);
	//curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, timeout);
	//curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 0);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, callback_fun);		//设置写入函数
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pParams);				//设置写入嘻嘻
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "test-agent/1.0");		//设置用户代理
    //curl_easy_setopt(curl_handle, CURLOPT_PROXY, "192.168.0.1:808");	//设置代理地址
    //curl_easy_setopt(curl_handle, CURLOPT_HTTPPROXYTUNNEL, 1L);			//设置代理协议通道

																		// post
	//curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);
	//curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, request.c_str());
	int ret = curl_easy_perform(curl_handle);

	switch (ret)
	{
	case CURLE_OK:
		printf("Curl Success\n");
		break;
	case CURLE_UNSUPPORTED_PROTOCOL:
		printf("unsupport protocol\n");
		break;
	case CURLE_FAILED_INIT:
		printf("Init failed\n");
		break;
	case CURLE_URL_MALFORMAT:
		printf("URL using bad/illegal format or missing URL \n");
		break;
	case CURLE_URL_MALFORMAT_USER:
		printf("unknown error \n");
		break;
	case CURLE_COULDNT_RESOLVE_PROXY:
		printf("couldn’t resolve proxy name \n");
		break;
	case CURLE_COULDNT_RESOLVE_HOST:
		printf("couldn’t resolve host name \n");
		break;
	case CURLE_COULDNT_CONNECT:
		printf("couldn’t connect to server \n");
		break;
	case CURLE_FTP_WEIRD_SERVER_REPLY:
		printf("FTP: weird server reply \n");
		break;
	case CURLE_FTP_USER_PASSWORD_INCORRECT:
		printf(" unknown error \n");
		break;
	case CURLE_FTP_WEIRD_PASS_REPLY:
		printf("FTP: unknown PASS reply \n");
		break;
	case CURLE_FTP_WEIRD_USER_REPLY:
		printf("FTP: unknown USER reply \n");
		break;
	case CURLE_FTP_WEIRD_PASV_REPLY:
		printf(" FTP: unknown PASV reply \n");
		break;
	case CURLE_FTP_WEIRD_227_FORMAT:
		printf("FTP: unknown 227 response format \n");
		break;
	default:
		printf("fuck error\n");
		break;
	}

	curl_easy_cleanup(curl_handle);
	return ret;
}

/*
//返回值含�?
int ret = curl_easy_perform()

CURLE_OK = 0, 0: no error
CURLE_UNSUPPORTED_PROTOCOL, 1: unsupported protocol
CURLE_FAILED_INIT, 2: failed init
CURLE_URL_MALFORMAT, 3: URL using bad/illegal format or missing URL
CURLE_URL_MALFORMAT_USER, 4: unknown error
CURLE_COULDNT_RESOLVE_PROXY, 5: couldn’t resolve proxy name
CURLE_COULDNT_RESOLVE_HOST, 6: couldn’t resolve host name
CURLE_COULDNT_CONNECT, 7: couldn’t connect to server
CURLE_FTP_WEIRD_SERVER_REPLY, 8: FTP: weird server reply
CURLE_FTP_ACCESS_DENIED,9
CURLE_FTP_USER_PASSWORD_INCORRECT, 10: unknown error
CURLE_FTP_WEIRD_PASS_REPLY, 11: FTP: unknown PASS reply
CURLE_FTP_WEIRD_USER_REPLY, 12: FTP: unknown USER reply
CURLE_FTP_WEIRD_PASV_REPLY, 13: FTP: unknown PASV reply
CURLE_FTP_WEIRD_227_FORMAT, 14: FTP: unknown 227 response format
CURLE_FTP_CANT_GET_HOST, 15: FTP: can’t figure out the host in the PASV response
CURLE_FTP_CANT_RECONNECT, 16: FTP: can’t connect to server the response code is unknown
CURLE_FTP_COULDNT_SET_BINARY, 17: FTP: couldn’t set binary mode
CURLE_PARTIAL_FILE, 18: Transferred a partial file
CURLE_FTP_COULDNT_RETR_FILE, 19: FTP: couldn’t retrieve (RETR failed) the specified file
CURLE_FTP_WRITE_ERROR, 20: FTP: the post-transfer acknowledge response was not OK
CURLE_FTP_QUOTE_ERROR, 21: FTP: a quote command returned error
CURLE_HTTP_RETURNED_ERROR, 22: HTTP response code said error
CURLE_WRITE_ERROR, 23: failed writing received data to disk/application
CURLE_MALFORMAT_USER, 24: unknown error
CURLE_UPLOAD_FAILED, 25: upload failed (at start/before it took off)
CURLE_READ_ERROR, 26: failed to open/read local data from file/application
CURLE_OUT_OF_MEMORY, 27: out of memory
CURLE_OPERATION_TIMEOUTED, 28: a timeout was reached
CURLE_FTP_COULDNT_SET_ASCII, 29: FTP could not set ASCII mode (TYPE A)
CURLE_FTP_PORT_FAILED, 30: FTP command PORT failed
CURLE_FTP_COULDNT_USE_REST, 31: FTP command REST failed
CURLE_FTP_COULDNT_GET_SIZE, 32: FTP command SIZE failed
CURLE_HTTP_RANGE_ERROR, 33: a range was requested but the server did not deliver it
CURLE_HTTP_POST_ERROR, 34: internal problem setting up the POST
CURLE_SSL_CONNECT_ERROR, 35: SSL connect error
CURLE_BAD_DOWNLOAD_RESUME, 36: couldn’t resume download
CURLE_FILE_COULDNT_READ_FILE, 37: couldn’t read a file
CURLE_LDAP_CANNOT_BIND, 38: LDAP: cannot bind
CURLE_LDAP_SEARCH_FAILED, 39: LDAP: search failed
CURLE_LIBRARY_NOT_FOUND, 40: a required shared library was not found
*/