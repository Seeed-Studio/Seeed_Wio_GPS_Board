/*
    MC20_Common.h
    A library for SeeedStudio GPS Tracker

    Copyright (c) 2017 seeed technology inc.
    Website    : www.seeed.cc
    Author     : lawliet zou, lambor
    Create Time: April 2017
    Change Log :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef __MC20_ARDUINO_INTERFACE_H__
#define __MC20_ARDUINO_INTERFACE_H__

#include <Arduino.h>

#define serialMC20 Serial1
#define serialDebug SerialUSB

#define DEFAULT_TIMEOUT              5   //seconds
#define DEFAULT_INTERCHAR_TIMEOUT 3000   //miliseconds

#define UART_DEBUG true

#ifdef UART_DEBUG
    #define ERROR(x)            SerialUSB.println(x)
    #define DEBUG(x)            SerialUSB.println(x)
#else
    #define ERROR(x)
    #define DEBUG(x)
#endif

enum DataType {
    CMD     = 0,
    DATA    = 1,
};

void  MC20_init();
int   MC20_check_readable();
int   MC20_wait_readable(int wait_time);
void  MC20_flush_serial();
void  MC20_read_buffer(char* buffer, int count,  unsigned int timeout = DEFAULT_TIMEOUT,
                       unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
void  MC20_clean_buffer(char* buffer, int count);
void  MC20_send_byte(uint8_t data);
void  MC20_send_char(const char c);
void  MC20_send_cmd(const char* cmd);
void  MC20_send_cmd(const __FlashStringHelper* cmd);
void  MC20_send_cmd_P(const char* cmd);
boolean  MC20_Test_AT(void);
void  MC20_send_End_Mark(void);
boolean MC20_wait_for_resp(const char* resp, DataType type, unsigned int timeout = DEFAULT_TIMEOUT,
                           unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT, bool debug = false);
boolean  MC20_check_with_cmd(const char* cmd, const char* resp, DataType type, unsigned int timeout = DEFAULT_TIMEOUT,
                             unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT * 5, bool debug = false);
boolean  MC20_check_with_cmd(const __FlashStringHelper* cmd, const char* resp, DataType type,
                             unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT, bool debug = false);

#endif
