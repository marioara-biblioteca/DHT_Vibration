#include "MKL25Z4.h"

#include "uart.h"

extern int timerCounter;

void Timer_Init(void); // Functie de initializarea a modulului periferic
void PIT_IRQHandler(void); // Functia de tratarea a intreruperilor pentru perifericul PIT

void delayMicroseconds(int x);
void delay(int x);
unsigned long micros(void);