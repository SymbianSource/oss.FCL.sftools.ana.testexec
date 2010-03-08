/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/


#include <windows.h>
#include <stdio.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <malloc.h>
#include <newdev.h>

#ifdef _UNICODE
#define UPDATEDRIVERFORPLUGANDPLAYDEVICES "UpdateDriverForPlugAndPlayDevicesW"
#else
#define UPDATEDRIVERFORPLUGANDPLAYDEVICES "UpdateDriverForPlugAndPlayDevicesA"
#endif

typedef BOOL (WINAPI *PUPDATEDRIVERFORPLUGANDPLAYDEVICES)(HWND hwndParent,
                                                         LPCTSTR HardwareId,
                                                         LPCTSTR FullInfPath,
                                                         DWORD InstallFlags,
                                                         PBOOL bRebootRequired OPTIONAL
                                                         );

int UpdateDevice(LPCTSTR inf, LPCTSTR hwid)
{
    HMODULE hMod = NULL;
    int nRet = -1;
    PUPDATEDRIVERFORPLUGANDPLAYDEVICES pUpdate;
    BOOL bReboot = FALSE;
    DWORD dwFlags = 0;
    DWORD res;
    TCHAR InfPath[MAX_PATH];

    res = GetFullPathName(inf,MAX_PATH,InfPath,NULL);
    if((res < MAX_PATH) && (res != 0)) {
		if(GetFileAttributes(InfPath) != (DWORD)(-1)) {
			inf = InfPath;
			dwFlags |= INSTALLFLAG_FORCE;

			hMod = LoadLibrary(TEXT("newdev.dll"));
			if(hMod) {
				pUpdate = (PUPDATEDRIVERFORPLUGANDPLAYDEVICES)GetProcAddress(hMod,UPDATEDRIVERFORPLUGANDPLAYDEVICES);
				if(pUpdate)
				{
					if(pUpdate(NULL,hwid,inf,dwFlags,&bReboot)) {
						nRet = 0;
					}
				}
			}
		}
	}

    if(hMod) {
        FreeLibrary(hMod);
    }

    return -1;
}



int install(LPCTSTR inf, LPCTSTR hwid)
{
	
    HDEVINFO hDeviceInfoSet = INVALID_HANDLE_VALUE;
    SP_DEVINFO_DATA DeviceInfoData;
    GUID ClassGUID;
    TCHAR ClassName[MAX_CLASS_NAME_LEN];
    TCHAR hwIdList[LINE_LEN+4];
    TCHAR InfPath[MAX_PATH];
    int nRet = -1;
    DWORD flags = 0;

    if(GetFullPathName(inf,MAX_PATH,InfPath,NULL) < MAX_PATH) {
		
		ZeroMemory(hwIdList,sizeof(hwIdList));
		lstrcpyn(hwIdList,hwid,LINE_LEN);

		if (SetupDiGetINFClass(InfPath,&ClassGUID,ClassName,sizeof(ClassName)/sizeof(ClassName[0]),0))
		{

			hDeviceInfoSet = SetupDiCreateDeviceInfoList(&ClassGUID,0);
			if(hDeviceInfoSet != INVALID_HANDLE_VALUE)
			{

				DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
				if (SetupDiCreateDeviceInfo(hDeviceInfoSet,
					ClassName,
					&ClassGUID,
					NULL,
					0,
					DICD_GENERATE_ID,
					&DeviceInfoData))
				{

					if(SetupDiSetDeviceRegistryProperty(hDeviceInfoSet,
						&DeviceInfoData,
						SPDRP_HARDWAREID,
						(LPBYTE)hwIdList,
						(lstrlen(hwIdList)+1+1)*sizeof(TCHAR)))
					{

						if (SetupDiCallClassInstaller(DIF_REGISTERDEVICE,
							hDeviceInfoSet,
							&DeviceInfoData))
						{
							// update the driver for the device we just created
							//
							nRet = UpdateDevice(inf,hwid);
						}
					}
				}

				if (hDeviceInfoSet != INVALID_HANDLE_VALUE) {
					SetupDiDestroyDeviceInfoList(hDeviceInfoSet);
				}
			}
		}
	}
	return nRet;
}



BOOL IsWow64()
{
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE hProcess, BOOL *Wow64Process);
	BOOL bIsWow64 = FALSE;
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress( GetModuleHandle("kernel32"),"IsWow64Process");
	if( NULL != fnIsWow64Process)
	{
		if( !fnIsWow64Process( GetCurrentProcess(), &bIsWow64))
		{
		// handle error
		}
	}
	return bIsWow64;
}

void CreateChildProcessAndWaitUntilDone(const LPSTR strCmdLine) 
{
	PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO siStartInfo;

	// Set up members of STARTUPINFO structure.
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.lpReserved = NULL;
	siStartInfo.lpReserved2 = NULL; 
	siStartInfo.cbReserved2 = 0;
	siStartInfo.lpDesktop = NULL; 
	siStartInfo.dwFlags = 0;


	// Create the child process.
	CreateProcess(
		NULL,
		strCmdLine,
		NULL, // process security attributes
		NULL, // primary thread security attributes
		0, // handles are inherited
		0, // creation flags
		NULL, // use parent's environment
		NULL, // use parent's current directory
		&siStartInfo, // STARTUPINFO pointer
		&piProcInfo); // receives PROCESS_INFORMATION

	// Wait for the processs to finish
	DWORD rc = WaitForSingleObject(
	piProcInfo.hProcess, // process handle
	INFINITE); 
}
void main(int argc, LPTSTR argv[])
{
	int a=0;

	if(IsWow64())
	{
		SetCurrentDirectory("x64driver");
		CreateChildProcessAndWaitUntilDone("tapinstaller.x64.exe");
	}
	else 
	{
		OSVERSIONINFO ver;
		ver.dwOSVersionInfoSize = sizeof(ver);
		GetVersionEx(&ver);
		if(ver.dwMajorVersion == 6)
		{
			LPTSTR inf="tap0901.inf";
			LPTSTR hwid = "tap0901";
			install(inf,hwid);
		}
		else 
		{
			LPTSTR inf="OemWin2k.inf";
			LPTSTR hwid = "tap0801";
			install(inf,hwid);
		}
	}
	return;
}
