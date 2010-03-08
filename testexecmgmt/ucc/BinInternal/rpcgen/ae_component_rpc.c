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
* System Includes
*
*/



#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "rpc_util.h"
#include "rpc_pars.h"
#include "rpc_scan.h"


/*********************************************************************
 *
 * Local Includes
 *
 ********************************************************************/
#include "ae_component_rpc.h"


/*********************************************************************
 *
 * Definitions
 *
 ********************************************************************/
#define BANNER_START "/****************************************************************************************\n"
#define BANNER_LINE  " * "
#define BANNER_END   " ***************************************************************************************/\n"


/*********************************************************************
 *
 * Macro Functions
 *
 ********************************************************************/
#define O(x)	     f_print(fout,x);


/*********************************************************************
 *
 * Types
 *
 ********************************************************************/
typedef void (*header_func_t)(char *filename, definition *program);
typedef void (*footer_func_t)(definition *program);
typedef void (*proc_func_t)(definition *program,proc_list *proc, char *version);


/*********************************************************************
 *
 * Prototypes
 *
 ********************************************************************/
void open_input( char *infile, char *define );
void open_output( char *infile, char *outfile );
char *extendfile( char *file, char *ext );
char *locase( char *str);

// filename related helpers
void PrintStartDefs( definition *def );
void PrintEndDefs( definition *def );
void PrintIncludeInterfaceHeader();

// banners
void PrintFileBanner( char *filename );
void PrintBanner( char *title );
void PrintAutogenBanner( char *filename );
void PrintAutogenModBanner( char *filename );

// classname helpers
char *ComponentClassname( definition *def );
char *ClientComponentClassname( definition *def );
char *ServiceManagerClassname( definition *def );

// function helpers
void PrintClientStubSignature( int indent, char *classname, proc_list *proc );
void PrintComponentInstanceMethodSignature( char *classname, definition *program, proc_list *proc );
void PrintSetErrorValue( int indent, definition *program, proc_list *proc, char *error );
int IsSetErrorCallRequired( proc_list *proc );
void PrintServiceCheck( definition *program, proc_list *proc );
void PrintCallFromServiceStubToInstance( definition *program, proc_list *proc, char *extraargs );
void PrintCallFromServiceStubToStatic( definition *program, proc_list *proc, char *extra_args );

// misc helpers
char *ucase( char *str );
void trace( int indent, definition *program, proc_list *proc, char *fname, int enter );

// service stub implementations
void output_header_service_implementation( char *filename, definition *program );
void output_footer_service_implementation( definition *program );
void output_proc_service_implementation( definition *program, proc_list *proc, char *version );
void output_std_proc_service_implementation( definition *program, proc_list *proc );
void output_cstr_proc_service_implementation( definition *program, proc_list *proc );
void output_dstr_proc_service_implementation( definition *program, proc_list *proc );
void output_startup_proc_service_implementation( definition *program, proc_list *proc );
void output_shutdown_proc_service_implementation( definition *program, proc_list *proc );
void output_list_proc_service_implementation( definition *program, proc_list *proc );
void output_static_proc_service_implementation( definition *program, proc_list *proc );

// service manager class header
void output_header_svc_header( char *filename, definition *program );
void output_footer_svc_header( definition *program );
void output_proc_svc_header( definition *program, proc_list *proc, char *version );

// service manager class cpp
void output_header_svc_cpp( char *filename, definition *program );
void output_footer_svc_cpp( definition *program );
void output_proc_svc_cpp( definition *program, proc_list *proc, char *version );

// server component class header
void output_header_inst_header( char *filename, definition *program );
void output_footer_inst_header( definition *program );
void output_proc_inst_header( definition *program, proc_list *proc, char *version );

// server component class cpp
void output_header_inst_cpp( char *filename, definition *program );
void output_footer_inst_cpp( definition *program );
void output_proc_inst_cpp( definition *program, proc_list *proc, char *version );

// client component stub class header
void output_header_stub_header( char *filename, definition *program );
void output_footer_stub_header( definition *program );
void output_proc_stub_header( definition *program, proc_list *proc, char *version );

// client component stub class cpp
void output_header_stub_cpp( char *filename, definition *program );
void output_footer_stub_cpp( definition *program );
void output_proc_stub_cpp( definition *program, proc_list *proc, char *version );
void output_proc_stub_cpp_std( definition *program, proc_list *proc, char *version );


/*********************************************************************
 *
 * Static Vars
 *
 ********************************************************************/
static header_func_t hf[] = { output_header_service_implementation, output_header_svc_header, output_header_svc_cpp,	output_header_inst_header,	output_header_inst_cpp,	output_header_stub_header,	output_header_stub_cpp	};
static footer_func_t ff[] = { output_footer_service_implementation, output_footer_svc_header, output_footer_svc_cpp,	output_footer_inst_header,	output_footer_inst_cpp,	output_footer_stub_header,	output_footer_stub_cpp	};
static proc_func_t   pf[] = { output_proc_service_implementation,   output_proc_svc_header,	  output_proc_svc_cpp,		output_proc_inst_header,	output_proc_inst_cpp,	output_proc_stub_header,	output_proc_stub_cpp	};

static char *input_filename;
static int gtype;
static int trace_points[7] = { 0 };

/*********************************************************************
 *
 * ae_output_rpc_file()
 *
 ********************************************************************/
void ae_output_rpc_file( char *infile, char *outfile, int type )
{
	char tmpfilename[128];
	char *outfilename;
	int foundprogram = 0;
	definition *def, *def_program;
	version_list *vp;
	proc_list *proc;

	// save some stuff for use down the chain
	input_filename = infile;
	gtype = type;

	// open the input file 
	open_input( infile, "-DRPC_SVC" );

	// look for the program definition
	while( (def = get_definition()) ) {
		if( def->def_kind == DEF_PROGRAM ) {
			def_program = def;
			foundprogram++;
		}
	}

	// if there is no program then exit 
	if( foundprogram == 0 ) {
		return;
	}

	// open the output file
	if( (type == TYPE_SVC_MGR_H) || (type == TYPE_SVC_MGR_C) ) {
		sprintf( tmpfilename, "%s", ServiceManagerClassname(def_program) );
		infile = tmpfilename;
	} else if( (type == TYPE_INST_H) || (type == TYPE_INST_C) ) {
		sprintf( tmpfilename, "%s", ComponentClassname(def_program) );
		infile = tmpfilename;
	} else if( (type == TYPE_CLIENT_H) || (type == TYPE_CLIENT_C) ) {
		sprintf( tmpfilename, "%s", ClientComponentClassname(def_program) );
		infile = tmpfilename;
	}
	outfilename = extendfile(infile, outfile);
	open_output( infile, outfilename );

	// if there is more than one program then say I don't handle this
	if( foundprogram > 1 ) {
		fprintf( stderr, "ERROR: PR0094 modifications can't handle multiple program definitions in a single file.\n" );
		return;
	}

	// I also don't handle multiple versions in a single file
	vp = def_program->def.pr.versions;
	if( vp->next != NULL ) {
		fprintf( stderr, "ERROR: PR0094 modifications can't handle multiple program versions in a single file.\n" );
		return;
	}

	// HOOK: Insert header code
	hf[type]( outfilename, def_program );

	// process each procedure
	for( proc = vp->procs; proc != NULL; proc = proc->next ) {
		// HOOK: Insert proc code
		pf[type]( def_program, proc, vp->vers_num );
	}

	// HOOK: Insert footer code
	ff[type]( def_program );

	// done
	return;
}


/*********************************************************************
 *
 * SECTION: Service Stub Implementation
 *
 ********************************************************************/

/*********************************************************************
 *
 * Service Stub Implementation -- START
 *
 ********************************************************************/
void output_header_service_implementation( char *filename, definition *program )
{
	// output the autogen banner
	PrintAutogenBanner( filename );

	// output the system includes
	PrintBanner( "System Includes" );
	f_print( fout, "#include <assert.h>\n" );

	// output the local includes
	PrintBanner( "Local Includes" );
	f_print( fout, "#include \"%s.h\"\n", ServiceManagerClassname(program) );
	f_print( fout, "#include \"%s.h\"\n", ComponentClassname(program) );
	f_print( fout, "#include \"CComponentManager.h\"\n" );

	// output the static variables
	PrintBanner( "Static Variables" );
	f_print( fout, "static CComponentManager<%s> *iComponentManager;\n", ComponentClassname(program) );
}


/*********************************************************************
 *
 * Service Stub Implementation -- END
 *
 ********************************************************************/
void output_footer_service_implementation( definition *program )
{
}


/*********************************************************************
 *
 * Service Stub Implementation -- METHOD GENERATION (TOP LEVEL)
 *
 ********************************************************************/
