#
# Makefile for PSPRICK
#

INCDIR = ../include zlib
CFLAGS = -O3 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
USE_NEWLIB_LIBC = 0

LIBDIR =
LIBS = -lpspaudiolib -lpspaudio

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = PSPRICK v0.3
PSP_EBOOT_ICON = ICON0.PNG
PSP_EBOOT_PIC1 = PIC1.PNG

TARGET    = rick
LIBRARIES = 
MATHLIB   = 

.SUFFIXES: .o .c .h .m .i .S .rc .res

OBJS = unzip.o scr_xrick.o scr_pause.o scr_imain.o scr_imap.o scr_gameover.o scr_getname.o dat_picsPC.o dat_picsST.o dat_screens.o dat_tilesPC.o dat_tilesST.o dat_maps.o dat_ents.o dat_spritesST.o dat_spritesPC.o ents.o e_bullet.o e_bomb.o e_rick.o e_sbonus.o e_them.o e_bonus.o e_box.o rects.o util.o game.o draw.o maps.o sysvid.o syskbd.o control.o system.o scroller.o sysevt.o sysarg.o syssnd.o sysjoy.o dat_snd.o zlib/inflate.o zlib/inftrees.o zlib/crc32.o zlib/adler32.o zlib/zutil.o zlib/inffast.o psprick.o data.o 
#OBJS = psprick.o
       
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
