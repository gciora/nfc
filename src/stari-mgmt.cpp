#include "debug.h"
#include "stari-mgmt.h"

#if defined(APP_DEBUG)
    const char* StareDispozitivStr[NUMAR_STARI_DISPOZITIV] = 
    {
        "SYS:WAIT_CONFIG",    // waiting for configuration from aplication
        "SYS:INITIAL_CONFIG", // configuration started
        "SYS:RUNNING",        // normal runnig stare
        "SYS:RESET_CONFIG",   // configuration initialized 
        "SYS:SYSTEM_ERROR"    // something went wrong, device should reset in this stare?
    };

    const char* StareNfcStr[NUMAR_STARI_NFC] = 
    {
        "NFC:VEGHE",
        "NFC:CARD_NOU",
        "NFC:AUTENTIFICAT",
        "NFC:ZAVOR_DESCHIS",
        "NFC:SCHIMBARE_CHEIE",
        "NFC:ASTEPTARE",
        "NFC:NFC_ERROR"
    };
#endif
