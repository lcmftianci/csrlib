#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib,"Ws2_32.lib")


int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET serverSocket;

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(sockaddr_in));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = inet_addr("74.125.71.147");
	//serverAddress.sin_addr.S_un.S_addr = inet_addr("211.100.26.77");
	serverAddress.sin_port = htons(80);

	connect(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	char * msgHttpRequest = "GET http://portablecb.googlecode.com/files/LoveDEV.7z HTTP/1.1\r\n"
		"Host:google.comt\r\n"
		"Accept:*/* \r\n\r\n";

	FILE *fp;
	fp = fopen("l.jpg", "w+");
	int nCount = 0;
	int nTimes = 1;
	while (nTimes--)
	{
		send(serverSocket, msgHttpRequest, strlen(msgHttpRequest), 0);

		Sleep(3100);
		char recvBuff[1024] = { 0 };
		int ret = 0;
		while (1)
		{
			//记得检查HTTP的状态
			memset(recvBuff, 0, 1024);
			ret = recv(serverSocket, recvBuff, 1024, 0);
			if (ret < 0)
			{
				closesocket(serverSocket);
				break;
			}
			if (ret == 0)
			{
				break;
			}
			printf("%s\n", recvBuff);
		}

		char * pCharFind = NULL;
		pCharFind = strstr(recvBuff, "bytes");
		int len = pCharFind - recvBuff;
		len += 4;
		int recvLen = strlen(recvBuff);
		fwrite((void*)(recvBuff + len), strlen(recvBuff) - len, strlen(recvBuff) - len, fp);
		fflush(fp);
		fclose(fp);
		printf("len:%d\n%s\n", recvLen, recvBuff);
		printf("----------------------------------------");
		printf("%d\n\n", nCount++);
		Sleep(100);
	}

	return 0;
}