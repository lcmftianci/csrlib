//服务器的代码文件有  
/* 
message.h 
source.h 
source.cpp 
server.h 
server.cpp 
*/  
//客户端的代码文件有  
/* 
message.h   和服务器端一样 
client.h 
client.cpp 
*/  
//message.h  
#pragma once  
#include<iostream>  
using namespace std;  
#define MAX_PACK_SIZE  10240   //数据包的长度  
#define MAX_FILE_NAME_LENGTH 256  //文件名的长度  
#define INVALID_MSG -1       //无效的消息  
#define MSG_FILE_LENGTH 1    //文件长度  
#define MSG_FILE_NAME 2    //文件名  
#define MSG_FILE   4     //文件内容  
#define MSG_READY   3     //准备好消息  
#define MSG_SEND_FILE 5  //发送文件  
#define MSG_DOWNLOAD_FILE 6  //下载文件  
#define MSG_COMPLETE 7     //完成信息  
class Message  
{  
public:  
    struct MsgHead     //头消息  
    {  
        int msgId;    //消息标识  
        MsgHead(int msg=INVALID_MSG):msgId(msg){};  
    };  
    struct MsgFileLength :public MsgHead  
    {  
        _int64 fileLength;   //文件长度  
        MsgFileLength():MsgHead(MSG_FILE_LENGTH){}  
    };  
    struct MsgFileName:public MsgHead  
    {  
        char fileName[MAX_FILE_NAME_LENGTH];  
        MsgFileName():MsgHead(MSG_FILE_NAME){}  
    };  
    struct MsgFile:public MsgHead  
    {  
        MsgFile():MsgHead(MSG_FILE){}  
    };  
    struct MsgReady:public MsgHead        //准备好消息  
    {  
        MsgReady():MsgHead(MSG_READY){}  
    };  
    struct MsgSendFile:public MsgHead  //发送文件消息  
    {  
        MsgSendFile():MsgHead(MSG_SEND_FILE){}  
    };  
    struct MsgDownLoadFile:public MsgHead     //下载文件消息  
    {  
        MsgDownLoadFile():MsgHead(MSG_DOWNLOAD_FILE){}  
    };  
    struct MsgComplete:public MsgHead  
    {  
        MsgComplete():MsgHead(MSG_COMPLETE){}  
    };  
};  
//source.h   获取指定文件加下的符合要求的文件  
#pragma once  
#include<iostream>  
#include<fstream>  
#include<vector>  
#include<io.h>  
#include<string>  
using namespace std;  
class Source  
{  
public:  
    vector<string> catalogInfo;  
    void GetFiles(string path,string ext,vector<string> &files);//获取文件  
};  
//server.h  
#pragma once  
#include<iostream>  
#include<WinSock2.h>  
#include"message.h"  
#pragma comment(lib,"Ws2_32.lib")  
#define PORT 10000  
using namespace std;  
class Server  
{  
public:  
    SOCKET sd;  
    _int64 fileLength;  
    char fileName[MAX_FILE_NAME_LENGTH];  
    bool InitSock();    //初始winsocket  
    SOCKET BindListen();  //绑定监听套接字  
    SOCKET AcceptConnection(SOCKET sd);  //接收客户端  
    bool ProcessConnection(SOCKET sd);  //传送数据  
    bool ReceiveFile(SOCKET sd);     //接收文件内容  
    bool RecvFileName(SOCKET sd);     //接收文件名  
    bool GetAndSendFileLength(SOCKET sd);    //获取文件长度  
    bool SendFileName(SOCKET sd);    //发送文件名  
    bool SendFile(SOCKET sd);      //发送文件  
    void CloseSocket();   //关闭套接字  
};  
//source.cpp  
#pragma once  
#include<iostream>  
#include<vector>  
#include<io.h>  
#include<string>  
#include"source.h"  
using namespace std;  
void Source::GetFiles(string path,string ext,vector<string> &files)  
{  
    long hFile=0;    //文件句柄  
    _finddata_t fileInfo; //文件信息  
    string pathName;  
    if((hFile=_findfirst(pathName.assign(path).append("\\*").c_str(),&fileInfo))!=-1) //判断路径是否有效并获取第一个文件  
    {  
        do  
        {  
            if(fileInfo.attrib & _A_SUBDIR)   //如果是子文件夹  
            {  
                if(strcmp(fileInfo.name,".")!=0 && strcmp(fileInfo.name,"..")!=0)  
                {  
                    GetFiles(pathName.assign(path).append("\\").append(fileInfo.name),ext,files);  
                }  
            }  
            else  
            {  
                string filePath;  
                filePath=pathName.assign(path).append("\\").append(fileInfo.name);  
                char fileDrive[_MAX_DRIVE];  
                char fileDir[_MAX_DIR];  
                char fileName[_MAX_FNAME];  
                char fileExt[_MAX_EXT];  
                _splitpath(filePath.c_str(),fileDrive,fileDir,fileName,fileExt);  //分解路径获取磁盘区路径文件名后缀  
                                if(strcmp(fileExt,ext.c_str())==0)  
                {  
                    files.push_back(filePath);  
                }  
            }  
        }while(_findnext(hFile,&fileInfo)==0);  
        _findclose(hFile);  
    }  
}  
//server.cpp  
#pragma once  
#include<iostream>  
#include<string>  
#include<fstream>  
#include<WinSock2.h>  
#include"message.h"  
#include"server.h"  
#include"source.h"  
using namespace std;  
int main()  
{  
    Server server;  
    if(!server.InitSock())   //初始化失败  
    {  
        cout<<"初始化失败"<<endl;  
    }  
    server.sd=server.BindListen();  
    if(server.sd==INVALID_SOCKET)  
    {  
        return -1;  
    }  
    SOCKET sdListen=server.AcceptConnection(server.sd);  
    if(sdListen==INVALID_SOCKET)  
    {  
        return -1;  
    }  
    while(server.ProcessConnection(sdListen))  
    {  
    }  
    server.CloseSocket();  
    return 0;  
}  
bool Server::InitSock()   //初始化winsocket  
{  
    WSADATA wsData;  
    WORD wr=MAKEWORD(2,2);  
    if(WSAStartup(wr,&wsData)==0)  
    {  
        return true;  
    }  
    return false;  
}  
SOCKET Server::BindListen()  //绑定套接字  
{  
    SOCKET sd=socket(AF_INET,SOCK_STREAM,0);  
    if(sd==INVALID_SOCKET)  
    {  
        cout<<"创建套接字失败"<<WSAGetLastError()<<endl;  
        return INVALID_SOCKET;  
    }  
    sockaddr_in sListen;  
    sListen.sin_family=AF_INET;  
    sListen.sin_addr.s_addr=htonl(INADDR_ANY);  
    sListen.sin_port=htons(PORT);  
    int nSize;  
    nSize=sizeof(sockaddr_in);  
    if(bind(sd,(sockaddr*)&sListen,nSize)==SOCKET_ERROR)  
    {  
        closesocket(sd);  
        cout<<"绑定失败"<<WSAGetLastError()<<endl;  
        return INVALID_SOCKET;  
    }  
    if(listen(sd,10)==SOCKET_ERROR)  
    {  
        closesocket(sd);  
        cout<<"监听失败"<<WSAGetLastError()<<endl;  
        return INVALID_SOCKET;  
    }  
    return sd;  
      
}  
SOCKET Server::AcceptConnection(SOCKET sd)    //接收客户端  
{  
    sockaddr_in saRemote;  
    int nSize=sizeof(sockaddr_in);  
    SOCKET sdListen=accept(sd,(sockaddr*)&saRemote,&nSize);  
    if(sdListen==INVALID_SOCKET)  
    {  
        cout<<"接收客户端失败"<<WSAGetLastError()<<endl;  
        return INVALID_SOCKET;  
    }  
    return sdListen;  
}  
bool Server::ReceiveFile(SOCKET sd)  
{  
    char buff[MAX_PACK_SIZE];  
    FILE *pFile;  
    pFile=fopen(fileName,"a+b");  
    _int64 i=0;  
    while(i+1<fileLength)  
    {  
        int nRecv=recv(sd,buff,MAX_PACK_SIZE,0);  
        if(nRecv==SOCKET_ERROR)  
        {  
           return false;  
        }  
        fwrite(buff,sizeof(char),nRecv,pFile);  
        i+=nRecv;  
        memset(buff,0,sizeof(char)*MAX_PACK_SIZE);  
    }  
    fclose(pFile);  
    return true;  
}  
void Server::CloseSocket()  
{  
    closesocket(sd);  
    WSACleanup();  
}  
bool Server::ProcessConnection(SOCKET sd)  
{  
    //----------------------------------------------  
    //可以将下面代码看做设置系统缓冲区  
    int nRecvBuf=1024000;//设置为1000K  
    setsockopt(sd,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));  
      //发送缓冲区  
    int nSendBuf=1024000;//设置为1000K  
    setsockopt(sd,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));  
    //-------------------------------------------------------------  
    char buff[MAX_PACK_SIZE];  
    Message::MsgHead *msgHead;  
    if(recv(sd,buff,MAX_PACK_SIZE,0)==SOCKET_ERROR)  
    {  
        cout<<"接收失败"<<WSAGetLastError()<<endl;  
        return false;  
    }  
    msgHead=(Message::MsgHead *)&buff;  
    switch(msgHead->msgId)  
    {  
    case MSG_SEND_FILE:         //客户端向服务器发送文件  
        cout<<"客户端请求向服务器发送文件"<<endl;  
        break;  
    case MSG_DOWNLOAD_FILE:      //客户端从服务器下载文件  
        {  
            cout<<"客户端请求从服务器下载文件"<<endl;  
            Source source;      //用来获取指定文件加下的后缀为jpg文件  
            string sPath="E:\\图片";  
            string sExt=".jpg";  
            source.GetFiles(sPath,sExt,source.catalogInfo);  
            int nSize;  
            nSize=source.catalogInfo.size();  
            cout<<"搜集到"<<nSize<<"个文件"<<endl;  
            char buff[MAX_PACK_SIZE];  
            for(int i=0;i<nSize;i++)   //将目录信息发送到客户端  
            {  
                strcpy(buff,source.catalogInfo[i].c_str());  
                //cout<<source.catalogInfo[i]<<endl;  
                if(send(sd,buff,MAX_PACK_SIZE,0)==SOCKET_ERROR)  
                {  
                    cout<<"发送目录信息失败"<<WSAGetLastError()<<endl;  
                    return false;  
                }  
                Sleep(10);   //睡眠10ms让接收端将信息取走  
            }  
            Message::MsgComplete msgComplete;  
            if(send(sd,(char *)&msgComplete,sizeof(Message::MsgComplete),0)==SOCKET_ERROR)  
            {  
                cout<<"发送完成信息失败"<<WSAGetLastError()<<endl;  
                return false;  
            }  
            Sleep(10);  
            if(!RecvFileName(sd))  
            {  
                return false;  
            }  
            Sleep(10);  
            if(!GetAndSendFileLength(sd))  
            {  
                return false;  
            }  
            Sleep(10);  
            if(!SendFileName(sd))  
            {  
                return false;  
            }  
            Sleep(10);  
            if(!SendFile(sd))  
            {  
                return false;  
            }  
        }  
        break;  
    case MSG_FILE_NAME:      //发送的文件名  
        {  
            Message::MsgFileName *msgFileName;  
            msgFileName=(Message::MsgFileName*)msgHead;  
            strcpy(fileName,msgFileName->fileName);  
            cout<<"收到发送来的文件名"<<fileName<<endl;  
        }  
        break;  
    case MSG_FILE_LENGTH:    //发送的文件长度  
        {  
            Message::MsgFileLength *msgFileLength;  
            msgFileLength=(Message::MsgFileLength *)msgHead;  
            fileLength=msgFileLength->fileLength;  
            cout<<"接收到文件的长度为"<<fileLength<<endl;  
        }  
        break;  
    case MSG_FILE:     //发送的文件内容  
        {  
            cout<<"开始接收文件"<<endl;  
            if(!ReceiveFile(sd))  
            {  
                cout<<"接收文件失败"<<endl;  
                return false;  
            }  
        }  
        break;  
    default:  
        cout<<"非标准消息"<<endl;  
        return false;  
    }  
    return true;  
}  
bool Server::RecvFileName(SOCKET sd)  
{  
    //memset(fileName,0,sizeof(char)*MAX_FILE_NAME_LENGTH);  //清空  
    char buff[MAX_PACK_SIZE];  
    Message::MsgFileName *msgFileName;  
    if(recv(sd,buff,MAX_PACK_SIZE,0)==SOCKET_ERROR)  
    {  
        cout<<"接收文件名失败"<<WSAGetLastError()<<endl;  
        return false;  
    }  
    msgFileName=(Message::MsgFileName *)buff;  
    strcpy(fileName,msgFileName->fileName);  
    cout<<"接收的文件名为"<<fileName<<endl;  
    return true;  
}  
bool Server::GetAndSendFileLength(SOCKET sd)       //获取客户端要下载的文件长度  
{  
    Message::MsgFileLength msgFileLength;  
    FILE *pFile;  
    pFile=fopen(fileName,"r+b");  
    if(pFile==NULL)  
    {  
        cout<<"打开文件失败"<<endl;  
        return false;  
    }  
    fseek(pFile,0,SEEK_END);  
    fileLength=_ftelli64(pFile);  
    fclose(pFile);  
    msgFileLength.fileLength=fileLength;  
    if(send(sd,(char*)&msgFileLength,sizeof(Message::MsgFileLength),0)==SOCKET_ERROR)  
    {  
        cout<<"发送文件长度失败"<<WSAGetLastError()<<endl;  
        return false;  
    }  
    return true;  
}  
bool Server::SendFileName(SOCKET sd)       //向客户端发送文件名  
{  
    Message::MsgFileName msgFileName;  
    char fileDrive[_MAX_DRIVE];  
    char fileDir[_MAX_DIR];  
    char Name[_MAX_FNAME];  
    char fileExt[_MAX_EXT];  
    _splitpath(fileName,fileDrive,fileDir,Name,fileExt);  
    strcat(Name,fileExt);  
    strcpy(msgFileName.fileName,Name);  
    cout<<"要发送的文件名为"<<Name<<endl;  
    if(send(sd,(char *)&msgFileName,sizeof(Message::MsgFileName),0)==SOCKET_ERROR)  
    {  
        cout<<"发送文件名出错"<<WSAGetLastError()<<endl;  
        return false;  
    }  
    return true;  
}  
bool Server::SendFile(SOCKET sd)  //向客户端发送文件  
{  
    cout<<"进入到发送文件内容"<<endl;  
    cout<<"要发送的文件为"<<fileName<<endl;  
    FILE *pFile;  
    pFile=fopen(fileName,"r+b");  
    fseek(pFile,0,SEEK_SET);   //定位到文件首位置  
    _int64 i=0;  
    char buff[MAX_PACK_SIZE];  
    cout<<"要发送的文件长度为"<<fileLength<<endl;  
    while(i<fileLength)  
    {  
        int nSize;  
        if(i+MAX_PACK_SIZE>fileLength)  
        {  
            nSize=(int)(fileLength-i);  
        }  
        else  
        {  
            nSize=MAX_PACK_SIZE-1;  
        }  
        fread(buff,sizeof(char),nSize,pFile);  
        int nSend;  
        nSend=send(sd,buff,nSize,0);  
        if(nSend==SOCKET_ERROR)  
        {  
            cout<<"发送失败"<<WSAGetLastError()<<endl;  
            return false;  
        }  
        i+=nSend;  
        fseek(pFile,-(nSize-nSend),SEEK_CUR);  //定位到实际已发送到的位置  
        memset(buff,0,sizeof(char)*MAX_PACK_SIZE); //将buff清空  
    }  
    fclose(pFile);  
    return true;  
}  
//client.h  
#pragma once  
#include<iostream>  
#include<fstream>  
#include<vector>  
#include<WinSock2.h>  
#pragma comment(lib,"Ws2_32.lib")  
using namespace std;  
#define SERVER_IP "127.0.0.1"  
#define PORT  10000  
class Client  
{  
public:  
    _int64 nFileLength;  
    char fileName[_MAX_FNAME+_MAX_EXT];  
    SOCKET sd;  
    bool InitSock();   //初始化winsock  
    u_long ResolveAdress(char *serverIp);    //解析服务器地址  
    SOCKET ConnectServer(u_long serverIp,int port);//连接服务器  
    bool ProcessConnection(SOCKET sd);    //客户端服务器交互  
    void CloseSocket();         //释放套接字  
    bool SendFileLength(SOCKET sd,char *filePath);  //发送文件长度  
    bool SendFile(SOCKET sd,char *filePath);    //发送文件  
    bool RecvCatalogInfo(SOCKET sd);     //接收目录信息  
    bool SendDownLoadFileName(SOCKET sd);  //发送要下载的文件名  
    bool ReceiveFileLength(SOCKET sd);    //接收文件长度  
    bool ReceiveFileName(SOCKET sd);   //接收文件名  
    bool ReceiveFile(SOCKET sd);      //接收文件  
    //void DoWork();       //主体函数  
};  
//client.cpp  
#define _CRT_SECURE_NO_WARNINGS  
#pragma once  
#include<iostream>  
#include<vector>  
#include<WinSock2.h>  
#include"client.h"  
#include"message.h"  
using namespace std;  
int main()  
{  
    Client client;  
    if(!client.InitSock())  
    {  
        cout<<"初始socket失败"<<endl;  
        return -1;  
    }  
    SOCKET saRemote=client.ConnectServer(client.ResolveAdress(SERVER_IP),PORT);  
    if(saRemote==INVALID_SOCKET)  
    {  
        cout<<"连接服务器失败"<<endl;  
        return -1;  
    }  
    if(!client.ProcessConnection(saRemote))  
    {  
        return -1;  
    }  
    client.CloseSocket();  
    return 0;  
}  
bool Client::InitSock()    //初始socket  
{  
    WSADATA wsData;  
    WORD wr=MAKEWORD(2,2);  
    if(WSAStartup(wr,&wsData)==0)  
    {  
        return true;  
    }  
    return false;  
}  
u_long Client::ResolveAdress(char *serverIp)   //解析IP地址  
{  
    u_long nAddr=inet_addr(serverIp);  
    if(nAddr==INADDR_NONE)  //表明serverIp使用的是主机名形式  
    {  
        hostent *ent=gethostbyname(serverIp);  
        if(ent==NULL)  
        {  
            cout<<"获取主机名出错"<<WSAGetLastError()<<endl;  
        }  
        else  
        {  
            nAddr=*((u_long *)ent->h_addr_list[0]);  
        }  
    }  
    if(nAddr==INADDR_NONE)  
    {  
        cout<<"解析主机地址失败"<<endl;  
    }  
    return nAddr;  
}  
SOCKET Client::ConnectServer(u_long serverIp,int port)   //连接服务器  
{  
    sd=socket(AF_INET,SOCK_STREAM,0);  
    if(sd==INVALID_SOCKET)  
    {  
        cout<<"床架套接字失败"<<endl;  
        return INVALID_SOCKET;  
    }  
    sockaddr_in saServer;  
    saServer.sin_family=AF_INET;  
    saServer.sin_addr.S_un.S_addr=serverIp;  
    saServer.sin_port=htons(port);  
    if(connect(sd,(sockaddr*)&saServer,sizeof(sockaddr_in))==SOCKET_ERROR)  
    {  
        cout<<"连接服务器失败"<<WSAGetLastError()<<endl;  
        closesocket(sd);  
        return INVALID_SOCKET;  
    }  
    return sd;  
}  
bool Client::ProcessConnection(SOCKET sd)      //进行通信  
{  
    //-------------------------------------------------  
    //可以将下面代码看做设置系统缓冲区  
    int nRecvBuf=1024000;//设置为1000K  
    setsockopt(sd,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));  
      //发送缓冲区  
    int nSendBuf=1024000;//设置为1000K  
    setsockopt(sd,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));  
    //---------------------------------------------------------  
    while(true)  
    {  
        cout<<"（1）向服务器传送文件"<<endl;  
        cout<<"（2）从服务器下载文件"<<endl;  
        cout<<"（3）退出业务"<<endl;  
        int n;  
        loop:cin>>n;  
        switch(n)  
        {  
        case 1:  
            {  
                //向服务器发送传送文件消息  
                Message::MsgSendFile msgSendFile;  
                if(send(sd,(char *)&msgSendFile,sizeof(Message::MsgSendFile),0)==SOCKET_ERROR)  
                {  
                    cout<<"发送消息失败"<<endl;  
                    return false;  
                }  
                Sleep(10);     //睡眠10ms保证对方将发送的消息取走  
                char filePath[MAX_FILE_NAME_LENGTH];  
                cout<<"请输入文件路径如：F:/a/b.jpg"<<endl;  
                cin>>filePath;  
                char fileDrive[_MAX_DRIVE];  
                char fileDir[_MAX_DIR];  
                char fileName[_MAX_FNAME];  
                char fileExt[_MAX_EXT];  
                _splitpath(filePath,fileDrive,fileDir,fileName,fileExt);  //将文件路径解析  
                Message::MsgFileName msgFileName;  
                strcat(fileName,fileExt);  
                strcpy(msgFileName.fileName,fileName);  
                if(send(sd,(char *)&msgFileName,sizeof(Message::MsgFileName),0)==SOCKET_ERROR)  //发送文件名  
                {  
                    cout<<"发送文件名出错"<<WSAGetLastError()<<endl;  
                }  
                Sleep(10);  
                if(!SendFileLength(sd,filePath))  //发送文件长度  
                {  
                    cout<<"发送文件长度出错"<<endl;  
                    return false;  
                }  
                Sleep(10);  
                if(!SendFile(sd,filePath))  //发送文件  
                {  
                    cout<<"发送文件出错"<<endl;  
                    return false;  
                }  
            }  
            break;  
        case 2:  
            {  
                Message::MsgDownLoadFile msgDownLoadFile;  
                if(send(sd,(char *)&msgDownLoadFile,sizeof(Message::MsgDownLoadFile),0)==SOCKET_ERROR)  
                {  
                    cout<<"发送下载文件消息失败"<<WSAGetLastError()<<endl;  
                    return false;  
                }  
                if(!RecvCatalogInfo(sd))  
                {  
                    return false;  
                }  
                if(!SendDownLoadFileName(sd))  
                {  
                    return false;  
                }  
                if(!ReceiveFileLength(sd))  
                {  
                    return false;  
                }  
                if(!ReceiveFileName(sd))  
                {  
                    return false;  
                }  
                if(!ReceiveFile(sd))  
                {  
                    return false;  
                }  
            }  
            break;  
        case 3:  
            break;  
        default:  
            cout<<"你输入的不符合要求，重新输入"<<endl;  
            goto loop;  
        }  
    }  
    return true;  
}  
bool Client::SendFileLength(SOCKET sd,char *filePath)  
{  
      
    FILE *pFile;  
    pFile=fopen(filePath,"r+b");  
    fseek(pFile,0,SEEK_END);  
    nFileLength=_ftelli64(pFile);  
    Message::MsgFileLength msgFileLength;  
    msgFileLength.fileLength=nFileLength;  
    fclose(pFile);  
    if(send(sd,(char *)&msgFileLength,sizeof(Message::MsgFileLength),0)==SOCKET_ERROR)  
    {  
        return false;  
    }  
    return true;  
}  
bool Client::SendFile(SOCKET sd,char *filePath)   //发送文件  
{  
    cout<<"进入到发送文件内容"<<endl;  
    Message::MsgFile msgFile;  
    if(send(sd,(char *)&msgFile,sizeof(Message::MsgFile),0)==SOCKET_ERROR)  
    {  
        cout<<"发送文件消息出错"<<WSAGetLastError()<<endl;  
        return false;  
    }  
    Sleep(10);  
    FILE *pFile;  
    pFile=fopen(filePath,"r+b");  
    fseek(pFile,0,SEEK_SET);   //定位到文件首位置  
    _int64 i=0;  
    char buff[MAX_PACK_SIZE];  
    while(i<nFileLength)  
    {  
        int nSize;  
        if(i+MAX_PACK_SIZE>nFileLength)  
        {  
            nSize=(int)(nFileLength-i);  
        }  
        else  
        {  
            nSize=MAX_PACK_SIZE-1;  
        }  
        fread(buff,sizeof(char),nSize,pFile);  
        int nSend;  
        nSend=send(sd,buff,nSize,0);  
        if(nSend==SOCKET_ERROR)  
        {  
            cout<<"发送失败"<<endl;  
            return false;  
        }  
        i+=nSend;  
        fseek(pFile,-(nSize-nSend),SEEK_CUR);  //定位到实际已发送到的位置  
        memset(buff,0,sizeof(char)*MAX_PACK_SIZE); //将buff清空  
    }  
    fclose(pFile);  
    return true;  
}  
bool Client::RecvCatalogInfo(SOCKET sd)   //接收目录信息  
{  
    int flag=1;     //接收目录信息成功标志  
    char buff[MAX_PACK_SIZE];  
    Message::MsgHead *msgHead;  
    while(true)  
    {  
        if(recv(sd,buff,MAX_PACK_SIZE,0)==SOCKET_ERROR)  
        {  
            cout<<"接收目录信息失败"<<WSAGetLastError()<<endl;  
            flag=0;  
            break;  
        }  
        msgHead=(Message::MsgHead *)buff;  
        if(msgHead->msgId==MSG_COMPLETE)      //判断消息是否是标准消息  
        {  
            cout<<"目录信息发送完成"<<endl;  
            break;  
        }  
        else  
        {  
            cout<<buff<<endl;     //发送来的是目录信息，即文件名  
        }  
    }  
    if(flag==0)  
    {  
        return false;  
    }  
    return true;  
}  
bool Client::SendDownLoadFileName(SOCKET sd)      //发送下载的文件名  
{  
    cout<<"请输入你要下载的文件名"<<endl;  
    char fileName[_MAX_FNAME+_MAX_EXT];  
    cin>>fileName;  
    Message::MsgFileName msgFileName;  
    strcpy(msgFileName.fileName,fileName);  
    if(send(sd,(char *)&msgFileName,MAX_PACK_SIZE,0)==SOCKET_ERROR)  
    {  
        cout<<"发送下载文件名出错"<<WSAGetLastError()<<endl;  
        return false;  
    }  
    return true;  
}  
bool Client::ReceiveFileLength(SOCKET sd)     //接收下载的文件长度  
{  
    char buff[MAX_PACK_SIZE];  
    Message::MsgFileLength *msgFileLength;  
    if(recv(sd,buff,MAX_PACK_SIZE,0)==SOCKET_ERROR)  
    {  
        cout<<"接收文件长度失败"<<WSAGetLastError()<<endl;  
        return false;  
    }  
    msgFileLength=(Message::MsgFileLength *)buff;  
    nFileLength=msgFileLength->fileLength;  
    cout<<"接收到文件长度"<<nFileLength<<endl;  
    return true;  
}  
bool Client::ReceiveFileName(SOCKET sd)   //接收下载的文件名  
{  
    char buff[MAX_PACK_SIZE];  
    memset(fileName,0,sizeof(char)*(_MAX_FNAME+_MAX_EXT));  
    Message::MsgFileName *msgFileName;  
    if(recv(sd,buff,MAX_PACK_SIZE,0)==SOCKET_ERROR)  
    {  
        cout<<"接收文件名出错"<<endl;  
        return false;  
    }  
    msgFileName=(Message::MsgFileName *)buff;  
    strcpy(fileName,msgFileName->fileName);  
    cout<<"接收到文件名"<<fileName<<endl;  
    return true;  
}  
bool Client::ReceiveFile(SOCKET sd)      //接收文件内容  
{  
   char buff[MAX_PACK_SIZE];  
   FILE *pFile;  
   pFile=fopen(fileName,"a+b");  
   _int64 i=0;  
   while(i+1<nFileLength)  
   {  
        int nRecv=recv(sd,buff,MAX_PACK_SIZE,0);  
        if(nRecv==SOCKET_ERROR)  
        {  
            return false;  
        }  
        fwrite(buff,sizeof(char),nRecv,pFile);  
        i+=nRecv;  
        memset(buff,0,sizeof(char)*MAX_PACK_SIZE);  
    }  
    fclose(pFile);  
    return true;  
}  
void Client::CloseSocket()   //关闭套接字  
{  
    closesocket(sd);  
    WSACleanup();  
}  


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
void main() {
// 初始化
    WSADATA wsaData;
    int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
    if ( iResult != NO_ERROR )
        printf("Error at WSAStartup()/n");
    // 建立socket
    SOCKET server;
    server = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( server == INVALID_SOCKET ) {
        printf( "Error at socket(): %ld/n", WSAGetLastError() );
        WSACleanup();
        return;
    }
    // 绑定socket
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    service.sin_port = htons(8000);
    if ( bind( server, (SOCKADDR*) &service, sizeof(service) ) == SOCKET_ERROR ) {
        printf( "bind() failed./n" );
        closesocket(server);
        return;
    }
    
    // 监听 socket
    if ( listen( server, 1 ) == SOCKET_ERROR )
        printf( "Error listening on socket./n");
    // 接受连接
    SOCKET AcceptSocket;
    printf( "Waiting for a client to connect.../n" );
    while (1) {
        AcceptSocket = SOCKET_ERROR;
        while ( AcceptSocket == SOCKET_ERROR ) {
            AcceptSocket = accept( server, NULL, NULL );
        }
        printf( "Client Connected./n");
        server = AcceptSocket; 
        break;
    }
    
    // 发送接受数据
//    int bytesSent;
    char sendbuf[1024];//发送缓冲区
 //DWORD dwFileSize;//文件大小
 DWORD        dwRead;
    BOOL         bRet;
    char filename[]="c://MyMain.java";
 HANDLE hFile=CreateFile(filename,GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
 //dwFileSize=GetFileSize(hFile,NULL);
 while(true)
    {
      bRet=ReadFile(hFile,sendbuf,1024,&dwRead,NULL);
      if(bRet==FALSE)
      {
        MessageBox(NULL,"Read Buf ERROR!","Error",MB_OK);
        break;
      }
      else if(dwRead==0)
      {
        MessageBox(NULL,"Send file OK!","OK",MB_OK);
        break;
      }
      else
      {
       send(server,sendbuf,dwRead,0);
      }
    };
    CloseHandle(hFile);
    return;
}
接收
#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
void main() {
    // 初始化 Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
    if ( iResult != NO_ERROR )
        printf("Error at WSAStartup()/n");
    // 建立socket socket.
    SOCKET client;
    client = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( client == INVALID_SOCKET ) {
        printf( "Error at socket(): %ld/n", WSAGetLastError() );
        WSACleanup();
        return;
    }
    // 连接到服务器.
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    clientService.sin_port = htons( 8000 );
    if ( connect( client, (SOCKADDR*) &clientService, sizeof(clientService) ) == SOCKET_ERROR) {
        printf( "Failed to connect./n" );
        WSACleanup();
        return;
    }
char recvbuf[1024];//发送缓冲区
 int read;
 DWORD        dwWrite;
    BOOL         bRet;
    // 发送并接收数据.
    char filename[]="c://syn.txt";
 HANDLE hFile=CreateFile(filename,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
while(true)
    {
   read=recv( client, recvbuf, 1024, 0 );
   if(read==-1)break;
   bRet=WriteFile(hFile,recvbuf,read,&dwWrite,NULL);
      if(bRet==FALSE)
      {
        MessageBox(NULL,"Write Buf ERROR!","Error",MB_OK);
        break;
      }
           
    }
 MessageBox(NULL,"Receive file OK!","OK",MB_OK);
    return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
    afx_msg void OnBnClickedButton1();
public:
    BOOL InitSocket();    //初始化并创建套接字

    static DWORD WINAPI ThreadProc(LPVOID lpParameter);    //创建线程去执行服务器accept()

	void CSendFileServerDlg::OnBnClickedButton1()
{
    // TODO: 在此添加控件通知处理程序代码
    if (InitSocket())
    {
        GetDlgItem(IDC_EDIT1)->SetWindowText(_T("服务器开启监听。。。 \r\n"));

        //创建线程
        HANDLE hThread = CreateThread(NULL,0,ThreadProc,NULL,0,NULL);
        //关闭该接收线程句柄，释放引用计数
        CloseHandle(hThread);
    }
}

BOOL CSendFileServerDlg::InitSocket()
{
    //加载套接字库
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD( 1, 1 );
    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 ) 
    {
        return FALSE;
    }

    if ( LOBYTE( wsaData.wVersion ) != 1 || HIBYTE( wsaData.wVersion ) != 1 ) 
    {
            WSACleanup( );
            return FALSE;
    }

    //创建套接字
    //SOCKET m_socket=socket(AF_INET,SOCK_STREAM,0);
    m_socket=socket(AF_INET,SOCK_STREAM,0);
    if (m_socket == INVALID_SOCKET)
    {
        AfxMessageBox(_T("套接字创建失败！"));
        return FALSE;
    }

    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    addrSrv.sin_family=AF_INET;
    addrSrv.sin_port=htons(8099);

    err = bind(m_socket,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));    //绑定本地端口
    if (err==SOCKET_ERROR)
    {
        closesocket(m_socket);
        AfxMessageBox(_T("绑定失败！"));
        return FALSE;
    }
    listen(m_socket,5);//开启监听

    return TRUE;

}

DWORD WINAPI CSendFileServerDlg::ThreadProc(LPVOID lpParameter)
{
    SOCKADDR_IN addrClient;
    int len = sizeof(SOCKADDR);

    while (true)
    {
        SOCKET sockConn=accept(m_socket,(SOCKADDR*)&addrClient,&len);

        CString filename = _T("E:\\test.zip");
        HANDLE hFile;
        unsigned long long file_size = 0;

        char Buffer[1024];
        DWORD dwNumberOfBytesRead;

        hFile = CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
        file_size = GetFileSize(hFile,NULL);
        send(sockConn,(char*)&file_size,sizeof(unsigned long long)+1,NULL);

        do 
        {
            ::ReadFile(hFile,Buffer,sizeof(Buffer),&dwNumberOfBytesRead,NULL);
            ::send(sockConn,Buffer,dwNumberOfBytesRead,0);
        } while (dwNumberOfBytesRead);


        CloseHandle(hFile);
    }

    return 0;
}


public:
    afx_msg void OnBnClickedButton1();
 
    BOOL InitSocket();
    void ConnectServer();
    void ConnectRecvFileData(DWORD ip,int port);
	private:
    CProgressCtrl *m_progress; //进度条

	BOOL CRecvFileClientDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);            // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标

    // TODO: 在此添加额外的初始化代码

    m_progress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1); 
    m_progress->SetPos(0); 

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRecvFileClientDlg::OnBnClickedButton1()
{
    // TODO: 在此添加控件通知处理程序代码
    ConnectServer();
    
}


