#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#pragma comment(lib, "libzmq.lib")

#ifdef ZMQ
#include <zmq.h>
#include <zmq_utils.h>
#endif

int main(int argc, char** argv)
{
	//printf("mnmsg %s-%s-%s", __FUNCTION__, __LINE__, __FILE__);
	printf("mnmsg\n");

	int a, b, c;
	zmq_version(&a, &b, &c);

	std::cout << a << "--" << b << "--" << c << std::endl;

	void* pCtx = NULL;
	void* pSock = NULL;
	const char* pAddr = "tcp://*:8866";

	//创建context,并在context上创建zmq的socket
	if ((pCtx = zmq_ctx_new()) == NULL)
		return 0;

	//创建zmq socket,socket的有六个属性，此处使用dealer方式
	if ((pSock = zmq_socket(pCtx, ZMQ_DEALER)) == NULL)
	{
		zmq_ctx_destroy(pCtx);
		return 0;
	}

	int iRecvTimeOut = 5000;

	//设置zmq接受超时为5秒
	if (zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &iRecvTimeOut, sizeof(iRecvTimeOut)) < 0)
	{
		zmq_close(pSock);
		zmq_ctx_destroy(pCtx);
		return 0;
	}

	//绑定地址
	if (zmq_bind(pSock, pAddr) < 0)
	{
		zmq_close(pSock);
		zmq_ctx_destroy(pCtx);
		return 0;
	}

	std::cout << "bind at:" << pAddr << std::endl;

	while (1)
	{
		char szMsg[1024] = { 0 };
		printf("waiting...\n");
		errno = 0;
		//循环接受消息
		if (zmq_recv(pSock, szMsg, sizeof(szMsg), 0) < 0)
		{
			printf("errno = %s\n", zmq_strerror(errno));
			continue;
		}
		printf("recv message:%s\n", szMsg);
	}
	return 0;
}