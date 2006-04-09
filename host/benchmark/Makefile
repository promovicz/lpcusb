# app defs
EXE = main.exe
LIBUSB = "C:\Program Files\LibUSB-Win32-0.1.10.1"

# tool defs
CFLAGS = -W -Wall -g -I$(LIBUSB)/include
LIBS = $(LIBUSB)/lib/gcc/libusb.a

all: $(EXE)

$(EXE): main.o
	$(CC) -o $(EXE) $< $(LIBS)

clean:
	$(RM) $(EXE) main.o 
	
	