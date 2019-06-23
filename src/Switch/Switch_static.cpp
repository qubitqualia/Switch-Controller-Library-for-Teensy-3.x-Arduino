 /* Switch_static.cpp
 *
 * Copyright (c) 2019 by Justin Holland <jmatthew_7@yahoo.com>
 * Switch controller library for Teensy 3.x.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "Switch.h"
#include "Timer.h"
#include <algorithm>
#include <vector>

std::vector<Switch*> Switch::switch_list;

/*PUBLIC STATIC FUNCTIONS*/

//Initialization & Loop update functions
void Switch::init(Timer* t) {
	std::vector<Switch*>::iterator it;


	for (it=switch_list.begin();it!=switch_list.end();it++) {
		(*it)->timer = t;


		if ((*it)->pos1 != NULL) {
			if ((*it)->pos1->InputPin != 255) {
				if ((*it)->input_level == HIGH) {
					pinMode((*it)->pos1->InputPin, INPUT);
				}
				else if ((*it)->input_level == LOW) {
					pinMode((*it)->pos1->InputPin, INPUT_PULLUP);
				}
				(*it)->pos1->input_state = digitalRead((*it)->pos1->InputPin);
			}

			for (int i=0;i<10;i++) {
				if ((*it)->pos1->RelayPin[i] != 255) {
					pinMode((*it)->pos1->RelayPin[i], OUTPUT);
					if ((*it)->output_level == HIGH) {
						digitalWrite((*it)->pos1->RelayPin[i], LOW);
						toggleOutputState((*it), 0, LOW);
					}
					else if ((*it)->output_level == LOW) {
						digitalWrite((*it)->pos1->RelayPin[i], HIGH);
						toggleOutputState((*it), 0, HIGH);
					}
				}
			}
		}


		if ((*it)->pos2 != NULL) {
			if ((*it)->pos2->InputPin != 255) {
				if ((*it)->input_level == HIGH) {
					pinMode((*it)->pos2->InputPin, INPUT);
				}
				else if ((*it)->input_level == LOW) {
					pinMode((*it)->pos2->InputPin, INPUT_PULLUP);
				}

				(*it)->pos2->input_state = digitalRead((*it)->pos2->InputPin);
			}

			for (int i=0;i<10;i++) {
				if ((*it)->pos2->RelayPin[i] != 255) {
					pinMode((*it)->pos2->RelayPin[i], OUTPUT);
					if ((*it)->output_level == HIGH) {
						digitalWrite((*it)->pos2->RelayPin[i], LOW);
						toggleOutputState((*it), 1, LOW);
					}
					else if ((*it)->output_level == LOW) {
						digitalWrite((*it)->pos2->RelayPin[i], HIGH);
						toggleOutputState((*it), 1, HIGH);
					}
				}
			}
		}


		if ((*it)->pos3 != NULL) {
			if ((*it)->pos3->InputPin != 255) {
				if ((*it)->input_level == HIGH) {
					pinMode((*it)->pos3->InputPin, INPUT);
				}
				else if ((*it)->input_level == LOW) {
					pinMode((*it)->pos3->InputPin, INPUT_PULLUP);
				}

				(*it)->pos3->input_state = digitalRead((*it)->pos3->InputPin);
			}

			for (int i=0;i<10;i++) {
				if ((*it)->pos3->RelayPin[i] != 255) {
					pinMode((*it)->pos3->RelayPin[i], OUTPUT);
					if ((*it)->output_level == HIGH) {
						if (!(*it)->pos3->external_trigger) {
							digitalWrite((*it)->pos3->RelayPin[i], LOW);
							toggleOutputState((*it), 2, LOW);
						}
						else {
							digitalWrite((*it)->pos3->RelayPin[i], HIGH);
							toggleOutputState((*it), 2, HIGH);
						}
					}
					else if ((*it)->output_level == LOW) {
						digitalWrite((*it)->pos3->RelayPin[i], HIGH);
						toggleOutputState((*it), 2, HIGH);
					}

				}
			}
		}

	}
}
void Switch::update() {
	std::vector<Switch*>::iterator it;
	switch_list[0]->timer->update();

	if (!PAUSE) {

		for (it=switch_list.begin(); it!=switch_list.end(); it++) {

			if (!(*it)->pause_state) {
				//Update input states
				updateInputStates((*it));

				SwitchType sw_type = (*it)->sw_type;

				switch (sw_type) {

					case (SwitchType::SPST) : {
						handleSwitchPosition((*it), 0);
						break;
					}

					case (SwitchType::SPDT) : {
						handleSwitchPosition((*it), 0);
						handleSwitchPosition((*it), 1);
						break;
					}

					case (SwitchType::SP3T) : {
						handleSwitchPosition((*it), 0);
						handleSwitchPosition((*it), 1);
						handleSwitchPosition((*it), 2);
						break;
					}

				}
			}
		}
	}
}

