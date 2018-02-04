#include "csrnetutil.h"

#include <Iphlpapi.h>  
#pragma comment(lib,"Iphlpapi.lib") //需要添加Iphlpapi.lib库  
  
bool CSocketComm::GetLocalAddress()  
{  
std::string strAddress;  
int nCardNo = 1;  
    //PIP_ADAPTER_INFO结构体指针存储本机网卡信息  
    PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();  
    //得到结构体大小,用于GetAdaptersInfo参数  
    unsigned long stSize = sizeof(IP_ADAPTER_INFO);  
    //调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量  
    int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);  
    //记录网卡数量  
    int netCardNum = 0;  
    //记录每张网卡上的IP地址数量  
    int IPnumPerNetCard = 0;  
    if (ERROR_BUFFER_OVERFLOW == nRel)  
    {  
        //如果函数返回的是ERROR_BUFFER_OVERFLOW  
        //则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小  
        //这也是说明为什么stSize既是一个输入量也是一个输出量  
        //释放原来的内存空间  
        delete pIpAdapterInfo;  
        //重新申请内存空间用来存储所有网卡信息  
        pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];  
        //再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量  
        nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);      
    }  
    if (ERROR_SUCCESS == nRel)  
    {  
        //输出网卡信息  
        //可能有多网卡,因此通过循环去判断  
        while (pIpAdapterInfo)  
        {  
            //可能网卡有多IP,因此通过循环去判断  
            IP_ADDR_STRING *pIpAddrString =&(pIpAdapterInfo->IpAddressList);  
            switch(pIpAdapterInfo->Type)  
            {  
            case MIB_IF_TYPE_OTHER:  
            case MIB_IF_TYPE_ETHERNET:  
            case MIB_IF_TYPE_TOKENRING:  
            case MIB_IF_TYPE_FDDI:  
            case MIB_IF_TYPE_PPP:  
            case MIB_IF_TYPE_LOOPBACK:  
            case MIB_IF_TYPE_SLIP:  
                {  
                    strAddress = pIpAddrString->IpAddress.String;  
               // 需要注意的是有时可能获取的IP地址是0.0.0.0，这时需要过滤掉  
                    if(std::string("0.0.0.0")==strAddress)  
                        break;  
           std::cout<<_T("第")<< nCardNo<<_T("张网卡的IP地址是")<< strAddress<<std::endl;  
                     nCardNo++;  
                     break;  
                }  
            default:  
                // 未知类型网卡就跳出  
                break;  
            }  
            pIpAdapterInfo = pIpAdapterInfo->Next;  
        }  
    }  
    //释放内存空间  
    if (pIpAdapterInfo)  
    {  
        delete pIpAdapterInfo;  
    }  
}  

std::string GetLocalAddress(std::string& strAddress)
{
	char strHost[1024] = { 0 };

	// get host name, if fail, SetLastError is called  
	if (SOCKET_ERROR != gethostname(strHost, sizeof(strHost)))
	{
		struct hostent* hp;
		hp = gethostbyname(strHost);
		if (hp != NULL && hp->h_addr_list[0] != NULL)
		{
			// IPv4: Address is four bytes (32-bit)  
			if (hp->h_length < 4)
				return false;

			// Convert address to . format  
			strHost[0] = 0;

			// IPv4: Create Address string  
			sprintf(strHost, "%u.%u.%u.%u",
				(UINT)(((PBYTE)hp->h_addr_list[0])[0]),
				(UINT)(((PBYTE)hp->h_addr_list[0])[1]),
				(UINT)(((PBYTE)hp->h_addr_list[0])[2]),
				(UINT)(((PBYTE)hp->h_addr_list[0])[3]));

			strAddress = strHost;
			return strAddress;
		}
	}
	else
		SetLastError(ERROR_INVALID_PARAMETER);

	return "";
}

