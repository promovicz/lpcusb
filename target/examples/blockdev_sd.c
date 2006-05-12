/*****************************************************************************\
*              efs - General purpose Embedded Filesystem library              *
*          --------------------- -----------------------------------          *
*                                                                             *
* Filename : sd.c                                                             *
* Revision : Initial developement                                             *
* Description : This file contains the functions needed to use efs for        *
*               accessing files on an SD-card.                                *
*                                                                             *
* This library is free software; you can redistribute it and/or               *
* modify it under the terms of the GNU Lesser General Public                  *
* License as published by the Free Software Foundation; either                *
* version 2.1 of the License, or (at your option) any later version.          *
*                                                                             *
* This library is distributed in the hope that it will be useful,             *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU           *
* Lesser General Public License for more details.                             *
*                                                                             *
*                                                    (c)2005 Michael De Nil   *
*                                                    (c)2005 Lennart Yseboodt *
\*****************************************************************************/

/*
	2006, Bertrik Sikken, modified for LPCUSB
*/


#include "type.h"
#include "usbdebug.h"


#include "blockdev.h"
#include "spi.h"



#define	CMDREADCSD       	9
#define CMDREADCID			10
#define	CMDREAD				17
#define	CMDWRITE			24
#define CMDWRITE_MULTIPLE	25


static void Command(U8 cmd, U16 paramx, U16 paramy)
{
	SPISend(0xff);

	SPISend(0x40 | cmd);
	SPISend((U8) (paramx >> 8));	/* MSB of parameter x */
	SPISend((U8) (paramx));	/* LSB of parameter x */
	SPISend((U8) (paramy >> 8));	/* MSB of parameter y */
	SPISend((U8) (paramy));	/* LSB of parameter y */

	SPISend(0x95);			/* Checksum (should be only valid for first command (0) */

	SPISend(0xff);			/* eat empty command - response */
}


/*****************************************************************************/

static U8 Resp8b(void)
{
	U8 i;
	U8 resp;

	/* Respone will come after 1 - 8 pings */
	for (i = 0; i < 8; i++) {
		resp = SPISend(0xff);
		if (resp != 0xff) {
			return resp;
		}
	}

	return resp;
}

/*****************************************************************************/

static void Resp8bError(U8 value)
{
	switch (value) {
	case 0x40:	DBG("Argument out of bounds.\n"); 				break;
	case 0x20:	DBG("Address out of bounds.\n");				break;
	case 0x10:	DBG("Error during erase sequence.\n"); 			break;
	case 0x08:	DBG("CRC failed.\n"); 							break;
	case 0x04:	DBG("Illegal command.\n"); 						break;
	case 0x02:	DBG("Erase reset (see SanDisk docs p5-13).\n");	break;
	case 0x01:	DBG("Card is initialising.\n"); 				break;
	default: 	
		DBG("Unknown error 0x%x (see SanDisk docs p5-13).\n", value); 
		break;
	}
}


/* ****************************************************************************
 calculates size of card from CSD 
 (extension by Martin Thomas, inspired by code from Holger Klabunde)
 */
int BlockDevGetSize(U32 *pdwDriveSize)
{
	U8 cardresp, i, by;
	U8 iob[16];
	U16 c_size, c_size_mult, read_bl_len;

	Command(CMDREADCSD, 0, 0);

	do {
		cardresp = Resp8b();
	} while (cardresp != 0xFE);

	DBG((("CSD:")));
	for (i = 0; i < 16; i++) {
		iob[i] = Resp8b();
		DBG(" %02x", iob[i]);
	}
	DBG((("\n")));

	SPISend(0xff);
	SPISend(0xff);

	c_size = iob[6] & 0x03;		// bits 1..0
	c_size <<= 10;
	c_size += (U16) iob[7] << 2;
	c_size += iob[8] >> 6;

	by = iob[5] & 0x0F;
	read_bl_len = 1 << by;

	by = iob[9] & 0x03;
	by <<= 1;
	by += iob[10] >> 7;

	c_size_mult = 1 << (2 + by);

	*pdwDriveSize = (U32) (c_size + 1) * (U32) c_size_mult *(U32) read_bl_len;

	return 0;
}


/*****************************************************************************/

static U16 Resp16b(void)
{
	U16 resp;

	resp = (Resp8b() << 8) & 0xff00;
	resp |= SPISend(0xff);

	return resp;
}


