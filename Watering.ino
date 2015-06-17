/*
### Arduino Watering System ###
This is a simple controller to allow programmable watering of plants

*/

//libraries to include
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>  //To be included in future releases
#include "RTClib.h"
#include <SoftwareSerial.h>

#define DEBUG_MODE

#ifdef DEBUG_MODE
#endif

// Pin configuration
const byte pump1Pin = 13;
const byte pump1LedPin = 13;

// Var Declarations
String inBuffer;
int mins;
String command;
bool pumpIsActive, pumpOvrStop = 0;
int num2, num3;

//#ifdef DEBUG_MODE
#define DB_RX_PIN 8
#define DB_TX_PIN 9
//#endif

void setup()
{
 //pinMode declaration
pinMode(pump1Pin, OUTPUT); 


//Serials configurations

//Serial.begin(115200); // Serial for ESP8266 Wi-Fi Module
Serial.begin(9800); //Serial for HC-06 Bluetooth module


#ifdef DEBUG_MODE
SoftwareSerial Serialdb(DB_RX_PIN,DB_TX_PIN); // RX,TX - Serial for debugging purposes
Serialdb.begin(9800);
Serialdb.println("Debug Mode ON");
#endif
}

void loop()
{
	
	if (pumpIsActive & !pumpOvrStop){
		digitalWrite(pump1Pin, HIGH);
		digitalWrite(pump1LedPin, HIGH);
	}
	else {
		digitalWrite(pump1Pin, LOW);
		digitalWrite(pump1LedPin, LOW);
	}
	
	
	

	
	if (Serial.available())
	{
		char c = Serial.read();
		if (c == '\n')
		{
			parseCommand(inBuffer);
			inBuffer = "";
		}
		else
		{
			inBuffer += c;
		}
		
	}
	
}

void parseCommand(String com)
{
	String part1,part2,part3;
	int nextSpace, lastSpace;
	part1 = com.substring(0, com.indexOf(" "));
	lastSpace = com.indexOf(" ");
	nextSpace = com.indexOf(' ',lastSpace+1);
	part2 = com.substring(lastSpace +1,nextSpace);
	nextSpace = com.indexOf(' ',lastSpace+1);
	lastSpace = nextSpace;
	part3 = com.substring(lastSpace+1);
	/*
		Serial.print("Part1: \t"); 												//TESTING
		Serial.println(part1);														//TESTING
		Serial.print("Part2: \t");													//TESTING
		Serial.println(part2);														//TESTING
		Serial.print("Part3: \t");													//TESTING
		Serial.println(part3);														//TESTING
	*/
		char part[10];					//convert to char array to elaborate command.
		part1.toCharArray(part,10);
		num2 = part2.toInt();
		num3 = part3.toInt();
	/*
		Serial.print("Num2: \t");													//TESTING
		Serial.println(num2);														//TESTING
		Serial.print("Num3: \t");													//TESTING
		Serial.println(num3);														//TESTING
	*/	
	if (strcasestr(part,"START"))		//START command
	{
		if (num2 > 0)
		{
			Serial.print("Minutes SET to:");										//TESTING
			Serial.println(num2);													//TESTING
		}
		pumpIsActive = 1;
	}
	else if (strcasestr(part,"STOP"))		//STOP command
	{
		
	//	Serial.println("STOP RECEIVED");		//TESTING
		pumpIsActive = 0;
		
	}
	
	
}

//Starts watering function lasting specified minutes
void DoWatering(int minutes)
{
	unsigned long milliseconds = (minutes * 60000);
	unsigned long now_milliseconds = millis();
		Serial.print("Starting DoWatering() \t"); 							//TESTING
		Serial.print("now_milliseconds = "); 									//TESTING
		Serial.println(now_milliseconds);										//TESTING
		Serial.print("milliseconds to water = "); 							//TESTING
		Serial.println(milliseconds);												//TESTING
		Serial.print("milliseconds Sum = "); 									//TESTING
		Serial.println(milliseconds+now_milliseconds);						//TESTING
	while (millis() < now_milliseconds + milliseconds)
		{
			digitalWrite(pump1Pin, HIGH);
			digitalWrite(pump1LedPin, HIGH);
		}
	digitalWrite(pump1Pin, LOW);
	digitalWrite(pump1LedPin, LOW);
	
	
}

void StopWatering(int minutes)
{
	unsigned long milliseconds = (minutes * 60000);
	unsigned long now_milliseconds = millis();
	while (now_milliseconds + milliseconds < millis())
		{		}
	digitalWrite(pump1Pin, LOW);
	digitalWrite(pump1LedPin, LOW);
	
}