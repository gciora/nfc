#include "debug.h"
#include "stari-mgmt.h"

#if defined(APP_DEBUG)
const char* StareSystemStr[MAX_SYSTEM_CONFIG_VALUE] = {
    "SYS:WAIT_CONFIG",   // waiting for configuration from aplication
    "SYS:CONFIGURING",   // configuration started
    "SYS:RUNNING",       // normal runnig stare
    "SYS:RESET_CONFIG",  // configuration initialized 
    "SYS:SYSTEM_ERROR"   // something went wrong, device should reset in this stare?
};

const char* StareNfcStr[MAX_NFC_CONFIG_VALUE] = {
    "NFC:IDLE",
    "NFC:CARD_NOU",
    "NFC:AUTENTIFICAT",
    "NFC:ZAVOR_DESCHIS",
    "NFC:SCRIERE_CHEIE",
    "NFC:ASTEPTARE",
    "NFC:NFC_ERROR"
};

#endif
