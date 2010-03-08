/*********************************************************************
 * RPC for the Windows NT Operating System
 * 1993 by Martin F. Gergeleit
 * Users may use, copy or modify Sun RPC for the Windows NT Operating 
 * System according to the Sun copyright below.
 *
 * RPC for the Windows NT Operating System COMES WITH ABSOLUTELY NO 
 * WARRANTY, NOR WILL I BE LIABLE FOR ANY DAMAGES INCURRED FROM THE 
 * USE OF. USE ENTIRELY AT YOUR OWN RISK!!!
 *********************************************************************/

/* @(#)rpc_main.c	2.2 88/08/01 4.0 RPCSRC */
/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 * 
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 * 
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 * 
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 * 
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 * 
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */
//#ifndef lint
//static char sccsid[] = "@(#)rpc_main.c 1.7 87/06/24 (C) 1987 SMI";
//#endif

/*
 * rpc_main.c, Top level of the RPC protocol compiler. 
 * Copyright (C) 1987, Sun Microsystems, Inc. 
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rpc_util.h"
#include "rpc_pars.h"
#include "rpc_scan.h"

#ifdef WIN32
#include <direct.h>
#include <process.h>
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#include <sys/file.h>
#endif

#include "ae_component_rpc.h"

#define EXTEND	1		/* alias for TRUE */

static char *cmdname;
#ifdef WIN32
#ifdef __BORLANDC__
static char CPP[] = "cpp32";
static char CPPFLAGS[] = "-C -P- -oCON";
#else
static char CPP[] = "cl";
static char CPPFLAGS[] = "/C /EP /nologo";
#endif
#else
static char CPP[] = "/usr/bin/cpp";
static char CPPFLAGS[] = "-C";
#endif
static char *allv[] = {
	"rpcgen", "-s", "udp", "-s", "tcp",
};
static int allc = sizeof(allv)/sizeof(allv[0]);


/* prototypes */
extern void crash();
static void do_registers( int argc, char *argv[] );
struct commandline command_line;


/*
 * add extension to filename 
 */
char *
extendfile(file, ext)
	char *file;
	char *ext;
{
#ifdef WIN32
	int i;
#endif
	char *res;
	char *p;

	res = alloc(strlen(file) + strlen(ext) + 1);
	if (res == NULL) {
		abort();
	}
#ifdef WIN32
	p = NULL;
	for (i = 0; file[i] != '\0'; i++)
		if (file[i] == '.')
			p = &file[i];
#else
	p = rindex(file, '.');
#endif
	if (p == NULL) {
		p = file + strlen(file);
	}
	(void) strcpy(res, file);
	(void) strcpy(res + (p - file), ext);
	return (res);
}

/*
 * Open output file with given extension 
 */
void
open_output(infile, outfile)
	char *infile;
	char *outfile;
{
	char *ptr;
	char buff[1024];

	// check that some file is specified
	if (outfile == NULL) {
		fout = stdout;
		return;
	}

	// set the buffer with the specified output path
	strcpy( buff, command_line.outfile );

	// only copy the filename itself
	ptr = strrchr( outfile, '\\' );
	if( ptr == NULL ) {
		ptr = strrchr( outfile, '/' );
		if( ptr == NULL ) {
			ptr = outfile - 1;
		}
	}
	ptr++;
	strcat( buff, ptr );

	// check that we are not going to overwrite the input file
	if (infile != NULL && streq(buff, infile)) {
		f_print(stderr, "%s: output would overwrite %s\n", cmdname, infile);
		crash();
	}

//	fprintf( stderr, "INFO: opening output file %s\n", buff );

	// open the output file
	fout = fopen(buff, "w");
	if (fout == NULL) {
		f_print(stderr, "%s: unable to open output file.", cmdname);
		perror(buff);
		crash();
	}
	record_open(buff);
}

/*
 * Open input file with given define for C-preprocessor 
 */
void
open_input(infile, define)
	char *infile;
	char *define;
{
#ifdef WIN32
#ifdef __BORLANDC__
#define _P_WAIT P_WAIT
#define _spawnlp spawnlp
#endif
	int old;
	int pd[2];

	infilename = (infile == NULL) ? "<stdin>" : infile;
	_pipe(pd, 0xffff, O_TEXT);

		old = dup(1);
		(void) dup2(pd[1], 1);

		if (_spawnlp(_P_WAIT, CPP, CPP, CPPFLAGS, 
					define, infile, NULL) < 0) {
			f_print(stderr, "%s: unable to open ", cmdname);
			perror(CPP);
			crash();
		}

	(void) dup2(old, 1);

	(void) close(pd[1]);
	fin = fdopen(pd[0], "r");
	if (fin == NULL) {
		f_print(stderr, "%s: ", cmdname);
		perror(infilename);
		crash();
	}
#else
	int pd[2];

	infilename = (infile == NULL) ? "<stdin>" : infile;
	(void) pipe(pd);
	switch (fork()) {
	case 0:
		(void) close(1);
		(void) dup2(pd[1], 1);
		(void) close(pd[0]);
		execl(CPP, CPP, CPPFLAGS, define, infile, NULL);
		perror("execl");
		exit(1);
	case -1:
		perror("fork");
		exit(1);
	}
	(void) close(pd[1]);
	fin = fdopen(pd[0], "r");
	if (fin == NULL) {
		f_print(stderr, "%s: ", cmdname);
		perror(infilename);
		crash();
	}
#endif
}