void output_proc_service_implementation( definition *program, proc_list *proc, char *version )
{
	char str[512];

	// print banner
	sprintf( str, "PUBLIC FUNCTION: %s", locase(proc->proc_name) );
	PrintBanner( str );

	// print the function signature
	ptype( proc->res_prefix, proc->res_type, 1 );
	f_print( fout, "*");
	pvname( proc->proc_name, version );
	f_print(fout, "( ");
	ptype( proc->arg_prefix, proc->arg_type, 1 );
	f_print(fout, "*aArgs, CLIENT *aDutout )\n");

	// demux based on the name of the prefix
	if( strncmp(proc->proc_name,"SS_",3) == 0 ) {
		output_startup_proc_service_implementation( program, proc );
	} else if( strncmp(proc->proc_name,"SC_",3) == 0 ) {
		output_shutdown_proc_service_implementation( program, proc );
	} else if( strncmp(proc->proc_name,"CSTR_",5) == 0 ) {
		output_cstr_proc_service_implementation( program, proc );
	} else if( strncmp(proc->proc_name,"DSTR_",5) == 0 ) {
		output_dstr_proc_service_implementation( program, proc );
	} else if( strncmp(proc->proc_name,"LIST_",5) == 0 ) {
		output_list_proc_service_implementation( program, proc );
	} else if( strncmp(proc->proc_name,"ST_",3) == 0 ) {
		output_static_proc_service_implementation( program, proc );
	} else {
		output_std_proc_service_implementation( program, proc );
	}
}


/*********************************************************************
 *
 * Service Stub Implementation -- METHOD GENERATION (STANDARD METHODS)
 *
 ********************************************************************/
void output_std_proc_service_implementation( definition *program, proc_list *proc )
{
	int void_return_type = 0;

	// start
	f_print( fout, "{\n" );

	// check for void return value
	if( strncmp(proc->res_type,"void",4) == 0 ) {
		void_return_type = 1;
	}

	// declare variables
	f_print( fout, "\tstatic " );
	if( void_return_type == 0 ) {
		ptype( proc->res_prefix, proc->res_type, 1 );
	} else {
		f_print( fout, "int " );
	}	
	f_print( fout, "rv;\n");
	f_print( fout, "\tint instance_key;\n" );
	f_print( fout, "\t%s *session;\n\n", ComponentClassname(program) );
	trace( 1, program, proc, NULL, 1 );

	// check that the service is already started
	PrintServiceCheck( program, proc );

	// set the return value to generic error
	O( "\t// set the return value to generic error\n" );
	PrintSetErrorValue( 1, program, proc, "ERR_FAILED_TO_RETRIEVE_KEY" );
	f_print( fout, "\n" );

	// get the key for the instance from the args
	O( "\t// get the key for the instance from the args\n" );
	f_print( fout, "\tinstance_key = %s::GetInstanceKeyFromArgs( %s, (void*)aArgs );\n", ServiceManagerClassname(program), proc->proc_name );
	O( "\tif( instance_key < 0 ) {\n" );
	trace( 2, program, proc, NULL, 0 );
	O( "\t\treturn &rv;\n" );
	O( "\t}\n" );
	O( "\n" );

	// make sure the key is valid
	O( "\t// make sure the key is valid\n" );
	O( "\tif( iComponentManager->IsValidKey(instance_key) == 0 ) {\n" );		
	PrintSetErrorValue( 2, program, proc, "ERR_INSTANCE_DOES_NOT_EXIST" );
	O( "\t\treturn &rv;\n" );
	O( "\t}\n" );
	O( "\n" );
	
	// get the instance
	O( "\t// get the instance\n" );
	O( "\tsession = iComponentManager->GetInstance( instance_key );\n" );
	O( "\tassert( session != NULL );\n" );
	O( "\n" );

	// call the corresponding method on the instance and return the result
	PrintCallFromServiceStubToInstance( program, proc, NULL );
	trace( 1, program, proc, NULL, 0 );
	f_print( fout, "\treturn &rv;\n" );
	f_print( fout, "}\n" );
}


/*********************************************************************
 *
 * Service Stub Implementation -- METHOD GENERATION (STATIC METHODS)
 *
 ********************************************************************/
void output_static_proc_service_implementation( definition *program, proc_list *proc )
{
	int void_return_type = 0;

	// start
	f_print( fout, "{\n" );

	// check for void return value
	if( strncmp(proc->res_type,"void",4) == 0 ) {
		void_return_type = 1;
	}

	// declare the return variable
	f_print( fout, "\tstatic " );
	if( void_return_type == 0 ) {
		ptype( proc->res_prefix, proc->res_type, 1 );
	} else {
		f_print( fout, "int " );
	}	
	f_print( fout, "rv;\n");
	trace( 1, program, proc, NULL, 1 );
	O( "\n" );

	// check that the service is already started
	PrintServiceCheck( program, proc );

	// call the corresponding method on the instance and return the result
	PrintCallFromServiceStubToStatic( program, proc, NULL );
	trace( 1, program, proc, NULL, 0 );
	f_print( fout, "\treturn &rv;\n" );
	f_print( fout, "}\n" );
}


/*********************************************************************
 *
 * Service Stub Implementation -- METHOD GENERATION (CSTR METHODS)
 *
 ********************************************************************/
void output_cstr_proc_service_implementation( definition *program, proc_list *proc )
{
	int integer_return_type;

	// start
	f_print( fout, "{\n" );

	// a void return type is illegal for a cstr_ method
	if( strncmp(proc->res_type,"void",4) == 0 ) {
		fprintf( stderr, "ERROR: 'cstr_' methods must not have void return type.\n" );
		return;
	}

	// declare variables
	f_print( fout, "\tstatic " );
	ptype( proc->res_prefix, proc->res_type, 1 );
	f_print( fout, "rv;\n" );
	f_print( fout, "\tint result;\n" );
	f_print( fout, "\tint internal_err;\n" );
	f_print( fout, "\tint instance_key;\n" );
	f_print( fout, "\t%s *session;\n", ComponentClassname(program) );
	trace( 1, program, proc, NULL, 1 );
	O( "\n" );
	
	// check that the service is already started
	PrintServiceCheck( program, proc );

	// create a new component instance
	O( "\t// create a new component instance\n" );
	O( "\tinstance_key = iComponentManager->CreateInstance();\n" );
	O( "\tif( instance_key < 0 ) {\n" );
	PrintSetErrorValue( 2, program, proc, "instance_key" );
	trace( 2, program, proc, NULL, 0 );
	O( "\t\treturn &rv;\n" );
	O( "\t}\n\n" );

	// get the instance
	O( "\t// get the instance\n" );
	O( "\tsession = iComponentManager->GetInstance( instance_key );\n" );
	O( "\tassert( session != NULL );\n" );
	O( "\n" );

	// call the corresponding method on the instance
	PrintCallFromServiceStubToInstance( program, proc, NULL );

	// now extract the top level error code
	integer_return_type = IsSetErrorCallRequired( proc );
	if( integer_return_type == 0 ) {
		f_print( fout, "\tresult = rv;\n" );
	} else {
		f_print( fout, "\tresult = %s::GetError( %s, (void*)&rv );\n", ServiceManagerClassname(program), proc->proc_name );
	}
	O( "\n" );

	// if the method fails then we cleanup the instance -- otherwise we set the instance key
	// as the return value
	O( "\t// if the method fails then we cleanup the instance -- otherwise we set the instance key\n" );
	O( "\t// as the return value\n" );
	O( "\tif( result != ERR_NONE ) {\n" );
	O( "\t\tinternal_err = iComponentManager->DeleteInstance( instance_key );\n" );
	O( "\t\tassert( internal_err == 0 );\n" );
	O( "\t} else { \n" );
	PrintSetErrorValue( 2, program, proc, "instance_key" );
	O( "\t}\n" );
	O( "\n" );

	// done
	f_print( fout, "\t// done\n" );
	trace( 1, program, proc, NULL, 0 );
	f_print( fout, "\treturn &rv;\n" );
	f_print( fout, "}\n" );
}


/*********************************************************************
 *
 * Service Stub Implementation -- METHOD GENERATION (DSTR METHODS)
 *
 ********************************************************************/
