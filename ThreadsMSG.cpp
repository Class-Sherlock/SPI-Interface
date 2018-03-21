/*
 * Launch_GoldiLocks_Lab2Part1_Thread.cpp
 *
 *  Created on: Oct 28, 2016
 *      Author: bernard.kleiner
 */

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


char *temperature[] = {"00", "01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19",
						"20", "21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39",
						"40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50"};

void MSG0()
{
	if (commandStringBeingSent)
	{
		return;
	}

	enum MessageState {Cursor_to_Start, Message1, Message2, Message3, Message4};

	static MessageState currentState = Cursor_to_Start;
	static int countMessage = 1;
	static int tempIndex = 0;

	switch(currentState)
	{
		case Cursor_to_Start:
			char cursorHome[] = {0x01};
			commandStringIsInstruction = true;
			strcpy(SPI_MessageBuffer, cursorHome);

			if (countMessage == 1)
				currentState = Message1;
			if (countMessage == 2)
				currentState = Message2;
			if (countMessage == 3)
				currentState = Message3;
			if (countMessage == 4)
				currentState = Message4;

		break;

		case Message1:
			commandStringIsInstruction = false;
			strcpy(SPI_MessageBuffer, "So long, and ");
			currentState = Cursor_to_Start;
			countMessage = 2;
		break;

		case Message2:
			commandStringIsInstruction = false;
			strcpy(SPI_MessageBuffer, "thanks for all ");
			currentState = Cursor_to_Start;
			countMessage = 3;
		break;

		case Message3:
			commandStringIsInstruction = false;
			strcpy(SPI_MessageBuffer, "the fish");
			currentState = Cursor_to_Start;
			countMessage = 4;
		break;

		case Message4:
			commandStringIsInstruction = false;
			strcpy(SPI_MessageBuffer, "Temperature ");
			strcat(SPI_MessageBuffer, temperature[tempIndex]);
			currentState = Cursor_to_Start;
			countMessage = 1;

			tempIndex++;
			if (tempIndex >= 50)
			{
				tempIndex = 0;
			}
		break;

	}

	commandStringBeingSent = true;


	charToSend = strlen(SPI_MessageBuffer);
	charsSent = 0;
}

