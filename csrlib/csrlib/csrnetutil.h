
#ifndef _CSR_NET_UTIL_H_
#define _CSR_NET_UTIL_H_

#include "comheader.h"

#pragma comment(lib, "ws2_32.lib")

unsigned _stdcall tcpclient(void* lpParam);

unsigned _stdcall tcpserver(void* lpParam);

DWORD WINAPI udpserver(LPWORD lpParam);

DWORD WINAPI udpclient(LPWORD lpParam);


//fileserver
//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>  
//#include <stdlib.h>  
//#include <string.h>  
//#include <WinSock2.h>  

//#define PORT 8087  
//#define SERVER_IP "127.0.0.1"  
//#define BUFFER_SIZE 1024  
//#define FILE_NAME_MAX_SIZE 512  
////#pragma comment(lib, "WS2_32")  
///*文件从服务器发往客户端，客户端中输入的文件地址应该在相应的系统内存在，在客户端中的文件名是存贮的文件名*/
//int fileserver()
//{
//	// 声明并初始化一个服务端(本地)的地址结构  
//	struct sockaddr_in server_addr;
//	server_addr.sin_family = AF_INET;
//	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
//	server_addr.sin_port = htons(PORT);
//
//
//	// 初始化socket dll  
//	WSADATA wsaData;
//	WORD socketVersion = MAKEWORD(2, 0);
//	if (WSAStartup(socketVersion, &wsaData) != 0)
//	{
//		printf("Init socket dll error!");
//		exit(1);
//	}
//
//	// 创建socket  
//	SOCKET m_Socket = socket(AF_INET, SOCK_STREAM, 0);
//	if (SOCKET_ERROR == m_Socket)
//	{
//		printf("Create Socket Error!");
//		exit(1);
//	}
//	//int opt = 1;
//	std::string opt = "1";
//	setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, /*&opt*/ opt.c_str(), sizeof(opt));
//	//绑定socket和服务端(本地)地址  
//	if (SOCKET_ERROR == bind(m_Socket, (LPSOCKADDR)&server_addr, sizeof(server_addr)))
//	{
//		printf("Server Bind Failed: %d", WSAGetLastError());
//		exit(1);
//	}
//
//	//监听  
//	if (SOCKET_ERROR == listen(m_Socket, 10))
//	{
//		printf("Server Listen Failed: %d", WSAGetLastError());
//		exit(1);
//	}
//
//
//	while (1)
//	{
//		printf("Listening To Client...\n");
//
//		struct sockaddr_in client_addr;
//		int client_addr_len = sizeof(client_addr);
//
//		SOCKET m_New_Socket = accept(m_Socket, (struct sockaddr *)&client_addr, &client_addr_len);
//		if (SOCKET_ERROR == m_New_Socket)
//		{
//			printf("Server Accept Failed: %d", WSAGetLastError());
//			break;
//		}
//
//		char buffer[BUFFER_SIZE];//="G:\Code\GTK+程序设计中文版.pdf"
//		memset(buffer, 0, BUFFER_SIZE);
//		if (recv(m_New_Socket, buffer, BUFFER_SIZE, 0) < 0)
//		{
//			printf("Server Receive Data Failed!");
//			break;
//		}
//
//		char file_name[FILE_NAME_MAX_SIZE + 1];
//		memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
//		strncpy(file_name, buffer, strlen(buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buffer));
//		file_name[strlen(buffer)] = '\0';
//		printf("%s\n", file_name);
//
//		FILE * fp = fopen(file_name, "rb"); //windows下是"rb",表示打开一个只读的二进制文件  
//		if (NULL == fp)
//		{
//			printf("File: %s Not Found\n", file_name);
//		}
//		else
//		{
//			memset(buffer, 0, BUFFER_SIZE);
//			int length = 0;
//
//			while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
//			{
//				if (send(m_New_Socket, buffer, length, 0) < 0)
//				{
//					printf("Send File: %s Failed\n", file_name);
//					break;
//				}
//				memset(buffer, 0, BUFFER_SIZE);
//			}
//
//			fclose(fp);
//			printf("File: %s Transfer Successful!\n", file_name);
//		}
//		closesocket(m_New_Socket);
//	}
//
//	closesocket(m_Socket);
//	//释放winsock库  
//	WSACleanup();
//	return 0;
//}
//
////#define _CRT_SECURE_NO_WARNINGS
////#include <stdio.h>  
////#include <stdlib.h>  
////#include <string.h>  
////#include <WinSock2.h>  
//
//#define PORT 8087  
//#define SERVER_IP "192.168.1.130"  
//#define BUFFER_SIZE 1024  
//#define FILE_NAME_MAX_SIZE 512  
//
//int fileclient()
//{
//	// 初始化socket dll  
//	WSADATA wsaData;
//	WORD socketVersion = MAKEWORD(2, 0);
//	if (WSAStartup(socketVersion, &wsaData) != 0)
//	{
//		printf("Init socket dll error!");
//		exit(1);
//	}
//
//	//创建socket  
//	SOCKET c_Socket = socket(AF_INET, SOCK_STREAM, 0);
//	if (SOCKET_ERROR == c_Socket)
//	{
//		printf("Create Socket Error!");
//		system("pause");
//		exit(1);
//	}
//
//	//指定服务端的地址  
//	struct sockaddr_in server_addr;
//	server_addr.sin_family = AF_INET;
//	server_addr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
//	server_addr.sin_port = htons(PORT);
//	//int opt = 1;
//	std::string opt = "1";
//	setsockopt(c_Socket, SOL_SOCKET, SO_REUSEADDR, /*&opt*/ opt.c_str(), sizeof(opt));
//	if (SOCKET_ERROR == connect(c_Socket, (LPSOCKADDR)&server_addr, sizeof(server_addr)))
//	{
//		printf("Can Not Connect To Server IP!\n");
//		system("pause");
//		exit(1);
//	}
//
//	//输入文件名  
//	char file_name[FILE_NAME_MAX_SIZE + 1];
//	memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
//	printf("Please Input File Name it will transfer to Server: ");
//	scanf("%s", &file_name);
//
//	char buffer[BUFFER_SIZE];
//	memset(buffer, 0, BUFFER_SIZE);
//	strncpy(buffer, file_name, strlen(file_name) > BUFFER_SIZE ? BUFFER_SIZE : strlen(file_name));
//
//	//向服务器发送文件名  
//	if (send(c_Socket, buffer, BUFFER_SIZE, 0) < 0)
//	{
//		printf("Send File Name Failed\n");
//		system("pause");
//		exit(1);
//	}
//
//	//打开文件，准备写入  
//	FILE * fp = fopen("client.txt", "wb");
//	//FILE * fp = fopen(file_name, "wb"); //windows下是"wb",表示打开一个只写的二进制文件  
//	if (NULL == fp)
//	{
//		printf("File: %s Can Not Open To Write\n", file_name);
//		system("pause");
//		exit(1);
//	}
//	else
//	{
//		memset(buffer, 0, BUFFER_SIZE);
//		int length = 0;
//		while ((length = recv(c_Socket, buffer, BUFFER_SIZE, 0)) > 0)
//		{
//			if (fwrite(buffer, sizeof(char), length, fp) < length)
//			{
//				printf("File: %s Write Failed\n", file_name);
//				break;
//			}
//			memset(buffer, 0, BUFFER_SIZE);
//		}
//
//		printf("Receive File: %s From Server Successful!\n", file_name);
//	}
//
//	fclose(fp);
//	closesocket(c_Socket);
//
//	//释放winsock库  
//	WSACleanup();
//
//	system("pause");
//	return 0;
//}


