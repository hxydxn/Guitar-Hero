// main.cpp
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10 in C++

// Last Modified: 1/17/2020 
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
#include <string>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "Random.h"
#include "PLL.h"
#include "SlidePot.h"
#include "Images.h"
#include "UART.h"
#include "Timer0.h"
#include "Timer1.h"
#include "Timer2.h"
#include "Sound.h"

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
extern "C" void SysTick_Handler(void);
extern "C" void GPIOE_Handler(void);

//// Creating a struct for the Sprite.
//typedef enum {dead,alive} status_t;
//struct sprite{
//  uint32_t x;      // x coordinate
//  uint32_t y;      // y coordinate
//  const unsigned short *image; // ptr->image
//  status_t life;            // dead/alive
//};          
//typedef struct sprite sprite_t;

//sprite_t bill={60,9,SmallEnemy20pointB,alive};

//uint32_t time = 0;
//volatile uint32_t flag;
//void background(void){
//  flag = 1; // semaphore
//  if(bill.life == alive){
//    bill.y++;
//  }
//  if(bill.y>155){
//    bill.life = dead;
//  }
//}
//void clock(void){
//  time++;
//}
//int main(void){
//  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
//  // TExaS_Init();
//  Random_Init(1);
//  Output_Init();
//  Timer0_Init(&background,1600000); // 50 Hz
//  Timer1_Init(&clock,80000000); // 1 Hz
//  EnableInterrupts();
//  ST7735_DrawBitmap(52, 159, PlayerShip0, 18,8); // player ship middle bottom
//  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
//  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
//  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
//  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
//  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);
//  ST7735_DrawBitmap(100, 9, SmallEnemy30pointB, 16,10);
//  while(bill.life == alive){
//    while(flag==0){};
//    flag = 0;
//    ST7735_DrawBitmap(bill.x,bill.y,bill.image,16,10);
//  }

//  ST7735_FillScreen(0x0000);            // set screen to black
//  ST7735_SetCursor(1, 1);
//  ST7735_OutString((char*)"GAME OVER");
//  ST7735_SetCursor(1, 2);
//  ST7735_SetTextColor(ST7735_WHITE);
//  ST7735_OutString((char*)"Nice try,");
//  ST7735_SetCursor(1, 3);
//  ST7735_OutString((char*)"Earthling!");
//  ST7735_SetCursor(2, 4);
//  ST7735_SetTextColor(ST7735_WHITE);
//  while(1){
//    while(flag==0){};
//    flag = 0;
//    ST7735_SetCursor(2, 4);
//    ST7735_OutUDec(time);
//  }

//}

SlidePot my(1625,196);
int diff; //0 -> Easy, 1 -> Hard
int lang; //0 -> English, 1 -> Espanol
int score = 0;

void TitleScreen(void){
	if(lang == 0){
		ST7735_FillScreen(0);
		ST7735_SetCursor(1,1);
		ST7735_SetTextColor(ST7735_WHITE);
		ST7735_OutString((char*) "GUITAR HERO");
		ST7735_SetCursor(1,3);
		ST7735_SetTextColor(ST7735_WHITE);
		ST7735_OutString((char*) "PRESS ANY BUTTON TO");
		ST7735_SetCursor(1,4);
		ST7735_OutString((char*) "CONTINUE");
		while((GPIO_PORTE_DATA_R&0x0F) == 0x00){}
		return;
	} else{
		ST7735_FillScreen(0);
		ST7735_SetCursor(1,1);
		ST7735_SetTextColor(ST7735_WHITE);
		ST7735_OutString((char*) "HÉROE DE");
		ST7735_SetCursor(1,2);
		ST7735_OutString((char*) "LA GUITARRA");
		ST7735_SetCursor(1,4);
		ST7735_SetTextColor(ST7735_WHITE);
		ST7735_OutString((char*) "PRESIONE CUALQUIER");
		ST7735_SetCursor(1,5);
		ST7735_OutString((char*) "BOTÓN");
		ST7735_SetCursor(1,6);
		ST7735_OutString((char*) "PARA CONTINUAR");
		while((GPIO_PORTE_DATA_R&0x0F) == 0x00){}
		return;
		}
}



