#include "MC20_Common.h"
#include "MC20_Arduino_Interface.h"


GPSTracker gpsTracker = GPSTracker();

void setup() {
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  SerialUSB.begin(115200);
  // while(!SerialUSB);

  gpsTracker.Power_On();
  SerialUSB.println("Power On!");


}

void loop() {
  /* Debug */
  if(SerialUSB.available()){
    serialMC20.write(SerialUSB.read());
  }
  if(serialMC20.available()){     
    SerialUSB.write(serialMC20.read()); 
  }
}
