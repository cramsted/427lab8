/*
 * audio.h
 *
 *  Created on: Oct 26, 2016
 *      Author: superman
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <stdint.h>

// audio events
#define AUDIO_TANK_EXPLOSION    0x1  
#define AUDIO_TANK_FIRE_NOISE   0x2
#define AUDIO_UFO_NOISE         0x4
#define AUDIO_EXPLOSION_ALIEN   0x8
#define AUDIO_ALIEN_MOVEMENT    0x10
#define AUDIO_ALL_EVENTS (AUDIO_TANK_EXPLOSION | AUDIO_TANK_FIRE_NOISE | AUDIO_UFO_NOISE | AUDIO_EXPLOSION_ALIEN | AUDIO_ALIEN_MOVEMENT)

//clears all audio events
void clearAudio();
//puts the  highest priority audio event in the FIFO buffer
void playAudio();
//set an event
void setAudioEvent(uint32_t event);
//clear an event
void clearAudioEvent(uint32_t event);
//initializes the AC97
void initAudio();
//turns up the volume of the audio stream
void increaseVolume();
//turns down the volume of the audio stream
void decreaseVolume();

#endif /* AUDIO_H_ */