BOOL CRecvFileClientDlg::InitSocket()
{
    //加载套接字库
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD( 1, 1 );
    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 ) 
    {
        return FALSE;
    }

    if ( LOBYTE( wsaData.wVersion ) != 1 || HIBYTE( wsaData.wVersion ) != 1 ) 
    {
        WSACleanup( );
        return FALSE;
    }

    return TRUE;

}

void CRecvFileClientDlg::ConnectRecvFileData(DWORD ip,int port)
{
    
    unsigned long long file_size=0;
    SOCKET sockClient = socket(AF_INET,SOCK_STREAM,0);
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr=htonl(ip);
    addrSrv.sin_port=ntohs(port);
    addrSrv.sin_family = AF_INET;
    //connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
    //recv(sockClient,(char*)&file_size,sizeof(unsigned long long)+1,NULL);

    if (!connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR)))
    {
        GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T(""));
        GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T("连接服务器成功！\r\n"));

        

        recv(sockClient,(char*)&file_size,sizeof(unsigned long long)+1,NULL);

        unsigned short maxvalue = file_size;    //此处不太稳妥 当数据很大时可能会出现异常
        m_progress->SetRange(0,maxvalue); 

        if (file_size>0)
        { 
            GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T(""));
            GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T("文件下载到本地 d：\\test.zip \r\n"));

            DWORD dwNumberOfBytesRecv=0;
            DWORD dwCountOfBytesRecv=0;
            char Buffer[1024];
            CString filename = _T("d:\\test.zip");
            HANDLE hFile;
            hFile = CreateFile(filename,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

            do 
            {
                m_progress->SetPos(dwCountOfBytesRecv);//更新进度条

                dwNumberOfBytesRecv = ::recv(sockClient,Buffer,sizeof(Buffer),0);
                ::WriteFile(hFile,Buffer,dwNumberOfBytesRecv,&dwNumberOfBytesRecv,NULL);
                dwCountOfBytesRecv += dwNumberOfBytesRecv;                
            } while (file_size - dwCountOfBytesRecv);


            CloseHandle(hFile);
            
            GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T(""));
            GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T("文件接收完毕！\r\n"));
            AfxMessageBox(_T("文件接收完毕!"));//醒目可以注释
        }else
        {
            AfxMessageBox(_T("获取文件总大小失败！"));
        }
    }else
    {
        AfxMessageBox(_T("连接服务器失败、请确认IP地址或端口号！"));
    }
    
    

    closesocket(sockClient);//关闭套接字


}

void CRecvFileClientDlg::ConnectServer()
{
    if (InitSocket())
    {
        DWORD strIp =NULL;
        CString strPort = _T("");

        ((CIPAddressCtrl*)GetDlgItem(IDC_IP))->GetAddress(strIp);
        GetDlgItem(IDC_PORT)->GetWindowText(strPort);

        if (strIp==NULL||strPort=="")
        {
            AfxMessageBox(_T("Ip地址或Port端口号不能为空！"));

        }else
        {
            int port = atoi(strPort.GetBuffer(1));
            ConnectRecvFileData(strIp,port);
            
        }
    }
}