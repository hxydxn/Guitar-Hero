// SlidePot.cpp
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "SlidePot.h"
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(void){
	SYSCTL_RCGCADC_R |= 0x0001;   // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= 0x08;    // 2) activate clock for Port D
	__nop();
	__nop();
//  while((SYSCTL_PRGPIO_R&0x10) == 0);  // 3 for stabilization
  GPIO_PORTD_DIR_R &= ~0x04;    // 4) make PD2 input
  GPIO_PORTD_AFSEL_R |= 0x04;   // 5) enable alternate function on PD2
  GPIO_PORTD_DEN_R &= ~0x04;    // 6) disable digital I/O on PD2
  GPIO_PORTD_AMSEL_R |= 0x04;   // 7) enable analog functionality on PD2
	
  ADC0_PC_R &= ~0xF;
  ADC0_PC_R |= 0x1;             // 8) configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123;        // 9) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;      // 10) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;       // 11) seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;
  ADC0_SSMUX3_R += 5;           // 12) set channel
  ADC0_SSCTL3_R = 0x0006;       // 13) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;         // 14) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;       // 15) enable sample sequencer 3
	ADC0_SAC_R = 0x04;
}

//------------ADCIn------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void){
	uint32_t result;
	ADC0_PSSI_R = 0x0008;
	while((ADC0_RIS_R&0x08)==0){};
  result = ADC0_SSFIFO3_R&0xFFF;
	ADC0_ISC_R = 0x0008;
  return result; // remove this, replace with real code
}

// constructor, invoked on creation of class
// m and b are linear calibration coeffients 
SlidePot::SlidePot(uint32_t m, uint32_t b){
	slope = m;
	offset = b;
}

void SlidePot::Save(uint32_t n){
	data = n;
	distance = Convert(data);
	flag = 1;
}
uint32_t SlidePot::Convert(uint32_t n){
  return ((1625*n)/4096) + 196;
}

void SlidePot::Sync(void){
	while(!flag);
	flag = 0; 
}

uint32_t SlidePot::ADCsample(void){ // return ADC sample value (0 to 4095)

  return data;
}

uint32_t SlidePot::Distance(void){  // return distance value (0 to 2000), 0.001cm
// lab 8
  return distance;
}


