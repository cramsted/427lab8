/*
 * bullets.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "bullets.h"
#include "render.h"		//for edit_frameBuffer
#include "aliens.h"		//for alien struct and functions
#include "events.h"		//for event setters
#include "bunkers.h"	//for bunker struct and functions
#include "ufo.h"		//for ufo struct and functions
#include "audio_files/audio.h"	//for the sound event setting and clearing functions
#include <stdlib.h>
#include <stdio.h>

//values that determine the dimensions of the sprites on the screen
#define BULLET_WIDTH (3 * 2)	//sprite width
#define BULLET_HEIGHT (5 * 2)	//sprite height
#define ALIEN_BULLETS_UPDATE_Y 6	//alien bullet speed
#define TANK_BULLET_UPDATE_Y -8	//tank bullet speed
#define BULLET_MIN_Y 45	//min y position of a bullet
#define BULLET_MAX_Y 435	//max y position of a bullet

//all the sprite structures defined in sprite_bit_maps.c
extern const int bulletCross_3x5[];
extern const int bulletLightning_3x5[];
extern const int alien_explosion_12x8[];

// Used for determining if which kind of bullet hit
// This is the return value for bulletCollidesWithSprite
typedef enum {
	no_hit = 0, tank_bullet_hit = 1, alien_bullet_hit = 2
} bullet_type_e;

//the bullets object
Bullets bullets;

// function prototypes - see descriptions in function definitions
static int bulletCollidesWithSprite(Bullet *bullet, Sprite *sprite,
		Position *spritePos);
static void checkTankBulletCollisions();
static int checkAlienBulletCollisions(Bullet *bullet);
static void eraseBullet(Bullet *bullet);
static void destroyBullet(Bullet *bullet);
static void drawBullet(Bullet *bullet, int updateY);
static void alienHit(Bullet *bullet);
static int bunkerHit(Bullet *bullet);
static int tankHit(Bullet *bullet);
static void ufoHit(Bullet *bullet);

//creates an initialized bullet struct
static Bullet initBullet(const int *sprite) {
	Bullet b;
	b.active = 0; //init to inactive
	b.sp = initSprite(BULLET_HEIGHT, BULLET_WIDTH, WHITE, sprite);
	return b;
}

//creates the containing bullet struct
Bullets initBullets() {
	Bullets b;
	int i;
	for (i = 0; i < MAX_BULLETS; i++) {
		if (i & 1) { //odd array index numbers have lightning sprites
			b.bullets[i] = initBullet(bulletLightning_3x5);
		} else { //even array index number have cross sprites
			b.bullets[i] = initBullet(bulletCross_3x5);
		}
	}
	return b;
}

// Update all tank and alien bullet positions
void updateBullets(Bullets *bullets) {
	int i;
	Bullet *b = bullets->bullets;

	// Move the tank bullet
	if (b[0].active) {
		eraseBullet(&b[0]);
		// Delete the bullet if it has left the screen
		if (b[0].p.y <= BULLET_MIN_Y) {
			b[0].active = 0;
			return;
		}
		drawBullet(&b[0], TANK_BULLET_UPDATE_Y);
		checkTankBulletCollisions();
	}

	// Move the alien bullets
	for (i = 1; i < MAX_BULLETS; i++) {
		Bullet *alienBullet = &b[i];
		if (alienBullet->active) {
			eraseBullet(alienBullet);
			// Delete the bullet if it has left the screen
			if (alienBullet->p.y >= BULLET_MAX_Y) {
				destroyBullet(alienBullet);
				aliens.numActiveBullets--;
			} else {
				drawBullet(alienBullet, ALIEN_BULLETS_UPDATE_Y);
				// If the bullet hits something, update the count of
				// active alien bullets so additional bullets can be fired.
				if (checkAlienBulletCollisions(alienBullet)) {
					aliens.numActiveBullets--;
				}
			}
		}
	}
}

// Initialize and fire the tank bullet
void tankPew(Tank *tank, Bullets *bullets) {
	Bullet *b = &bullets->bullets[0];
	if (b->active) { //allows for only one tank bullet at a time
		return;
	}
	//queues the tank fire noise to play
	setAudioEvent(AUDIO_TANK_FIRE_NOISE);
//	xil_printf("tank noise\n\r");
	//sets bullet position based on the current tank position
	b->p.x = tank->p.x + (tank->sp.width / 2) - (b->sp.width / 2);
	b->p.y = tank->p.y - b->sp.height - 1;
	b->active = 1;
	drawBullet(b, TANK_BULLET_UPDATE_Y);
}

// Initialize and fire an alien bullet
void alienPew(Aliens *aliens, Bullets *bullets) {

	//limits number of bullets aliens can have active at one time
	if (aliens->numActiveBullets >= MAX_ALIEN_BULLETS) {
		return;
	}

	// Pick a random alien column
	int alien_column = rand() % ALIENS_COL;

	// If the whole column is dead, don't shoot, just return.
	Alien *a = aliens->frontRowAliens[alien_column];
	if (a->status == dead) {
		return;
	}

	aliens->numActiveBullets++;

	// Find an inactive bullet
	Bullet *b = (void*) 0;
	Bullet *bulletList = bullets->bullets;
	int i;
	for (i = 1; i < MAX_BULLETS; i++) {
		if (!bulletList[i].active) {
			b = &bulletList[i];
			break;
		}
	}

	// Initialize the bullet position and draw
	b->p.x = a->p.x + (a->sp.width / 2) - (b->sp.width / 2);
	b->p.y = a->p.y + a->sp.height + 1;
	b->active = 1;

	drawBullet(b, ALIEN_BULLETS_UPDATE_Y);
}

// Erase the bullet from the screen
static void eraseBullet(Bullet *bullet) {
	bullet->sp.Color.color = BLACK;
	editFrameBuffer(&bullet->sp, &bullet->p);
}

// Erases, deactivates, and resets the bullet
static void destroyBullet(Bullet *bullet) {
	eraseBullet(bullet);
	bullet->active = 0; //deactivates the bullet;
	bullet->p.x = bullet->p.y = 0; //reset position
}

// Update the bullet's y position and draw it on the screen
static void drawBullet(Bullet *bullet, int updateY) {
	bullet->sp.Color.color = WHITE;
	bullet->p.y += updateY;
	editFrameBuffer(&bullet->sp, &bullet->p); //update frame buffer
}

// Figure out which section (row,col) of the bunker was hit
// by a bullet. This depends on where the bullet hit, which
// is given by bulletType: alien (bottom of the bullet hit),
// or tank (top of the bullet). Return the section in row,col.
static void computeBunkerSection(Bullet *bullet, bullet_type_e bulletType,
		Bunker *bunker, int *row, int *col) {

	// Temporary variables are easier to use.
	int bulletX = bullet->p.x + bullet->sp.width / 2; // middle of the bullet
	int bunkerX = bunker->p.x;
	int bunkerY = bunker->p.y;
	int bunkerSectionWidth = BUNKER_WIDTH / EROSION_COLS;
	int bunkerSectionHeight = BUNKER_HEIGHT / EROSION_ROWS;

	// Figure out the bunker section column with some simple math.
	int i;
	for (i = EROSION_COLS - 1; i >= 0; --i) {
		if (bulletX >= bunkerX + i * bunkerSectionWidth) {
			*col = i;
			break;
		}
	}

	// First, get the bulletY value.
	// This is dependent on whether
	// a tank bullet hit the bunker (top of the bullet hit) or
	// an alien bullet hit the bunker (bottom of the bullet hit).
	int bulletY;
	if (bulletType == tank_bullet_hit) {
		bulletY = bullet->p.y;
	} else if (bulletType == alien_bullet_hit) {
		bulletY = bullet->p.y + bullet->sp.height;
	} else {
		// If we get here, then we have a bug.
		bulletY = 0;
	}

	// Now we can figure out the bunker section row with simple math.
	for (i = EROSION_ROWS - 1; i >= 0; --i) {
		if (bulletY >= bunkerY + i * bunkerSectionHeight) {
			*row = i;
			break;
		}
	}

	return;
}

// Check if the bullet hit a bunker
// Return 1 if true, else 0
static int bunkerHit(Bullet *bullet) {
	int i;
	bullet_type_e bulletType;

	// Check each bunker.
	for (i = 0; i < MAX_BUNKERS; i++) {
		Bunker *bunker = &bunkers.bunkers[i];

		// Did the bullet hit this bunker?
		bulletType = bulletCollidesWithSprite(bullet, &bunker->sp, &bunker->p);
		if (bulletType != no_hit) {
			// It was a hit; compute the bunker section
			int row = 0, col = 0;
			computeBunkerSection(bullet, bulletType, bunker, &row, &col);

			// If the bullet collided with a part of the bunker that isn't
			// all the way eroded, continue to erode the bunker.
			// Otherwise, just let it pass through (no collision)
			if (bunker->erosionLevel[row][col] != gone) {
				erodeBunker(bunker, row, col);
				destroyBullet(bullet);
				return 1;
			} else {
				return 0;
			}
		}
	}
	return 0;
}

//checks if an aliens sprite has been hit
static void alienHit(Bullet *bullet) {
	int row, col;
	//iterates through the array of aliens
	for (row = 0; row < ALIENS_ROW; row++) {
		for (col = 0; col < ALIENS_COL; col++) {
			Alien *alien = &aliens.aliens[row][col];
			//checks if the bullet and alien sprites are colliding
			if (bulletCollidesWithSprite(bullet, &alien->sp, &alien->p)) {
				//kills the alien if it is alive, ignores it otherwise
				if (alien->status == alive) {
					destroyBullet(bullet);
					killAlien(alien, row, col);
				}
			}
		}
	}

}

// check if a bullet hit the tank
// return 1 if true, else 0
static int tankHit(Bullet *bullet) {
	//checks if the bullet and tank sprites are colliding
	if (bulletCollidesWithSprite(bullet, &tank.sp, &tank.p)) {
		setEvent(TANK_DEATH_EVENT);
		destroyBullet(bullet);
		return 1;
	}
	return 0;
}

//checks if the bullet and ufo sprites are colliding
static void ufoHit(Bullet *bullet) {
	if (bulletCollidesWithSprite(bullet, &ufo.sp, &ufo.p)) {
		if (ufo.sp.Color.color != BLACK) {
			//get rid of bullet before drawing the score otherwise it with draw
			//over the score
			destroyBullet(bullet);
			killUfo();
		}
	}
}

// check if a bullet hit a sprite
// return tank_bullet_hit or alien_bullet_hit if true, no_hit otherwise
static int bulletCollidesWithSprite(Bullet *bullet, Sprite *sprite,
		Position *spritePos) {
	if (!bullet->active) {
		return 0; //false because the bullet is not on the screen
	}
	int spriteX = spritePos->x;
	int spriteY = spritePos->y;
	int spriteXMax = spriteX + sprite->width;
	int spriteYMax = spriteY + sprite->height;
	int bulletX = bullet->p.x;
	int bulletY = bullet->p.y;
	int bulletXMax = bulletX + BULLET_WIDTH;
	int bulletYMax = bulletY + BULLET_HEIGHT;

	//checks for overlapping on the top left of the bullet sprite
	if ((bulletX >= spriteX) && (bulletX <= spriteXMax)) {
		if ((bulletY >= spriteY) && (bulletY <= spriteYMax)) {
			return tank_bullet_hit;
		}
	}
	//checks for overlapping on the bottom right of the bullet sprite
	if ((bulletXMax >= spriteX) && (bulletXMax <= spriteXMax)) {
		if ((bulletYMax >= spriteY) && (bulletYMax <= spriteYMax)) {
			return alien_bullet_hit;
		}
	}
	return no_hit;
}

static void checkTankBulletCollisions() {
	Bullet *tankBullet = &bullets.bullets[0];
	//check bunker
	bunkerHit(tankBullet);
	//check aliens
	alienHit(tankBullet);
	//check ufo
	ufoHit(tankBullet);
}

// check if an alien bullet hits anything
// return 1 if true, else 0
static int checkAlienBulletCollisions(Bullet *alienBullet) {
	//check if it hit the bunker
	if (bunkerHit(alienBullet)) {
		return 1;
	}
	//check if it hit the tank
	if (tankHit(alienBullet)) {
		return 1;
	}
	// it didn't hit anything
	return 0;
}
