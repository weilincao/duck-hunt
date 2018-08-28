// main.cpp
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10 in C++

// Last Modified: 4/19/2018 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2017

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2017

 Copyright 2018 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "Random.h"
#include "PLL.h"
#include "SlidePot.h"
#include "Images.h"
#include "UART.h"
#include "Timer.h"
#include "Sound.h"
#include "DAC.h"
#include "music.h"
#include "object.h"
#include "TExaS.h"
#include "bottom.h"


SlidePot my(1500,0);
SlidePot my2(1500,0,60,95);

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
extern "C" void SysTick_Handler(void);


uint32_t time = 0;
	int index=0;
	int counter=0;

void clock(void){
  if(counter==3)
	{
		time++;
		counter=0;
	}
	else
	{
		
		counter=counter+1;
		
	}
	
	
	Sound_Play(Fur_Elise[index].sound);
	if(index<219)
	{
		index++;
	}
	else
	{
		index=0;
	}
	
}

void save()
{
	my.Save(ADC_In(),ADC2_In());
	
}

void save2()
{
	int savex=ADC1x_In();
	int savey=ADC1y_In();
	my.Save(ADC_In(),ADC2_In());
	my2.Save(savex,savey);
	duckx=my2.x;
	ducky=my2.y;
	if(duckstate!=GOTSHOT && duckstate!=DEAD )
	{
	
		if(savex>2500 && savey<1500)
		{
			duckstate=BL;
		}
		else if(savex<1500 && savey<1500)
		{
			duckstate=BR;
		}
		else if(savex<1500 && savey>2500)
		{
			duckstate=TR;
		}
		else if(savex>2500 && savey>2500)
		{
			duckstate=TL;
		}
		else if(savex<1500 && savey>1500 && savey<2500)
		{
			duckstate=R;
		}
		else if(savex>2500 && savey >1500 && savey<2500)
		{
			duckstate=L;
		}
		else if(savey<1500 && savex >1500 && savex<2500)
		{
			duckstate=BL;
		}
		else if(savey>2500 && savex >1500 && savex<2500)
		{
			duckstate=TL;
		}
	
	}
	if(duckstate==GOTSHOT)
	{

		duckstate=DEAD;
	}
	
	
}



int restart=1;


