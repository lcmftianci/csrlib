#include <iostream>
#include "csrthread.h"
#include "filenameio.h"
#include "csrtask.h"
#include "csrthreadpool.h"
#include "csrcurldownload.h"

using namespace std;

pthread_mutex_t g_namemutex;

//文件指针
typedef struct _FileData {
	FILE *fp;
}FileData, *pFileData;

//文件存盘
size_t WriteToDisk(void *contents, size_t size, size_t mem, void* userp)
{
	pFileData pData = (pFileData)userp;
	size_t num = mem*size;
	size_t write_num = fwrite(contents, 1, num, pData->fp);
	return write_num;
}
int g_Task = 0;

//线程函数
void *RunTaskFunc(void * arg)
{
	pthread_mutex_lock(&g_namemutex);
	++g_Task;
	const char* url = (const char*)arg;
	cout << "thread index: " << g_Task << "\n" << url << endl;
	CsrCurlDownload* manager = new CsrCurlDownload();

	FileData dd;
	char path[8];
	memset(path, 0, sizeof(path));
	sprintf(path, "%d.zip", g_Task);
	dd.fp = fopen(path, "wb");
	pthread_mutex_unlock(&g_namemutex);
	manager->Process(url, &WriteToDisk, &dd);

	fclose(dd.fp);
	return NULL;
}


int main(void)
{
	pthread_mutex_init(&g_namemutex, NULL);
	cout << "测试程序开始" << endl;
	CsrThreadPool *pool = new CsrThreadPool(5);
	pool->Activate();
#if 0
	for (int o = 0; o < 10; ++o)
	{
		int *i = new int;
		*i = o;
		pool->AddAsynTask(&RunTaskFunc, i);
	}
#endif

	//启动任务去下载东西
	const char* capsuleTensor = "https://codeload.github.com/naturomics/CapsNet-Tensorflow/zip/master";
	pool->AddAsynTask(&RunTaskFunc, (void*)capsuleTensor);
	const char* capsuleKeras = "https://codeload.github.com/XifengGuo/CapsNet-Keras/zip/master";
	pool->AddAsynTask(&RunTaskFunc, (void*)capsuleKeras);

	//用sdl显示图片


	getchar();

	pool->Destroy();
	delete pool;
	cout << "测试线程池结束" << endl;

	system("pause");
	return 0;
}