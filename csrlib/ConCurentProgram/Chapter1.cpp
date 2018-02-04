#include <windows.h>
#include <stdio.h>
#include <process.h>

DWORD WINAPI ThreadFun(LPVOID pParams) 
{
	printf("线程ID %d\n", GetCurrentThreadId());
	return 0;
}

//VOID WINAPI InitializeSRWLock(
//	_Out_ PSRWLOCK SRWLock
//);
////函数功能：初始化一个读写锁。
////函数参数：一个指向读写锁的指针。
//
//VOID WINAPI AcquireSRWLockExclusive(
//	_Inout_ PSRWLOCK SRWLock
//);
////函数功能：以独占模式获取锁。
////函数参数：一个指向读写锁的指针。
//
//VOID WINAPI ReleaseSRWLockExclusive(
//	_Inout_ PSRWLOCK SRWLock
//);
////函数功能：释放一个以独占模式获取的锁。
////函数参数：一个指向读写锁的指针。
//
//
//VOID WINAPI AcquireSRWLockShared(
//	_Inout_ PSRWLOCK SRWLock
//);
////函数功能：以共享模式获取锁。
////函数参数：一个指向读写锁的指针。
//
//
//VOID WINAPI ReleaseSRWLockShared(
//	_Inout_ PSRWLOCK SRWLock
//);
////函数功能：释放一个以共享模式获取的锁。
////函数参数：一个指向读写锁的指针。

BOOL SetConsoleColor(WORD wAttributes)
{
	
}


int main(void) 
{
	printf("主线程\n");
	HANDLE hHandle = CreateThread(NULL, 0, ThreadFun, NULL, 0, NULL);
	WaitForSingleObject(hHandle, INFINITE);
	system("pause");
	return 0;
}