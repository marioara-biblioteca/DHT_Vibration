#include "vibration.h"

uint16_t vibrationCount = 0;

int readVibration(void)
{
	return vibrationCount;	
}

void printVibration(void)
{
		
	char buffer[6];
	int temp_vibration = readVibration();
	itoa(temp_vibration, buffer, 6);
	buffer[6]='\0';
	UART_print("V");
	UART_println(buffer);
	//vibrationCount = 0;
}