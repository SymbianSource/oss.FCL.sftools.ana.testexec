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



#include "wintap.h"

#include <assert.h>
#include <stdio.h>

#ifdef EKA2
#include <platform/emulator.h>
#endif
/**
 *
 *	The start of most EtherTap specific routines
 *
 **/

static TIsr GlobIsr = NULL;
static void * GlobObj = NULL;

static int MatchTunAdapter(const char* aDevName)
	{
	const char match[] = "TAP";
	const char matchl[] = "tap";

	// Check the first 3 charecters
	if (aDevName[0] == match[0]
		&& aDevName[1] == match[1]
		&& aDevName[2] == match[2])
		return 1;
	else if (aDevName[0] == matchl[0]
		&& aDevName[1] == matchl[1]
		&& aDevName[2] == matchl[2])
		return 1;
	else
		return 0;
	}

static int GetIfName(IP_ADAPTER_INFO& iAdapterinfo)
	{
	DWORD			numInterfaces;
    HMODULE hModule = LoadLibraryA("iphlpapi.dll");
    if(!hModule)
        return -1;
    DWORD (WINAPI * pfGetNumberOfInterfaces)(PDWORD) = (DWORD (WINAPI * )(PDWORD))GetProcAddress(hModule, "GetNumberOfInterfaces");
    DWORD (WINAPI * pfGetAdaptersInfo)(PIP_ADAPTER_INFO,PULONG) = (DWORD (WINAPI *)(PIP_ADAPTER_INFO,PULONG))GetProcAddress(hModule, "GetAdaptersInfo");

	// weiredness alert - windows returns found adapters+1
	if (pfGetNumberOfInterfaces(&numInterfaces) != ERROR_SUCCESS)
		return -1;

	ULONG            ulOutBufLen;
	ulOutBufLen = sizeof(IP_ADAPTER_INFO) * (numInterfaces);
	
	IP_ADAPTER_INFO  *pAdapterInfo = NULL;

	pAdapterInfo = new IP_ADAPTER_INFO[numInterfaces];
	
	if (pAdapterInfo && (pfGetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS)) 
		{
		delete pAdapterInfo;
		return -1;
		}

	IP_ADAPTER_INFO  *pCurAdapterInfo = pAdapterInfo;
	int count = 1, matchCount = 0;
	while (pCurAdapterInfo != NULL)
		{	
		// Optimize for when we already have the index
		if (iAdapterinfo.Index == pCurAdapterInfo->Index)
			{
			matchCount++;
			iAdapterinfo = *pCurAdapterInfo;
			break;
			}

		if (MatchTunAdapter(pCurAdapterInfo->Description))
			{
			matchCount++;
			iAdapterinfo = *pCurAdapterInfo;
			}

		pCurAdapterInfo = pCurAdapterInfo->Next;
		count++;
		}

	if (matchCount == 0)
		{
		delete[] pAdapterInfo;
		return -1;
		}
	else if (matchCount > 1)
		{}
	iAdapterinfo.Next = NULL;
	delete[] pAdapterInfo;
	return 0;
	}

static int WorkThread(HANDLE * iWinTapHandle);

