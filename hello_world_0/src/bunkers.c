/*
 * bunkers.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "bunkers.h"
#include "render.h"	//edit_frameBuffer
#include <string.h>	//memset
#include <stdio.h>	//for debugging purposes

//values that determine the dimensions of the sprites on the screen
#define XBUNKER_PADDING (80)	//padding in the x direction of the bunker
#define MAX_EROSION 4	//max erosion level

//all the sprite structures defined in sprite_bit_maps.c
extern const int bunker_24x18[];
extern const int bunkerDamage0_6x6[];
extern const int bunkerDamage1_6x6[];
extern const int bunkerDamage2_6x6[];
extern const int bunkerDamage3_6x6[];

//one off versions of each sprite for drawing over bunkers with
Sprite erosion_1;
Sprite erosion_2;
Sprite erosion_3;
Sprite erosion_4;

//used to change erosion level based on index using the erosion_e enum
Sprite* erosion_sprites[] = { NULL, &erosion_1, &erosion_2, &erosion_3, &erosion_4 };

Bunkers bunkers;

//function prototypes
void setErosionLevel(Bunker *bunker, erosion_e level);

//creates a bunker initialized with the passed in x,y
Bunker initBunker(int x, int y) {
	Bunker b;
	// Sets the erosion levels. Most are initially 0, but the middle two
	// sections are the bottom row are initialized to gone because
	// they are black sections of the bunker sprite anyway.
	setErosionLevel(&b, none);
	b.erosionLevel[EROSION_ROWS-1][1] = gone;
	b.erosionLevel[EROSION_ROWS-1][2] = gone;

	// Initialize the position
	b.p = initPosition(x, y);

	// Initialize the sprite
	b.sp = initSprite(BUNKER_HEIGHT, BUNKER_WIDTH, GREEN, bunker_24x18);
	return b;
}

//creates the bunkers block at the coordinate x,y
Bunkers initBunkers(int x, int y) {
	Bunkers b;
	int row;
	const int bunker_width = BUNKER_WIDTH + XBUNKER_PADDING;
	for (row = 0; row < MAX_BUNKERS; row++) { //populates bunkers array
		b.bunkers[row] = initBunker(BUNKER_START_X + (row * bunker_width), y);
	}
	// Initialize all the erosion sprites.
	erosion_1 = initSprite(EROSION_HEIGHT_AND_WIDTH, EROSION_HEIGHT_AND_WIDTH,
			GREEN, bunkerDamage0_6x6);
	erosion_2 = initSprite(EROSION_HEIGHT_AND_WIDTH, EROSION_HEIGHT_AND_WIDTH,
			GREEN, bunkerDamage1_6x6);
	erosion_3 = initSprite(EROSION_HEIGHT_AND_WIDTH, EROSION_HEIGHT_AND_WIDTH,
			GREEN, bunkerDamage2_6x6);
	erosion_4 = initSprite(EROSION_HEIGHT_AND_WIDTH, EROSION_HEIGHT_AND_WIDTH,
			GREEN, bunkerDamage3_6x6);
	return b;
}

//draws the bunkers block at the specified x,y
void drawBunkers(int x, int y) {
	int row;
	for (row = 0; row < MAX_BUNKERS; row++) {
		Bunker *temp = &bunkers.bunkers[row];
		editFrameBuffer(&temp->sp, &temp->p);
	}
}

//erodes a single seciton of a bunker based on the passed in row and col
// param bunker selects which bunker to erode
void erodeBunker(Bunker *b, int row, int col) {
	// row and col must be valid
	if (col < 0 || col > EROSION_COLS || row < 0 || row > EROSION_ROWS) {
		return;
	}

	// don't erode the empty bunker section
	if( row == 2){
		if(col == 1 || col == 2){
			return;
		}
	}

	b->erosionLevel[row][col]++;
	if (b->erosionLevel[row][col] > MAX_EROSION)
		return;

	// get position
	Position p;
	p.x = b->p.x + (EROSION_HEIGHT_AND_WIDTH * col);
	p.y = b->p.y + (EROSION_HEIGHT_AND_WIDTH * row);

	// erase and redraw
	Sprite *sp = erosion_sprites[b->erosionLevel[row][col]];
	sp->Color.color = BLACK;
	editFrameBuffer(sp, &p);
	sp->Color.color = GREEN;
	editFrameBuffer(sp, &p);
}

// Erase the bunker.
// Also set the erosion levels to gone because we don't want bullets
// colliding with and eroding the bunker after it is destroyed.
void destroyWholeBunker(Bunker *bunker) {
	bunker->sp.Color.color = BLACK;
	editFrameBuffer(&bunker->sp, &bunker->p);
	setErosionLevel(bunker, gone);
}


void setErosionLevel(Bunker *bunker, erosion_e level){
	int row, col;
	for(row = 0; row < EROSION_ROWS; row++){
		for(col = 0; col < EROSION_COLS; col++){
			bunker->erosionLevel[row][col] = level;
		}
	}
}

void eraseBunkerSection(Bunker *b, int row, int col) {
	// row and col must be valid
	if (col < 0 || col > EROSION_COLS || row < 0 || row > EROSION_ROWS) {
		return;
	}

	// don't erode the empty bunker section
	if( row == 2){
		if(col == 1 || col == 2){
			return;
		}
	}

	b->erosionLevel[row][col] = gone;

	// get position
	Position p;
	p.x = b->p.x + (EROSION_HEIGHT_AND_WIDTH * col);
	p.y = b->p.y + (EROSION_HEIGHT_AND_WIDTH * row);

	// erase and redraw
	Sprite *sp = erosion_sprites[b->erosionLevel[row][col]];
	sp->Color.color = BLACK;
	editFrameBuffer(sp, &p);
}
