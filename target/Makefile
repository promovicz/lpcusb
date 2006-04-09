LIBNAME	= usbstack

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

CFLAGS  = -I./ -I../ -c -W -Wall -Os -g -DDEBUG -mcpu=arm7tdmi
ARFLAGS = -rcs

LIBOBJS =	usbhw_lpc.o usbcontrol.o usbstdreq.o usbinit.o usbdescrip.o

all: lib examples

clean:
	$(RM) -f $(LIBNAME).a $(LIBOBJS)
	make -C examples clean

examples:
	make -C examples

# build lib
lib: $(LIBNAME).a

$(LIBNAME).a: $(LIBOBJS)
	$(AR) $(ARFLAGS) $@ $^

# Builds release tar file
dist: clean
	cd .. && $(TAR) --exclude={CVS,cvs} -cvzf $(PKG_NAME)-$(DATE).tar.gz $(PKG_NAME)

# recompile if the Makefile changes
$(LIBOBJS): Makefile

# phony targets
.PHONY: clean examples
