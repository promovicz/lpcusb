/*
	LPCUSB, an USB device driver for LPC microcontrollers	
	Copyright (C) 2006 Bertrik Sikken (bertrik@sikken.nl)

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	   derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/**
	Hardware register definitions for the LPC214x USB controller

	These are private to the usbhw module
*/


#define MAMCR		*(volatile unsigned int *)0xE01FC000
#define MAMTIM		*(volatile unsigned int *)0xE01FC004

#define PLLCON		*(volatile unsigned int *)0xE01FC080
#define PLLCFG		*(volatile unsigned int *)0xE01FC084
#define PLLSTAT		*(volatile unsigned int *)0xE01FC088
#define PLLFEED		*(volatile unsigned int *)0xE01FC08C

#define VPBDIV		*(volatile unsigned int *)0xE01FC100

#define PINSEL0		*(volatile unsigned int *)0xE002C000

#define U0THR		*(volatile unsigned int *)0xE000C000
#define U0RBR		*(volatile unsigned int *)0xE000C000
#define U0DLL		*(volatile unsigned int *)0xE000C000
#define U0DLM		*(volatile unsigned int *)0xE000C004
#define U0FCR		*(volatile unsigned int *)0xE000C008
#define U0LCR		*(volatile unsigned int *)0xE000C00C
#define U0LSR		*(volatile unsigned int *)0xE000C014

// interrupts
#define VICIntSelect   *((volatile unsigned int *) 0xFFFFF00C)
#define VICIntEnable   *((volatile unsigned int *) 0xFFFFF010)
#define VICVectAddr    *((volatile unsigned int *) 0xFFFFF030)
#define VICVectAddr0   *((volatile unsigned int *) 0xFFFFF100)
#define VICVectCntl0   *((volatile unsigned int *) 0xFFFFF200)

/* Common LPC2148 definitions, related to USB */
#define	PCONP			*(volatile unsigned int *)0xE01FC0C4
#define	PLL1CON			*(volatile unsigned int *)0xE01FC0A0
#define	PLL1CFG			*(volatile unsigned int *)0xE01FC0A4
#define	PLL1STAT		*(volatile unsigned int *)0xE01FC0A8
#define	PLL1FEED		*(volatile unsigned int *)0xE01FC0AC

#define PINSEL0			*(volatile unsigned int *)0xE002C000
#define PINSEL1			*(volatile unsigned int *)0xE002C004
#define IOPIN0			*(volatile unsigned int *)0xE0028000
#define IOSET0			*(volatile unsigned int *)0xE0028004
#define IODIR0			*(volatile unsigned int *)0xE0028008
#define IOCLR0			*(volatile unsigned int *)0xE002800C

/* USB register definitions */
#define USBIntSt		*(volatile unsigned int *)0xE01FC1C0

#define USBDevIntSt		*(volatile unsigned int *)0xE0090000
#define USBDevIntEn		*(volatile unsigned int *)0xE0090004
#define USBDevIntClr	*(volatile unsigned int *)0xE0090008
#define USBDevIntSet	*(volatile unsigned int *)0xE009000C
#define USBDevIntPri	*(volatile unsigned int *)0xE009002C

#define USBEpIntSt		*(volatile unsigned int *)0xE0090030
#define USBEpIntEn		*(volatile unsigned int *)0xE0090034
#define USBEpIntClr		*(volatile unsigned int *)0xE0090038
#define USBEpIntSet		*(volatile unsigned int *)0xE009003C
#define USBEpIntPri		*(volatile unsigned int *)0xE0090040

#define USBReEp			*(volatile unsigned int *)0xE0090044
#define USBEpInd		*(volatile unsigned int *)0xE0090048
#define USBMaxPSize		*(volatile unsigned int *)0xE009004C

#define USBRxData		*(volatile unsigned int *)0xE0090018
#define USBRxPLen		*(volatile unsigned int *)0xE0090020
#define USBTxData		*(volatile unsigned int *)0xE009001C
#define USBTxPLen		*(volatile unsigned int *)0xE0090024
#define USBCtrl			*(volatile unsigned int *)0xE0090028

#define USBCmdCode		*(volatile unsigned int *)0xE0090010
#define USBCmdData		*(volatile unsigned int *)0xE0090014

