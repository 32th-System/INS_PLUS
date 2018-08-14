// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "INS.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		init();
		MessageBox(NULL,_T("注入成功"), _T("这里是ins_Plus.dll"), MB_OK);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

