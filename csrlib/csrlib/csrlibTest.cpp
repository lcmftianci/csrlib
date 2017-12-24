#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "csrorder.h"
#include "interview.h"
#include "csrlist.h"
#include "csrplot.h"
#include "csrlog.h"
#include "csrcopy.h"
#include "csrdoublelist.h"
//#include "plot/koolplot.h"
#include "csrnetutil.h"
#include "csrscreencututils.h"
#include "csrscreenscanner.h"


using namespace std;


int main(int argc, char** argv)
{
#if 0
	cout << "begin" << endl;
	float arrF[] = { 1.0, 1.4, 1.7, 2.7, 6.4, 5.5 };
	SortBubble(arrF, sizeof(arrF) / sizeof(float), false);
	DisplayData(arrF, sizeof(arrF) / sizeof(float));

	cout << AddNum(3, 120) << endl;

	cout << MaxNumByte(3, 6) << endl;
	cout << MaxNumByte(9, 4) << endl;
	cout << MaxNumByte(-13, 15) << endl;
	(cout << MaxNumByte(-10, -189)) << endl;

	printf("hello world\n");

	float a = 113.3f;
	float b = 115.8f;
	swap(a, b);
	cout << a << "  " << b << endl;

	int i;
	int array[10] = {1,2,3,4,5,6,7,8,9,10};
	ACCESS_BUFORE(array[5], 4, 6);
	for (i = 0; i < 10; ++i)
	{
		printf("%d\n", array[i]);
	}

	addressBin();

	cout << thranslateScale("10", 10, 2).c_str() << endl;

	cout << thranslateScale("0000000011111010", 2, 10).c_str() << endl;

	//list测试代码
	linuxlist tlist;
	listCreate(&tlist);

	for (int i = 0; i < 10; ++i)
		listHeadInsert(&tlist, i);

	for (int i = 35; i > 20; --i)
		listHeadInsert(&tlist, i);

	listDisplay(&tlist);

	listReverse(&tlist);

	listDisplay(&tlist);

	listBubbleSort(&tlist);

	listDisplay(&tlist);

	int arrNum[] = { 1, 4, 6, 8, 3, 100, 32, 45, 2, 7, 6, 2000, 5000, 30, 90, 80, 10, 20, 30, 40, 50, 60, 70, 80, 100, 200, 300, 400, 500, -5, -3};
	QuickSort(arrNum, sizeof(arrNum) / sizeof(int), 0, sizeof(arrNum) / sizeof(int), true);
	DisplayData(arrNum, sizeof(arrNum) / sizeof(int));

	list * t_list = (list *)malloc(sizeof(list));

	csrdoulistCreate(t_list);

	for (int i = 0;i < 10;i++)
	{
		csrdoulistHeadInsert(t_list, i);
	}

	node t_node = csrdoulistFind(t_list, 7);

	printf("%d\n", t_node->m_data);

	csrdoulistModify(t_list, 1, 99);

	csrdoulistDelete(t_list, 3);

	csrdoulistDisplayAsc(t_list);

	csrdoulistDisplayDesc(t_list);

	csrdoulistDestroy(t_list);

	free(t_list);

	t_list = NULL;

	printf("你好，明天\n");
#endif

#if 0
	while (1)
	{
		system("cls");
		int funIndex = 0;
		cout << "1:关机" << endl;
		cout << "2:取消关机" << endl;
		cin >> funIndex;
		switch (funIndex)
		{
		case 1:
			system("COLOR 24");
			system("shutdown -s -t 10");
			system("COLOR 35");
			break;
		case 2:
			system("COLOR 38");
			system("shutdown -a");
			system("COLOR 94");
		default:
			break;
		}
	}

#endif

#if 0

	//绘制曲线
	//srand((unsigned int)time(0));
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);//使用双缓存模式和深度缓存
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("2D Bezier曲线");
	//initGLLib();
	glutDisplayFunc(drawLine);
	glutReshapeFunc(reshape);
	//glutKeyboardFunc(keyboard);
	//glutIdleFunc(drawLine);//设置空闲时调用的函数

	InitLogSys();
	LogWrite("hellos");
	CopyFileToDest("E:\\照片\\picture", "D:\\bak\\dss", true, true);
	EndLogSys();

	glutMainLoop();
#endif
	//glutInit(&argc, argv);
	////初始化GLUT库OpenGL窗口的显示模式
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	//glutInitWindowSize(500, 500);
	//glutInitWindowPosition(100, 100);
	//glutCreateWindow(argv[0]);
	//glutDisplayFunc(drawLine);
	//glutReshapeFunc(ChangeSize);
	//SetupRC();
	//glutMainLoop();

//传输截屏文件
	//1.截屏
	//CaptureData capData;
	//ConfigureCapture(GetDesktopWindow(), &capData);
	//CaptureScreen(&capData);

	//2.传输数据
	//HANDLE hangle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)udpscreenclient, (LPVOID)&capData, 0, NULL);
	//CloseHandle(hangle);

	//linux与windows交互
	//HANDLE handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)udpclient, NULL, 0, NULL);
	//CloseHandle(handle);

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
	addrClient.sin_addr.S_un.S_addr = inet_addr("192.168.100.26"); //inet_addr("192.168.122.1");//inet_addr("192.168.174.130");
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
		if (sendto(sockClient, sendBuf, sizeof(sendBuf)/*strlen(sendBuf) + 1*/, 0, (SOCKADDR*)&addrClient, len) > 0)
			printf("Send ok\n");
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

	system("pause");
	return 0;
}