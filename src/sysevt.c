/*
 * xrick/src/sysevt.c
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
 * 20021010 SDLK_n replaced by SDLK_Fn because some non-US keyboards
 *          requires that SHIFT be pressed to input numbers.
 */

#include "psp.h"
#include "system.h"
#include "config.h"
#include "game.h"
#include "debug.h"
#include "control.h"
#include "draw.h"

#define SETBIT(x,b) x |= (b)
#define CLRBIT(x,b) x &= ~(b)

static ctrl_data_t ctl;
static unsigned long control_bef_ctl  = 0;
static unsigned long control_bef_tick = 0;
#define REPEAT_TIME 0x40000

unsigned long Read_Key(void) 
{
	sceCtrlReadBufferPositive(&ctl, 1); 
	control_bef_ctl  = ctl.buttons;
	control_bef_tick = ctl.frame;
	return control_bef_ctl;
} 

/*
 * Process events, if any, then return
 */
void
sysevt_poll(void)
{
	unsigned long key;
	key = Read_Key();

	if (key & CTRL_CROSS)
	{
		SETBIT(control_status, CONTROL_FIRE);
	}
	else
	{
		CLRBIT(control_status, CONTROL_FIRE);
	}
	if (key & CTRL_LEFT)
	{
		SETBIT(control_status, CONTROL_LEFT);
	}
	else
	{
		CLRBIT(control_status, CONTROL_LEFT);
	}
	if (key & CTRL_RIGHT)
	{
		SETBIT(control_status, CONTROL_RIGHT);
	}
	else
	{
		CLRBIT(control_status, CONTROL_RIGHT);
	}
	if (key & CTRL_UP || key & CTRL_CIRCLE)
	{
		SETBIT(control_status, CONTROL_UP);
	}
	else
	{
		CLRBIT(control_status, CONTROL_UP);
	}
	if (key & CTRL_DOWN)
	{
		SETBIT(control_status, CONTROL_DOWN);
	}
	else
	{
		CLRBIT(control_status, CONTROL_DOWN);
	}
	if (key & CTRL_SQUARE)
	{
		//bomb
		SETBIT(control_status, CONTROL_FIRE);
		SETBIT(control_status, CONTROL_DOWN);
		CLRBIT(control_status, CONTROL_LEFT);
		CLRBIT(control_status, CONTROL_RIGHT);
		CLRBIT(control_status, CONTROL_UP);
	}
	static int in_bullet=0;
	if (key & CTRL_TRIANGLE)
	{
		//bullet
		SETBIT(control_status, CONTROL_FIRE);
		if(!in_bullet)
		{
			in_bullet = 1;
		}
		else
		{
			SETBIT(control_status, CONTROL_UP);
		}
		CLRBIT(control_status, CONTROL_LEFT);
		CLRBIT(control_status, CONTROL_RIGHT);
		CLRBIT(control_status, CONTROL_DOWN);
	}
	else
	{
		in_bullet = 0;
	}

	if(key & CTRL_SELECT)
	{
		//map selection
		memset(pgGetVramAddr(0,0),0,512*2*272);
		extern void pgPrint(unsigned long x,unsigned long y,unsigned long color,const char *str);

		pgPrint(0,0,0xFFFFFF,"Select map:");
		pgPrint(0,2,0xFFFFFF,"1 - South America");
		pgPrint(0,3,0xFFFFFF,"2 - Egypt");
		pgPrint(0,4,0xFFFFFF,"3 - Europe");
		pgPrint(0,5,0xFFFFFF,"4 - Europe 2");

		pgPrint(0,7,0xFFFFFF,"O - Accept     X - Cancel");

		int sel = sysarg_args_map;

		while(1)
		{
			unsigned long key;
			key = Read_Key();

			pgPrint(30,2+sel,0xFFFFFF,"<---");

			if(key & CTRL_CIRCLE) 
			{
				sysarg_args_map = sel;
				game_reset();
				memset(pgGetVramAddr(0,0),0,512*2*272);
				break;
			}
			if(key & CTRL_CROSS)
			{
				memset(pgGetVramAddr(0,0),0,512*2*272);
				sysvid_update(&draw_SCREENRECT);
				break;
			}
			if(key & CTRL_DOWN)
			{
				pgPrint(30,2+sel,0xFFFFFF,"    ");
				sel++;
				if(sel>3) sel=3;
				while(1)
				{
					key = Read_Key();
					if(!(key&CTRL_DOWN)) break;
				}
			}
			if(key & CTRL_UP)
			{
				pgPrint(30,2+sel,0xFFFFFF,"    ");
				sel--;
				if(sel<0) sel=0;
				while(1)
				{
					key = Read_Key();
					if(!(key&CTRL_UP)) break;
				}
			}
		}
	}
}

/*
 * Wait for an event, then process it and return
 */
void
sysevt_wait(void)
{
	sysevt_poll();
}