/*****************************************************************************/

static int State(void)
{
	U16 value;

	Command(13, 0, 0);
	value = Resp16b();

	switch (value) {
	case 0x0000: return 1;
	case 0x0001: DBG("Card is Locked.\n"); 													break;
	case 0x0002: DBG("WP Erase Skip, Lock/Unlock Cmd Failed.\n"); 							break;
	case 0x0004: DBG("General / Unknown error -- card broken?.\n"); 						break;
	case 0x0008: DBG("Internal card controller error.\n"); 									break;
	case 0x0010: DBG("Card internal ECC was applied, but failed to correct the data.\n");	break;
	case 0x0020: DBG("Write protect violation.\n"); 										break;
	case 0x0040: DBG("An invalid selection, sectors for erase.\n"); 						break;
	case 0x0080: DBG("Out of Range, CSD_Overwrite.\n"); 									break;
	default:
		if (value > 0x00FF) {
			Resp8bError((U8) (value >> 8));
		}
		else {
			DBG("Unknown error: 0x%x (see SanDisk docs p5-14).\n", value);
		}
		break;
	}
	return -1;
}

/*****************************************************************************/


int BlockDevInit(void)
{
	int i;
	U8 resp;

	SPIInit();				/* init at low speed */

	/* Try to send reset command up to 100 times */
	i = 100;
	do {
		Command(0, 0, 0);
		resp = Resp8b();
	} while (resp != 1 && i--);

	if (resp != 1) {
		if (resp == 0xff) {
			DBG("resp=0xff\n");
			return -1;
		}
		else {
			Resp8bError(resp);
			DBG("resp!=0xff\n");
			return -2;
		}
	}

	/* Wait till card is ready initialising (returns 0 on CMD1) */
	/* Try up to 32000 times. */
	i = 32000;
	do {
		Command(1, 0, 0);

		resp = Resp8b();
		if (resp != 0) {
			Resp8bError(resp);
		}
	} while (resp == 1 && i--);

	if (resp != 0) {
		Resp8bError(resp);
		return -3;
	}

	/* increase speed after init */
	SPISetSpeed(SPI_PRESCALE_MIN);

	if (State() < 0) {
		DBG((("Card didn't return the ready state, breaking up...\n")));
		return -2;
	}

	DBG((("Init done...\n")));

	return 0;
}

/*****************************************************************************/



/*****************************************************************************/


/*****************************************************************************/

/* ****************************************************************************
 * WAIT ?? -- FIXME
 * CMDWRITE
 * WAIT
 * CARD RESP
 * WAIT
 * DATA BLOCK OUT
 *      START BLOCK
 *      DATA
 *      CHKS (2B)
 * BUSY...
 */

int BlockDevWrite(U32 dwAddress, U8 * pbBuf)
{
	U32 place;
	U16 t = 0;

	place = 512 * dwAddress;
	Command(CMDWRITE, (U16) (place >> 16), (U16) place);

	Resp8b();				/* Card response */

	SPISend(0xfe);			/* Start block */
	SPISendN(pbBuf, 512);
	SPISend(0xff);			/* Checksum part 1 */
	SPISend(0xff);			/* Checksum part 2 */

	SPISend(0xff);

	while (SPISend(0xff) != 0xff) {
		t++;
	}

	return 0;
}

/*****************************************************************************/

/* ****************************************************************************
 * WAIT ?? -- FIXME
 * CMDCMD
 * WAIT
 * CARD RESP
 * WAIT
 * DATA BLOCK IN
 * 		START BLOCK
 * 		DATA
 * 		CHKS (2B)
 */

int BlockDevRead(U32 dwAddress, U8 * pbBuf)
{
	U8 cardresp;
	U8 firstblock;
	U16 fb_timeout = 0xffff;
	U32 place;

	place = 512 * dwAddress;
	Command(CMDREAD, (U16) (place >> 16), (U16) place);

	cardresp = Resp8b();		/* Card response */

	/* Wait for startblock */
	do {
		firstblock = Resp8b();
	} while (firstblock == 0xff && fb_timeout--);

	if (cardresp != 0x00 || firstblock != 0xfe) {
		Resp8bError(firstblock);
		return -1;
	}

	SPIRecvN(pbBuf, 512);

	/* Checksum (2 byte) - ignore for now */
	SPISend(0xff);
	SPISend(0xff);

	return 0;
}

/*****************************************************************************/
