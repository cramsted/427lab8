#include <stdint.h>
#include "alienMovement1.h"
// fastinvader1.wav 

static int32_t soundRate = 11025;
static int32_t soundFrames = 982;
static int32_t sound[] = { 122, 105, 88, 60, 43, 20, 15, 9, 20, 31, 48, 65, 94, 105,
		128, 144, 167, 184, 195, 218, 224, 235, 240, 246, 252, 255, 252, 252,
		252, 246, 246, 240, 235, 224, 218, 207, 201, 195, 184, 178, 167, 161,
		144, 139, 133, 122, 116, 105, 99, 88, 82, 71, 65, 54, 60, 65, 65, 77,
		82, 94, 94, 88, 94, 82, 71, 60, 48, 37, 26, 3, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 15, 26, 31, 48, 60, 71, 77,
		94, 111, 122, 128, 139, 150, 167, 178, 190, 201, 207, 218, 224, 235,
		240, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 235, 235, 229, 224,
		212, 212, 201, 195, 195, 184, 178, 167, 161, 156, 150, 150, 139, 133,
		128, 122, 116, 111, 105, 99, 94, 94, 82, 82, 77, 77, 65, 65, 60, 43,
		20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 26, 43, 60, 77, 82, 99,
		105, 116, 116, 128, 128, 128, 128, 128, 128, 122, 116, 111, 105, 99,
		105, 94, 88, 82, 77, 65, 60, 48, 43, 37, 31, 26, 20, 15, 9, 3, 3, 0, 0,
		0, 3, 9, 26, 37, 48, 48, 48, 48, 43, 37, 26, 15, 9, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 15, 26, 37, 48, 60, 71, 88,
		105, 116, 128, 139, 150, 167, 178, 195, 207, 218, 235, 246, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 252, 240, 240, 235, 224, 212, 207,
		195, 195, 184, 173, 167, 161, 156, 150, 139, 133, 133, 128, 128, 116,
		111, 105, 105, 94, 88, 71, 43, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 26,
		37, 54, 71, 82, 94, 105, 111, 116, 116, 122, 122, 122, 128, 122, 122,
		116, 116, 111, 111, 99, 88, 82, 77, 71, 65, 54, 43, 43, 31, 20, 15, 3,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 9, 15, 20, 15, 20, 9, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
		15, 26, 43, 54, 71, 88, 99, 111, 122, 133, 150, 161, 173, 190, 201,
		212, 224, 235, 246, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 246, 240, 235, 224, 218, 212, 207,
		201, 190, 184, 178, 173, 167, 156, 156, 150, 139, 128, 128, 128, 122,
		116, 111, 105, 99, 94, 94, 88, 65, 43, 15, 0, 0, 0, 0, 0, 0, 0, 0, 3,
		15, 31, 48, 65, 82, 94, 105, 116, 128, 133, 139, 144, 150, 150, 144,
		150, 144, 139, 139, 133, 128, 116, 111, 99, 94, 82, 82, 77, 60, 54, 48,
		43, 37, 26, 15, 15, 9, 3, 0, 0, 0, 0, 0, 3, 15, 26, 31, 43, 43, 37, 37,
		31, 20, 15, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 3, 15, 31, 43, 48, 65, 77, 94, 105, 122, 128, 144, 156, 167, 184,
		195, 207, 218, 229, 240, 252, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 252, 246, 235, 229, 224, 218,
		212, 207, 201, 195, 184, 178, 178, 173, 156, 150, 150, 139, 128, 128,
		128, 122, 116, 111, 99, 94, 88, 88, 82, 77, 71, 71, 65, 60, 54, 48, 43,
		48, 43, 37, 31, 26, 31, 26, 26, 26, 26, 20, 20, 15, 15, 9, 15, 9, 15,
		9, 15, 9, 15, 15, 15, 9, 15, 15, 15, 20, 26, 20, 26, 20, 26, 31, 26,
		31, 31, 31, 31, 37, 37, 43, 43, 43, 48, 54, 48, 54, 60, 54, 60, 60, 65,
		71, 65, 71, 77, 71, 77, 82, 82, 88, 94, 88, 94, 99, 99, 105, 99, 105,
		105, 111, 111, 116, 116, 122, 128, 128, 128, 128, 128, 128, 128, 133,
		128, 133, 139, 139, 133, 139, 139, 144, 150, 144, 150, 156, 150, 156,
		150, 156, 161, 156, 161, 156, 161, 161, 161, 161, 167, 161, 167, 173,
		167, 173, 173, 173, 167, 173, 173, 178, 173, 178, 173, 178, 173, 173,
		178, 173, 173, 178, 173, 173, 178, 173, 178, 173, 173, 178, 173, 178,
		173, 167, 167, 173, 167, 173, 167, 161, 161, 167, 161, 167, 161, 161,
		161, 167, 161, 156, 161, 161, 161, 156, 150, 156, 156, 156, 156, 150,
		156, 156, 156, 150, 144, 144, 150, 144, 150, 144, 150, 144, 139, 144,
		139, 144, 144, 139, 144, 139, 133, 139, 139, 139, 133, 139, 133, 133,
		133, 139, 133, 133, 128, 133, 128, 128, 128, 128, 128, 128, 128, 128,
		128, 116, 122, 116, 122, 116, 111, 116, 116, 111, 111, 116, 111, 116,
		111, 116, 111, 111, 116, 111, 116, 111, 111, 116, 111, 116, 111, 111,
		111 };

int32_t getAienMovement_1_SoundRate(){
	return soundRate;
}

int32_t getAienMovement_1_SoundFrames(){
	return soundFrames;
}

int32_t* getAienMovement_1_Sound(){
	return sound;
}