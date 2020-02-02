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

#ifndef __MC20_BT_H__
#define __MC20_BT_H__

#include "MC20_Common.h"
#include "MC20_Arduino_Interface.h"

enum BT_STATE {
    INITIAL_ON = 0,
    DEACTIVATING = 1,
    ACTIVATING = 2,
    IDLE = 5,
    SCANNING = 6,
    SCAN_RESULT_INDICATION = 7,
    SCAN_CANCELLING = 8,
    INITIATE_PAIRING = 9,
    CONNECTING = 12,
    UNPAIRING = 13,
    DELETING_PAIRED_DEVICE = 14,
    DELETING_ALL = 15,
    DISCONNECTING = 16,
    NUMERIC_CONFIRM = 19,
    NUMERIC_CONFIrM_reSPONSE = 20,
    CONNECT_ACCEPT_CONFIRM_INDICATION = 25,
    SERVICE_REFRESHING = 26,
    DEVICe_NAME_SETTING = 29,
    AUTHENTICATION_SETTING = 30,
    RELEASE_ALL_CONNECTION = 31,
    ACTIVATING_PROFILES = 36
};

enum PROFILE_ID {
    SPP_PROFILE = 0,
    HF_PROFILE = 5,
    HFG_PROFILE = 6
};


class BlueTooth : public GPSTracker {
  public:
    BlueTooth(): GPSTracker() {

    }

    /** power on BlueTooth module
        @returns
            0 on success
            -1 on error
    */
    int BTPowerOn(void);

    /** power off BlueTooth module
        @returns
            0 on success
            -1 on error
    */
    int BTPowerOff(void);

    /** get host device name of BlueTooth module
        @param  deviceName  buffer array to save device name.
        @returns
            0 on success
            -1 on error
    */
    // int getHostDeviceName(char* deviceName);

    /** scan for target device according to device name
        @param  deviceName  device which will be scanned for
        @returns
            0 on success
            -1 on error
    */
    int scanForTargetDevice(char* deviceName);

    /** send pairing request to device according to deviceID
        @param  deviceID    device ID
        @returns
            0 on success
            -1 on error
    */
    int sendPairingReqstToDevice(int deviceID);

    /** accept other BlueTooth module's pairing request
        @returns
            true on success
            false on error
    */
    bool acceptPairing(void);

    /** unPair with paired BlueTooth device
        @returns
            0 on success
            -1 on error
    */
    int unPair(void);

    /** accept other BlueTooth device's connecting request
        @returns
            0 on success
            -1 on error
    */
    int acceptConnect(void);

    /** wait to handle other BlueTooth device's pairing or connecting  request
        @returns
            0 on success
            -1 on error
    */
    int loopHandle(void);

    /** disconnect with connected BlueTooth device
        @param  deviceID    device that will be disconnected
        @returns
            0 on success
            -1 on error
    */
    int disconnect(int deviceID);

    /** connect with other BlueTooth device in SPP profile
        @param  deviceID    device that will be connect in SPP profile
        @returns
            0 on success
            -1 on error
    */
    // int connectInSPP(int deviceID);

    /** receive data in SPP profile
        @param  data    buffer array to receive data from other BlueTooth device in SPP profile
        @returns
            0 on success
            -1 on error
    */
    // int recvInSPP(char* data);

    /** send data in SPP profile
        @param  data    buffer array to send data to other BlueTooth device in SPP profile
        @returns
            0 on success
            -1 on error
    */
    // int sendInSPP(char* data);

    /** get BT state
        @return BT_STATE
    */
    int getBTState(void);

    /** Check if BT device paired and return its id
        @param deviceName     BT device name
        @return
           (0,n)paired device ID
           -1 not found
    */
    int getPairedDeviceID(char* deviceName);

    /** connect paired BT device
        @param deviceID    device id
        @param profileName    profile name that connecting device in
    */
    bool BTConnectPairedDevice(int deviceID, int profileName);

    /**

    */
    bool BTFastConnect(char* deviceID, int profileName);

  private:

    /** BlueTooth power flag
    */
    bool bluetoothPower;

    /** target device ID
    */
    int targetDeviceID;

};

#endif
