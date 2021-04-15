/*
 * xrick/src/system.c
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

//#include <SDL.h>

#include <stdarg.h>   /* args for sys_panic */
#include <fcntl.h>    /* fcntl in sys_panic */
#include <stdio.h>    /* printf */
#include <stdlib.h>
//#include <signal.h>

#include "system.h"
#include "psp.h"

/*
 * Panic
 */
void
sys_panic(char *err, ...)
{
#if 1
  va_list argptr;
  char s[1024];

  /* change stdin to non blocking */
  /*fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) & ~FNDELAY);*/
  /* NOTE HPUX: use ... is it OK on Linux ? */
  /* fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) & ~O_NDELAY); */

  /* prepare message */
  va_start(argptr, err);
  vsprintf(s, err, argptr);
  va_end(argptr);

  /* print message and die */
//  printf("%s\npanic!\n", s);
	pgPrint(0,1,0xffff,"panic!!");
	pgPrint(0,2,0xffff,s);
  exit(1);
#endif
}


/*
 * Print a message
 */
void
sys_printf(char *msg, ...)
{
#if 0
  va_list argptr;
  char s[1024];

  /* change stdin to non blocking */
  /*fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) & ~FNDELAY);*/
  /* NOTE HPUX: use ... is it OK on Linux ? */
  /* fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) & ~O_NDELAY); */

  /* prepare message */
  va_start(argptr, msg);
  vsprintf(s, msg, argptr);
  va_end(argptr);
  printf(s);
#endif
}

/*
 * Return number of microseconds elapsed since first call
 */
/* unsigned int getTicks()
 {
	unsigned now = sceKernelLibcClock();
	unsigned diff = now - prevtick;
	prevtick = now;
	return diff/(TICKFREQ/freq);
 }*/

U32
sys_gettime(void)
{
	static int first = 1;
    static long start_sec;
    struct timeval tv;

    sceKernelLibcGettimeofday (&tv, NULL);
    if (first)
    {
   		start_sec = tv.tv_sec;
		first = 0;
    }
	return ((unsigned long) ((tv.tv_sec - start_sec) * 1000 + tv.tv_usec / 1000));
}

/*
 * Sleep a number of microseconds
 */
void
sys_sleep(int s)
{
	U32 start = sys_gettime();
	while(sys_gettime()<(start+s));
}

/*
 * Initialize system
 */
void
sys_init(int argc, char **argv)
{
	sysarg_init(argc, argv);
	sysvid_init();
#ifdef ENABLE_JOYSTICK
	sysjoy_init();
#endif
#ifdef ENABLE_SOUND
	if (sysarg_args_nosound == 0)
		syssnd_init();
#endif
	atexit(sys_shutdown);
//	signal(SIGINT, exit);
//	signal(SIGTERM, exit);
}

/*
 * Shutdown system
 */
void
sys_shutdown(void)
{
#ifdef ENABLE_SOUND
	syssnd_shutdown();
#endif
#ifdef ENABLE_JOYSTICK
	sysjoy_shutdown();
#endif
	sysvid_shutdown();
}

/* eof */
