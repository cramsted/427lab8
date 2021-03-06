/*
 * aliens.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#ifndef ALIENS_H_
#define ALIENS_H_

//for access to the Sprite and Position structs
#include "sprites.h"

//positional and size constraints on the aliens block
#define ALIENS_START_X 8	//starting x position of the alien block - use 175 to place near bunker
#define ALIENS_START_Y 75	//starting y position of the alien block - use 200 to place near bunker
#define ALIENS_COL 11		//number of aliens in a row
#define ALIENS_ROW 5		//number of aliens in a column
#define ALIENS_SHIFT_X 10	//number of pixels the aliens shift once in the x direction
#define ALIENS_SHIFT_Y 12	//number of pixels the aliens shift once in the y direction

//custom variable set for easily changing the guise of any type of alien sprites
typedef enum {
	top_alien_in = 0, // 0
	top_alien_out, // 1
	middle_alien_in, // 2
	middle_alien_out, // 3
	bottom_alien_in, // 4
	bottom_alien_out // 5
} alien_type_e;

//possible directions of movement for the alien block
typedef enum {
	left = -ALIENS_SHIFT_X,
	down = ALIENS_SHIFT_Y,
	right = ALIENS_SHIFT_X
} alien_direction_e;

//possible life statuses of alien structs
typedef enum {
	dead, alive, exploded
} alien_life_status_e;

//struct for all needed info to draw a single alien
typedef struct {
	Position p; //x,y position of the sprite
	Sprite sp; //sprite object
	alien_type_e type; //what sprite type the alien is and what guise it is in
	alien_life_status_e status; //alive or dead
} Alien;

//struct for all needed info to draw the block of aliens
typedef struct {
	Alien aliens[ALIENS_ROW][ALIENS_COL]; //an array (row x col) of all the aliens in the block
	Alien* frontRowAliens[ALIENS_COL]; //array keeping track of what aliens are on the front
	//row. Used for calculating position of bullets fired by aliens.
	uint8_t numActiveBullets; //number of active bullets fired by aliens
	uint8_t killCount;
	alien_direction_e direction; //what direction the block of aliens is currently traveling in
} Aliens;

extern Aliens aliens;

//function to initialize the alien block struct
//param x sets starting x position
//param y sets starting y position
void initAliens(int32_t x, int32_t y);
//moves the alien block in the direction of alien_direction_e by a preset number of pixels
void updateAliens();
//kills the alien at the specified row and col of the alien block
void killAlien(Alien *alien, int32_t row, int32_t col);
void drawAliens(int32_t xUpdate, int32_t yUpdate);
void eraseAlienExplosionSprite();

#endif /* ALIENS_H_ */
