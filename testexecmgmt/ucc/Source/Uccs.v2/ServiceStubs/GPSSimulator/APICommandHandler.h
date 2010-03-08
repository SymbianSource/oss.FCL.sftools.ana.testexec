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




#ifndef __APICOMMANDHANDLER_H__
#define __APICOMMANDHANDLER_H__
#include <rpc/types.h>
#include <rpc/rpc.h>

#define ERR_NONE 0
#define ERR_INVALID_ERROR_CODE -16
#define ERR_SERVICE_ALREADY_STARTED -1
#define ERR_SERVICE_NOT_RUNNING -2
#define ERR_ACTIVE_USER_SESSIONS -3
#define ERR_FAILED_TO_REMOVE_ACTIVE_SESSIONS -4
#define ERR_FAILED_TO_CREATE_COMPONENT_MANAGER -5
#define ERR_FAILED_TO_RETRIEVE_KEY -6
#define ERR_INSTANCE_DOES_NOT_EXIST -7
#define ERR_CANNOT_CREATE_NEW_INSTANCE -9
#define ERR_MISSING_PARAMETER -17
#define ERR_INVALID_METHOD -8
#define ERR_STUB_ALREADY_CONNECTED -10
#define ERR_FAILED_TO_CONNECT -11
#define ERR_STUB_NOT_CONNECTED -12
#define ERR_INVALID_RV_POINTER -13
#define ERR_RPC_ERROR -14
#define ERR_INVALID_SERVICE -15
#define ERR_INVALIDARG -50
#define ERR_INVALIDSTATE -51
#define ERR_GENERAL -52
#define INFO_MAXIMUM_OBJECTS 256

typedef struct {
	u_int TComponentList_len;
	int *TComponentList_val;
} TComponentList;
#ifdef __cplusplus
extern "C" {
bool_t xdr_APICommandHandler_TComponentList(...);
}
#else
bool_t xdr_APICommandHandler_TComponentList();
#endif

#define MAXFIELDLENGTH 128
#define MAXSTRINGLENGTH 1024

typedef int FIELD_ID;
#ifdef __cplusplus
extern "C" {
bool_t xdr_FIELD_ID(...);
}
#else
bool_t xdr_FIELD_ID();
#endif

#define F_NUMBER_OF_FIELDS 56
#define F_NULL_ID 0
#define F_TIME 1
#define F_STATUS 2
#define F_DATE 3
#define F_LATITUDE 4
#define F_LAT_NORTH_SOUTH 5
#define F_LONGITUDE 6
#define F_LON_EAST_WEST 7
#define F_SPEED_OVER_GROUND 8
#define F_COURSE_OVER_GROUND 9
#define F_MAGNETIC_VARIATION 10
#define F_MAG_EAST_WEST 11
#define F_MODE_INDICATOR 12
#define F_OPERATION_MODE 13
#define F_FIX_MODE 14
#define F_SATELLITE_ONE 15
#define F_SATELLITE_TWO 16
#define F_SATELLITE_THREE 17
#define F_SATELLITE_FOUR 18
#define F_SATELLITE_FIVE 19
#define F_SATELLITE_SIX 20
#define F_SATELLITE_SEVEN 21
#define F_SATELLITE_EIGHT 22
#define F_SATELLITE_NINE 23
#define F_SATELLITE_TEN 24
#define F_SATELLITE_ELEVEN 25
#define F_SATELLITE_TWELVE 26
#define F_PDOP 27
#define F_HDOP 28
#define F_VDOP 29
#define F_SATELLITES_IN_USE 30
#define F_ANTENNA_ALTITUDE 31
#define F_ANTENNA_METERS 32
#define F_GEOIDAL_SEPARATION 33
#define F_GEOIDAL_METERS 34
#define F_DIFFERENTIAL_AGE 35
#define F_DIFFERENTIAL_REF_ID 36
#define F_GPS_QUALITY 37
#define F_NUMBER_OF_SENTENCES 38
#define F_SENTENCE_NUMBER 39
#define F_SATELLITE_ID_NUMBER_ONE 40
#define F_SATELLITE_ID_NUMBER_TWO 41
#define F_SATELLITE_ID_NUMBER_THREE 42
#define F_SATELLITE_ID_NUMBER_FOUR 43
#define F_SATELLITE_ELEVATION_ONE 44
#define F_SATELLITE_ELEVATION_TWO 45
#define F_SATELLITE_ELEVATION_THREE 46
#define F_SATELLITE_ELEVATION_FOUR 47
#define F_SATELLITE_AZIMUTH_ONE 48
#define F_SATELLITE_AZIMUTH_TWO 49
#define F_SATELLITE_AZIMUTH_THREE 50
#define F_SATELLITE_AZIMUTH_FOUR 51
#define F_SATELLITE_SNR_ONE 52
#define F_SATELLITE_SNR_TWO 53
#define F_SATELLITE_SNR_THREE 54
#define F_SATELLITE_SNR_FOUR 55
#define F_CHECKSUM 56