void output_dstr_proc_service_implementation( definition *program, proc_list *proc )
{
	int integer_return_type;

	// start
	f_print( fout, "{\n" );

	// declare variables
	f_print( fout, "\tstatic " );
	ptype( proc->res_prefix, proc->res_type, 1 );
	f_print( fout, "rv;\n" );
	f_print( fout, "\tint result;\n" );
	f_print( fout, "\tint internal_err;\n" );
	f_print( fout, "\tint instance_key;\n" );
	f_print( fout, "\tint should_delete = 1;\n" );
	f_print( fout, "\t%s *session;\n\n", ComponentClassname(program) );
	trace( 1, program, proc, NULL, 1 );
	
	// check that the service is already started
	PrintServiceCheck( program, proc );

	// get the key for the instance from the args
	O( "\t// get the key for the instance from the args\n" );
	f_print( fout, "\tinstance_key = %s::GetInstanceKeyFromArgs( %s, (void*)aArgs );\n", ServiceManagerClassname(program), proc->proc_name );
	O( "\tif( instance_key < 0 ) {\n" );
	PrintSetErrorValue( 2, program, proc, "instance_key" );
	trace( 2, program, proc, NULL, 0 );
	O( "\t\treturn &rv;\n" );
	O( "\t}\n" );

	// make sure the key is valid
	O( "\t// make sure the key is valid\n" );
	O( "\tif( iComponentManager->IsValidKey(instance_key) == 0 ) {\n" );		
	PrintSetErrorValue( 2, program, proc, "ERR_INSTANCE_DOES_NOT_EXIST" );
	O( "\t\treturn &rv;\n" );
	O( "\t}\n" );
	O( "\n" );

	// get the instance
	O( "\t// get the instance\n" );
	O( "\tsession = iComponentManager->GetInstance( instance_key );\n" );
	O( "\tassert( session != NULL );\n" );
	O( "\n" );

	// call the corresponding method on the instance
	PrintCallFromServiceStubToInstance( program, proc, "&should_delete" );
	O( "\n" );

	// now extract the top level error code
	integer_return_type = IsSetErrorCallRequired( proc );
	if( integer_return_type == 0 ) {
		f_print( fout, "\tresult = rv;\n" );
	} else {
		f_print( fout, "\tresult = %s::GetError( %s, (void*)&rv );\n", ServiceManagerClassname(program), proc->proc_name );
	}
	O( "\n" );

	// it is illegal for the method to succeed but not delete the instance
	O( "\t// it is illegal for the method to succeed but not delete the instance\n" );	
	O( "\tassert( (result != ERR_NONE) || (should_delete == 1) );\n\n" );

	// delete the instance if requested
	O( "\t// delete the instance if requested\n" );
	O( "\tif( should_delete != 0 ) {\n" );
	O( "\t\tinternal_err = iComponentManager->DeleteInstance( instance_key );\n" );
	O( "\t\tassert( internal_err == ERR_NONE );\n" );
	O( "\t}\n\n" );

	// done
	f_print( fout, "\t// done\n" );
	trace( 1, program, proc, NULL, 0 );
	f_print( fout, "\treturn &rv;\n" );
	f_print( fout, "}\n" );
}


/*********************************************************************
 *
 * Service Stub Implementation -- METHOD GENERATION (STARTUP METHODS)
 *
 ********************************************************************/
void output_startup_proc_service_implementation( definition *program, proc_list *proc )
{
	// check the function's signature
	if( strcmp(proc->res_type,"int") != 0 ) {
		fprintf( stderr, "ERROR: 'SS_' method has the incorrect return type. Must be 'int'.\n" );
	}

	// start
	f_print( fout, "{\n" );

	// declare variables
	f_print( fout, "\tstatic int rv;\n" );
	trace( 1, program, proc, NULL, 1 );

	// check that the service isn't already started
	f_print( fout, "\n" );
	f_print( fout, "\t// if the service is already started then return an error\n" );
	f_print( fout, "\tif( iComponentManager != NULL ) {\n" );
	f_print( fout, "\t\trv = ERR_SERVICE_ALREADY_STARTED;\n" );
	trace( 2, program, proc, NULL, 0 );
	f_print( fout, "\t\treturn &rv;\n" );
	f_print( fout, "\t}\n" );

	// create the component manager
	f_print( fout, "\n" );
	f_print( fout, "\t// create the component manager\n" );
	f_print( fout, "\tiComponentManager = new CComponentManager<%s>( INFO_MAXIMUM_OBJECTS );\n", ComponentClassname(program) );
	f_print( fout, "\tif( iComponentManager == NULL ) {\n" );
	f_print( fout, "\t\trv = ERR_FAILED_TO_CREATE_COMPONENT_MANAGER;\n" );
	trace( 2, program, proc, NULL, 0 );
	f_print( fout, "\t\treturn &rv;\n" );
	f_print( fout, "\t}\n" );

	// call the service manager
	f_print( fout, "\n" );
	f_print( fout, "\t// call the custom service manager\n" );
	f_print( fout, "\trv = %s::StartRPCService( iComponentManager, aArgs );\n", ServiceManagerClassname(program) );
	f_print( fout, "\tif( rv != ERR_NONE ) {\n" );
	f_print( fout, "\t\tdelete iComponentManager;\n" );
	f_print( fout, "\t\tiComponentManager = NULL;\n" );
	trace( 2, program, proc, NULL, 0 );
	f_print( fout, "\t\treturn &rv;\n" );
	f_print( fout, "\t}\n" );

	// done
	f_print( fout, "\n" );
	f_print( fout, "\t// success\n" );
	f_print( fout, "\trv = ERR_NONE;\n" );
	trace( 1, program, proc, NULL, 0 );
	f_print( fout, "\treturn &rv;\n" );
	f_print( fout, "}\n" );
}


/*********************************************************************
 *
 * Service Stub Implementation -- METHOD GENERATION (SHUTDOWN METHODS)
 *
 ********************************************************************/
void output_shutdown_proc_service_implementation( definition *program, proc_list *proc )
{
	// check the function's signature
	if( strcmp(proc->res_type,"int") != 0 ) {
		fprintf( stderr, "ERROR: 'SC_' method has the incorrect return type. Must be 'int'.\n" );
	}
	if( strcmp(proc->arg_type,"int") != 0 ) {
		fprintf( stderr, "ERROR: 'SC_' method has the incorrect argument type. Must be 'int'.\n" );
	}

	// start
	f_print( fout, "{\n" );

	// declare variables
	f_print( fout, "\tstatic int rv;\n" );
	f_print( fout, "\tint instance_count;\n" );
	f_print( fout, "\tint err;\n" );
	trace( 1, program, proc, NULL, 1 );
	f_print( fout, "\n" );
	
	// check that the service is already started
	PrintServiceCheck( program, proc );

	// check for active sessions
	f_print( fout, "\t// if there are active sessions and the force flag isn't set then return an error\n" );
	f_print( fout, "\tinstance_count = iComponentManager->GetInstanceCount();\n" );
	f_print( fout, "\tif( (instance_count > 0) && (*aArgs == 0) ) {\n" );
	f_print( fout, "\t\trv = ERR_ACTIVE_USER_SESSIONS;\n" );
	trace( 2, program, proc, NULL, 0 );
	f_print( fout, "\t\treturn &rv;\n" );
	f_print( fout, "\t};\n" );

	f_print( fout, "\n" );
	f_print( fout, "\t// if the force flag IS set then reset all active connections\n" );
	f_print( fout, "\tinstance_count = iComponentManager->GetInstanceCount();\n" );
	f_print( fout, "\tif( instance_count > 0 ) {\n" );
	f_print( fout, "\t\terr = iComponentManager->DeleteAllInstances();\n" );
	f_print( fout, "\t\tif( err != 0 ) {\n" ); 
	f_print( fout, "\t\t\trv = ERR_FAILED_TO_REMOVE_ACTIVE_SESSIONS;\n" );
	trace( 3, program, proc, NULL, 0 );
	f_print( fout, "\t\t\treturn &rv;\n" );
	f_print( fout, "\t\t}\n" );
	f_print( fout, "\t}\n" );

	// call the service manager
	f_print( fout, "\n" );
	f_print( fout, "\t// call the custom service manager\n" );
	f_print( fout, "\trv = %s::StopRPCService();\n", ServiceManagerClassname(program) );
	f_print( fout, "\tif( rv != ERR_NONE ) {\n" );
	trace( 2, program, proc, NULL, 0 );
	f_print( fout, "\t\treturn &rv;\n" );
	f_print( fout, "\t}\n" );
	
	// delete the component manager
	f_print( fout, "\n" );
	f_print( fout, "\t// delete the component manager\n" );
	f_print( fout, "\tdelete iComponentManager;\n" );
	f_print( fout, "\tiComponentManager = NULL;\n" );

	// done
	f_print( fout, "\n" );
	f_print( fout, "\t// success\n" );
	f_print( fout, "\trv = ERR_NONE;\n" );
	trace( 1, program, proc, NULL, 0 );
	f_print( fout, "\treturn &rv;\n" );
	f_print( fout, "}\n" );
}


/*********************************************************************
 *
 * Service Stub Implementation -- METHOD GENERATION (LIST METHODS)
 *
 ********************************************************************/
