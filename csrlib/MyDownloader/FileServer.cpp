
#include<winsock2.h>
#include<windows.h>
#include<stdio.h>
#include<Ws2tcpip.h>
#include<iostream>
#include<fstream>

#include  <direct.h>

#pragma comment(lib,"Ws2_32.lib")
using namespace std;

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   8088
#define BUFFER_SIZE 4096
char    SendBuffer[BUFFER_SIZE], RecvBuffer[BUFFER_SIZE];   //发送缓冲区及接收缓冲区
SOCKET          AcceptSocket;
DWORD WINAPI ClientThread(LPVOID lp);
struct CLIENTINFO {
	SOCKET          clientsocket;
	sockaddr_in     sockaddr;
};

int main(int argc, TCHAR* argv[])
{
	int         Ret;
	WSADATA     wsaData;

	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != NO_ERROR)
	{
		printf("WSAStartup failed with error %d\n", Ret);
		return -1;
	}//if

	 // Create a new socket to listening for client connections.
	SOCKET      ListenSocket;
	if ((ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
		== INVALID_SOCKET)
	{
		printf("socket failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}//if

	sockaddr_in     service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(DEFAULT_PORT);

	if (bind(ListenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		printf("bind failed with error %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return -1;
	}//if

	if (listen(ListenSocket, 10) == SOCKET_ERROR)
	{
		printf("Error listening on socket.\n");
		closesocket(ListenSocket);
		WSACleanup();
		return -1;
	}//if

	SOCKET          AcceptSocket;
	sockaddr_in     ClientAddr;//接收客户端地址
	int     addrClientlen = sizeof(ClientAddr);
	while (true)
	{

		CLIENTINFO      clientInfo;
		printf("waiting for client to connect......\n");
		AcceptSocket = accept(ListenSocket, (sockaddr*)&ClientAddr, &addrClientlen);
		if (AcceptSocket == INVALID_SOCKET)
		{
			printf("accept failed : %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}//if
		clientInfo.clientsocket = AcceptSocket;
		memcpy(&clientInfo.sockaddr, &ClientAddr, sizeof(sockaddr_in));
		printf("a client is coming......\n");
		DWORD dwThreadID;
		CreateThread(NULL, 0, ClientThread, (LPVOID*)&clientInfo, 0, &dwThreadID);
	}//while
	closesocket(ListenSocket);
	WSACleanup();
	return 0;
} //main


DWORD WINAPI ClientThread(LPVOID lp)
{
	CLIENTINFO *pClientInfo = (CLIENTINFO*)lp;
	SOCKET  clientsocket = pClientInfo->clientsocket;
	sockaddr_in sockaddr;
	memcpy(&sockaddr, &pClientInfo->sockaddr, sizeof(sockaddr_in));
	int     iResult, bytes;
	char    recvRFileName[DEFAULT_BUFLEN] = "";
	char    temp_buffer[BUFFER_SIZE];
	unsigned readlen;
	DWORD dwReadSize = 0;
	char   curPath[MAX_PATH];
	char quit[10] = "quit";
	//获得相对路径
	GetModuleFileName(NULL, (LPSTR)curPath, sizeof(curPath));
	strrchr(curPath, '\\')[0] = 0;

	ifstream infile;
	while (true)
	{
		memset(recvRFileName, 0, sizeof(recvRFileName));
		iResult = recv(clientsocket, recvRFileName, DEFAULT_BUFLEN, 0);
		if (SOCKET_ERROR == iResult)
		{
			printf("recvieve failed with error : %d\n", WSAGetLastError());
			closesocket(clientsocket);
			WSACleanup();
			return -1;
		}//if


		 //命令有get help exit -s(文件大小)

		 //help
		char dir[10] = "dir";
		char get[10] = "get";
		printf("传过来的数据:%s", recvRFileName);

		if (strcmp(recvRFileName, dir) == 0) {
			//返回命令
			system("cmd /c dir /a-d /b >allfiles.txt");
			printf("查看allfiles文件\n");

			//将打印出来的文件信息读取出来

			FILE *file;
			char buf[1024];
			int len = 0, i = 0;
			char *array[1024];
			char filedir[1024];//要传过去的文件目录

			file = fopen("allfiles.txt", "r");//打开TXST.TxT文件
			if (!file)return -1;
			while (fgets(buf, 1024, file))//读取TXT中字符
			{
				len = strlen(buf);
				array[i] = (char*)malloc(len + 1);
				if (!array[i])break;
				strcpy(array[i++], buf);
			}//while

			fclose(file);
			i--;
			while (i >= 0 && array[i])
			{
				printf("%s\n", array[i]);//打印test文档的字符

				iResult = send(clientsocket, array[i], strlen(array[i]), 0);
				if (iResult == SOCKET_ERROR) { printf("发送失败\n"); }

				free(array[i--]);

			}//while
			printf("发送结束quit\n");
			send(clientsocket, quit, strlen(quit), 0);
			continue;

		}//if

		 //完整文件名
		strcat(curPath, "\\");
		strcat(curPath, recvRFileName);
		strcpy(recvRFileName, curPath);



		//获取文件大小

		FILE * pFile;
		long size;
		pFile = fopen(recvRFileName, "rb");
		if (pFile == NULL)
			perror("Error opening file");
		else
		{
			fseek(pFile, 0, SEEK_END);   ///将文件指针移动文件结尾
			size = ftell(pFile); ///求出当前文件指针距离文件开始的字节数
			fclose(pFile);
			printf("FileSize : %ld bytes.\n", size);
			char a[30];
			//将size保存在字符串a中
			sprintf(a, "%d", size);
			send(clientsocket, a, 30, 0);
			printf("发送文件大小:%s", a);
		}


		printf("文件：%s\n", recvRFileName);
		infile.open(recvRFileName, ios::in | ios::binary);

		if (!infile)
		{    //未成功打开文件
			printf("Sorry, cannot open %s. Please try again.\r\n", recvRFileName);
			//break;
		}//if

		else
		{
			printf("The file %s is found,ready to transfer.\n", recvRFileName);
			printf("Transfering\r\n");



			while (!infile.eof())
			{    //循环读取文件并通过h_NewSocket发送到客户端
				 //printf(SendBuffer,"%s",temp_buffer); 
				infile.read(SendBuffer, BUFFER_SIZE);
				readlen = infile.gcount();
				send(clientsocket, SendBuffer, readlen, 0);
				//printf("."); 
			}
			infile.close();
			printf("Transfer completed... \r\n");


			//bytes = send(AcceptSocket, SendBuffer, strlen(SendBuffer), 0);

		}//else      




		 //如果没有数据发送
		iResult = shutdown(clientsocket, SD_BOTH);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed: %d\n", WSAGetLastError());
		}//if

		closesocket(clientsocket);
		return 0;


	}//while
}//main

//////////////////////////////////////////////////////////////////////////
//time_t start, end;
//double cost;
//time(&start);
//printf("开始计算传输时间\n");
////传输完成。输出时间
//time(&end);
//cost = difftime(end, start);
//printf("传输时间:%f\n", cost);