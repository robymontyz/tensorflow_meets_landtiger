/****************************************Copyright (c)****************************************************
**                                      
**                                        robymontyz
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               tensortiger.c
** Descriptions:            The application functions
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Roberto Alessi - s290180
** Created date:            2021-06-21
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "lpc17xx.h"
#include "timer/timer.h"
#include "tensortiger.h"
#include "GLCD/GLCD.h"
#include "TouchPanel/TouchPanel.h"
#include "tensorflow/lite/micro/examples/hello_world/main_functions.h"
#include "tensorflow/lite/c/common.h"
#include "icons/polito.c"

/* Private variables ---------------------------------------------------------*/
uint8_t nums[MAX_NUMS_LEN] = "";
uint8_t nums_len = 0;

extern TfLiteTensor* input;
extern TfLiteTensor* output;

/*******************************************************************************
* Function Name  : start
* Description    : create the start page
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void start() {	
	LCD_Clear(White);
	GUI_Text(15, 5, (uint8_t *) "TensorFlow meets LandTiger", White, Black);
	
	// Draw black frame
	// vertical
	LCD_ColorArea(0,70,210,50,Black);
	LCD_ColorArea(190,70,210,50,Black);
	// horizontal
	LCD_ColorArea(50,70,30,140,Black);
	GUI_Text(45, 80, (uint8_t *) "Draw a number below", White, Black);
	LCD_ColorArea(50,240,40,140,Black);
	GUI_Text(95, 250, (uint8_t *) "-> ? <-", Yellow, Black);

	// Draw virtual buttons
	GUI_Text(15, 290, (uint8_t *) "Delete", Black, White);
	LCD_DrawLine(80, 280, 80, MAX_Y, Black);
	GUI_Text(100, 290, (uint8_t *) "Clear", Black, White);
	LCD_DrawLine(160, 280, 160, MAX_Y, Black);
	GUI_Text(170, 290, (uint8_t *) "Confirm", Black, White);
	
	// print numbers already recognized
	writeNums();
}

/*******************************************************************************
* Function Name  : writeNums
* Description    : Write recognized numbers on screen
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void writeNums() {
	LCD_ColorArea(10, 25, 15, MAX_X, White);
	GUI_Text(10, 25, (uint8_t *) nums, Black, White);
}

/*******************************************************************************
* Function Name  : writeProb
* Description    : Write confidence (probability) on last recognized number
*                  on screen
* Input          : - prob: probability
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void writeProb(char* prob) {
	LCD_ColorArea(190, 50, 15, MAX_X, White);
	GUI_Text(205, 50, (uint8_t *) prob, Black, White);
}

/*******************************************************************************
* Function Name  : displaySampling
* Description    : Sample screen from 140x140 matrix to 28x28 matrix into
*                  tensorflow input
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void displaySampling() {
	int x, y;
	
	// sampling screen (140x140 -> 28x28 matrix)
	// Set input data as int8 (-128 or 127) (quantization)
	for (y = 0; y < INPUT_MAX_Y; y++) {
		for (x = 0; x < INPUT_MAX_X; x++) {
			uint16_t color = LCD_GetPoint(50+x, 100+y);
			float x_scaled = (float)(x) / INPUT_MAX_X * NN_MAX_X;
			float y_scaled = (float)(y) / INPUT_MAX_Y * NN_MAX_Y;
 			if (color == Black) {
				input->data.int8[(int)(x_scaled) + (int)(y_scaled) * NN_MAX_Y] = 127;
			} else {
				input->data.int8[(int)(x_scaled) + (int)(y_scaled) * NN_MAX_Y] = -128;
			}
		}
	}
}

/*******************************************************************************
* Function Name  : recognition
* Description    : Run inference of NN and collect the output
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void recognition() {
	int i;
	int number = 7;
	int maxIndex = 0;
	float probability = 0;
	float maxScore = 0;
	char num2char[3] = "";
	char prob2char[5] = "";

	// get input matrix from display
	displaySampling();
	
	runInference();
	
	// get result
	for(i = 0; i < 10; i++) {
		// get output + dequantization from integer to floating-point
		float score = (output->data.int8[i] - output->params.zero_point) * output->params.scale;

		if (score > maxScore) {
			maxScore = score;
			maxIndex = i;
		}
	}
	number = maxIndex;
	probability = maxScore;
	
	if(nums_len < MAX_NUMS_LEN) {
		sprintf(num2char, "%d", number);
		nums[nums_len] = num2char[0];
		nums[nums_len+1] = 0;
		nums_len++;
		// output recognized numbers
		writeNums();

		sprintf(prob2char,"%1.2f", probability);
		writeProb(prob2char);
	}
}

/*******************************************************************************
* Function Name  : clearScreen
* Description    : Clear the screen of the input
* Input          : - speed: speed
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void clearScreen() {
	LCD_ColorArea(50, 100, INPUT_MAX_X, INPUT_MAX_Y, White);
}

/*******************************************************************************
* Function Name  : deleteLast
* Description    : Delete last recognized number
* Input          : - speed: speed
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void deleteLast() {
	if(nums_len > 0) {
		nums_len--;
		nums[nums_len] = 0;
	}

	writeNums();
}

/*******************************************************************************
* Function Name  : easterPage
* Description    : Show easter egg page with credits
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void easterPage() {
	LCD_Clear(White);
	
	GUI_Text(90, 10, (unsigned char*) "CREDITS", Black, White);
	GUI_Text(220, 5, (unsigned char*) "X", Red, White);
	
	/* initialize and show bitmap */
	LCD_Bitmap(60, 40, POLITO_WIDTH, POLITO_HEIGHT, (unsigned char*)&POLITO_pixel_data);
	
	GUI_Text(35, 180, (unsigned char*) "Computer Architecture", Black, White);
	GUI_Text(80, 195, (unsigned char*) "2020/2021", Black, White);
	GUI_Text(35, 215, (unsigned char*) "Made with <3 at PoliTo", Red, White);
	
	GUI_Text(10, 250, (unsigned char*) "Professor:    Bernardi P.", Black, White);
	GUI_Text(10, 265, (unsigned char*) "A. Professor: Angione F.", Black, White);
	GUI_Text(10, 280, (unsigned char*) "Student:      Alessi R.", Black, White);
}
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
