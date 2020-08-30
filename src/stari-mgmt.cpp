#include "debug.h"
#include "stari-mgmt.h"


#if defined(APP_DEBUG)
    const char* StareDispozitivStr[NUMAR_STARI_DISPOZITIV] = 
    {
        "SYS:ASTEPT_CONFIG",    // asteptare cheie pornire de la Blynk 
        "SYS:INITIAL_CONFIG", // configurare pt. rulare (running)
        "SYS:RUNNING"        
    };

    const char* StareNfcStr[NUMAR_STARI_NFC] = 
    {
        "NFC:VEGHE",
        "NFC:CARD_NOU",
        "NFC:AUTENTIFICAT",
        "NFC:ZAVOR_DESCHIS",
        "NFC:SCHIMBARE_CHEIE",
        "NFC:ASTEPTARE"
    };
#endif 