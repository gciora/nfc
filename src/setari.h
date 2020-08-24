#ifndef SETARI_CONFIG_FILE
#define SETARI_CONFIG_FILE

#include <MFRC522.h>

#ifdef ARDUINO_AVR_UNO
    #include "BlynkSimpleStream.h"
#endif

#ifdef ARDUINO_ESP8266_WEMOS_D1MINILITE
    #include <BlynkSimpleEsp8266.h>
#endif

#include "secrete.h"

#define NFC_FW_VER "1.0"
#define NFC_HW_VER "1.0"
#define NFC_SHIELD "PN532"

#define BOARD_NAME "Authorizer Reader"
#define BOARD_VENDOR "Raspberry Pi"

#define APP_DEBUG

#ifdef APP_DEBUG
    constexpr bool debug = true;
#else
    constexpr bool debug = false;
#endif

// Blynk
constexpr unsigned int blynk_serial_speed = 9600;
constexpr unsigned long blynk_config_timeout = 10000L;
constexpr unsigned int zavor_config_timeout = 5000;

constexpr unsigned long TIMEOUT_INTRE_AUTENTIFICARE_SI_IDLE   = 2000L;
constexpr unsigned long TIMEOUT_INTRE_SCHIMBARE_CHEIE_SI_IDLE = 4000L;

#define CHN_SECURE_KEY           V0 // canal pentru receptie cheie sigura de la server Blynk
#define CHN_UPDATE_OR_NO_KEY     V1 // canal pentru receptie daca sa updatam cheia de pe card sau nu (1/0)
#define CHN_CONFIG_KEY_TO_UPDATE V2 // canal pentru receptie care cheie sa o punem pe card (1/2/3 = cheie fabrica, cheie repository, cheie sigura)
#define CHN_ZAVOR                V3 // canal pentru receptie deschiderea manuala a zavorului

#define CHN_CARD_UID   V10 // canal pentru transmitere UID card la server Blynk
#define CHN_VALID_CARD V11 // canal pentru transmitere daca cardul e valid sau nu la server Blynk (1/0)
#define CHN_AUTH       V12 // canal pentru transmiterea starii de autentificare la server Blynk (1/0)


// Dispozitiv
#ifdef ARDUINO_AVR_UNO
    constexpr byte PIN_ZAVOR = 2; // pin unde releul pentru zavor este conectat
#endif
#ifdef ARDUINO_ESP8266_WEMOS_D1MINILITE
    constexpr byte PIN_ZAVOR = D4;
    constexpr byte PIN_LED_BLUE = D0;
#endif

// NFC settings
constexpr byte BLOC_AUTENTIFICARE = 7; //3 acest bloc e folosit pentru verificarea autentificarii

enum ListaStariAuth {
    NOT_AUTH,
    DEFAULT_AUTH_KEY,
    NEW_AUTH_KEY,

    MAX_AUTH_CONFIG_VALUE // keep this entry the last one to use as "length" of enum
                            // for other defines
};

#ifdef ARDUINO_AVR_UNO
    constexpr unsigned char RST_PIN = 9;
    constexpr unsigned char SS_PIN  = 10;
#endif

#ifdef ARDUINO_ESP8266_WEMOS_D1MINILITE
    constexpr unsigned char RST_PIN = D3;
    constexpr unsigned char SS_PIN  = D8;
#endif

constexpr unsigned char debug_serial_rx = 3;
constexpr unsigned char debug_serial_tx = 4;
constexpr unsigned int  debug_speed     = 38400;

constexpr unsigned long system_timer_interval = 1000L;
constexpr unsigned long nfc_timer_interval = 200L; // 500L

 // All keys "A" and "B" are set to FFFFFFFFFFFFh at delivery from factory.
constexpr MFRC522::MIFARE_Key nfc_default_key_a = {.keyByte = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};
constexpr MFRC522::MIFARE_Key nfc_default_key_b = {.keyByte = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

#endif
