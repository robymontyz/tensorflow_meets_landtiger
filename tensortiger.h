/****************************************Copyright (c)**************************************************                         
**
**                                        robymontyz
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			tensortiger.h
** Descriptions:	The main application functions
**------------------------------------------------------------------------------------------------------
** Created by:			Roberto Alessi - s290180
** Created date:		2021-06-21
** Version:				  1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef __TENSORTIGER_H
#define __TENSORTIGER_H

/* Includes ------------------------------------------------------------------*/
#include "lpc17xx.h"

/* Private define ------------------------------------------------------------*/
#define MAX_NUMS_LEN 28

#define NN_MAX_X 28
#define NN_MAX_Y 28
#define INPUT_MAX_X 140
#define INPUT_MAX_Y 140
//#define INPUT_MAX_X 28
//#define INPUT_MAX_Y 28

/* Private function prototypes -----------------------------------------------*/				
void start(void);
void writeNums(void);
void writeProb(char* prob);
void displaySampling(void);
void recognition(void);
void clearScreen(void);
void deleteLast(void);
void easterPage(void);

#endif /* end __TENSORTIGER_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
