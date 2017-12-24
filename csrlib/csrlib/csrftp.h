#ifndef _CSR_FTP_H_
#define _CSR_FTP_H_

#include "comheader.h"

using namespace std;

#pragma commit(lib, "ws2_32.lib")

#if 0

int FtpInit(char* Host, char* Account, char* Passwd)
{
	short       shPortNumber;
	long        lAddr;
	char 		RecvBuf[1024];
	char 		SendBuf[1024];
	int         RecvLen, SendLen;
	char*		HostName;
	char*		rcvtime;
	char*       keepalive;
	SOCKET      hClient;
	shPortNumber = htons(21);
	lAddr = inet_addr(Host);
	memset(HostName, 0, 16);
	memcpy(HostName, Host, strlen(Host));
	hClient = socket(AF_INET, SOCK_STREAM, 0);
	if (hClient < 0)
	{
		return -1;
	}
	if (setsockopt(hClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&rcvtime, sizeof(int)))
	{
		closesocket(hClient);
		return -1;
	}

	if (setsockopt(hClient, SOL_SOCKET, SO_KEEPALIVE, (char *)&keepalive, sizeof(int)))
	{
		closesocket(hClient);
		return -1;
	}
	SOCKADDR_IN SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = lAddr;
	SockAddr.sin_port = shPortNumber;

	if (connect(hClient, (const struct sockaddr *)&SockAddr, sizeof(SockAddr)) < 0)
	{
		closesocket(hClient);
		return -1;
	}
	memset(RecvBuf, 0, 1024);
	if ((RecvLen = FtpMatchReceive(hClient, RecvBuf, "220 ", 1024)) <= 0)
	{
		closesocket(hClient);
		return -1;
	}
	if (SendLen = GetFtpSendBuf("USER", Account, SendBuf, 1024) <= 0)
	{
		closesocket(hClient);
		return -1;
	}
	if (SendFTPCommand(hClient, SendBuf) != 331)
	{
		closesocket(hClient);
		return -1;
	}
	if (SendLen = GetFtpSendBuf("PASS", Passwd, SendBuf, 1024) <= 0)
	{
		closesocket(hClient);
		return -1;
	}
	if (SendFTPCommand(hClient, SendBuf) != 230)
	{
		closesocket(hClient);
		return -1;
	}

}
int FtpGetFile(char * FileName)
{
	int     hListenSocket;
	int     hDataSocket;
	int     RetWriteFile;
	int     ReturnCode;
	char    RemoteFileName[256];
	char    RemoteFile[256];
	char    ExecCommand[256];
	char *  pcSubDir;
	SOCKET  hClient;
	memset(RemoteFileName, 0, 256);
	memset(RemoteFile, 0, 256);
	memset(ExecCommand, 0, 256);

	strcpy(RemoteFileName, FileName);
	memcpy(ExecCommand, "CWD ", 4);

	pcSubDir = strrchr(RemoteFileName, '\\');
	if (pcSubDir != NULL)
	{
		strncat(ExecCommand, RemoteFileName, RemoteFileName - pcSubDir);
		if (SendFTPCommand(hClient, ExecCommand) != 250)
		{
			closesocket(hClient);
			return -1;
		}
		strcpy(RemoteFile, pcSubDir);
	}
	else
	{
		strcpy(RemoteFile, RemoteFileName);
	}

	if ((hListenSocket = CreatListenSocket(hClient)) < 0)
	{
		closesocket(hClient);
		return -1;
	}
	if (RequestDataConnection(hClient, hListenSocket) < 0)
	{
		closesocket(hClient);
		return -1;
	}
	memset(ExecCommand, 0, 256);
	memcpy(ExecCommand, "RETR ", 5);
	strcat(ExecCommand, RemoteFile);
	printf("The FileName=%s\n", RemoteFile);
	strcat(ExecCommand, "\r\n");

	ReturnCode = SendFTPCommand(hClient, ExecCommand);
	if (ReturnCode != 150)
	{
		closesocket(hClient);
		return -1;
	}
	if ((hDataSocket = AcceptDataConnection(hListenSocket)) < 0)
	{
		closesocket(hClient);
		return -1;
	}
	if ((RetWriteFile = WriteFile(hDataSocket, RemoteFile)) < 0)
	{
		closesocket(hDataSocket);
		closesocket(hClient);
		return -1;
	}
}

#endif

#endif // _CSR_FTP_H_