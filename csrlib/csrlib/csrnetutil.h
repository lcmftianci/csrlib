#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

unsigned _stdcall tcpclient(void* lpParam);

unsigned _stdcall tcpserver(void* lpParam);

DWORD WINAPI udpserver(LPWORD lpParam);

DWORD WINAPI udpclient(LPWORD lpParam);
