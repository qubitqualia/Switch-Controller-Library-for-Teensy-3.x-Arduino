/*
 * string_Teensy.cpp
 *
 *  Created on: May 4, 2019
 *      Author: jmatt
 */
#include "Arduino.h"
#include <string>

template class std::basic_string<char>;

namespace std {
void __throw_bad_alloc() {
	Serial.println("Unable to allocate memory");
}
void __throw_length_error(char const*e) {
	Serial.print("Length Error :");
	Serial.println(e);
}
void __throw_logic_error(char const*e) {
	Serial.print("Logic Error :");
	Serial.println(e);
}

void
  __throw_out_of_range_fmt(const char*, ...)
 {
	Serial.print("Out of range error : ");

}
}


