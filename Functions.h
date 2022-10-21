#pragma once
#include <Windows.h>
#include <TlHelp32.h>

#include <xmmintrin.h>

#include "Offsets.h"
#include "Globals.h"

//rpm und wpm (write und read process memory)
template <class T>
void wpm(DWORD address, T value)
{
	WriteProcessMemory(pHandle, (PVOID)address, &value, sizeof(T), 0);
}
template <class T>
T rpm(DWORD address)
{
	T data;
	bool state = ReadProcessMemory(pHandle, (PVOID)address, &data, sizeof(T), 0);
	if (state)
		return data;
}

bool AttachProcess(const char* processName);
DWORD GetModule(const char* moduleName);