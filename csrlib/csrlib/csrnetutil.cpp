#include "csrnetutil.h"

unsigned _stdcall tcpclient(void* lpParam)
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

	//声明SOcket变量
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	//初始化地址和端口
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	while (1)
	{
		char recvBuf[100];
		char sendBuf[100];
		recv(sockClient, recvBuf, 100, 0);
		printf("%s:说：%s\n", "TCP服务器", recvBuf);
		printf("TCP Client please Input data:\n");
		gets_s(sendBuf);
		send(sockClient, sendBuf, strlen(sendBuf) + 1, 0);
	}
	closesocket(sockClient);
	WSACleanup();
}

unsigned _stdcall tcpserver(void* lpParam)
{
	WORD wVersionRequested;
	WSADATA wsadata;
	wVersionRequested = MAKEWORD(1, 1);
	int err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0)return 1;
	if(LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1)
	{
		WSACleanup();
		return 1;
	}

	//创建socket
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	//创建地址端口
	SOCKADDR_IN addrSrv;

	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	//绑定
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	//监听事件
	listen(sockSrv, 5);
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
	char sendBuf[100];
	char tempBuf[200];
	sprintf(tempBuf, "welcome %s to screen\n", inet_ntoa(addrClient.sin_addr));
	printf(tempBuf);
	//循环监听事件
	while (1)
	{
		printf("Tcp ServerPlease input Data\n");
		gets_s(sendBuf);
		send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);
		char recvBuf[100];
		recv(sockConn, recvBuf, 100, 0);
		printf("tcp客户端%s说：%s\n", inet_ntoa(addrClient.sin_addr), recvBuf);
	}
	closesocket(sockConn);
}

DWORD WINAPI udpserver(LPWORD lpParam)
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

DWORD WINAPI udpclient(LPWORD lpParam)
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

	//声明socket变量
	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, 0);
	//初始化地址接口
	SOCKADDR_IN addrClient;
	addrClient.sin_addr.S_un.S_addr = inet_addr("192.168.174.130");
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(6000);
	char recvBuf[100];
	char sendBuf[100];
	char tempBuf[200];
	int len = sizeof(SOCKADDR);
	while (1)
	{
		printf("Udp client please input data:\n");
		gets_s(sendBuf);
		sendto(sockClient, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrClient, len);
		recvfrom(sockClient, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len);
		if ('q' == recvBuf[0])
		{
			sendto(sockClient, "q", strlen("q") + 1, 0, (SOCKADDR*)&addrClient, len);
			printf("End");
			break;
		}
		sprintf(tempBuf, "Udp服务器%s say:%s\n", inet_ntoa(addrClient.sin_addr), recvBuf);
		printf("%s\n", tempBuf);
	}
	closesocket(sockClient);
	WSACleanup();
	return NULL;
}

