/*
 * Timer.h
 *
 * Copyright (c) 2019 by Justin Holland <jmatthew_7@yahoo.com>
 * Switch controller library for Teensy 3.x.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "Arduino.h"
#include <string>
#include <vector>


struct Stopwatch {
	unsigned long interval;
	unsigned long talarm;
	bool active;
	bool status;
	bool ack_rcv;
};

struct ToggleTimer {
	std::string ON_time;
	std::string OFF_time;
	time_t on_time;
	time_t off_time;
	bool active;
	bool ON_status;
	bool OFF_status;
	bool ack_rcv;
};

class Timer {


public:

	Timer();

	/*-----------------------------------------
	 * Set Local Timezone (US only)
	 * ----------------------------------------
	 * ARGUMENTS:
	 * tz = "Eastern", "Central", "Mountain",
	 *         "Pacific", "Alaska", "Hawaii"
	 *
	 */
	 void setTimeZone(std::string tz);

	 /*-----------------------------------------
	  * Set Local Timezone (International)
	  * ----------------------------------------
	  * ARGUMENTS:
	  * offset = UTC offset in hours
	  */
	 void setTimeZone(int offset);

	 /*-----------------------------------------
	  * Set Daylight Saving Time
	  * ----------------------------------------
	  * ARGUMENTS:
	  * on = TRUE for DST, FALSE for Standard Time
	  */
	 void setDST(bool on);

	/*-----------------------------------------
	 * Set Current Unix Time for Timer Class
	 * ----------------------------------------
	 * ARGUMENTS:
	 * utc = Unix time
	 *
	 * Be sure to set local timezone and DST flag
	 * prior to calling this function!
	 */
	void setCurrentTime(time_t utc);

	/*-----------------------------------------
	 * Set Current Unix Time for Timer Class
	 * ----------------------------------------
	 * ARGUMENTS:
	 * localt = Time string ("MM/DD/YY HH:mm:ss" in 24-hour format)
	 *
	 */
	void setCurrentTime(std::string localt);

	/*-------------------------------------------
	 * Set the update interval for system timer
	 * ------------------------------------------
	 * ARGUMENTS:
	 * t = Update interval in millis
	 *
	 */
	void setSysUpdateInterval(int t);

	/*--------------------------------------------
	 * Set the update interval for Unix timer
	 * -------------------------------------------
	 * ARGUMENTS:
	 * t = Update interval in millis
	 *
	 */
	void setUnixUpdateInterval(int t);

	/*---------------------------------------------
	 * Get current local time
	 * --------------------------------------------
	 * ARGUMENTS:
	 * none
	 *
	 * Returns local time as a string having format
	 * "MM/DD/YY, HH:mm:ss"
	 */
	std::string getCurrentTime_str();	//"MM/DD/YYYY, HH:MM:SS"

	/*---------------------------------------------
	 * Get current local time
	 * --------------------------------------------
	 * ARGUMENTS:
	 * none
	 *
	 * Returns local time as time_t
	 */
	time_t getCurrentTime_tm();

	/*----------------------------------------------
	 * Get current system time
	 * ---------------------------------------------
	 * ARGUMENTS:
	 * none
	 *
	 * Returns system time as time_t
	 */
	time_t getCurrentSysTime_tm();

	/*-----------------------------------------------
	 * Get timezone
	 * ----------------------------------------------
	 * ARGUMENTS:
	 * none
	 *
	 * Returns current timezone setting
	 *
	 */
	std::string getTimeZone();

	/*-----------------------------------------------
	 * Get UTC offset
	 * ----------------------------------------------
	 * ARGUMENTS:
	 * none
	 *
	 * Returns current UTC offset
	 */
	int getUTCoffset();

	/*-----------------------------------------------
	 * Get DST Setting
	 * ----------------------------------------------
	 * ARGUMENTS:
	 * none
	 *
	 * Returns TRUE, FALSE
	 */
	bool getDST();

	/*-----------------------------------------------
	 * Create stopwatch timer
	 * ----------------------------------------------
	 * ARGUMENTS:
	 * mils = Alarm interval in millis
	 * active = TRUE, FALSE
	 *
	 * Returns pointer to stopwatch instance
	 */
	Stopwatch* createStopWatch(int mils, bool active);

	/*-----------------------------------------------
	 * Create stopwatch timer
	 * ----------------------------------------------
	 * ARGUMENTS:
	 * none
	 *
	 * Returns pointer to empty stopwatch instance
	 */
	Stopwatch* createStopWatch();

	/*-----------------------------------------------
	 * Create toggle timer
	 * ----------------------------------------------
	 * ARGUMENTS:
	 * ONtime = On time ("HH:mm:ss" in 24-hour format)
	 * OFFtime = Off time ("HH:mm:ss" in 24-hour format)
	 * active = TRUE, FALSE
	 *
	 * Returns pointer to ToggleTimer instance
	 */
	ToggleTimer* createToggler(std::string ONtime, std::string OFFtime, bool active);

	/*-------------------------------------------------
	 * Create toggle timer
	 * ------------------------------------------------
	 * ARGUMENTS:
	 * none
	 *
	 * Returns pointer to empty ToggleTimer instance
	 */
	ToggleTimer* createToggler();

	/*--------------------------------------------------
	 * Delete a stopwatch alarm
	 * -------------------------------------------------
	 * ARGUMENTS:
	 * sw = Pointer to stopwatch
	 *
	 */
	void deleteAlarm(Stopwatch* sw);

	/*--------------------------------------------------
	 * Delete a toggle timer
	 * -------------------------------------------------
	 * ARGUMENTS:
	 * tog = Pointer to ToggleTimer
	 *
	 */
	void deleteAlarm(ToggleTimer* tog);

	/*--------------------------------------------------
	 * Toggle stopwatch alarm state
	 * -------------------------------------------------
	 * ARGUMENTS:
	 * sw = Pointer to stopwatch
	 * active = TRUE for Active, FALSE for inactive
	 *
	 */
	void toggleAlarm(Stopwatch* sw, bool active);

	/*--------------------------------------------------
	 * Toggle state of toggle timer
	 * -------------------------------------------------
	 * ARGUMENTS:
	 * tog = Pointer to toggle timer
	 * active = TRUE for Active, FALSE for inactive
	 *
	 */
	void toggleAlarm(ToggleTimer* tog, bool active);

	/*--------------------------------------------------
	 * Acknowledge a toggle timer that is alarming
	 * -------------------------------------------------
	 * ARGUMENTS:
	 * tog = Pointer to toggle timer
	 *
	 * Calling this function is required for proper
	 * functionality of alarm.
	 */
	void ackAlarm(ToggleTimer* tog);

	/*--------------------------------------------------
	 * Acknowledge a stopwatch that is alarming
	 * -------------------------------------------------
	 * ARGUMENTS:
	 * sw = Pointer to stopwatch
	 *
	 * Calling this function is required for proper
	 * functionality of alarm.
	 */
	void ackAlarm(Stopwatch* sw);

	/*--------------------------------------------------
	 * Check if stopwatch is alarming
	 * -------------------------------------------------
	 * ARGUMENTS:
	 * sw = Pointer to stopwatch
	 *
	 * Returns 0 if NOT alarming, 1 if alarming
	 */
	int isAlarming(Stopwatch* sw);

	/*--------------------------------------------------
	 * Check if toggle timer is alarming
	 * -------------------------------------------------
	 * ARGUMENTS:
	 * tog = Pointer to toggle timer
	 *
	 * Returns 0 if NOT alarming, 1 if ON is alarming,
	 * -1 if OFF is alarming
	 */
	int isAlarming(ToggleTimer* tog);

	/*--------------------------------------------------
	 * Check if stopwatch is active
	 * -------------------------------------------------
	 * ARGUMENTS:
	 * sw = Pointer to stopwatch
	 *
	 * Returns 0 if inactive, 1 if active
	 */
	int isActive(Stopwatch* sw);

	/*--------------------------------------------------
	 * Check if toggle timer is active
	 * -------------------------------------------------
	 * ARGUMENTS:
	 * tog = Pointer to toggle timer
	 *
	 * Returns 0 if inactive, 1 if active
	 */
	int isActive(ToggleTimer* tog);

	/*---------------------------------------------------
	 * Get time remaining until next stopwatch alarm
	 * --------------------------------------------------
	 * ARGUMENTS:
	 * sw = Pointer to stopwatch
	 *
	 * Returns time until next stopwatch alarm in millis
	 */
	int timeRemaining(Stopwatch* sw);

	/*---------------------------------------------------
	 * Get time remaining until next toggle timer alarm
	 * --------------------------------------------------
	 * ARGUMENTS:
	 * tog = Pointer to toggle timer
	 * pos = "ON" for on time alarm, "OFF" for off time alarm
	 *
	 * Returns time until next toggle timer alarm in seconds
	 */
	int timeRemaining(ToggleTimer* tog, std::string pos);

	/*--------------------------------------------------
	 * Updates Timer instance in main loop
	 * -------------------------------------------------
	 *
	 * Handles all updates to system and local time as well
	 * as stopwatch and toggle timer alarms
	 */
	void update();


private:
	time_t current_time;
	unsigned long current_sys_time;
	long internal_timer;
	int time_update_interval;
	int system_update_interval;
	bool waiting_for_sync;
	tm* ton;
	tm* toff;
	int hh;
	int mm;
	int ss;
	int MM;
	int DD;
	int YY;
	elapsedMillis systemTimer;
	elapsedMillis unixTimer;
	elapsedMillis expireTimer;
	int timeout;

	static int utc_offset ;
	static const char* tzone;
	static volatile bool dst;
	static std::vector<Stopwatch*> stop_watch_list;
	static std::vector<ToggleTimer*> toggler_list;

	void getTimeFromString(std::string t);
	void getDateFromString(std::string d);
	void LocalONOFFtime(ToggleTimer* it, std::string flag);



};







#endif /* TIMER_H_ */
