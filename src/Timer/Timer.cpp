/*
 * Timer.cpp
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

#include "Timer.h"
#include "Arduino.h"
#include <algorithm>
#include <vector>
#include <string>
#include <ctime>
#include <cmath>


const char* Timer::tzone = "Eastern";
volatile bool Timer::dst = true;
int Timer::utc_offset = 0;
std::vector<Stopwatch*> Timer::stop_watch_list;
std::vector<ToggleTimer*> Timer::toggler_list;

Timer::Timer()
{
	current_time = 0;		//secs
	current_sys_time = 0;	//millis
	internal_timer = 0;
	system_update_interval = 20;
	time_update_interval = 1000;
	waiting_for_sync = false;
	ton = new tm();
	toff = new tm();
	timeout = 30000;
	ss = 0;
	hh = 0;
	mm = 0;
	MM = 0;
	DD = 0;
	YY = 0;
}

/*PUBLIC FUNCTIONS*/

void Timer::setTimeZone(std::string tz) {
//US timezones only!

	if (tz == "Eastern") {
		tzone = "Eastern";
		utc_offset = 5;
	}
	else if (tz == "Central") {
		tzone = "Central";
		utc_offset = 6;
	}
	else if (tz == "Mountain") {
		tzone = "Mountain";
		utc_offset = 7;
	}
	else if (tz == "Pacific") {
		tzone = "Pacific";
		utc_offset = 8;
	}
	else if (tz == "Alaska") {
		tzone = "Alaska";
		utc_offset = 9;
	}
	else if (tz == "Hawaii") {
		tzone = "Hawaii";
		utc_offset = 10;
	}
	else {
		Serial.println("Timer Error: Invalid timezone argument");
	}
}
void Timer::setTimeZone(int offset) {
	utc_offset = offset;
}
void Timer::setDST(bool on) {
	dst = on;
}
void Timer::setCurrentTime(time_t utc) {
	int offset = getUTCoffset();
	current_time = utc - offset*3600;

}
void Timer::setCurrentTime(std::string localt) {
//Format "MM/DD/YY HH:MM:SS" in 24-hour format

	const char* date = localt.substr(0, 8).c_str();
	const char* time = localt.substr(9, 16).c_str();

	getTimeFromString(time);
	getDateFromString(date);

	tm* tptr = new tm();

	tptr->tm_hour = hh;
	tptr->tm_min = mm;
	tptr->tm_sec = ss;
	tptr->tm_mday = DD;
	tptr->tm_mon = MM - 1;
	tptr->tm_year = 100 + YY;

	time_t tnow = mktime(tptr);

	current_time = tnow;

}
void Timer::setSysUpdateInterval(int t) {

	system_update_interval = t;
	return;
}
void Timer::setUnixUpdateInterval(int t) {
	time_update_interval = t;
	return;
}
std::string Timer::getCurrentTime_str() {
//Returns current time as string with format:
//"MM/DD/YY HH:MM:SS"

	char tbuff[100];
	tm* tnow = gmtime(&current_time);

	std::strftime(tbuff, sizeof(tbuff), "%D %T", tnow);
	std::string tnow_str = (std::string) tbuff;

	return tnow_str;
}
time_t Timer::getCurrentTime_tm() {

	return current_time;
}
time_t Timer::getCurrentSysTime_tm() {

	return current_sys_time;

}
std::string Timer::getTimeZone() {

	return (std::string) tzone;
}
int Timer::getUTCoffset() {

	return utc_offset;

}
bool Timer::getDST() {

	return dst;
}
Stopwatch* Timer::createStopWatch() {
	Stopwatch* swatch = new Stopwatch();

	stop_watch_list.push_back(swatch);

	return swatch;
}
ToggleTimer* Timer::createToggler() {
	ToggleTimer* toggler = new ToggleTimer();

	toggler_list.push_back(toggler);

	return toggler;
}
Stopwatch* Timer::createStopWatch(int mils, bool active) {

	Stopwatch* swatch = new Stopwatch();

	swatch->interval = mils;
	swatch->talarm = 0;
	swatch->active = active;
	swatch->status = false;

	stop_watch_list.push_back(swatch);

	return swatch;
}
ToggleTimer* Timer::createToggler(std::string ONtime, std::string OFFtime, bool active) {

	ToggleTimer* ttimer = new ToggleTimer();
	ttimer->ON_time = ONtime;
	ttimer->OFF_time = OFFtime;
	ttimer->active = active;
	ttimer->ON_status = false;
	ttimer->OFF_status = false;

	toggler_list.push_back(ttimer);

	return ttimer;
}
void Timer::deleteAlarm(Stopwatch* sw) {

	stop_watch_list.erase(std::remove(stop_watch_list.begin(), stop_watch_list.end(), sw), stop_watch_list.end());

	delete sw;

	return;
}
void Timer::deleteAlarm(ToggleTimer* tog) {

	toggler_list.erase(std::remove(toggler_list.begin(), toggler_list.end(), tog), toggler_list.end());

	delete tog;

	return;
}
void Timer::toggleAlarm(Stopwatch* sw, bool active) {

	sw->active = active;

	return;
}
void Timer::toggleAlarm(ToggleTimer* tog, bool active) {

	tog->active = active;

	return;
}
void Timer::ackAlarm(Stopwatch* sw) {

	sw->ack_rcv = true;

	return;
}
void Timer::ackAlarm(ToggleTimer* tog) {

	tog->ack_rcv = true;

	return;
}
int Timer::isAlarming(Stopwatch* sw) {

	int ret;

	bool val = sw->status;
	if (val == true) {ret = 1;}
	else {ret = 0;}

	return ret;
}
int Timer::isAlarming(ToggleTimer* tog) {
//Returns -1 if OFF is alarming
//Returns 1 if ON is alarming
//Returns 0 if neither is alarmin

	int ret = 0;

	bool ONval = tog->ON_status;
	bool OFFval = tog->OFF_status;

	if (!ONval && !OFFval) {
		ret = 0;
	}
	else if (ONval && !OFFval) {
		ret = 1;
	}
	else if (!ONval && OFFval) {
		ret = -1;
	}

	return ret;
}
int Timer::isActive(Stopwatch* sw) {

	int ret;

	ret = sw->active;

	return ret;
}
int Timer::isActive(ToggleTimer* tog) {

	int ret;

	ret = tog->active;

	return ret;
}
void Timer::update() {

	//Clear system timer if exceeds max
	//CHECK HERE IF YOU HAVE A PROBLEM AFTER 23 DAYS!
	if (current_sys_time >= 2000000000) {

		std::vector<Stopwatch*>::iterator it;

		for (it=stop_watch_list.begin(); it != stop_watch_list.end(); it++) {
			unsigned long offset = (*it)->talarm - current_sys_time;
			if (offset > 0) {
				(*it)->talarm = offset;
			}
		}
		current_sys_time = 0;
	}

	//Update system timer
	if ((int) systemTimer >= system_update_interval) {
		current_sys_time += (int) systemTimer;
		systemTimer = 0;
	}

	//Update unix timer
	if ((int) unixTimer >= time_update_interval) {
		if (current_time != 0) {
			current_time += (int) unixTimer/1000;
		}
		unixTimer = 0;
	}


	//Handle toggle timers and stopwatches
	std::vector<Stopwatch*>::iterator it;
	std::vector<ToggleTimer*>::iterator it2;

	int count = 0;
	//Check all stopwatches for alarm condition
	for (it=stop_watch_list.begin(); it!=stop_watch_list.end(); it++) {
		count++;

		//Set alarm time if it hasn't been yet
		if ((*it)->talarm == 0) {
			(*it)->talarm = (*it)->interval + current_sys_time;
		}
		else {
			//Check current time against alarm time (pass if ack has been received or alarm inactive)
			if (current_sys_time >= (*it)->talarm && !(*it)->ack_rcv && (*it)->active) {
				(*it)->status = true;
			}
			//Check if ack has been received for alarm
			else if ((*it)->ack_rcv) {
				(*it)->status = false;
				(*it)->ack_rcv = false;
				(*it)->talarm = 0;
			}
		}
	}

	if (current_time != 0) {

		//Check all toggle timers for alarm conditions
		for (it2=toggler_list.begin(); it2!=toggler_list.end(); it2++) {
			//Set local on and off times if they haven't been yet
			if ((*it2)->on_time == 0) {
				LocalONOFFtime((*it2), "both");	//Set initial ON and OFF times
			}
			else {

				//Check current_time against ON time (unix comparison) (pass if ack has been received or alarm inactive)
				if (current_time >= (*it2)->on_time && !(*it2)->ack_rcv && (*it2)->active) {
					(*it2)->ON_status = true;
					LocalONOFFtime((*it2), "on");	//Adjust ON time for next alarm
				}
				//Check if ack has been received for ON alarm
				else if ((*it2)->ON_status && (*it2)->ack_rcv && (*it2)->active) {
					(*it2)->ON_status = false;
					(*it2)->ack_rcv = false;
				}

				//Check current_time against OFF time (unix comparison) (pass if ack has been received or alarm inactive)
				if (current_time >= (*it2)->off_time && !(*it2)->ack_rcv && (*it2)->active) {
					(*it2)->OFF_status = true;
					LocalONOFFtime((*it2), "off");	//Adjust OFF time for next alarm
				}
				//Check if ack has been received for OFF alarm
				else if ((*it2)->OFF_status && (*it2)->ack_rcv && (*it2)->active){
					(*it2)->OFF_status = false;
					(*it2)->ack_rcv = false;
				}

			}
		}
	}

}

