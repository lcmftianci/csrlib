#include "csrscreenscanner.h"
#include "csrscreencututils.h"
#include <iostream>


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
	while (1)
	{
		recvfrom(sockSrv, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len);
		if ('q' == recvBuf[0])
		{
			sendto(sockSrv, "q", strlen("q") + 1, 0, (SOCKADDR*)&addrClient, len);
			printf("udp chat end\n");
			break;
		}
		sprintf(tempBuf, "UDP客户端%s say: %s", inet_ntoa(addrClient.sin_addr), recvBuf);
		printf("%s\n", tempBuf);
		printf("Udp Server please input data:\n");
		gets_s(sendBuf);
		sendto(sockSrv, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrClient, len);
	}
	closesocket(sockSrv);
	WSACleanup();
	return NULL;
}

#define BUFFER_SIZE 1024

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
	addrClient.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
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
		FILE * fp = fopen(strFilePath.c_str(), "wb");
		if (NULL == fp)
		{
			printf("File: %s Can Not Open To Write\n", strFilePath.c_str());
			system("pause");
			exit(1);
		}
		else
		{
			/*memset(buffer, 0, BUFFER_SIZE);
			int length = 0;
			while ((length = recv(sockClient, buffer, BUFFER_SIZE, 0)) > 0)
			{
				if (fwrite(buffer, sizeof(char), length, fp) < length)
				{
					printf("File: %s Write Failed\n", lpData->szCaptureFilename);
					break;
				}
				memset(buffer, 0, BUFFER_SIZE);
			}

			printf("Receive File: %s From Server Successful!\n", lpData->szCaptureFilename);*/

			memset(buffer, 0, BUFFER_SIZE);
			int length = 0;

			while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
			{
				if (send(sockClient, buffer, length, 0) < 0)
				{
					printf("Send File: %s Failed\n", lpData->szCaptureFilename);
					break;
				}
				memset(buffer, 0, BUFFER_SIZE);
			}

			fclose(fp);
			printf("File: %s Transfer Successful!\n", lpData->szCaptureFilename);
		}
		
	    fclose(fp);
		//sendto(sockClient, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrClient, len);
	}
	closesocket(sockClient);
	WSACleanup();
	return NULL;
}
#undef BUFFER_SIZE

