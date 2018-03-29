
#include <blackfin.h>
#include "Lab4_uTTCOSg2017_main.h"
#include <stdio.h>
#include <string>

#undef  EXECUTE_EVERY_SECOND
// #define EXECUTE_EVERY_SECOND 		uTTCOSg_GetTickFrequency( )
#define EXECUTE_EVERY_SECOND 		(25 * uTTCOSg_GetTickFrequency( )) // ADJUST ME

#undef  SECOND
#define SECOND 						(25 * uTTCOSg_GetTickFrequency( ))

#define SECONDS2		8
#define SECONDS4		25


bool Ready()
{
	if (*pSPI_STAT & 1 == 1)
		return true;

	return false;
}

void Write(unsigned short SPIvalue)
{
	*pSPI_TDBR = SPIvalue;
	ssync();
}

void Init_SPI()
{
	// Set slave select in SPI flag register (Set PF5 as an output)
	*pSPI_FLG = 0x0020;


	// Set Baud rate
	*pSPI_BAUD = 0x8000;
	ssync();

	//0101 0001 0000 0101
	*pSPI_CTL = 0x5101;
	ssync();
}

void Init_LCD()
{
	char LCDInit[] = {0x30, 0x30, 0x3C, 0x0F, 0x01,0x00};
	strcpy(Message, LCDInit);
	Next_char = 5;
	Perv_Char = 0;
	Sending_String = true;
	Instruction = true;
}

void SPI_Controller()
{
	if (Next_char == 0)
	{
		Sending_String = false;
		return;
	}

	enum SPIState {ToggleHigh, ToggleLow, ToggleHighAgain};

	if (Ready())
	{
		static SPIState currentState = ToggleHigh;
		SPIState nextState = currentState;
		unsigned short int toSend = (unsigned short int)Message[Perv_Char];

		if (Instruction == false)
		{
			toSend |= 0x0400;
		}

		switch (currentState)
		{
			case ToggleHigh:
				Write(0x0100 | toSend);
				nextState = ToggleLow;
			break;

			case ToggleLow:
				Write(toSend);
				nextState = ToggleHighAgain;
			break;

			case ToggleHighAgain:
				Write(0x0100 | toSend);
				nextState = ToggleHigh;
				Next_char--;
				Perv_Char++;
			break;
		}
		currentState = nextState;
	}


}

void message_select()
{
	enum MessageState {Cursor_to_Start, Message1, Message2};

	static MessageState currentState = Cursor_to_Start;
	static int countMessage = 1;
	char cursorHome[] = {0x01, 0x00};

	if (Sending_String)
		{
			return;
		}

	switch(currentState)
		{
			case Cursor_to_Start:
				Instruction = true;
				strcpy(Message, cursorHome);

				if (countMessage == 1)
					currentState = Message1;
				if (countMessage == 2)
					currentState = Message2;
			break;

			case Message1:
				Instruction = false;
				strcpy(Message, "Merry Christmas                         Aman <3 ");
				currentState = Cursor_to_Start;
				countMessage = 1;
			break;

			case Message2:
				Instruction = false;
				strcpy(Message, "Aman <3");
				currentState = Cursor_to_Start;
				countMessage = 1;
			break;
		}

	Sending_String = true;

	Next_char = strlen(Message);
	Perv_Char = 0;

}


