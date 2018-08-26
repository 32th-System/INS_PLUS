#include "stdafx.h"
#include "D3D9N.h"
Direct3DCreate9Type RealDirect3DCreate9 = NULL;

//把MyDirect3DCreate9导出为Direct3DCreate9，用__declspec(dllexport)的话实际上导出的是_MyDirect3DCreate9@4
#ifndef _WIN64
#pragma comment(linker, "/EXPORT:Direct3DCreate9=_MyDirect3DCreate9@4")
#else
#pragma comment(linker, "/EXPORT:Direct3DCreate9=MyDirect3DCreate9")
#endif
extern "C" void* WINAPI MyDirect3DCreate9(UINT SDKVersion)
{
	return RealDirect3DCreate9(SDKVersion);
}