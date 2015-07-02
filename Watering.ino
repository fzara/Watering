/*
### Arduino Watering System ###
This is a simple controller to allow programmable watering of plants

Commands available over serial/bluetooth (case-sensitive):
	# LEDTEST 	Makes the led blink 5 times (testing purposes)
	# START		Enables the pump
	# STOP		Disables the pump
	# OVR 1/0	Enables/Disables Pump Override Stop

*/

//libraries to include
#include <SerialCommand.h>
//#include <LiquidCrystal_I2C.h>  //To be included in future releases
//#include "RTClib.h" 	//To be included with Wire.h
//#include <Wire.h>		//To be included with RTClib.h
#include <SoftwareSerial.h>

#define DEBUG_MODE

#ifdef DEBUG_MODE
#endif

// Pin configuration
const byte pump1Pin = 13;
const byte pump1LedPin = 13;

// Var Declarations
int mins;
bool pumpIsActive, pumpOvrStop = 0;

//#ifdef DEBUG_MODE
#define DB_RX_PIN 8
#define DB_TX_PIN 9
//#endif

//SerialCommand object
SerialCommand SCmd;

void setup()
{
 //pinMode declaration
pinMode(pump1Pin, OUTPUT); 

//Serials configurations

//Serial.begin(115200); // Serial for ESP8266 Wi-Fi Module
Serial.begin(9800); //Serial for receiving commands, correct baud rate accordingly

// Setup callbacks for SerialCommand commands 
  SCmd.addCommand("LEDTEST",LED_test);   	// Tests the led with 5 rapid blinking
  SCmd.addCommand("OVR",OvrSet);		// Enables/disables security override 
  SCmd.addDefaultHandler(unrecognized);  	// Handler for command that isn't matched  (says "What?") 
  Serial.println("READY"); 

#ifdef DEBUG_MODE
SoftwareSerial Serialdb(DB_RX_PIN,DB_TX_PIN); // RX,TX - Serial for debugging purposes
Serialdb.begin(9800);
Serialdb.println("Debug Mode ON");
#endif
}

void loop()
{												
	SCmd.readSerial();     // Process Serial Commands
}

void LED_test()
{
  for (byte i=0; i<5; i++)
  {
	  digitalWrite(pump1LedPin, HIGH);
	  delay(100);
	  digitalWrite(pump1LedPin, LOW);
	  delay(100);
  }
}

void OvrSet()
{
	char *arg;
	int setting;
	arg = SCmd.next();
	if (arg)
	{
		setting = atoi(arg);
		if (setting == 1)
		{
			pumpOvrStop = 1;
		}
		else if (setting == 0)
		{
			pumpOvrStop = 0;
		}
	}
}

void unrecognized()
{
  Serial.println("What?"); 
}
