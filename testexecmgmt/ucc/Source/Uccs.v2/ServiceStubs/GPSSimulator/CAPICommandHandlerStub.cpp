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




#include "CAPICommandHandlerStub.h"
#include "APICommandHandler.h"

CAPICommandHandlerStub::CAPICommandHandlerStub()
{
	iGPSSimulator = new CCGpssimulator();
}

CAPICommandHandlerStub::~CAPICommandHandlerStub()
{
	delete iGPSSimulator;
}

int CAPICommandHandlerStub::StartUccsService( char *aHostName,  int *aErrorCode, int *aUnused )
{
	int client_stub_error;
	
	*aErrorCode = 0;
	
	// connect to the remote service
	client_stub_error = iGPSSimulator->Connect( aHostName );

	// return the result
	return client_stub_error;
}

int CAPICommandHandlerStub::StopUccsService( int *aErrorCode, int *aUnused )
{
	int client_stub_error;
	*aErrorCode = 0;

	// shutdown the stub -- force it to close
	client_stub_error = iGPSSimulator->Disconnect();
	
	// done
	return client_stub_error;
}

CDataRecord* CAPICommandHandlerStub::IssueCommand( CDataRecord* aRequest )
{
	int call_result, err, client_stub_error, method_id = -1;
	
	CDataRecord *request_reply;

	TStartupInfo sarg_startup_info;
	int rv_integer, sarg_integer;

	// create a standard reply 
	request_reply = CreateBaseReply( aRequest );

	// get and check the method_id
	err = request_reply->GetFieldAsInt( "METHODID", &method_id );
	if( method_id == -1 )
	{
		UpdateCompletionCode( request_reply, ERR_INVALID_METHOD );
		return request_reply;
	}

	// now dispatch and call the appropriate method
	switch( method_id )
	{
		// ss_startuprpcservice
		case 1:
			{
				// extract the parameters
				sarg_startup_info.iEmpty = 0;

				// make the call and update the return value
				client_stub_error = iGPSSimulator->ss_startuprpcservice( sarg_startup_info, &rv_integer );
				UpdateCompletionCode( request_reply, client_stub_error );		
				if( client_stub_error != ERR_NONE )
				{
					break;
				}
				// set any return information
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// sc_shutdownrpcservice
		case 2: 
			{
				// extract the parameters
				GETINTEGERARGUMENT( "FORCE", &sarg_integer, 1, 1, aRequest, request_reply );

				// make the call and update the return value
				client_stub_error = iGPSSimulator->sc_shutdownrpcservice( sarg_integer, &rv_integer );
				UpdateCompletionCode( request_reply, client_stub_error );
				if( client_stub_error != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// startsimulator		
		case 10:
			{
				// Make the call and update the return values
				call_result = iGPSSimulator->startsimulator( &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// stopsimulator		
		case 11:
			{
				// Make the call and update the return values
				call_result = iGPSSimulator->stopsimulator( &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;
			
		// setfielddefault
		case 12:
			{
				// Extract the parameters
				int		fieldID;
				int		state;
				char*	value = NULL;
	
				GETINTEGERARGUMENT( "FIELD", &fieldID, 1, 0, aRequest, request_reply );
				GETINTEGERARGUMENT( "STATE", &state, 2, 0, aRequest, request_reply );
				GETSTRINGARGUMENT( "VALUE", &value, 3, 1, aRequest, request_reply );

				TField	field = {0,0,0};
				field.iFieldId = fieldID;
				field.iState = state;
				if( value!=NULL )
				{
					strcpy( field.iValue, value );
				}

				// Make the call and update the return values
				call_result = iGPSSimulator->setfielddefault( field, &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// setsatellitedefault
		case 13:
			{
				// Extract the parameters
				int		elevation;
				int		azimuth;
				int		snr;
				int		id;

				GETINTEGERARGUMENT( "ELEVATION", &elevation, 1, 0, aRequest, request_reply );
				GETINTEGERARGUMENT( "AZIMUTH", &azimuth, 2, 0, aRequest, request_reply );
				GETINTEGERARGUMENT( "SNR", &snr, 3, 0, aRequest, request_reply );
				GETINTEGERARGUMENT( "SATID", &id, 4, 0, aRequest, request_reply );

				TSatellite	satellite;
				satellite.iAzimuth = azimuth;
				satellite.iElevation = elevation;
				satellite.iSNR = snr;
				satellite.iId = id;

				// Make the call and update the return values
				call_result = iGPSSimulator->setsatellitedefault( satellite, &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// positionset
		case 14:
			{
				// Extract the parameters
				char*	latitude;
				char*	ns;
				char*	longitude;
				char*	ew;

				GETSTRINGARGUMENT( "LATITUDE", &latitude, 1, 0, aRequest, request_reply );
				GETSTRINGARGUMENT( "NS", &ns, 2, 0, aRequest, request_reply );
				GETSTRINGARGUMENT( "LONGITUDE", &longitude, 3, 0, aRequest, request_reply );
				GETSTRINGARGUMENT( "EW", &ew, 4, 0, aRequest, request_reply );

				TPositionInfo	pos;
				pos.iLatitude = atof( latitude );
				pos.iNS = strcmp("N", ns)?1:0;
				pos.iLongitude = atof( longitude );
				pos.iEW = strcmp("E", ew)?1:0;

				// Make the call and update the return values
				call_result = iGPSSimulator->positionset( pos, &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;
			
		// courseset
		case 15:
			{
				// Extract the parameters
				int	speed;
				int	direction;

				GETINTEGERARGUMENT( "SPEED", &speed, 1, 0, aRequest, request_reply );
				GETINTEGERARGUMENT( "DIRECTION", &direction, 2, 0, aRequest, request_reply );

				TCourse	course;
				course.iSpeed = (double)speed;
				course.iDirection = (double)direction;

				// Make the call and update the return values
				call_result = iGPSSimulator->courseset( course, &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;
			
		// accuracyset
		case 16:
			{
				// Extract the parameters
				char* accuracyStr;
				char* hwconstantStr;
				GETSTRINGARGUMENT( "ACCURACY", &accuracyStr, 1, 0, aRequest, request_reply );
				GETSTRINGARGUMENT( "HWCONSTANT", &hwconstantStr, 1, 0, aRequest, request_reply );

				// Make the call and update the return values
				TAccuracy accuracy;
				accuracy.iAccuracy = atof(accuracyStr);
				accuracy.iHWConstant = atof(hwconstantStr);
				call_result = iGPSSimulator->accuracyset( accuracy, &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// satelliteset
		case 17:
			{
				// Extract the parameters
				int		elevation;
				int		azimuth;
				int		snr;
				int		id;

				GETINTEGERARGUMENT( "ELEVATION", &elevation, 1, 0, aRequest, request_reply );
				GETINTEGERARGUMENT( "AZIMUTH", &azimuth, 2, 0, aRequest, request_reply );
				GETINTEGERARGUMENT( "SNR", &snr, 3, 0, aRequest, request_reply );
				GETINTEGERARGUMENT( "SATID", &id, 4, 0, aRequest, request_reply );

				TSatellite	satellite;
				satellite.iAzimuth = azimuth;
				satellite.iElevation = elevation;
				satellite.iSNR = snr;
				satellite.iId = id;

				// Make the call and update the return values
				call_result = iGPSSimulator->satelliteset( satellite, &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// batchappendsentence
		case 18:
			{
				// Extract the parameters
				int		sentenceId;
				int		frequency;

				GETINTEGERARGUMENT( "SENTENCEID", &sentenceId, 1, 0, aRequest, request_reply );
				GETINTEGERARGUMENT( "FREQUENCY", &frequency, 2, 0, aRequest, request_reply );

				TAppendSentence sentence;
				sentence.iSentenceId = sentenceId;
				sentence.iFrequency = frequency;

				// Make the call and update the return values
				call_result = iGPSSimulator->batchappendsentence( sentence, &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// batchappendusersentence
		case 19:
			{
				// Extract the parameters
				int	sentenceId;
				int	frequency = 1;
				int numFields = 0;

				GETINTEGERARGUMENT( "NUMFIELDS", &numFields, 1, 0, aRequest, request_reply );
				GETINTEGERARGUMENT( "SENTENCEID", &sentenceId, 2, 0, aRequest, request_reply );
				GETINTEGERARGUMENT( "FREQUENCY", &frequency, 3, 1, aRequest, request_reply );

				TAppendSentence	sentence;
				sentence.iSentenceId = sentenceId;
				sentence.iFrequency = frequency;
				TAppendUserSentence userSentence;
				userSentence.iAppendSentence = sentence;

				// Allocate enough space for the number of user fields
				userSentence.iUserFields.iUserFields_len = numFields;
				userSentence.iUserFields.iUserFields_val = new TField[numFields];
				int count = 0;

				// Check each possible field id to see if it has been entered in the script
				for( int field_id = 1 ; field_id <= F_NUMBER_OF_FIELDS ; field_id++ )
				{
					string field_str;
					switch( field_id )
					{
						case F_TIME:				field_str = "TIME"; break;
						case F_STATUS:				field_str = "STATUS"; break;
						case F_DATE:				field_str = "DATE"; break;
						case F_LATITUDE:			field_str = "LATITUDE"; break;
						case F_LAT_NORTH_SOUTH:		field_str = "LAT_NORTH_SOUTH"; break;
						case F_LONGITUDE:			field_str = "LONGITUDE"; break;
						case F_LON_EAST_WEST:		field_str = "LON_EAST_WEST"; break;
						case F_SPEED_OVER_GROUND:	field_str = "SPEED"; break;
						case F_COURSE_OVER_GROUND:	field_str = "COURSE"; break;
						case F_MAGNETIC_VARIATION:	field_str = "MAGNETIC_VARIATION"; break;
						case F_MAG_EAST_WEST:		field_str = "MAG_EAST_WEST"; break;
						case F_MODE_INDICATOR:		field_str = "MODE_INDICATOR"; break;
						case F_OPERATION_MODE:		field_str = "OPERATION_MODE"; break;
						case F_FIX_MODE:			field_str = "FIX_MODE"; break;
						case F_SATELLITE_ONE:		field_str = "SATELLITE_ONE"; break;
						case F_SATELLITE_TWO:		field_str = "SATELLITE_TWO"; break;
						case F_SATELLITE_THREE:		field_str = "SATELLITE_THREE"; break;
						case F_SATELLITE_FOUR:		field_str = "SATELLITE_FOUR"; break;
						case F_SATELLITE_FIVE:		field_str = "SATELLITE_FIVE"; break;
						case F_SATELLITE_SIX:		field_str = "SATELLITE_SIX"; break;
						case F_SATELLITE_SEVEN:		field_str = "SATELLITE_SEVEN"; break;
						case F_SATELLITE_EIGHT:		field_str = "SATELLITE_EIGHT"; break;
						case F_SATELLITE_NINE:		field_str = "SATELLITE_NINE"; break;
						case F_SATELLITE_TEN:		field_str = "SATELLITE_TEN"; break;
						case F_SATELLITE_ELEVEN:	field_str = "SATELLITE_ELEVEN"; break;
						case F_SATELLITE_TWELVE:	field_str = "SATELLITE_TWELVE"; break;
						case F_PDOP:				field_str = "PDOP"; break;
						case F_HDOP:				field_str = "HDOP"; break;
						case F_VDOP:				field_str = "VDOP"; break;
						case F_SATELLITES_IN_USE:	field_str = "SATELLITES_IN_USE"; break;
						case F_ANTENNA_ALTITUDE:	field_str = "ANTENNA_ALTITUDE"; break;
						case F_ANTENNA_METERS:		field_str = "ANTENNA_METERS"; break;
						case F_GEOIDAL_SEPARATION:	field_str = "GEOIDAL_SEPARATION"; break;
						case F_GEOIDAL_METERS:		field_str = "GEOIDAL_METERS"; break;
						case F_DIFFERENTIAL_AGE:	field_str = "DIFFERENTIAL_AGE"; break;
						case F_DIFFERENTIAL_REF_ID:	field_str = "DIFFERENTIAL_REF_ID"; break;
						case F_GPS_QUALITY:			field_str = "GPS_QUALITY"; break;
						case F_NUMBER_OF_SENTENCES:	field_str = "NUMBER_OF_SENTENCES"; break;
						case F_CHECKSUM:			field_str = "CHECKSUM"; break;
						default:
							break;
					}
					
					// Append _STATE for each id for the state retrieval
					string field_state_str = field_str + "_STATE";
					char* field_state = NULL;
					GETSTRINGARGUMENT( (char*)field_state_str.c_str(), &field_state, field_id+4, 1, aRequest, request_reply );

					if( field_state != NULL && count<numFields )
					{
						userSentence.iUserFields.iUserFields_val[count].iFieldId = field_id;
						userSentence.iUserFields.iUserFields_val[count].iState = atoi( field_state );

						if( atoi(field_state) == S_VALUE )
						{
							char* field = NULL;
							GETSTRINGARGUMENT( (char*)field_str.c_str(), &field, field_id+4+F_NUMBER_OF_FIELDS, 0, aRequest, request_reply );

							if( field != NULL && count < numFields )
							{
								strcpy( userSentence.iUserFields.iUserFields_val[count++].iValue, field );
							}							
						}
					}
				}

				// Make the call and update the return values
				call_result = iGPSSimulator->batchappendusersentence( userSentence, &rv_integer );

				// Cleanup the user fields now
				delete[] userSentence.iUserFields.iUserFields_val;

				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// batchsetdelay
		case 20:
			{
				// Extract the parameters
				int	delay;

				GETINTEGERARGUMENT( "DELAY", &delay, 1, 0, aRequest, request_reply );

				// Make the call and update the return values
				call_result = iGPSSimulator->batchsetdelay( delay, &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// batchreset
		case 21:
			{
				// Make the call and update the return values
				call_result = iGPSSimulator->batchreset( &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;
			
		// setcomport
		case 22:
			{
				// Extract the parameters
				char*	port;
	
				GETSTRINGARGUMENT( "COMPORT", &port, 1, 0, aRequest, request_reply );

				// Make the call and update the return values
				call_result = iGPSSimulator->setcomport( port, &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// setchunkmode
		case 23:
			{
				// Extract the parameters
				int	mode;
	
				GETINTEGERARGUMENT( "MODE", &mode, 1, 0, aRequest, request_reply );

				// Make the call and update the return values
				call_result = iGPSSimulator->setchunkmode( mode==0?false:true, &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// setchunksize
		case 24:
			{
				// Extract the parameters
				int	size;
	
				GETINTEGERARGUMENT( "SIZE", &size, 1, 0, aRequest, request_reply );

				// Make the call and update the return values
				call_result = iGPSSimulator->setchunksize( size, &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;
			
		// setchunkdelay
		case 25:
			{
				// Extract the parameters
				int	delay;
	
				GETINTEGERARGUMENT( "DELAY", &delay, 1, 0, aRequest, request_reply );

				// Make the call and update the return values
				call_result = iGPSSimulator->setchunkdelay( delay, &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;
			
		// startcomms
		case 26:
			{
				// Make the call and update the return values
				call_result = iGPSSimulator->startcomms( &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// stopcomms
		case 27:
			{
				// Make the call and update the return values
				call_result = iGPSSimulator->stopcomms( &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// batchappenduserstring
		case 28:
			{
				// Extract the parameters
				char*	userString;
				int		frequency;

				GETSTRINGARGUMENT( "STRING", &userString, 1, 0, aRequest, request_reply );
				GETINTEGERARGUMENT( "FREQUENCY", &frequency, 2, 0, aRequest, request_reply );

				TAppendString sentence;
				strcpy( sentence.iUserString, userString );
				sentence.iFrequency = frequency;

				// Make the call and update the return values
				call_result = iGPSSimulator->batchappenduserstring( sentence, &rv_integer );
				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;
			
		// Any other method id results in an invalid method id result
		default:
			{
				UpdateCompletionCode( request_reply, ERR_INVALID_METHOD );
			}
			break;
	}

	// everything should be handled above 
	return request_reply;
}

int CAPICommandHandlerStub::GetStatus()
{
	return -1;
}

char *CAPICommandHandlerStub::GetLastRPCError( int *aIntError )
{
	return iGPSSimulator->GetLastRPCError( aIntError );
}
