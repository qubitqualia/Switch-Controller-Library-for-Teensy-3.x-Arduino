/* Switch.h
 *
 * Copyright (c) 2019 by Justin Holland <jmatthew_7@yahoo.com>
 * Switch controller library for Teensy 3.x.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include "Arduino.h"
#include <vector>
#include <string>

class Timer;
class Stopwatch;
class ToggleTimer;

enum class SwitchType {
	SPST,
	SPDT,
	SP3T
};

enum class SwitchActive {
	High,
	Low
};

enum class SwitchAlarm {
	Stopwatch,
	Toggler,
	External
};

struct Position {
	std::string name;
	uint8_t InputPin;
	uint8_t RelayPin[10];
	Stopwatch* swatch;
	ToggleTimer* toggler;
	uint8_t input_state;
	uint8_t output_state;
	int swatch_ontime;
	bool swatch_in_progress;
	int trigger_ontime;		//init to -1
	bool external_trigger;
	bool trigger_rcv;
	bool trigger_in_progress;
	bool active;
	elapsedMillis trigger_timer;
	elapsedMillis swatch_timer;
};

class Switch {


	public:
		Switch(SwitchType type, std::string sw_name);
		Switch(SwitchType type, std::string sw_name, std::string pos1_name, std::string pos2_name);
		Switch(SwitchType type, std::string sw_name, std::string pos1_name, std::string pos2_name, std::string pos3_name);
		Switch(SwitchType type);
		~Switch();

		/*------------PUBLIC MEMBER VARIABLES & FUNCTIONS--------------------*/

		/*------------------------------
		 * Define name of switch
		 * -----------------------------
		 * ARGUMENTS:
		 * name = Switch name
		 *
		 * Switch instances do not require a name since each one can be fully
		 * controlled by calling its member functions.  However, naming can be beneficial
		 * when several switches need to be managed.  In this case, it is better
		 * to call the static member functions which will accept the switch name as
		 * an argument.
		 */
		void setSwitchName(std::string name);

		/*-------------------------------
		 * Define position name
		 * ------------------------------
		 * ARGUMENTS:
		 * pos = Position number (0, 1, 2)
		 * name = Position name
		 *
		 * Naming positions is not required but may be helpful when managing several
		 * switches with multiple positions. Positions are designated by integer values
		 * which can be used in addition to names provided by user:
		 *
		 * SPST --> Position 0
		 * SPDT --> Position 0, Position 1
		 * SP3T --> Position 0, Position 1, Position 2
		 *
		 */
		void setPositionName(int pos, std::string name);

		/*---------------------------------
		 * Define pins for SPST Switch
		 *---------------------------------
		 * ARGUMENTS:
		 * ip = Input pin number
		 * op = Output pin number (optional)
		 *
		 * This function can only be called for SPST switches!
		 * The input pin is the pin physically attached to the switch that is dedicated
		 * to detecting changes in the switch state. The output pin is optional. It should
		 * be defined when an output relay needs to be activated/deactivated based on switch
		 * position.
		 */
		void defineInputPin(uint8_t ip);
		static void _defineInputPin(std::string sw_name, uint8_t ip);
		void defineOutputPins(uint8_t op);
		void defineOutputPins(uint8_t op[], int size);
		static void _defineOutputPins(std::string sw_name, uint8_t op);
		static void _defineOutputPins(std::string sw_name, uint8_t op[], int size);

		/*----------------------------------
		 * Define pins for all switch types
		 * ---------------------------------
		 * ARGUMENTS:
		 * pos_name = Position name
		 * ip = Input pin number
		 * op = Output pin number
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * The input pin is the pin physically attached to the switch that is dedicated
		 * to detecting changes in the switch state. The output pin is optional. It should
		 * be defined when an output relay needs to be activated/deactivated based on switch
		 * position.
		 */
		void defineInputPin(std::string pos_name, uint8_t ip);
		static void _defineInputPin(std::string sw_name, std::string pos_name, uint8_t ip);
		void defineOutputPins(std::string pos_name, uint8_t op);
		void defineOutputPins(std::string pos_name, uint8_t op[], int size);
		static void _defineOutputPins(std::string sw_name, std::string pos_name, uint8_t op);
		static void _defineOutputPins(std::string sw_name, std::string pos_name, uint8_t op[], int size);

		/*-----------------------------------
		 * Define pins for all switch types
		 * ----------------------------------
		 * ARGUMENTS:
		 * pos = Position number (0, 1, 2)
		 * ip = Input pin number
		 * op = Output pin number
		 *
		 * Function can be called for SPST, SPDT, and SP3T switches.
		 * Overloaded version that accepts position number as argument.
		 */
		void defineInputPin(int pos, uint8_t ip);
		static void _defineInputPin(std::string sw_name, int pos, uint8_t ip);
		void defineOutputPins(int pos, uint8_t op);
		void defineOutputPins(int pos, uint8_t op[], int size);
		static void _defineOutputPins(std::string sw_name, int pos, uint8_t op);
		static void _defineOutputPins(std::string sw_name, int pos, uint8_t op[], int size);

		/*-----------------------------------
		 * Add alarm to a SPST switch
		 * ----------------------------------
		 * ARGUMENTS:
		 * type = SwitchAlarm::Stopwatch OR SwitchAlarm::Toggler
		 *
		 * Function can only be called for SPST switches!
		 * Add an alarm to a SPST switch.  Two alarm types are provided by the Timer
		 * class: Stopwatch and Toggler.  Stopwatches alarm at a specified time interval.
		 * Togglers provide ON/OFF alarm notifications based on 24-hour clock time.
		 *
		 */
		void addAlarm(SwitchAlarm type);
		static void _addAlarm(SwitchAlarm type, std::string sw_name);

		/*-----------------------------------
		 * Add alarm to any switch type
		 * ----------------------------------
		 * ARGUMENTS:
		 * pos = Position number (0, 1, 2)
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * Overloaded version accepts position number as argument.
		 */
		void addAlarm(SwitchAlarm type, int pos);
		static void _addAlarm(SwitchAlarm type, std::string sw_name, int pos);

		/*-----------------------------------
		 * Add alarm to any switch type
		 * ----------------------------------
		 * ARGUMENTS:
		 * type = SwitchAlarm::Stopwatch OR SwitchAlarm::Toggler
		 * pos_name = Position name
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * Add an alarm to a switch position. Two alarm types are provided by the
		 * Timer class: Stopwatch and Toggler. Stopwatches alarm at a specified time
		 * interval. Togglers provide ON/OFF alarm notifications based on 24-hour clock time.
		 */
		void addAlarm(SwitchAlarm type, std::string pos_name);
		static void _addAlarm(SwitchAlarm type, std::string sw_name, std::string pos_name);

		/*------------------------------------
		 * Set stopwatch alarm for SPST switch
		 * -----------------------------------
		 * ARGUMENTS:
		 * mils = Alarm interval in millis
		 *
		 * Function can only be called for SPST switches!
		 * Sets the time interval for a stopwatch alarm that has been added to a SPST
		 * switch using `addAlarm(SwitchAlarm type)`
		 */
		void setAlarm(int mils, int on_time);
		static void _setAlarm(std::string sw_name, int mils, int on_time);

		/*------------------------------------
		 * Set toggler alarm for SPST switch
		 * -----------------------------------
		 * ARGUMENTS:
		 * on_time = ON time for alarm ("HH:MM:SS" in 24-hour format)
		 * off_time = OFF time for alarm ("HH:MM:SS" in 24-hour format)
		 *
		 * Function can only be called for SPST switches!
		 * Sets the ON and OFF times for a toggler alarm that has been added to a SPST
		 * switch using `addAlarm(SwitchAlarm type)`
		 */
		void setAlarm(std::string on_time, std::string off_time);
		static void _setAlarm(std::string sw_name, std::string on_time, std::string off_time);

		/*----------------------------------------
		 * Set stopwatch alarm for any switch type
		 * ---------------------------------------
		 * ARGUMENTS:
		 * pos = Position number (0, 1, 2)
		 * mils = Alarm interval in millis
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * Overloaded version accepts position number as argument
		 */
		void setAlarm(int pos, int mils, int on_time);
		static void _setAlarm(std::string sw_name, int pos, int mils, int on_time);

		/*----------------------------------------
		 * Set stopwatch alarm for any switch type
		 * ---------------------------------------
		 * ARGUMENTS:
		 * pos_name = Position name
		 * mils = Alarm interval in millis
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * Sets the time interval for a stopwatch alarm that has been added to a
		 * switch position.
		 */
		void setAlarm(std::string pos_name, int mils, int on_time);
		static void _setAlarm(std::string sw_name, std::string pos_name, int mils, int on_time);


		/*----------------------------------------
		 * Set toggler alarm for any switch type
		 * ---------------------------------------
		 * ARGUMENTS:
		 * pos = Position number (0, 1, 2)
		 * on_time = ON time for alarm ("HH:MM:SS" in 24-hour format)
		 * off_time = OFF time for alarm ("HH:MM:SS" in 24-hour format)
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * Overloaded version accepts position number as argument
		 */
		void setAlarm(int pos, std::string on_time, std::string off_time);
		static void _setAlarm(std::string sw_name, int pos, std::string on_time, std::string off_time);

		/*----------------------------------------
		 * Set toggler alarm for any switch type
		 * ---------------------------------------
		 * ARGUMENTS:
		 * pos_name = Position name
		 * on_time = ON time for alarm ("HH:MM:SS" in 24-hour format)
		 * off_time = OFF time for alarm ("HH:MM:SS" in 24-hour format)
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * Sets the ON and OFF times for a toggler alarm that has been added to a switch
		 * position.
		 */
		void setAlarm(std::string pos_name, std::string on_time, std::string off_time);
		static void _setAlarm(std::string sw_name, std::string pos_name, std::string on_time, std::string off_time);

		/*-------------------------------------------
		 * Delete an alarm attached to a SPST switch
		 * ------------------------------------------
		 * ARGUMENTS:
		 * none
		 *
		 * Function can only be called for SPST switches!
		 */
		void deleteAlarm();
		static void _deleteAlarm(std::string sw_name);

		/*--------------------------------------------
		 * Delete an alarm attached to any switch type
		 * -------------------------------------------
		 * ARGUMENTS:
		 * pos = Position number (0, 1, 2)
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 */
		void deleteAlarm(int pos);
		static void _deleteAlarm(std::string sw_name, int pos);

		/*--------------------------------------------
		 * Delete an alarm attached to any switch type
		 * -------------------------------------------
		 * ARGUMENTS:
		 * pos_name = Position name
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 */
		void deleteAlarm(std::string pos_name);
		static void _deleteAlarm(std::string sw_name, std::string pos_name);

		/*-------------------------------------
		 * Toggle alarm attached to SPST switch
		 * ------------------------------------
		 * ARGUMENTS:
		 * on = TRUE for alarm active, FALSE for alarm inactive
		 *
		 * Function can only be called for SPST switches!
		 */
		void toggleAlarm(bool on);
		static void _toggleAlarm(std::string sw_name, bool on);

		/*-----------------------------------------
		 * Toggle alarm attached to any switch type
		 * ----------------------------------------
		 * ARGUMENTS:
		 * pos = Position number (0, 1, 2)
		 * on = TRUE for alarm active, FALSE for alarm inactive
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * Overloaded version accepts position number as argument.
		 */
		void toggleAlarm(int pos, bool on);
		static void _toggleAlarm(std::string sw_name, int pos, bool on);

		/*------------------------------------------
		 * Toggle alarm attached to any switch type
		 * -----------------------------------------
		 * ARGUMENTS:
		 * pos_name = Position name
		 * on = TRUE for alarm active, FALSE for alarm inactive
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 */
		void toggleAlarm(std::string pos_name, bool on);
		static void _toggleAlarm(std::string sw_name, std::string pos_name, bool on);

		/*-------------------------------------------
		 * Pause a switch
		 *-------------------------------------------
		 * ARGUMENTS:
		 * none
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * While switch is paused, the input state will be ignored and
		 * no changes will be made to the output state.
		 */
		void pause();
		static void _pause(std::string sw_name);

		/*--------------------------------------------
		 * Resume a switch
		 *--------------------------------------------
		 * ARGUMENTS:
		 * none
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * Ends pause condition and returns switch to normal functionality.
		 */
		void resume();
		static void _resume(std::string sw_name);

		/*----------------------------------
		 * Get input state for SPST switch
		 *----------------------------------
		 * ARGUMENTS:
		 * none
		 *
		 * Function can be only be called for SPST switches!
		 * Returns input state of switch as bool
		 */
		uint8_t getInputState();
		static uint8_t _getInputState(std::string sw_name);

		/*-------------------------------------
		 * Get input state for any switch type
		 *-------------------------------------
		 * ARGUMENTS:
		 * pos_name = Position name
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * Returns input state of switch position as bool
		 */
		uint8_t getInputState(std::string pos_name);
		static uint8_t _getInputState(std::string sw_name, std::string pos_name);

		/*-------------------------------------
		 * Get input state for any switch type
		 *-------------------------------------
		 * ARGUMENTS:
		 * pos = Position number (0, 1, 2)
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * Overloaded version accepts position number as argument
		 */
		uint8_t getInputState(int pos);
		static uint8_t _getInputState(std::string sw_name, int pos);

		/*------------------------------------
		 * Get output state for SPST switch
		 * -----------------------------------
		 * ARGUMENTS:
		 * none
		 *
		 * Function can only be called for SPST switches!
		 * Returns output state of switch as bool
		 */
		uint8_t getOuputState();
		static uint8_t _getOutputState(std::string sw_name);

		/*--------------------------------------
		 * Get output state for any switch type
		 *--------------------------------------
		 * ARGUMENTS:
		 * pos_name = Position name
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * Returns output state of switch position as bool
		 */
		uint8_t getOutputState(std::string pos_name);
		static uint8_t _getOutputState(std::string sw_name, std::string pos_name);

		/*--------------------------------------
		 * Get output state for any switch type
		 * -------------------------------------
		 * ARGUMENTS:
		 * pos = Position number (0, 1, 2)
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * Overloaded version accepts position number as argument
		 */
		uint8_t getOutputState(int pos);
		static uint8_t _getOutputState(std::string sw_name, int pos);

		/*---------------------------------------
		 * Trigger SPST switch ON/OFF (external)
		 * --------------------------------------
		 * ARGUMENTS:
		 * none
		 *
		 * Function can only be called for SPST switches.
		 * Intended to be called by external classes to trigger switch
		 * on/off.
		 *
		 * NOTE 1: Switch instance must be set to accept external
		 * triggers by calling `addAlarm(SwitchAlarm::External)`
		 *
		 * NOTE 2: Trigger only acknowledged when physical switch position
		 * is active
		 */
		void triggerON();
		void triggerOFF();
		static void _triggerON(std::string sw_name);
		static void _triggerOFF(std::string sw_name);

		/*----------------------------------------------
		 * Trigger ON/OFF for any switch type (external)
		 * ---------------------------------------------
		 * ARGUMENTS:
		 * pos = Position number (0, 1, 2)
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * Intended to be called by external classes to trigger switch
		 * position on/off
		 *
		 * NOTE 1: Switch instance must be set to accept external
		 * triggers by calling `addAlarm(SwitchAlarm::External,...)`
		 *
		 * NOTE 2: Trigger only acknowledged when physical switch position
		 * is active
		 */

		void triggerON(int pos);
		void triggerOFF(int pos);
		static void _triggerON(std::string sw_name, int pos);
		static void _triggerOFF(std::string sw_name, int pos);

		/*-----------------------------------------------
		 * Trigger ON/OFF for any switch type (external)
		 * ----------------------------------------------
		 * ARGUMENTS:
		 * pos_name = Position name
		 *
		 * Function can be called for SPST, SPDT and SP3T switches.
		 * Intended to be called by external classes to trigger switch
		 * position on/off
		 *
		 * NOTE 1: Switch instance must be set to accept external
		 * triggers by calling `addAlarm(SwitchAlarm::External,...)`
		 *
		 * NOTE 2: Trigger only acknowledged when physical switch position
		 * is active
		 */

		void triggerON(std::string pos_name);
		void triggerOFF(std::string pos_name);
		static void _triggerON(std::string sw_name, std::string pos_name);
		static void _triggerOFF(std::string sw_name, std::string pos_name);

		/*--------------------------------------------------------
		 * Trigger ON for SPST for specific time period (external)
		 * -------------------------------------------------------
		 * ARGUMENTS:
		 * on_time = Time to keep switch ON in millis
		 *
		 * Function can only be called for SPST switches.
		 * Intended to be called by external classes to trigger switch
		 * position on/off
		 *
		 * NOTE 1: Switch instance must be set to accept external triggers
		 * by calling `addAlarm(SwitchAlarm::External)
		 *
		 * NOTE 2: Trigger only acknowledged when physical switch position
		 * is active
		 */

		void trigger(int on_time);
		static void _trigger(std::string sw_name, int on_time);

		/*------------------------------------------------------------------
		 * Trigger ON for any switch type for specific time period (external)
		 * -----------------------------------------------------------------
		 * ARGUMENTS:
		 * pos = Position number (0, 1, 2)
		 * on_time = Time to keep switch ON in millis
		 *
		 * Function can be called by SPST, SPDT and SP3T switches.
		 * Intended to be called by external classes to trigger switch position on/off
		 *
		 * NOTE 1: Switch instance must be set to accept external triggers
		 * by calling `addAlarm(SwitchAlarm::External,...)
		 *
		 * NOTE 2: Trigger only acknowledged when physical switch position
		 * is active
		 *
		 */
		void trigger(int pos, int on_time);
		static void _trigger(std::string sw_name, int pos, int on_time);

		/*--------------------------------------------------------------------
		 * Trigger ON for any switch type for specific time period (external)
		 * -------------------------------------------------------------------
		 * ARGUMENTS:
		 * pos_name = Position name
		 * on_time = Time to keep switch ON in millis
		 *
		 * Function can be called by SPST, SPDT and SP3T switches.
		 * Intended to be called by external classes to trigger switch position on/off
		 *
		 * NOTE 1: Switch instance must be set to accept external triggers
		 * by calling `addAlarm(SwitchAlarm::External,...)
		 *
		 * NOTE 2: Trigger only acknowledged when physical switch position
		 * is active
		 */
		void trigger(std::string pos_name, int on_time);
		static void _trigger(std::string sw_name, std::string pos_name, int on_time);




		/*------------PUBLIC STATIC MEMBER VARIABLES & FUNCTIONS--------------*/

		static std::vector<Switch*> switch_list;
		static volatile bool PAUSE;

		/*---------------------
		 *Initialize switches
		 *---------------------
		 *ARGUMENTS:
		 *t = Pointer to Timer instance
		 *
		 *Function must be called in Setup!
		 *Timer instance pointer is only required if stopwatch and/or
		 *toggler alarms will be implemented.
		 */
		static void init(Timer* t);

		/*---------------------
		 * Update switches
		 *---------------------
		 * ARGUMENTS:
		 * none
		 *
		 * Function must be called in Loop!
		 * Monitors switch inputs and handles updates to output relay states
		 * Handles alarms attached to switches
		 */
		static void update();

		/*--------------------------
		 * Pause all switches
		 * -------------------------
		 * ARGUMENTS:
		 * none
		 */
		static void _pauseAll();

		/*--------------------------
		 * Resume all switches
		 * -------------------------
		 * ARGUMENTS:
		 * none
		 */
		static void _resumeAll();

		/*------------------------------------------
		 * Trigger all switches ON/OFF (external)
		 * -----------------------------------------
		 * ARGUMENTS:
		 * none
		 *
		 * Intended to be called by external classes to trigger
		 * active switch positions on/off
		 *
		 * NOTE 1: All switch positions to be triggered must be set
		 * to accept external triggers by calling
		 * `addAlarm(SwitchAlarm::External,...)`
		 *
		 * NOTE 2: Trigger only acknowledged by switch positions that
		 * are physically selected
		 */
		static void _triggerONAll();
		static void _triggerOFFAll();

		/*-----------------------------------------------------------
		 * Trigger all switches for a specific time period (external)
		 * ----------------------------------------------------------
		 * ARGUMENTS:
		 * on_time = Time to keep switches ON in millis
		 *
		 * Intended to be called by external classes to trigger
		 * active switch positions on/off
		 *
		 * NOTE 1: All switch positions to be triggered must be set
		 * to accept external triggers by calling
		 * `addAlarm(SwitchAlarm::External,...)`
		 *
		 * NOTE 2: Trigger only acknowledged by switch positions that
		 * are physically selected
		 */
		static void _triggerAll(int on_time);

		/*-----------------------------------------------------------
		 * Print status of all positions for a switch to Serial port
		 * ----------------------------------------------------------
		 * ARGUMENTS:
		 * sw_name = Switch name
		 */
		void printSwitchStatus();
		static void _printSwitchStatus(std::string sw_name);

		/*-----------------------------------------------------------
		 * Print status of a specific switch position to Serial port
		 * ----------------------------------------------------------
		 * ARGUMENTS:
		 * sw_name = Switch name
		 * pos = Position number (0, 1, 2)
		 */
		void printSwitchStatus(int pos);
		static void _printSwitchStatus(std::string sw_name, int pos);

		/*-----------------------------------------------------------
		 * Print status of a specific switch position to Serial port
		 * ----------------------------------------------------------
		 * ARGUMENTS:
		 * sw_name = Switch name
		 * pos_name = Position name
		 */
		void printSwitchStatus(std::string pos_name);
		static void _printSwitchStatus(std::string sw_name, std::string pos_name);

		/*-----------------------------------------------
		 * Print status of all switches to Serial port
		 * ----------------------------------------------
		 * ARGUMENTS:
		 * none
		 *
		 */
		static void _printSwitchStatusAll();

		/*----------------------------------------------------------------------
		 * Define active logic level for input pins associated with all switches
		 * ---------------------------------------------------------------------
		 * ARGUMENTS:
		 * level = HIGH, LOW
		 *
		 * Use HIGH for switches that are wired to an external voltage source
		 * Use LOW for switches that are wired to GND (configures input pins as PULLUP)
		 */
		void setInputActiveLevel(uint8_t level);
		static void _setInputActiveLevel(std::string sw_name, uint8_t level);
		static void _setInputActiveLevelAll(uint8_t level);

		/*-------------------------------------------------------------------------
		 * Define active logic level for output relays associated with all switches
		 * ------------------------------------------------------------------------
		 * ARGUMENTS:
		 * level = HIGH, LOW
		 */
		void setOutputActiveLevel(uint8_t level);
		static void _setOutputActiveLevel(std::string sw_name, uint8_t level);
		static void _setOutputActiveLevelAll(uint8_t level);


	private:
		Position* pos1;
		Position* pos2;
		Position* pos3;

		SwitchType sw_type;
		Timer* timer;
		std::string sw_name;
		bool pause_state;
		uint8_t input_level;
		uint8_t output_level;


		static Switch* findSwitch(std::string name);
		void toggleOutputState(uint8_t lev);
		void toggleOutputState(std::string pos_name, uint8_t lev);
		void toggleOutputState(int pos, uint8_t lev);
		static void _toggleOutputStateAll(uint8_t lev);

		/*static void _toggleOutputState(std::string sw_name, std::string pos_name, uint8_t lev);
		static void _toggleOutputState(std::string sw_name, int pos, uint8_t lev);
		static void _toggleOutputState(std::string sw_name, uint8_t lev);*/

		static void defineInputPin(Switch* _switch, uint8_t ip);
		static void defineOutputPins(Switch* _switch, uint8_t op);
		static void defineOutputPins(Switch* _switch, uint8_t op[], int size);
		static void defineInputPin(Switch* _switch, int pos, uint8_t ip);
		static void defineOutputPins(Switch* _switch, int pos, uint8_t op);
		static void defineOutputPins(Switch* _switch, int pos, uint8_t op[], int size);
		static void defineInputPin(Switch* _switch, std::string pos_name, uint8_t ip);
		static void defineOutputPins(Switch* _switch, std::string pos_name, uint8_t op);
		static void defineOutputPins(Switch* _switch, std::string pos_name, uint8_t op[], int size);
		static void addAlarm(Switch* _switch, SwitchAlarm type);
		static void addAlarm(Switch* _switch, SwitchAlarm type, int pos);
		static void addAlarm(Switch* _switch, SwitchAlarm type, std::string pos_name);
		static void setAlarm(Switch* _switch, int mils, int on_time);
		static void setAlarm(Switch* _switch, std::string on_time, std::string off_time);
		static void setAlarm(Switch* _switch, int pos, int mils, int on_time);
		static void setAlarm(Switch* _switch, std::string pos_name, int mils, int on_time);
		static void setAlarm(Switch* _switch, int pos, std::string on_time, std::string off_time);
		static void setAlarm(Switch* _switch, std::string pos_name, std::string on_time, std::string off_time);
		static void deleteAlarm(Switch* _switch);
		static void deleteAlarm(Switch* _switch, int pos);
		static void deleteAlarm(Switch* _switch, std::string pos_name);
		static void toggleAlarm(Switch* _switch, bool on);
		static void toggleAlarm(Switch* _switch, int pos, bool on);
		static void toggleAlarm(Switch* _switch, std::string pos_name, bool on);
		static void pause(Switch* _switch);
		static void resume(Switch* _switch);
		static uint8_t getInputState(Switch* _switch);
		static uint8_t getInputState(Switch* _switch, std::string pos_name);
		static uint8_t getInputState(Switch* _switch, int pos);
		static uint8_t getOutputState(Switch* _switch);
		static uint8_t getOutputState(Switch* _switch, std::string pos_name);
		static uint8_t getOutputState(Switch* _switch, int pos);
		static void toggleOutputState(Switch* _switch, uint8_t lev);
		static void toggleOutputState(Switch* _switch, std::string pos_name, uint8_t lev);
		static void toggleOutputState(Switch* _switch, int pos, uint8_t lev);
		static void handleSwitchPosition(Switch* _switch, int pos);
		static void updateInputStates(Switch* _switch);
		static void printError(int errno);
		static void printSwitchStatus(Switch* _switch);
		static void printSwitchStatus(Switch* _switch, int pos);
		static void printSwitchStatus(Switch* _switch, std::string pos_name);
		static void setInputActiveLevel(Switch* _switch, uint8_t level);
		static void setOutputActiveLevel(Switch* _switch, uint8_t level);
		static void trigger(Switch* _switch, int on_time);
		static void trigger(Switch* _switch, int pos, int on_time);
		static void trigger(Switch* _switch, std::string pos_name, int on_time);
		static void triggerON(Switch* _switch);
		static void triggerON(Switch* _switch, int pos);
		static void triggerON(Switch* _switch, std::string pos_name);
		static void triggerOFF(Switch* _switch);
		static void triggerOFF(Switch* _switch, int pos);
		static void triggerOFF(Switch* _switch, std::string pos_name);





};




#endif /* SWITCH_H_ */
