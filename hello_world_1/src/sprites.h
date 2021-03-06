/*
 * sprites.h
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */

#ifndef SPRITES_H_
#define SPRITES_H_

#include <stdint.h> //used for uint..._t

//y coordinate of the ground
#define GROUND_START_Y 450

//colors
#define GREEN 	0x0000FF00
#define WHITE 	0x00FFFFFF
#define BLACK 	0x00000000
#define RED		0x00FF0000

//meta data for the sprites
typedef struct {
	int32_t height;
	int32_t width;
	union { //used to make a single 32 bit int32_t. Works well with our #defined colors
		struct {
			uint8_t red;
			uint8_t green;
			uint8_t blue;
			uint8_t padding;
		} RGB;
		uint32_t color;
	} Color;
	const int32_t *sprite; //points to the actual sprite struct
} Sprite;

//struct for sprite positional data
typedef struct {
	int32_t x;
	int32_t y;
} Position;

//initializes a sprite
Sprite initSprite(int32_t height, int32_t width, uint32_t color, const int32_t *sprite);
//initializes and x,y coordinate
Position initPosition(int32_t x, int32_t y);

#endif /* SPRITES_H_ */
