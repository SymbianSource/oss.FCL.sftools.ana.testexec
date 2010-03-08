/*********************************************************************
 * RPC for the Windows NT Operating System
 * 1993 by Martin F. Gergeleit
 *
 * RPC for the Windows NT Operating System COMES WITH ABSOLUTELY NO 
 * WARRANTY, NOR WILL I BE LIABLE FOR ANY DAMAGES INCURRED FROM THE 
 * USE OF. USE ENTIRELY AT YOUR OWN RISK!!!
 *********************************************************************/
/*********************************************************************
 *
 * Copyright (c) 1993 Martin F. Gergeleit
 * 
 *********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void bcopy(char *,char*,int);
void bcopy_nf(char *,char *,int);
void bcopy_fn(char *,char *,int);
void bcopy_ff(char *,char *,int);
void bzero(char*,int);
#ifdef __cplusplus
};
#endif

