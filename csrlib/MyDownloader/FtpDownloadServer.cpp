#include <WINSOCK2.H>
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include "package.h"
#pragma comment (lib,"ws2_32.lib")
using namespace ::std;
char * getByteFromFile(FILE * fp, char buf[], int start, int length) {
	int sign;
	int c;
	int i = 0;
	char set[6];
	c = getc(fp);
	while (!feof(fp) && i < BUFFERLENGTH - 1)
	{
		*(buf + i) = c;
		i++;
		c = getc(fp);
	}
	if (feof(fp))
	{
		set[0] = '0';
		buf[i] = '\0';
	}
	else
	{
		set[0] = '1';
		buf[i] = c;
	}
	buf[i + 1] = '\0';

	int f = 1000;
	for (int j = 1;j < 5;j++)
	{
		set[j] = strlen(buf) / f % 10 + '0';
		f = f / 10;
	}
	set[5] = '\0';
	return set;
}

DWORD WINAPI serverFunction(LPVOID parameter) {
	SOCKET acceptSock = (SOCKET)parameter;

	char receiveBuf[BUFFERLENGTH + 1];//接收
	char sendBuf[BUFFERLENGTH + 1];//接收

	strcpy(sendBuf, "hello!");
	printf("Send:%s\n", sendBuf);
	send(acceptSock, sendBuf, strlen(sendBuf) + 1, 0);
	int len = recv(acceptSock, receiveBuf, sizeof(receiveBuf) - 1, 0);
	receiveBuf[len] = '\0';
	printf("recv:%s\n", receiveBuf);
	//将收到数据变为文件名
	FILE * fp;
	if (strcmp(receiveBuf, "") == 0) return -1;
	//查找文件是否存在
	if ((fp = fopen(receiveBuf, "r")) == NULL)
	{
		printf("Can't open the %s", receiveBuf);
		send(acceptSock, "99999", 6, 0);
		return 0;
	}
	else {
		//传输文件
		char * head = (char*)malloc(sizeof(char)*(5 + BUFFERLENGTH + 1));
		do
		{
			strcpy(head, getByteFromFile(fp, sendBuf, 0, BUFFERLENGTH));
			strcat(head, sendBuf);
			//printf("%s",head);
			send(acceptSock, head, strlen(head) + 1, 0);
			Sleep(40);
		} while (head[0] == '1');
		fclose(fp);
		return 0;
	}

}
int main() {
	WSADATA WSAdata;
	SOCKET listenSock, acceptSock;
	SOCKADDR_IN addr_in;
	WORD versionRequest;
	versionRequest = MAKEWORD(2, 2);
	int err;
	err = WSAStartup(versionRequest, &WSAdata);
	if (err) {
		cout << "Error: WSAStartup()!" << endl;
	}
	if ((listenSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		printf("error:  WSASocket()! error_number:%d", WSAGetLastError());
		return 1;
	}
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(SERVERPORT);
	addr_in.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if (bind(listenSock, (sockaddr*)&addr_in, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		printf(" Failed bind() \n");
		return -1;
	}

	listen(listenSock, 5);

	SOCKADDR_IN clientsocket;
	int len = sizeof(SOCKADDR);
	cout << "server startup sucessful" << endl;
	while (1) {
		acceptSock = accept(listenSock, (SOCKADDR*)&clientsocket, &len);

		cout << "new client in!" << endl;
		CreateThread(NULL, 0, serverFunction, (LPVOID)acceptSock, 0, NULL);
	}
	WSACleanup();//释放资源的操作
	return -1;
}