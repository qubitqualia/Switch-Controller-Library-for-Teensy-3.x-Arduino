/* Switch.cpp
 *
 * Copyright (c) 2019 by Justin Holland <jmatthew_7@yahoo.com>
 * Switch controller library for Teensy 3.x.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 *
 */

#include "Switch.h"
#include "Arduino.h"
#include <algorithm>
#include "Timer.h"

volatile bool Switch::PAUSE = false;

/* PUBLIC FUNCTIONS */

Switch::Switch(SwitchType type) : sw_type(type)
{
	switch (sw_type) {

		case (SwitchType::SPST) : {
			pos1 = new Position();
			pos1->swatch = NULL;
			pos1->toggler = NULL;
			pos1->InputPin = 255;
			for (int i=0; i<10; i++) {
				pos1->RelayPin[i] = 255;
			}
			pos1->external_trigger = false;
			pos1->trigger_rcv = false;
			pos1->trigger_in_progress = false;
			pos1->swatch_in_progress = false;
			pos1->active = false;
			pos1->trigger_ontime = -1;
			pos2 = NULL;
			pos3 = NULL;
			break;
		}
		case (SwitchType::SPDT) : {
			pos1 = new Position();
			pos2 = new Position();
			pos1->InputPin = 255;
			pos2->InputPin = 255;
			for (int i=0; i<10; i++) {
				pos1->RelayPin[i] = 255;
				pos2->RelayPin[i] = 255;
			}
			pos1->external_trigger = false;
			pos2->external_trigger = false;
			pos1->trigger_rcv = false;
			pos2->trigger_rcv = false;
			pos1->trigger_in_progress = false;
			pos2->trigger_in_progress = false;
			pos1->swatch_in_progress = false;
			pos2->swatch_in_progress = false;
			pos1->active = false;
			pos2->active = false;
			pos1->trigger_ontime = -1;
			pos2->trigger_ontime = -1;
			pos1->swatch = NULL;
			pos1->toggler = NULL;
			pos2->swatch = NULL;
			pos2->toggler = NULL;
			pos3 = NULL;
			break;
		}
		case (SwitchType::SP3T) : {
			pos1 = new Position();
			pos2 = new Position();
			pos3 = new Position();
			pos1->InputPin = 255;
			pos2->InputPin = 255;
			pos3->InputPin = 255;
			for (int i=0; i<10; i++) {
				pos1->RelayPin[i] = 255;
				pos2->RelayPin[i] = 255;
				pos3->RelayPin[i] = 255;
			}
			pos1->external_trigger = false;
			pos2->external_trigger = false;
			pos3->external_trigger = false;
			pos1->trigger_rcv = false;
			pos2->trigger_rcv = false;
			pos3->trigger_rcv = false;
			pos1->trigger_in_progress = false;
			pos2->trigger_in_progress = false;
			pos3->trigger_in_progress = false;
			pos1->swatch_in_progress = false;
			pos2->swatch_in_progress = false;
			pos3->swatch_in_progress = false;
			pos1->active = false;
			pos2->active = false;
			pos3->active = false;
			pos1->trigger_ontime = -1;
			pos2->trigger_ontime = -1;
			pos3->trigger_ontime = -1;
			pos1->swatch = NULL;
			pos1->toggler = NULL;
			pos2->swatch = NULL;
			pos2->toggler = NULL;
			pos3->swatch = NULL;
			pos3->toggler = NULL;
			break;
		}
	}
	input_level = LOW;
	output_level = LOW;
	pause_state = false;
	timer = NULL;

	switch_list.push_back(this);
}
Switch::Switch(SwitchType type, std::string sw_name) : sw_type(type), sw_name(sw_name)
{
	switch (sw_type) {

		case (SwitchType::SPST) : {
			pos1 = new Position();
			pos1->InputPin = 255;
			for (int i=0; i<10; i++) {
				pos1->RelayPin[i] = 255;
			}
			pos1->swatch = NULL;
			pos1->toggler = NULL;
			pos1->external_trigger = false;
			pos1->trigger_rcv = false;
			pos1->trigger_in_progress = false;
			pos1->swatch_in_progress = false;
			pos1->active = false;
			pos1->trigger_ontime = -1;
			pos2 = NULL;
			pos3 = NULL;
			break;
		}
		case (SwitchType::SPDT) : {
			pos1 = new Position();
			pos2 = new Position();
			pos1->InputPin = 255;
			pos2->InputPin = 255;
			for (int i=0; i<10; i++) {
				pos1->RelayPin[i] = 255;
				pos2->RelayPin[i] = 255;
			}
			pos1->external_trigger = false;
			pos2->external_trigger = false;
			pos1->trigger_rcv = false;
			pos2->trigger_rcv = false;
			pos1->trigger_in_progress = false;
			pos2->trigger_in_progress = false;
			pos1->swatch_in_progress = false;
			pos2->swatch_in_progress = false;
			pos1->active = false;
			pos2->active = false;
			pos1->trigger_ontime = -1;
			pos2->trigger_ontime = -1;
			pos1->swatch = NULL;
			pos1->toggler = NULL;
			pos2->swatch = NULL;
			pos2->toggler = NULL;
			pos3 = NULL;
			break;
		}
		case (SwitchType::SP3T) : {
			pos1 = new Position();
			pos2 = new Position();
			pos3 = new Position();
			pos1->InputPin = 255;
			pos2->InputPin = 255;
			pos3->InputPin = 255;
			for (int i=0; i<10; i++) {
				pos1->RelayPin[i] = 255;
				pos2->RelayPin[i] = 255;
				pos3->RelayPin[i] = 255;
			}
			pos1->external_trigger = false;
			pos2->external_trigger = false;
			pos3->external_trigger = false;
			pos1->trigger_rcv = false;
			pos2->trigger_rcv = false;
			pos3->trigger_rcv = false;
			pos1->trigger_in_progress = false;
			pos2->trigger_in_progress = false;
			pos3->trigger_in_progress = false;
			pos1->swatch_in_progress = false;
			pos2->swatch_in_progress = false;
			pos3->swatch_in_progress = false;
			pos1->active = false;
			pos2->active = false;
			pos3->active = false;
			pos1->trigger_ontime = -1;
			pos2->trigger_ontime = -1;
			pos3->trigger_ontime = -1;
			pos1->swatch = NULL;
			pos1->toggler = NULL;
			pos2->swatch = NULL;
			pos2->toggler = NULL;
			pos3->swatch = NULL;
			pos3->toggler = NULL;
			break;
		}
	}
	input_level = LOW;
	output_level = LOW;
	pause_state = false;
	timer = NULL;

	switch_list.push_back(this);
}
Switch::Switch(SwitchType type, std::string sw_name, std::string pos1_name, std::string pos2_name) : sw_type(type), sw_name(sw_name) {
	switch (sw_type) {
		case (SwitchType::SPST) : {
			printError(11);
			pos1 = NULL;
			pos2 = NULL;
			pos3 = NULL;
			break;
		}
		case (SwitchType::SPDT) : {
			pos1 = new Position();
			pos2 = new Position();
			pos1->InputPin = 255;
			pos2->InputPin = 255;
			for (int i=0; i<10; i++) {
				pos1->RelayPin[i] = 255;
				pos2->RelayPin[i] = 255;
			}
			pos1->external_trigger = false;
			pos2->external_trigger = false;
			pos1->trigger_rcv = false;
			pos2->trigger_rcv = false;
			pos1->trigger_in_progress = false;
			pos2->trigger_in_progress = false;
			pos1->swatch_in_progress = false;
			pos2->swatch_in_progress = false;
			pos1->active = false;
			pos2->active = false;
			pos1->trigger_ontime = -1;
			pos2->trigger_ontime = -1;
			pos1->swatch = NULL;
			pos1->toggler = NULL;
			pos2->swatch = NULL;
			pos2->toggler = NULL;
			pos1->name = pos1_name;
			pos2->name = pos2_name;
			pos3 = NULL;
			break;
		}
		case (SwitchType::SP3T) : {
			pos1 = new Position();
			pos2 = new Position();
			pos3 = new Position();
			pos1->InputPin = 255;
			pos2->InputPin = 255;
			pos3->InputPin = 255;
			for (int i=0; i<10; i++) {
				pos1->RelayPin[i] = 255;
				pos2->RelayPin[i] = 255;
				pos3->RelayPin[i] = 255;
			}
			pos1->external_trigger = false;
			pos2->external_trigger = false;
			pos3->external_trigger = false;
			pos1->trigger_rcv = false;
			pos2->trigger_rcv = false;
			pos3->trigger_rcv = false;
			pos1->trigger_in_progress = false;
			pos2->trigger_in_progress = false;
			pos3->trigger_in_progress = false;
			pos1->swatch_in_progress = false;
			pos2->swatch_in_progress = false;
			pos3->swatch_in_progress = false;
			pos1->active = false;
			pos2->active = false;
			pos3->active = false;
			pos1->trigger_ontime = -1;
			pos2->trigger_ontime = -1;
			pos3->trigger_ontime = -1;
			pos1->swatch = NULL;
			pos1->toggler = NULL;
			pos2->swatch = NULL;
			pos2->toggler = NULL;
			pos3->swatch = NULL;
			pos3->toggler = NULL;
			pos1->name = pos1_name;
			pos2->name = pos2_name;
			break;
		}
	}
	input_level = LOW;
	output_level = LOW;
	pause_state = false;
	timer = NULL;

	switch_list.push_back(this);

}
Switch::Switch(SwitchType type, std::string sw_name, std::string pos1_name, std::string pos2_name, std::string pos3_name) : sw_type(type), sw_name(sw_name)
{
	switch (sw_type) {

		case (SwitchType::SPST) : {
			printError(11);
			pos1 = NULL;
			pos2 = NULL;
			pos3 = NULL;
			break;
		}
		case (SwitchType::SPDT) : {
			printError(12);
			pos1 = NULL;
			pos2 = NULL;
			pos3 = NULL;
			break;
		}
		case (SwitchType::SP3T) : {
			pos1 = new Position();
			pos2 = new Position();
			pos3 = new Position();
			pos1->InputPin = 255;
			pos2->InputPin = 255;
			pos3->InputPin = 255;
			for (int i=0; i<10; i++) {
				pos1->RelayPin[i] = 255;
				pos2->RelayPin[i] = 255;
				pos3->RelayPin[i] = 255;
			}
			pos1->external_trigger = false;
			pos2->external_trigger = false;
			pos3->external_trigger = false;
			pos1->trigger_rcv = false;
			pos2->trigger_rcv = false;
			pos3->trigger_rcv = false;
			pos1->trigger_in_progress = false;
			pos2->trigger_in_progress = false;
			pos3->trigger_in_progress = false;
			pos1->swatch_in_progress = false;
			pos2->swatch_in_progress = false;
			pos3->swatch_in_progress = false;
			pos1->active = false;
			pos2->active = false;
			pos3->active = false;
			pos1->trigger_ontime = -1;
			pos2->trigger_ontime = -1;
			pos3->trigger_ontime = -1;
			pos1->swatch = NULL;
			pos1->toggler = NULL;
			pos2->swatch = NULL;
			pos2->toggler = NULL;
			pos3->swatch = NULL;
			pos3->toggler = NULL;
			pos1->name = pos1_name;
			pos2->name = pos2_name;
			pos3->name = pos3_name;
			break;
		}
	}

	input_level = LOW;
	output_level = LOW;
	pause_state = false;
	timer = NULL;

	switch_list.push_back(this);
}
Switch::~Switch()
{
	std::vector<Switch*>::iterator it;
	const Switch* sw = this;
	it = std::find(switch_list.begin(), switch_list.end(), sw);
	switch_list.erase(it);
}