/*PRIVATE FUNCTIONS*/

void Timer::LocalONOFFtime(ToggleTimer* it, std::string flag) {

	//Pointers to Alarm ON and OFF times (only contain tm_hour, tm_min, tm_sec)
	getTimeFromString(it->ON_time);
	ton->tm_hour = hh;
	ton->tm_min = mm;
	ton->tm_sec = ss;

	getTimeFromString(it->OFF_time);
	toff->tm_hour = hh;
	toff->tm_min = mm;
	toff->tm_sec = ss;


	//Convert current unix time to local time
	tm* nowptr = gmtime(&current_time);

	//Convert alarm ON and OFF times to tm structs
	ton->tm_isdst = (int) dst;
	ton->tm_mday = nowptr->tm_mday;
	ton->tm_mon = nowptr->tm_mon;
	ton->tm_wday = nowptr->tm_wday;
	ton->tm_yday = nowptr->tm_yday;
	ton->tm_year = nowptr->tm_year;

	toff->tm_isdst = (int) dst;
	toff->tm_mday = nowptr->tm_mday;
	toff->tm_mon = nowptr->tm_mon;
	toff->tm_wday = nowptr->tm_wday;
	toff->tm_yday = nowptr->tm_yday;
	toff->tm_year = nowptr->tm_year;

	//Convert tm structs to time_t and adjust on/off times
	if (flag == "both") {
		if (mktime(ton) <= current_time) {
			it->on_time = mktime(ton) + 24*60*60;
			it->off_time = mktime(toff) + 24*60*60;
		}
		else {
			it->on_time = mktime(ton);
			it->off_time = mktime(toff);
		}
	}
	else if (flag == "on") {
		if (mktime(ton) <= current_time) {
			it->on_time = mktime(ton) + 24*60*60;
		}
		else {
			it->on_time = mktime(ton);
		}
	}
	else {
		if (mktime(toff) <= current_time) {
			it->off_time = mktime(toff) + 24*60*60;
		}
		else {
			it->off_time = mktime(toff);
		}
	}

}
int Timer::timeRemaining(Stopwatch* sw) {
	int ret;

	ret = sw->talarm - current_sys_time;

	return ret;
}
int Timer::timeRemaining(ToggleTimer* tog, std::string pos) {
	int ret;

	if (pos == "ON") {
		ret = tog->on_time - current_time;
	}
	else if (pos == "OFF") {
		ret = tog->off_time - current_time;
	}

	return ret;
}
void Timer::getTimeFromString(std::string t) {
	//Format: "HH:MM:SS" 24-hr format

	const char* hour = t.substr(0, 2).c_str();
	const char* min = t.substr(3, 4).c_str();
	const char* sec = t.substr(6, 7).c_str();

	hh = atoi(hour);
	mm = atoi(min);
	ss = atoi(sec);


	return;
}
void Timer::getDateFromString(std::string d) {
	//Format: "MM/DD/YY"

	const char* mon = d.substr(0, 2).c_str();
	const char* day = d.substr(3, 4).c_str();
	const char* year = d.substr(6, 7).c_str();

	MM = atoi(mon);
	DD = atoi(day);
	YY = atoi(year);

	return;
}

