#include "dht.h"
#include "vibration.h"

extern double humidity;
extern double temperature;
extern uint16_t vibrationCount;
extern uint8_t ledState;
extern uint8_t ledOverride;

/* Only works for vibration, since temp. & hum. DHT11 sensor
* doesn't pick up values properly for the moment
*/
void decideSensorValues()
{
	if (vibrationCount < 256)
		ledState = 2;
	else if (vibrationCount < 1024)
		ledState = 3;
	else 
		ledState = 1;
	vibrationCount = 0;
}

int main()
{
	UART_Init(9600);
	Timer_Init();
	Gpio_Init();
	
	UART_println("Start Program");
	UART_println("------------------");
	ledOverride = 0;
	delay(500);
	for(;;) {
		printTemperature();
		printVibration();
		decideSensorValues();
		showLedStatus();
		UART_println("------------------");
		delay(50);
	}
}
