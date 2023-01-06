#include "gpio.h"
uint8_t ledState = 0;
uint8_t ledOverride = 0;
void Gpio_Init(void) {
	
	// Activation of PORT A, PORT B and PORT D signals
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	
	/* DHT11 Temperature Sensor START*/
	// Setting the multiplex value on 1 to activate the port
	PORTA->PCR[DHT_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[DHT_PIN] |= PORT_PCR_MUX(1);
	
	
	GPIOA_PDDR |= (1 << DHT_PIN);
	GPIOA_PSOR |= (1 << DHT_PIN);
	/* DHT11 Temperature Sensor STOP*/
	
	/* Vibration Sensor START*/
	// Setting the multiplex value on 1 to activate the port
	PORTD->PCR[VIB_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[VIB_PIN] |= PORT_PCR_MUX(1);
	
	// Interrupt on falling edge
	PORTD->PCR[VIB_PIN] |= PORT_PCR_IRQC(0x0A) | PORT_PCR_PE_MASK;
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_SetPriority(PORTD_IRQn, 128);
	NVIC_EnableIRQ(PORTD_IRQn);
	/* Vibration Sensor STOP*/
	
	/* LED START */
	// --- RED LED ---
	
	// Utilizare GPIO ca varianta de multiplexare
	PORTB->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOB_PDDR |= (1<<RED_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 1 logic) - LED Stins pe HIGH
	GPIOB_PSOR |= (1<<RED_LED_PIN);
	
	// --- GREEN LED ---
	
	// Utilizare GPIO ca varianta de multiplexare
	PORTB->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOB_PDDR |= (1<<GREEN_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 1 logic) - LED Stins pe HIGH
	GPIOB_PSOR |= (1<<GREEN_LED_PIN);
	
	// --- BLUE LED ---
	
		// Utilizare GPIO ca varianta de multiplexare
	PORTD->PCR[BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOD_PDDR |= (1<<BLUE_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 1 logic) - LED Stins pe HIGH
	GPIOD_PSOR |= (1<<BLUE_LED_PIN);
	
	/* LED STOP */
}

int digitalReadA(int pin){
	
	delayMicroseconds(1);
	
	if(GPIOA_PDIR & (1 << pin))
		return 1;
	
	return 0;
}

void digitalWriteA(int pin, int value){

	
	if(value == LOW){
		GPIOA_PCOR |= (1 << pin);
	}
	else if (value == HIGH){
		GPIOA_PDOR |= 1 << pin;
	}
	
	//delayMicroseconds(1);
}

void pinModeA(int pin, int mode){
	if (mode == OUTPUT){
		GPIOA_PDDR |= 1 << pin;
	}
	else {
		GPIOA_PDDR &= ~(1 << pin);
	}
	 
}

void PORTD_IRQHandler() {
	vibrationCount++;
	PORTD_ISFR = (1<<VIB_PIN);
}
char* itoa(int value, char* result, int base) {

    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
void setLEDStatus(int LED_PIN, int status){
	switch (LED_PIN){
		case RED_LED_PIN:
			if (status == HIGH){
				GPIOB_PCOR |= (1<<RED_LED_PIN);
			}
			else{
				GPIOB_PDOR |= (1<<RED_LED_PIN);
			}
			break;
		case GREEN_LED_PIN:
			if (status == HIGH){
				GPIOB_PCOR |= (1<<GREEN_LED_PIN);
			}
			else{
				GPIOB_PDOR |= (1<<GREEN_LED_PIN);
			}
			break;
		case BLUE_LED_PIN:
			if (status == HIGH){
				GPIOD_PCOR |= (1<<BLUE_LED_PIN);
			}
			else{
				GPIOD_PDOR |= (1<<BLUE_LED_PIN);
			}
			break;
			default:
				break;
	}
}
void showLedStatus(void){
	switch (ledOverride)
	{
		case 0:
			break;
		case 1:
			setLEDStatus(RED_LED_PIN, HIGH);
			setLEDStatus(GREEN_LED_PIN, LOW);
			setLEDStatus(BLUE_LED_PIN, LOW);
			return;
		case 2:
			setLEDStatus(RED_LED_PIN, LOW);
			setLEDStatus(GREEN_LED_PIN, HIGH);
			setLEDStatus(BLUE_LED_PIN, LOW);
			return;
		case 3:
			setLEDStatus(RED_LED_PIN, LOW);
			setLEDStatus(GREEN_LED_PIN, LOW);
			setLEDStatus(BLUE_LED_PIN, HIGH);
			return;
		default:
			break;
	}
	switch (ledState)
	{
		case 0:
			break;
		case 1:
			setLEDStatus(RED_LED_PIN, HIGH);
			setLEDStatus(GREEN_LED_PIN, LOW);
			setLEDStatus(BLUE_LED_PIN, LOW);
			return;
		case 2:
			setLEDStatus(RED_LED_PIN, LOW);
			setLEDStatus(GREEN_LED_PIN, HIGH);
			setLEDStatus(BLUE_LED_PIN, LOW);
			return;
		case 3:
			setLEDStatus(RED_LED_PIN, LOW);
			setLEDStatus(GREEN_LED_PIN, LOW);
			setLEDStatus(BLUE_LED_PIN, HIGH);
			return;
		default:
			break;
	}
	return;
}