void LangScreen(void){
	ST7735_FillScreen(0);
	ST7735_SetCursor(1,1);
	ST7735_SetTextColor(ST7735_GREEN);
	ST7735_OutString((char*) "PRESS GREEN FOR");
	ST7735_SetCursor(1,2);
	ST7735_OutString((char*) "ENGLISH");
	ST7735_SetCursor(1,4);
	ST7735_SetTextColor(ST7735_RED);
	ST7735_OutString((char*) "PRESIONE ROJO PARA");
	ST7735_SetCursor(1,5);
	ST7735_OutString((char*) "ESPAÑOL");
	while((GPIO_PORTE_DATA_R&0x03) == 0x00){}
	if((GPIO_PORTE_DATA_R&0x01) == 0x01){
		while((GPIO_PORTE_DATA_R&0x01) == 0x01){}
		lang = 0;
		ST7735_FillScreen(0);
	}
	else if((GPIO_PORTE_DATA_R&0x02) == 0x02){
		while((GPIO_PORTE_DATA_R&0x02) == 0x02){}
		lang = 1;
		ST7735_FillScreen(0);
	}
}
	

void DiffScreen(void){
	int data = 0;
	if(lang == 0){
		ST7735_FillScreen(0);
		ST7735_SetCursor(1,1);
		ST7735_SetTextColor(ST7735_GREEN);
		ST7735_OutString((char*) "MOVE THE SLIDER UP");
		ST7735_SetCursor(1,2);
		ST7735_OutString((char*) "FOR EASY MODE");
		ST7735_SetCursor(1,4);
		ST7735_SetTextColor(ST7735_RED);
		ST7735_OutString((char*) "MOVE THE SLIDER");
		ST7735_SetCursor(1,5);
		ST7735_OutString((char*) "DOWN FOR HARD MODE");
		ST7735_SetCursor(1,6);
		ST7735_OutString((char*) "(2x Score");
		ST7735_SetCursor(1,7);
		ST7735_OutString((char*) "(Multiplier)");
		ST7735_SetCursor(1,9);
		ST7735_SetTextColor(ST7735_WHITE);
		ST7735_OutString((char*) "PRESS ANY BUTTON TO");
		ST7735_SetCursor(1,10);
		ST7735_OutString((char*) "CONTINUE");
		while((GPIO_PORTE_DATA_R&0x0F) == 0x00){
			data = ADC_In();
		}
		if (data <= 2048){
			diff = 0;
			//ST7735_FillScreen(0);
			return;
		} diff = 1;
		//ST7735_FillScreen(0);
		return;
			
			
	} else{
		ST7735_FillScreen(0);
		ST7735_SetCursor(1,1);
		ST7735_SetTextColor(ST7735_GREEN);
		ST7735_OutString((char*) "MUEVA EL DESLIZADOR");
		ST7735_SetCursor(1,2);
		ST7735_OutString((char*) "HACIA ARRIBA PARA");
		ST7735_SetCursor(1,3);
		ST7735_OutString((char*) "EL MODO FÁCIL");
		ST7735_SetCursor(1,5);
		ST7735_SetTextColor(ST7735_RED);
		ST7735_OutString((char*) "MUEVA EL DESLIZADOR");
		ST7735_SetCursor(1,6);
		ST7735_OutString((char*) "HACIA ABAJO PARA");
		ST7735_SetCursor(1,7);
		ST7735_OutString((char*) "EL MODO DIFÍCIL");
		ST7735_SetCursor(1,8);
		ST7735_OutString((char*) "(PUNTUACIÓN");
		ST7735_SetCursor(1,9);
		ST7735_OutString((char*) "MULTIPLICADORA2X)");
		ST7735_SetCursor(1,11);
		ST7735_SetTextColor(ST7735_WHITE);
		ST7735_OutString((char*) "PRESIONE CUALQUIER");
		ST7735_SetCursor(1,12);
		ST7735_OutString((char*) "BOTÓN PARA");
		ST7735_SetCursor(1,13);
		ST7735_OutString((char*) "CONTINUAR");
		while((GPIO_PORTE_DATA_R&0x0F) == 0x00){
			data = ADC_In();
		}
		if (data <= 2048){
			diff = 0;
			//ST7735_FillScreen(0);
			//ST7735_DrawBitmap(4, 19, GreenPuck, 20, 20);
			return;
		} diff = 1;
		//ST7735_DrawBitmap(32, 19, RedPuck, 20, 20);
		//ST7735_FillScreen(0);
		return;
		}
}
	

class Note{
	public:
		int color;
		int y;
	
		Note(int c, int yval){
			color = c; //0 -> Green, 1 -> Red, 2 -> Yellow, 3 -> Blue
			y = yval;
		}
	
};

Note a(0, 19);
Note b(1, 19);
Note c(2, 19);
Note d(3, 19);
Note e(3, 19);
Note f(2, 19);
Note g(0, 19);
Note h(2, 19);
Note ii(1, 19);
Note j(2, 19);
Note k(0, 19);
Note l(2, 19);
Note m(2, 19);
Note n(3, 19);
Note o(3, 19);
Note p(3, 19);
Note q(1, 19);
Note r(3, 19);
Note s(2, 19);
Note t(0, 19);
Note u(0, 19);
Note v(2, 19);
Note w(0, 19);
Note x(1, 19);
Note y(2, 19);
Note z(1, 19);
Note aa(1, 19);
Note bb(2, 19);
Note cc(1, 19);
Note dd(0, 19);
Note ee(1, 19);
Note ff(2, 19);
Note gg(1, 19);
Note hh(0, 19);
Note iii(1, 19);
Note jj(2, 19);
Note kk(1, 19);
Note ll(0, 19);