//#include <sys/uio.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <unistd.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <fcntl.h>
//#include <errno.h>
//#include <setjmp.h>
//#include <pwd.h>
//#include <grp.h>
//#include <limits.h>
//#include <shadow.h>
//#include <sys/fsuid.h>
//#include<netdb.h>
//#include<time.h>
//#include<sys/time.h>
//#include <locale.h>
//#include <sys/utsname.h>
//#include <dirent.h>
//#include<limits.h>
//#include<ctype.h>
//#include <strings.h>
//#include<sys/socket.h>
//#include<arpa/inet.h>
//#include<netinet/in.h>
//#include<sys/un.h>
//void err(const char *msg)
//{
//	perror("msg:");
//	exit(1);
//}
//void strerr(const char *msg)
//{
//	fprintf(stderr, "%s %s\n", msg, strerror(errno));
//	exit(1);
//}
//
//
//#define BUFF_SIZE 1024
//#define PORT 8087
//#define FILE_NAME_SIZE 512
//
//int linuxfileserver()
//{
//
//	struct sockaddr_in s_addr, c_addr;
//	int s_sockfd, c_sockfd;
//	char buf[BUFF_SIZE];
//	int c_addr_len = sizeof(struct sockaddr_in);
//	int s_addr_len = sizeof(struct sockaddr_in);
//	s_sockfd = socket(AF_INET, SOCK_STREAM, 0);
//	if (s_sockfd == -1)
//		strerr("socket error");
//	memset(&s_addr, 0, s_addr_len);
//	s_addr.sin_family = AF_INET;
//	s_addr.sin_port = htons(PORT);
//	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//	int opt = 1;
//	setsockopt(s_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
//	if (bind(s_sockfd, (struct sockaddr*)&s_addr, s_addr_len) == -1)
//		strerr("bind error");
//	if (listen(s_sockfd, 5) == -1)
//		strerr("listen error");
//
//	while (1)
//	{
//		printf("listening to connect........\n");
//		c_sockfd = accept(s_sockfd, (struct sockaddr*)&c_addr, &c_addr_len);
//		if (c_sockfd == -1)
//			strerr("accept error");
//
//		memset(buf, 0, BUFF_SIZE);
//		if (recv(c_sockfd, buf, BUFF_SIZE, 0) == -1)
//			strerr("recv error");
//		char filename[FILE_NAME_SIZE + 1];
//		memset(filename, 0, FILE_NAME_SIZE + 1);
//		strncpy(filename, buf, strlen(buf) > FILE_NAME_SIZE ? FILE_NAME_SIZE : strlen(buf));
//		filename[strlen(buf)] = '\0';
//		printf("%s\n", filename);
//
//		FILE *fp = fopen(filename, "rb");
//		if (NULL == fp)
//			strerr("fopen error");
//		else
//		{
//			memset(buf, 0, BUFF_SIZE);
//			int length = 0;
//			while ((length = fread(buf, sizeof(char), BUFF_SIZE, fp)) > 0)
//			{
//				if (send(c_sockfd, buf, length, 0) == -1)
//					strerr("send error");
//				memset(buf, 0, BUFF_SIZE);
//			}
//			fclose(fp);
//			printf("FILE %s transfer successful\n", filename);
//		}//end else    
//		close(c_sockfd);
//	}//end while(1)
//
//	close(s_sockfd);
//	return 0;
//}
//
//
//
//#define PORT 8087
////#define SERVER_IP "127.0.0.1"
//#define BUFF_SIZE 1024  
//#define FILE_NAME_MAX_SIZE 512  
//
//int linuxfileclient()
//{
//	if (argc != 3)
//	{
//		printf("usage : %s ip filename and format\n", argv[0]);
//		exit(1);
//	}
//	int sockfd;
//	struct sockaddr_in c_addr;
//	int c_addr_len = sizeof(struct sockaddr_in);
//	char buffer[BUFF_SIZE];
//	sockfd = socket(AF_INET, SOCK_STREAM, 0);
//	if (-1 == sockfd)
//		strerr("socket error");
//	memset(&c_addr, 0, c_addr_len);
//	c_addr.sin_family = AF_INET;
//	c_addr.sin_addr.s_addr = inet_addr(argv[1]);
//	c_addr.sin_port = htons(PORT);
//	int opt = 1;
//	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
//	if (connect(sockfd, (struct sockaddr*)&c_addr, c_addr_len) == -1)
//		strerr("connect error");
//	char file_name[FILE_NAME_MAX_SIZE + 1];
//	memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
//	printf("Please Input File Name it will transfer to Server: ");
//	scanf("%s", &file_name);
//	memset(buffer, 0, BUFF_SIZE);
//	strncpy(buffer, file_name, strlen(file_name) > BUFF_SIZE ? BUFF_SIZE : strlen(file_name));
//	if (send(sockfd, buffer, BUFF_SIZE, 0) == -1)
//		strerr("send error");
//	FILE *fp = fopen(argv[2], "wb");
//	if (fp == NULL)
//		strerr("fopen error");
//	else
//	{
//		memset(buffer, 0, BUFF_SIZE);
//		int length = 0;
//		while ((length = recv(sockfd, buffer, BUFF_SIZE, 0)) > 0)
//		{
//			if (fwrite(buffer, sizeof(char), length, fp) < length)
//				strerr("fwrite error");
//			memset(buffer, 0, BUFF_SIZE);
//		}
//		printf("receive file : %s from server successful\n", file_name);
//	}
//	fclose(fp);
//	close(sockfd);
//	return 0;
//}

#endif