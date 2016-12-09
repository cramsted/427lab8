/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "platform.h" //needed to setup the FPGA for drawing to a screen
#include "render.h"	//needed for videoInit()
#include "controls.h"	//needed for readInput()
#include "timer.h"
#include "events.h"
#include "dmacontroller.h"
#include "xparameters.h"
#include "audio_files/audio.h"

#define START_GAME_DELAY 30000000

void print(char *str);

int main()
{
//	uint32_t source_word = 0xDEADBEEF;
//	uint32_t destination_word = 0x0;
		uint32_t source_word[] = {1,2,3,4,5};
		uint32_t destination_word[] = {0,0,0,0,0};

	init_platform();

	print("Hello World\n\r");

	printf("Printing value before DMA transfer.\n\r");
//	xil_printf("%d\r\n", destination_word);
		int i;
		for(i = 0; i < 5; i++){
			xil_printf("%d\r\n", destination_word[i]);
		}

//	DMACONTROLLER_MasterRecvWord(0x7b000000, (Xuint32) &source_word);
//	DMACONTROLLER_MasterSendWord(0x7b000000, (Xuint32) &destination_word);

	//write to slave regs
	DMACONTROLLER_mWriteSlaveReg0(XPAR_DMACONTROLLER_0_BASEADDR, 0, &source_word);
	DMACONTROLLER_mWriteSlaveReg1(XPAR_DMACONTROLLER_0_BASEADDR, 0, &destination_word);
	DMACONTROLLER_mWriteSlaveReg2(XPAR_DMACONTROLLER_0_BASEADDR, 0, 5);
	disable_caches();
	//start the dma controller
//	cleanup_platform();
	xil_printf("before BE\n\r");
	Xil_Out16(XPAR_DMACONTROLLER_0_BASEADDR+DMACONTROLLER_MST_BE_REG_OFFSET, 0xFFFF);
	xil_printf("before start\n\r");
	Xil_Out8(XPAR_DMACONTROLLER_0_BASEADDR+DMACONTROLLER_MST_GO_PORT_OFFSET, MST_START);
	xil_printf("after start\n\r");

	printf("Printing value after DMA transfer.\n\r");
//	xil_printf("%d\r\n", destination_word);
		for(i = 0; i < 5; i++){
			xil_printf("%d\r\n", destination_word[i]);
		}

	//    cleanup_platform();
	enable_caches();
	return 0;
}

//int main() {
//    // seed random number generator
//    srand(time(NULL));
//    init_platform(); // Necessary for all programs.
//    initAudio(); // Initialize the AC97 audio controller
//    initVideoDMAController(); //sets up video hardware
//    videoInit(); //initializes the screen to its starting point
//
//    // short delay before things get going
//    volatile int delay = START_GAME_DELAY;
//    while (--delay)
//        ;
//    //initializes buttons, the FIT timer, and interrupts
//    // Do this AFTER the delay so there's time to draw the screen
//    timerInit();
//    //	while (1) {
//    //		readInput(); //waits for control input
//    //	}
//
//    // This spins in a while(1) loop, polling the events variable.
//    eventsLoop();
//
//    // Obligatory clean-up. The program will never actually get here.
//    cleanup_platform();
//
//    return 0;
//}
