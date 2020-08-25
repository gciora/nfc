#ifndef DEBUG_CONFIG_FILE
   #define DEBUG_CONFIG_FILE

   #include "setari.h"

   #ifdef APP_DEBUG  
      #define DEBUG_PRINT(...)   Serial.print(__VA_ARGS__);
      #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__);
   #else
      #define DEBUG_PRINT(...)   ;
      #define DEBUG_PRINTLN(...) ;
   #endif
#endif