unsigned _stdcall tcpclient(void* lpParam)
{
	WORD wVersionRequested;
	WSADATA wsadata;
	wVersionRequested = MAKEWORD(1, 1);
	int err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0)return 1;
	if (LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1)
	{
		WSACleanup();
		return 1;
	}

	//声明SOcket变量
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	//初始化地址和端口
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	while (1)
	{
		char recvBuf[100];
		char sendBuf[100];
		recv(sockClient, recvBuf, 100, 0);
		printf("%s:说：%s\n", "TCP服务器", recvBuf);
		printf("TCP Client please Input data:\n");
		gets_s(sendBuf);
		send(sockClient, sendBuf, strlen(sendBuf) + 1, 0);
	}
	closesocket(sockClient);
	WSACleanup();
}

unsigned _stdcall tcpserver(void* lpParam)
{
	WORD wVersionRequested;
	WSADATA wsadata;
	wVersionRequested = MAKEWORD(1, 1);
	int err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0)return 1;
	if(LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1)
	{
		WSACleanup();
		return 1;
	}

	//创建socket
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	//创建地址端口
	SOCKADDR_IN addrSrv;

	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	//绑定
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	//监听事件
	listen(sockSrv, 5);
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
	char sendBuf[100];
	char tempBuf[200];
	sprintf(tempBuf, "welcome %s to screen\n", inet_ntoa(addrClient.sin_addr));
	printf(tempBuf);
	//循环监听事件
	while (1)
	{
		printf("Tcp ServerPlease input Data\n");
		gets_s(sendBuf);
		send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);
		char recvBuf[100];
		recv(sockConn, recvBuf, 100, 0);
		printf("tcp客户端%s说：%s\n", inet_ntoa(addrClient.sin_addr), recvBuf);
	}
	closesocket(sockConn);
}

DWORD WINAPI udpserver(LPWORD lpParam)
{
	WORD wVersionRequested;
	WSADATA wsadata;
	wVersionRequested = MAKEWORD(1, 1);
	int err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0)return 1;
	if (LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1)
	{
		WSACleanup();
		return 1;
	}

	//创建socket
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);

	//创建地址端口
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	//绑定
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	char recvBuf[100];
	char sendBuf[100];
	char tempBuf[200];
	while (1)
	{
		recvfrom(sockSrv, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len);
		if ('q' == recvBuf[0])
		{
			sendto(sockSrv, "q", strlen("q") + 1, 0, (SOCKADDR*)&addrClient, len);
			printf("udp chat end\n");
			break;
		}
		sprintf(tempBuf, "UDP客户端%s say: %s", inet_ntoa(addrClient.sin_addr), recvBuf);
		printf("%s\n", tempBuf);
		printf("Udp Server please input data:\n");
		gets_s(sendBuf);
		sendto(sockSrv, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrClient, len);
	}
	closesocket(sockSrv);
	WSACleanup();
	return NULL;
}

DWORD WINAPI udpclient(LPWORD lpParam)
{
	WORD wVersionRequested;
	WSADATA wsadata;
	wVersionRequested = MAKEWORD(1, 1);
	int err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0)return 1;
	if (LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1)
	{
		WSACleanup();
		return 1;
	}

	//声明socket变量
	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, 0);
	//初始化地址接口
	SOCKADDR_IN addrClient;
	addrClient.sin_addr.S_un.S_addr = inet_addr("192.168.174.130");
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(6000);
	char recvBuf[100];
	char sendBuf[100];
	char tempBuf[200];
	int len = sizeof(SOCKADDR);
	while (1)
	{
		printf("Udp client please input data:\n");
		gets_s(sendBuf);
		sendto(sockClient, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrClient, len);
		recvfrom(sockClient, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len);
		if ('q' == recvBuf[0])
		{
			sendto(sockClient, "q", strlen("q") + 1, 0, (SOCKADDR*)&addrClient, len);
			printf("End");
			break;
		}
		sprintf(tempBuf, "Udp服务器%s say:%s\n", inet_ntoa(addrClient.sin_addr), recvBuf);
		printf("%s\n", tempBuf);
	}
	closesocket(sockClient);
	WSACleanup();
	return NULL;
}