/*
 * Compile into an XDR routine output file
 */
static void
c_output(infile, define, extend, outfile)
	char *infile;
	char *define;
	int extend;
	char *outfile;
{
	definition *def;
	char *include;
	char *outfilename;
	long tell;

	open_input(infile, define);	
	outfilename = extend ? extendfile(infile, outfile) : outfile;
	open_output(infile, outfilename);
	f_print(fout, "#include <rpc/rpc.h>\n");
	if (infile && (include = extendfile(infile, ".h"))) {
		f_print(fout, "#include \"%s\"\n", include);
		free(include);
	}
	tell = ftell(fout);
	while ((def = get_definition())) {
		emit(def);
	}
	if (extend && tell == ftell(fout)) {
		(void) unlink(outfilename);
	}
}

/*
 * Compile into an XDR header file
 */
static void
h_output(infile, define, extend, outfile)
	char *infile;
	char *define;
	int extend;
	char *outfile;
{
	definition *def;
	char *outfilename;
	long tell;
	char nbuff[256];

	open_input(infile, define);
	outfilename =  extend ? extendfile(infile, outfile) : outfile;
	open_output(infile, outfilename);
	tell = ftell(fout);

	// AE -- do proper #ifdef
	{
		char *ptr;
		int slen;
		int i;

		// get a pointer to the filename
		ptr = strrchr( outfilename, '\\' );
		ptr = ((ptr == NULL) ? outfilename : ptr+1);

		// now copy the filename
		slen = strlen(ptr);
		assert( slen < 256 );
		memcpy( nbuff, ptr, slen );
		nbuff[slen] = 0;

		// make everything upper case
		for( i = 0; i < slen; i++ ) {
			nbuff[i] = toupper( nbuff[i] );
		}
		assert( nbuff[slen-2] == '.' );
		nbuff[slen-2] = '_';

		// now print the ifdef
		f_print( fout, "#ifndef __%s__\n", nbuff );
		f_print( fout, "#define __%s__\n", nbuff );
	}
		

	/*** The following line was added,  ***/
	/*** 31.03.92, Detlef Schwellenbach ***/

	f_print(fout, "#include <rpc/types.h>\n");

	// AE -- include this for the CLIENT type
	{
		f_print(fout, "#include <rpc/rpc.h>\n\n");
	}

	while ((def = get_definition())) {
		print_datadef(def);
	}
	if (extend && tell == ftell(fout)) {
		(void) unlink(outfilename);
	}

	// AE -- end the #def
	f_print(fout, "#endif /* __%s__ */\n", nbuff );
}

/*
 * Compile into an RPC service
 */
static void
s_output(argc, argv, infile, define, extend, outfile, nomain)
	int argc;
	char *argv[];
	char *infile;
	char *define;
	int extend;
	char *outfile;
	int nomain;
{
	char *include;
	definition *def;
	int foundprogram;
	char *outfilename;

	open_input(infile, define);
	outfilename = extend ? extendfile(infile, outfile) : outfile;
	open_output(infile, outfilename);
	f_print(fout, "#include <stdio.h>\n");
	f_print(fout, "#include <rpc/rpc.h>\n");
	f_print(fout, "#ifdef WIN32\n");
	f_print(fout, "#include <rpc/PMAP_CLN.H>\n");
	f_print(fout, "#else\n");
	f_print(fout, "#include <rpc/pmap_clnt.h>\n");
	f_print(fout, "#endif\n");
	if (infile && (include = extendfile(infile, ".h"))) {
		f_print(fout, "#include \"%s\"\n", include);
		free(include);
	}
	foundprogram = 0;
	while ((def = get_definition())) {
		foundprogram |= (def->def_kind == DEF_PROGRAM);
	}
	if (extend && !foundprogram) {
		(void) unlink(outfilename);
		return;
	}
	if (nomain) {
		write_programs((char *)NULL);
	} else {
		write_most();
		do_registers(argc, argv);
		write_rest();
		write_programs("static");
	}
}

static void
l_output(infile, define, extend, outfile)
	char *infile;
	char *define;
	int extend;
	char *outfile;
{
	char *include;
	definition *def;
	int foundprogram;
	char *outfilename;

	open_input(infile, define);
	outfilename = extend ? extendfile(infile, outfile) : outfile;
	open_output(infile, outfilename);
	f_print(fout, "#include <rpc/rpc.h>\n");
	if (infile && (include = extendfile(infile, ".h"))) {
		f_print(fout, "#include \"%s\"\n", include);
		free(include);
	}
	foundprogram = 0;
	while ((def = get_definition())) {
		foundprogram |= (def->def_kind == DEF_PROGRAM);
	}
	if (extend && !foundprogram) {
		(void) unlink(outfilename);
		return;
	}
	write_stubs();
}

/*
 * Perform registrations for service output 
 */
