/*
    MC20_BT.cpp
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

#include "MC20_BT.h"

int BlueTooth::BTPowerOn(void) {
    MC20_Test_AT();
    if (0 == bluetoothPower) {
        if (!MC20_check_with_cmd(F("AT+QBTPWR?\r\n"), "+QBTPWR: 1", CMD, DEFAULT_TIMEOUT)) {
            if (!MC20_check_with_cmd(F("AT+QBTPWR=1\r\n"), "OK", CMD, DEFAULT_TIMEOUT)) {
                ERROR("\r\nERROR:bluetoothPowerOn\r\n");
                return -1;
            } else {
                bluetoothPower = 1;
            }
        } else {
            bluetoothPower = 1;
        }
    }
    return 0;
}

int BlueTooth::BTPowerOff(void) {
    if (1 == bluetoothPower) {
        if (!MC20_check_with_cmd("AT+QBTPWR=0\r\n", "OK", CMD, DEFAULT_TIMEOUT)) {
            ERROR("\r\nERROR:bluetoothPowerOff\r\n");
            return -1;
        } else {
            bluetoothPower = 0;
        }
    }
    return 0;
}

// int BlueTooth::getHostDeviceName(char* deviceName)
// {
//     char Buffer[40];
//     char *s,*p;
//     int i = 0;
//     MC20_send_cmd("AT+BTHOST?\r\n");
//     MC20_read_buffer(Buffer,40,DEFAULT_TIMEOUT);
//     if(NULL == (s = strstr(Buffer,"+BTHOST:"))) {
//         ERROR("\r\nERROR: get host device name error\r\n");
//         return -1;
//     }
//     p = s + 9;//+BTHOST: SIM800H,33:7d:77:18:62:60
//     while(*(p) != ','){
//         deviceName[i++] = *p;
//         p++;
//     }
//     deviceName[i] = '\0';
//     return i;
// }

int BlueTooth::scanForTargetDevice(char* deviceName) {
    char Buffer[256];
    char* s;
    int deviceID;

    MC20_clean_buffer(Buffer, 256);
    MC20_send_cmd("AT+QBTSCAN\r\n"); //scan 20s
    MC20_read_buffer(Buffer, 256, 30, 30000); //+QBTSCAN: 4,"Mobile",DC0C5CB8C9F1
    DEBUG(Buffer);
    if (NULL == (s = strstr(Buffer, deviceName))) {
        ERROR("\r\nERROR: scan For Target Device error\r\n");
        return -1;
    }
    deviceID = atoi(s - 3);
    return deviceID;
}

int BlueTooth::sendPairingReqstToDevice(int deviceID) {
    char cmd[30];
    if (0 == deviceID) {
        return -1;
    }
    sprintf(cmd, "AT+QBTPAIR=%d\r\n", deviceID);
    MC20_send_cmd(cmd);
    return 0;
}
int BlueTooth::unPair(void) {
    char cmd[30];
    if (0 == targetDeviceID) {
        return -1;
    }
    sprintf(cmd, "AT+QBTUNPAIR=%d\r\n", targetDeviceID);
    if (!MC20_check_with_cmd(cmd, "OK", CMD, DEFAULT_TIMEOUT)) {
        ERROR("\r\nERROR: AT+QBTUNPAIR\r\n");
        return -1;
    }
    return 0;
}

bool BlueTooth::acceptPairing(void) {
    return MC20_check_with_cmd("AT+QBTPAIRCNF=1\r\n", "OK", CMD, 5);
}

int BlueTooth::acceptConnect(void) {
    if (!MC20_check_with_cmd("AT+QBTACPT=1\r\n", "OK", CMD, DEFAULT_TIMEOUT)) {
        ERROR("\r\nERROR:AT+QBTACPT\r\n");
        return -1;
    }
    return 0;
}
int BlueTooth::disconnect(int targetDeviceID) {
    char cmd[30];
    if (0 == targetDeviceID) {
        return -1;
    }
    sprintf(cmd, "AT+QBTDISCONN=%d\r\n", targetDeviceID);
    if (!MC20_check_with_cmd(cmd, "OK", CMD, DEFAULT_TIMEOUT)) {
        ERROR("\r\nERROR: AT+QBTDISCONN\r\n");
        return -1;
    }
    return 0;
}

int BlueTooth::loopHandle(void) {
    char Buffer[100];
    MC20_clean_buffer(Buffer, 100);
    while (!MC20_check_readable());
    MC20_read_buffer(Buffer, 100, DEFAULT_TIMEOUT);
    DEBUG(Buffer);

    if (NULL != strstr(Buffer, "+QBTIND: \"pair\"")) {
        if (!acceptPairing()) {
            return -1;
            ERROR("\r\nERROR:bluetoothAcceptPairing\r\n");
        }
    }
    return 0;
}
// int BlueTooth::connectInSPP(int deviceID) //Serial Port Profile
// {
//     char Buffer[200];
//     char cmd[20];
//     char *s;
//     sprintf(cmd,"AT+BTGETPROF=%d\r\n",deviceID);
//     MC20_send_cmd(cmd);
//     MC20_read_buffer(Buffer,200,DEFAULT_TIMEOUT);
//     if(NULL == ( s = strstr(Buffer,"\"SPP\""))) {
//         ERROR("\r\nERROR: No SPP Profile\r\n");
//         return -1;
//     }
//     MC20_clean_buffer(cmd,20);
//     sprintf(cmd,"AT+BTCONNECT=%d,%c\r\n",deviceID,*(s-2));
//     if(!MC20_check_with_cmd(cmd, "OK", CMD, DEFAULT_TIMEOUT)){
//         ERROR("\r\nERROR:AT+BTCONNECT\r\n");
//         return -1;
//     }
//     return 0;
// }

// int BlueTooth::recvInSPP(char* data)
// {
//     char bluetoothBuffer[64];
//     char * p = NULL;
//     int count = 0;
//     if(!MC20_check_with_cmd("AT+BTSPPGET=0\r\n", "OK", CMD, DEFAULT_TIMEOUT)){
//         ERROR("\r\nERROR:AT+BTSPPGET\r\n");
//         return -1;
//     }
//     while(!serialMC20.available()){
//         delay(200);
//     }
//     MC20_read_buffer(bluetoothBuffer,64,DEFAULT_TIMEOUT);
//     p = strstr(bluetoothBuffer,"+BTSPPDATA:");
//     if(NULL != p){
//         p += 11;
//         while(count != 2){
//             if(*(p++) == ','){
//                 count++;
//             }
//         }
//         int i = 0;
//         while(*(p++) != '$'){
//             data[i++] = *p;
//         }
//     }else{
//         ERROR("\r\nERROR: get data error\r\n");
//         return -1;
//     }
//     return 0;
// }

// int BlueTooth::sendInSPP(char* data)
// {
//     if(!MC20_check_with_cmd("AT+BTSPPSEND\r\n", ">", CMD, DEFAULT_TIMEOUT)){
//         ERROR("\r\nERROR:AT+BTSPPSEND\r\n");
//         return -1;
//     }
//     MC20_send_cmd(data);
//     // sendEndMark();
//     return 0;
// }

int BlueTooth::getBTState(void) {
    char* s;
    char buffer[256];
    MC20_clean_buffer(buffer, 256);
    MC20_send_cmd("AT+QBTSTATE\r\n");
    MC20_read_buffer(buffer, 256, 2, 500);

    // +QBTSTATE: 7

    int state = -1;
    DEBUG(buffer);
    if (NULL == (s = strstr(buffer, "+QBTSTATE:"))) {
        ERROR("\r\nERROR: Get BT state error!\r\n");
        return -1;
    }
    state = atoi(s + 11);

    return state;

}

int BlueTooth::getPairedDeviceID(char* deviceName) {
    char Buffer[256];
    char* s;
    int device_id;

    MC20_clean_buffer(Buffer, 256);
    MC20_send_cmd("AT+QBTSTATE\r\n"); //scan 20s
    MC20_read_buffer(Buffer, 256, 2, 1000); //+QBTSCAN: 4,"Mobile",DC0C5CB8C9F1

    DEBUG(Buffer);
    if (NULL == (s = strstr(Buffer, deviceName))) {
        ERROR("\r\nERROR: Device not paired!\r\n");
        return -1;
    }
    device_id = atoi(s - 3);
    return device_id;
}

bool BlueTooth::BTConnectPairedDevice(int deviceID, int profileName) {
    // char recvBuffer[256];
    char sendBuffer[20];

    MC20_clean_buffer(sendBuffer, 20);
    sprintf(sendBuffer, "AT+QBTCONN=%d,%d\n\r", deviceID, profileName);

    bool ret = MC20_check_with_cmd(sendBuffer, "+QBTCONN: 1", CMD, 5);
    if (ret) {
        targetDeviceID = deviceID;
    }
    return ret;
}

bool BlueTooth::BTFastConnect(char* deviceName, int profileName) {
    // char recvBuffer[256];
    char sendBuffer[20];
    int deviceID;
    bool ret;

    MC20_clean_buffer(sendBuffer, 20);
    deviceID = getPairedDeviceID(deviceName);
    if (-1 == deviceID) {
        deviceID = scanForTargetDevice(deviceName);
        if (0 > deviceID) {
            return false;
        }
        sprintf(sendBuffer, "AT+QBTPAIR=%d\n\r", deviceID);
        if (!MC20_check_with_cmd(sendBuffer, "+QBTPAIRCNF:", CMD, 5)) {
            return false;
        }
    }

    ret = BTConnectPairedDevice(deviceID, profileName);

    return ret;

}


