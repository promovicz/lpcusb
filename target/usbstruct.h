/*
	(c) 2006, Bertrik Sikken, bertrik@sikken.nl

	definitions of structure of standard USB packets
*/

#ifndef _USBSTRUCT_H_
#define _USBSTRUCT_H_


#include "type.h"


/*
	setup packet definitions
*/
typedef struct {
	U8	bmRequestType;
	U8	bRequest;
	U16	wValue;
	U16	wIndex;
	U16	wLength;
} TSetupPacket;


#define REQTYPE_GET_DIR(x)		(((x)>>7)&0x01)
#define REQTYPE_GET_TYPE(x)		(((x)>>5)&0x03)
#define REQTYPE_GET_RECIP(x)	((x)&0x1F)

#define REQTYPE_DIR_TO_DEVICE	0
#define REQTYPE_DIR_TO_HOST		1

#define REQTYPE_TYPE_STANDARD	0
#define REQTYPE_TYPE_CLASS		1
#define REQTYPE_TYPE_VENDOR		2
#define REQTYPE_TYPE_RESERVED	3

#define REQTYPE_RECIP_DEVICE	0
#define REQTYPE_RECIP_INTERFACE	1
#define REQTYPE_RECIP_ENDPOINT	2
#define REQTYPE_RECIP_OTHER		3

/* standard requests */
#define	REQ_GET_STATUS			0x00
#define REQ_CLEAR_FEATURE		0x01
#define REQ_SET_FEATURE			0x03
#define REQ_SET_ADDRESS			0x05
#define REQ_GET_DESCRIPTOR		0x06
#define REQ_SET_DESCRIPTOR		0x07
#define REQ_GET_CONFIGURATION	0x08
#define REQ_SET_CONFIGURATION	0x09
#define REQ_GET_INTERFACE		0x0A
#define REQ_SET_INTERFACE		0x0B
#define REQ_SYNCH_FRAME			0x0C


/*
	USB descriptors
*/

typedef struct {
	U8	bLength;
	U8	bDescriptorType;
} TUSBDescHeader;

#define DESC_DEVICE				1
#define DESC_CONFIGURATION		2
#define DESC_STRING				3
#define DESC_INTERFACE			4
#define DESC_ENDPOINT			5
#define DESC_DEVICE_QUALIFIER	6
#define DESC_OTHER_SPEED		7
#define DESC_INTERFACE_POWER	8

#define GET_DESC_TYPE(x)		(((x)>>8)&0xFF)
#define GET_DESC_INDEX(x)		((x)&0xFF)

#endif /* _USBSTRUCT_H_ */

