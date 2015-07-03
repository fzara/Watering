
# Arduino Watering System #
This is a simple controller to allow programmable watering of plants. It basically controls a pump (PWM capable) via a transistor (TIP-122). 

The sketch receives commands via the serial interface.

####Commands available over serial/bluetooth (case-sensitive):####

| Command   | Action         |
| ----------| ----------- |
| LEDTEST   | Makes the led blink 5 times (testing)|
| PUMPTEST  | Activates the pump for 2 seconds (testing)|  
| START     | Enables the pump    |
| STOP      | Disables the pump    |
| OVR 1/0   | Enables/Disables Pump Override Stop   |


ToDo:
* Add RTC 
* Allow programming of RTC via Bluetooth
* Add controls via TCP/IP. (I have no idea how to do it!)
