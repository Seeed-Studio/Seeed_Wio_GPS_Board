/*
    MC20_Arduino_Interface.cpp
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

#include <stdio.h>
#include "MC20_Common.h"

// GPSTracker* GPSTracker::inst;

GPSTracker::GPSTracker() {
    // inst = this;
    // MC20_init();
    // io_init();
    pinMode(RGB_PIN, OUTPUT);
    digitalWrite(RGB_PIN, LOW);
}

bool GPSTracker::init(void) {
    if (!MC20_check_with_cmd(F("AT\r\n"), "OK\r\n", CMD)) {
        return false;
    }

    if (!MC20_check_with_cmd(F("AT+CPIN?\r\n"), "OK\r\n", CMD)) {
        return false;
    }

    if (!checkSIMStatus()) {
        return false;
    }
    return true;
}

bool GPSTracker::Check_If_Power_On(void) {
    return MC20_check_with_cmd(F("AT\n\r"), "OK", CMD, 2, 2000);
}

void GPSTracker::Power_On(void) {
    MC20_init();
    if (Check_If_Power_On()) {
        return;
    }

    pinMode(PWR_BAT, OUTPUT);
    pinMode(PWR_KEY, OUTPUT);

    digitalWrite(PWR_BAT, HIGH);
    digitalWrite(PWR_KEY, HIGH);
    delay(2000);
    digitalWrite(PWR_KEY, LOW);
    // delay(2000);
}

void GPSTracker::powerReset(void) {
    digitalWrite(PWR_KEY, LOW);
}

void GPSTracker::io_init() {
    for (int i = 0; i < 20; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
    }
}

bool GPSTracker::checkSIMStatus(void) {
    char mc20_Buffer[32];
    int count = 0;
    MC20_clean_buffer(mc20_Buffer, 32);
    while (count < 3) {
        MC20_send_cmd("AT+CPIN?\r\n");
        MC20_read_buffer(mc20_Buffer, 32, DEFAULT_TIMEOUT);
        if ((NULL != strstr(mc20_Buffer, "+CPIN: READY"))) {
            break;
        }
        count++;
        delay(300);
    }
    if (count == 3) {
        return false;
    }
    return true;
}

bool GPSTracker::waitForNetworkRegister(void) {
    int errCounts = 0;

    //
    while (!MC20_check_with_cmd("AT+CREG?\n\r", "+CREG: 0,1", CMD, 2, 2000)) {
        errCounts++;
        if (errCounts > 30) { // Check for 30 times
            return false;
        }
        delay(1000);
    }

    errCounts = 0;
    while (!MC20_check_with_cmd("AT+CGREG?\n\r", "+CGREG: 0,1", CMD, 2, 2000)) {
        errCounts++;
        if (errCounts > 30) { // Check for 30 times
            return false;
        }
        delay(1000);
    }

    return true;
}

bool GPSTracker::sendSMS(char* number, char* data) {
    //char cmd[32];
    if (!MC20_check_with_cmd(F("AT+CMGF=1\r\n"), "OK\r\n", CMD)) { // Set message mode to ASCII
        return false;
    }
    delay(500);
    MC20_flush_serial();
    MC20_send_cmd("AT+CMGS=\"");
    MC20_send_cmd(number);
    if (!MC20_check_with_cmd(F("\"\r\n"), ">", CMD)) {
        return false;
    }
    delay(1000);
    MC20_send_cmd(data);
    delay(500);
    MC20_send_End_Mark();
    return MC20_wait_for_resp("OK\r\n", CMD, 10);
}

bool GPSTracker::readSMS(int messageIndex, char* message, int length, char* phone, char* datetime) {
    /*  Response is like:
        AT+CMGR=2

        +CMGR: "REC READ","XXXXXXXXXXX","","14/10/09,17:30:17+08"
        SMS text here

        So we need (more or lees), 80 chars plus expected message length in buffer. CAUTION FREE MEMORY
    */

    int i = 0;
    char mc20_Buffer[80 + length];
    //char cmd[16];
    char num[4];
    char* p, *p2, *s;

    MC20_check_with_cmd(F("AT+CMGF=1\r\n"), "OK\r\n", CMD);
    delay(1000);
    //sprintf(cmd,"AT+CMGR=%d\r\n",messageIndex);
    //MC20_send_cmd(cmd);
    MC20_send_cmd("AT+CMGR=");
    itoa(messageIndex, num, 10);
    MC20_send_cmd(num);
    MC20_send_cmd("\r\n");
    MC20_clean_buffer(mc20_Buffer, sizeof(mc20_Buffer));
    MC20_read_buffer(mc20_Buffer, sizeof(mc20_Buffer));

    if (NULL != (s = strstr(mc20_Buffer, "READ\",\""))) {
        // Extract phone number string
        p = strstr(s, ",");
        p2 = p + 2; //We are in the first phone number character
        p = strstr((char*)(p2), "\"");
        if (NULL != p) {
            i = 0;
            while (p2 < p) {
                phone[i++] = *(p2++);
            }
            phone[i] = '\0';
        }
        // Extract date time string
        p = strstr((char*)(p2), ",");
        p2 = p + 1;
        p = strstr((char*)(p2), ",");
        p2 = p + 2; //We are in the first date time character
        p = strstr((char*)(p2), "\"");
        if (NULL != p) {
            i = 0;
            while (p2 < p) {
                datetime[i++] = *(p2++);
            }
            datetime[i] = '\0';
        }
        if (NULL != (s = strstr(s, "\r\n"))) {
            i = 0;
            p = s + 2;
            while ((*p != '\r') && (i < length - 1)) {
                message[i++] = *(p++);
            }
            message[i] = '\0';
        }
        return true;
    }
    return false;
}

