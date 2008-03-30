/*
	2006, Bertrik Sikken
*/


#include "type.h"
#include "debug.h"

#include "sdcard.h"
#include "blockdev.h"

static U32 getsdbits(U8 *buf, int offset, int len)
{
	U32		mask, data;
	int		bytepos, bitpos;
	int		shift;
	
	offset = 127 - offset;
	bytepos = offset / 8;
	bitpos = offset % 8;
	mask = (1 << len) - 1;

	data = 0;
	for (shift = -(len + bitpos); shift < 0; shift += 8) {
		data = (data << 8) | buf[bytepos++];
	}
	return (data >> shift) & mask;
}


BOOL BlockDevInit(void)
{
	return SDInit();
}


BOOL BlockDevWrite(U32 dwBlock, U8* pbBuf)
{
	return SDWriteBlock(pbBuf, dwBlock);
}


BOOL BlockDevRead(U32 dwBlock, U8* pbBuf)
{
	return SDReadBlock(pbBuf, dwBlock);
}


BOOL BlockDevGetSize(U32 *pdwDriveSize)
{
	U8	abBuf[16];
	U32	c_size, num_blocks, block_size;
	U16	read_bl_len;
	U8	csd_structure, c_size_mult; 
	
	/* read CSD to determine block size and number of blocks */
	if (!SDReadCSD(abBuf)) {
		return FALSE;
	}
	csd_structure =	getsdbits(abBuf, 127, 2);
	if (csd_structure != 0) {
		DBG("Invalid CSD structure (%d)!\n", csd_structure);
		return FALSE;
	}
	read_bl_len =	getsdbits(abBuf, 83, 4);
	c_size =		getsdbits(abBuf, 73, 12);
	c_size_mult =	getsdbits(abBuf, 49, 3);

	num_blocks = (c_size + 1) << (c_size_mult + 2);
	block_size = 1 << read_bl_len;

	*pdwDriveSize = num_blocks * block_size;
	return TRUE;
}

