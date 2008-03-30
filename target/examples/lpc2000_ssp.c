/**
 * Driver for SSP port
 *
 * 2008, Bertrik Sikken
 */
 
#include "type.h"
#include "debug.h"

#include "lpc214x.h"
#include "hal.h"

#include "spi.h"

/*****************************************************************************/

#define SPI_SCK_PIN    17
#define SPI_MISO_PIN   18
#define SPI_MOSI_PIN   19
#define SPI_SS_PIN	   20

// SSPSR  Bit-Definitions
#define TNF     (1<<1)
#define RNE     (1<<2)
#define BSY		(1<<4)

#define IDLE_CHAR	0xFF

/*****************************************************************************/

static BOOL	fInit = FALSE;


/*****************************************************************************/

void SPITick(int iCount)
{
	int i;
	BYTE b;

	ASSERT(fInit);

	for (i = 0; i < iCount; i++) {
		// write idle char
		while (!(SSPSR & TNF));
		SSPDR = IDLE_CHAR;

		// read dummy incoming chars
		while (!(SSPSR & RNE));
		b = SSPDR;
	}

	// enable full control of SSEL
	SSPCR1 = 0;
	HalPinSelect(SPI_SS_PIN, 2);
	SSPCR1 = (1 << 1);
}


void SPISetSpeed(int iFrequency)
{
	int iClock, iDivider;

	ASSERT(fInit);

	// get base SSP clock
	iClock = HalSysGetPCLK() / SSPCPSR;
	// calculate divider (strictly round up)
	iDivider = (iClock + iFrequency - 1) / iFrequency;
	// set it
	SSPCR0 = ((iDivider - 1) << 8) | (SSPCR0 & 0x0F);
}


void SPITransfer(int iCount, U8 *pbTxData, U8 *pbRxData)
{
	int iRecv;
	int iXmit;
	U8 bData;

	ASSERT(fInit);

	iRecv = iCount;
	iXmit = iCount;
	while ((iXmit != 0) || (iRecv != 0)) {
		if ((SSPSR & TNF) && (iXmit > 0)) {
			bData = IDLE_CHAR;
			if (pbTxData != NULL) {
				bData = *pbTxData++;
			}
			SSPDR = bData;
			iXmit--;
		}
		if ((SSPSR & RNE) && (iRecv > 0)) {
			bData = SSPDR;
			if (pbRxData != NULL) {
				*pbRxData++ = bData;
			}
			iRecv--;
		}
	}
}


void SPIInit(void)
{
	// enable SPI1 power
	PCONP |= PCSPI1;

	// disable SPI1 during initialisation
	SSPCR1 = 0;

 	// set clock divider
	SSPCPSR = 2;

	SSPCR0 = (7 << 0) |		// 8 bit transfers
			 (0 << 4) |		// format = SPI
			 (0 << 6) |		// CPOL = 0
			 (0 << 7) |		// CPHA = 0
			 (0 << 8);		// SCR: divider - 1

	HalPinSelect(SPI_SCK_PIN,	2);	// SPI
	HalPinSelect(SPI_MISO_PIN,	2);	// SPI
	HalPinSelect(SPI_MOSI_PIN,	2);	// SPI
	HalPinSelect(SPI_SS_PIN,	0);	// GPIO until fully initialised

	// set select as high output
	IODIR0 |= (1 << SPI_SS_PIN);
	IOSET0 = (1 << SPI_SS_PIN);

	// enable SPI1
	SSPCR1 |= (1 << 1);		// SSP_SSE;

	fInit = TRUE;
}


