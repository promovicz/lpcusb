LIBNAME	= usbstack
APPNAME = main

# Package definitions
PKG_NAME	= target
DATE		= $$(date +%Y%m%d)

# Tool definitions
CC      = arm-elf-gcc
LD      = arm-elf-ld -v
AR      = arm-elf-ar
AS      = arm-elf-as
CP      = arm-elf-objcopy
OD		= arm-elf-objdump
RM		= rm
TAR		= tar

CFLAGS  = -I./ -c -W -Wall -Os -g -DDEBUG -mcpu=arm7tdmi
ASFLAGS = -ahls -mapcs-32 -o crt.o
LFLAGS  =  -T lpc2148_flash.cmd -nostartfiles
CPFLAGS = -O ihex
ODFLAGS	= -x --syms

OBJS =	crt.o startup.o printf.o console.o main_hid.o

LIBOBJS =	usbhw_lpc.o usbcontrol.o usbstdreq.o usbinit.o usbdescrip.o

all: lib app

clean:
	$(RM) -f $(LIBNAME).a $(LIBOBJS)
	$(RM) -f $(APPNAME).hex $(APPNAME).elf $(OBJS) *.lst $(APPNAME).dmp $(APPNAME).map

# build lib
lib: $(LIBOBJS)
	$(RM) -f $(LIBNAME).a
	$(AR) -r $(LIBNAME).a $(LIBOBJS)

app: $(APPNAME).elf
	@ echo "...copying"
	$(CP) $(CPFLAGS) $(APPNAME).elf $(APPNAME).hex
	$(OD) $(ODFLAGS) $(APPNAME).elf > $(APPNAME).dmp

$(APPNAME).elf: $(OBJS) $(LIBNAME).a lpc2148_flash.cmd
	@ echo "..linking"
	$(CC) $(OBJS) $(LIBNAME).a $(LFLAGS) -o $(APPNAME).elf -Wl,-Map,$(APPNAME).map

crt.o: crt.s
	@ echo ".assembling"
	$(CC) -c $(AFLAGS) -Wa,-ahlms=crt.lst crt.s -o crt.o
	
# Builds release tar file
dist: clean
	cd .. && $(TAR) --exclude={CVS,cvs} -cvzf $(PKG_NAME)-$(DATE).tar.gz $(PKG_NAME)

# recompile if the Makefile changes
$(OBJS) $(LIBOBJS): Makefile

