#pragma once
#pragma once

#include <windows.h>

#ifdef CYAPIWRAPPERDLL_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif


extern "C" MATHLIBRARY_API void __cdecl fibonacci_init(
    const unsigned long long a, const unsigned long long b);

// Produce the next value in the sequence.
// Returns true on success and updates current value and index;
// false on overflow, leaves current value and index unchanged.
extern "C" MATHLIBRARY_API bool __cdecl fibonacci_next();

// Get the current value in the sequence.
extern "C" MATHLIBRARY_API unsigned long long fibonacci_current();

// Get the position of the current value in the sequence.
extern "C" MATHLIBRARY_API unsigned __cdecl fibonacci_index();


extern "C" MATHLIBRARY_API void __cdecl init();
extern "C" MATHLIBRARY_API int __cdecl GetReceiveData(UINT32 * data,int index);

extern "C" MATHLIBRARY_API int __cdecl GetXferLength();
extern "C" MATHLIBRARY_API int __cdecl GetReceiveTotalNumber();

extern "C" MATHLIBRARY_API int __cdecl ConnectDevice();
extern "C" MATHLIBRARY_API int __cdecl StartSampleData();
extern "C" MATHLIBRARY_API int __cdecl EndSampleData();
extern "C" MATHLIBRARY_API const char* __cdecl  DeviceName();