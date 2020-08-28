#ifndef SETARI_CONFIG_FILE
    #define SETARI_CONFIG_FILE

    #include <MFRC522.h>
    #include <BlynkSimpleEsp8266.h>
    #include "secret.h"

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

    constexpr unsigned long TIMEOUT_INTRE_AUTENTIFICAT_SI_VEGHE   = 2000L;
    constexpr unsigned long TIMEOUT_INTRE_SCHIMBARE_CHEIE_SI_VEGHE = 4000L;

    #define CHN_CHEIE_SECRETA                  V0 // canal pentru receptie cheie secretă de la server Blynk
    #define CHN_OPTIUNE_SCHIMBARE_STARI_SYSTEM V1 // canal pentru receptie daca sa updatam cheia de pe card sau nu (1/0)
    #define CHN_CHEIE_DE_SCHIMBAT              V2 // canal pentru receptie care cheie sa o punem pe card (1/2 = cheie fabrica/cheie secretă)
    #define CHN_ZAVOR                          V3 // canal pentru receptie deschiderea manuala a zavorului

    #define CHN_CARD_UID   V10 // canal pentru transmitere UID card la server Blynk
    #define CHN_CARD_VALID V11 // canal pentru transmitere daca cardul e valid sau nu la server Blynk (1/0)
    #define CHN_AUTH       V12 // canal pentru transmiterea starii de autentificare la server Blynk (1/0)


    // Dispozitiv

    constexpr byte PIN_ZAVOR = D4;
    constexpr byte PIN_LED_BLUE = D0;

    // NFC settings
    constexpr byte BLOC_AUTENTIFICARE = 7; //acest bloc e folosit pentru verificarea autentificarii

    enum ListaStariAutentificare 
    {
        NEAUTENTIFICAT,
        CHEIE_FABRICA,
        CHEIE_SECRETA,

        NUMAR_STARI_AUTENTIFICARE //retine nr.de stari
    };

    constexpr unsigned char RST_PIN = D3;
    constexpr unsigned char SS_PIN  = D8;

    constexpr unsigned char debug_serial_rx = 3;
    constexpr unsigned char debug_serial_tx = 4;
    constexpr unsigned int  debug_speed     = 38400;

    constexpr unsigned long dispozitiv_timer_interval = 1000L;
    constexpr unsigned long nfc_timer_interval = 200L; // 500L

    // All keys "A" and "B" are set to FFFFFFFFFFFFh at delivery from factory.
    constexpr MFRC522::MIFARE_Key nfc_default_key_a = {.keyByte = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};
    constexpr MFRC522::MIFARE_Key nfc_default_key_b = {.keyByte = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

#endif
