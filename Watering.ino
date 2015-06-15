/*
### Arduino Watering System ###
This is a simple controller to allow programmable watering of plants

*/

// Pin configuration
const byte pump1Pin = 15;


//pinMode declaration
pinMode (pump1Pin, OUTPUT);


void setup()
{
  
}

void loop()
{
	
}

//Start watering function lasting specified minutes
void DoWatering(int minutes)
{
	unsigned long milliseconds = minutes * 60000);
	unsigned long now_milliseconds = millis();
	while (now_milliseconds + milliseconds < millis())
		{
			//CONTINUARE
		}
	
	
}