/*
 * xrick/src/sysarg.c
 *
 * Copyright (C) 1998-2002 BigOrno (bigorno@bigorno.net). All rights reserved.
 *
 * The use and distribution terms for this software are contained in the file
 * named README, which can be found in the root of this distribution. By
 * using this software in any fashion, you are agreeing to be bound by the
 * terms of this license.
 *
 * You must not remove this notice, or any other, from this software.
 */

/*
 * 20021010 added test to prevent buffer overrun in -keys parsing.
 */

#include <stdio.h>

int sysarg_args_map = 0;
int sysarg_args_submap = 0;
int sysarg_args_period = 0;
int sysarg_args_nosound = 0;
char *sysarg_args_data = NULL;

/*
 * Read and process arguments
 */
void
sysarg_init(int argc, char **argv)
{
	sysarg_args_map = 0;
}
