/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2021-06-21
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../tensortiger.h"

/* Private define ------------------------------------------------------------*/
#define PRESS_THRESHOLD 100
#define DRAW_SIZE 2

/* Private variables ------------------------------------------------------------*/
static int pressed = 0;
static int easter = 0;
int isEaster = 0;

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:  None
**
******************************************************************************/
void TIMER0_IRQHandler (void)
{
	getDisplayPoint(&display, Read_Ads7846(), &matrix );
	#ifdef SIMULATOR
	if(display.x <= MAX_X && display.x > 0 && display.y < MAX_Y){
	#elif
	if(display.x <= 240 && display.x > 0 && display.y != 0){
	#endif
		if(display.y >= 100 && display.y <= 240 && display.x >= 50 && display.x <= 190 && isEaster == 0){
			// draw with touch screen
			//TP_DrawPoint(display.x,display.y);
			TP_DrawBigPoint(display.x,display.y, DRAW_SIZE); 
			pressed = 0;
		}
		else if(display.y > 240 && display.y < 280 && display.x < 150 && display.x > 95 && isEaster == 0) {
				// easter egg button pressed (3 clicks)
				easter++;
				if(easter == 300) {
					easter = 0;
					pressed = 0;
					isEaster = 1;
					// create credits page
					easterPage();
				}
		}
		else if(display.y < 30 && display.x > 210 && isEaster == 1) {
				// exit button pressed
				pressed++;
				if(pressed == PRESS_THRESHOLD) {
					pressed = 0;
					isEaster = 0;
					// create start page when exit from credits page
					start();
				}
		}
		else if(display.y > 280 && display.x <= 80 && isEaster == 0) {
				// delete button pressed
				pressed++;
				if(pressed == PRESS_THRESHOLD) {
					pressed = 0;
					// delete last letter from output
					deleteLast();
				}
		}
		else if(display.y > 280 && display.x > 80 && display.x <= 160 && isEaster == 0) {
				// clear button pressed
				pressed++;
				if(pressed == PRESS_THRESHOLD) {
					pressed = 0;
					// clear input screen
					clearScreen();
				}
		}
		else if(display.y > 280 && display.x > 160 && isEaster == 0) {
				// confirm button pressed
				pressed++;
				if(pressed == PRESS_THRESHOLD) {
					pressed = 0;
					// start recognition
					recognition();

					clearScreen();
				}
		}
	}
	else{
		//do nothing if touch returns values out of bounds
	}
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:	None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
