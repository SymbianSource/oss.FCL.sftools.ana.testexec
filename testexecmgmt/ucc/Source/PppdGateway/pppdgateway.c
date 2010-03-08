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



//#define SYSTEM_TEST_MP5


/*******************************************************************************
 *
 * System Includes
 *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>

/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "socket_helper.h"


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define RECVBUFFSIZE      (4096*4)
#define MAXIDSTR          64


/*******************************************************************************
 *
 * Prototypes
 *
 ******************************************************************************/
void *receive_ppp_frames( void *x );
void *send_ppp_frames( void * );
void closesocket( int sockfd );
int do_client_server_protocol( int aMobsterDeviceID );


/*******************************************************************************
 *
 * File-scope
 *
 ******************************************************************************/
int sockfd;


/*******************************************************************************
 *
 * Signal Handler
 *
 ******************************************************************************/
void sigterm( int signum )
{
  pid_t mp = getpid();
  fprintf( stderr, "PG: pppdgateway(%d) received a SIGTERM -- ignoring\n", mp );
}


/*******************************************************************************
 *
 * Main()
 *
 ******************************************************************************/
int main( int argc, char *argv[] )
{
  int err;
  int mobster_device_id = 0;
  pthread_t threadhandle_send, threadhandle_recv;
  struct sockaddr_in mobster_addr;
  __sighandler_t oh;

  // check args
  if( argc != 4 ) {
    fprintf( stderr, "usage: %s mobster_address mobster_port mobster_device_id\n", argv[0] );
    return -1;
  }
  mobster_device_id = atoi(argv[3]);
  fprintf( stderr, "PG: pppdgateway starting\n" );

  // setup the signal handler -- note that we ignore SIGTERM! This is because if we shutdown on a SIGTERM we don't
  // output the TERMREQ -- or receive the TERMACK. Therefore, we rely on the fact that when pppd wants to stop
  // it will close stdin -- this will cause the output thread to stop -- it will also close the socket which
  // will cause the input thread to stop -- then the main thread will exit
  oh = signal( SIGTERM, sigterm );
  if( oh == SIG_ERR ) {
    fprintf( stderr, "PG: error registering signal handler (SIGTERM)\n" );
    return -1;
  }
  oh = signal( SIGHUP, sigterm );
  if( oh == SIG_ERR ) {
    fprintf( stderr, "PG: error registering signal handler (SIGHUP)\n" );
    return -1;
  }

  // create a TCP socket
  sockfd = socket( AF_INET, SOCK_STREAM, 0 );
  if( sockfd == -1 ) {
    fprintf( stderr, "PG: failed to create socket, exiting.\n" );
    return -1;
  }

  // create the address
  mobster_addr = getsockaddr( argv[1], argv[2] );
  if( mobster_addr.sin_addr.s_addr == 0 ) {
    fprintf( stderr, "PG: invalid address (zero or couldn't resolve hostname), exiting.\n" );
    closesocket( sockfd );
    return -1;
  }

  // connect to mobster
  err = connect( sockfd, (struct sockaddr*)&mobster_addr, sizeof(mobster_addr) );
  if( err != 0 ) {
    perror( "PG: connect() to server failed\n" );
    closesocket( sockfd );
    return -1;
  }

  // do the client server protocol - if there is an error the exit
  err = do_client_server_protocol( mobster_device_id );
  if( err != 0 ) {
    fprintf( stderr, "PG: client/server protocol failed.\n" );
    closesocket( sockfd );
    return -1;
  }
    
  // change the streams to unbuffered so that this exe doesn't add latency
  err = setvbuf( stdout, NULL, _IONBF, 0 );
  if( err != 0 ) {
    fprintf( stderr, "PG: failed to set buffering on stdout (%d)\n", err );
    closesocket( sockfd );
    return -1;
  }
  err = setvbuf( stdin, NULL, _IONBF, 0 );
  if( err != 0 ) {
    fprintf( stderr, "PG: failed to set buffering on stdout (%d)\n", err );
    closesocket( sockfd );
    return -1;
  }
  err = setvbuf( stderr, NULL, _IONBF, 0 );
  if( err != 0 ) {
    fprintf( stderr, "PG: failed to set buffering on stdout (%d)\n", err );
    closesocket( sockfd );
    return -1;
  }

  // all the setup is done -- create the threads and let them do their stuff
  err = pthread_create( &threadhandle_send, NULL, send_ppp_frames, 0 );
  assert( err == 0 );
  err = pthread_create( &threadhandle_recv, NULL, receive_ppp_frames, 0 );
  assert( err == 0 );


  // wait for them to exit
  pthread_join( threadhandle_send, NULL );
  pthread_join( threadhandle_recv, NULL );


  // return
  fprintf( stderr, "PG: pppdgateway has completed.\n" );
  return 0;
}

  
/*******************************************************************************
 *
 * receive_ppp_frames
 *
 ******************************************************************************/
