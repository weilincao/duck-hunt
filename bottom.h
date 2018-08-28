#include "tm4c123gh6pm.h"
void bottom_init()
{
SYSCTL_RCGCGPIO_R |= 0x10;
volatile unsigned long delay;
delay = SYSCTL_RCGCGPIO_R;
	
GPIO_PORTE_DIR_R &= ~0x03;																			// PE0-3 inputs
GPIO_PORTE_AFSEL_R &= ~0x03;																				// disable alternate function
GPIO_PORTE_AMSEL_R &= ~0x03;	
GPIO_PORTE_DEN_R |= 0x03;

}