//Switch Configuration functions
void Switch::_defineInputPin(std::string sw_name, int pos, uint8_t ip) {

	Switch* _switch = findSwitch(sw_name);

	defineInputPin(_switch, pos, ip);
}
void Switch::_defineOutputPins(std::string sw_name, int pos, uint8_t op) {

	Switch* _switch = findSwitch(sw_name);

	defineOutputPins(_switch, pos, op);
}
void Switch::_defineOutputPins(std::string sw_name, int pos, uint8_t op[], int size) {

	Switch* _switch = findSwitch(sw_name);

	defineOutputPins(_switch, pos, op, size);
}
void Switch::_defineInputPin(std::string sw_name, std::string pos_name, uint8_t ip) {

	Switch* _switch = findSwitch(sw_name);

	defineInputPin(_switch, pos_name, ip);

}
void Switch::_defineOutputPins(std::string sw_name, std::string pos_name, uint8_t op) {

	Switch* _switch = findSwitch(sw_name);

	defineOutputPins(_switch, pos_name, op);
}
void Switch::_defineOutputPins(std::string sw_name, std::string pos_name, uint8_t op[], int size) {

	Switch* _switch = findSwitch(sw_name);

	defineOutputPins(_switch, pos_name, op, size);
}
void Switch::_defineInputPin(std::string sw_name, uint8_t ip) {

	Switch* _switch = findSwitch(sw_name);

	defineInputPin(_switch, ip);
}
void Switch::_defineOutputPins(std::string sw_name, uint8_t op) {

	Switch* _switch = findSwitch(sw_name);

	defineOutputPins(_switch, op);
}
void Switch::_defineOutputPins(std::string sw_name, uint8_t op[], int size) {

	Switch* _switch = findSwitch(sw_name);

	defineOutputPins(_switch, op, size);
}
void Switch::_setInputActiveLevel(std::string sw_name, uint8_t level) {

	Switch* _switch = findSwitch(sw_name);

	setInputActiveLevel(_switch, level);

}
void Switch::_setOutputActiveLevel(std::string sw_name, uint8_t level) {

	Switch* _switch = findSwitch(sw_name);

	setOutputActiveLevel(_switch, level);
}
void Switch::_setInputActiveLevelAll(uint8_t level) {
	std::vector<Switch*>::iterator it;

	for (it=switch_list.begin(); it!=switch_list.end(); it++) {
		(*it)->input_level = level;
	}
}
void Switch::_setOutputActiveLevelAll(uint8_t level) {
	std::vector<Switch*>::iterator it;

	for (it=switch_list.begin(); it!=switch_list.end(); it++) {
		(*it)->output_level = level;
	}
}
void Switch::_addAlarm(SwitchAlarm type, std::string sw_name) {
	Switch* _switch = findSwitch(sw_name);

	addAlarm(_switch, type);
}
void Switch::_addAlarm(SwitchAlarm type, std::string sw_name, int pos) {

	Switch* _switch = findSwitch(sw_name);

	addAlarm(_switch, type, pos);
}
void Switch::_addAlarm(SwitchAlarm type, std::string sw_name, std::string pos_name) {

	Switch* _switch = findSwitch(sw_name);

	addAlarm(_switch, type, pos_name);
}
void Switch::_deleteAlarm(std::string sw_name) {

	Switch* _switch = findSwitch(sw_name);

	deleteAlarm(_switch);
}
void Switch::_deleteAlarm(std::string sw_name, int pos) {

	Switch* _switch = findSwitch(sw_name);

	deleteAlarm(_switch, pos);
}
void Switch::_deleteAlarm(std::string sw_name, std::string pos_name) {

	Switch* _switch = findSwitch(sw_name);

	deleteAlarm(_switch, pos_name);
}
void Switch::_setAlarm(std::string sw_name, int mils, int on_time) {

	Switch* _switch = findSwitch(sw_name);

	setAlarm(_switch, mils, on_time);
}
void Switch::_setAlarm(std::string sw_name, std::string on_time, std::string off_time) {

	Switch* _switch = findSwitch(sw_name);

	setAlarm(_switch, on_time, off_time);
}
void Switch::_setAlarm(std::string sw_name, std::string pos_name, int mils, int on_time) {

	Switch* _switch = findSwitch(sw_name);

	setAlarm(_switch, pos_name, mils, on_time);
}
void Switch::_setAlarm(std::string sw_name, std::string pos_name, std::string on_time, std::string off_time) {

	Switch* _switch = findSwitch(sw_name);

	setAlarm(_switch, pos_name, on_time, off_time);
}
void Switch::_setAlarm(std::string sw_name, int pos, int mils, int on_time) {
	Switch* _switch = findSwitch(sw_name);

	setAlarm(_switch, pos, mils, on_time);
}
void Switch::_setAlarm(std::string sw_name, int pos, std::string on_time, std::string off_time) {

	Switch* _switch = findSwitch(sw_name);

	setAlarm(_switch, pos, on_time, off_time);

}
void Switch::_setAlarm(std::string sw_name, std::string clock_time, int on_time) {

	Switch* _switch = findSwitch(sw_name);

	setAlarm(_switch, clock_time, on_time);
}
void Switch::_setAlarm(std::string sw_name, std::string pos_name, std::string clock_time, int on_time) {

	Switch* _switch = findSwitch(sw_name);

	setAlarm(_switch, pos_name, clock_time, on_time);
}
void Switch::_setAlarm(std::string sw_name, int pos, std::string clock_time, int on_time) {

	Switch* _switch = findSwitch(sw_name);

	setAlarm(_switch, pos, clock_time, on_time);
}


//Switch Control functions
void Switch::_toggleAlarm(std::string sw_name, bool on) {

	Switch* _switch = findSwitch(sw_name);

	toggleAlarm(_switch, on);
}
void Switch::_toggleAlarm(std::string sw_name, std::string pos_name, bool on) {

	Switch* _switch = findSwitch(sw_name);

	toggleAlarm(_switch, pos_name, on);
}
void Switch::_toggleAlarm(std::string sw_name, int pos, bool on) {

	Switch* _switch = findSwitch(sw_name);

	toggleAlarm(_switch, pos, on);
}
void Switch::_pause(std::string sw_name) {

	Switch* _switch = findSwitch(sw_name);

	pause(_switch);
}
void Switch::_resume(std::string sw_name) {

	Switch* _switch = findSwitch(sw_name);

	resume(_switch);
}
void Switch::_pauseAll() {
	std::vector<Switch*>::iterator it;
	PAUSE = true;

	for (it=switch_list.begin(); it!=switch_list.end(); it++) {
		(*it)->pause_state = true;
		if ((*it)->pos1->swatch != NULL) {
			(*it)->pos1->swatch->active = false;
		}
		else if ((*it)->pos1->toggler != NULL) {
			(*it)->pos1->toggler->active = false;
		}
		if ((*it)->pos2->swatch != NULL) {
			(*it)->pos2->swatch->active = false;
		}
		else if ((*it)->pos2->toggler != NULL) {
			(*it)->pos2->toggler->active = false;
		}
		if ((*it)->pos3->swatch != NULL) {
			(*it)->pos3->swatch->active = false;
		}
		else if ((*it)->pos3->toggler != NULL) {
			(*it)->pos3->toggler->active = false;
		}
	}
}
void Switch::_resumeAll() {
	std::vector<Switch*>::iterator it;
	PAUSE = false;

	for (it=switch_list.begin(); it!=switch_list.end(); it++) {
		(*it)->pause_state = false;
		if ((*it)->pos1->swatch != NULL) {
			(*it)->pos1->swatch->active = true;
		}
		else if ((*it)->pos1->toggler != NULL) {
			(*it)->pos1->toggler->active = true;
		}
		if ((*it)->pos2->swatch != NULL) {
			(*it)->pos2->swatch->active = true;
		}
		else if ((*it)->pos2->toggler != NULL) {
			(*it)->pos2->toggler->active = true;
		}
		if ((*it)->pos3->swatch != NULL) {
			(*it)->pos3->swatch->active = true;
		}
		else if ((*it)->pos3->toggler != NULL) {
			(*it)->pos3->toggler->active = true;
		}
	}
}
void Switch::_trigger(std::string sw_name, int on_time) {

	Switch* _switch = findSwitch(sw_name);

	trigger(_switch, on_time);
}
void Switch::_trigger(std::string sw_name, int pos, int on_time) {

	Switch* _switch = findSwitch(sw_name);

	trigger(_switch, pos, on_time);
}
void Switch::_trigger(std::string sw_name, std::string pos_name, int on_time) {

	Switch* _switch = findSwitch(sw_name);

	trigger(_switch, pos_name, on_time);
}
void Switch::_triggerON(std::string sw_name) {

	Switch* _switch = findSwitch(sw_name);

	triggerON(_switch);
}
void Switch::_triggerON(std::string sw_name, int pos) {

	Switch* _switch = findSwitch(sw_name);

	triggerON(_switch, pos);
}
void Switch::_triggerON(std::string sw_name, std::string pos_name) {

	Switch* _switch = findSwitch(sw_name);

	triggerON(_switch, pos_name);
}
void Switch::_triggerOFF(std::string sw_name) {

	Switch* _switch = findSwitch(sw_name);

	triggerOFF(_switch);
}
void Switch::_triggerOFF(std::string sw_name, int pos) {

	Switch* _switch = findSwitch(sw_name);

	triggerOFF(_switch, pos);
}
void Switch::_triggerOFF(std::string sw_name, std::string pos_name) {

	Switch* _switch = findSwitch(sw_name);

	triggerOFF(_switch, pos_name);
}


