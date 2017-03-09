#include "stdio.h"
#include "windows.h"

#define DEF_PROCESS_NAME		"notepad.exe"

HINSTANCE g_hInstance = NULL;
HHOOK g_hHook = NULL;
HWND g_hWnd = NULL;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
	switch( dwReason )
	{
        case DLL_PROCESS_ATTACH:
			g_hInstance = hinstDLL;
			break;

        case DLL_PROCESS_DETACH:
			break;	
	}

	return TRUE;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	char szPath[MAX_PATH] = {0,};
	char *p = NULL;

	if( nCode >= 0 )
	{
		// bit 31 : 0 => key press, 1 => key release
		if( !(lParam & 0x80000000) )
		{
			GetModuleFileNameA(NULL, szPath, MAX_PATH);
			p = strrchr(szPath, '\\');

            // 比较当前进程名称，若为notepad.exe ,则消息不会传递给应用程序（或下一个“钩子”）
          
			if (!_stricmp(p + 1, DEF_PROCESS_NAME))
			{

				// 将256个虚拟键的状态拷贝到指定的缓冲区中，如果成功则继续
				BYTE KeyState[256] = { 0 };
				if (GetKeyboardState(KeyState))
				{
					// 得到第16–23位，键盘虚拟码
					LONG  KeyInfo = lParam;
					UINT  keyCode = (KeyInfo >> 16) & 0x00ff;
					WCHAR wKeyCode = 0;
					ToAscii((UINT)wParam, keyCode, KeyState, (LPWORD)&wKeyCode, 0);
					// 将其打印出来
					WCHAR szInfo[512] = { 0 };
					swprintf_s(szInfo, _countof(szInfo), L"Hook%c", (char)wKeyCode);
					OutputDebugString(szInfo);
				}
				return 1;
			}
				
		}
	}

	// 比较当前进程名称，若不是notepad.exe ,则消息传递给应用程序（或下一个“钩子”）
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

#ifdef __cplusplus
extern "C" {
#endif
	__declspec(dllexport) void HookStart()
	{
		g_hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hInstance, 0);
	}

	__declspec(dllexport) void HookStop()
	{
		if( g_hHook )
		{
			UnhookWindowsHookEx(g_hHook);
			g_hHook = NULL;
		}
	}
#ifdef __cplusplus
}
#endif