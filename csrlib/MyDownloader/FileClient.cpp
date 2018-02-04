#include<winsock2.h>
#include<windows.h>
#include<Ws2tcpip.h>
#include<stdio.h>
#include<string>
#include<iostream>
#include<fstream>

using namespace std;

#pragma comment(lib,"Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   5150
#define BUFFER_SIZE 4096

int main(int argc, TCHAR* argv[])
{
	int         iResult, rec;
	WSADATA     wsaData;
	HANDLE hout;
	ofstream    outfile;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct sockaddr_in clientService;
	char    sendbuf[DEFAULT_BUFLEN] = "";
	char    szBuffer[BUFFER_SIZE];
	BOOL    issend = true;

	//初始化Windows Sockets DLL
	if (iResult = WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		printf("WSAStartup failed with error : %d\n", iResult);
		return -1;
	}
	//创建socket
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("socket failed with error : %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	////////////////////

	char addr[20];
	int prot;
	printf("现在运行的是客户端！\n");
	printf("**************************************************************\n");
	printf("请输入服务器的IP地址:");
	scanf("%s", addr);
	printf("请输入服务器的端口:");
	scanf("%d", &prot);



	///////////////////


	//设置目的网络地址并连接服务器
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(addr);
	clientService.sin_port = htons(prot);
	iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR)
	{
		printf("connect failed with error : %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return -1;
	}

	printf("connect with server successfully!!!\n");
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hout,
		FOREGROUND_GREEN);
	cout << "输入命令:******************* help 获取命令提示**********************:\n";
	while (true)
	{



		//输入命令：
		char cmd[20];
		char help[10] = "help";
		char exit[10] = "exit";
		char get[10] = "get";
		char dir[10] = "dir";
		char quit[10] = "quit";
		scanf("%s", &cmd);
		printf("输入的命令是%s\n", cmd);
		if (strcmp(cmd, help) == 0) {


			printf("命令参数:\n help----------获取命令帮助\n dir----------获取文件目录\n get----------下载文件\n exit----------退出\n");
			continue;
		}
		//  列目录
		if (strcmp(cmd, dir) == 0) {

			iResult = send(ConnectSocket, dir, strlen(dir), 0);

			if (SOCKET_ERROR == iResult)
			{
				printf("send failed with error : %d\n", WSAGetLastError());
				closesocket(ConnectSocket);
				WSACleanup();
				return -1;
			}
			//接收传过来的目录

			SetConsoleTextAttribute(hout,
				FOREGROUND_GREEN |
				FOREGROUND_BLUE);
			printf("**********************文件目录**********************\n");
			char dirs[30];


			while (true) {
				ZeroMemory(dirs, 30);
				iResult = recv(ConnectSocket, dirs, 30, 0);
				//数据发送完成就跳出循环，结束时，服务器发过来quit
				if (strcmp(dirs, quit) == 0) { break; }

				else
					printf("%s\n", dirs);


				if (SOCKET_ERROR == iResult)
				{
					printf("recvieve failed with error : %d\n", WSAGetLastError());
					closesocket(ConnectSocket);
					WSACleanup();
					return -1;
				}//if

				 //接收完再次返回
				 //  continue;
			}//while
			continue;
		}//if



		if (strcmp(cmd, exit) == 0) { break; }

		if ((strcmp(cmd, dir) != 0) && (strcmp(cmd, exit) != 0) && (strcmp(cmd, help) != 0) && (strcmp(cmd, get) != 0)) {
			printf("命令参数:\n help----------获取命令帮助\n dir----------获取文件目录\n get----------下载文件\n exit----------退出\n");
			continue;

		}


		if (strcmp(cmd, get) == 0) {
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			printf("您输入的命令是get，下载文件，请依次输入要下载的文件名、保存本地文件名\n");





			//文件下载   参数get后使用
			char  str[20];
			//文件名
			scanf("%s", &str);
			ZeroMemory(sendbuf, DEFAULT_BUFLEN);
			strcpy(sendbuf, str);
			if (strcmp(str, quit) == 0) {



				printf("您输入了exit，退出get\n");
				continue;
			}

			SetConsoleTextAttribute(hout,
				FOREGROUND_RED |
				FOREGROUND_GREEN |
				FOREGROUND_BLUE);
			iResult = send(ConnectSocket, sendbuf, strlen(sendbuf), 0);
			if (SOCKET_ERROR == iResult)
			{
				printf("send failed with error : %d\n", WSAGetLastError());
				closesocket(ConnectSocket);
				WSACleanup();
				return -1;
			}

			//接收文件总大小
			int size;
			char sizestr[30];
			recv(ConnectSocket, sizestr, 30, 0);
			size = strtol(sizestr, NULL, 10);

			SetConsoleTextAttribute(hout,
				FOREGROUND_BLUE |
				FOREGROUND_RED);

			printf("总的数据大小：%ld\n", size);

			//cout<<"输入存放文件（路径+文件名）:\n";
			char    CFileName[DEFAULT_BUFLEN] = "";
			scanf("%s", &CFileName);


			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

			printf("保存文件：%s\n", CFileName);

			outfile.open(CFileName, ios::out | ios::binary);

			if (!outfile)   //打开文件准备写入
			{
				printf("Error of opening file !");

				iResult = shutdown(ConnectSocket, SD_SEND);
				iResult = closesocket(ConnectSocket);
				WSACleanup();
				return -1;
			}


			COORD   coord;

			CONSOLE_SCREEN_BUFFER_INFO csbi;

			hout = GetStdHandle(STD_OUTPUT_HANDLE);
			GetConsoleScreenBufferInfo(hout, &csbi);
			coord.X = csbi.dwCursorPosition.X;
			coord.Y = csbi.dwCursorPosition.Y;

			float cnt = 0;

			while (issend)
			{
				//读取流       

				rec = recv(ConnectSocket, szBuffer, BUFFER_SIZE, 0);
				cnt += rec;
				if (rec == 0)
				{
					SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
					printf("Download completed ! \n");
					break;
				}
				else if (rec == SOCKET_ERROR)
				{
					printf("Receive function failed with error : %d\n ", WSAGetLastError());
					break;
				}

				SetConsoleCursorPosition(hout, coord);
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);

				printf("Received %f%% bytes \n", cnt / size * 100);
				outfile.write(szBuffer, rec);

			}

			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
			printf("Transfer is completed!\n");
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

			outfile.close();

		}

		// 若没有数据要发送，单向关闭连接
		iResult = shutdown(ConnectSocket, SD_SEND);
		// Step 6: 关闭连接
		iResult = closesocket(ConnectSocket);

		WSACleanup();
		return 0;

	}
}
