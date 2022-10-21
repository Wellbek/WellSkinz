#include "Functions.h"
#include "Globals.h"
#include <iostream>

using namespace std;


// Funktionen zum Erkennen des Programms
bool AttachProcess(const char* processName)
{
	PROCESSENTRY32 procEntry32;
	procEntry32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcSnap == INVALID_HANDLE_VALUE)
		return false;

	while (Process32Next(hProcSnap, &procEntry32))
	{
		if (!strcmp(processName, procEntry32.szExeFile))
		{
			cout << processName << " successfully found." << endl;
			HANDLE temp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
			if (temp == INVALID_HANDLE_VALUE) {
				CloseHandle(hProcSnap);
				return false;
			}

			pID = procEntry32.th32ProcessID;
			pHandle = temp;

			CloseHandle(hProcSnap);
			return true;
		}
	}

	cout << "ERROR: " << processName << " couldnt be found." << endl;
	CloseHandle(hProcSnap);
	return false;
}

DWORD GetModule(const char* moduleName)
{
	HANDLE hSnapShot;
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); //sammelt alle Prozesse, heaps, Module und threads im System //pID muss bei TH32CS_SNAPMODULE 0 sein laut https://docs.microsoft.com/en-us/windows/win32/api/tlhelp32/nf-tlhelp32-createtoolhelp32snapshot 

	MODULEENTRY32 mEntry;					//Listen Eintrag der Module https://docs.microsoft.com/en-us/windows/win32/api/tlhelp32/ns-tlhelp32-moduleentry32	
	mEntry.dwSize = sizeof(MODULEENTRY32);

	if (hSnapShot == INVALID_HANDLE_VALUE) {
		CloseHandle(hSnapShot);
		return 0;
	}

	//Prueft erstes Element der Liste an Modulen im System, ob der Name des Modules mit dem Parameter uebereinstimmt.
	//Stimmen sie ueberein, wird die Liste geschlossen bzw. entfernt und die Adresse des ersten Elements bzw. des Moduls returnt.
	if (Module32First(hSnapShot, &mEntry))
	{
		if (!strcmp(moduleName, mEntry.szModule))
		{
			cout << moduleName << " successfully found." << endl;
			CloseHandle(hSnapShot);
			return (DWORD)mEntry.modBaseAddr;
		}
	}

	//Prueft alle Elemente der Liste an Modulen im System, bis auf das erste, ob der Name des Modules mit dem Parameter uebereinstimmt.
	//Stimmen sie ueberein, wird die Liste geschlossen bzw. entfernt und die Adresse des uebereinstimmenden Elementes und somit Moduls returnt.
	while (Module32Next(hSnapShot, &mEntry))
	{
		if (!strcmp(moduleName, mEntry.szModule)) //stringcompare, also Vergleich von Strings (warum ! verstehe ich auch nicht, somit wird es in c++ ganz normal verglichen; ohne ! funktioniert es nicht)
		{
			cout << moduleName << " successfully found." << endl;
			CloseHandle(hSnapShot);
			return (DWORD)mEntry.modBaseAddr;
		}
	}
	cout << "ERROR: " << moduleName << " couldnt be found." << endl;
	CloseHandle(hSnapShot);
	return false; // Stimmt nichts �berein wird die Methode beendet, jedoch muss etwas returnt werden. Da NULL auch ein g�ltiges Ergebnis fuer ein DWORD Datentyp ist, mit dem er weiter arbeiten kann, wird false returnt.
}