static void
do_registers(argc, argv)
	int argc;
	char *argv[];
{
	int i;

	for (i = 1; i < argc; i++) {
		if (streq(argv[i], "-s")) {
			write_register(argv[i + 1]);
			i++;
		}
	}
}

/*
 * Parse command line arguments 
 */
static int parseargs( int argc, char *argv[], struct commandline *cmd )
{
	int i, err;
	char *opt;

	// check params
	assert( argv != NULL );
	assert( cmd != NULL );

	// clear the cmd struct
	memset( cmd, 0, sizeof(*cmd) );

	// if we don't have at least one extra param (the filename) then exit
	if( argc < 2 ) {
		return 0;
	}

	// now parse each token
	for( i = 1; i < argc; i++ ) {

		// if the first char is not '-' then this is the filename and we should break
		if( argv[i][0] != '-' ) {
			break;
		}

		// get a pointer to the option
		opt = &(argv[i][1]);

		// now look for a match
		if( strcmp(opt,"client") == 0 ) {
			cmd->client_flag = 1;
		} else if( strcmp(opt,"server") == 0 ) {
			cmd->server_flag = 1;
		} else if( strcmp(opt,"component_base") == 0 ) {
			cmd->component_base_flag = 1;
		} else if( strcmp(opt,"component_mod_service_manager") == 0 ) {
			cmd->component_mod_service_manager = 1;
		} else if( strcmp(opt,"component_mod_component_impl") == 0 ) {
			cmd->component_mod_component_impl = 1;
		} else if( strcmp(opt,"wrapstring") == 0 ) {
			cmd->wrapstring_flag = 1;
		} else if( strcmp(opt,"header") == 0 ) {
			cmd->header = 1;
		} else if( opt[0] == 't' ) {
			opt++;
			ae_set_trace( atoi(opt) );
		} else if( strncmp(opt,"o:",2) == 0 ) {
			opt = &(argv[i][3]);
			strcpy( cmd->outfile, opt );
		} else if( strncmp(opt,"p:",2) == 0 ) {
			opt = &(argv[i][3]);
			err = chdir( opt );
			if( err != 0 ) {
				fprintf( stderr, "WARNING: couldn't change to the specified path.\n" );
				return 0;
			}
//			fprintf( stderr, "INFO: changing directory to %s.\n", opt );
		} else {
			fprintf( stderr, "WARNING: unknown option '-%s'\n", opt );
		}
	}

	// now read in the filename
	if( i == argc ) {
		fprintf( stderr, "WARNING: no filename specified.\n" );
		return 0;
	}
	cmd->infile = argv[i];
	
	// done 
	return 1;
}


int
main(argc, argv)
	int argc;
	char *argv[];

{
	// parse the command line
	if (!parseargs(argc, argv, &command_line)) {
		fprintf( stderr, "usage: %s [-header | -client | -server | -component_base | -component_mod_service_manager | -component_mod_component_impl | -w | -t#] interface_file\n", argv[0] );
		fprintf( stderr, "\tt0 - Component stubs.\n" );
		fprintf( stderr, "\tt2 - Service manager.\n" );
		fprintf( stderr, "\tt4 - Component instance.\n" );
		fprintf( stderr, "\tt6 - Client stub.\n" );
		exit(1);
	}

	// print the header if appropriate
	if( command_line.client_flag || command_line.server_flag || command_line.header ) {
		h_output(command_line.infile, "-DRPC_HDR", EXTEND, ".h");
		reinitialize();
	}

	// output the xdr file if appropriate
	if( command_line.client_flag || command_line.server_flag ) {
		c_output(command_line.infile, "-DRPC_XDR", EXTEND, "_xdr.c");
		reinitialize();
	}

	// output the client files
	if( command_line.client_flag ) {
		l_output(command_line.infile, "-DRPC_CLNT", EXTEND, "_clnt.c");
		reinitialize();
		if( command_line.component_base_flag ) {
			ae_output_rpc_file( command_line.infile, ".h", TYPE_CLIENT_H );
			reinitialize();
			ae_output_rpc_file( command_line.infile, ".cpp", TYPE_CLIENT_C );
			reinitialize();
		}
	}

	// output the server files
	if( command_line.server_flag ) {
		s_output(allc, allv, command_line.infile, "-DRPC_SVC", EXTEND, "_svc.c", 0);
		reinitialize();
		if( command_line.component_base_flag ) {
			ae_output_rpc_file( command_line.infile, "_svc_stub_impl.cpp", TYPE_SVC_STUB_IMPL );
			reinitialize();
		}
		if( command_line.component_mod_service_manager ) {
			ae_output_rpc_file( command_line.infile, ".cpp", TYPE_SVC_MGR_C );
			reinitialize();
			ae_output_rpc_file( command_line.infile, ".h", TYPE_SVC_MGR_H );
			reinitialize();
		}
		if( command_line.component_mod_component_impl ) {
			ae_output_rpc_file( command_line.infile, ".h", TYPE_INST_H );
			reinitialize();
			ae_output_rpc_file( command_line.infile, ".cpp", TYPE_INST_C );
			reinitialize();
		}
	}

	exit(0);
}