void *receive_ppp_frames( void *x )
{
  int recvcount, writecount;
  char recvbuff[RECVBUFFSIZE];

  // listen on the udp socket and send anything received to stdout -- no framing is ever done on the input channel
  while( 1 ) {

    // wait for some data -- on error print error, close the socket, and exit. Closing the socket means that the other thread will clean up and exit
    recvcount = recv( sockfd, recvbuff, RECVBUFFSIZE, 0 );
    if( recvcount == -1 ) {
      fprintf( stderr, "PG: error occured while reading socket.\n" );
      closesocket( sockfd );
      pthread_exit( NULL );
    }
    //    fprintf( stderr, "PG: %d bytes incoming\n", recvcount );

    // send the data to the pppd server    
    writecount = fwrite( recvbuff, 1, recvcount, stdout );
    if( writecount != recvcount ) {
      fprintf( stderr, "PG: error occured while writing to stdout.\n" );
      closesocket( sockfd );
      pthread_exit( NULL );
    }
  }

  // done
  return NULL;
}   


/*******************************************************************************
 *
 * send_ppp_frames
 *
 ******************************************************************************/
void *send_ppp_frames( void *x ) 
{
  int err;
  int sendcount;
  int c = 0;
  
  // listen on stdin for data and write it to the socket -- on error print an error, closesocket, and exit. 
  while( 1 ) {

    // read the next char
    err = fread( &c, 1, 1, stdin );
    if( err != 1 ) {
      fprintf( stderr, "PG: error occured while reading from stdin.\n" );
      closesocket( sockfd );
      pthread_exit( NULL );
    }
    //    fprintf( stderr, "PG: %d bytes outgoing\n", err );

    // write it to the socket
    sendcount = send( sockfd, &c, 1, 0 );
    if( sendcount != 1 ) {
      fprintf( stderr, "PG: error occured while writing to the socket.\n" );
      closesocket( sockfd );
      pthread_exit( NULL );
    }
  }

  // done 
  return NULL;
}


/*******************************************************************************
 *
 * closesocket
 *
 ******************************************************************************/
void closesocket( int sockfd )
{
  shutdown( sockfd, SHUT_RDWR );
  close( sockfd );
}


/*******************************************************************************
 *
 * do_client_server_protocol()
 *
 ******************************************************************************/
int do_client_server_protocol( int aMobsterDeviceID )
{
  char c;
  int len, expected_length, i, err;
  char idstr[MAXIDSTR];
  char rpstr[MAXIDSTR];

  // construct the client string for the csprotocol
  snprintf( idstr, MAXIDSTR, "ID=%d", aMobsterDeviceID );
  len = strlen(idstr);
  idstr[len] = 0x0d;
  idstr[len+1] = 0;

#ifdef SYSTEM_TEST_MP5
  assert( !"TEST" );
#endif

  // send the string
  err = send( sockfd, idstr, len+1, 0 );
  if( err != (len+1) ) {
    fprintf( stderr, "PG: failed to write id string, exiting.\n" );
    return -1;
  }
  
  // construct the expected reply string to get the expected length
  snprintf( rpstr, MAXIDSTR, "SERVER(%d)", aMobsterDeviceID );
  expected_length = strlen(rpstr);

  // now read until there 
  for( i = 0; ; i++ ) {

    // read a byte
    err = recv( sockfd, &c, 1, 0 );
    if( err == -1 ) {
      fprintf( stderr, "PG: failed to read reply string (%d, %d).\n", err, errno );
      return -1;
    }

    // check for a mismatch
    if( c != rpstr[i] ) {
      fprintf( stderr, "PG: unexpected character from server.\n" );
      return -1;
    }

    // see if we've matched all chars
    if( (i+1) == expected_length ) {
      break;
    }
  }

  // done - success
  return 0;
}