void Switch::setSwitchName(std::string name) {
	sw_name = name;

	return;
}
void Switch::setPositionName(int pos, std::string name) {

	//Check for valid arguments
	bool error = false;
	if (sw_type == SwitchType::SPST && pos != 0) {
		Serial.println("Switch Error -- Invalid position number specified; SPST can only define Position 0");
		error = true;
	}
	else if (sw_type == SwitchType::SPDT && pos != 0 && pos != 1) {
		Serial.println("Switch Error -- Invalid position number specified; SPDT can only define Positions 0 and 1");
		error = true;
	}
	else if (pos > 2 || pos < 0) {
		Serial.println("Switch Error -- Position number out of bounds; must be 0, 1 or 2");
		error = true;
	}

	if (!error) {
		if (pos == 0) {
			pos1->name = name;
		}
		else if (pos == 1) {
			pos2->name = name;
		}
		else if (pos == 2) {
			pos3->name = name;
		}
	}

	return;
}
void Switch::defineInputPin(int pos, uint8_t ip) {

	defineInputPin(this, pos, ip);
}
void Switch::defineOutputPins(int pos, uint8_t op) {

	defineOutputPins(this, pos, op);
}
void Switch::defineOutputPins(int pos, uint8_t op[], int size) {

	defineOutputPins(this, pos, op, size);
}
void Switch::defineInputPin(std::string pos_name, uint8_t ip) {

	defineInputPin(this, pos_name, ip);
}
void Switch::defineOutputPins(std::string pos_name, uint8_t op) {

	defineOutputPins(this, pos_name, op);
}
void Switch::defineOutputPins(std::string pos_name, uint8_t op[], int size) {

	defineOutputPins(this, pos_name, op, size);
}
void Switch::defineInputPin(uint8_t ip) {

	defineInputPin(this, ip);
}
void Switch::defineOutputPins(uint8_t op) {

	defineOutputPins(this, op);
}
void Switch::defineOutputPins(uint8_t op[], int size) {

	defineOutputPins(this, op, size);
}
void Switch::addAlarm(SwitchAlarm type) {

	addAlarm(this, type);

}
void Switch::addAlarm(SwitchAlarm type, int pos) {

	addAlarm(this, type, pos);
}
void Switch::addAlarm(SwitchAlarm type, std::string pos_name) {

	addAlarm(this, type, pos_name);
}
void Switch::deleteAlarm() {

	deleteAlarm(this);
}
void Switch::deleteAlarm(int pos) {

	deleteAlarm(this, pos);
}
void Switch::deleteAlarm(std::string pos_name) {

	deleteAlarm(this, pos_name);
}
void Switch::setAlarm(std::string pos_name, std::string on_time, std::string off_time) {

	setAlarm(this, pos_name, on_time, off_time);
}
void Switch::setAlarm(std::string pos_name, int mils, int on_time) {

	setAlarm(this, pos_name, mils, on_time);
}
void Switch::setAlarm(int pos, int mils, int on_time) {

	setAlarm(this, pos, mils, on_time);
}
void Switch::setAlarm(int pos, std::string on_time, std::string off_time) {

	setAlarm(this, pos, on_time, off_time);
}
void Switch::setAlarm(int mils, int on_time) {

	setAlarm(this, mils, on_time);
}
void Switch::setAlarm(std::string on_time, std::string off_time) {

	setAlarm(this, on_time, off_time);
}
void Switch::setAlarm(std::string clock_time, int on_time) {
	setAlarm(this, clock_time, on_time);
}
void Switch::setAlarm(std::string pos_name, std::string clock_time, int on_time) {
	setAlarm(this, pos_name, clock_time, on_time);
}
void Switch::setAlarm(int pos, std::string clock_time, int on_time) {
	setAlarm(this, pos, clock_time, on_time);
}
void Switch::toggleAlarm(std::string pos_name, bool on) {

	toggleAlarm(this, pos_name, on);
}
void Switch::toggleAlarm(int pos, bool on) {

	toggleAlarm(this, pos, on);
}
void Switch::toggleAlarm(bool on) {

	toggleAlarm(this, on);
}
void Switch::pause() {

	pause(this);
}
void Switch::resume() {
	resume(this);
}
uint8_t Switch::getInputState() {
	uint8_t ret;

	ret = getInputState(this);

	return ret;
}
uint8_t Switch::getInputState(std::string pos_name) {
	uint8_t ret;

	ret = getInputState(this, pos_name);

	return ret;
}
uint8_t Switch::getInputState(int pos) {
	uint8_t ret;

	ret = getInputState(this, pos);

	return ret;
}
uint8_t Switch::getOutputState(std::string pos_name) {
	uint8_t ret;

	ret = getOutputState(this, pos_name);

	return ret;
}
uint8_t Switch::getOutputState(int pos) {
	uint8_t ret;

	ret = getOutputState(this, pos);

	return ret;
}
void Switch::setInputActiveLevel(uint8_t level) {

	setInputActiveLevel(this, level);
}
void Switch::setOutputActiveLevel(uint8_t level) {

	setOutputActiveLevel(this, level);
}
void Switch::printSwitchStatus(int pos) {

	printSwitchStatus(this, pos);
}
void Switch::printSwitchStatus(std::string pos_name) {

	printSwitchStatus(this, pos_name);
}
void Switch::printSwitchStatus() {

	printSwitchStatus(this);
}
void Switch::trigger(int on_time) {

	trigger(this, on_time);
}
void Switch::trigger(int pos, int on_time) {

	trigger(this, pos, on_time);
}
void Switch::trigger(std::string pos_name, int on_time) {

	trigger(this, pos_name, on_time);
}
void Switch::triggerON() {

	triggerON(this);
}
void Switch::triggerON(int pos) {

	triggerON(this, pos);
}
void Switch::triggerON(std::string pos_name) {

	triggerON(this, pos_name);
}
void Switch::triggerOFF() {

	triggerOFF(this);
}
void Switch::triggerOFF(int pos) {

	triggerOFF(this, pos);
}
void Switch::triggerOFF(std::string pos_name) {

	triggerOFF(this, pos_name);
}


/*PRIVATE FUNCTIONS*/
void Switch::toggleOutputState(uint8_t lev) {

	toggleOutputState(this, lev);
}
void Switch::toggleOutputState(std::string pos_name, uint8_t lev) {

	toggleOutputState(this, pos_name, lev);
}
void Switch::toggleOutputState(int pos, uint8_t lev) {

	toggleOutputState(this, pos, lev);
}
