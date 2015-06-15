/*
### Arduino Watering System ###
This is a simple controller to allow programmable watering of plants

*/

//libraries to include
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>  //To be included in future releases
#include "RTClib.h"

// Pin configuration
const byte pump1Pin = 15;
const byte pump1LedPin = 13;





void setup()
{
 //pinMode declaration
pinMode(pump1Pin, OUTPUT); 
}

void loop()
{
	
}

//Start watering function lasting specified minutes
void DoWatering(int minutes)
{
	unsigned long milliseconds = (minutes * 60000);
	unsigned long now_milliseconds = millis();
	while (now_milliseconds + milliseconds < millis())
		{
			//CONTINUARE
		}
	
	
}