Note array[] = {a, b, c, d, e, f, g, h, ii, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, iii, jj, kk, ll};

int time = 0;
void clock(){
	time++;
}


int NotesLeft = 30;
uint32_t i = 0;

void GameEngine(void){
	if(i < 30){
			if(array[i].color == 0){
					ST7735_DrawBitmap(4, array[i].y, Black, 20, 20);
					ST7735_DrawBitmap(4, array[i].y, GreenPuck, 20, 20);
					array[i].y = array[i].y + 6;
				//ST7735_DrawBitmap(0, 159, FinishedBack, 120, 160);
				//i++;
			}
			else if(array[i].color == 1){
					ST7735_DrawBitmap(36, array[i].y, Black, 20, 20);
					ST7735_DrawBitmap(36, array[i].y, RedPuck, 20, 20);
					array[i].y = array[i].y + 6;
				//ST7735_DrawBitmap(0, 159, FinishedBack, 120, 160);
				//i++;
			}
			else if(array[i].color == 2){
				//while(array[i].y != 160){
					ST7735_DrawBitmap(68, array[i].y, Black, 20, 20);
					ST7735_DrawBitmap(68, array[i].y, YellowPuck, 20, 20);
					array[i].y = array[i].y + 6;
				//}
//				ST7735_DrawBitmap(0, 159, FinishedBack, 120, 160);
//				i++;
			}
			else if(array[i].color == 3){
				//while(array[i].y != 160){
					ST7735_DrawBitmap(96, array[i].y, Black, 20, 20);
					ST7735_DrawBitmap(96, array[i].y, BluePuck, 20, 20);
					array[i].y = array[i].y + 6;
//				}
//				ST7735_DrawBitmap(0, 159, FinishedBack, 120, 160);
//				i++;
			}
			if(array[i].y > 160){
				ST7735_DrawBitmap(1, 160, FinishedBack, 120, 160);
				i++;
				NotesLeft--;
			}
		}
		return;
}

void GameEngine2(void){
	if(i < 30){
			if(array[i].color == 0){
					ST7735_DrawBitmap(4, array[i].y, Black, 20, 20);
					ST7735_DrawBitmap(4, array[i].y, GreenPuck, 20, 20);
					array[i].y = array[i].y + 8;
				//ST7735_DrawBitmap(0, 159, FinishedBack, 120, 160);
				//i++;
			}
			else if(array[i].color == 1){
					ST7735_DrawBitmap(36, array[i].y, Black, 20, 20);
					ST7735_DrawBitmap(36, array[i].y, RedPuck, 20, 20);
					array[i].y = array[i].y + 8;
				//ST7735_DrawBitmap(0, 159, FinishedBack, 120, 160);
				//i++;
			}
			else if(array[i].color == 2){
				//while(array[i].y != 160){
					ST7735_DrawBitmap(68, array[i].y, Black, 20, 20);
					ST7735_DrawBitmap(68, array[i].y, YellowPuck, 20, 20);
					array[i].y = array[i].y + 8;
				//}
//				ST7735_DrawBitmap(0, 159, FinishedBack, 120, 160);
//				i++;
			}
			else if(array[i].color == 3){
				//while(array[i].y != 160){
					ST7735_DrawBitmap(96, array[i].y, Black, 20, 20);
					ST7735_DrawBitmap(96, array[i].y, BluePuck, 20, 20);
					array[i].y = array[i].y + 8;
//				}
//				ST7735_DrawBitmap(0, 159, FinishedBack, 120, 160);
//				i++;
			}
			if(array[i].y > 160){
				ST7735_DrawBitmap(1, 160, FinishedBack, 120, 160);
				i++;
				NotesLeft--;
			}
		}
		return;
}