/*
	LPCUSB, an USB device driver for LPC microcontrollers	
	Copyright (C) 2006 Bertrik Sikken (bertrik@sikken.nl)

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	   derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/**
	Hardware register definitions for the LPC214x USB controller

	These are private to the usbhw module
*/


#define MAMCR		*(volatile unsigned int *)0xE01FC000
#define MAMTIM		*(volatile unsigned int *)0xE01FC004

#define PLLCON		*(volatile unsigned int *)0xE01FC080
#define PLLCFG		*(volatile unsigned int *)0xE01FC084
#define PLLSTAT		*(volatile unsigned int *)0xE01FC088
#define PLLFEED		*(volatile unsigned int *)0xE01FC08C

#define VPBDIV		*(volatile unsigned int *)0xE01FC100

#define PINSEL0		*(volatile unsigned int *)0xE002C000

#define U0THR		*(volatile unsigned int *)0xE000C000
#define U0RBR		*(volatile unsigned int *)0xE000C000
#define U0DLL		*(volatile unsigned int *)0xE000C000
#define U0DLM		*(volatile unsigned int *)0xE000C004
#define U0FCR		*(volatile unsigned int *)0xE000C008
#define U0LCR		*(volatile unsigned int *)0xE000C00C
#define U0LSR		*(volatile unsigned int *)0xE000C014

// interrupts
#define VICIntSelect   *((volatile unsigned int *) 0xFFFFF00C)
#define VICIntEnable   *((volatile unsigned int *) 0xFFFFF010)
#define VICVectAddr    *((volatile unsigned int *) 0xFFFFF030)
#define VICVectAddr0   *((volatile unsigned int *) 0xFFFFF100)
#define VICVectCntl0   *((volatile unsigned int *) 0xFFFFF200)

/* Common LPC2148 definitions, related to USB */
#define	PCONP			*(volatile unsigned int *)0xE01FC0C4
#define	PLL1CON			*(volatile unsigned int *)0xE01FC0A0
#define	PLL1CFG			*(volatile unsigned int *)0xE01FC0A4
#define	PLL1STAT		*(volatile unsigned int *)0xE01FC0A8
#define	PLL1FEED		*(volatile unsigned int *)0xE01FC0AC

#define PINSEL0			*(volatile unsigned int *)0xE002C000
#define PINSEL1			*(volatile unsigned int *)0xE002C004
#define IOPIN0			*(volatile unsigned int *)0xE0028000
#define IOSET0			*(volatile unsigned int *)0xE0028004
#define IODIR0			*(volatile unsigned int *)0xE0028008
#define IOCLR0			*(volatile unsigned int *)0xE002800C

/* USB register definitions */
#define USBIntSt		*(volatile unsigned int *)0xE01FC1C0

#define USBDevIntSt		*(volatile unsigned int *)0xE0090000
#define USBDevIntEn		*(volatile unsigned int *)0xE0090004
#define USBDevIntClr	*(volatile unsigned int *)0xE0090008
#define USBDevIntSet	*(volatile unsigned int *)0xE009000C
#define USBDevIntPri	*(volatile unsigned int *)0xE009002C

#define USBEpIntSt		*(volatile unsigned int *)0xE0090030
#define USBEpIntEn		*(volatile unsigned int *)0xE0090034
#define USBEpIntClr		*(volatile unsigned int *)0xE0090038
#define USBEpIntSet		*(volatile unsigned int *)0xE009003C
#define USBEpIntPri		*(volatile unsigned int *)0xE0090040

#define USBReEp			*(volatile unsigned int *)0xE0090044
#define USBEpInd		*(volatile unsigned int *)0xE0090048
#define USBMaxPSize		*(volatile unsigned int *)0xE009004C

#define USBRxData		*(volatile unsigned int *)0xE0090018
#define USBRxPLen		*(volatile unsigned int *)0xE0090020
#define USBTxData		*(volatile unsigned int *)0xE009001C
#define USBTxPLen		*(volatile unsigned int *)0xE0090024
#define USBCtrl			*(volatile unsigned int *)0xE0090028

#define USBCmdCode		*(volatile unsigned int *)0xE0090010
#define USBCmdData		*(volatile unsigned int *)0xE0090014

