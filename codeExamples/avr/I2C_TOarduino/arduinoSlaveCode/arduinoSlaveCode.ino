#include <Wire.h>

volatile byte x = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Los gehts");
  // Define the LED pin as Output
  pinMode (LED_BUILTIN, OUTPUT);
  // Start the I2C Bus as Slave on address 9
  Wire.begin(8); 
  Wire.setClock(100000L);
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
  Serial.println("Initialisierung abgeschlossen ...");
}

void receiveEvent(int bytes) {
  x = Wire.read();    // read one character from the I2C
  Serial.println(x);
}

void loop() {
  //If value received is 0 blink LED for 200 ms
  if (x == 0) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  //If value received is 3 blink LED for 400 ms
  if (x == 3) {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
