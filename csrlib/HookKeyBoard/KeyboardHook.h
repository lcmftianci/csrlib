#pragma once

#include <windows.h>

HHOOK g_hHook;

//安装钩子
extern "C" __declspec(dllexport) BOOL WINAPI SetKeyHook(BOOL bInstall);

//键盘狗子处理函数
LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam);