int InitDriver(HANDLE& iWinTapHandle, HANDLE& iThreadHandle, PACKET& iWritePacket, IP_ADAPTER_INFO& iAdapterinfo)
// Sets up the PDD
	{
	int ret = 0;

	if ((ret = GetIfName(iAdapterinfo)) != 0)
		return ret;	

	// Fromat the GUID to get the file name
	char szFileName[256];	
	sprintf(szFileName, ("%s%s%s"), ("\\\\.\\"), iAdapterinfo.AdapterName, (".tap"));

	iWinTapHandle = CreateFileA(szFileName,
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_OVERLAPPED,
								NULL);

	if (iWinTapHandle == INVALID_HANDLE_VALUE)
		{
		//OUTPUT_ERROR("could not open file handle\n");
		CloseHandle(iWinTapHandle);
		ret = -1;
		return ret;
		}

	
	// set the device to connected
	ULONG status = 1;
	DWORD len;
	if (!DeviceIoControl(iWinTapHandle,
		TAP_IOCTL_SET_MEDIA_STATUS ,&status, sizeof(status), &status, sizeof(status), &len, NULL))
		{
		// FUDGE alert
		if (!DeviceIoControl(iWinTapHandle,
			TAP_IOCTL_SET_MEDIA_STATUS_V8 ,&status, sizeof(status), &status, sizeof(status), &len, NULL))
			{
			//OUTPUT_ERROR("Could not set media status to open\n");
			return ret;
			}
		}
	
	// Call GetDriverName again, this time to reset the correct info for the connected Device
	if((ret = GetIfName(iAdapterinfo)) != 0)
		return ret;

	iWritePacket.OverLapped.hEvent 		= CreateEvent(NULL, TRUE, FALSE, NULL);

	u_long WorkerThreadId;
	    
	if ((iThreadHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)WorkThread, &iWinTapHandle,
		CREATE_SUSPENDED,&WorkerThreadId)) != NULL)
		{
		ret = 0;
		}
	else
		{
		//OUTPUT_ERROR("CreateThread() failed to create worker thread");
		}

	return ret;
	}

void DoClose(HANDLE& iWinTapHandle, HANDLE& iThreadHandle, PACKET& iWritePacket)
	{
	if (iWinTapHandle != INVALID_HANDLE_VALUE)
		{
		// Cancel reads, Writes 
		CancelIo(iWinTapHandle);

		// Close read and write events
		CloseHandle(iWritePacket.OverLapped.hEvent);
		
		// Close the read thread
		if (!TerminateThread(iThreadHandle, 0))
			{
			//__KTRACE_OPT(KHARDWARE, Kern::Printf("Could not Terminate thread\n"));
			}
		CloseHandle(iThreadHandle);

		DWORD status = 0, len;
		if (!DeviceIoControl(iWinTapHandle,
			TAP_IOCTL_SET_MEDIA_STATUS ,&status, sizeof(status), &status, sizeof(status), &len, NULL))
			{
			// Big fudge --
			if (!DeviceIoControl(iWinTapHandle,
				TAP_IOCTL_SET_MEDIA_STATUS_V8 ,&status, sizeof(status), &status, sizeof(status), &len, NULL))
				{
				//__KTRACE_OPT(KHARDWARE, Kern::Printf("Could not set media status to close\n"));
				}
			}

		CloseHandle(iWinTapHandle);
		iWinTapHandle = INVALID_HANDLE_VALUE;
		}
	}

void SetIsr(void * aObject, TIsr aIsr)
	{
	assert(aIsr != NULL);
	assert(aObject != NULL);
	GlobObj = aObject;
	GlobIsr = aIsr;
	}

static int WorkThread(HANDLE * iWinTapHandle)
	{
    //__KTRACE_OPT(KHARDWARE, Kern::Printf("WorkThread(DEthernetWins * aDEthernetWins)"));
	int err = 0;

	u_char			buffer[2000];
	PACKET			m_ReadPacket;

	m_ReadPacket.OverLapped.hEvent 		= CreateEvent(NULL, TRUE, FALSE, NULL);
	
	while( err == 0 )
		{		
		m_ReadPacket.OverLapped.Offset 		= 0;
		m_ReadPacket.OverLapped.OffsetHigh 	= 0;
		m_ReadPacket.OverLapped.Internal	= 0;
		m_ReadPacket.OverLapped.InternalHigh= 0;
		m_ReadPacket.Buffer					= buffer;
		m_ReadPacket.Length					= 0;

    	//__KTRACE_OPT(KHARDWARE, Kern::Printf("Starting to Read \n"));
	    if (*iWinTapHandle != INVALID_HANDLE_VALUE)
			{
			if (!ReadFile(*iWinTapHandle, m_ReadPacket.Buffer, 2000, 
				&(m_ReadPacket.Length), &m_ReadPacket.OverLapped))
				{
				if(GetLastError() != ERROR_IO_PENDING) 
					{
					//__KTRACE_OPT(KHARDWARE, Kern::Printf("m_ReadPacket() Failed "));
					err = -3;
					break;
					}
				
				DWORD Result = WaitForSingleObject(m_ReadPacket.OverLapped.hEvent, INFINITE);

				switch(Result)
					{
					// OVERLAPPED structure's event has been signaled. 
					case WAIT_OBJECT_0:
						if (GetOverlappedResult(*iWinTapHandle, &m_ReadPacket.OverLapped, 
							&m_ReadPacket.Length, FALSE) == 0)
							{
    						//__KTRACE_OPT(KHARDWARE, Kern::Printf("** Error in m_ReadPacket() **\n"));
							err = -3;
							}
						else
							{
							// m_ReadPacket operation completed successfully.
    						//__KTRACE_OPT(KHARDWARE, Kern::Printf("** Completed in m_ReadPacket() **\n"));
							err = 0;
							}
						break;
					default:
						 // An error has occurred in WaitForSingleObject.
						 // This usually indicates a problem with the
						// OVERLAPPED structure's event handle.
						err = -3;
						break;
					}
				}
			else
				{
				err = 0;
    			//__KTRACE_OPT(KHARDWARE, Kern::Printf("** Completed in m_ReadPacket() **\n"));
				}			
			}
		else
			{
    		//__KTRACE_OPT(KHARDWARE, Kern::Printf("ReadPacket() No adaptor open\n"));
			err = -1;
			}
		GlobIsr(GlobObj, err, buffer, &m_ReadPacket.Length);
		}
	CloseHandle(m_ReadPacket.OverLapped.hEvent);
	return -3;
	}

int DoSend(PACKET& iWritePacket, HANDLE& iWinTapHandle)
	{
    //__KTRACE_OPT(KHARDWARE, Kern::Printf("DEthernetWins::Send(TBuf8<KMaxEthernetPacket+32> &aBuffer)"));
    int err;
    if ( iWinTapHandle != INVALID_HANDLE_VALUE )
		{
		iWritePacket.OverLapped.Offset 		= 0;
		iWritePacket.OverLapped.OffsetHigh 	= 0;
		iWritePacket.OverLapped.Internal		= 0;
		iWritePacket.OverLapped.InternalHigh	= 0;
		
		if(!WriteFile(iWinTapHandle, iWritePacket.Buffer, iWritePacket.Length, NULL, &iWritePacket.OverLapped))
			{
			if(GetLastError() != ERROR_IO_PENDING) 
				{
				//__KTRACE_OPT(KHARDWARE, Kern::Printf("WriteFile() Failed  "));
				err = -23; //KErrWrite
				return err;
				}
			else	//Pending Write
				{
				DWORD written;
				DWORD Result = WaitForSingleObject(iWritePacket.OverLapped.hEvent, INFINITE);
				switch(Result)
					{
					// OVERLAPPED structure's event has been signaled. 
					case WAIT_OBJECT_0:
						if (!GetOverlappedResult(iWinTapHandle, &iWritePacket.OverLapped, &written, FALSE))
							{
			            	//__KTRACE_OPT(KHARDWARE, Kern::Printf("** Error in WritePacket() **\n"));
							err = -23; //KErrWrite
							}
						else
							{
							// Write operation completed successfully.
            				//__KTRACE_OPT(KHARDWARE, Kern::Printf("** Completed in WritePacket() **"));
							err = 0;
							}
						break;
					default:
						 // An error has occurred in WaitForSingleObject.
						 // This usually indicates a problem with the
						// OVERLAPPED structure's event handle.
						err = -23; //KErrWrite
						break;
					}
				}
			}
		else //Write went fine
			{
            //__KTRACE_OPT(KHARDWARE, Kern::Printf("** Completed in WritePacket() **\n"));
			err = 0;
			}
		}
    else
		{
        //__KTRACE_OPT(KHARDWARE, Kern::Printf("WritePacket() No adaptor open\n"));
		err = -12; //KErrPathNotFound
		}
    return err;
	}
