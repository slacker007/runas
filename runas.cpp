
// @slacker007  http://cybersyndicates.com
// runas.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <userenv.h>
#include <iostream>
#pragma comment(lib, "Userenv.lib")
using namespace std;

//auto Menu_1();

auto Menu_1()
{
	printf("\n\n");
	printf( "\tMENU \n\n");
	printf("Please select from the following: \n\n");
	printf( "(a) Run an executable \n");
	printf( "(b) Return a Token \n");

	auto answer = 0;

	cin >> (answer);

	switch (answer)
	{
	case 'a':
		cout << "You have chosen to run an executable /n";
		break;
		return true;
	case 'b':
		cout << "You have chosen to create and return a token /n";
		return true;
		break;
	default:
		cout << "Sorry, invalid choice /n/n";
		return false;
		break;
	}


}

void DisplayError(LPWSTR pszAPI)
{
	LPVOID lpvMessageBuffer;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpvMessageBuffer, 0, NULL);

	//
	//... now display this string
	//
	wprintf(L"ERROR: API        = %s.\n", pszAPI);
	wprintf(L"       error code = %d.\n", GetLastError());
	wprintf(L"       message    = %s.\n", (LPWSTR)lpvMessageBuffer);

	//
	// Free the buffer allocated by the system
	//
	LocalFree(lpvMessageBuffer);

	ExitProcess(GetLastError());
}

void wmain(int argc, WCHAR *argv[])
{
	DWORD     dwSize;
	HANDLE    hToken;
	LPVOID    lpvEnv;
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO         si = { 0 };
	WCHAR               szUserProfile[256] = L"";

	si.cb = sizeof(STARTUPINFO);
	//(L"Usage: %s [user@domain] [password] [cmd]", argv[0]);

	Menu_1();

	if (argc != 4)
	{
		wprintf(L"\n\n");
		wprintf(L"Usage: %s [user@domain] [password] [cmd]", argv[0]);
		wprintf(L"\n\n");
		return;
	}

	//
	// TO DO: change NULL to '.' to use local account database
	//
	if (!LogonUser(argv[1],NULL, argv[2], LOGON32_LOGON_INTERACTIVE,
		LOGON32_PROVIDER_DEFAULT, &hToken))
		DisplayError(L"LogonUser");
	if (!CreateEnvironmentBlock(&lpvEnv, hToken, TRUE))
		DisplayError(L"CreateEnvironmentBlock");

	dwSize = sizeof(szUserProfile) / sizeof(WCHAR);
	if (!GetUserProfileDirectoryW(hToken, szUserProfile, &dwSize))
		DisplayError(L"GetUserProfileDirectory");

	//
	// TO DO: change NULL to '.' to use local account database
	//
	if (!CreateProcessWithLogonW(argv[1], NULL, argv[2],
		LOGON_WITH_PROFILE, NULL, argv[3],
		CREATE_UNICODE_ENVIRONMENT, lpvEnv, szUserProfile,
		&si, &pi))
		DisplayError(L"CreateProcessWithLogonW");

	if (!DestroyEnvironmentBlock(lpvEnv))
    	DisplayError(L"DestroyEnvironmentBlock");

	CloseHandle(hToken);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}


