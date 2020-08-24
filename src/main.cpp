#include <Arduino.h>

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>

#include "debug.h"
#include "initializare.h"

BLYNK_CONNECTED() 
{
   Blynk.syncAll();
}

void setup() 
{
   deviceInitializare.begin();
}

void loop() 
{
   Blynk.run();
   blynk_timer.run();
}
