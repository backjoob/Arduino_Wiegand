/*
  Example using the Wiegand library
  Author: backjoob
  7 September 2018
*/

#include <Wiegand.h>

void setup() {
  // Initialize the Wiegand protocol to use pin 2 for the 0 line and pin 3
  // for the 1 line. These must be digital interrupt pins.
  Wiegand::initialize(2, 3);
  // Initialize Serial so we can see output.
  Serial.begin(115200);
}

void loop() {
  // Check if the buffer contains data
  if(Wiegand::avail()){
    Serial.print('[');
    do {
      // Pull data from buffer
      Serial.print(Wiegand::dequeue_buf());
      Serial.print(", ");
    } while(Wiegand::avail());
    Serial.print(']');
    Serial.println();
  } else {
    Serial.println("[]");
  }
  delay(500);
}