void output_list_proc_service_implementation( definition *program, proc_list *proc )
{
	int void_return_type = 0;

	// check that the signature is correct
	if( strcmp(proc->res_type,"TComponentList") != 0 ) {
		fprintf( stderr, "ERROR: 'LIST_' method has incorrect return type. Must be TComponentList.\n" );
		return;
	}
	if( strcmp(proc->arg_type,"void") != 0 ) {
		fprintf( stderr, "ERROR: 'LIST_' method has incorrect argument type. Must be void.\n" );
		return;
	}

	// start
	f_print( fout, "{\n" );

	// check for void return value
	if( strncmp(proc->res_type,"void",4) == 0 ) {
		void_return_type = 1;
	}

	// declare variables
	f_print( fout, "\tstatic " );
	if( void_return_type == 0 ) {
		ptype( proc->res_prefix, proc->res_type, 1 );
	} else {
		f_print( fout, "int " );
	}	
	f_print( fout, "rv;\n");
	f_print( fout, "\tint i, valid_count, component_instance_count;\n" );
	f_print( fout, "\n" );
	trace( 1, program, proc, NULL, 1 );

	// free any allocate memory
	O( "\t//free any allocated memory\n" );
	O( "\tif( rv.TComponentList_val != NULL ) {\n" );
	O( "\t\tfree( rv.TComponentList_val );\n" );
	O( "\t\trv.TComponentList_val = NULL;\n" );
	O( "\t\trv.TComponentList_len = 0;\n" );
	O( "\t}\n" );
	O( "\n" );

	// check that the service is already started
	PrintServiceCheck( program, proc );

	// set the return value to generic error
	O( "\t// set the return value to generic error\n" );
	PrintSetErrorValue( 1, program, proc, "ERR_FAILED_TO_RETRIEVE_KEY" );
	f_print( fout, "\n" );

	// get the number of active component instances
	O( "\t// get the number of active component instances\n" );
	O( "\tcomponent_instance_count = iComponentManager->GetInstanceCount();\n" );
	O( "\n" );
	O( "\t// if there are one or more instances then allocate the memory\n" );
	O( "\tif( component_instance_count > 0 ) {\n" );
	O( "\t\trv.TComponentList_val = (int*)malloc( sizeof(int) * component_instance_count );\n" );
	O( "\t\tassert( rv.TComponentList_val != NULL );\n" );
	O( "\t\trv.TComponentList_len = component_instance_count;\n" );
	O( "\t}\n" );
	O( "\n" );

	// set the values
	O( "\t// store all the currently active IDs in the list\n" );
	O( "\tfor( i = 0, valid_count = 0; i < INFO_MAXIMUM_OBJECTS; i++ ) {\n" );
	O( "\t\tif( iComponentManager->IsValidKey(i) != 0 ) {\n" );
	O( "\t\t\tassert( valid_count < component_instance_count );\n" );
	O( "\t\t\t(rv.TComponentList_val)[valid_count++] = i;\n" );
	O( "\t\t}\n" );
	O( "\t}\n" );
	O( "\n" );

	// ok finish up
	O( "\t// ok return everything\n" );
	f_print( fout, "\treturn &rv;\n" );
	f_print( fout, "}\n" );
}

/*********************************************************************
 *
 * SECTION: Service Manager Definition (HEADER FILE)
 *
 ********************************************************************/

/*********************************************************************
 *
 * Service Manager Definition -- START
 *
 ********************************************************************/
void output_header_svc_header( char *filename, definition *program )
{
	char cname[256];

	// output the autogen banner
	PrintAutogenModBanner( filename );
	PrintStartDefs( program );

	// output the local includes
	PrintBanner( "Local Includes" );
	PrintIncludeInterfaceHeader();
	f_print( fout, "#include \"%s.h\"\n", ComponentClassname(program) );
	f_print( fout, "#include \"CComponentManager.h\"\n" );

	// output the definition banner
	sprintf( cname, "Definition: %s", ServiceManagerClassname(program) );
	PrintBanner( cname );

	// output the definition 
	f_print( fout, "class %s\n{\n", ServiceManagerClassname(program) );
	f_print( fout, "public:\n" );
	f_print( fout, "\t/* standard methods */\n" );
	f_print( fout, "\tstatic int GetInstanceKeyFromArgs( int aMethod, void *aArgs );\n" );
	f_print( fout, "\tstatic int SetError( int aMethod, void *aArgs, int aError );\n" );
	f_print( fout, "\tstatic int GetError( int aMethod, void *aArgs );\n" );
}


/*********************************************************************
 *
 * Service Manager Definition -- END
 *
 ********************************************************************/
void output_footer_svc_header( definition *program )
{
	f_print( fout, "};\n" );
	PrintEndDefs( program );
}


/*********************************************************************
 *
 * Service Manager Definition -- METHOD GENERATION
 *
 ********************************************************************/
void output_proc_svc_header( definition *program, proc_list *proc, char *version )
{
	static int custom_comment_written = 0;

	// the StartRPCService function takes the argument to the SS_ function.
	if( strncmp(proc->proc_name,"SS_",3) == 0 ) {
		f_print( fout, "\tstatic int StartRPCService( CComponentManager<%s> *aComponentManager, %s *aArg );\n", ComponentClassname(program), proc->arg_type );
	}

	// the StopRPCService function takes no params
	if( strncmp(proc->proc_name,"SC_",3) == 0 ) {
		f_print( fout, "\tstatic int StopRPCService();\n" );
	}

	// print the header for any static methods
	if( strncmp(proc->proc_name,"ST_",3) == 0 ) {
		if( custom_comment_written == 0 ) {
			f_print( fout, "\n\t/* custom methods */\n" );
			custom_comment_written = 1;
		}
		f_print( fout, "\tstatic " );
		PrintComponentInstanceMethodSignature( NULL, program, proc );
		f_print( fout, ";\n" );
	}
}


/*********************************************************************
 *
 * SECTION: Service Manager Implementation (CPP FILE)
 *
 ********************************************************************/

/*********************************************************************
 *
 * Service Manager Implementation -- START
 *
 ********************************************************************/
void output_header_svc_cpp( char *filename, definition *program )
{
	proc_list *proc;

	// output the autogen banner
	PrintAutogenModBanner( filename );

	// output the system includes
	PrintBanner( "System Includes" );
	f_print( fout, "#include <stdio.h>\n" );

	// output the local includes
	PrintBanner( "Local Includes" );
	f_print( fout, "#include \"%s.h\"\n", ServiceManagerClassname(program) );
	PrintIncludeInterfaceHeader();

	// file-scope statics
	PrintBanner( "File-scope variables" );
	f_print( fout, "static CComponentManager<%s> *iComponentManager;\n", ComponentClassname(program) );

	// output the implementation banner
	PrintBanner( "Implementation" );

	// output GetInstanceKeyFromArgs
	PrintBanner( "PUBLIC: GetInstanceKeyFromArgs" );
	f_print( fout, "int %s::GetInstanceKeyFromArgs( int aMethod, void *aArgs )\n", ServiceManagerClassname(program) );
	f_print( fout, "{\n" );
	f_print( fout, "\tint rv;\n" );
	trace( 1, program, NULL, "GetInstanceKeyFromArgs", 1 );
	f_print( fout, "\tswitch( aMethod ) {\n" );
	for( proc = (program->def.pr.versions)->procs; proc != NULL; proc = proc->next ) {
		if( strncmp(proc->proc_name,"SS_",3) != 0 ) {
			if( strncmp(proc->proc_name,"SC_",3) != 0 ) {
				if( strncmp(proc->proc_name,"LIST_",5) != 0 ) {
					if( strncmp(proc->proc_name,"CSTR_",5) != 0 ) {
						if( strncmp(proc->proc_name,"ST_",3) != 0 ) {
							f_print( fout, "\tcase %s:\n", proc->proc_name );
						}
					}
				}
			}
		}
	}
	f_print( fout, "\tdefault:\n" );
	f_print( fout, "\t\trv = ERR_INVALID_METHOD;\n" );
	f_print( fout, "\t\tbreak;\n" );
	f_print( fout, "\t}\n" );
	trace( 1, program, NULL, "GetInstanceKeyFromArgs", 0 );
	f_print( fout, "\treturn rv;\n" );
	f_print( fout, "}\n" );

	// output SetError
	PrintBanner( "PUBLIC: SetError" );
	f_print( fout, "int %s::SetError( int aMethod, void *aArgs, int aError )\n", ServiceManagerClassname(program) );
	f_print( fout, "{\n" );
	f_print( fout, "\tint rv;\n" );
	trace( 1, program, NULL, "SetError", 1 );
	f_print( fout, "\tswitch( aMethod ) {\n" );
	for( proc = (program->def.pr.versions)->procs; proc != NULL; proc = proc->next ) {
		if( IsSetErrorCallRequired(proc) ) {
			f_print( fout, "\tcase %s:\n", proc->proc_name );
		}
	}
	f_print( fout, "\tdefault:\n" );
	f_print( fout, "\t\trv = ERR_INVALID_METHOD;\n" );
	f_print( fout, "\t\tbreak;\n" );
	f_print( fout, "\t}\n" );
	trace( 1, program, NULL, "SetError", 0 );
	f_print( fout, "\treturn rv;\n" );
	f_print( fout, "}\n" );

	// output GetError
	PrintBanner( "PUBLIC: GetError" );
	f_print( fout, "int %s::GetError( int aMethod, void *aArgs )\n", ServiceManagerClassname(program) );
	f_print( fout, "{\n" );
	f_print( fout, "\tint rv = ERR_NONE;\n" );
	trace( 1, program, NULL, "GetError", 1 );
	f_print( fout, "\tswitch( aMethod ) {\n" );
	for( proc = (program->def.pr.versions)->procs; proc != NULL; proc = proc->next ) {
		if( IsSetErrorCallRequired(proc) ) {
			f_print( fout, "\tcase %s:\n", proc->proc_name );
		}
	}
	f_print( fout, "\tdefault:\n" );
	f_print( fout, "\t\tassert( \"!INVALID CALL\" );\n" );
//	f_print( fout, "\t\trv = ERR_INVALID_METHOD;\n" );
	f_print( fout, "\t\tbreak;\n" );
	f_print( fout, "\t}\n" );
	trace( 1, program, NULL, "GetError", 0 );
	f_print( fout, "\treturn rv;\n" );
	f_print( fout, "}\n" );

}


