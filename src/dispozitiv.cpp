#include "dispozitiv.h"

Dispozitiv dispozitiv;

void Dispozitiv::begin(void) 
{
   stare.set(ASTEPT_CONFIG);
}

void Dispozitiv::run(void) 
{ 
   // implementeaza masina de stari ale dispozitivului
   switch (stare.get()) 
   {
      case ASTEPT_CONFIG:
         //DEBUG_PRINTLN("Astept Cheie secreta");
         break;
      case INITIAL_CONFIG:
         break;
      case RUNNING:
         break;
   }
}

bool Dispozitiv::isRunning(void) 
{
   return RUNNING == stare.get();
}

void Dispozitiv::configurareNeprimita(void) 
{
   if(ASTEPT_CONFIG == stare.get()) 
      stare.set(RUNNING);
};

void Dispozitiv::configurarePrimita(const unsigned char buffer[], size_t length) 
{
   stare.set(INITIAL_CONFIG);
   nfc.save_new_key(buffer, length); // salveaza cheia primita
}

void Dispozitiv::configurareTerminata(void) 
{
   stare.set(RUNNING);
}