/*
    MC20_BT.h
    A library for SeeedStudio GPS Tracker BT

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

#ifndef __MC20_GNSS_H__
#define __MC20_GNSS_H__

#include "MC20_Common.h"
#include "MC20_Arduino_Interface.h"

enum GNSS_MDOE {
    GNSS_DEFAULT_MODE = 0, // Default quick start GNSS mode
    EPO_QUICK_MODE = 1, // EPO quick mode
    EPO_LP_MODE = 2, // EPO low power consumption mode
    EPO_RL_MODE = 3  // Reference-location mode
};

class GNSS : public GPSTracker {
  public:
    double longitude;
    double latitude;
    char str_longitude[16];
    char str_latitude[16];
    double ref_longitude = 22.584322;
    double ref_latitude = 113.966678;
    char North_or_South[2];
    char West_or_East[2];

    /**

    */
    bool initialize();

    /**

    */
    bool open_GNSS(int mode);

    /**

    */
    bool close_GNSS(void);

    /** Open GNSS by directly

    */
    bool open_GNSS_default_mode(void);   // Default GNSS mode

    /** Open GNSS and turn on EPO mode

    */
    bool open_GNSS_EPO_quick_mode(void); // Quick mode with EPO

    /** Open EPO before open GNSS,that cost down GNSS consumption

    */
    bool open_GNSS_EPO_LP_mode(void);   // Low power consumption mode with EPO

    /** Before open EPO and GNSS, write Reference-Location into flash, this can help search location faster

    */
    bool open_GNSS_RL_mode(void);     // Reference-location mode

    /** open GNSS

    */
    bool open_GNSS(void);

    /**

    */
    bool settingContext(void);

    /**

    */
    bool isNetworkRegistered(void);

    /**

    */
    bool isTimeSynchronized(void);

    /**

    */
    bool enableEPO(void);

    /**

    */
    bool triggerEPO(void);

    /**
        Convert double coordinate data to string
    */
    void doubleToString(double longitude, double latitude);


    /** Get coordinate infomation

    */
    bool getCoordinate(void);

    /**

    */
    bool dataFlowMode(void);

    /*
        MTK and PQ commands
    */
    uint8_t getCheckSum(char* string);
    bool enable_EASY(void);
    bool enable_GLP(int enable, int save);
    bool set_DGPS_Mode();

    bool getQueryStatus_LOCUS(void);
    bool eraseFlash_LOCUS(void);
    bool stopLogger_LOCUS(int status);
    bool queryData_LOCUS(void);
    bool setPeriodicMode();
    bool set1PPS(bool status);
    bool setAlwaysLocateMode(int mode);

    bool select_searching_satellite(int gps, int beidou);

    bool setWorkMode(int mode);
    bool setStandbyMode(int mode);
    /*
        End of MTK and PQ commands
    */
};

#endif