//Switch state getters
uint8_t Switch::_getInputState(std::string sw_name) {

	Switch* _switch = findSwitch(sw_name);
	uint8_t ret;

	ret = getInputState(_switch);

	return ret;
}
uint8_t Switch::_getInputState(std::string sw_name, std::string pos_name) {

	Switch* _switch = findSwitch(sw_name);
	uint8_t ret;

	ret = getInputState(_switch, pos_name);

	return ret;
}
uint8_t Switch::_getInputState(std::string sw_name, int pos) {
	Switch* _switch = findSwitch(sw_name);
	uint8_t ret;

	ret = getInputState(_switch, pos);

	return ret;
}
uint8_t Switch::_getOutputState(std::string sw_name, std::string pos_name) {
	Switch* _switch = findSwitch(sw_name);
	uint8_t ret;

	ret = getOutputState(_switch, pos_name);

	return ret;
}
uint8_t Switch::_getOutputState(std::string sw_name, int pos) {
	Switch* _switch = findSwitch(sw_name);
	uint8_t ret;

	ret = getOutputState(_switch, pos);

	return ret;
}
uint8_t Switch::_getOutputState(std::string sw_name) {
	Switch* _switch = findSwitch(sw_name);
	uint8_t ret;

	ret = getOutputState(_switch);

	return ret;
}

//Status functions
void Switch::_printSwitchStatus(std::string sw_name) {
	Switch* _switch = findSwitch(sw_name);

	printSwitchStatus(_switch);
}
void Switch::_printSwitchStatus(std::string sw_name, int pos) {
	Switch* _switch = findSwitch(sw_name);

	printSwitchStatus(_switch, pos);
}
void Switch::_printSwitchStatus(std::string sw_name, std::string pos_name) {
	Switch* _switch = findSwitch(sw_name);

	printSwitchStatus(_switch, pos_name);
}


/*PRIVATE STATIC FUNCTIONS*/

Switch* Switch::findSwitch(std::string name) {
	std::vector<Switch*>::iterator it;
	it = std::find_if(switch_list.begin(), switch_list.end(), [&name](const Switch* x)
	{
		return x->sw_name == name;
	});

	if (it == switch_list.end()) {
		(*it) = NULL;
	}

	return (*it);
}

