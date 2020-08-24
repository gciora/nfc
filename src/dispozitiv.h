#ifndef SYSTEM_CONFIG_FILE
#define SYSTEM_CONFIG_FILE

#include "stari-mgmt.h"
#include "nfc.h"

class Dispozitiv {
    public:
        void begin(void);
        void run(void);
        bool isRunning(void);
        void configurareNePrimita(void);
        void primitConfigurare(const unsigned char buffer[], size_t length);
        void configurare_terminata(void);
    private:
        StareSystem stare;
};

extern Dispozitiv dispozitiv;

#endif