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

/* @(#)rpc_cout.c	2.1 88/08/01 4.0 RPCSRC */
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
//static char sccsid[] = "@(#)rpc_cout.c 1.8 87/06/24 (C) 1987 SMI";
//#endif

/*
 * rpc_cout.c, XDR routine outputter for the RPC protocol compiler 
 * Copyright (C) 1987, Sun Microsystems, Inc. 
 */
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <string.h>
#include "rpc_util.h"
#include "rpc_pars.h"
#else
#include <string.h>
#include "rpc_util.h"
#include "rpc_pars.h"
#endif
#include "ae_component_rpc.h"

/* prototypes */
static void print_ifstat( int indent, char *prefix, char *type, relation rel, char *amax, char *objname, char *name );


/* implementation */
static void
print_stat(dec)
	declaration *dec;
{
	char *prefix = dec->prefix;
	char *type = dec->type;
	char *amax = dec->array_max;
	relation rel = dec->rel;
	char name[256];

	if (isvectordef(type, rel)) {
		s_print(name, "objp->%s", dec->name);
	} else {
		s_print(name, "&objp->%s", dec->name);
	}
	print_ifstat(1, prefix, type, rel, amax, name, dec->name);
}


static void
space()
{
	f_print(fout, "\n\n");
}

static int
findtype(def, type)
	definition *def;
	char *type;
{
	if (def->def_kind == DEF_PROGRAM || def->def_kind == DEF_CONST) {
		return (0);
	} else {
		return (streq(def->def_name, type));
	}
}

static int
undefined(type)
	char *type;
{
	definition *def;

	def = (definition *) FINDVAL(defined, type, findtype);
	return (def == NULL);
}


static void
print_header(def)
	definition *def;
{
	space();
	f_print(fout, "bool_t\n");
	f_print(fout, "xdr_%s(xdrs, objp)\n", ae_extend_custom_types(def->def_name));
	f_print(fout, "\tXDR *xdrs;\n");
	f_print(fout, "\t%s ", def->def_name);
	if (def->def_kind != DEF_TYPEDEF ||
	    !isvectordef(def->def.ty.old_type, def->def.ty.rel)) {
		f_print(fout, "*");
	}
	f_print(fout, "objp;\n");
	f_print(fout, "{\n");
}

static void
print_trailer()
{
	f_print(fout, "\treturn (TRUE);\n");
	f_print(fout, "}\n");
	space();
}


static void
print_ifopen(indent, name)
	int indent;
	char *name;
{
	tabify(fout, indent);
	f_print(fout, "if (!xdr_%s(xdrs", ae_extend_custom_types(name));
}


static void
print_ifarg(arg)
	char *arg;
{
	f_print(fout, ", %s", arg);
}


static void
print_ifsizeof(prefix, type)
	char *prefix;
	char *type;
{
	if (streq(type, "bool")) {
		f_print(fout, ", sizeof(bool_t), xdr_bool");
	} else {
		f_print(fout, ", sizeof(");
		if (undefined(type) && prefix) {
			f_print(fout, "%s ", prefix);
		}
		f_print(fout, "%s), (void*)xdr_%s", type, ae_extend_custom_types(type));
	}
}

static void
print_ifclose(indent)
	int indent;
{
	f_print(fout, ")) {\n");
	tabify(fout, indent);
	f_print(fout, "\treturn (FALSE);\n");
	tabify(fout, indent);
	f_print(fout, "}\n");
}

