/*-------------------------------------------------------------------------
 *
 * pg_pause.c
 *      Pause operations in a database.  DDL is the first.
 *
 * Copyright (c) 2016, David Fetter (OpenSCG)
 * david@fetter.org
 *
 * Parts
 *
 * Copyright (c) 2012, Guillaume Lelarge (Dalibo),
 * guillaume.lelarge@dalibo.com
 *
 *-------------------------------------------------------------------------*/
#include "postgres.h"

#include <ctype.h>

#include "catalog/dependency.h"
#include "catalog/objectaccess.h"
#include "catalog/pg_class.h"
#include "catalog/pg_database.h"
#include "catalog/pg_namespace.h"
#include "catalog/pg_proc.h"
#include "commands/seclabel.h"
#include "executor/executor.h"
#include "fmgr.h"
#include "miscadmin.h"
#include "tcop/utility.h"
#include "utils/guc.h"

#include "commands/user.h"
#include "fmgr.h"
#include "libpq/md5.h"

static bool		ddl = false;
static object_access_hook_type old_object_access_hook;


PG_MODULE_MAGIC;

extern void _PG_init(void);

/*
 * pg_pause_ddl
 *
 * Disallow DDL
 *
 */
static void
pg_pause_ddl(ObjectAccessType access,
                       Oid classId,
                       Oid objectId,
                       int subId,
                       void *arg)
{
	if ((access == OAT_POST_CREATE || access == OAT_DROP || access == OAT_POST_ALTER) && ddl)
		ereport(ERROR,
				(errcode(ERRCODE_INSUFFICIENT_PRIVILEGE),
				errmsg("DDL is paused...for now")));

	if (old_object_access_hook != NULL)
		old_object_access_hook(access, classId, objectId, subId, arg);
}

/*
 * Module initialization function
 */
void
_PG_init(void)
{
    /* Add the hook function
	 * XXX this should actually go to a marshaler */
	old_object_access_hook = object_access_hook;
	object_access_hook = pg_pause_ddl;

    // define the custom parameter
    DefineCustomBoolVariable("pg_pause.ddl",
                             gettext_noop("If true, disallow DDL."),
                             NULL,
                             &ddl,
                             false,
                             PGC_USERSET,
                             0,
                             NULL,
                             NULL,
                             NULL);
}
