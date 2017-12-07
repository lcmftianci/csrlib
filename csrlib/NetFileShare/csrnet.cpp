
#include "stdafx.h"
#include "csrnet.h"
#include <iostream>

csrudpnet::csrudpnet()
{

}

bool csrudpnet::csrudpinitsocket(int port)
{
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
		return false;
	SOCKADDR_IN sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int recv = bind(m_socket, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
	if (recv == SOCKET_ERROR)
		return false;
	return true;
}

bool csrudpnet::csrudpsendmessage(DWORD dwIP, CString strSend, int port)
{
	SOCKADDR_IN addrTo;
	addrTo.sin_family = AF_INET;
	addrTo.sin_port = htons(port);
	addrTo.sin_addr.S_un.S_addr = htonl(dwIP);
	sendto(m_socket, strSend, strSend.GetLength() + 1, 0, (SOCKADDR*)&addrTo, sizeof(SOCKADDR));
	return true;
}

bool csrudpnet::csrudpgetmessage(RecvData *pRecvParam)
{
	HANDLE hHandle = CreateThread(NULL, 0, RecvProc, (LPVOID)pRecvParam, 0, NULL);
	CloseHandle(hHandle);
	return true;
}

DWORD WINAPI csrudpnet::RecvProc(LPVOID lpParameter)
{
	SOCKET sock = ((RecvData*)lpParameter)->sock;
	HWND hwnd = ((RecvData*)lpParameter)->hwnd;

	SOCKADDR_IN sockFrom;
	int len = sizeof(SOCKADDR);
	char recvBuf[200];
	char tempBuf[300];
	int retval;
	while(TRUE)
	{
		retval = recvfrom(sock, recvBuf, 200, 0, (SOCKADDR*)&sockFrom, &len);
		if(retval == SOCKET_ERROR)
			break;
		sprintf(tempBuf, "%s say: %s", inet_ntoa(sockFrom.sin_addr), recvBuf);
		::PostMessage(hwnd, WM_RECVDATA, 0, (LPARAM)tempBuf);
	}
	return NULL;
}

