/*
 * text.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "sprites.h"

//values that determine the dimensions of the sprites on the screen and max draw and coordinate values
#define MAX_SCORE_DIGITS 4	//max score DIGITS_START_X
#define MAX_CHARACTER_LETTERS 5 //longest string of chars that can be printed
#define GAME_START_X 250		//start x value for the game text
#define OVER_START_X 350		//start x value for the over text
#define GAME_OVER_Y 250			//start y value for the game over text
#define SCORE_START_X 20	//score starting x position
#define DIGITS_START_X 160 //digits starting x position
#define LIVES_START_X 350	//lives starting x position
#define TEXT_START_Y 25	//texy starting x position
#define CHARACTER_PADDING 4	//padding on sides of characters
//sprite dimensions
#define CHARACTER_HEIGHT (5 * 2)
#define CHARACTER_WIDTH (8 * 2)

//information needed to draw a character
typedef struct {
	Position p;
	Sprite sp;
} Character;

//information needed to draw the score
typedef struct {
	uint32_t score;
	//index 0 - thousandths
	//index 1 - hundredths
	//index 2 -tens
	//index 3 - ones
	Character sp[MAX_SCORE_DIGITS];
} Score;

//creates a character struct
Character initChar(int32_t x, int32_t y, const int32_t *sprite, uint32_t color);
//creates all the words at the top of the screen
void drawCharacters();
//draws the game over text
void drawGameOver();
//initializes and draws score
void initScore();
//updates the score by the passed in amount
void updateScore(uint32_t points);
//draws the point in the place of the ufo
void drawUfoPoints(Position *p, int32_t points, uint32_t color);

#endif /* TEXT_H_ */
