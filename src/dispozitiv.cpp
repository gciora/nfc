#include "dispozitiv.h"

Dispozitiv dispozitiv;

void Dispozitiv::begin(void) 
{
   stare.set(WAIT_CONFIG);
}

void Dispozitiv::run(void) 
{ 
   // this is the actual stare machine of the application
   switch (stare.get()) 
   {
      case WAIT_CONFIG:
         //DEBUG_PRINTLN("Astept Configurare");
         break;
      case INITIAL_CONFIG:
         stare.set(INITIAL_CONFIG);
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

bool Dispozitiv::isRunning(void) 
{
   return RUNNING == stare.get();
}

void Dispozitiv::configurareNeprimita(void) 
{
   if(WAIT_CONFIG == stare.get()) 
      stare.set(RUNNING);
};

void Dispozitiv::configurarePrimita(const unsigned char buffer[], size_t length) 
{
   stare.set(INITIAL_CONFIG);
   nfc.save_new_key(buffer, length); // save received key
}

void Dispozitiv::configurareTerminata(void) 
{
   stare.set(RUNNING);
}