bool GPSTracker::readSMS(int messageIndex, char* message, int length) {
    int i = 0;
    char mc20_Buffer[100];
    //char cmd[16];
    char num[4];
    char* p, *s;

    MC20_check_with_cmd(F("AT+CMGF=1\r\n"), "OK\r\n", CMD);
    delay(1000);
    MC20_send_cmd("AT+CMGR=");
    itoa(messageIndex, num, 10);
    MC20_send_cmd(num);
    // MC20_send_cmd("\r\n");
    MC20_send_cmd("\r\n");
    //  sprintf(cmd,"AT+CMGR=%d\r\n",messageIndex);
    //    MC20_send_cmd(cmd);
    MC20_clean_buffer(mc20_Buffer, sizeof(mc20_Buffer));
    MC20_read_buffer(mc20_Buffer, sizeof(mc20_Buffer), DEFAULT_TIMEOUT);
    if (NULL != (s = strstr(mc20_Buffer, "+CMGR:"))) {
        if (NULL != (s = strstr(s, "\r\n"))) {
            p = s + 2;
            while ((*p != '\r') && (i < length - 1)) {
                message[i++] = *(p++);
            }
            message[i] = '\0';
            return true;
        }
    }
    return false;
}

bool GPSTracker::deleteSMS(int index) {
    //char cmd[16];
    char num[4];
    //sprintf(cmd,"AT+CMGD=%d\r\n",index);
    MC20_send_cmd("AT+CMGD=");
    if (index > 998) {
        MC20_send_cmd("1,4");
    } else {
        itoa(index, num, 10);
        MC20_send_cmd(num);
    }
    // We have to wait OK response
    //return MC20_check_with_cmd(cmd,"OK\r\n",CMD);
    return MC20_check_with_cmd(F("\r"), "OK\r\n", CMD);
}


bool GPSTracker::callUp(char* number) {
    //char cmd[24];
    if (!MC20_check_with_cmd(F("AT+COLP=1\r\n"), "OK\r\n", CMD)) {
        return false;
    }
    delay(1000);
    //HACERR quitar SPRINTF para ahorar memoria ???
    //sprintf(cmd,"ATD%s;\r\n", number);
    //MC20_send_cmd(cmd);
    MC20_send_cmd("ATD");
    MC20_send_cmd(number);
    MC20_send_cmd(";\r\n");
    return true;
}

void GPSTracker::answer(void) {
    MC20_send_cmd("ATA\r\n");  //TO CHECK: ATA doesnt return "OK" ????
}

bool GPSTracker::hangup(void) {
    return MC20_check_with_cmd(F("ATH\r\n"), "OK\r\n", CMD);
}


bool GPSTracker::getSignalStrength(int* buffer) {
    //AT+CSQ                        --> 6 + CR = 10
    //+CSQ: <rssi>,<ber>            --> CRLF + 5 + CRLF = 9
    //OK                            --> CRLF + 2 + CRLF =  6

    byte i = 0;
    char mc20_Buffer[26];
    char* p, *s;
    char buffers[4];
    MC20_flush_serial();
    MC20_send_cmd("AT+CSQ\r");
    MC20_clean_buffer(mc20_Buffer, 26);
    MC20_read_buffer(mc20_Buffer, 26, DEFAULT_TIMEOUT);
    if (NULL != (s = strstr(mc20_Buffer, "+CSQ:"))) {
        s = strstr((char*)(s), " ");
        s = s + 1;  //We are in the first phone number character
        p = strstr((char*)(s), ",");  //p is last character """
        if (NULL != s) {
            i = 0;
            while (s < p) {
                buffers[i++] = *(s++);
            }
            buffers[i] = '\0';
        }
        *buffer = atoi(buffers);
        return true;
    }
    return false;
}

int GPSTracker::recv(char* buf, int len) {
    MC20_clean_buffer(buf, len);
    MC20_read_buffer(buf,
                     len);  //Ya he llamado a la funcion con la longitud del buffer - 1 y luego le estoy añadiendo el 0
    return strlen(buf);
}

bool GPSTracker::GSM_work_mode(int mode) {
    char buf_w[20];
    MC20_clean_buffer(buf_w, 20);
    sprintf(buf_w, "AT+CFUN=%d", mode);
    MC20_send_cmd(buf_w);
    return MC20_check_with_cmd("\n\r", "OK", CMD, 2, 2000, UART_DEBUG);
}

bool GPSTracker::GSM_config_slow_clk(int mode) {
    char buf_w[20];
    MC20_clean_buffer(buf_w, 20);
    sprintf(buf_w, "AT+QSCLK=%d", mode);
    MC20_send_cmd(buf_w);
    return MC20_check_with_cmd("\n\r", "OK", CMD, 2, 2000, UART_DEBUG);
}

bool GPSTracker::AT_PowerDown(void) {
    return MC20_check_with_cmd("AT+QPOWD=1\n\r", "NORMAL POWER DOWN", CMD, 5, 2000, UART_DEBUG);
}