/*********************************************************************
 *
 * Service Manager Implementation -- END
 *
 ********************************************************************/
void output_footer_svc_cpp( definition *program )
{
}


/*********************************************************************
 *
 * Service Manager Implementation -- METHOD GENERATION
 *
 ********************************************************************/
void output_proc_svc_cpp( definition *program, proc_list *proc, char *version )
{
	int void_return_type = 0;
	char str[512];

	// output StartService
	if( strncmp(proc->proc_name,"SS_",3) == 0 ) {
		PrintBanner( "PUBLIC: StartRPCService" );
		f_print( fout, "int %s::StartRPCService( CComponentManager<%s> *aComponentManager, %s *aArg )\n", ServiceManagerClassname(program), ComponentClassname(program), proc->arg_type );
		f_print( fout, "{\n" );
		trace( 1, program, NULL, "StartRPCService", 1 );
		f_print( fout, "\tassert( iComponentManager == NULL );\n" );
		f_print( fout, "\tiComponentManager = aComponentManager;\n" );
		trace( 1, program, NULL, "StartRPCService", 0 );
		f_print( fout, "\treturn ERR_NONE;\n" );
		f_print( fout, "}\n" );
	}

	// output StopRPCService
	if( strncmp(proc->proc_name,"SC_",3) == 0 ) {
		PrintBanner( "PUBLIC: StopRPCService" );
		f_print( fout, "int %s::StopRPCService()\n", ServiceManagerClassname(program) );
		f_print( fout, "{\n" );
		trace( 1, program, NULL, "StopRPCService", 1 );
		f_print( fout, "\tiComponentManager = NULL;\n" );
		trace( 1, program, NULL, "StopRPCService", 0 );
		f_print( fout, "\treturn ERR_NONE;\n" );
		f_print( fout, "}\n" );
	}

	// if this isn't a static method then exit
	if( strncmp(proc->proc_name,"ST_",3) != 0 ) {
		return;
	}

	// print banner
	_snprintf( str, 512, "PUBLIC: %s", locase(proc->proc_name) );
	PrintBanner( str );

	// print the function signature
	PrintComponentInstanceMethodSignature( ServiceManagerClassname(program), program, proc );
	f_print( fout, "\n");

	// check if the returntype is void
	if( strncmp(proc->res_type,"void",4) == 0 ) {
		void_return_type = 1;
	}

	// open the function
	f_print( fout, "{\n" );

	// declare the return value
	if( void_return_type == 0 ) {
		f_print( fout, "\t" );
		ptype( proc->res_prefix, proc->res_type, 1 );
		f_print( fout, "rv;\n" );
		trace( 1, program, proc, NULL, 1 );
		PrintSetErrorValue( 1, program, proc, "ERR_NONE" );
	} else {
		trace( 1, program, proc, NULL, 1 );
	}

	// return the value
	trace( 1, program, proc, NULL, 0 );
	if( void_return_type == 0 ) {
		f_print( fout, "\treturn rv;\n" );
	}

	// close the function
	f_print( fout, "}\n" );
}


/*********************************************************************
 *
 * SECTION: Service Component Definition (HEADER FILE)
 *
 ********************************************************************/

/*********************************************************************
 *
 * Service Component Definition - START
 *
 ********************************************************************/
void output_header_inst_header( char *filename, definition *program )
{
	char cname[256];

	// output the file banner
	PrintAutogenModBanner( filename );
	PrintStartDefs( program );

	// output the local includes
	PrintBanner( "Local Includes" );
	PrintIncludeInterfaceHeader();

	// output the definition banner
	sprintf( cname, "Definition: %s", ComponentClassname(program) );
	PrintBanner( cname );

	// output the definition 
	f_print( fout, "class %s\n{\n", ComponentClassname(program) );
	f_print( fout, "public:\n" );
	f_print( fout, "\t// Standard Methods\n" );
	f_print( fout, "\t%s();\n", ComponentClassname(program) );
	f_print( fout, "\t~%s();\n", ComponentClassname(program) );
	f_print( fout, "\tint GetKey();\n" );
	f_print( fout, "\tvoid SetKey( int aKey );\n" );
	f_print( fout, "\n" );
	f_print( fout, "\t// RPC Service Methods\n" );
}


/*********************************************************************
 *
 * Service Component Definition - END
 *
 ********************************************************************/
void output_footer_inst_header( definition *program )
{
	f_print( fout, "\n" );
	f_print( fout, "private:\n" );
	f_print( fout, "\tint iKey;\n" );
	f_print( fout, "};\n" );
	PrintEndDefs( program );
}


/*********************************************************************
 *
 * Service Component Definition - METHOD GENERATION
 *
 ********************************************************************/
void output_proc_inst_header( definition *program, proc_list *proc, char *version )
{
	// ss and sc methods aren't included in the component
	if( strncmp(proc->proc_name,"SS_",3) == 0 ) {
		return;
	}
	if( strncmp(proc->proc_name,"SC_",3) == 0 ) {
		return;
	}
	if( strncmp(proc->proc_name,"LIST_",5) == 0 ) {
		return;
	}
	if( strncmp(proc->proc_name,"ST_",3) == 0 ) {
		return;
	}
	// print the function signature
	f_print( fout, "\t" );
	PrintComponentInstanceMethodSignature( NULL, program, proc );
	f_print( fout, ";\n" );
}


/*********************************************************************
 *
 * SECTION: Service Component Implementation 
 *
 ********************************************************************/

/*********************************************************************
 *
 * Service Component Implementation - START
 *
 ********************************************************************/
void output_header_inst_cpp( char *filename, definition *program )
{
	// output the system includes
	PrintAutogenModBanner( filename );
	PrintFileBanner( "System Includes" );
	f_print( fout, "#include <stdio.h>\n" );

	// output the local includes
	PrintBanner( "Local Includes" );
	f_print( fout, "#include \"%s.h\"\n", ServiceManagerClassname(program) );
	f_print( fout, "#include \"%s.h\"\n", ComponentClassname(program) );

	// output the implementation banner
	PrintBanner( "Implementation" );

	// output the impl for the constructor
	f_print( fout, "%s::%s()\n", ComponentClassname(program), ComponentClassname(program) );
	f_print( fout, "{\n" );
	trace( 1, program, NULL, "Constructor", 1 );
	trace( 1, program, NULL, "Constructor", 0 );
	f_print( fout, "}\n\n" );

	// output the impl for the destructor
	f_print( fout, "%s::~%s()\n", ComponentClassname(program), ComponentClassname(program) );
	f_print( fout, "{\n" );
	trace( 1, program, NULL, "Destructor", 1 );
	trace( 1, program, NULL, "Destructor", 0 );
	f_print( fout, "}\n\n" );

	// output the impl for get key
	f_print( fout, "int %s::GetKey()\n", ComponentClassname(program) );
	f_print( fout, "{\n" );
	trace( 1, program, NULL, "GetKey", 1 );
	trace( 1, program, NULL, "GetKey", 0 );
	f_print( fout, "\treturn iKey;\n" );
	f_print( fout, "}\n\n" );

	// output the impl for set key
	f_print( fout, "void %s::SetKey( int aKey )\n", ComponentClassname(program) );
	f_print( fout, "{\n" );
	trace( 1, program, NULL, "SetKey", 1 );
	f_print( fout, "\tiKey = aKey;\n" );
	trace( 1, program, NULL, "SetKey", 0 );
	f_print( fout, "}\n" );
}


/*********************************************************************
 *
 * Service Component Implementation - END
 *
 ********************************************************************/
void output_footer_inst_cpp( definition *program )
{
}


/*********************************************************************
 *
 * Service Component Implementation - METHOD GENERATION
 *
 ********************************************************************/
void output_proc_inst_cpp( definition *program, proc_list *proc, char *version )
{
	char str[256];
	int void_return_type = 0;

	// ss and sc methods aren't included in the component
	if( strncmp(proc->proc_name,"SS_",3) == 0 ) {
		return;
	}
	if( strncmp(proc->proc_name,"SC_",3) == 0 ) {
		return;
	}
	if( strncmp(proc->proc_name,"LIST_",5) == 0 ) {
		return;
	}
	if( strncmp(proc->proc_name,"ST_",3) == 0 ) {
		return;
	}

	// print banner
	sprintf( str, "PUBLIC FUNCTION: %s", locase(proc->proc_name) );
	PrintBanner( str );

	// print the function signature
	PrintComponentInstanceMethodSignature( ComponentClassname(program), program, proc );
	f_print( fout, "\n");

	// check if the returntype is void
	if( strncmp(proc->res_type,"void",4) == 0 ) {
		void_return_type = 1;
	}

	// open the function
	f_print( fout, "{\n" );

	// declare the return value
	if( void_return_type == 0 ) {
		f_print( fout, "\t" );
		ptype( proc->res_prefix, proc->res_type, 1 );
		f_print( fout, "rv;\n" );
		trace( 1, program, proc, NULL, 1 );
		PrintSetErrorValue( 1, program, proc, "ERR_NONE" );
	} else {
		trace( 1, program, proc, NULL, 1 );
	}

	// return the value
	trace( 1, program, proc, NULL, 0 );
	if( void_return_type == 0 ) {
		f_print( fout, "\treturn rv;\n" );
	}

	// close the function
	f_print( fout, "}\n" );
}