typedef int STATE;
#ifdef __cplusplus
extern "C" {
bool_t xdr_STATE(...);
}
#else
bool_t xdr_STATE();
#endif

#define S_NULL 0
#define S_VALUE 1
#define S_OMIT 2
#define S_CALCULATE 3

typedef int NORTH_SOUTH;
#ifdef __cplusplus
extern "C" {
bool_t xdr_NORTH_SOUTH(...);
}
#else
bool_t xdr_NORTH_SOUTH();
#endif

#define NORTH 0
#define SOUTH 1

typedef int EAST_WEST;
#ifdef __cplusplus
extern "C" {
bool_t xdr_EAST_WEST(...);
}
#else
bool_t xdr_EAST_WEST();
#endif

#define EAST 0
#define WEST 1

typedef int SENTENCE_ID;
#ifdef __cplusplus
extern "C" {
bool_t xdr_SENTENCE_ID(...);
}
#else
bool_t xdr_SENTENCE_ID();
#endif

#define RMC 1
#define GGA 2
#define GSA 3
#define GNS 4
#define GSV 5

typedef int SATELLITE_ID;
#ifdef __cplusplus
extern "C" {
bool_t xdr_SATELLITE_ID(...);
}
#else
bool_t xdr_SATELLITE_ID();
#endif

#define SAT_UNKNOWN -1
#define SAT_FIRST 1
#define SAT_TWO 2
#define SAT_THREE 3
#define SAT_FOUR 4
#define SAT_FIVE 5
#define SAT_SIX 6
#define SAT_SEVEN 7
#define SAT_EIGHT 8
#define SAT_NINE 9
#define SAT_TEN 10
#define SAT_ELEVEN 11
#define SAT_LAST 12

typedef int ErrorCode;
#ifdef __cplusplus
extern "C" {
bool_t xdr_ErrorCode(...);
}
#else
bool_t xdr_ErrorCode();
#endif

#define ERR_OK 0
#define ERR_INVALID_FIELD -100
#define ERR_NO_SUCH_FIELD -101
#define ERR_INVALID_ID -102
#define ERR_INVALID_INDEX -103
#define ERR_DEFAULT_NOT_SET -104
#define ERR_INVALID_DELAY -105
#define ERR_BAD_SENTENCE -106
#define ERR_RESET_FAILED -107
#define ERR_CLEAR_FAILED -108
#define ERR_SATELLITE_INVALID -109
#define ERR_ADD_FAILED -110
#define ERR_CONSTRUCT_FAILED -111
#define ERR_INVALID_VALUE -112
#define ERR_TOO_MANY_SATELLITES -113
#define ERR_INVALID_PORT -114
#define ERR_STOP_FAILED -115
#define ERR_START_FAILED -116
#define ERR_SERIAL_ERROR -117
#define ERR_NOT_CONFIGURED -118
#define ERR_INVALID_COURSE -119
#define ERR_INVALID_CHUNK_SIZE -120
#define ERR_UNKNOWN -121

struct TStartupInfo {
	int iEmpty;
};
typedef struct TStartupInfo TStartupInfo;
#ifdef __cplusplus
extern "C" {
bool_t xdr_APICommandHandler_TStartupInfo(...);
}
#else
bool_t xdr_APICommandHandler_TStartupInfo();
#endif


struct TPositionInfo {
	double iLatitude;
	bool_t iNS;
	double iLongitude;
	bool_t iEW;
};
typedef struct TPositionInfo TPositionInfo;
#ifdef __cplusplus
extern "C" {
bool_t xdr_APICommandHandler_TPositionInfo(...);
}
#else
bool_t xdr_APICommandHandler_TPositionInfo();
#endif


