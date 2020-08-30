#ifndef STARI_MGMT_CONFIG_FILE
    #define STARI_MGMT_CONFIG_FILE

    #include <string.h>

    #include "debug.h"

    enum ListaStariDispozitiv 
    {
        ASTEPT_CONFIG,
        INITIAL_CONFIG,
        RUNNING,

        NUMAR_STARI_DISPOZITIV //retine nr.de stari 
    };

    enum ListaStariNfc 
    {
        VEGHE,          // verifica regulat daca exista card nou (polling)
        CARD_NOU,      // un nou card a fost detectat
        AUTENTIFICAT,  // noul card a fost autentificat cu una dintre chei
        ZAVOR_DESCHIS, // am actionat zavorul, si asteptam timeout zavor
        SCHIMBARE_CHEIE, // scriem o cheie diferita pe card
        ASTEPTARE,     // Asteptam sa ne intoarcem la VEGHE

        NUMAR_STARI_NFC //retine nr.de stari
    };

     enum ListaStariAutentificare 
    {
        NEAUTENTIFICAT,
        CHEIE_FABRICA,
        CHEIE_SECRETA,

        NUMAR_STARI_AUTENTIFICARE //retine nr.de stari
    };


   #if defined(APP_DEBUG)
        extern const char* StareDispozitivStr[];
        extern const char* StareNfcStr[];
    #endif
  

   template <typename T>
    class Stare 
    {
        protected:
            volatile T stare;
            #if defined(APP_DEBUG)
                const char** StareStr;
            #endif

        public:
            T get() { return stare; }
            bool isState(T stare_noua) { return (stare == stare_noua); }
            void set(T stare_noua) 
            {
                if (stare != stare_noua) 
                {  
                    // change state only if different from current one
                    DEBUG_PRINT(StareStr[stare]); DEBUG_PRINT(" => "); DEBUG_PRINTLN(StareStr[stare_noua]);
                    stare = stare_noua;
                }
            }   
    };


    class StareDispozitiv: public Stare<ListaStariDispozitiv> 
    {
        public:
            #if defined(APP_DEBUG)
                StareDispozitiv(void) 
                {
                    StareStr = StareDispozitivStr;
                }
            #endif
    };


    class StareNfc: public Stare<ListaStariNfc> 
    {
        public:
            #if defined(APP_DEBUG)
                StareNfc(void) 
                {
                    StareStr = StareNfcStr;
                }
            #endif
    };


#endif  // STARI_MGMT_CONFIG_FILE
