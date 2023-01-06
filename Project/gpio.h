#include "MKL25Z4.h"

#include "timer.h"

/* Arduino-type defines */
#define LOW 0
#define HIGH 1
#define INPUT 0
#define OUTPUT 1

/* PIN definition */
#define DHT_PIN (12) // PORT A Digital Temp Sensor
#define RED_LED_PIN (18) // PORT B built-in Red LED
#define GREEN_LED_PIN (19) // PORT B built-in Green LED
#define BLUE_LED_PIN (1) // PORT D built-in Blue LED
#define VIB_PIN (4) // PORT D Digital Vibration Sensor

extern int write;
extern char buffer[4];
extern uint16_t vibrationCount;

void Gpio_Init(void); // Initialises registers and sets them accordingly 
void PORTD_IRQHandler(void); // Used for vibration sensor interrupts
char* itoa(int value, char* result, int base); // Int to ASCII conversion
void showLedStatus(void); // LED status show
void setLEDStatus(int LED_PIN, int status); // Change built-in LEDs status easily
/* Used only for Arduino-type library of the DHT11 Temp Sensor Sensor */
int digitalReadA(int pin); // Read digital value of the pin on PORT A
void digitalWriteA(int pin, int value); // Write digital value to the pin of PORT A
void pinModeA(int pin, int mode); // Sets direction of the pin on PORT A