struct TSatellite {
	int iElevation;
	int iAzimuth;
	int iSNR;
	int iId;
};
typedef struct TSatellite TSatellite;
#ifdef __cplusplus
extern "C" {
bool_t xdr_APICommandHandler_TSatellite(...);
}
#else
bool_t xdr_APICommandHandler_TSatellite();
#endif


struct TField {
	int iFieldId;
	int iState;
	char iValue[MAXFIELDLENGTH];
};
typedef struct TField TField;
#ifdef __cplusplus
extern "C" {
bool_t xdr_APICommandHandler_TField(...);
}
#else
bool_t xdr_APICommandHandler_TField();
#endif


struct TAppendSentence {
	int iSentenceId;
	int iFrequency;
};
typedef struct TAppendSentence TAppendSentence;
#ifdef __cplusplus
extern "C" {
bool_t xdr_APICommandHandler_TAppendSentence(...);
}
#else
bool_t xdr_APICommandHandler_TAppendSentence();
#endif


struct TAppendString {
	char iUserString[MAXSTRINGLENGTH];
	int iFrequency;
};
typedef struct TAppendString TAppendString;
#ifdef __cplusplus
extern "C" {
bool_t xdr_APICommandHandler_TAppendString(...);
}
#else
bool_t xdr_APICommandHandler_TAppendString();
#endif


struct TAppendUserSentence {
	TAppendSentence iAppendSentence;
	struct {
		u_int iUserFields_len;
		TField *iUserFields_val;
	} iUserFields;
};
typedef struct TAppendUserSentence TAppendUserSentence;
#ifdef __cplusplus
extern "C" {
bool_t xdr_APICommandHandler_TAppendUserSentence(...);
}
#else
bool_t xdr_APICommandHandler_TAppendUserSentence();
#endif


struct TCourse {
	double iSpeed;
	double iDirection;
};
typedef struct TCourse TCourse;
#ifdef __cplusplus
extern "C" {
bool_t xdr_APICommandHandler_TCourse(...);
}
#else
bool_t xdr_APICommandHandler_TCourse();
#endif


struct TAccuracy {
	double iAccuracy;
	double iHWConstant;
};
typedef struct TAccuracy TAccuracy;
#ifdef __cplusplus
extern "C" {
bool_t xdr_APICommandHandler_TAccuracy(...);
}
#else
bool_t xdr_APICommandHandler_TAccuracy();
#endif