/*
	LPCUSB, an USB device driver for LPC microcontrollers	
	Copyright (C) 2006 Bertrik Sikken (bertrik@sikken.nl)

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	   derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/**
	Hardware register definitions for the LPC214x USB controller

	These are private to the usbhw module
*/


#define MAMCR		*(volatile unsigned int *)0xE01FC000
#define MAMTIM		*(volatile unsigned int *)0xE01FC004

#define PLLCON		*(volatile unsigned int *)0xE01FC080
#define PLLCFG		*(volatile unsigned int *)0xE01FC084
#define PLLSTAT		*(volatile unsigned int *)0xE01FC088
#define PLLFEED		*(volatile unsigned int *)0xE01FC08C

#define VPBDIV		*(volatile unsigned int *)0xE01FC100

#define PINSEL0		*(volatile unsigned int *)0xE002C000

#define U0THR		*(volatile unsigned int *)0xE000C000
#define U0RBR		*(volatile unsigned int *)0xE000C000
#define U0DLL		*(volatile unsigned int *)0xE000C000
#define U0DLM		*(volatile unsigned int *)0xE000C004
#define U0FCR		*(volatile unsigned int *)0xE000C008
#define U0LCR		*(volatile unsigned int *)0xE000C00C
#define U0LSR		*(volatile unsigned int *)0xE000C014

// interrupts
#define VICIntSelect   *((volatile unsigned int *) 0xFFFFF00C)
#define VICIntEnable   *((volatile unsigned int *) 0xFFFFF010)
#define VICVectAddr    *((volatile unsigned int *) 0xFFFFF030)
#define VICVectAddr0   *((volatile unsigned int *) 0xFFFFF100)
#define VICVectCntl0   *((volatile unsigned int *) 0xFFFFF200)

/* Common LPC2148 definitions, related to USB */
#define	PCONP			*(volatile unsigned int *)0xE01FC0C4
#define	PLL1CON			*(volatile unsigned int *)0xE01FC0A0
#define	PLL1CFG			*(volatile unsigned int *)0xE01FC0A4
#define	PLL1STAT		*(volatile unsigned int *)0xE01FC0A8
#define	PLL1FEED		*(volatile unsigned int *)0xE01FC0AC

#define PINSEL0			*(volatile unsigned int *)0xE002C000
#define PINSEL1			*(volatile unsigned int *)0xE002C004
#define IOPIN0			*(volatile unsigned int *)0xE0028000
#define IOSET0			*(volatile unsigned int *)0xE0028004
#define IODIR0			*(volatile unsigned int *)0xE0028008
#define IOCLR0			*(volatile unsigned int *)0xE002800C

/* USB register definitions */
#define USBIntSt		*(volatile unsigned int *)0xE01FC1C0

#define USBDevIntSt		*(volatile unsigned int *)0xE0090000
#define USBDevIntEn		*(volatile unsigned int *)0xE0090004
#define USBDevIntClr	*(volatile unsigned int *)0xE0090008
#define USBDevIntSet	*(volatile unsigned int *)0xE009000C
#define USBDevIntPri	*(volatile unsigned int *)0xE009002C

#define USBEpIntSt		*(volatile unsigned int *)0xE0090030
#define USBEpIntEn		*(volatile unsigned int *)0xE0090034
#define USBEpIntClr		*(volatile unsigned int *)0xE0090038
#define USBEpIntSet		*(volatile unsigned int *)0xE009003C
#define USBEpIntPri		*(volatile unsigned int *)0xE0090040

#define USBReEp			*(volatile unsigned int *)0xE0090044
#define USBEpInd		*(volatile unsigned int *)0xE0090048
#define USBMaxPSize		*(volatile unsigned int *)0xE009004C

#define USBRxData		*(volatile unsigned int *)0xE0090018
#define USBRxPLen		*(volatile unsigned int *)0xE0090020
#define USBTxData		*(volatile unsigned int *)0xE009001C
#define USBTxPLen		*(volatile unsigned int *)0xE0090024
#define USBCtrl			*(volatile unsigned int *)0xE0090028

#define USBCmdCode		*(volatile unsigned int *)0xE0090010
#define USBCmdData		*(volatile unsigned int *)0xE0090014

