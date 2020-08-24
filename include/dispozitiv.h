#ifndef SYSTEM_CONFIG_FILE
#define SYSTEM_CONFIG_FILE

#include "stari-mgmt.h"
#include "nfc.h"

class Dispozitiv 
{
    public:
        void begin(void);
        void run(void);
        bool isRunning(void);
        void configurareNeprimita(void);
        void configurarePrimita(const unsigned char buffer[], size_t length);
        void configurareTerminata(void);
    private:
        StareSystem stare;
};

extern Dispozitiv dispozitiv;

#endif