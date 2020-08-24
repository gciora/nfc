#include "dispozitiv.h"

Dispozitiv dispozitiv;

void Dispozitiv::begin(void) {
   stare.set(WAIT_CONFIG);
}

void Dispozitiv::run(void) { // this is the actual stare machine of the application
   switch (stare.get()) {
      case WAIT_CONFIG:
         //DEBUG_PRINTLN("Astept Configurare");
         break;
      case CONFIGURING:
         stare.set(CONFIGURING);
         break;
      case RUNNING:
         break;
      case RESET_CONFIG:
         break;
      case SYSTEM_ERROR:
         //DEBUG_PRINTLN("Eroare => Reseteaza Dispozitiv!");
      default:
         stare.set(SYSTEM_ERROR);
         break;
   }
}

bool Dispozitiv::isRunning(void) {
   return RUNNING == stare.get();
}

void Dispozitiv::configurareNePrimita(void) {
   if(WAIT_CONFIG == stare.get()) {
      stare.set(RUNNING);
   }
};

void Dispozitiv::primitConfigurare(const unsigned char buffer[], size_t length) {
   stare.set(CONFIGURING);
   nfc.save_new_key(buffer, length); // save received key
}

void Dispozitiv::configurare_terminata(void) {
   stare.set(RUNNING);
}