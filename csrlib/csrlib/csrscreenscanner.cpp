#include "csrscreenscanner.h"
#include "csrscreencututils.h"


#define BUFFER_SIZE 1024

//视屏接受端
DWORD WINAPI udpscreenserver(LPWORD lpParam)
{
	WORD wVersionRequested;
	WSADATA wsadata;
	wVersionRequested = MAKEWORD(1, 1);
	int err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0)return 1;
	if (LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1)
	{
		WSACleanup();
		return 1;
	}

	//创建socket
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);

	//创建地址端口
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	//绑定
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	char recvBuf[100];
	char sendBuf[100];
	char tempBuf[200];
	char buffer[BUFFER_SIZE];
	while (1)
	{
		//先接受文件名称
		recvfrom(sockSrv, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len);
		//再接受文件内容
		//sendto(sockSrv, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrClient, len);
		//打开文件，准备写入  
		FILE * fp = fopen(recvBuf, "wb");
		if (NULL == fp)
		{
			printf("File: %s Can Not Open To Write\n", recvBuf);
			system("pause");
			exit(1);
		}
		else
		{
			memset(buffer, 0, BUFFER_SIZE);
			int length = 0;
			while ((length = recv(sockSrv, buffer, BUFFER_SIZE, 0)) > 0)
			{
				if (buffer[0] == 'q')
					break;
				if (fwrite(buffer, sizeof(char), length, fp) < length)
				{
					printf("File: %s Write Failed\n", recvBuf);
					break;
				}
				memset(buffer, 0, BUFFER_SIZE);
			}

			printf("Receive File: %s From Server Successful!\n", recvBuf);
		}
		fclose(fp);
		break;
	}
	closesocket(sockSrv);
	WSACleanup();
	return NULL;
}


//视屏输出端
DWORD WINAPI udpscreenclient(LPWORD lpParam)
{
	CaptureData* lpData = (CaptureData*)lpParam;
	//先处理socket相关操作

	//进入循环后就直接截屏
	WORD wVersionRequested;
	WSADATA wsadata;
	wVersionRequested = MAKEWORD(1, 1);
	int err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0)return 1;
	if (LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1)
	{
		WSACleanup();
		return 1;
	}

	//声明socket变量
	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, 0);
	//初始化地址接口
	SOCKADDR_IN addrClient;
	addrClient.sin_addr.S_un.S_addr = inet_addr("192.168.100.24");
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(6000);
	char recvBuf[100];
	char sendBuf[100];
	char tempBuf[200];
	char buffer[BUFFER_SIZE];
	int len = sizeof(SOCKADDR);
	while (1)
	{ 
		std::string strFilePath = lpData->szCapturePath;
		strFilePath += "\\";
		strFilePath += lpData->szCaptureFilename;

		//先发送文件名称
		sendto(sockClient, lpData->szCaptureFilename, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrClient, len);

		//再发送文件内容
		FILE * fp = fopen(strFilePath.c_str(), "rb");
		if (NULL == fp)
		{
			printf("File: %s Can Not Open To Write\n", strFilePath.c_str());
			system("pause");
			exit(1);
		}
		else
		{
			memset(buffer, 0, BUFFER_SIZE);
			int length = 0;

			while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
			{
				if (sendto(sockClient, buffer, length, 0, (SOCKADDR*)&addrClient, len) < 0)
				{
					printf("Send File: %s Failed\n", lpData->szCaptureFilename);
					break;
				}
				memset(buffer, 0, BUFFER_SIZE);
			}

			fclose(fp);
			printf("File: %s Transfer Successful!\n", lpData->szCaptureFilename);
		}
		
		//发送结束符
		strcpy(buffer, "q");
		sendto(sockClient, buffer, 10, 0, (SOCKADDR*)&addrClient, len);
	    fclose(fp);
		break;
	}
	closesocket(sockClient);
	WSACleanup();
	return NULL;
}
#undef BUFFER_SIZE

