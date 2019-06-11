# Switch-Controller-Library-for-Teensy-3.x-Arduino

## Introduction
Switch Control Library for Teensy 3.x/Arduino is a useful tool for managing power relays associated with physical switches.  The library provides several operating modes for physical switches including: standard ON/OFF behavior, stopwatch timer with variable duty cycle, toggle timer for ON/OFF occurring at local clock times and support for triggers sent from other classes.  The associated Timer library can also be used independently for other applications requiring stopwatches and toggle timers.
 
**Note: To use these libraries with Arduino, the elapsedMillis class is required from here.**

## Version
The current version is Beta-1.0. Not all features of the library have been evaluated at present.

## Disclaimer
The Switch Control Library is intended to be used for hobby purposes only.  Under no circumstances should it be used for applications that could potentially threaten life, safety or environment.  The author assumes no responsibility for any damages caused by real-world devices/equipment controlled using this library.

## Usage
![Example Circuit](/example_circuit.bmp)

In the above example, there are two SPST switches, one SPDT switch and one SP3T switch with input signals feeding into pull-up pins on a Teensy 3.6.  When a particular switch position is in the “active” state, it means that its associated pull-up pin is pulled LOW.  Let’s configure our system to work in the following manner:


