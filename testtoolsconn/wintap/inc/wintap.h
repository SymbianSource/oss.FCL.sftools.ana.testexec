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
#include <time.h>
#include <IPHlpApi.h>

#include <winioctl.h>

// TAP driver definitions
#define TAP_IOCTL_GET_LASTMAC		CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD | 8000, 0 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define TAP_IOCTL_GET_MAC			CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD | 8000, 1 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define TAP_IOCTL_SET_STATISTICS	CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD | 8000, 2 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define TAP_IOCTL_GET_VERSION		CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD | 8000, 3 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define TAP_IOCTL_GET_MTU			CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD | 8000, 4 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define TAP_IOCTL_GET_INFO			CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD | 8000, 5 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define TAP_IOCTL_CONFIG_POINT_TO_POINT		CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD | 8000, 6 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define TAP_IOCTL_SET_MEDIA_STATUS	CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD | 8000, 7 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define TAP_IOCTL_CONFIG_DHCP_MASQ	CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD | 8000, 8 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define TAP_IOCTL_GET_LOG_LINE		CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD | 8000, 9 , METHOD_BUFFERED, FILE_ANY_ACCESS)

// TAP -v8 ioctls
#define TAP_CONTROL_CODE(request,method) \
  CTL_CODE (FILE_DEVICE_UNKNOWN, request, method, FILE_ANY_ACCESS)

#define TAP_IOCTL_GET_MAC_V8               TAP_CONTROL_CODE (1, METHOD_BUFFERED)
#define TAP_IOCTL_GET_VERSION_V8           TAP_CONTROL_CODE (2, METHOD_BUFFERED)
#define TAP_IOCTL_GET_MTU_V8               TAP_CONTROL_CODE (3, METHOD_BUFFERED)
#define TAP_IOCTL_GET_INFO_V8              TAP_CONTROL_CODE (4, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_POINT_TO_POINT_V8 TAP_CONTROL_CODE (5, METHOD_BUFFERED)
#define TAP_IOCTL_SET_MEDIA_STATUS_V8      TAP_CONTROL_CODE (6, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_DHCP_MASQ_V8      TAP_CONTROL_CODE (7, METHOD_BUFFERED)
#define TAP_IOCTL_GET_LOG_LINE_V8          TAP_CONTROL_CODE (8, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_DHCP_SET_OPT_V8   TAP_CONTROL_CODE (9, METHOD_BUFFERED)


// Redefinition
typedef struct _PACKET
	{	
	OVERLAPPED			OverLapped;	///< MUST BE FIRST
	void			*	Buffer; 	///< ptr to buffer
	DWORD				Length;
	unsigned int		Queued;
	} PACKET, *PPACKET;

typedef void (*TIsr)(void const* aObject, int aErr, u_char* pkt_data, DWORD* aLength);

void SetIsr(void * aObject, TIsr aIsr);
int InitDriver(HANDLE& iWinTapHandle, HANDLE& iThreadHandle, PACKET& iWritePacket, IP_ADAPTER_INFO& iAdapterinfo);
void DoClose(HANDLE& iWinTapHandle, HANDLE& iThreadHandle, PACKET& iWritePacket);
int DoSend(PACKET& iWritePacket, HANDLE& iWinTapHandle);