#define GPSSIMULATOR ((u_long)0x34630666)
#define GPSSIMULATOR_VERSION ((u_long)1)
#define SS_STARTUPRPCSERVICE ((u_long)1)
#ifdef __cplusplus
extern "C" {
extern int *ss_startuprpcservice_10( TStartupInfo * arg, CLIENT *cl );
}
#else
extern int *ss_startuprpcservice_10();
#endif /* __cplusplus */
#define SC_SHUTDOWNRPCSERVICE ((u_long)2)
#ifdef __cplusplus
extern "C" {
extern int *sc_shutdownrpcservice_10( int * arg, CLIENT *cl );
}
#else
extern int *sc_shutdownrpcservice_10();
#endif /* __cplusplus */
#define LIST_CONNECTIONS ((u_long)30)
#ifdef __cplusplus
extern "C" {
extern TComponentList *list_connections_1( void * arg, CLIENT *cl );
}
#else
extern TComponentList *list_connections_1();
#endif /* __cplusplus */
#define CSTR_STARTPROCESS ((u_long)31)
#ifdef __cplusplus
extern "C" {
extern int *cstr_startprocess_1( char ** arg, CLIENT *cl );
}
#else
extern int *cstr_startprocess_1();
#endif /* __cplusplus */
#define DSTR_REMOVEPROCESS ((u_long)32)
#ifdef __cplusplus
extern "C" {
extern int *dstr_removeprocess_1( int * arg, CLIENT *cl );
}
#else
extern int *dstr_removeprocess_1();
#endif /* __cplusplus */
#define STARTSIMULATOR ((u_long)10)
#ifdef __cplusplus
extern "C" {
extern int *startsimulator_1( void * arg, CLIENT *cl );
}
#else
extern int *startsimulator_1();
#endif /* __cplusplus */
#define STOPSIMULATOR ((u_long)11)
#ifdef __cplusplus
extern "C" {
extern int *stopsimulator_1( void * arg, CLIENT *cl );
}
#else
extern int *stopsimulator_1();
#endif /* __cplusplus */
#define SETFIELDDEFAULT ((u_long)12)
#ifdef __cplusplus
extern "C" {
extern int *setfielddefault_1( TField * arg, CLIENT *cl );
}
#else
extern int *setfielddefault_1();
#endif /* __cplusplus */
#define SETSATELLITEDEFAULT ((u_long)13)
#ifdef __cplusplus
extern "C" {
extern int *setsatellitedefault_1( TSatellite * arg, CLIENT *cl );
}
#else
extern int *setsatellitedefault_1();
#endif /* __cplusplus */
#define POSITIONSET ((u_long)14)
#ifdef __cplusplus
extern "C" {
extern int *positionset_1( TPositionInfo * arg, CLIENT *cl );
}
#else
extern int *positionset_1();
#endif /* __cplusplus */
#define COURSESET ((u_long)15)
#ifdef __cplusplus
extern "C" {
extern int *courseset_1( TCourse * arg, CLIENT *cl );
}
#else
extern int *courseset_1();
#endif /* __cplusplus */
#define ACCURACYSET ((u_long)16)
#ifdef __cplusplus
extern "C" {
extern int *accuracyset_1( TAccuracy * arg, CLIENT *cl );
}
#else
extern int *accuracyset_1();
#endif /* __cplusplus */
#define SATELLITESET ((u_long)17)
#ifdef __cplusplus
extern "C" {
extern int *satelliteset_1( TSatellite * arg, CLIENT *cl );
}
#else
extern int *satelliteset_1();
#endif /* __cplusplus */
#define BATCHAPPENDSENTENCE ((u_long)18)
#ifdef __cplusplus
extern "C" {
extern int *batchappendsentence_1( TAppendSentence * arg, CLIENT *cl );
}
#else
extern int *batchappendsentence_1();
#endif /* __cplusplus */
#define BATCHAPPENDUSERSENTENCE ((u_long)19)
#ifdef __cplusplus
extern "C" {
extern int *batchappendusersentence_1( TAppendUserSentence * arg, CLIENT *cl );
}
#else
extern int *batchappendusersentence_1();
#endif /* __cplusplus */
#define BATCHSETDELAY ((u_long)20)
#ifdef __cplusplus
extern "C" {
extern int *batchsetdelay_1( int * arg, CLIENT *cl );
}
#else
extern int *batchsetdelay_1();
#endif /* __cplusplus */
#define BATCHRESET ((u_long)21)
#ifdef __cplusplus
extern "C" {
extern int *batchreset_1( void * arg, CLIENT *cl );
}
#else
extern int *batchreset_1();
#endif /* __cplusplus */
#define SETCOMPORT ((u_long)22)
#ifdef __cplusplus
extern "C" {
extern int *setcomport_1( char ** arg, CLIENT *cl );
}
#else
extern int *setcomport_1();
#endif /* __cplusplus */
#define SETCHUNKMODE ((u_long)23)
#ifdef __cplusplus
extern "C" {
extern int *setchunkmode_1( bool_t * arg, CLIENT *cl );
}
#else
extern int *setchunkmode_1();
#endif /* __cplusplus */
#define SETCHUNKSIZE ((u_long)24)
#ifdef __cplusplus
extern "C" {
extern int *setchunksize_1( int * arg, CLIENT *cl );
}
#else
extern int *setchunksize_1();
#endif /* __cplusplus */
#define SETCHUNKDELAY ((u_long)25)
#ifdef __cplusplus
extern "C" {
extern int *setchunkdelay_1( int * arg, CLIENT *cl );
}
#else
extern int *setchunkdelay_1();
#endif /* __cplusplus */
#define STARTCOMMS ((u_long)26)
#ifdef __cplusplus
extern "C" {
extern int *startcomms_1( void * arg, CLIENT *cl );
}
#else
extern int *startcomms_1();
#endif /* __cplusplus */
#define STOPCOMMS ((u_long)27)
#ifdef __cplusplus
extern "C" {
extern int *stopcomms_1( void * arg, CLIENT *cl );
}
#else
extern int *stopcomms_1();
#endif /* __cplusplus */
#define BATCHAPPENDUSERSTRING ((u_long)28)
#ifdef __cplusplus
extern "C" {
extern int *batchappenduserstring_1( TAppendString * arg, CLIENT *cl );
}
#else
extern int *batchappenduserstring_1();
#endif /* __cplusplus */

#endif /* __APICOMMANDHANDLER_H__ */
