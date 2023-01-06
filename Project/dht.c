#include "dht.h"

#define TIMEOUT 10000 

uint8_t bits[5];  // buffer to receive data
double humidity;
double temperature;

// return values:
// DHTLIB_OK
// DHTLIB_ERROR_CHECKSUM
// DHTLIB_ERROR_TIMEOUT

int read11(uint8_t pin)
{
        // READ VALUES
        int rv = read(pin);
        if (rv != DHTLIB_OK)
				{
                humidity    = DHTLIB_INVALID_VALUE; // or is NaN prefered?
                temperature = DHTLIB_INVALID_VALUE; 
                return rv;
        }

        // CONVERT AND STORE
        humidity    = bits[0];  // bit[1] == 0;
				
				char result[20];
				itoa(humidity,result,10);

				result[19]='\0';
				UART_println(result);
				
        temperature = bits[2];  // bits[3] == 0;
				itoa(temperature,result,10);
				result[19]='\0';
				UART_println(result);
				
        // TEST CHECKSUM
        uint8_t sum = bits[0] + bits[2]; // bits[1] && bits[3] both 0
        if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;

        return DHTLIB_OK;
}

// return values:
// DHTLIB_OK
// DHTLIB_ERROR_TIMEOUT
int read(uint8_t pin)
{
        // INIT BUFFERVAR TO RECEIVE DATA
        uint8_t cnt = 7;
        uint8_t idx = 0;
				int i;

        // EMPTY BUFFER
        for (i=0; i< 5; i++) bits[i] = 0;

        // REQUEST SAMPLE
        pinModeA(pin, OUTPUT);
        digitalWriteA(pin, LOW);
        delay(18);
        digitalWriteA(pin, HIGH);
        delayMicroseconds(40);
				digitalWriteA(pin, LOW);
        pinModeA(pin, INPUT);

        // GET ACKNOWLEDGE or TIMEOUT
	//Once DHT detects the start signal, it will send out a low-voltage-level response signal, which lasts 80us.
        unsigned int loopCnt = TIMEOUT;
        while(digitalReadA(pin) == LOW)
                if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

        loopCnt = TIMEOUT;
				//Then the programme of DHT sets Data Single-bus voltage level from low to high and keeps it for 80us for DHT�s preparation for sending data
        while(digitalReadA(pin) == HIGH)
                if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;
				
				//delayMicroseconds(170);
        // READ THE OUTPUT - 40 BITS => 5 BYTES
        for (i=0; i<40; i++)
        {
					//When DHT is sending data to MCU, every bit of data begins with the 50us low-voltage-level and the length of the following high-voltage-level signal determines whether data bit is "0" or "1"
                loopCnt = TIMEOUT;
								int test = LOW;
                while(test == LOW){
									test = digitalReadA(pin);
									if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;
								}
                        					
                unsigned long t = micros();

                loopCnt = TIMEOUT;
								
                while(digitalReadA(pin) == HIGH){
										if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;
								}
								//70 microsecond voltage-length means data bit '1'
                if ((micros() - t) > 60){
									bits[idx] |= (1 << cnt);
									UART_print("1");
								}
								else{
									UART_print("0");
								}
                if (cnt == 0)   //trcem la urmatorul byte
                {
                        cnt = 7;   
                        idx++;      
                }
                else cnt--;
        }

        return DHTLIB_OK;
}

void printTemperature(void)
{
	char buffer[6];
	read11(DHT_PIN);
	int temperature_temp = (int)temperature;
	int humidity_temp = (int)humidity;
	itoa(temperature_temp, buffer, 6);
	
	buffer[6]='\0';
	UART_print("T");
	UART_println(buffer);
	itoa(humidity_temp, buffer, 6);
	buffer[6]='\0';
	UART_print("H");
	UART_println(buffer);
}
