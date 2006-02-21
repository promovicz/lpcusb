/*
	LPCUSB, an USB device driver for LPC microcontrollers	
	Copyright (C) 2006 Bertrik Sikken (bertrik@sikken.nl)

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "type.h"
#include "usbdebug.h"

#include "console.h"
#include "usbapi.h"
#include "startup.h"




#define BULK_OUT_EP		0x05
#define BULK_IN_EP		0x82

#define MAX_PACKET_SIZE	64

#define LE_WORD(x)		(x&0xFF),(x>>8)


static const U8 abDescriptors[] = {

// device descriptor	
	0x12,
	DESC_DEVICE,
	LE_WORD(0x0110),
	0x00,
	0x00,
	0x00,
	MAX_PACKET_SIZE,		// max packet size
	LE_WORD(0x1234),
	LE_WORD(0xABCD),
	LE_WORD(0x0100),
	0x01,
	0x02,
	0x03,
	0x01,

// configuration descriptor
	0x09,
	DESC_CONFIGURATION,
	LE_WORD(0x20),	// wLength
	0x01,			// 1 interface
	0x01,			// config value
	0x00,			// string descriptor
	0xC0,			// attributes: self-powered
	0x32,			// maxpower

// interface
	0x09,
	DESC_INTERFACE,
	0x00,		// interface #0
	0x00,		// alternate
	0x02,		// endpoints
	0x08,		// class code = MSC
	0x06,		// subclass = SCSI
	0x50,		// protocol = BOT
	0x00,		// string index
// EP
	0x07,
	DESC_ENDPOINT,
	BULK_IN_EP,		// EP address = in
	0x02,			// bmAttributes = bulk
	LE_WORD(0x40),		// max packet size
	0x00,			// interval
// EP
	0x07,
	DESC_ENDPOINT,
	BULK_OUT_EP,
	0x02,			// bmAttributes = bulk
	LE_WORD(0x40),	// max packet size
	0x00,			// interval

// string descriptors
	0x04,
	DESC_STRING,
	LE_WORD(0x0409),

	0x1A,
	DESC_STRING,
	'M', 0, 'a', 0, 'n', 0, 'u', 0, 'f', 0, 'a', 0, 'c', 0, 't', 0, 'u', 0, 'r', 0, 'e', 0, 'r', 0,

	0x12,
	DESC_STRING,
	'P', 0, 'r', 0, 'o', 0, 'd', 0, 'u', 0, 'c', 0, 't', 0, 'X', 0,

	0x12,
	DESC_STRING,
	'D', 0, 'E', 0, 'A', 0, 'D', 0, 'C', 0, 'O', 0, 'D', 0, 'E', 0,

// terminating zero
	0
};

static U8 abBulkBuf[64];


static void BulkIn(U8 bEP, U8 bEPStatus)
{
	DBG("IN %x\n", bEPStatus);
//	USBHwEPWrite(bEP, abBulkBuf, 64);
}

static void BulkOut(U8 bEP, U8 bEPStatus)
{
	int iLen;
	
	USBHwEPRead(bEP, abBulkBuf, &iLen);

	DBG("OUT %d\n", iLen);

	// echo
	USBHwEPWrite(BULK_IN_EP, abBulkBuf, iLen);
}


static BOOL HandleClassRequest(TSetupPacket *pSetup, int *piLen, U8 **ppbData)
{
	switch (pSetup->bRequest) {
	case 0xFE:
		*ppbData[0] = 0;		// No LUNs
		*piLen = 1;
		break;
	case 0xFF:
		break;
	default:
		DBG("Unhandled class\n");
		return FALSE;
	}
	return TRUE;
}




#define BAUD_RATE	115200

/*************************************************************************
	main
	====
**************************************************************************/
int main(void)
{
	// PLL and MAM
	Initialize();

	// init DBG
	ConsoleInit(60000000 / (16 * BAUD_RATE));

	DBG("Initialising USB stack\n");

	// initialise stack
	USBInit();
	
	// register descriptors
	USBRegisterDescriptors(abDescriptors);

	// register vendor class handler
	USBRegisterRequestHandler(REQTYPE_TYPE_CLASS, HandleClassRequest);
	
	// register endpoint handlers
	USBHwRegisterEPIntHandler(BULK_IN_EP, BulkIn);
	USBHwRegisterEPIntHandler(BULK_OUT_EP, BulkOut);

	DBG("Starting USB communication\n");

	// connect to bus
	USBHwConnect(TRUE);

	// call USB interrupt handler continuously
	while (1) {
		USBHwISR();
	}
	
	return 0;
}