/////////////////////////////////////////////main/////////////////////////////////////////////////////
int main(void){

	//PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
	TExaS_Init();
  Random_Init(100);
  Output_Init();
	ADC_Init(); 
	Sound_Init();
	bottom_init();
	Timer1_Init(&clock,20000000); // 4 Hz
	Timer1A_Start();
	ST7735_DrawBitmap(0, 80, title, 128,72);
	ST7735_SetCursor(1, 9);
	ST7735_SetTextColor(ST7735_RED);
	ST7735_OutString((char*)"RED:vs AI");
	ST7735_SetCursor(1, 10);
	ST7735_OutString((char*)"YELLOW: vs");
	ST7735_SetCursor(1, 11);
	ST7735_OutString((char*)"     real duck boi");

	
	ST7735_SetCursor(1, 12);
	ST7735_OutString((char*)"EE319K Valvano");
	ST7735_SetCursor(1, 13);
	ST7735_OutString((char*)"Weilin Cao");
	ST7735_SetCursor(1, 14);
	ST7735_OutString((char*)"and Zareef");
	

	while( (GPIO_PORTE_DATA_R & 0x02)== 0x00 && (GPIO_PORTE_DATA_R & 0x01)== 0x00){}
	Timer1A_Stop();
	NVIC_ST_CTRL_R &= ~0x00000001;
	if( (GPIO_PORTE_DATA_R& 0x01) ==0x01)
	{
	beginning();
	
  Timer0_Init(&reflesh,1600000); // 50 Hz
  
	Timer2_Init(&save,500000); //as long as it is fast enough
	//Timer3_Init(&play_title,8600);
	Timer0A_Start();
	
	Timer2A_Start();
	//Timer3A_Start();
  EnableInterrupts();
	
	
	
	
	
		
	
	
	
	ST7735_DrawBitmap(0, 149, perfect_background_bmp, 128,150);
	display_zero_duck();
	



	
	
  while(mission==UNFINISHED){
		if((GPIO_PORTE_DATA_R& 0x01)== 0x01 )
		{
			is_pressed=true;
			
		}
		else if(is_pressed==true && (GPIO_PORTE_DATA_R& 0x01)== 0x00)
		{
				
				is_released=true;
				before_play();
				Timer3_Init(&play_shot,8600);			
				Timer3A_Start();
				check(duckx,ducky,duckw,duckh,my.x,my.y,8,8);
				is_pressed=false;
				
				
		}
		
		ST7735_DrawBitmap(my.x, my.y, breddot, 8,8);
		display_duck();
		display_bullet();
		if(duckstate==DEAD && ducky>100)
		{
				mission=SUCCESS;
		}

		
	}
	if(mission==SUCCESS)
	{
		Timer0A_Stop();
		pick_duck();
		ST7735_DrawBitmap(0, 149, perfect_background_bmp, 128,150);
		display_one_duck();
		mission=UNFINISHED;
		
		TIMER0_TAILR_R = 600000-1;
		bullet=3;
		duckx=20;
		ducky=95;
		duckstate=TL;
		Timer0A_Start();
		
		while(mission==UNFINISHED){
			if((GPIO_PORTE_DATA_R& 0x01)== 0x01 )
			{
				is_pressed=true;
			
			}
			else if(is_pressed==true && (GPIO_PORTE_DATA_R& 0x01)== 0x00)
			{
				
				is_released=true;
				before_play();
				Timer3_Init(&play_shot,8600);			
				Timer3A_Start();
				check(duckx,ducky,duckw,ducky,my.x,my.y,8,8);
				is_pressed=false;
				
				
			 }
		
			ST7735_DrawBitmap(my.x, my.y, breddot, 8,8);
			display_duck();
			display_bullet();
			if(duckstate==DEAD && ducky>100)
			{
				mission=SUCCESS;
			}

		
		}
		
	}

	if(mission==SUCCESS)
	{
		ST7735_DrawBitmap(0, 149, perfect_background_bmp, 128,150);
		display_two_duck();
		pick_duck();
		int timer=0;
		while(timer<90000)
		{timer++;}
		
		ST7735_FillScreen(0x0000);            // set screen to black
		ST7735_SetCursor(1, 5);
		ST7735_OutString((char*)"Finally");
		ST7735_SetCursor(1, 6);
		ST7735_OutString((char*)"someone");
		ST7735_SetCursor(1, 7);
		ST7735_OutString((char*)"make it!");
		
		

	}
	
	if(mission==FAIL)
	{
		Timer0A_Stop();
		duckstate=TL;
		while(duckx>-24&&ducky>-24)
		{
			duckx--;ducky--;
			display_duck();
			int timer=0;
			int ptime=200000;
			while(timer<ptime){timer++;} timer=0;
		}
		Timer3_Init(&play_laugh,8600);	
		Timer3A_Start();
		display_laugh();
		//ST7735_FillScreen(0x0000);            // set screen to black
		//ST7735_SetCursor(1, 1);
		//ST7735_OutString((char*)"GAME OVER");
		ST7735_SetCursor(1, 2);
		ST7735_SetTextColor(ST7735_WHITE);
		ST7735_SetCursor(1, 4);
		ST7735_OutString((char*)"Dude,");
		ST7735_SetCursor(1, 5);
		ST7735_OutString((char*)"Even a DOG");
		ST7735_SetCursor(1, 6);
		ST7735_OutString((char*)"laughs at you!");
		//ST7735_SetCursor(2, 7);
		//ST7735_SetTextColor(ST7735_WHITE);
	}
  }
	else if((GPIO_PORTE_DATA_R& 0x02) ==0x02)
	{
		ADC1xy_Init();
		beginning();
		ST7735_DrawBitmap(0, 149, perfect_background_bmp, 128,150);
		/*////////////////////////
		ST7735_SetCursor(1, 1);
		ST7735_OutString((char*)"oh");
		ST7735_SetCursor(1, 2);
		ST7735_OutString((char*)"so you want to be");
		ST7735_SetCursor(1, 3);
		ST7735_OutString((char*)"a duck man");
		*//////////////////////
		Timer2_Init(&save2,500000); //as long as it is fast enough
		
		
		my.y=duckx;
		my.x=ducky;
		Timer2A_Start();
		
		while(mission==UNFINISHED)
		{
			if((GPIO_PORTE_DATA_R& 0x01)== 0x01 )
			{
				is_pressed=true;
			
			}
			else if(is_pressed==true && (GPIO_PORTE_DATA_R& 0x01)== 0x00)
			{
				
				is_released=true;
				before_play();
				Timer3_Init(&play_shot,8600);			
				Timer3A_Start();
				check(duckx,ducky,duckw,duckh,my.x,my.y,8,8);
				is_pressed=false;
				
				
			}
		
			ST7735_DrawBitmap(my.x, my.y, breddot, 8,8);
			display_duck();
			display_bullet();
			display_zero_duck();
			if(duckstate==DEAD)
			{
				mission=SUCCESS;
			}	
			
		}
		if(mission==SUCCESS)
	{
		Timer2A_Stop();
		ST7735_DrawBitmap(0, 149, perfect_background_bmp, 128,150);
		pick_duck();
		int timer=0;
		while(timer<90000)
		{timer++;}
		
		ST7735_FillScreen(0x0000);            // set screen to black
		ST7735_SetCursor(1, 1);
		ST7735_OutString((char*)"WINNER!");	
		ST7735_SetCursor(1, 2);
		ST7735_OutString((char*)"WINNER!");
		ST7735_SetCursor(1, 3);
		ST7735_OutString((char*)"DUCK!");
		ST7735_SetCursor(1, 4);
		ST7735_OutString((char*)"(NO CHICKEN)");
		ST7735_SetCursor(1, 5);
		ST7735_OutString((char*)"DINNER!");
		

	}
	if(mission==FAIL)
	{
		Timer2A_Stop();
		duckstate=TL;
		while(duckx>-24&&ducky>-24)
		{
			duckx--;ducky--;
			display_duck();
			int timer=0;
			int ptime=200000;
			while(timer<ptime){timer++;} timer=0;
		}
		Timer3_Init(&play_laugh,8600);	
		Timer3A_Start();
		display_laugh();
		//ST7735_FillScreen(0x0000);            // set screen to black
		//ST7735_SetCursor(1, 1);
		//ST7735_OutString((char*)"GAME OVER");
		ST7735_SetCursor(1, 2);
		ST7735_SetTextColor(ST7735_WHITE);
		ST7735_SetCursor(1, 4);
		ST7735_OutString((char*)"Why does");
		ST7735_SetCursor(1, 5);
		ST7735_OutString((char*)"this ");
		ST7735_SetCursor(1, 6);
		ST7735_OutString((char*)"stupid gun");
		ST7735_SetCursor(1, 7);
		ST7735_OutString((char*)"only");
		ST7735_SetCursor(1, 8);
		ST7735_OutString((char*)"got 3 bullet?");
		ST7735_SetCursor(1, 9);
		ST7735_OutString((char*)":(");
		//ST7735_SetCursor(2, 7);
		//ST7735_SetTextColor(ST7735_WHITE);
	}
	}
	
	

}




