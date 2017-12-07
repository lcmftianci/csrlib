#ifndef _CSR_NET_H_
#define _CSR_NET_H_

#include <winsock2.h>

#pragma commit(lib, "ws2_32.lib")

#define WM_RECVDATA WM_USER + 1

struct RecvData { SOCKET sock; HWND hwnd; };

class csrudpnet
{
public:
	csrudpnet();
	bool csrudpinitsocket(int port); //init socket

	bool csrudpsendmessage(DWORD dwIP, CString strSend, int port);        //send msg

	bool csrudpgetmessage(RecvData *pRecvParam);         //get msg

	static DWORD WINAPI RecvProc(LPVOID lpParameter);	// thread

	SOCKET GetSocket() { return m_socket; }

private:
	SOCKET m_socket;                 //socket
};

#endif // !_CSR_NET_H_

