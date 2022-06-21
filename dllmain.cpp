// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "kwm.hpp"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void __declspec(dllexport) kwmEncS(const char* fn)
{
    encrypt(fn);
}

void __declspec(dllexport) kwmDecS(const char* fn)
{
    decrypt(fn);
}

void __declspec(dllexport) kwmEncD(const char* fn, const char* pswFn)
{
    encrypt(fn, pswFn);
}

void __declspec(dllexport) kwmDecD(const char* fn, const char* pswFn)
{
    decrypt(fn, pswFn);
}
