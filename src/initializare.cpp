#include "debug.h"
#include "initializare.h"
#include <SoftwareSerial.h>
#include <Blynk/BlynkTimer.h>
#include <Blynk/BlynkApi.h>

// Variable-Obiect
Initializare dispozitivInitializare;
BlynkTimer blynk_timer;
char blynk_buffer[20];


// Clasa Initializare
void Initializare::begin() 
{

   #ifdef APP_DEBUG
      Serial.begin(blynk_serial_speed); // seriala hard folosita pentru debug (prin usb - serial monitor)
      #define BLYNK_PRINT Serial
      DEBUG_PRINTLN("             SALUT!");
   #endif

   Blynk.begin(blynk_auth, ssid, pass); // WiFi folosit in comunicare cu serverul Blynk


   // initializare pin zavor
   pinMode(PIN_ZAVOR,OUTPUT);
   digitalWrite(PIN_ZAVOR, LOW); // zavor inchis initial

   // initializare LED scriere cheie
   pinMode(PIN_LED_BLUE,OUTPUT);
   digitalWrite(PIN_LED_BLUE, HIGH); // LED stins initial
   
   // initializare completa dispozitiv
   dispozitiv.begin(); 

   // initializare modul nfc
   nfc.begin();  

   // configurare timere folosite
   blynk_timer.setInterval(dispozitiv_timer_interval, dispozitiv_timer);
   blynk_timer.setInterval(nfc_timer_interval, nfc_timer);
   blynk_timer.setTimer(blynk_config_timeout, timeout_config, 1);
}


// Functii pentru timere
void dispozitiv_timer(void) 
{ 
   // masina stari dispozitiv e executata pe acest timer
   dispozitiv.run();
}

void nfc_timer(void) 
{ 
   // masina stari nfc e executata pe acest timer
   if(dispozitiv.isRunning())
      nfc.run();
}

void timeout_config(void) 
{ 
   // functia pentru timer primire initiala cheie secreta (10s)
     if( nfc.cheie_secreta_primita() ) 
   {
      //DEBUG_PRINTLN("Timeout terminat cu succes - cheie secreta receptionata")
   } 
   else 
   {
      //DEBUG_PRINTLN("Cheie secreta nereceptionata");
      dispozitiv.configurareNeprimita();
   }
}

void timeout_zavor(void) 
{ 
   // functia pentru timer-zavor (5s)
   digitalWrite(PIN_ZAVOR, LOW);
   //DEBUG_PRINTLN("Activare zavor");
}


// Receptionare informatii de la serverul Blynk
BLYNK_WRITE(CHN_CHEIE_SECRETA) 
{ 
   dispozitiv.configurarePrimita((const unsigned char *)param.getBuffer(), param.getLength());
}

BLYNK_WRITE(CHN_OPTIUNE_SCHIMBARE_CHEIE) 
{
   nfc.set_permite_update_cheie(param.asInt());
}

BLYNK_WRITE(CHN_CHEIE_DE_SCHIMBAT) 
{
   nfc.set_cheie_de_schimbat(param.asInt());
}

BLYNK_WRITE(CHN_ZAVOR) 
{
   int zavor = param.asInt();
   if( 1 == zavor ) 
   {
      DEBUG_PRINTLN("Deschid zavor");
      digitalWrite(PIN_ZAVOR, HIGH);
      blynk_timer.setTimer(zavor_config_timeout, timeout_zavor, 1);
   }
}