/*********************************************************************
 *
 * SECTION - Client Stub Class Definition (HEADER FILE)
 *
 ********************************************************************/

/*********************************************************************
 *
 * Client Stub Class Definition - START
 *
 ********************************************************************/
void output_header_stub_header( char *filename, definition *program )
{
	char cname[256];

	// output the file banner
	PrintFileBanner( filename );
	PrintStartDefs( program );

	// output the system includes
	PrintBanner( "System Includes" );
	O( "#include <rpc/rpc.h>\n" );
	O( "#include <string>\n" );
	O( "using namespace std;\n" );
	PrintIncludeInterfaceHeader();

	// output the definition banner
	sprintf( cname, "Definition: %s", ClientComponentClassname(program) );
	PrintBanner( cname );

	// output the definition 
	f_print( fout, "class %s\n{\n", ClientComponentClassname(program) );
	f_print( fout, "public:\n" );
	f_print( fout, "\t// standard methods\n" );
	f_print( fout, "\t%s();\n", ClientComponentClassname(program) );
	f_print( fout, "\t~%s();\n", ClientComponentClassname(program) );
	f_print( fout, "\tint Connect( string aRemoteHost );\n" );
	f_print( fout, "\tint Disconnect();\n" );
	f_print( fout, "\tchar *GetLastRPCError( int *aIntErr );\n" );
	f_print( fout, "\n" );
	f_print( fout, "\t// service methods\n" );

}

/*********************************************************************
 *
 * Client Stub Class Definition - END
 *
 ********************************************************************/
void output_footer_stub_header( definition *program )
{
	f_print( fout, "\n" );
	f_print( fout, "private:\n" );
	f_print( fout, "\tstruct rpc_err iLastRPCError;\n" );
	f_print( fout, "\tCLIENT *cl;\n" );
	f_print( fout, "};\n" );
	PrintEndDefs( program );
}


/*********************************************************************
 *
 * Client Stub Class Definition - METHOD GENERATION
 *
 ********************************************************************/
void output_proc_stub_header( definition *program, proc_list *proc, char *version )
{
	// Print the signature header
	PrintClientStubSignature( 1, NULL, proc );
	f_print( fout, ";\n" );
}


/*********************************************************************
 *
 * SECTION - Client Stub Class Implementation
 *
 ********************************************************************/

/*********************************************************************
 *
 * Client Stub Class Implementation - START
 *
 ********************************************************************/
void output_header_stub_cpp( char *filename, definition *program )
{
	version_list *vp;

	// get the version info for use later 
	vp = program->def.pr.versions;

	// output the system includes
	PrintFileBanner( "System Includes" );
	O( "#include <stdio.h>\n" );
	O( "#include <assert.h>\n" );
	O( "#include <rpc/types.h>\n" );

	// output the local includes
	PrintBanner( "Local Includes" );
	f_print( fout, "#include \"%s.h\"\n", ClientComponentClassname(program) );

	// output the implementation banner
	PrintBanner( "Implementation" );

	// output the impl for the constructor
	f_print( fout, "%s::%s()\n", ClientComponentClassname(program), ClientComponentClassname(program) );
	f_print( fout, "{\n" );
	trace( 1, program, NULL, "Constructor", 1 );
	f_print( fout, "\tcl = NULL;\n" );
	f_print( fout, "\tiLastRPCError.re_status = RPC_SUCCESS;\n" );
	trace( 1, program, NULL, "Constructor", 0 );
	f_print( fout, "}\n\n" );

	// output the impl for the destructor
	f_print( fout, "%s::~%s()\n", ClientComponentClassname(program), ClientComponentClassname(program) );
	f_print( fout, "{\n" );
	trace( 1, program, NULL, "Destructor", 1 );
	f_print( fout, "\tassert( cl == NULL );\n" );
	trace( 1, program, NULL, "Destructor", 0 );
	f_print( fout, "}\n\n" );

	// output the impl for getlastrpcerror
	f_print( fout, "char *%s::GetLastRPCError( int *aIntErr )\n", ClientComponentClassname(program) );
	f_print( fout, "{\n" );
	f_print( fout, "\tstruct rpc_err rpcerr;\n" );
	f_print( fout, "\n" );
	trace( 1, program, NULL, "GetLastRPCError", 1 );
	f_print( fout, "\t// check that the handle is valid\n" );
	f_print( fout, "\tif( cl == NULL ) {\n" );
	f_print( fout, "\t\treturn NULL;\n" );
	f_print( fout, "\t}\n" );
	f_print( fout, "\n" );
	O( "\t// pass the aIntErr\n" );
	O( "\tif( aIntErr != NULL ) {\n" );
	O( "\t\tclnt_geterr( cl, &rpcerr );\n" );
	O( "\t\t*aIntErr = rpcerr.re_status;\n" );
	O( "\t}\n" );
	O( "\n" );
	O( "\t// return the errorstring\n" );
	trace( 1, program, NULL, "GetLastRPCError", 0 );
	O( "\treturn clnt_sperror( cl, NULL );\n" );
	O( "}\n" );
	f_print( fout, "\n" );

	// output the impl for connect
	f_print( fout, "int %s::Connect( string aRemoteHost )\n", ClientComponentClassname(program) );
	f_print( fout, "{\n" );
	trace( 1, program, NULL, "Connect", 1 );
	f_print( fout, "\t// check that we are not already connected\n" );
	f_print( fout, "\tif( cl != NULL ) {\n" );
	trace( 2, program, NULL, "Connect", 0 );
	f_print( fout, "\t\treturn ERR_STUB_ALREADY_CONNECTED;\n" );
	f_print( fout, "\t}\n" );
	f_print( fout, "\n" );
	f_print( fout, "\t// start the rpc library\n" );
	f_print( fout, "\trpc_nt_init();\n" );
	f_print( fout, "\n" );
	f_print( fout, "\t// connect to the service\n" );
	f_print( fout, "\tcl = clnt_create( aRemoteHost.c_str(), %s, %s, \"tcp\" );\n", program->def_name, vp->vers_name );
	f_print( fout, "\tif( cl == NULL ) {\n" );
	f_print( fout, "\t\trpc_nt_exit();\n" );
	trace( 2, program, NULL, "Connect", 0 );
	f_print( fout, "\t\treturn ERR_FAILED_TO_CONNECT;\n" );
	f_print( fout, "\t}\n" );
	f_print( fout, "\n" );
	f_print( fout, "\t// done\n" );
	trace( 1, program, NULL, "Connect", 0 );
	f_print( fout, "\treturn ERR_NONE;\n" );
	f_print( fout, "}\n\n" );

	// output the impl for disconnect
	f_print( fout, "int %s::Disconnect( )\n", ClientComponentClassname(program) );
	f_print( fout, "{\n" );
	trace( 1, program, NULL, "Disconnect", 1 );
	f_print( fout, "\t// check that we are connected\n" );
	f_print( fout, "\tif( cl == NULL ) {\n" );
	trace( 2, program, NULL, "Disconnect", 0 );
	f_print( fout, "\t\treturn ERR_STUB_NOT_CONNECTED;\n" );
	f_print( fout, "\t}\n" );
	f_print( fout, "\n" );
	f_print( fout, "\t// cleanup the client handle\n" );
	f_print( fout, "\tclnt_destroy( cl );\n" );
	f_print( fout, "\tcl = NULL;\n" );
	f_print( fout, "\trpc_nt_exit();\n" );
	f_print( fout, "\n" );
	f_print( fout, "\t// done\n" );
	trace( 1, program, NULL, "Disconnect", 0 );
	f_print( fout, "\treturn ERR_NONE;\n" );
	f_print( fout, "}\n" );
}


/*********************************************************************
 *
 * Client Stub Class Implementation - END
 *
 ********************************************************************/
void output_footer_stub_cpp( definition *program )
{
}


/*********************************************************************
 *
 * Client Stub Class Implementation - METHOD GENERATION
 *
 ********************************************************************/
void output_proc_stub_cpp( definition *program, proc_list *proc, char *version )
{
	char str[256];
	//	int void_return_type = 0;

	// ss and sc methods aren't included in the component
//	if( strncmp(proc->proc_name,"SS_",3) == 0 ) {
//		return;
//	}
//	if( strncmp(proc->proc_name,"SC_",3) == 0 ) {
//		return;
//	}

	// print banner
	sprintf( str, "PUBLIC FUNCTION: %s", locase(proc->proc_name) );
	PrintBanner( str );

	// print the function signature
	sprintf( str, "%s", ClientComponentClassname(program) );
	PrintClientStubSignature( 0, str, proc );
	f_print( fout, "\n" );

	// print the body
	output_proc_stub_cpp_std( program, proc, version );
}


