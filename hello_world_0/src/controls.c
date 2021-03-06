/*
 * controls.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "controls.h"
#include "aliens.h"
#include "bullets.h"
#include "bunkers.h"
#include "tank.h"
#include <stdio.h>

int isDigit(char c);
int getAlienNumber();

// Get input from the user
// The user enters in a number as a command, listed below.
void readInput() {
	char c;
	int num;
	c = getchar();

	if (!isDigit(c)) {
		xil_printf("Invalid digit\n\r");
		return;
	}

	// turn it into a digit
	c -= '0';

	switch (c) {
	case 2: // kill alien
		// Prompt the user to enter in an alien number
		// Valid indices are in the range [0,54]
		xil_printf("which alien number would you like to kill?\n\r");
		num = getAlienNumber();
		if (num >= 0 && num <= 54) {
			xil_printf("kill alien %d\n\r", num);
			// convert 0-54 to row/col
			int row = num / ALIENS_COL;
			int col = num % ALIENS_COL;
			killAlien(&aliens.aliens[row][col], row, col);
		} else {
			xil_printf("invalid input; enter a new command\n\r");
		}
		break;
	case 3: // fire random missile
		alienPew(&aliens, &bullets);
		break;
	case 4: // move tank left
		moveTankLeft(&tank);
		break;
	case 5: // fire tank bullet
		tankPew(&tank, &bullets);
		break;
	case 6: // move tank right
		moveTankRight(&tank);
		break;
	case 7: // erode bunker
		// Prompt the user for the bunker number
		// Valid indices in the range [0,3]
		// We can reuse the getAlienNumber function
		xil_printf("which bunker would you like to erode?\n\r");
		num = getAlienNumber();
		if (num >= 0 && num < MAX_BUNKERS) {
			destroyWholeBunker(&bunkers.bunkers[num]);
		}
		break;
	case 8: // update alien position
		updateAliens(&aliens);
		break;
	case 9: // update all bullets
		updateBullets(&bullets);
		break;
	default:
		return;
	}
}

// We use this to validate the user's input
int isDigit(char c) {
	if (c < '0' || c > '9') {
		return 0;
	}
	return 1;
}

// Reads input from the user
// This expects a one or two-digit number
// It will return -1 if the input is invalid
// Otherwise, it returns the number the user entered
int getAlienNumber() {
	char d10pos = getchar();
	char d1pos = getchar();
	if (!isDigit(d10pos))
		return -1;
	if (!isDigit(d1pos)) {
		return d10pos - '0';
	} else {
		d10pos -= '0';
		d1pos -= '0';
		return (d10pos * 10) + d1pos;
	}
}
