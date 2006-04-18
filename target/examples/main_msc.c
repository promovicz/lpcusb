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


#define BAUD_RATE	115200

#define BULK_OUT_EP		0x05
#define BULK_IN_EP		0x82

#define MAX_PACKET_SIZE	64

#define LE_WORD(x)		(x&0xFF),(x>>8)


static U8 abBulkBuf[64];
static U8 abClassReqData[4];

static const U8 abDescriptors[] = {

// device descriptor	
	0x12,
	DESC_DEVICE,			
	LE_WORD(0x0110),		// bcdUSB
	0x00,					// bDeviceClass
	0x00,					// bDeviceSubClass
	0x00,					// bDeviceProtocol
	MAX_PACKET_SIZE0,		// bMaxPacketSize
	LE_WORD(0xFFFF),		// idVendor
	LE_WORD(0x0003),		// idProduct
	LE_WORD(0x0100),		// bcdDevice
	0x01,					// iManufacturer
	0x02,					// iProduct
	0x03,					// iSerialNumber
	0x01,					// bNumConfigurations

// configuration descriptor
	0x09,
	DESC_CONFIGURATION,
	LE_WORD(0x20),			// wTotalLength
	0x01,					// bNumInterfaces
	0x01,					// bConfigurationValue
	0x00,					// iConfiguration
	0xC0,					// bmAttributes
	0x32,					// bMaxPower

// interface
	0x09,
	DESC_INTERFACE,
	0x00,					// bInterfaceNumber
	0x00,					// bAlternateSetting
	0x02,					// bNumEndPoints
	0x08,					// bInterfaceClass
	0x05,					// bInterfaceSubClass = 8070i ATAPI-over-USB
	0x50,					// bInterfaceProtocol = BOT
	0x00,					// iInterface
// EP
	0x07,
	DESC_ENDPOINT,
	BULK_IN_EP,				// bEndpointAddress
	0x02,					// bmAttributes = bulk
	LE_WORD(0x40),			// wMaxPacketSize
	0x00,					// bInterval
// EP
	0x07,
	DESC_ENDPOINT,
	BULK_OUT_EP,			// bEndpointAddress
	0x02,					// bmAttributes = bulk
	LE_WORD(0x40),			// wMaxPacketSize
	0x00,					// bInterval

// string descriptors
	0x04,
	DESC_STRING,
	LE_WORD(0x0409),

	0x0E,
	DESC_STRING,
	'L', 0, 'P', 0, 'C', 0, 'U', 0, 'S', 0, 'B', 0,

	0x12,
	DESC_STRING,
	'P', 0, 'r', 0, 'o', 0, 'd', 0, 'u', 0, 'c', 0, 't', 0, 'X', 0,

	0x1A,
	DESC_STRING,
	'D', 0, 'E', 0, 'A', 0, 'D', 0, 'C', 0, '0', 0, 'D', 0, 'E', 0, 'C', 0, 'A', 0, 'F', 0, 'E', 0,

// terminating zero
	0
};


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
	if (pSetup->wIndex != 0) {
		DBG("Invalid idx %X\n", pSetup->wIndex);
		return FALSE;
	}
	if (pSetup->wValue != 0) {
		DBG("Invalid val %X\n", pSetup->wValue);
		return FALSE;
	}

	switch (pSetup->bRequest) {
	// get max LUN
	case 0xFE:
		*ppbData[0] = 0;		// No LUNs
		*piLen = 1;
		break;
	// MSC reset
	case 0xFF:
		// MSC_Bot_Init();
		break;
	default:
		DBG("Unhandled class\n");
		return FALSE;
	}
	return TRUE;
}


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

	// register class request handler
	USBRegisterRequestHandler(REQTYPE_TYPE_CLASS, HandleClassRequest, abClassReqData);
	
	// register endpoint handlers
	USBHwRegisterEPIntHandler(BULK_IN_EP, MAX_PACKET_SIZE, BulkIn);
	USBHwRegisterEPIntHandler(BULK_OUT_EP, MAX_PACKET_SIZE, BulkOut);

	DBG("Starting USB communication\n");

	// connect to bus
	USBHwConnect(TRUE);

	// call USB interrupt handler continuously
	while (1) {
		USBHwISR();
	}
	
	return 0;
}

