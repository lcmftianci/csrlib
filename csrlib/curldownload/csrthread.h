#ifndef _CSR_THREAD_H_
#define _CSR_THREAD_H_

#include <pthread/pthread.h>
#include "csrtask.h"

class CsrThreadPool;

//任务线程
class CsrThread {
public:
	CsrThread(CsrThreadPool* pool);
	~CsrThread();

	//任务单元
	CsrTask *m_task;

	//添加任务到任务队列
	int AddToFreeThreadQueue();
	void Notify();

	//加锁
	void Lock();
	void UnLock();
	void Wait();
	void Join();
	int GetId();
	void Destroy();
	void Start();

private:
	pthread_mutex_t m_mutex;			// 锁
	pthread_cond_t	m_cond_t;
	bool			m_bIsDestroy;
	pthread_t		m_thread;
	int				m_thread_id;		// 线程id
	CsrThreadPool		*m_pool;            // 所在线程池

	static void *DoTask(void *pParams);
};


#endif //_TASK_THREAD_H_
