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
* Switches
*
*/




#ifndef __STANDARD_UNIX_H__
#define __STANDARD_UNIX_H__

// COMPILE ONLY - these defs get things to compile - not work!!!
#ifdef WIN32
#define mktime(x)		((time_t)0)
#define strptime(a,b,c)	
#define ioctl(a,b,c)	(0)
#endif

// COMPILE ONLY - Windows doesn't define SIGKILL
#ifdef WIN32
#define SIGKILL 0
#endif

// COMPILE ONLY - Ioctl constants
#ifdef WIN32
#define SIOCGIFDSTADDR 0
#define SIOCSIFADDR 0
#define SIOCSIFDSTADDR 0
struct ifreq {
	char *ifr_name;
	struct sockaddr ifr_addr;
	struct sockaddr ifr_dstaddr;
};
#endif

// Windows doesn't have getppid()
#ifdef WIN32
#define getppid()	(-1)
#endif

// Windows has inet_addr instead of inet_aton
#ifdef WIN32
#define inet_aton(str,pstruct)	(((pstruct)->ADDRESS_INTEGER = inet_addr(str)), 1)
#endif

// Windows has different naming conventions for some string methods.
#ifdef WIN32
#define	snprintf	_snprintf
#define strncasecmp	_strnicmp
#endif

// Windows and Linux have different Sleep function - Windows has milliseconds while Linux
// just has seconds. This is dangerous - so I replace with a standard millisecond version
#ifndef WIN32
#define SleepForMilliseconds(x)		sleep(((x)/1000))
#else
#define SleepForMilliseconds(x)		Sleep((x))
#endif

// Windows doesn't like these includes - but almost all use them - so include them here to collect hash-if-defs
#ifndef WIN32
#include <unistd.h>
#include <errno.h>
#endif

// Windows doesn't define snprintf 
#ifdef WIN32
#define snprintf _snprintf
#endif

// Windows doesn't follow berkely in_addr perfectly
#ifdef WIN32
#define ADDRESS_INTEGER S_un.S_addr
#else
#define ADDRESS_INTEGER s_addr
#endif

// Windows doesn't define IFNAMSIZ
#ifdef WIN32
#define IFNAMSIZ			100
#endif

// Windows doesn't have pid_t
#ifdef WIN32
#define pid_t int
#endif

// Windows doesn't have sighandler_t
#ifdef WIN32
typedef void (*sighandler_t)(int);
#endif

#endif