static void
print_ifstat(indent, prefix, type, rel, amax, objname, name)
	int indent;
	char *prefix;
	char *type;
	relation rel;
	char *amax;
	char *objname;
	char *name;
{
	char *alt = NULL;

	switch (rel) {
	case REL_POINTER:
		{
			int ischar;
			ischar = strcmp( type, "char" );
			if( (ischar == 0) && (command_line.wrapstring_flag) ) {
				print_ifopen( indent, "wrapstring" );
				f_print(fout, ", %s", objname);
			} else {
				print_ifopen(indent, "pointer");
				print_ifarg("(char **)");
				f_print(fout, "%s", objname);
				print_ifsizeof(prefix, type);
			}
			break;
		}
	case REL_VECTOR:
		if (streq(type, "string")) {
			alt = "string";
		} else if (streq(type, "opaque")) {
			alt = "opaque";
		}
		if (alt) {
			print_ifopen(indent, alt);
			print_ifarg(objname);
		} else {
			print_ifopen(indent, "vector");
			print_ifarg("(char *)");
			f_print(fout, "%s", objname);
		}
		print_ifarg(amax);
		if (!alt) {
			print_ifsizeof(prefix, type);
		}
		break;
	case REL_ARRAY:
		if (streq(type, "string")) {
			alt = "string";
		} else if (streq(type, "opaque")) {
			alt = "bytes";
		}
		if (streq(type, "string")) {
			print_ifopen(indent, alt);
			print_ifarg(objname);
		} else {
			if (alt) {
				print_ifopen(indent, alt);
			} else {
				print_ifopen(indent, "array");
			}
			print_ifarg("(char **)");
			if (*objname == '&') {
				f_print(fout, "%s.%s_val, (u_int *)%s.%s_len",
					objname, name, objname, name);
			} else {
				f_print(fout, "&%s->%s_val, (u_int *)&%s->%s_len",
					objname, name, objname, name);
			}
		}
		print_ifarg(amax);
		if (!alt) {
			print_ifsizeof(prefix, type);
		}
		break;
	case REL_ALIAS:
		print_ifopen(indent, type);
		print_ifarg(objname);
		break;
	}
	print_ifclose(indent);
}


/* ARGSUSED */
static void
emit_enum(def)
	definition *def;
{
	print_ifopen(1, "enum");
	print_ifarg("(enum_t *)objp");
	print_ifclose(1);
}


static void
emit_union(def)
	definition *def;
{
	declaration *dflt;
	case_list *cl;
	declaration *cs;
	char *object;
	char *format = "&objp->%s_u.%s";

	print_stat(&def->def.un.enum_decl);
	f_print(fout, "\tswitch (objp->%s) {\n", def->def.un.enum_decl.name);
	for (cl = def->def.un.cases; cl != NULL; cl = cl->next) {
		cs = &cl->case_decl;
		f_print(fout, "\tcase %s:\n", cl->case_name);
		if (!streq(cs->type, "void")) {
			object = alloc(strlen(def->def_name) + strlen(format) +
				       strlen(cs->name) + 1);
			s_print(object, format, def->def_name, cs->name);
			print_ifstat(2, cs->prefix, cs->type, cs->rel, cs->array_max,
				     object, cs->name);
			free(object);
		}
		f_print(fout, "\t\tbreak;\n");
	}
	dflt = def->def.un.default_decl;
	if (dflt != NULL) {
		if (!streq(dflt->type, "void")) {
			f_print(fout, "\tdefault:\n");
			object = alloc(strlen(def->def_name) + strlen(format) +
				       strlen(dflt->name) + 1);
			s_print(object, format, def->def_name, dflt->name);
			print_ifstat(2, dflt->prefix, dflt->type, dflt->rel,
				     dflt->array_max, object, dflt->name);
			free(object);
			f_print(fout, "\t\tbreak;\n");
		}
	} else {
		f_print(fout, "\tdefault:\n");
		f_print(fout, "\t\treturn (FALSE);\n");
	}
	f_print(fout, "\t}\n");
}



static void
emit_struct(def)
	definition *def;
{
	decl_list *dl;

	for (dl = def->def.st.decls; dl != NULL; dl = dl->next) {
		print_stat(&dl->decl);
	}
}




static void
emit_typedef(def)
	definition *def;
{
	char *prefix = def->def.ty.old_prefix;
	char *type = def->def.ty.old_type;
	char *amax = def->def.ty.array_max;
	relation rel = def->def.ty.rel;

	print_ifstat(1, prefix, type, rel, amax, "objp", def->def_name);
}




/*
 * Emit the C-routine for the given definition 
 */
void
emit(def)
	definition *def;
{
	if (def->def_kind == DEF_PROGRAM || def->def_kind == DEF_CONST) {
		return;
	}
	print_header(def);
	switch (def->def_kind) {
	case DEF_UNION:
		emit_union(def);
		break;
	case DEF_ENUM:
		emit_enum(def);
		break;
	case DEF_STRUCT:
		emit_struct(def);
		break;
	case DEF_TYPEDEF:
		emit_typedef(def);
		break;
	case DEF_CONST:
	case DEF_PROGRAM:
		break;
	}
	print_trailer();
}
