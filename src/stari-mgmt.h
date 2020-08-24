#ifndef STARI_MGMT_CONFIG_FILE
#define STARI_MGMT_CONFIG_FILE

#include <string.h>

#include "debug.h"
enum ListaStariSystem {
    WAIT_CONFIG,
    CONFIGURING,
    RUNNING,
    RESET_CONFIG,
    SYSTEM_ERROR,

    MAX_SYSTEM_CONFIG_VALUE // keep this entry the last one to use as "length" of enum
                            // for other defines
};
enum ListaStariNfc {
    IDLE,          // verifica regulat daca exista card nou (polling)
    CARD_NOU,      // un nou card a fost detectat
    AUTENTIFICAT,  // noul card a fost autentificat cu una dintre chei
    ZAVOR_DESCHIS, // am actionat zavorul, si asteptam timeout zavor
    SCRIERE_CHEIE, // scriem o cheie diferita pe card
    ASTEPTARE,     // Asteptam sa ne intoarcem la idle
    NFC_ERROR,     // nfc working error

    MAX_NFC_CONFIG_VALUE // keep this entry the last one to use as "length" of enum
                         // for other defines
};

#if defined(APP_DEBUG)
extern const char* StareSystemStr[];
extern const char* StareNfcStr[];
#endif

template <typename T>
class Stare {
    protected:
        volatile T stare;
    #if defined(APP_DEBUG)
        const char** StareStr;
    #endif

    public:
        T get() { return stare; }
        bool isState(T stare_noua) { return (stare == stare_noua); }
        void set(T stare_noua) {
            if (stare != stare_noua) {  // change state only if different from current one
                DEBUG_PRINT(StareStr[stare]); DEBUG_PRINT(" => "); DEBUG_PRINTLN(StareStr[stare_noua]);
                stare = stare_noua;
            }
        }
};

class StareSystem: public Stare<ListaStariSystem> {
    public:
        #if defined(APP_DEBUG)
        StareSystem(void) {
            StareStr = StareSystemStr;
        }
        #endif
};
class StareNfc: public Stare<ListaStariNfc> {
    public:
        #if defined(APP_DEBUG)
        StareNfc(void) {
            StareStr = StareNfcStr;
        }
        #endif
};

#endif  // STARI_MGMT_CONFIG_FILE
