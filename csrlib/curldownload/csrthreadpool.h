#ifndef __CSR_THREAD_POOL_H_
#define __CSR_THREAD_POOL_H_

/*
线程池创建
----------------------------------------------------------------------------
---->创建线程池---->添加任务--->选择空闲线程处理任务--->结束任务释放线程
//保持线程的共享数据和每个线程的独立数据
*/

#include <pthread/pthread.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>
#include "csrtask.h"
#include "csrdef.h"

//任务线程
class CsrThread;

//线程池
class CsrThreadPool {
public:
	CsrThreadPool(size_t maxthreadnum);
	~CsrThreadPool();

	void AddAsynTask(TaskFunc taskfunc, void* pParams);		//添加任务
	void Activate();		//激活线程池
	void Destroy();			//销毁线程池
	void WaitTaskFinishAndDestroy();				//等在所有线程完成任务
	void AddFreeThreadToQueue(CsrThread *thread);	//添加空闲线程进线程池
	/*
	函数功能：执行线程
	输出参数1：队列中剩余线程数量
	输出参数2：队列中剩余空闲线程数量
	*/
	void Start(size_t *queue_remain_num, size_t *free_thread_num);

	bool bDestroy;		//销毁线程
	bool bWaitDestroy;	//等待销毁线程
	int GetQueueTaskCount();
	void WaitQueueTaskDignal();

private:
	TPGETSET(unsigned, WatiTime)		//	等待时间
	TPGETSET(size_t, MaxThreadNum)		//	线程池容量
	TPGETSET(int, FreeThreadNum)		//	空闲线程

	static void* ScanTask(void* pParams);	//扫描任务

	void SignalTaskQueue();
	void LockTaskQueue();
	void UnLockTaskQueue();


	void LockFreeThreadQueue();
	void UnLockFreeThreadQueue();

	std::vector<CsrThread*> m_arrThreads;			//	线程池***容量
	std::queue<CsrThread*>  m_queFreeThreads;		//	空闲线程池队列
	pthread_mutex_t			  m_freeThreadMutex;	//	线程池互斥量

	std::queue<CsrTask*>  m_queTask;				//  任务队列
	pthread_mutex_t			  m_taskThreadMutex;	//  线程池互斥量
	pthread_cond_t			  m_threadCond;			//	线程池队列信号

	pthread_t                 m_taskThread;			//	队列扫描线程，判断线程池是否有未完成任务
};

#endif		//_THREAD_POOL_H_