
# Arduino Watering System #
This is a simple controller to allow programmable watering of plants. It basically controls a pump (PWM capable) via a transistor (TIP-122). 

The sketch receives commands via the serial interface.

####Commands available over serial/bluetooth (case-sensitive):####

*Italic commands are still in testing*

| Command   		| Action         |
| ---------------	| ----------- |
| LEDTEST   		| Makes the led blink 5 times (testing)     																|
| PUMPTEST  		| Activates the pump for 2 seconds (testing)																| 
| SPEED     		| Sets the pump speed via PWM (1-255)       																|
| START     		| Enables the pump                          																|
| STOP      		| Disables the pump                        														 		|
| OVR 1/0   		| Enables/Disables Pump Override Stop       																|
| *TIMER N H M D*	| *Sets the Timer n° N to hour H and minute M for a duration D (expressed in minutes)* 	|
| *TIMER N 1/0*	| *Activates/Deactivates timer n° N*																		|


ToDo:
* Add RTC 
* Allow programming of RTC via Bluetooth
* Add controls via TCP/IP. (I have no idea how to do it!)
