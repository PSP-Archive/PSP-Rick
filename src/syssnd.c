/*
 * xrick/src/syssnd.c
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

#include <stdlib.h>

#include "config.h"

#include "system.h"
#include "game.h"
#include "syssnd.h"
#include "debug.h"
#include "data.h"

#include "psp.h"

#include <pspaudiolib.h>

#ifdef ENABLE_SOUND

#define NB_SND_CHANS 4

struct sound_channel { 
  int is_playing; 
  int loop;
  unsigned char *ptr;
  int len;
  sound_t *sound;
}; 

struct sound_channel sndchans[4];
/*char *curptr=NULL;
int curlen=0;*/

static void wavout_snd0_callback(short *_buf, unsigned long _reqn)
{
	int j,k;
	int l,i;
	char *p;

	memset(_buf, 0, _reqn*4);

	l=((_reqn*4)/8);

	p=(char *)_buf;
	for(i=0;i<l;i++)
	{
		int pcm=0;
		for(j=0;j<NB_SND_CHANS;j++)
		{
			if(sndchans[j].is_playing)
			{
				pcm+=((int)(*(sndchans[j].ptr++))-0x80)*2;
				sndchans[j].len--;
				if(!sndchans[j].len)
					sndchans[j].is_playing = 0;
			}
		}
		if(pcm<-0x7f) pcm=-0x7f;
		if(pcm>0x7f) pcm=0x7f;
		for(k=0;k<8;k++)
			*(p++) = (char)pcm;
	}
}

void
syssnd_init(void)
{
	pspAudioSetChannelCallback(0, (void *)&wavout_snd0_callback); 
}

/*
 * Shutdown
 */
void
syssnd_shutdown(void)
{
}

/*
 * Pause
 *
 * pause: TRUE or FALSE
 * clear: TRUE to cleanup all sounds and make sure we start from scratch
 */
void
syssnd_pause(U8 pause, U8 clear)
{
}

/*
 *
 */
void
syssnd_free(sound_t *s)
{
	free(s);
}

/*
 * Load a sound.
 */
sound_t *
syssnd_load(char *name)
{
  sound_t *s;
  int len;

  data_file_t *f;
  f = data_file_open(name);
  if(!f)
  {
    sys_panic("can't load file: %s",name);
  }
  s = (sound_t *)malloc(sizeof(sound_t));
  data_file_seek(f, 0, SEEK_END);
  len=data_file_tell(f)-0x2c;
  data_file_seek(f, 0x2c, SEEK_SET);
  s->buf = (char *)malloc(len);
  s->len = len;
  data_file_read(f, s->buf, len, 1);
  data_file_close(f);
  return s;
}

/*
 * Play a sound
 *
 * loop: number of times the sound should be played, -1 to loop forever
 * returns: channel number, or -1 if none was available
 *
 * NOTE if sound is already playing, simply reset it (i.e. can not have
 * twice the same sound playing -- tends to become noisy when too many
 * bad guys die at the same time).
 */
S8
syssnd_play(sound_t *sound, S8 loop)
{
	int i;
	if (sound == NULL) return -1;
/*	curptr = sound->buf;
	curlen = sound->len;*/
	for(i=0;i<NB_SND_CHANS;i++)
	{
		if(!sndchans[i].is_playing)
		{
			sndchans[i].ptr=sound->buf;
			sndchans[i].len=sound->len;
			sndchans[i].sound=sound;
			sndchans[i].loop=loop;
			sndchans[i].is_playing=1;
			return i;
		}
	}
	return -1;
}

/*
 * Stop a sound
 */
void
syssnd_stopsound(sound_t *sound)
{
	int i;
	for(i=0;i<NB_SND_CHANS;i++)
	{
		if(sndchans[i].sound==sound)
		{
			sndchans[i].is_playing=0;
		}
	}
}

/*
 * Stops all channels.
 */
void
syssnd_stopall(void)
{
	int i;
	for(i=0;i<NB_SND_CHANS;i++)
	{
		sndchans[i].is_playing=0;
	}
}

#endif
