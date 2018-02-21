#include "csrthread.h"
#include "csrthreadpool.h"
#include <iostream>

using namespace std;

//构造函数，初始化成员变量
CsrThread::CsrThread(CsrThreadPool* pool)
{
	m_task = NULL;
	m_bIsDestroy = false;
	m_mutex = NULL;
	m_cond_t = NULL;
	m_pool = pool;
	m_thread_id = 0;

	//初始化锁
	pthread_mutex_init(&m_mutex, NULL);
	//初始化条件变量
	pthread_cond_init(&m_cond_t, NULL);
}

//析构函数，销毁锁和条件变量
CsrThread::~CsrThread()
{
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond_t);
}

//添加任务到任务队列
int CsrThread::AddToFreeThreadQueue()
{
	m_pool->AddFreeThreadToQueue(this);
	return m_pool->GetFreeThreadNum();					//当前还剩多少正在空闲的线程 
}
void CsrThread::Notify()
{
	pthread_cond_signal(&m_cond_t);
}

//加锁
void CsrThread::Lock()
{
	pthread_mutex_lock(&m_mutex);
}
void CsrThread::UnLock()
{
	pthread_mutex_unlock(&m_mutex);
}
void CsrThread::Wait()
{
	pthread_cond_wait(&m_cond_t, &m_mutex);
}
void CsrThread::Join()
{
	int res = pthread_join(m_thread, NULL);
	cout << "res :" << res << endl;
}
int CsrThread::GetId()
{
	return m_thread_id;
}
void CsrThread::Destroy()
{
	Lock();
	m_bIsDestroy = true;
	Notify();
	UnLock();
	Join();
}

//启动线程
void CsrThread::Start()
{
	pthread_create(&m_thread, NULL, &CsrThread::DoTask, this);
	m_thread_id = (int)m_thread.p;
}

void *CsrThread::DoTask(void *pParams)
{
	CsrThread *thread = (CsrThread*)pParams;
	while (true)
	{
		thread->Lock();
		if (thread->m_bIsDestroy)
		{
			thread->UnLock();
			break;
		}
		thread->UnLock();

		CsrTask *task = thread->m_task;
		if (task)
		{
			(*task->m_task)(task->m_params);
			cout << "task finish" << thread->GetId() << endl;
			delete task;
			thread->m_task = NULL;
		}

		//通知队列加入空闲线程，挂起线程，如果需要销毁，不需要进入等待
		thread->Lock();
		if (thread->m_bIsDestroy)
		{
			thread->UnLock();
			break;
		}
		thread->AddToFreeThreadQueue();
		thread->Wait();
		thread->UnLock();
	}
	cout << "thread finish:" << thread->GetId() << endl;
	return nullptr;
}