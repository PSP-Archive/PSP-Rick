#ifndef __PSP_H__
#define __PSP_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

 

typedef unsigned long	u32;
typedef unsigned char	u8;

/* Index for the two analog directions */
#define CTRL_ANALOG_X	0 
#define CTRL_ANALOG_Y	1 

/* Button bit masks */
#define CTRL_SQUARE		0x8000
#define CTRL_TRIANGLE	0x1000
#define CTRL_CIRCLE		0x2000
#define CTRL_CROSS		0x4000
#define CTRL_UP			0x0010
#define CTRL_DOWN		0x0040
#define CTRL_LEFT		0x0080
#define CTRL_RIGHT		0x0020
#define CTRL_START		0x0008
#define CTRL_SELECT		0x0001
#define CTRL_LTRIGGER	0x0100
#define CTRL_RTRIGGER	0x0200

/* Returned control data */ 
typedef struct _ctrl_data { 
	u32	frame; 
	u32	buttons; 
	u8	analog[4]; 
	u32	unused; 
} ctrl_data_t; 

/* Not 100% sure on this, init with 0 */ 
void sceCtrlInit(int unknown); 
/* Pass 1 to enable analogue mode */ 
void sceCtrlSetAnalogMode(int on); 
/* Read in the controller data. Unknown should be set to 1 */ 
void sceCtrlRead(ctrl_data_t* paddata, int unknown); 


#define VRAM_ADDR	(0x04000000)

#define SCREEN_WIDTH	480
#define SCREEN_HEIGHT	272

void sceDisplayWaitVblankStart();
void sceDisplaySetMode(long unknown, long width, long height);
void sceDisplaySetFrameBuf(char *topaddr,long linesize,long pixelsize,long);

#define PSP_O_RDONLY	0x0001 
#define PSP_O_WRONLY	0x0002 
#define PSP_O_RDWR		0x0003 
#define PSP_O_NBLOCK	0x0010 
#define PSP_O_APPEND	0x0100 
#define PSP_O_CREAT		0x0200 
#define PSP_O_TRUNC		0x0400 
#define PSP_O_NOWAIT	0x8000 

int sceIoOpen(const char* file, int mode, int unknown); 
void sceIoClose(int fd); 
int sceIoRead(int fd, void *data, int size); 
int sceIoWrite(int fd, void *data, int size); 
long long sceIoLseek(int fd, long long offset, int whence);
int sceIoRemove(const char *file); 
int sceIoMkdir(const char *dir, int mode); 
int sceIoRmdir(const char *dir); 
int sceIoRename(const char *oldname, const char *newname); 
//int sceIoDevctl(const char *name int cmd, void *arg, size_t arglen, void *buf, size_t *buflen); 

int sceKernelAllocPartitionMemory(void* buf,int size); 
void sceKernelFreePartitionMemory(void* buf);

struct timeval {
	long tv_sec;
	long tv_usec;
};

int sceKernelLibcGettimeofday( struct timeval* tv, int null );


#define POWER_CB_POWER		0x80000000 
#define POWER_CB_HOLDON		0x40000000 
#define POWER_CB_STANDBY	0x00080000 
#define POWER_CB_RESCOMP	0x00040000 
#define POWER_CB_RESUME		0x00020000 
#define POWER_CB_SUSPEND	0x00010000 
#define POWER_CB_EXT		0x00001000 
#define POWER_CB_BATLOW		0x00000100 
#define POWER_CB_BATTERY	0x00000080 
#define POWER_CB_BATTPOWER	0x0000007F 

void sceKernelExitGame();
int sceKernelCreateCallback(const char* name, void* callback );
void sceKernelRegisterExitCallback( int id );
void sceKernelRegisterPowerCallback( int unknown, int id );
void sceKernelPollCallbacks();

int sceAudio_2( int handle, int vol1, int vol2, char* buf );	// Output sound, blocking
int sceAudio_3( int unknown0, int samples, int unknown1 );		// Initialize channel and allocate buffer
int sceAudio_4( int handle );									// Terminate channel and deallocate buffer

typedef int (*threadfunc_t)(int args, void *argp);

int sceKernelCreateThread(const char *name, threadfunc_t func, unsigned long priority, unsigned long stacksize, unsigned long attrib, void *unk);
int sceKernelStartThread(int hthread, int arg0, void *arg1);
void sceKernelExitThread(int ret);
int sceKernelWaitThreadEnd(int hthread, void *unk);
int sceKernelDeleteThread(int hthread);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PSP_H__ */