void output_proc_stub_cpp_std( definition *program, proc_list *proc, char *version )
{
	int void_return_value = 0;
	int void_arg_type = 0;
	version_list *vp = program->def.pr.versions;

	// open the body
	f_print( fout, "{\n" );

	// find out if we have a void return value
	if( strncmp(proc->res_type,"void",4) == 0 ) {
		void_return_value = 1;
	}

	// find out if we have a void arg, if so then declare a dummy arg
	f_print( fout, "\tstruct rpc_err rerr;\n" );
	if( strncmp(proc->arg_type,"void",4) == 0 ) {
		void_arg_type = 1;
		f_print( fout, "\tint aArgs = 0;\n" );
	}
	f_print( fout, "\n" );
	trace( 1, program, proc, NULL, 1 );

	// check the rv (if needed)
	if( void_return_value == 0 ) {
		f_print( fout, "\t// check the rv pointer\n" );
		f_print( fout, "\tif( rv == NULL ) {\n" );
		trace( 2, program, proc, NULL, 0 );
		f_print( fout, "\t\treturn ERR_INVALID_RV_POINTER;\n" );
		f_print( fout, "\t}\n" );
		f_print( fout, "\n" );
	}

	// check that we are connected
	f_print( fout, "\t// check that we have a connection\n" );
	f_print( fout, "\tif( cl == NULL ) {\n" );
	f_print( fout, "\t\treturn ERR_STUB_NOT_CONNECTED;\n" );
	trace( 2, program, proc, NULL, 0 );
	f_print( fout, "\t}\n" );
	f_print( fout, "\n" );

	// do the call
	f_print( fout, "\t// do the call\n\t" );
	if( void_return_value == 0 ) {
		f_print( fout, "*rv = *" );
	}
	f_print( fout, "%s_%s( &aArgs, cl );\n", locase(proc->proc_name), vp->vers_num );
	f_print( fout, "\n" );

	// check for rpc errors and return the result
	f_print( fout, "\t// check for rpc errors and return the result\n" );
	f_print( fout, "\tclnt_geterr( cl, &rerr );\n" );
	f_print( fout, "\tif( rerr.re_status != RPC_SUCCESS ) {\n" );
	f_print( fout, "\t\tiLastRPCError = rerr;\n" );
	trace( 2, program, proc, NULL, 0 );
	f_print( fout, "\t\treturn ERR_RPC_ERROR;\n" );
	f_print( fout, "\t}\n" );
	trace( 1, program, proc, NULL, 0 );
	f_print( fout, "\treturn ERR_NONE;\n" );
	f_print( fout, "}\n" );
}



/*********************************************************************
 *
 * SECTION - HELPERS
 *
 ********************************************************************/

/*********************************************************************
 *
 * HELPERS: Filename Related Code Generation
 *
 ********************************************************************/
void PrintStartDefs( definition *def )
{
	char cstr[64];

	// find the prefix
	cstr[0] = 0;
	if( gtype == TYPE_CLIENT_H ) {
		sprintf( cstr, "CC" );
	} else if( gtype == TYPE_INST_H ) {
		sprintf( cstr, "CS" );
	} else if( gtype == TYPE_SVC_MGR_H ) {
		sprintf( cstr, "CSVC" );
	}

	// print the tags
	f_print( fout, "#ifndef __%s%s_H__\n", cstr, ucase(def->def_name) );
	f_print( fout, "#define __%s%s_H__\n", cstr, ucase(def->def_name) );
}


void PrintEndDefs( definition *def )
{
	f_print( fout, "\n#endif\n" ); 
}


void PrintIncludeInterfaceHeader()
{
	char *ptr, delim = 0;

	// find the '.' in the input filename (.x) and set to null
	ptr = strchr( input_filename, '.' );
	if( ptr != NULL ) {
		delim = *ptr;
		*ptr = 0;
	}

	// print the line
	f_print( fout, "#include \"%s.h\"\n", input_filename );

	// put the dot back
	if( delim != 0 ) {
		*ptr = delim;
	}
}


/*********************************************************************
 *
 * HELPERS: Classname Related Code Generation
 *
 ********************************************************************/
char *ComponentClassname( definition *def )
{
	static char component_classname[256];
	sprintf( component_classname, "CS%c%s", toupper(def->def_name[0]), locase(&(def->def_name[1])) );
	return component_classname;
}


char *ClientComponentClassname( definition *def )
{
	static char component_classname[256];
	sprintf( component_classname, "CC%c%s", toupper(def->def_name[0]), locase(&(def->def_name[1])) );
	return component_classname;
}


char *ServiceManagerClassname( definition *def )
{
	static char service_manager_classname[256];
	sprintf( service_manager_classname, "CSvc%c%s", toupper(def->def_name[0]), locase(&(def->def_name[1])) );
	return service_manager_classname;
}


/*********************************************************************
 *
 * HELPERS: Function Related Code Generation
 *
 ********************************************************************/
void PrintCallFromServiceStubToInstance( definition *program, proc_list *proc, char *extra_args )
{
	int void_arg_type = 0;
	int void_res_type = 0;

	// check for a void argument
	if( strncmp(proc->arg_type,"void",4) == 0 ) {
		void_arg_type = 1;
	}

	// check for a void ret
	if( strncmp(proc->res_type,"void",4) == 0 ) {
		void_res_type = 1;
	}

	// print the comment
	O( "\t// call the corresponding method on the instance\n\t" );

	// only store the return value is the thing doesn't return void
	if( void_res_type == 0 ) {
		f_print( fout, "rv = " );
	} else {
		f_print( fout, "rv = ERR_NONE;\n\t" );
	}

	// print the method name and open the bracket
	f_print( fout, "session->%s(", locase(proc->proc_name) );

	// if we have more to print then put in a space
	if( (void_arg_type == 0) || (extra_args != NULL) ) {
		f_print( fout, " " );
	}

	// put in the args 
	if( void_arg_type == 0 ) {
		f_print( fout, "*aArgs" );
		if( extra_args != NULL ) {
			f_print( fout, "," );
		}
		f_print( fout, " " );
	}

	// put the extra args + space
	if( extra_args != NULL ) {
		f_print( fout, "%s ", extra_args );
	}

	// close up
	f_print( fout, ");\n" );
}


void PrintCallFromServiceStubToStatic( definition *program, proc_list *proc, char *extra_args )
{
	int void_arg_type = 0;
	int void_res_type = 0;

	// check for a void argument
	if( strncmp(proc->arg_type,"void",4) == 0 ) {
		void_arg_type = 1;
	}

	// check for a void ret
	if( strncmp(proc->res_type,"void",4) == 0 ) {
		void_res_type = 1;
	}

	// print the comment
	O( "\t// call the corresponding method on the instance\n" );

	// only store the return value is the thing doesn't return void
	if( void_res_type == 0 ) {
		f_print( fout, "\trv = " );
	} else {
		f_print( fout, "\trv = ERR_NONE;\n\t" );
	}

	// print the method name and open the bracket
	f_print( fout, "%s::", ServiceManagerClassname(program) );
	f_print( fout, "%s(", locase(proc->proc_name) );

	// if we have more to print then put in a space
	if( (void_arg_type == 0) || (extra_args != NULL) ) {
		f_print( fout, " " );
	}

	// put in the args 
	if( void_arg_type == 0 ) {
		f_print( fout, "*aArgs" );
		if( extra_args != NULL ) {
			f_print( fout, "," );
		}
		f_print( fout, " " );
	}

	// put the extra args + space
	if( extra_args != NULL ) {
		f_print( fout, "%s ", extra_args );
	}

	// close up
	f_print( fout, ");\n" );
}


void PrintServiceCheck( definition *program, proc_list *proc )
{
	// check that the service is already started
	f_print( fout, "\t// check that the service is started\n" );
	f_print( fout, "\tif( iComponentManager == NULL ) {\n" );
	PrintSetErrorValue( 2, program, proc, "ERR_SERVICE_NOT_RUNNING" );
	trace( 2, program, proc, NULL, 0 );
	f_print( fout, "\t\treturn &rv;\n" );
	f_print( fout, "\t}\n\n" );
}


void PrintClientStubSignature( int indent, char *classname, proc_list *proc )
{
	int i;
	int non_void_argument;

	// indent
	for( i = 0; i < indent; i++ ) {
		f_print( fout, "\t" );
	}

	// print the function signature
	f_print( fout, "int " );
	
	// print classname if defined
	if( classname != NULL ) {
		f_print( fout, "%s::", classname );
	}
	f_print( fout, "%s( ", locase(proc->proc_name) );

	// only put in the arg if it isn't void
	if( strncmp(proc->arg_type,"void",4) != 0 ) {
		ptype( proc->arg_prefix, proc->arg_type, 1 );
		f_print( fout, "aArgs" );
		non_void_argument = 1;
	}

	// only put the return value if it is non-null
	if( strncmp(proc->res_type,"void",4) != 0 ) {
		if( non_void_argument == 1 ) {
			f_print( fout, ", " );
		}
		ptype( proc->res_prefix, proc->res_type, 1 );
		f_print( fout, "*rv " );
	} else {
		f_print( fout, " " );
	}

	// close the bracket
	f_print( fout, ")" );
}


