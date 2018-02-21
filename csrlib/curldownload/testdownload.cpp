#define  HAVE_STRUCT_TIMESPEC

#include <stdio.h>  
#include <pthread/pthread.h>  
#include <assert.h>  

#pragma comment(lib,"pthreadVC2.lib")  

void* Function_t(void* Param)
{
	printf("我是线程！ ");
	pthread_t myid = pthread_self();
	printf("线程ID=%d ", myid);
	return NULL;
}
#if 0
int main()
{
	pthread_t pid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&pid, &attr, Function_t, NULL);
	printf("======================================== ");
	getchar();
	pthread_attr_destroy(&attr);
	return 0;
}
#endif