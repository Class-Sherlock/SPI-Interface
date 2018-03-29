
#include <blackfin.h>
#include "Lab4_2016.h"
#include <stdio.h>
#include <string>

#undef  EXECUTE_EVERY_SECOND
// #define EXECUTE_EVERY_SECOND 		uTTCOSg_GetTickFrequency( )
#define EXECUTE_EVERY_SECOND 		(25 * uTTCOSg_GetTickFrequency( )) // ADJUST ME

#undef  SECOND
#define SECOND 						(25 * uTTCOSg_GetTickFrequency( ))

#define SECONDS2		8
#define SECONDS4		25


bool SPI_Ready()
{
	if (*pSPI_STAT & 1 == 1)
		return true;

	return false;
}

void WriteSPI(unsigned short SPIvalue)
{
	*pSPI_TDBR = SPIvalue;
	ssync();
}

void Init_SPI()
{
	// Set slave select in SPI flag register (Set PF5 as an output)
	*pSPI_FLG = 0x0020;

	//0101 0001 0000 0101
	*pSPI_CTL = 0x5105;
	ssync();

	// Set Baud rate
	*pSPI_BAUD = 0x8000;
	ssync();
}

void Init_LCD()
{
	char LCDInit[] = {0x30, 0x30, 0x3C, 0x0F, 0x01};
	strcpy(SPI_MessageBuffer, LCDInit);
	charToSend = 5;
	charsSent = 0;
	commandStringBeingSent = true;
	commandStringIsInstruction = true;
}

void SPI_controller()
{
	if (charToSend == 0)
	{
		commandStringBeingSent = false;
		return;
	}

	enum SPIState {ToggleHigh, ToggleLow, ToggleHighAgain};

	if (SPI_Ready())
	{
		static SPIState currentState = ToggleHigh;
		SPIState nextState = currentState;
		unsigned short int toSend = (unsigned short int)SPI_MessageBuffer[charsSent];

		if (commandStringIsInstruction == false)
		{
			toSend |= 0x0400;
		}

		switch (currentState)
		{
			case ToggleHigh:
				WriteSPI(0x0100 | toSend);
				nextState = ToggleLow;
			break;

			case ToggleLow:
				WriteSPI(toSend);
				nextState = ToggleHighAgain;
			break;

			case ToggleHighAgain:
				WriteSPI(0x0100 | toSend);
				nextState = ToggleHigh;
				charToSend--;
				charsSent++;
			break;
		}
		currentState = nextState;
	}



}



