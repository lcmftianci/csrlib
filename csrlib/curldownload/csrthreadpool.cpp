#include "csrthreadpool.h"
#include "csrtask.h"
#include "csrthread.h"
#include <stdlib.h>
#include <windows.h>
#include "filenameio.h"

using namespace std;

#pragma comment(lib,"pthreadVC2.lib")

#if 0
//测试C++接口
//加载配置文件
std::string strIniFile = GetMoudlePath();
strIniFile += "GB28181.ini";
cosip soip(strIniFile);

//初始化服务 用jrtp库接收
soip.StartPSStream(0);

while (true)
{
	//不断获取视频流
	//解码

	//sdl显示
}
#endif


//线程池构造函数
CsrThreadPool::CsrThreadPool(size_t maxthreadnum)
{
	MaxThreadNum = maxthreadnum;		//	最大线程个数
	bDestroy = false;					//	是否销毁线程池
	bWaitDestroy = false;               //	是否等待销毁
	m_taskThreadMutex = NULL;			//	任务线程锁，每插入一个任务是需要锁定任务队列
	m_freeThreadMutex = NULL;           //  空闲线程锁，每启动一个线程执行任务是需要锁定线程队列
	WatiTime = 500;						//	等待时间
	pthread_mutex_init(&m_taskThreadMutex, NULL);	//	初始化任务队列锁
	pthread_cond_init(&m_threadCond, NULL);			//	初始化条件变量
	pthread_mutex_init(&m_freeThreadMutex, NULL);	//	初始化线程队列锁
}

//销毁类对象及成员
CsrThreadPool::~CsrThreadPool()
{
	cout << "线程池析构函数" << endl;

	//分别释放线程队列锁，条件对象还有任务队列锁
	pthread_mutex_destroy(&m_taskThreadMutex);
	pthread_cond_destroy(&m_threadCond);
	pthread_mutex_destroy(&m_freeThreadMutex);
}

//添加任务
void CsrThreadPool::AddAsynTask(TaskFunc taskfunc, void* pParams)
{
	//声明并初始化一个线程任务
	CsrTask *task = new CsrTask(taskfunc, pParams);
	LockTaskQueue();			//	锁住任务队列
	m_queTask.push(task);		//	添加任务到队列
	SignalTaskQueue();			//	通知任务执行
	UnLockTaskQueue();			//	解出锁定
}

//激活线程池
void CsrThreadPool::Activate()
{
	for (int i = 0; i < MaxThreadNum; ++i)
	{
		CsrThread* thread = new CsrThread(this);
		m_arrThreads.push_back(thread);
		thread->Start();
	}

	//启动扫描线程
	pthread_create(&m_taskThread, NULL, &ScanTask, this);
}

//销毁线程池
void CsrThreadPool::Destroy()
{
	cout << "Destroy begin" << endl;
	bDestroy = true;
	SignalTaskQueue();	//通知线程退出等待；

						//停止扫描线程
	pthread_join(m_taskThread, NULL);

	//停止工作线程
	size_t size = m_arrThreads.size();
	for (size_t i = 0; i < size; ++i)
	{
		CsrThread* thread = m_arrThreads[i];
		thread->Destroy();
		delete thread;
	}

	size_t remain = m_queTask.size();
	for (size_t i = 0; i < remain; ++i)
	{
		CsrTask *task = m_queTask.front();
		m_queTask.pop();
		delete task;
	}

	cout << "Destroy end" << endl;
}

//等在所有线程完成任务
void CsrThreadPool::WaitTaskFinishAndDestroy()
{
	bWaitDestroy = true;
	pthread_join(m_taskThread, NULL);

	//停止工作线程
	for (size_t i = 0; i < m_arrThreads.size(); ++i)
	{
		CsrThread* thread = m_arrThreads[i];
		thread->Destroy();
		cout << "thread->Destroy()" << endl;
		delete thread;
	}
}

//添加空闲线程进线程池
void CsrThreadPool::AddFreeThreadToQueue(CsrThread *thread)
{
	//等待下一个任务
	LockFreeThreadQueue();
	m_queFreeThreads.push(thread);
	UnLockFreeThreadQueue();
}

//改变条件
void CsrThreadPool::SignalTaskQueue()
{
	pthread_cond_signal(&m_threadCond);
}

//锁住任务队列
void CsrThreadPool::LockTaskQueue()
{
	pthread_mutex_lock(&m_taskThreadMutex);
}

void CsrThreadPool::UnLockTaskQueue()
{
	pthread_mutex_unlock(&m_taskThreadMutex);
}

//锁住空闲线程队列
void CsrThreadPool::LockFreeThreadQueue()
{
	pthread_mutex_lock(&m_freeThreadMutex);
}

void CsrThreadPool::UnLockFreeThreadQueue()
{
	pthread_mutex_unlock(&m_freeThreadMutex);
}
//扫描任务
void* CsrThreadPool::ScanTask(void* pParams)
{
	CsrThreadPool *pool = (CsrThreadPool*)pParams;
	size_t queNum = 0;
	size_t freeQueNum = 0;
	while (true)
	{
		if (pool->bDestroy)
			break;
		pool->Start(&queNum, &freeQueNum);

		//如果处于销毁状态或者线程设置数为零就直接跳出
		if (pool->bWaitDestroy && !queNum && freeQueNum == pool->GetMaxThreadNum())
		{
			pool->bDestroy = true;
			break;
		}

		//如果任务数为零则退出
		if (queNum)
		{
			if (freeQueNum)
				continue;
			else
				Sleep(pool->GetWatiTime());		//无空闲线程
		}
		else//进入任务队列
		{
			pool->LockTaskQueue();
			cout << pool->GetQueueTaskCount() << endl;
			if (!pool->GetQueueTaskCount())
			{
				cout << "进入等待" << endl;
				pool->WaitQueueTaskDignal();
				cout << "跳出等待" << endl;
			}
			pool->UnLockTaskQueue();
		}
	}
	cout << "Scantask end" << endl;
	return nullptr;
}

int CsrThreadPool::GetQueueTaskCount()
{
	return m_queTask.size();
}
void CsrThreadPool::WaitQueueTaskDignal()
{
	pthread_cond_wait(&m_threadCond, &m_taskThreadMutex);
}

/*
函数功能：执行线程
输出参数1：队列中剩余任务数量
输出参数2：队列中剩余空闲线程数量
*/
void CsrThreadPool::Start(size_t *queue_remain_num, size_t *free_thread_num)
{
	cout << "开始执行任务" << endl;
	LockFreeThreadQueue();
	if (!m_queFreeThreads.empty())
	{
		LockTaskQueue();
		if (!m_queTask.empty())
		{
			CsrTask* task = m_queTask.front();
			m_queTask.pop();
			CsrThread* freeThread = m_queFreeThreads.front();
			m_queFreeThreads.pop();
			freeThread->m_task = task;
			freeThread->Notify();

			*queue_remain_num = m_queTask.size();
		}
		else
		{
			*queue_remain_num = 0;
		}

		UnLockTaskQueue();

		*free_thread_num = m_queFreeThreads.size();
	}
	else
	{
		*free_thread_num = 0;
	}
	UnLockFreeThreadQueue();
	cout << "任务执行完毕" << endl;
}