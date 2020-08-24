#include <Arduino.h>

#ifdef ARDUINO_AVR_UNO
   #include "BlynkSimpleStream.h"
#endif
#ifdef ARDUINO_ESP8266_WEMOS_D1MINILITE
   #define BLYNK_PRINT Serial
   #include <BlynkSimpleEsp8266.h>
#endif

#include "debug.h"
#include "initializare.h"

BLYNK_CONNECTED() {
   Blynk.syncAll();
}

void setup() {
   deviceInitializare.begin();
}

void loop() {
   Blynk.run();
   blynk_timer.run();
}
