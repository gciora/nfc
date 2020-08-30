#ifndef NFC_CONFIG_FILE
  #define NFC_CONFIG_FILE
 
  #include <SPI.h>
  #include <MFRC522.h>

  #include "setari.h"
  #include "debug.h"

  constexpr enum MFRC522::PICC_Command READ_KEYA = MFRC522::PICC_Command::PICC_CMD_MF_AUTH_KEY_A;
  constexpr enum MFRC522::PICC_Command READ_KEYB = MFRC522::PICC_Command::PICC_CMD_MF_AUTH_KEY_B;

  class NFC 
  {
    public:
      NFC();                         // constructor pentru a initializa atributul "key" cu 0x00
      void begin();                  // initializeaza modulul nfc
      void run(void);                // implementeaza masina de stari pt. nfc 

      bool is_new_processing(void);  // check if this card we attempt to read it before or not
      bool save_new_key(const unsigned char[], size_t); // retine cheia primita de la Blynk in "key"
      void set_cheie_de_schimbat(byte);  // care cheie se modifica
      void set_permite_update_cheie(byte);

      bool cheie_secreta_primita(void);

      void activare_functionalitate(void); // cat timp functionarea nu-i activa, nu ne putem autentifica
      void intrare_stare_veghe(void); // schimba starea la VEGHE

    private:
      bool apare_card(void);
      bool autentificare(void);
      void schimbare_cheie(void); // schimba cheia pe card cu cea din "key"
      void write_data(void);       // scriere pe card
      void retragere_card(void);

      bool autentificare_card(const enum MFRC522::PICC_Command, MFRC522::MIFARE_Key, byte);
      void config_intarziere_intoarcere_la_veghe(const unsigned long);
      bool write_block(byte block_number, byte buffer[16]);
      bool validare_card();

      bool ascii_to_byte(const unsigned char *, byte, byte []);
      bool string_digit_to_byte(const char digit, byte *destination);

      MFRC522 mfrc522;
      StareNfc stare;

      MFRC522::MIFARE_Key key;
      bool primire_cheie_noua = false;

      bool schimb_cheie = false;
      bool acces_permis = false;

      ListaStariAutentificare cheie_de_schimbat;

      char uid[7];
      byte card_data_buffer[18]; // trebuie sa fie 18 ca sa poata citi libraria. Doar 16 sunt folositi
  };

  extern NFC nfc;

#endif  // NFC_CONFIG_FILE
