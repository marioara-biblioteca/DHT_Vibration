#include "timer.h"

int timerCounter; // nuamra cate 2 microsecunde -> folosit in functiiile de delay
unsigned long microCounter; //numara microsecunde

void Timer_Init(void) {
	
	// Activarea semnalului de ceas pentru perifericul PIT
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	// Utilizarea semnalului de ceas pentru tabloul de timere
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;
	// Oprirea decrementarii valorilor numaratoarelor in modul debug
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	
	
	PIT->CHANNEL[0].LDVAL = 0x17; //2,3 MICROSECUNDE
  // Activarea întreruperilor pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	// Activarea timerului de pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	

	// Activarea întreruperii mascabile si setarea prioritatiis
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_SetPriority(PIT_IRQn,5);
	NVIC_EnableIRQ(PIT_IRQn);
}
void PIT_IRQHandler(void) {
	
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		
		timerCounter++;
		microCounter++;
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
	}
}

void delayMicroseconds(int x){
	
	timerCounter = 0;
	while(timerCounter < x){;}
}

//delay miliseconds
void delay(int x){
		delayMicroseconds(1000 * x);
}

unsigned long micros(){
	
	return microCounter;
}