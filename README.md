# Switch-Controller-Library-for-Teensy-3.x-Arduino

## Introduction
Switch Control Library for Teensy 3.x/Arduino is a useful tool for managing power relays associated with physical switches.  The library provides several operating modes for physical switches including: standard ON/OFF behavior, stopwatch timer with variable duty cycle, toggle timer for ON/OFF occurring at local clock times and support for triggers sent from other classes.  The associated Timer library can also be used independently for other applications requiring stopwatches and toggle timers.
 
**Note: To use these libraries with Arduino, the elapsedMillis class may be required from [here](https://github.com/pfeerick/elapsedMillis/archive/master.zip).**

## Version
The current version is Beta-1.0. Not all features of the library have been evaluated at present.

## Disclaimer
The Switch Control Library is intended to be used for hobby purposes only.  Under no circumstances should it be used for applications that could potentially threaten life, safety or environment.  The author assumes no responsibility for any damages caused by real-world devices/equipment controlled using this library.

## Usage
![Example Circuit](/example_circuit.bmp)

In the above example, there are two SPST switches, one SPDT switch and one SP3T switch with input signals feeding into pull-up pins on a Teensy 3.6.  When a particular switch position is in the “active” state, it means that its associated pull-up pin is pulled LOW.  Let’s configure our system to work in the following manner:

![Switch Table](/switch_table2.png)

The system above can be setup in two different ways depending on user preference:

### Setup Using Object Methods
```c++
#include "Arduino.h"
#include "Switch.h"
#include "Timer.h"
#include "string_Teensy.h"


Switch heater(SwitchType::SPST);
Switch camera(SwitchType::SPST);
Switch fan(SwitchType::SPDT);
Switch lights(SwitchType::SP3T);

Timer timer;
elapsedMillis dt;

void setup()
{
	Serial.begin(115200);
	
	//Define pins for Heater
	heater.defineInputPin(2);
	heater.defineOutputPins(23);
	
	//Define pins for Camera
	camera.defineInputPin(3);
	camera.defineOutputPins(24);
	
	//Define pins for Fan
	fan.setPositionName(0, "Low Speed");
	fan.setPositionName(1, "High Speed");
	fan.defineInputPin("Low Speed", 4);
	fan.defineOutputPins("Low Speed", 25);
	fan.defineInputPin("High Speed", 5);
	fan.defineOutputPins("High Speed", 26);
	
	//Define pins for Lights
	lights.setPositionName(0, "Day Mode");
	lights.setPositionName(1, "Night Mode");
	lights.setPositionName(2, "Timer");
	lights.defineInputPin("Day Mode", 28);
	lights.defineOutputPins("Day Mode", 37);
	lights.defineInputPin("Night Mode", 29);
	lights.defineOutputPins("Night Mode", 38);
	lights.defineOutputPins("Timer", 37);
	
	//Configure alarm for Fan
	fan.addAlarm(SwitchAlarm::Stopwatch, "Low Speed");
	fan.setAlarm("Low Speed", 100000, 10000);

	//Add external trigger for Camera
	fan.addAlarm(SwitchAlarm::External);
	
	//Configure alarm for Lights
	lights.addAlarm(SwitchAlarm::Toggler, "Timer");
	lights.setAlarm("Timer", "07:00:00", "19:01:00");
	
	//Set active input and output logic levels for switches
	Switch::_setInputActiveLevelAll(LOW);
	Switch::_setOutputActiveLevelAll(LOW);
	
	//Provide current utc unix time to Timer class
	timer.setCurrentTime(1559851450);
	
	//Initialize switches	
	Switch::init(&timer);
	
	dt = 0;
}
```
### Setup Using Static Methods

```c++
#include "Arduino.h"
#include "Switch.h"
#include "Timer.h"
#include "string_Teensy.h"

Switch switch1(SwitchType::SPST, "Heater");
Switch switch2(SwitchType::SPST, "Camera");
Switch switch3(SwitchType::SPDT, "Fan", "Low Speed", "High Speed");
Switch switch4(SwitchType::SP3T, "Lights", "Day Mode", "Night Mode", "Timer");

Timer;
elapsedMillis dt;

void setup()
{
	Serial.begin(115200);
	delay(6000);
	Serial.println("Switch Test Begin...");

	//Define pins for Heater
	Switch::_defineInputPin("Heater", 2);
	Switch::_defineOutputPins("Heater", 23);

	//Define pins for Camera
	Switch::_defineInputPin("Camera", 3);
	Switch::_defineOutputPins("Camera", 24);

	//Define pins for Fan
	Switch::_defineInputPin("Fan", "Low Speed", 4);
	Switch::_defineOutputPins("Fan", "Low Speed", 25);
	Switch::_defineInputPin("Fan", "High Speed", 5);
	Switch::_defineOutputPins("Feeder", "High Speed", 26);

	//Define pins for Lights
	Switch::_defineInputPin("Lights", "Day Mode", 28);
	Switch::_defineOutputPins("Lights", "Day Mode", 37);
	Switch::_defineInputPin("Lights", "Night Mode", 29);
	Switch::_defineOutputPins("Lights", "Night Mode", 38);
	Switch::_defineOutputPins("Lights", "Timer", 37);

	//Configure alarm for Fan
	Switch::_addAlarm(SwitchAlarm::Stopwatch, "Fan", "Low Speed");
	Switch::_setAlarm("Fan", "Low Speed", 100000, 10000);

	//Add external trigger alarm for Camera
	Switch::_addAlarm(SwitchAlarm::External, "Camera");

	//Configure alarm for Lights
	Switch::_addAlarm(SwitchAlarm::Toggler, "Lights", "Timer");
	Switch::_setAlarm("Lights", "Timer", "07:00:00", "19:01:00");

	//Set active input and output logic levels for switches
	Switch::_setInputActiveLevelAll(LOW);
	Switch::_setOutputActiveLevelAll(LOW);

	//Provide current utc unix time to Timer class
	timer.setCurrentTime(1559851450);
	
	//Initialize switches
	Switch::init(&timer);
	
	dt = 0;

}
```
## Loop Methods
Switch::update() handles all switch functionality in a non-blocking manner. It must be called in the loop block.  Additional functions are available to pause/resume one or more switches, print the current status of switches and trigger switches from external class.

## Example Program Behavior
![Behavior](/example_behavior2.bmp)
