#include "MC20_Common.h"
#include "MC20_BT.h"

// GPSTracker gpsTracker = GPSTracker();
BlueTooth bt = BlueTooth();
int bt_state = -1;
char* deviceName = "N-612";


void setup() {
    SerialUSB.begin(115200);
    while (!SerialUSB);

    bt.Power_On();
    SerialUSB.println("\n\rMC20 power On!");
    bt.BTPowerOn();
    SerialUSB.println("\n\rBT power On!");

    while (IDLE != (bt_state = bt.getBTState())) {
        SerialUSB.print("State: ");
        SerialUSB.println(bt_state);
        delay(1000);
    }

    bt.BTFastConnect(deviceName, HFG_PROFILE);
}

void loop() {
    /* Debug */
    if (SerialUSB.available()) {
        serialMC20.write(SerialUSB.read());
    }
    if (serialMC20.available()) {
        SerialUSB.write(serialMC20.read());
    }
}
