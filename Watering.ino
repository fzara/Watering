/*
### Arduino Watering System ###
This is a simple controller to allow programmable watering of plants

Commands available over serial/bluetooth (case-sensitive):
	# LEDTEST 	Makes the led blink 5 times (testing purposes)
	# PUMPTEST	Activates the pump for 2 seconds
	# SPEED X	Sets the pump speed to x (1-255)
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
const byte pump1Pin = 2; //Has to be a PWM capable pin
const byte pump1LedPin = 13;

// Var Declarations
int minsToWater,pump1Speed;
bool pumpIsActive, pumpOvrStop, minsWatering = 0;
unsigned long prevMillis;


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
pinMode(pump1LedPin, OUTPUT); 

//Stardard pump speed
pump1Speed = 255;

//Serials configurations

//Serial.begin(115200); // Serial for ESP8266 Wi-Fi Module
Serial.begin(9800); //Serial for receiving commands, correct baud rate accordingly

// Setup callbacks for SerialCommand commands 
  SCmd.addCommand("LEDTEST",LED_test);   	// Tests the led with 5 rapid blinking
  SCmd.addCommand("PUMPTEST",PUMP_test);  // Tests the pump, activating it for 2 seconds
  SCmd.addCommand("SPEED",SetPumpSpeed);	// Sets the pump speed (1-255)
  SCmd.addCommand("OVR",OvrSet);				// Enables/disables security override
  SCmd.addCommand("START",StartWatering);	// Starts watering for a given time, or indefinitely
  SCmd.addCommand("STOP",StopWatering);	// Stops watering
  SCmd.addDefaultHandler(unrecognized);  	// Handler for command that isn't matched  (says "INVALIDCOMMAND") 
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
	
	//Main Watering control
	
	if (!pumpOvrStop && pumpIsActive)
	{
		analogWrite(pump1Pin, pump1Speed);
		digitalWrite(pump1LedPin, HIGH);
	}
	else if (!pumpIsActive)
	{
		analogWrite(pump1Pin, 0);
		digitalWrite(pump1LedPin, LOW);
	}
	else if (pumpOvrStop) 
	{
		analogWrite(pump1Pin, 0);
		digitalWrite(pump1LedPin, LOW);
	}
	
	if (minsWatering)
	{
		if (millis() < (prevMillis + (minsToWater)*60000))
		{
			pumpIsActive = 1;
		}
		else pumpIsActive = 0;
	}
	
}

void LED_test()
{
  for (byte i=0; i<5; i++)
  {
	  digitalWrite(pump1LedPin, HIGH);
	  delay(200);
	  digitalWrite(pump1LedPin, LOW);
	  delay(200);
  }
}

void PUMP_test()
{
	  analogWrite(pump1Pin,255);
	  delay(2000);
	  analogWrite(pump1Pin,0);
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

void StartWatering()
{
	char *arg;
	arg = SCmd.next();
	if (arg != NULL)
	{
		minsToWater = atoi(arg);
		minsWatering = 1;
		prevMillis = millis();
		//Serial.print("minsWatering = true, Minutes to water: ");
		//Serial.println(minsToWater);
	}
	else 
	{
		minsWatering = 0;
	}
	pumpIsActive = 1;
}

void StopWatering()
{
	pumpIsActive = 0;
	minsWatering = 0;
}

void SetPumpSpeed()
{
	char *arg;
	arg = SCmd.next();
	if (arg != NULL)
	{
		pump1Speed = atoi(arg);
		
	}
	else 
	{
		pump1Speed = 255;
	}
}

void unrecognized()
{
  Serial.println("INVALIDCOMMAND"); 
}
