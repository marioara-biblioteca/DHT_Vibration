#include "MKL25Z4.h"
#include "gpio.h"

#ifndef dht_h
#define dht_h

#define DHT_LIB_VERSION "0.1.05"

#define DHTLIB_OK                0
#define DHTLIB_ERROR_CHECKSUM   -1
#define DHTLIB_ERROR_TIMEOUT    -2
#define DHTLIB_INVALID_VALUE    -999

extern uint8_t bits[5];  // buffer to receive data
extern double humidity;	// double to receive humidity value
extern double temperature; // double to receive temperature value


int read(uint8_t pin); // reads data from dht11/dht22 sensor
int read11(uint8_t pin); // reads data from dht11
void printTemperature(void);
#endif