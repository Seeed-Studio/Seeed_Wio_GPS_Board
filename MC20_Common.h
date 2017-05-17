/*
 * GPSTracker_Arduino_Interface.h 
 * A library for SeeedStudio GPS Tracker
 *  
 * Copyright (c) 2017 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : lawliet zou, lambor
 * Create Time: April 2017
 * Change Log :
 *
 * The MIT License (MIT)
  *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __MC20_COMMON_H__
#define __MC20_COMMON_H__

#include "MC20_Arduino_Interface.h"

/** GPSTracker class.
 *  used to realize GPSTracker communication
 */ 
 
enum Protocol {
    CLOSED = 0,
    TCP    = 1,
    UDP    = 2,
};
 
class GPSTracker
{
public:
    /** Create GPSTracker instance
     *  @param number default phone number during mobile communication
     */
     
    GPSTracker(); 
    
    /** get instance of GPSTracker class
     */
    // static GPSTracker* getInstance() {
    //     return inst;
    // };
    
    /** initialize GPSTracker module including SIM card check & signal strength
     *  @return true if connected, false otherwise
     */

    bool init(void);

   
    /** check if GPSTracker module is powered on or not
     *  @returns
     *      true on success
     *      false on error
     */
    bool Check_If_Power_On(void);

    
    /** power Up GPSTracker module
     *  @param  pin D13 is connectted to a power switch IC, the power is on when pin D13 pull up. 
     *  @returns
     *      
     */
    void Power_On(void);
    
    /** power reset for SIM800 board 
     *  @param  pin (preconfigurated as OUTPUT)
     *  @returns
     *      
     */ 
    void powerReset(void);

    void io_init();
     
     /** Wait for network register
     *  
     *  
     *  @returns
     *      true on success
     *      false on error
     */
     bool waitForNetworkRegister(void);


    /** send text SMS
     *  @param  *number phone number which SMS will be send to
     *  @param  *data   message that will be send to
     *  @returns
     *      true on success
     *      false on error
     */
    bool sendSMS(char* number, char* data);
    
    /** read SMS, phone and date if getting a SMS message. It changes SMS status to READ 
     *  @param  messageIndex  SIM position to read
     *  @param  message  buffer used to get SMS message
     *  @param  length  length of message buffer
     *  @param  phone  buffer used to get SMS's sender phone number
     *  @param  datetime  buffer used to get SMS's send datetime
     *  @returns
     *      true on success
     *      false on error
     */
    bool readSMS(int messageIndex, char *message, int length, char *phone, char *datetime); 

    /** read SMS if getting a SMS message
     *  @param  buffer  buffer that get from GPSTracker module(when getting a SMS, GPSTracker module will return a buffer array)
     *  @param  message buffer used to get SMS message
     *  @param  check   whether to check phone number(we may only want to read SMS from specified phone number)
     *  @returns
     *      true on success
     *      false on error
     */
    bool readSMS(int messageIndex, char *message, int length);

    /** delete SMS message on SIM card
     *  @param  index   the index number which SMS message will be delete
     *          index > 998, delete all message stored on SIM card
     *  @returns
     *      true on success
     *      false on error
     */
    bool deleteSMS(int index);

    /** call someone
     *  @param  number  the phone number which you want to call
     *  @returns
     *      true on success
     *      false on error
     */
    bool callUp(char* number);

    /** auto answer if coming a call
     *  @returns
     */    
    void answer(void);
    
    /** hang up if coming a call
     *  @returns
     *      true on success
     *      false on error
     */    
    bool hangup(void);  
    
    /** get Signal Strength from SIM900 (see AT command: AT+CSQ) as integer
    *  @param
    *  @returns
    *      true on success
    *      false on error
    */
    bool getSignalStrength(int *buffer);

    int recv(char* buf, int len);

    /** GSM power Mode
     * @param
     *      0, least consumption 1, 4
     *      1, standard mode
     *      4, shut down RF send and receive function
     */
    bool GSM_work_mode(int mode);

    /** Config GSM slow clock mode
     * @param
     *  0 Disable slow clock
     *  1 Enable slow clock, and it is controlled by DTR
     *  2 When there is no data on serial port in 5 seconds, module will enter Sleep Disable slow clock
     */
    bool GSM_config_slow_clk(int mode);


    /**
     * Turn off module power buy AT commnad
     */
    bool AT_PowerDown(void);
    
// private:
    bool checkSIMStatus(void);
    int PWR_KEY = 13;  // Pulse power control
    int PWR_BAT = 7;  // BAT power
    int RGB_PIN = 10;  // RGB LED Pin
    int VB_PIN = 12;  // VCCB control pin
    int DTR_PIN  = 9;  
};
#endif