void ButtonInterupt(void){
	SYSCTL_RCGCGPIO_R |= 0x00000010; // (a) activate clock for port E
	
  score = 0;             // (b) initialize count and wait for clock
	
  GPIO_PORTE_DIR_R &= ~0x0F;    // (c) make PF4 in (built-in button)

  GPIO_PORTE_DEN_R |= 0x0F;     //     enable digital I/O on PE

  GPIO_PORTE_PUR_R |= 0x0F;     //     enable weak pull-up on PE

  GPIO_PORTE_IS_R &= ~0x0F;     // (d) PE is edge-sensitive

  GPIO_PORTE_IBE_R &= ~0x0F;    //     PE is not both edges

  GPIO_PORTE_IEV_R &= ~0x0F;    //     PE falling edge event

  GPIO_PORTE_ICR_R = 0x0F;      // (e) clear flag4

  GPIO_PORTE_IM_R |= 0x0F;      // (f) arm interrupt on PE

  NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00)|0x00000040; // (g) priority 3

  NVIC_EN0_R = 1<<4;      // (h) enable interrupt 30 in NVIC

  EnableInterrupts();           // (i) Enable global Interrupt flag (I)
}

void GPIOE_Handler(void){

	if(((GPIO_PORTE_DATA_R&0x01) != 0x00) && (array[i].color == 0) && ((array[i].y < 160) && (array[i].y > 100))){
		score++;
	}
	if(((GPIO_PORTE_DATA_R&0x02) != 0x00) && (array[i].color == 1) && ((array[i].y < 160) && (array[i].y > 100))){
		score++;
	}
	if(((GPIO_PORTE_DATA_R&0x04) != 0x00) && (array[i].color == 2) && ((array[i].y < 160) && (array[i].y > 100))){
		score++;
	}
	if(((GPIO_PORTE_DATA_R&0x08) != 0x00) && (array[i].color == 3) && ((array[i].y < 160) && (array[i].y > 100))){
		score++;
	}
  GPIO_PORTE_ICR_R = 0x0F;      // acknowledge flag4


}

void GameOver(void){
	if(lang == 0){
		ST7735_FillScreen(0x0000);            // set screen to black
		ST7735_SetCursor(1, 1);
		ST7735_OutString((char*)"GAME OVER");
		ST7735_SetCursor(1, 2);
		ST7735_SetTextColor(ST7735_WHITE);
		ST7735_OutString((char*)"NICE TRY,");
		ST7735_SetCursor(1, 4);
		ST7735_OutString((char*)"SCORE:");
		ST7735_OutUDec(score);
	}
	else{
		ST7735_FillScreen(0x0000);            // set screen to black
		ST7735_SetCursor(1, 1);
		ST7735_OutString((char*)"JUEGO");
		ST7735_SetCursor(1, 2);
		ST7735_OutString((char*)"TERMINADO");
		ST7735_SetCursor(1, 3);
		ST7735_SetTextColor(ST7735_WHITE);
		ST7735_OutString((char*)"BUEN INTENTO,");
		ST7735_SetCursor(1, 5);
		ST7735_OutString((char*)"PUNTAJE: ");
		ST7735_OutUDec(score);
	}
}

int main(void){
	EnableInterrupts();
	PLL_Init(Bus80MHz);
	Random_Init(1);
	Output_Init();
	Sound_Init();
	ADC_Init();
	ST7735_SetRotation(2);
	SYSCTL_RCGCGPIO_R |= 0x10;
	GPIO_PORTE_DIR_R &= 0x0F;
	GPIO_PORTE_DEN_R |= 0x0F;
	LangScreen();
	TitleScreen();
	DiffScreen();
	//ST7735_FillScreen(0);
	//ST7735_DrawBitmap(0, 159, FinishedBack, 120, 160);
	//ST7735_DrawBitmap(4, 19, GreenPuck, 20, 20);
//	ST7735_DrawBitmap(32, 19, RedPuck, 20, 20);
//	ST7735_DrawBitmap(64, 19, YellowPuck, 20, 20);
//	ST7735_DrawBitmap(92, 19, BluePuck, 20, 20);
	//ST7735_DrawBitmap(0, 19, GreenPuck, 20,20);
	//ST7735_DrawBitmap(19, 19, RedPuck, 20,20);
	//ST7735_DrawBitmap(0, 19, YellowPuck, 20,20);
	ST7735_DrawBitmap(1, 160, FinishedBack, 120, 160);
	ButtonInterupt();
	if(diff == 0){
		//ST7735_DrawBitmap(4, 19, GreenPuck, 20, 20);
		playsound(0, 0);
		Timer0_Init(&GameEngine, 3500000);
	}
	else{
		//ST7735_DrawBitmap(32, 19, RedPuck, 20, 20);
		playsound(0, 1);
		Timer0_Init(&GameEngine2, 3500000);
	}
	while(NotesLeft > 0){}
	if(diff == 1){
		score *= 2;
	}
	Timer0A_Stop();
	playsound(1, 0);
  GameOver();
	
	//ST7735_DrawBitmap(0, 159, FinishedBack, 120, 160);
	//Timer1_Init(&GameEngine, 15000000);
	while(1){
	};
	
	
}




