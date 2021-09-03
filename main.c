/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The main function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Roberto Alessi - s290180
** Modified date:           2021-06-21
** Version:                 v1.0
** Descriptions:            Tensorflow implementation on LPC1768, using MNIST for number recognition
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "timer/timer.h"
#include "TouchPanel/TouchPanel.h"
#include "GLCD/GLCD.h"
#include "tensortiger.h"
#include "tensorflow/lite/micro/examples/hello_world/main_functions.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

int main(void) {
  SystemInit();  												/* System Initialization (i.e., PLL)  */

  LCD_Initialization();

	TP_Init();
	TouchPanel_Calibrate();

	// create start page
	start();
	
	// Setup NN
	setup();
	
	//init_timer(0, 0x1312D0 ); 						/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	//init_timer(0, 0x6108 ); 						  /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */

	enable_timer(0);
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
