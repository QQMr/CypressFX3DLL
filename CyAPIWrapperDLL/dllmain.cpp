﻿// dllmain.cpp : 定義 DLL 應用程式的進入點。
//#include "pch.h"
//#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

using namespace std;



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    printf("intit\n");
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

