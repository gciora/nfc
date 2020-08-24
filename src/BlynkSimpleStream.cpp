#ifdef ARDUINO_AVR_UNO
    #include "BlynkSimpleStream.h"
    BlynkTransportStream _blynkTransport;
    BlynkStream Blynk(_blynkTransport);
#endif
