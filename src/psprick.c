/*
 * xrick/src/xrick.c
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

#include "system.h"
#include "game.h"

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspaudiolib.h>

#define VRAM_ADDR	(0x04000000)
#define SCREEN_WIDTH	480
#define SCREEN_HEIGHT	272

/* Define the module info section */
PSP_MODULE_INFO("SDKTEST", 0, 1, 1);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int g_exitRick = 0;
int __errno = 0;

int ExitCallback()
{
	// Exit game
	extern int g_exitRick;
	g_exitRick = 1;
	return 0;
}

// Thread to create the callbacks and then begin polling
int CallbackThread(void *arg)
{
	int cbid;

	cbid = sceKernelCreateCallback( "Exit Callback", (void*)ExitCallback, NULL );
	sceKernelRegisterExitCallback( cbid );

	sceKernelSleepThreadCB();
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks()
{
	int thid = 0;

	thid = sceKernelCreateThread( "update_thread", CallbackThread, 0x11, 0xFA0, 0, 0 );
	sceKernelStartThread(thid, 0, 0);

	return thid;
}

/*
 * main
 */
int main(void)
{
	pspAudioInit();
	SetupCallbacks();

	sceDisplaySetMode( 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	sceDisplaySetFrameBuf( (char*)VRAM_ADDR, 512, 1, 1 );

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(1);

    //sysarg_args_data =  "fatms0:/PSP/GAME/PSPRICK/data";
	char tmpPath[1024];
	{
	  strcpy(tmpPath, g_elf_name);
  	  char *p=tmpPath+strlen(tmpPath)-1;
	  while(*p!='/') p--;
	  *p=0;
	  strcpy(p,"/data");
	}
	sysarg_args_data = tmpPath;

	sys_init(0, NULL);
	if (sysarg_args_data)
		data_setpath(sysarg_args_data);
	else
		data_setpath("data.zip");
	game_run();
//	data_closepath();
//	sys_shutdown();
	pspAudioEnd();
    sceKernelExitGame(); 
	return 0;
}

/* eof */
