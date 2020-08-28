#include "debug.h"
#include "initializare.h"
#include <SoftwareSerial.h>
#include <Blynk/BlynkTimer.h>
#include <Blynk/BlynkApi.h>

// Variables - Objects Declaration
Initializare dispozitivInitializare;

#if defined(APP_DEBUG)
   SoftwareSerial debug_serial(debug_serial_rx,debug_serial_tx);
#endif

BlynkTimer blynk_timer;
char blynk_buffer[20];

// Class Functions - Methods
void Initializare::begin() 
{

   #ifdef APP_DEBUG
      Serial.begin(blynk_serial_speed); // seriala hard folosita pentru debug (prin usb - serial monitor)
      #define BLYNK_PRINT Serial
      //DEBUG_PRINT("Hardware v "); DEBUG_PRINTLN( String(NFC_FW_VER).c_str() );
      //DEBUG_PRINT("Firmware v "); DEBUG_PRINTLN( String(NFC_HW_VER).c_str() );
      //DEBUG_PRINT("NFC Shield "); DEBUG_PRINTLN( String(NFC_SHIELD).c_str() );
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

void dispozitiv_timer(void) 
{ 
   // masina stari sistem (dispozitiv) e executata pe acest timer
   dispozitiv.run();
}

void nfc_timer(void) 
{ 
   // masina stari nfc e executata pe acest timer
   // Blynk.virtualWrite(V5, millis() / 1000);
   if(dispozitiv.isRunning())
      nfc.run();
}

void timeout_config(void) 
{ 
   // timer pentru a astepta cheia secreta
                           // configuration from server, so run with default config
   if( nfc.cheie_noua_primita() ) 
   {
      //DEBUG_PRINTLN("Timeout terminat cu succes - cheie secreta receptionata")
   } 
   else 
   {
      //DEBUG_PRINTLN("Cheie secreta nereceptionata");
      //DEBUG_PRINTLN("Folosim cheie din repository");
      dispozitiv.configurareNeprimita();
   }
}

void timeout_zavor(void) 
{ 
   // timer ca sa inchida zavorul dupa un interval prestabilit
   digitalWrite(PIN_ZAVOR, LOW);
   //DEBUG_PRINTLN("Activare zavor");
}

BLYNK_WRITE(CHN_CHEIE_SECRETA) 
{ 
   // Receptie cheie secretă de la aplicatie
   //DEBUG_PRINT("Cheie noua primita (HEX): "); DEBUG_PRINTLN(param.asStr() );
   dispozitiv.configurarePrimita((const unsigned char *)param.getBuffer(), param.getLength());
}

BLYNK_WRITE(CHN_OPTIUNE_SCHIMBARE_STARI_SYSTEM) 
{
   nfc.set_permite_update_cheie(param.asInt());
}

BLYNK_WRITE(CHN_CHEIE_DE_SCHIMBAT) 
{
   nfc.set_key_to_update(param.asInt());
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
