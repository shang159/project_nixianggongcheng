// ConsoleApplication1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<Windows.h>
DWORD WINAPI ThreadProc(LPVOID lParam)
{
	MessageBoxA(NULL, "www.reversecore.com", "ReverseCore", MB_OK);

	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
	MessageBoxA(NULL, "www.reversecore.com", "ReverseCore", MB_OK);
	//CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
	return 0;
}

