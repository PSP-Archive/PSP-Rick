#include <stdlib.h> /* malloc */

#include "system.h"
#include "game.h"
#include "img.h"
#include "debug.h"
#include "psp.h"

U8 *sysvid_fb; /* frame buffer */

static U16 palette[256];

#ifdef GFXPC
static U8 RED[] = { 0x00, 0x50, 0xf0, 0xf0, 0x00, 0x50, 0xf0, 0xf0 };
static U8 GREEN[] = { 0x00, 0xf8, 0x50, 0xf8, 0x00, 0xf8, 0x50, 0xf8 };
static U8 BLUE[] = { 0x00, 0x50, 0x50, 0x50, 0x00, 0xf8, 0xf8, 0xf8 };
#endif
#ifdef GFXST
static U8 RED[] = { 0x00, 0xd8, 0xb0, 0xf8,
                    0x20, 0x00, 0x00, 0x20,
                    0x48, 0x48, 0x90, 0xd8,
                    0x48, 0x68, 0x90, 0xb0,
                    /* cheat colors */
                    0x50, 0xe0, 0xc8, 0xf8,
                    0x68, 0x50, 0x50, 0x68,
                    0x80, 0x80, 0xb0, 0xe0,
                    0x80, 0x98, 0xb0, 0xc8
};
static U8 GREEN[] = { 0x00, 0x00, 0x6c, 0x90,
                      0x24, 0x48, 0x6c, 0x48,
                      0x6c, 0x24, 0x48, 0x6c,
                      0x48, 0x6c, 0x90, 0xb4,
		      /* cheat colors */
                      0x54, 0x54, 0x9c, 0xb4,
                      0x6c, 0x84, 0x9c, 0x84,
                      0x9c, 0x6c, 0x84, 0x9c,
                      0x84, 0x9c, 0xb4, 0xcc
};
static U8 BLUE[] = { 0x00, 0x00, 0x68, 0x68,
                     0x20, 0xb0, 0xd8, 0x00,
                     0x20, 0x00, 0x00, 0x00,
                     0x48, 0x68, 0x90, 0xb0,
		     /* cheat colors */
                     0x50, 0x50, 0x98, 0x98,
                     0x68, 0xc8, 0xe0, 0x50,
                     0x68, 0x50, 0x50, 0x50,
                     0x80, 0x98, 0xb0, 0xc8};
#endif

void
sysvid_setPalette(img_color_t *pal, U16 n)
{
  U16 i;

  for (i = 0; i < n; i++) {
	palette[i] = ((pal[i].b>>3)<<10) + ((pal[i].g>>3)<<5) + (pal[i].r>>3);
  }
}

void
sysvid_setGamePalette()
{
  U8 i;
  img_color_t pal[256];

  for (i = 0; i < 32; ++i) {
    pal[i].r = RED[i];
    pal[i].g = GREEN[i];
    pal[i].b = BLUE[i];
  }
  sysvid_setPalette(pal, 32);
}

/*
 * Initialise video
 */
void
sysvid_init(void)
{
  /*
   * create v_ frame buffer
   */
  sysvid_fb = (U8 *)malloc(SYSVID_WIDTH * SYSVID_HEIGHT);
  if (!sysvid_fb)
    sys_panic("xrick/video: sysvid_fb malloc failed\n");
}

/*
 * Shutdown video
 */
void
sysvid_shutdown(void)
{
  free(sysvid_fb);
  sysvid_fb = NULL;
}

#define	PIXELSIZE	1				//in short
#define	LINESIZE	512				//in short
#define CMAX_X 60
#define CMAX_Y 38
#define FRAMESIZE				0x44000			//in byte

char *pg_vramtop=(char *)0x04000000;

char *pgGetVramAddr(unsigned long x,unsigned long y)
{
	return pg_vramtop+x*PIXELSIZE*2+y*LINESIZE*2+0x40000000;
}

#include "font.c"
void pgPutChar(unsigned long x,unsigned long y,unsigned long color,unsigned long bgcolor,unsigned char ch,char drawfg,char drawbg,char mag)
{
	unsigned char *vptr0;		//pointer to vram
	unsigned char *vptr;		//pointer to vram
	const unsigned char *cfont;		//pointer to font
	unsigned long cx,cy;
	unsigned long b;
	char mx,my;

	if (ch>255) return;
	cfont=font+ch*8;
	vptr0=(unsigned char*)pgGetVramAddr(x,y);
	for (cy=0; cy<8; cy++) {
		for (my=0; my<mag; my++) {
			vptr=vptr0;
			b=0x80;
			for (cx=0; cx<8; cx++) {
				for (mx=0; mx<mag; mx++) {
					if ((*cfont&b)!=0) {
						if (drawfg) *(unsigned short *)vptr=color;
						if (drawfg) *(unsigned short *)(vptr + FRAMESIZE)=color;
					} else {
//						if (drawbg) *(unsigned short *)vptr=bgcolor;
						*(unsigned short *)vptr=bgcolor;
						*(unsigned short *)(vptr + FRAMESIZE)=bgcolor;
					}
					vptr+=PIXELSIZE*2;
				}
				b=b>>1;
			}
			vptr0+=LINESIZE*2;
		}
		cfont++;
	}
}

void pgPrint(unsigned long x,unsigned long y,unsigned long color,const char *str)
{
	while (*str!=0 && x<CMAX_X && y<CMAX_Y) {
		pgPutChar(x*8,y*8,color,0,*str,1,0,1);
		str++;
		x++;
		if (x>=CMAX_X) {
			x=0;
			y++;
		}
	}
}

/*
 * Update screen
 * NOTE errors processing ?
 */
void
sysvid_update(rect_t *rects)
{
	int i,j;
	U8 *p, *q, *p0, *q0;

  if (rects == NULL)
    return;

  while (rects) {
    p0 = sysvid_fb;
    p0 += rects->x + rects->y * SYSVID_WIDTH;
    q0 = (U8 *)pgGetVramAddr(rects->x+(SCREEN_WIDTH-SYSVID_WIDTH)/2, rects->y+(SCREEN_HEIGHT-SYSVID_HEIGHT)/2);
    for(j=0;j<rects->height;j++)
	  {
		U8 *src = p0;
		U16 *dst = (U16 *)q0;
		for(i=0;i<rects->width;i++)
		  {
             *(dst++)=palette[*(src++)];
		  }
		p0 += SYSVID_WIDTH;
		q0 += 512*2;
	  }
    rects = rects->next;
  }
}

/*
 * Clear screen
 * (077C)
 */
void
sysvid_clear(void)
{
	memset(sysvid_fb, 0, SYSVID_WIDTH * SYSVID_HEIGHT);
}
