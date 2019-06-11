/*
 * string_Teensy.h
 *
 *  Created on: May 4, 2019
 *      Author: jmatt
 */

#ifndef STRING_TEENSY_H_
#define STRING_TEENSY_H_

namespace std {
extern void __throw_bad_alloc();
extern void __throw_length_error(char const*e);
extern void __throw_logic_error(char const*e);
extern void
 __throw_out_of_range_fmt(const char*, ...) __attribute__((__noreturn__))
    __attribute__((__format__(__printf__, 1, 2)));

}






#endif /* STRING_TEENSY_H_ */
