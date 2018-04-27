#define GrovePowerPin   12

void setup() {
  pinMode(GrovePowerPin, OUTPUT);
  digitalWrite(GrovePowerPin, HIGH);
  SerialDBG.begin(115200);
  
}

void loop() {  
  SerialDBG.println("Hello Grove UART.");
  delay(500);
}