void PrintComponentInstanceMethodSignature( char *classname, definition *program, proc_list *proc )
{
	int void_arg_type = 0;
	int i_am_a_destructor = 0;

	// print the return type
	ptype( proc->res_prefix, proc->res_type, 1 );

	// now print the method name
	if( classname != NULL ) {
		f_print( fout, "%s::", classname );
	}
	f_print( fout, "%s( ", locase(proc->proc_name) );

	// see if the arg is null
	if( strncmp(proc->arg_type,"void",4) == 0 ) {
		void_arg_type = 1;
	}

	// see if this is a destr
	if( strncmp(proc->proc_name,"DSTR_",5) == 0 ) {
		i_am_a_destructor = 1;
	}

	// if the rv is null and we are not a destructor then
	if( (void_arg_type == 1) && (i_am_a_destructor == 1) ) {
		f_print( fout, "int *aDeleteInstance " );
	} else if( (void_arg_type == 0) && (i_am_a_destructor == 1) ) {
		ptype( proc->arg_prefix, proc->arg_type, 1 );
		f_print( fout, "aArgs, int *aDeleteInstance " );
	} else if( (void_arg_type == 1) && (i_am_a_destructor == 0) ) {
		f_print( fout, "void " );
	} else if( (void_arg_type == 0) && (i_am_a_destructor == 0) ) {
		ptype( proc->arg_prefix, proc->arg_type, 1 );
		f_print( fout, "aArgs " );
	}
	f_print( fout, ")");
}


void PrintSetErrorValue( int indent, definition *program, proc_list *proc, char *error )
{
	int i;
	int set_error_call_required = 0;

	// check params
	assert( program != NULL );
	assert( proc != NULL );
	assert( error != NULL );

	// write the indent
	for( i = 0; i < indent; i++ ) {
		f_print( fout, "\t" );
	}

	// check if we need a set error call
	set_error_call_required = IsSetErrorCallRequired( proc );

	// if it is integral then we just do an assignment
	if( set_error_call_required == 0 ) {
		f_print( fout, "rv = %s;\n", error );
	} else {
		f_print( fout, "%s::SetError( %s, (void*)&rv, %s );\n", ServiceManagerClassname(program), proc->proc_name, error );
	}

	// done
}


int IsSetErrorCallRequired( proc_list *proc )
{
	int set_error_call_required = 1;

	if( strncmp(proc->res_type,"int",3) == 0 ) {
		set_error_call_required = 0;
	} else if( strncmp(proc->res_type,"short",5) == 0 ) {
		set_error_call_required = 0;
	} else if( strncmp(proc->res_type,"long",4) == 0 ) {
		set_error_call_required = 0;
	} else if( strncmp(proc->res_type,"void",4) == 0 ) {
		set_error_call_required = 0;
	}

	return set_error_call_required;
}

/*********************************************************************
 *
 * HELPERS: File Banners
 *
 ********************************************************************/
void PrintAutogenBanner( char *filename )
{
	time_t ltime;

	f_print( fout, "%s", BANNER_START );
	f_print( fout, "%s\n", BANNER_LINE );
	f_print( fout, "%sTHIS FILE IS AUTOGENERATED. Do not modify the contents of this file directly\n", BANNER_LINE );
	f_print( fout, "%sas changes will be lost\n", BANNER_LINE );
	f_print( fout, "%s\n", BANNER_LINE );
	f_print( fout, "%s%s\n", BANNER_LINE, filename );
	time( &ltime );
	f_print( fout, "%s%s", BANNER_LINE, ctime(&ltime) );
	f_print( fout, "%s\n", BANNER_LINE );
	f_print( fout, "%s", BANNER_END );
}

void PrintAutogenModBanner( char *filename )
{
	time_t ltime;

	f_print( fout, "%s", BANNER_START );
	f_print( fout, "%s\n", BANNER_LINE );
	f_print( fout, "%sThis file was autogenerated by rpcgen, but should be modified by the developer.\n", BANNER_LINE );
	f_print( fout, "%sMake sure you don't use the -component_mod flag in future or this file will be overwritten.\n", BANNER_LINE );
	f_print( fout, "%s\n", BANNER_LINE );
	f_print( fout, "%s%s\n", BANNER_LINE, filename );
	time( &ltime );
	f_print( fout, "%s%s", BANNER_LINE, ctime(&ltime) );
	f_print( fout, "%s\n", BANNER_LINE );
	f_print( fout, "%s", BANNER_END );
}


void PrintBanner( char *title )
{
	f_print( fout, "\n\n%s", BANNER_START );
	f_print( fout, "%s\n", BANNER_LINE );
	f_print( fout, "%s%s\n", BANNER_LINE, title );
	f_print( fout, "%s\n", BANNER_LINE );
	f_print( fout, "%s", BANNER_END );
}


void PrintFileBanner( char *filename )
{
	f_print( fout, "%s", BANNER_START );
	f_print( fout, "%s\n", BANNER_LINE );
	f_print( fout, "%s%s\n", BANNER_LINE, filename );
	f_print( fout, "%s\n", BANNER_LINE );
	f_print( fout, "%s", BANNER_END );
}


/*********************************************************************
 *
 * HELPERS: Miscellaneous Helpers
 *
 ********************************************************************/
char *ucase( char *str )
{
	unsigned int i;
	static char rstr[1024];
	assert( strlen(str) < (1024 + 1) );
	for( i = 0; i < strlen(str); i++ ) {
		rstr[i] = toupper(str[i]);
	}
	rstr[i] = 0;
	return rstr;
}


void trace( int indent, definition *program, proc_list *proc, char *fname, int enter )
{
	int i;
	char direction[32];
	char function_name[512];

	// return if the type is not traced
	if( trace_points[gtype] == 0 ) {
		return;
	}

	// do the indent
	for( i = 0; i < indent; i++ ) {
		f_print( fout, "\t" );
	}	

	// get the direction string
	sprintf( direction, "%s", ((enter == 1) ? "Entering " : "Exiting" ) );

	// get the classname
	switch( gtype ) {
	case TYPE_SVC_STUB_IMPL:
		sprintf( function_name, "%s::", "ServiceStub" );
		break;

	case TYPE_SVC_MGR_C:
		sprintf( function_name, "%s::", ServiceManagerClassname(program) );
		break;

	case TYPE_INST_C:
		sprintf( function_name, "%s::", ComponentClassname(program) );
		break;

	case TYPE_CLIENT_C:
		sprintf( function_name, "%s::", ClientComponentClassname(program) );
		break;

	case TYPE_SVC_MGR_H:
	case TYPE_INST_H:
	case TYPE_CLIENT_H:
		assert( !"INVALID TYPE" );
		break;
	}

	// append the function name
	if( proc == NULL ) {
		sprintf( &(function_name[strlen(function_name)]), "%s", fname );
	} else {
		sprintf( &(function_name[strlen(function_name)]), "%s", proc->proc_name );
	}

	// output the trace line
	f_print( fout, "fprintf( stderr, \"TRACE: " );
	f_print( fout, "%s ", direction );
	f_print( fout, "%s ", function_name );
	f_print( fout, "(%%d)\\n\", __LINE__ );\n" );
}
	
	 
/*********************************************************************
 *
 * PUBLIC FUNCTION: ae_set_trace
 *
 ********************************************************************/
void ae_set_trace( int type )
{
	trace_points[type] = 1;
}


/*********************************************************************
 *
 * PUBLIC FUNCTION: ae_extend_custom_types - if the passed type is
 * one of the basic types then it is returned unmodified. Otherwise
 * we extend it with some namespacing construct.
 *
 * I am going to say that any type that begins with 'T' is a custom
 * type. This is true of everything (I think). I'm still going to 
 * check whether it is one of the in-built types to verify this. If
 * something is neither 'T' nor an inbuilt type then I'll print a 
 * warning.
 *
 ********************************************************************/
char *ae_extend_custom_types( char *base_type )
{
	int is_basic_type = 0, match, i;
	char *ptr, *n;
	static char *basic_type[] = { "void", "int", "bool", "short", "array", "bytes", "char", "double", "float", "long", "string", "u_char", "u_int", "u_long", "u_short", "vector", "enum", "struct", "wrapstring", NULL };

	// see if the type is one of the basic types. 	
	for( i = 0; basic_type[i] != NULL; i++ ) {
		match = strcmp( base_type, basic_type[i] );
		if( match == 0 ) {
			is_basic_type = 1;
		}
	}

	// check that either 'T' or is_basic_type is set
	if( (base_type[0] != 'T') && (is_basic_type == 0) ) {
		fprintf( stderr, "WARNING: don't know how to handle type '%s'.\n", base_type );
		is_basic_type = 1;
	}

	// check that both are not set
	assert( (base_type[0] != 'T') || (is_basic_type != 1) );

	// if is_basic_type is set then just strcpy
	if( is_basic_type ) {
		n = base_type;
	}

	// if 'T' then extend
	if( base_type[0] == 'T' ) {
		n = (char*)malloc( strlen(infilename) + 1 + strlen(base_type) + 1 ); 
		assert( n != NULL );
		strcpy( n, infilename );
		ptr = strchr( n, '.' );
		if( ptr != NULL )
			*ptr = 0;
		strcat( n, "_" );
		strcat( n, base_type );
	}

	return n;
}
