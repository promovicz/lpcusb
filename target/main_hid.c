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


#define INTR_IN_EP		0x81

#define MAX_PACKET_SIZE	64

#define LE_WORD(x)		(x&0xFF),(x>>8)



// see the joystick example from the usb.org HID Descriptor Tool
static U8 abReportDesc[] = {
	0x05, 0x01,
	0x15, 0x00,
	0x09, 0x04,
	0xA1, 0x01,
	0x05, 0x02,
	0x09, 0xBB,
	0x15, 0x81,
	0x25, 0x7F,
	0x75, 0x08,
	0x95, 0x01,
	0x81, 0x02,
	0x05, 0x01,
	0x09, 0x01,
	0xA1, 0x00,
	0x09, 0x30,
	0x09, 0x31,
	0x95, 0x02,
	0x81, 0x02,
	0xC0,
	0x09, 0x39,
	0x15, 0x00,
	0x25, 0x03,
	0x35, 0x00,
	0x46, 0x0E, 0x01,
	0x65, 0x14,
	0x75, 0x04,
	0x95, 0x01,
	0x81, 0x02,
	0x05, 0x09,
	0x19, 0x01,
	0x29, 0x04,
	0x15, 0x00,
	0x25, 0x01,
	0x75, 0x01,
	0x95, 0x04,
	0x55, 0x00,
	0x65, 0x00,
	0x81, 0x02,
	0xC0
};

static const U8 abDescriptors[] = {

/* Device descriptor */
	0x12,              		
	DESC_DEVICE,       		
	LE_WORD(0x0110),		// bcdUSB	
	0x00,              		// bDeviceClass
	0x00,              		// bDeviceSubClass
	0x00,              		// bDeviceProtocol
	MAX_PACKET_SIZE0,  		// bMaxPacketSize
	LE_WORD(0x1234),		// idVendor
	LE_WORD(0x8888),		// idProduct
	LE_WORD(0x0100),		// bcdDevice
	0x01,              		// iManufacturer
	0x02,              		// iProduct
	0x03,              		// iSerialNumber
	0x01,              		// bNumConfigurations

// configuration
	0x09,
	DESC_CONFIGURATION,
	0x22,  					// wTotalLength
	0x00,					// bNumInterfaces
	0x01,  					// bConfigurationValue
	0x01,  					// iConfiguration
	0x00,  					// bmAttributes
	0xA0,  					// bMaxPower
	0x32,  

// interface
	0x09,   				
	DESC_INTERFACE, 
	0x00,  		 			// bInterfaceNumber
	0x00,   				// bAlternateSetting
	0x01,   				// bNumEndPoints
	0x03,   				// bInterfaceClass
	0x00,   				// bInterfaceSubClass
	0x00,   				// bInterfaceProtocol
	0x00,   				// iInterface

// HID descriptor

	0x09, 
	0x21, 					// bDescriptorType = HID
	LE_WORD(0x0110),		// bcdHID
	0x00,   				// bCountryCode
	0x01,   				// bNumDescriptors = report
	0x22,   				// bDescriptorType
	LE_WORD(sizeof(abReportDesc)),

	0x07,   		
	0x05,   		
	INTR_IN_EP,				// bEndpointAddress
	0x03,   				// bmAttributes = INT
	LE_WORD(0x40),   		// wMaxPacketSize
	10,						// bInterval   		

// string descriptors
	0x04,
	0x03,
	LE_WORD(0x0409),

	// manufacturer string
	0x0E,
	DESC_STRING,
	'L', 0, 'P', 0, 'C', 0, 'U', 0, 'S', 0, 'B', 0,

	// product string
	0x12,
	DESC_STRING,
	'P', 0, 'r', 0, 'o', 0, 'd', 0, 'u', 0, 'c', 0, 't', 0, 'X', 0,

	// serial number string
	0x12,
	DESC_STRING,
	'D', 0, 'E', 0, 'A', 0, 'D', 0, 'C', 0, 'O', 0, 'D', 0, 'E', 0,
	
	// terminator
	0
};



static void IntrIn(U8 bEP, U8 bEPStatus)
{
	DBG("IN %x\n", bEPStatus);
}


/*************************************************************************
	HandleClassRequest
	==================
		HID class request handler
		
**************************************************************************/
static BOOL HandleClassRequest(TSetupPacket *pSetup, int *piLen, U8 **ppbData)
{
	switch (pSetup->bRequest) {
	
	// set_idle:
	case 0x0A:
		break;

	default:
		DBG("Unhandled class\n");
		return FALSE;
	}
	return TRUE;
}


#define BAUD_RATE	115200


/*************************************************************************
	HIDHandleStdReq
	===============
		Standard request handler for HID devices.
		
	This function tries to service any HID specific requests.
	If the request turns out to be not HID specific, it is passed on to the
	'chapter 9' standard request handler.
		
**************************************************************************/
static BOOL HIDHandleStdReq(TSetupPacket *pSetup, int *piLen, U8 **ppbData)
{
	U8	bType, bIndex;

	if ((pSetup->bmRequestType == 0x81) &&			// standard IN request for interface
		(pSetup->bRequest == REQ_GET_DESCRIPTOR)) {	// get descriptor
		
		bType = GET_DESC_TYPE(pSetup->wValue);
		bIndex = GET_DESC_INDEX(pSetup->wValue);
		switch (bType) {

		case 0x21:
			// HID descriptor
			break;

		case 0x22:
			// report
			*ppbData = abReportDesc;
			*piLen = sizeof(abReportDesc);
			break;

		default:
			// unknown request
			return FALSE;
		}
		
		return TRUE;
	}
	else {
		// pass it on to the chapter 9 standard request handler
		return USBHandleStandardRequest(pSetup, piLen, ppbData);
	}
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
	
	// register device descriptors
	USBRegisterDescriptors(abDescriptors);

	// override standard request handler
	USBRegisterRequestHandler(REQTYPE_TYPE_STANDARD, HIDHandleStdReq);

	// register class request handler
	USBRegisterRequestHandler(REQTYPE_TYPE_CLASS, HandleClassRequest);

	// register endpoint handlers
	USBHwRegisterEPIntHandler(INTR_IN_EP, MAX_PACKET_SIZE, IntrIn);

	DBG("Starting USB communication\n");

	// connect to bus
	USBHwConnect(TRUE);

	// call USB interrupt handler continuously
	while (1) {
		USBHwISR();
	}
	
	return 0;
}

