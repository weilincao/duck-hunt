// SlidePot.cpp
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0 and use a slide pot to measure distance
// Created: 3/28/2018 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "SlidePot.h"
#include "tm4c123gh6pm.h"


// ADC initialization function 
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(void){ 
//*** students write this ******
	volatile uint32_t delay; 
	SYSCTL_RCGCADC_R |= 0x00000001; // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; // 1) activate clock for Port E
  delay = SYSCTL_RCGCGPIO_R;      // 2) allow time for clock to stabilize
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTE_DIR_R &= ~0x30;      // 3) make PE4 PE5 input
  GPIO_PORTE_AFSEL_R |= 0x30;     // 4) enable alternate function on PE4 PE5
  GPIO_PORTE_DEN_R &= ~0x30;      // 5) disable digital I/O on PE4 PE5
                                  // 5a) configure PE4 as ?? (skip this line because PCTL is for digital only)
  GPIO_PORTE_PCTL_R = GPIO_PORTE_PCTL_R&0xFF00FFFF;
  GPIO_PORTE_AMSEL_R |= 0x30;     // 6) enable analog functionality on PE4 PE5
  ADC0_PC_R &= ~0xF;              // 8) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC0_SSPRI_R = 0x3210;          // 9) Sequencer 3 is lowest priority
  ADC0_ACTSS_R &= ~0x0004;        // 10) disable sample sequencer 2
  ADC0_EMUX_R &= ~0x0F00;         // 11) seq2 is software trigger
  ADC0_SSMUX2_R = 0x0089;         // 12) set channels for SS2
  ADC0_SSCTL2_R = 0x0060;         // 13) no TS0 D0 IE0 END0 TS1 D1, yes IE1 END1
  ADC0_IM_R &= ~0x0004;           // 14) disable SS2 interrupts
  ADC0_ACTSS_R |= 0x0004;         // 15) enable sample sequencer 2
	
	
}

void ADC1xy_Init(void){ 
//*** students write this ******
	volatile uint32_t delay; 
	SYSCTL_RCGCADC_R |= 0x00000002; // 1) activate ADC1
  SYSCTL_RCGCGPIO_R |= 0x08; //initialize port d
  delay = SYSCTL_RCGCGPIO_R;      // 2) allow time for clock to stabilize
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTD_DIR_R &= ~0x0C;      // 3) make PD3 PD2 input
  GPIO_PORTD_AFSEL_R |= 0x0C;     // 4) enable alternate function on PD2 PD3
  GPIO_PORTD_DEN_R &= ~0x0C;      // 5) disable digital I/O on PD2 PD3
  
  GPIO_PORTD_AMSEL_R |= 0x0C;     // 6) enable analog functionality on PD4 PD5
  ADC1_PC_R &= ~0xF;              // 8) clear max sample rate field
  ADC1_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC1_SSPRI_R = 0x3210;          // 9) Sequencer 3 is lowest priority
  ADC1_ACTSS_R &= ~0x0004;        // 10) disable sample sequencer 2
  ADC1_EMUX_R &= ~0x0F00;         // 11) seq2 is software trigger
  ADC1_SSMUX2_R = 0x0054;         // 12) set channels for SS2
  ADC1_SSCTL2_R = 0x0060;         // 13) no TS0 D0 IE0 END0 TS1 D1, yes IE1 END1
  ADC1_IM_R &= ~0x0004;           // 14) disable SS2 interrupts
  ADC1_ACTSS_R |= 0x0004;         // 15) enable sample sequencer 2
	
	
}
//------------ADCIn------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void){  
//*** students write this ******
		
		
	ADC0_PSSI_R = 0x0004;            // 1) initiate SS2
  while((ADC0_RIS_R&0x04)==0){};   // 2) wait for conversion done
  j1x = ADC0_SSFIFO2_R&0xFFF;  // 3A) read first result
  j1y = ADC0_SSFIFO2_R&0xFFF;  // 3B) read second result
  ADC0_ISC_R = 0x0004;             // 4) acknowledge completion
	
	return j1x;
}

uint32_t ADC2_In(void){  
//*** students write this ******
	
  return j1y;
}

uint32_t ADC1x_In(void)
{
	ADC1_PSSI_R = 0x0004;            // 1) initiate SS2
  while((ADC1_RIS_R&0x04)==0){};   // 2) wait for conversion done
  j2x = ADC1_SSFIFO2_R&0xFFF;  // 3A) read first result
  j2y = ADC1_SSFIFO2_R&0xFFF;  // 3B) read second result
  ADC1_ISC_R = 0x0004;             // 4) acknowledge completion
	
	return j2x;
}
uint32_t ADC1y_In(void)
{
	return j2y;
}


// constructor, invoked on creation of class
// m and b are linear calibration coeffients 
SlidePot::SlidePot(uint32_t m, uint32_t b){
//*** students write this ******
// initialize all private variables
	this->data=0;
	this->flag=0;
	this->distance=0;
// make slope equal to m and offset equal to b
	this->slope=m;
	this->offset=b;
	this->x=50;
	this->y=50;
}

SlidePot::SlidePot(uint32_t m, uint32_t b, uint32_t ox, uint32_t oy){
//*** students write this ******
// initialize all private variables
	this->data=0;
	this->flag=0;
	this->distance=0;
// make slope equal to m and offset equal to b
	this->slope=m;
	this->offset=b;
	this->x=ox;
	this->y=oy;
}


void SlidePot::Save(uint32_t n, uint32_t n2){
	
//*** students write this ******
	
// 1) save ADC sample into private variable
	this->data=n;
// 2) calculate distance from ADC, save into private variable
	this->distance=Convert(n);
	
	if(n>2500 && x>0)
	{
		x--;
	}
	else if(n<1500 && x<120)
	{
		x++;
	}
	if(n2>2500 && y>8)
	{
		y--;
	}
	else if(n2<1500 && y<90)
	{
		y++;
	}
	
	
	//this->x=Convert(n)/12;
	//this->y=Convert(n2)/19+10;
// 3) set semaphore flag = 1
	this->flag=1;
}


uint32_t SlidePot::Convert(uint32_t n){
  //*** students write this 
	
	return this->slope*n/4096+this->offset;
	
	/*/////////////////
	if(n<10)
		return 0;
	else if (n<500)
		return (2500*n)/4096+100;
	else if(n<1200)
  return (2500*n)/4096;
	else if(n>=1200&&n<2000)
		return (2100*n)/4096;
	else
		return (2000*n)/4096;
	*//////////////////////////
  // use calibration data to convert ADC sample to distance
 // replace this with solution
}

void SlidePot::Sync(void){
// 1) wait for semaphore flag to be nonzero
	while(this->flag==0){}
		
// 2) set semaphore flag to 0
		this->flag=0;
}