void Switch::updateInputStates(Switch* _switch) {


	uint8_t lev;
	//Is switch SPST?
	if (_switch->sw_type == SwitchType::SPST) {
		//Does switch have Input Pin defined for Position 1?
		if (_switch->pos1->InputPin != 255) {
			lev = digitalRead(_switch->pos1->InputPin);
			_switch->pos1->input_state = lev;
		}
		else {
			if (_switch->pos1->external_trigger) {
				lev = LOW;
			}
			_switch->pos1->input_state = lev;
		}
	}
	//Is switch SPDT?
	else if (_switch->sw_type == SwitchType::SPDT) {

		//Both positions have input pins defined
		if (_switch->pos1->InputPin != 255 && _switch->pos2->InputPin != 255) {
			lev = digitalRead(_switch->pos1->InputPin);
			_switch->pos1->input_state = lev;
			lev = digitalRead(_switch->pos2->InputPin);
			_switch->pos2->input_state = lev;
		}

		//Neither position has input pin defined
		else if (_switch->pos2->InputPin == 255 && _switch->pos1->InputPin == 255) {
			_switch->pos1->input_state = LOW;
			_switch->pos2->input_state = LOW;
		}

		//Position 2 has input pin defined, position 1 not defined
		else if (_switch->pos1->InputPin == 255 && _switch->pos2->InputPin != 255) {
			if (digitalRead(_switch->pos2->InputPin) == HIGH && _switch->input_level == HIGH) {
				_switch->pos1->input_state = LOW;
				_switch->pos2->input_state = HIGH;
			}
			else if (digitalRead(_switch->pos2->InputPin) == HIGH && _switch->input_level == LOW) {
				_switch->pos1->input_state = LOW;
				_switch->pos2->input_state = HIGH;
			}
			else if (digitalRead(_switch->pos2->InputPin) == LOW && _switch->input_level == HIGH) {
				_switch->pos1->input_state = HIGH;
				_switch->pos2->input_state = LOW;
			}
			else if (digitalRead(_switch->pos2->InputPin) == LOW && _switch->input_level == LOW) {
				_switch->pos1->input_state = HIGH;
				_switch->pos2->input_state = LOW;
			}
		}

		//Position 1 has input pin defined, position 2 not defined
		else if (_switch->pos1->InputPin != 255 && _switch->pos2->InputPin == 255) {
			if (digitalRead(_switch->pos1->InputPin) == HIGH && _switch->input_level == HIGH) {
				_switch->pos2->input_state = LOW;
				_switch->pos1->input_state = HIGH;
			}
			else if (digitalRead(_switch->pos1->InputPin) == HIGH && _switch->input_level == LOW) {
				_switch->pos2->input_state = LOW;
				_switch->pos1->input_state = HIGH;
			}
			else if (digitalRead(_switch->pos1->InputPin) == LOW && _switch->input_level == HIGH) {
				_switch->pos2->input_state = HIGH;
				_switch->pos1->input_state = LOW;
			}
			else if (digitalRead(_switch->pos1->InputPin) == LOW && _switch->input_level == LOW) {
				_switch->pos2->input_state = HIGH;
				_switch->pos1->input_state = LOW;
			}
		}

	}
	//Is switch SP3T?
	else if (_switch->sw_type == SwitchType::SP3T) {
		uint8_t lev1, lev2, lev3;
		//Position 1 defined, position 2 undefined, position 3 undefined
		if (_switch->pos1->InputPin != 255 && _switch->pos2->InputPin == 255 && _switch->pos3->InputPin == 255) {
			lev1 = digitalRead(_switch->pos1->InputPin);
			_switch->pos1->input_state = lev1;
			_switch->pos2->input_state = LOW;
			_switch->pos3->input_state = LOW;
		}

		//Position 1 defined, position 2 defined, position 3 undefined
		else if (_switch->pos1->InputPin != 255 && _switch->pos2->InputPin != 255 && _switch->pos3->InputPin == 255) {
			lev1 = digitalRead(_switch->pos1->InputPin);
			lev2 = digitalRead(_switch->pos2->InputPin);
			_switch->pos1->input_state = lev1;
			_switch->pos2->input_state = lev2;
			if (lev1 != _switch->input_level && lev2 != _switch->input_level) {
				_switch->pos3->input_state = _switch->input_level;
			}
			else {
				if (_switch->input_level == HIGH) {
					_switch->pos3->input_state = LOW;
				}
				else {
					_switch->pos3->input_state = HIGH;
				}
			}
		}

		//Position 1 defined, position 2 defined, position 3 defined
		else if (_switch->pos1->InputPin != 255 && _switch->pos2->InputPin != 255 && _switch->pos3->InputPin != 255) {
			lev1 = digitalRead(_switch->pos1->InputPin);
			lev2 = digitalRead(_switch->pos2->InputPin);
			lev3 = digitalRead(_switch->pos3->InputPin);

			_switch->pos1->input_state = lev1;
			_switch->pos2->input_state = lev2;
			_switch->pos3->input_state = lev3;
		}

		//Position 1 undefined, position 2 defined, position 3 undefined
		else if (_switch->pos1->InputPin == 255 && _switch->pos2->InputPin != 255 && _switch->pos3->InputPin == 255) {
			lev2 = digitalRead(_switch->pos2->InputPin);

			_switch->pos2->input_state = lev2;
			_switch->pos1->input_state = LOW;
			_switch->pos3->input_state = LOW;
		}

		//Position 1 undefined, position 2 undefined, position 3 defined
		else if (_switch->pos1->InputPin == 255 && _switch->pos2->InputPin == 255 && _switch->pos3->InputPin != 255) {
			lev3 = digitalRead(_switch->pos3->InputPin);

			_switch->pos3->input_state = lev3;
			_switch->pos1->input_state = LOW;
			_switch->pos2->input_state = LOW;
		}
		//Position 1 undefined, position 2 defined, position 3 defined
		else if (_switch->pos1->InputPin == 255 && _switch->pos2->InputPin != 255 && _switch->pos3->InputPin != 255) {
			lev2 = digitalRead(_switch->pos2->InputPin);
			lev3 = digitalRead(_switch->pos3->InputPin);

			_switch->pos2->input_state = lev2;
			_switch->pos3->input_state = lev3;
			if (lev2 != _switch->input_level && lev3 != _switch->input_level) {
				_switch->pos1->input_state = _switch->input_level;
			}
			else {
				if (_switch->input_level == HIGH) {
					_switch->pos1->input_state = LOW;
				}
				else {
					_switch->pos1->input_state = HIGH;
				}
			}
		}
		//Position 1 defined, position 2 undefined, position 3 defined
		else if (_switch->pos1->InputPin != 255 && _switch->pos2->InputPin == 255 && _switch->pos3->InputPin != 255) {
			lev1 = digitalRead(_switch->pos1->InputPin);
			lev3 = digitalRead(_switch->pos3->InputPin);

			_switch->pos1->input_state = lev1;
			_switch->pos3->input_state = lev3;
			if (lev1 != _switch->input_level && lev3 != _switch->input_level) {
				_switch->pos2->input_state = _switch->input_level;
			}
			else {
				if (_switch->input_level == HIGH) {
					_switch->pos2->input_state = LOW;
				}
				else {
					_switch->pos2->input_state = HIGH;
				}
			}

		}
		//Position 1 undefined, position 2 undefined, position 3 undefined
		else if (_switch->pos1->InputPin == 255 && _switch->pos2->InputPin == 255 && _switch->pos3->InputPin == 255) {
			_switch->pos1->input_state = LOW;
			_switch->pos2->input_state = LOW;
			_switch->pos3->input_state = LOW;
		}

	}
}
void Switch::handleSwitchPosition(Switch* _switch, int pos) {
	Position* pos_ptr;

	//Get Position pointer
	if (pos == 0) {pos_ptr = _switch->pos1;}
	else if (pos == 1) {pos_ptr = _switch->pos2;}
	else {pos_ptr = _switch->pos3;}


	int op_count = 0;
	//Get number of Relay Pins
	for (int i=0; i<10; i++) {
		if (pos_ptr->RelayPin[i] != 255) {
			op_count++;
		}
	}

	//Handle external trigger receipt ONLY IF position input state is equal to active state
	if (pos_ptr->external_trigger) {
		if (pos_ptr->trigger_rcv) {
			pos_ptr->trigger_rcv = false;
			if (pos_ptr->input_state == _switch->input_level) {
				if (pos_ptr->external_trigger) {
					if (_switch->output_level == HIGH) {
						_switch->toggleOutputState(pos, HIGH);
						if (op_count != 0) {
							for (int i=0; i<op_count; i++) {
								digitalWrite(pos_ptr->RelayPin[i], HIGH);
							}
							if (pos_ptr->trigger_ontime != -1) {
								pos_ptr->trigger_in_progress = true;
								pos_ptr->trigger_timer = 0;
							}
						}
					}
					else {
						_switch->toggleOutputState(pos, LOW);
						if (op_count != 0) {
							for (int i=0; i<op_count; i++) {
								digitalWrite(pos_ptr->RelayPin[i], LOW);
							}
							if (pos_ptr->trigger_ontime != -1) {
								pos_ptr->trigger_in_progress = true;
								pos_ptr->trigger_timer = 0;
							}
						}
					}
				}
				else {
					printError(13);
				}
			}
			else {
				pos_ptr->trigger_rcv = false;
			}
		}

		//Handle in-progress external triggers
		if (pos_ptr->trigger_in_progress) {
			if (pos_ptr->trigger_timer >= pos_ptr->trigger_ontime) {
				pos_ptr->trigger_in_progress = false;
				if (_switch->getOutputState(pos) == HIGH) {
					_switch->toggleOutputState(pos, LOW);
					for (int i=0; i<op_count; i++) {
						digitalWrite(pos_ptr->RelayPin[i], LOW);
					}
				}
				else {
					_switch->toggleOutputState(pos, HIGH);
					for (int i=0; i<op_count; i++) {
						digitalWrite(pos_ptr->RelayPin[i], HIGH);
					}
				}
			}
		}
	}

	//Handle in-progress stopwatch triggers
	else if (pos_ptr->swatch_in_progress) {
		if (pos_ptr->swatch_timer >= pos_ptr->swatch_ontime) {
			pos_ptr->swatch_in_progress = false;
			if (_switch->getOutputState(pos) == HIGH) {
				_switch->toggleOutputState(pos, LOW);
				for (int i=0; i<op_count; i++) {
					digitalWrite(pos_ptr->RelayPin[i], LOW);
				}
			}
			else {
				_switch->toggleOutputState(pos, HIGH);
				for (int i=0; i<op_count; i++) {
					digitalWrite(pos_ptr->RelayPin[i], HIGH);
				}
			}
		}
	}

	//Handle switch position when it has ACTIVE Input
	else if (pos_ptr->input_state == _switch->input_level) {

		//Handle outputs when switch position is changed
		if (!pos_ptr->active) {
//Changed != to ==
			if (pos_ptr->swatch == NULL || pos_ptr->toggler == NULL) {
				uint8_t lev;
				if (_switch->output_level == HIGH) {
					lev = LOW;
				}
				else {
					lev = HIGH;
				}
				_switch->toggleOutputState(pos, lev);
				if (op_count != 0) {
					for (int i=0; i<op_count; i++) {
						digitalWrite(pos_ptr->RelayPin[i], lev);
					}
				}
			}
			pos_ptr->active = true;
		}

		//Handle switch position with attached Stopwatch alarm
		if (pos_ptr->swatch != NULL) {
			if (_switch->timer->isAlarming(pos_ptr->swatch)) {
				_switch->timer->ackAlarm(pos_ptr->swatch);
				if (_switch->output_level == LOW) {
					_switch->toggleOutputState(pos, LOW);
					if (op_count != 0) {
						for (int i=0; i<op_count; i++) {
							digitalWrite(pos_ptr->RelayPin[i], LOW);
						}
						pos_ptr->swatch_timer = 0;
						pos_ptr->swatch_in_progress = true;
					}
				}
				else {
					_switch->toggleOutputState(pos, HIGH);
					if (op_count != 0) {
						for (int i=0; i<op_count; i++) {
							digitalWrite(pos_ptr->RelayPin[i], HIGH);
						}
						pos_ptr->swatch_timer = 0;
						pos_ptr->swatch_in_progress = true;
					}
				}
			}
		}

		//Handle switch position with attached Toggler alarm
		else if (pos_ptr->toggler != NULL) {
			if (_switch->timer->isAlarming(pos_ptr->toggler) == 1) {
				_switch->timer->ackAlarm(pos_ptr->toggler);
				if (_switch->output_level == LOW) {
					_switch->toggleOutputState(pos, LOW);
					if (op_count != 0) {
						Serial.println("Writing LOW to relay");
						for (int i=0; i<op_count; i++) {
							Serial.print("Relay #");
							Serial.print(i);
							Serial.print(": ");
							Serial.println(pos_ptr->RelayPin[i]);
							digitalWrite(pos_ptr->RelayPin[i], LOW);
						}
					}
				}
				else {
					_switch->toggleOutputState(pos, HIGH);
					if (op_count != 0) {
						for (int i=0; i<op_count; i++) {
							digitalWrite(pos_ptr->RelayPin[i], HIGH);
						}
					}
				}
			}
			else if (_switch->timer->isAlarming(pos_ptr->toggler) == -1) {
				_switch->timer->ackAlarm(pos_ptr->toggler);
				if (_switch->output_level == LOW) {
					_switch->toggleOutputState(pos, HIGH);
					if (op_count != 0) {
						for (int i=0; i<op_count; i++) {
							digitalWrite(pos_ptr->RelayPin[i], HIGH);
						}
					}
				}
				else {
					_switch->toggleOutputState(pos, LOW);
					if (op_count != 0) {
						for (int i=0; i<op_count; i++) {
							digitalWrite(pos_ptr->RelayPin[i], LOW);
						}
					}
				}
			}
		}

		//Handle switch position with NO alarm attached
		else {
			if (_switch->output_level == LOW) {
				_switch->toggleOutputState(pos, LOW);
				if (op_count != 0) {
					for (int i=0; i<op_count; i++) {
						digitalWrite(pos_ptr->RelayPin[i], LOW);
					}
				}
			}
			else {
				_switch->toggleOutputState(pos, HIGH);
				if (op_count != 0) {
					for (int i=0; i<op_count; i++) {
						digitalWrite(pos_ptr->RelayPin[i], HIGH);
					}
				}
			}
		}
	}

	//Handle switch position when it does NOT have ACTIVE Input
	else {
		pos_ptr->active = false;
		bool pass = isDormant(pos_ptr);

		if (!pass) {

			if (_switch->output_level == LOW) {
				_switch->toggleOutputState(pos, HIGH);
				if (op_count != 0) {
					for (int i=0; i<op_count; i++) {
						digitalWrite(pos_ptr->RelayPin[i], HIGH);
					}
				}
			}
			else {
				_switch->toggleOutputState(pos, LOW);
				if (op_count != 0) {
					for (int i=0; i<op_count; i++) {
						digitalWrite(pos_ptr->RelayPin[i], LOW);
					}
				}
			}
		}
	}

}
bool Switch::isDormant(Position* pos) {
	std::vector<Switch*>::iterator it;
	bool pass = false;

	//Check all switches against pos to see if they share same relay pin
	//If relay pin is shared, pos has to become dormant so that it does not
	//interfere with intended operation
	//e.g. pos = "Day Light", RelayPin = 28
	//     (*it)->pos = "Timer", RelayPin = 28
	//	   pos --> dormant (i.e. does not modify output state of relay pin)
	for (it=switch_list.begin(); it!=switch_list.end(); it++) {
		for (int i=0;i<10;i++) {
			Position* pos1 = (*it)->pos1;
			Position* pos2 = (*it)->pos2;
			Position* pos3 = (*it)->pos3;
			uint8_t pos1_pin = (*it)->pos1->RelayPin[i];
			uint8_t pos2_pin = (*it)->pos2->RelayPin[i];
			uint8_t pos3_pin = (*it)->pos3->RelayPin[i];
			uint8_t posx_pin = pos->RelayPin[i];

			if (pos1 != pos) {
				if (pos1_pin == posx_pin && posx_pin != 255) {
					pass = true;
				}
			}

			if (pos2 != pos) {
				if (pos2_pin == posx_pin && posx_pin != 255) {
					pass = true;
				}
			}

			if (pos3 != pos) {
				if (pos3_pin == posx_pin && posx_pin != 255) {
					pass = true;
				}
			}

		}
	 }

	return pass;
}
void Switch::defineInputPin(Switch* _switch, uint8_t ip) {

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else {
			_switch->pos1->InputPin = ip;
		}
	}
}
void Switch::defineOutputPins(Switch* _switch, uint8_t op) {

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else {
			_switch->pos1->RelayPin[0] = op;
		}
	}
}
void Switch::defineOutputPins(Switch* _switch, uint8_t op[], int size) {

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else {
			for (int i=0; i<size; i++) {
				_switch->pos1->RelayPin[i] = op[i];
			}
		}
	}
}
void Switch::defineInputPin(Switch* _switch, int pos, uint8_t ip) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {
			pos_ptr->InputPin = ip;
		}
	}
}
void Switch::defineOutputPins(Switch* _switch, int pos, uint8_t op) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {
			pos_ptr->RelayPin[0] = op;
		}
	}
}
void Switch::defineOutputPins(Switch* _switch, int pos, uint8_t op[], int size) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {
			for (int i=0; i<size; i++) {
				pos_ptr->RelayPin[i] = op[i];
			}
		}
	}
}
void Switch::defineInputPin(Switch* _switch, std::string pos_name, uint8_t ip) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		pos_ptr->InputPin = ip;
	}
}
void Switch::defineOutputPins(Switch* _switch, std::string pos_name, uint8_t op) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		pos_ptr->RelayPin[0] = op;
	}
}
void Switch::defineOutputPins(Switch* _switch, std::string pos_name, uint8_t op[], int size) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		for (int i=0; i<size; i++) {
			pos_ptr->RelayPin[i] = op[i];
		}
	}
}
void Switch::addAlarm(Switch* _switch, SwitchAlarm type) {

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else {
			if (type == SwitchAlarm::Stopwatch_Interval || type == SwitchAlarm::Stopwatch_Clock) {
				if (_switch->pos1->toggler == NULL) {
					_switch->pos1->swatch = _switch->timer->createStopWatch();
				}
				else {
					printError(10);
				}
			}
			else if (type == SwitchAlarm::Toggler) {
				if (_switch->pos1->swatch == NULL) {
					_switch->pos1->toggler = _switch->timer->createToggler();
				}
				else {
					printError(10);
				}
			}
			else if (type == SwitchAlarm::External) {
				_switch->pos1->external_trigger = true;
			}
		}
	}
}
void Switch::addAlarm(Switch* _switch, SwitchAlarm type, int pos) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {
			if (type == SwitchAlarm::Stopwatch_Interval || type == SwitchAlarm::Stopwatch_Clock) {
				if (pos_ptr->toggler == NULL) {
					pos_ptr->swatch = _switch->timer->createStopWatch();
				}
				else {
					printError(10);
				}
			}
			else if (type == SwitchAlarm::Toggler) {
				if (pos_ptr->swatch == NULL) {
					pos_ptr->toggler = _switch->timer->createToggler();
				}
				else {
					printError(10);
				}
			}
			else if (type == SwitchAlarm::External) {
				pos_ptr->external_trigger = true;
			}
		}
	}
}
void Switch::addAlarm(Switch* _switch, SwitchAlarm type, std::string pos_name) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		if (type == SwitchAlarm::Stopwatch_Interval || type == SwitchAlarm::Stopwatch_Clock) {
			if (pos_ptr->toggler == NULL) {
				pos_ptr->swatch = _switch->timer->createStopWatch();
			}
			else {
				printError(10);
			}
		}
		else if (type == SwitchAlarm::Toggler) {
			if (pos_ptr->swatch == NULL) {
				pos_ptr->toggler = _switch->timer->createToggler();
			}
			else {
				printError(10);
			}
		}
		else if (type == SwitchAlarm::External) {
			pos_ptr->external_trigger = true;
		}
	}
}
void Switch::deleteAlarm(Switch* _switch) {
	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else {
			if (_switch->pos1->swatch != NULL) {
				_switch->timer->deleteAlarm(_switch->pos1->swatch);
				_switch->pos1->swatch = NULL;
			}
			else if (_switch->pos1->toggler != NULL) {
				_switch->timer->deleteAlarm(_switch->pos1->toggler);
				_switch->pos1->toggler = NULL;
			}
		}
	}
}
void Switch::deleteAlarm(Switch* _switch, int pos) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {
			if (pos_ptr->swatch != NULL) {
				_switch->timer->deleteAlarm(pos_ptr->swatch);
				pos_ptr->swatch = NULL;
			}
			else if (pos_ptr->toggler != NULL) {
				_switch->timer->deleteAlarm(pos_ptr->toggler);
				pos_ptr->toggler = NULL;
			}
		}
	}
}
void Switch::deleteAlarm(Switch* _switch, std::string pos_name) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {

		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		if (pos_ptr-> swatch != NULL) {
			_switch->timer->deleteAlarm(pos_ptr->swatch);
			pos_ptr->swatch = NULL;
		}
		else if (pos_ptr->toggler != NULL) {
			_switch->timer->deleteAlarm(pos_ptr->toggler);
			pos_ptr->toggler = NULL;
		}
	}
}
void Switch::setAlarm(Switch* _switch, int mils, int on_time) {

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else {
			_switch->pos1->swatch->interval = mils;
			_switch->pos1->swatch->active = true;
			_switch->pos1->swatch_ontime = on_time;
		}
	}
}
void Switch::setAlarm(Switch* _switch, std::string on_time, std::string off_time) {

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else {
			_switch->pos1->toggler->ON_time = on_time;
			_switch->pos1->toggler->OFF_time = off_time;
			_switch->pos1->toggler->active = true;
		}
	}
}
void Switch::setAlarm(Switch* _switch, int pos, int mils, int on_time) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {

			if (pos_ptr->swatch == NULL) {
				printError(9);
			}
			else {
				pos_ptr->swatch->interval = mils;
				pos_ptr->swatch->active = true;
				pos_ptr->swatch_ontime = on_time;
			}
		}
	}
}
void Switch::setAlarm(Switch* _switch, std::string pos_name, int mils, int on_time) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		if (pos_ptr->swatch == NULL) {
			printError(9);
		}
		else {
			pos_ptr->swatch->interval = mils;
			pos_ptr->swatch->active = true;
			pos_ptr->swatch_ontime = on_time;
		}
	}
}
void Switch::setAlarm(Switch* _switch, int pos, std::string on_time, std::string off_time) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {
			if (pos_ptr->toggler == NULL) {
				printError(8 );
			}
			else {
				pos_ptr->toggler->ON_time = on_time;
				pos_ptr->toggler->OFF_time = off_time;
				pos_ptr->toggler->active = true;
			}
		}
	}
}
void Switch::setAlarm(Switch* _switch, std::string pos_name, std::string on_time, std::string off_time) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		if (pos_ptr->toggler == NULL) {
			printError(8);
		}
		else {
			pos_ptr->toggler->ON_time = on_time;
			pos_ptr->toggler->OFF_time = off_time;
			pos_ptr->toggler->active = true;
		}
	}
}
void Switch::setAlarm(Switch* _switch, std::string clock_time, int on_time) {

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else {
			_switch->pos1->swatch->clock_time = clock_time;
			_switch->pos1->swatch->active = true;
			_switch->pos1->swatch_ontime = on_time;
		}
	}
}
void Switch::setAlarm(Switch* _switch, std::string pos_name, std::string clock_time, int on_time) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		if (pos_ptr->swatch == NULL) {
			printError(9);
		}
		else {
			pos_ptr->swatch->clock_time = clock_time;
			pos_ptr->swatch->active = true;
			pos_ptr->swatch_ontime = on_time;
		}
	}
}
void Switch::setAlarm(Switch* _switch, int pos, std::string clock_time, int on_time) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {

			if (pos_ptr->swatch == NULL) {
				printError(9);
			}
			else {
				pos_ptr->swatch->clock_time = clock_time;
				pos_ptr->swatch->active = true;
				pos_ptr->swatch_ontime = on_time;
			}
		}
	}
}
void Switch::toggleAlarm(Switch* _switch, bool on) {

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else {
			if (_switch->pos1->swatch != NULL) {
				_switch->pos1->swatch->active = on;
			}
			else if (_switch->pos1->toggler != NULL) {
				_switch->pos1->toggler->active = on;
			}
			else {
				printError(7);
			}
		}
	}
}
void Switch::toggleAlarm(Switch* _switch, int pos, bool on) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {
			if (pos_ptr->swatch != NULL) {
				pos_ptr->swatch->active = on;
			}
			else if (pos_ptr->toggler != NULL) {
				pos_ptr->toggler->active = on;
			}
			else {
				printError(7);
			}
		}
	}
}
void Switch::toggleAlarm(Switch* _switch, std::string pos_name, bool on) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		if (pos_ptr->swatch != NULL) {
			pos_ptr->swatch->active = on;
		}
		else if (pos_ptr->toggler != NULL) {
			pos_ptr->toggler->active = on;
		}
		else {
			printError(7);
		}
	}
}
void Switch::pause(Switch* _switch) {

	if (_switch == NULL) {
		printError(0);
	}
	else {
		_switch->pause_state = true;

		if (_switch->pos1 != NULL) {
			if (_switch->pos1->swatch != NULL) {
				_switch->pos1->swatch->active = false;
			}
			else if (_switch->pos1->toggler != NULL) {
				_switch->pos1->toggler->active = false;
			}
		}
		if (_switch->pos2 != NULL) {
			if (_switch->pos2->swatch != NULL) {
				_switch->pos2->swatch->active = false;
			}
			else if (_switch->pos2->toggler != NULL) {
				_switch->pos2->toggler->active = false;
			}
		}
		if (_switch->pos3 != NULL) {
			if (_switch->pos3->swatch != NULL) {
				_switch->pos3->swatch->active = false;
			}
			else if (_switch->pos3->toggler != NULL) {
				_switch->pos3->toggler->active = false;
			}
		}
	}
}
void Switch::resume(Switch* _switch) {

	if (_switch == NULL) {
		printError(0);
	}
	else {
		_switch->pause_state = false;

		if (_switch->pos1 != NULL) {
			if (_switch->pos1->swatch != NULL) {
				_switch->pos1->swatch->active = true;
			}
			else if (_switch->pos1->toggler != NULL) {
				_switch->pos1->toggler->active = true;
			}
		}
		if (_switch->pos2 != NULL) {
			if (_switch->pos2->swatch != NULL) {
				_switch->pos2->swatch->active = true;
			}
			else if (_switch->pos2->toggler != NULL) {
				_switch->pos2->toggler->active = true;
			}
		}
		if (_switch->pos3 != NULL) {
			if (_switch->pos3->swatch != NULL) {
				_switch->pos3->swatch->active = true;
			}
			else if (_switch->pos3->toggler != NULL) {
				_switch->pos3->toggler->active = true;
			}
		}
	}
}
uint8_t Switch::getInputState(Switch* _switch) {
	uint8_t ret;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else {

			ret = _switch->pos1->input_state;
		}
	}

	return ret;
}
uint8_t Switch::getInputState(Switch* _switch, std::string pos_name) {
	Position* pos_ptr;
	uint8_t ret;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		ret = pos_ptr->input_state;
	}

	return ret;
}
uint8_t Switch::getInputState(Switch* _switch, int pos) {
	uint8_t ret;
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {
			ret = pos_ptr->input_state;
		}

	}

	return ret;
}
uint8_t Switch::getOutputState(Switch* _switch) {
	uint8_t ret;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else {
			ret = _switch->pos1->output_state;
		}
	}

	return ret;
}
uint8_t Switch::getOutputState(Switch* _switch, std::string pos_name) {
	Position* pos_ptr;
	uint8_t ret;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		ret = pos_ptr->output_state;
	}

	return ret;
}
uint8_t Switch::getOutputState(Switch* _switch, int pos) {
	Position* pos_ptr;
	uint8_t ret;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {
			ret = pos_ptr->output_state;
		}
	}

	return ret;

}
void Switch::toggleOutputState(Switch* _switch, uint8_t lev) {

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else {
			_switch->pos1->output_state = lev;
		}
	}
}
void Switch::toggleOutputState(Switch* _switch, int pos, uint8_t lev) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {
			pos_ptr->output_state = lev;
		}
	}
}
void Switch::toggleOutputState(Switch* _switch, std::string pos_name, uint8_t lev) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		pos_ptr->output_state = lev;

	}
}
void Switch::_toggleOutputStateAll(uint8_t lev) {
	std::vector<Switch*>::iterator it;

	for (it=switch_list.begin(); it!=switch_list.end(); it++) {
		if ((*it)->pos1 != NULL) {
			(*it)->pos1->output_state = lev;
		}
		if ((*it)->pos2 != NULL) {
			(*it)->pos2->output_state = lev;
		}
		if ((*it)->pos3 != NULL) {
			(*it)->pos3->output_state = lev;
		}
	}
}
void Switch::setInputActiveLevel(Switch* _switch, uint8_t level) {

	if (_switch == NULL) {
		printError(0);
	}
	else {
		_switch->input_level = level;
	}
}
void Switch::setOutputActiveLevel(Switch* _switch, uint8_t level) {

	if (_switch == NULL) {
		printError(0);
	}
	else {
		_switch->output_level = level;
	}
}
void Switch::printSwitchStatus(Switch* _switch) {

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type == SwitchType::SPST) {
			printSwitchStatus(_switch, 0);
		}
		else if (_switch->sw_type == SwitchType::SPDT) {
			printSwitchStatus(_switch, 0);
			printSwitchStatus(_switch, 1);

		}
		else if (_switch->sw_type == SwitchType::SP3T) {
			printSwitchStatus(_switch, 0);
			printSwitchStatus(_switch, 1);
			printSwitchStatus(_switch, 2);
		}
	}
}
void Switch::printSwitchStatus(Switch* _switch, std::string pos_name) {

	int c = 0;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {c = 0;}
		else if (_switch->pos2->name == pos_name) {c = 1;}
		else if (_switch->pos3->name == pos_name) {c = 2;}
		else {printError(1);}

		printSwitchStatus(_switch, c);

	}
}
void Switch::_printSwitchStatusAll() {
	std::vector<Switch*>::iterator it;

	for (it=switch_list.begin(); it!=switch_list.end(); it++) {
		if ((*it)->sw_type == SwitchType::SPST) {
			printSwitchStatus((*it), 0);
		}
		else if ((*it)->sw_type == SwitchType::SPDT) {
			printSwitchStatus((*it), 0);
			printSwitchStatus((*it), 1);
		}
		else if ((*it)->sw_type == SwitchType::SP3T) {
			printSwitchStatus((*it), 0);
			printSwitchStatus((*it), 1);
			printSwitchStatus((*it), 2);
		}
	}

}
void Switch::printSwitchStatus(Switch* _switch, int pos) {
	Position* pos_ptr;
	int c;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1; c = 1;}
		else if (pos == 1) {pos_ptr = _switch->pos2; c = 2;}
		else if (pos == 2) {pos_ptr = _switch->pos3; c = 3;}
		else {printError(2);}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {
			Serial.println(F("***************SWITCH STATUS***************"));
			Serial.print(F("Switch Name: "));
			if (_switch->sw_name == "") {Serial.println(F("Undefined"));}
			else {Serial.println(_switch->sw_name.c_str());}
			Serial.print(F("POSITION #"));
			Serial.print(c);
			Serial.println(F(": "));
			Serial.print(F("Name: "));
			if (pos_ptr->name == "") {Serial.println(F("Undefined"));}
			else {Serial.println(pos_ptr->name.c_str());}
			Serial.print(F("Input Pin: "));
			Serial.print(pos_ptr->InputPin);
			Serial.print(F("  , Input State: "));
			if (pos_ptr->input_state == HIGH) {
				Serial.println("HIGH");
			}
			else if (pos_ptr->input_state == LOW) {
				Serial.println("LOW");
			}
			else {
				Serial.println("Undef.");
			}
			Serial.print(F("Output Pins: "));
			for (int i=0;i<10;i++) {
				if (pos_ptr->RelayPin[i] != 255) {
					Serial.print(pos_ptr->RelayPin[i]);
					Serial.print(",");
				}
			}
			Serial.print(F("  , Output State: "));
			if (pos_ptr->output_state == HIGH) {
				Serial.println("HIGH");
			}
			else if (pos_ptr->output_state == LOW) {
				Serial.println("LOW");
			}
			else {
				Serial.println("Undef.");
			}
			Serial.println();
			Serial.print(F("Alarm: "));
			if (pos_ptr->swatch != NULL) {
				if (pos_ptr->swatch->clock_time == "") {
					Serial.print(F("Stopwatch_Interval"));
					Serial.print(F(" , "));
					Serial.print(F("Current Time: "));
					Serial.print(_switch->timer->getCurrentSysTime_tm());
					Serial.print(F(" , "));
					Serial.print(F("Next Alarm: "));
					Serial.println(pos_ptr->swatch->talarm);
					Serial.println();
				}
				else {
					Serial.print(F("Stopwatch_Clock"));
					Serial.print(F(" , "));
					Serial.print(F("Current Time: "));
					Serial.print(_switch->timer->getCurrentSysTime_tm());
					Serial.print(F(" , "));
					Serial.print(F("Next Alarm: "));
					Serial.print(pos_ptr->swatch->talarm);
					Serial.print(F(" , "));
					Serial.print(F("Interval: "));
					Serial.println(pos_ptr->swatch->interval);
					Serial.println();

				}
			}
			else if (pos_ptr->toggler != NULL) {
				Serial.print(F("Toggler"));
				Serial.print(F(" , "));
				Serial.print(F("Current Time: "));
				Serial.print(_switch->timer->getCurrentTime_str().c_str());
				Serial.print(F(" , "));
				Serial.print(F("Next On time: "));
				Serial.print(pos_ptr->toggler->ON_time.c_str());
				Serial.print(F(" , "));
				Serial.print(F("Next Off time: "));
				Serial.print(pos_ptr->toggler->OFF_time.c_str());
				Serial.println();
				Serial.print("Alarm status: ");
				Serial.println(_switch->timer->isAlarming(pos_ptr->toggler));

			}
			else {
				Serial.println(F("None"));
			}



		}

	}
}
void Switch::printError(int errno) {
	Serial.print(F("Switch Error "));
	if (errno == 0) {
		Serial.print(F("0: "));
		Serial.println(F("Specified switch name not found in static vector."));
	}
	else if (errno == 1) {
		Serial.print(F("1: "));
		Serial.println(F("Invalid position name."));
	}
	else if (errno == 2) {
		Serial.print(F("2: "));
		Serial.println(F("Invalid position number."));
	}
	else if (errno == 3) {
		Serial.print(F("3: "));
		Serial.println(F("Specified position name is undefined."));
	}
	else if (errno == 4) {
		Serial.print(F("4: "));
		Serial.println(F("Specified position number is undefined."));
	}
	else if (errno == 5) {
		Serial.print(F("5: "));
		Serial.println(F("Incorrect function called for non-SPST switch."));
	}
	else if (errno == 6) {
		Serial.print(F("6: "));
		Serial.println(F("Alarm not defined for specified switch position."));
	}
	else if (errno == 7) {
		Serial.print(F("7: "));
		Serial.println(F("No alarm defined for switch position."));
	}
	else if (errno == 8) {
		Serial.print(F("8: "));
		Serial.println(F("No Toggler instance has been defined for switch position."));
	}
	else if (errno == 9) {
		Serial.print(F("9: "));
		Serial.println(F("No Stopwatch instance has been defined for switch position."));
	}
	else if (errno == 10) {
		Serial.print(F("10: "));
		Serial.println(F("Alarm already defined for specified switch position."));
	}
	else if (errno == 11) {
		Serial.print(F("11: "));
		Serial.println(F("Incorrect class constructor called for SPST switch"));
	}
	else if (errno == 12) {
		Serial.print(F("12: "));
		Serial.println(F("Incorrect class constructor called for SPDT switch"));
	}
	else if (errno == 13) {
		Serial.print(F("13: "));
		Serial.println(F("Switch position is not set to external trigger mode"));
	}



}
void Switch::trigger(Switch* _switch, int on_time) {
	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else if (!_switch->pos1->external_trigger) {
			printError(13);
		}
		else {
			_switch->pos1->trigger_rcv = true;
			_switch->pos1->trigger_ontime = on_time;
		}
	}
}
void Switch::trigger(Switch* _switch, int pos, int on_time) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {
			if (!pos_ptr->external_trigger) {
				printError(13);
			}
			else {
				pos_ptr->trigger_rcv = true;
				pos_ptr->trigger_ontime = on_time;
			}
		}
	}

}
void Switch::trigger(Switch* _switch, std::string pos_name, int on_time) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		if (!pos_ptr->external_trigger) {
			printError(13);
		}
		else {
			pos_ptr->trigger_rcv = true;
			pos_ptr->trigger_ontime = on_time;
		}
	}
}
void Switch::triggerON(Switch* _switch) {
	int op_count = 0;

	for (int i=0;i<10;i++) {
		if (_switch->pos1->RelayPin[i] != 255) {
			op_count++;
		}
	}

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else if (!_switch->pos1->external_trigger) {
			printError(13);
		}
		else {
			if (_switch->output_level == LOW) {
				toggleOutputState(_switch, LOW);
				if (op_count != 0) {
					for (int i=0;i<op_count;i++) {
						digitalWrite(_switch->pos1->RelayPin[i], LOW);
					}
				}

			}
			else {
				toggleOutputState(_switch, HIGH);
				if (op_count != 0) {
					for (int i=0;i<op_count;i++) {
						digitalWrite(_switch->pos1->RelayPin[i], HIGH);
					}
				}
			}
		}
	}
}
void Switch::triggerON(Switch* _switch, int pos) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		int op_count = 0;

		for (int i=0;i<10;i++) {
			if (pos_ptr->RelayPin[i] != 255) {
				op_count++;
			}
		}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {
			if (!pos_ptr->external_trigger) {
				printError(13);
			}
			else {
				if (_switch->output_level == LOW) {
					toggleOutputState(_switch, pos, LOW);
					if (op_count != 0) {
						for (int i=0;i<op_count;i++) {
							digitalWrite(pos_ptr->RelayPin[i], LOW);
						}
					}
				}
				else {
					toggleOutputState(_switch, pos, HIGH);
					if (op_count != 0) {
						for (int i=0;i<op_count;i++) {
							digitalWrite(pos_ptr->RelayPin[i], HIGH);
						}
					}
				}
			}
		}
	}
}
void Switch::triggerON(Switch* _switch, std::string pos_name) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		int op_count = 0;

		for (int i=0;i<10;i++) {
			if (pos_ptr->RelayPin[i] != 255) {
				op_count++;
			}
		}

		if (!pos_ptr->external_trigger) {
			printError(13);
		}
		else {
			if (_switch->output_level == LOW) {
				toggleOutputState(_switch, pos_name, LOW);
				if (op_count != 0) {
					for (int i=0;i<op_count;i++) {
						digitalWrite(pos_ptr->RelayPin[i], LOW);
					}
				}
			}
			else {
				toggleOutputState(_switch, pos_name, HIGH);
				if (op_count != 0) {
					for (int i=0;i<op_count;i++) {
						digitalWrite(pos_ptr->RelayPin[i], HIGH);
					}
				}
			}

		}
	}
}
void Switch::triggerOFF(Switch* _switch) {

	int op_count = 0;

	for (int i=0;i<10;i++) {
		if (_switch->pos1->RelayPin[i] != 255) {
			op_count++;
		}
	}

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->sw_type != SwitchType::SPST) {
			printError(5);
		}
		else if (!_switch->pos1->external_trigger) {
			printError(13);
		}
		else {
			if (_switch->output_level == LOW) {
				toggleOutputState(_switch, HIGH);
				if (op_count != 0) {
					for (int i=0;i<op_count;i++) {
						digitalWrite(_switch->pos1->RelayPin[i], HIGH);
					}
				}
			}
			else {
				toggleOutputState(_switch, LOW);
				if (op_count != 0) {
					for (int i=0;i<op_count;i++) {
						digitalWrite(_switch->pos1->RelayPin[i], LOW);
					}
				}
			}
		}
	}
}
void Switch::triggerOFF(Switch* _switch, int pos) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (pos == 0) {pos_ptr = _switch->pos1;}
		else if (pos == 1) {pos_ptr = _switch->pos2;}
		else if (pos == 2) {pos_ptr = _switch->pos3;}
		else {printError(2);}

		int op_count = 0;

		for (int i=0;i<10;i++) {
			if (pos_ptr->RelayPin[i] != 255) {
				op_count++;
			}
		}

		if (pos_ptr == NULL) {
			printError(4);
		}
		else {
			if (!pos_ptr->external_trigger) {
				printError(13);
			}
			else {
				if (_switch->output_level == LOW) {
					toggleOutputState(_switch, pos, HIGH);
					if (op_count != 0) {
						for (int i=0;i<op_count;i++) {
							digitalWrite(pos_ptr->RelayPin[i], HIGH);
						}
					}
				}
				else {
					toggleOutputState(_switch, pos, LOW);
					if (op_count != 0) {
						for (int i=0;i<op_count;i++) {
							digitalWrite(pos_ptr->RelayPin[i], LOW);
						}
					}
				}
			}
		}
	}
}
void Switch::triggerOFF(Switch* _switch, std::string pos_name) {
	Position* pos_ptr;

	if (_switch == NULL) {
		printError(0);
	}
	else {
		if (_switch->pos1->name == pos_name) {pos_ptr = _switch->pos1;}
		else if (_switch->pos2->name == pos_name) {pos_ptr = _switch->pos2;}
		else if (_switch->pos3->name == pos_name) {pos_ptr = _switch->pos3;}
		else {printError(1);}

		int op_count = 0;

		for (int i=0;i<10;i++) {
			if (pos_ptr->RelayPin[i] != 255) {
				op_count++;
			}
		}

		if (!pos_ptr->external_trigger) {
			printError(13);
		}
		else {
			if (_switch->output_level == LOW) {
				toggleOutputState(_switch, pos_name, HIGH);
				if (op_count != 0) {
					for (int i=0;i<op_count;i++) {
						digitalWrite(pos_ptr->RelayPin[i], HIGH);
					}
				}
			}
			else {
				toggleOutputState(_switch, pos_name, LOW);
				if (op_count != 0) {
					for (int i=0;i<op_count;i++) {
						digitalWrite(pos_ptr->RelayPin[i], LOW);
					}
				}
			}

		}
	}
}

