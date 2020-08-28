#include <Arduino.h>

#include <BlynkSimpleEsp8266.h>
#include "debug.h"
#include "initializare.h"

#define BLYNK_PRINT Serial

BLYNK_CONNECTED() 
{
   Blynk.syncAll();
}

void setup() 
{
   dispozitivInitializare.begin();
}

void loop() 
{
   Blynk.run();
   blynk_timer.run();
}
