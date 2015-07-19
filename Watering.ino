/*
### Arduino Watering System ###
This is a simple controller to allow programmable watering of plants

Commands available over serial/bluetooth (case-sensitive):
	# LEDTEST 				Makes the led blink 5 times (testing purposes)
	# PUMPTEST				Activates the pump for 2 seconds
	# SPEED X				Sets the pump speed to x (1-255)
	# START					Enables the pump
	# STOP					Disables the pump
	# SETTIMER N X Y D 	Sets timer N to hour X and minutes Y for a duration of D (in minutes)
	# TIMER N 1/0			Enables/Disables timer N watering
	# OVR 1/0				Enables/Disables Pump Override Stop
*/

#define MAX_TIMERS_NUMBER 2


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
int minsToWater,minsToTimedWater,pump1Speed;
bool pumpIsActive, pumpOvrStop, minsWatering, TimedWatering = 0;
unsigned long prevMillis, prevMillisTimedWatering;


//#ifdef DEBUG_MODE
#define DB_RX_PIN 8
#define DB_TX_PIN 9
//#endif

//SerialCommand object
SerialCommand SCmd;

struct WaterTimer {
int hour;
int minute;
int period;
bool set;
} WateringTimer[MAX_TIMERS_NUMBER];


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
  SCmd.addCommand("SETTIMER",SetTimer);	// Sets timer 1
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
		else 
		{
			pumpIsActive = 0;
			Serial.print("minsWatering time elapsed");				//REMOVE
		}
	}
	
	if (TimedWatering)
	{
		if (millis() < (prevMillisTimedWatering + (minsToTimedWater)*60000))
		{
			pumpIsActive = 1;
		}
		else 
		{
			pumpIsActive = 0;
			// Unsure.... minsWatering = 0;
			Serial.print("minsToTimedWater time elapsed");				//REMOVE
		}
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

// TODO: Add Duration setting
void SetTimer(){
	char *arg;
	int TimerNum;
	arg = SCmd.next();
	if (arg)
		{
		Serial.println("Received timer arg"); 				// REMOVE
		TimerNum = atoi(arg);
		char *arg1, *arg2;
		int TimerHour,TimerMinute;
		arg1 = SCmd.next();
		arg2 = SCmd.next();
		if (arg1 && arg2)
			{
			Serial.println("Received timer arg1 & arg2"); // REMOVE
			TimerHour = atoi(arg1);
			TimerMinute = atoi(arg2);
			WateringTimer[TimerNum].hour = TimerHour;
			WateringTimer[TimerNum].minute = TimerMinute;
			Serial.print("Setting timer to:");
			Serial.print("H:");
			Serial.print(WateringTimer[TimerNum].hour);
			Serial.print("M:");
			Serial.println(WateringTimer[TimerNum].minute);
			}
		char *arg3;
		int TimerPeriod;
		arg3 = SCmd.next();
		if (arg3)
			{
			Serial.println("Received timer arg3");			// REMOVE
			TimerPeriod = atoi(arg3);
			}
		else 
			{
			TimerPeriod = 1;
			Serial.println("Did not receive arg3"); 		// REMOVE
			}
		WateringTimer[TimerNum].period = TimerPeriod;
		Serial.print("Setting timer to:");					// REMOVE	
		Serial.print("H:");										// REMOVE
		Serial.print(WateringTimer[TimerNum].hour);		// REMOVE
		Serial.print("M:");										// REMOVE
		Serial.print(WateringTimer[TimerNum].minute);	// REMOVE
		Serial.print("For a duration of: ");				// REMOVE
		Serial.print(WateringTimer[TimerNum].period);	// REMOVE
		Serial.println(" minutes.");							// REMOVE
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

//TODO Complete function
void StartTimedWatering()
{
	//Add code to obtain now() from RTC
	for (int i=0; i <= MAX_TIMERS_NUMBER; i++)
	{
		//if (WateringTimer[i].hour == now.hour && WateringTimer[i].minute == now.minute && WateringTimer[i].set && !minsWatering)
		// Create function Timer() to enable timer. Or insert the part in SetTimer()
		{
			minsWatering = 0;
			minsToTimedWater = WateringTimer[i].period;
			TimedWatering = 1;
			prevMillisTimedWatering = millis();
		}
		//else TimedWatering = 0;
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
