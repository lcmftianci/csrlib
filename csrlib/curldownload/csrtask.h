#ifndef _CSR_TASK_H_
#define _CSR_TASK_H_

//线程执行函数
typedef void* (*TaskFunc)(void* pParams);

class CsrTask {
public:
	CsrTask(TaskFunc task, void* pParams)
	{
		m_params = pParams;
		m_task = task;
	}

public:
	void*	  m_params;
	TaskFunc  m_task;
};

#endif // _THREAD_TASK_H_
