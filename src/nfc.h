#ifndef NFC_CONFIG_FILE
#define NFC_CONFIG_FILE
 
#include <SPI.h>
#include <MFRC522.h>

#include "setari.h"
#include "debug.h"

constexpr enum MFRC522::PICC_Command READ_KEYA = MFRC522::PICC_Command::PICC_CMD_MF_AUTH_KEY_A;
constexpr enum MFRC522::PICC_Command READ_KEYB = MFRC522::PICC_Command::PICC_CMD_MF_AUTH_KEY_B;

class NFC {
  public:
    NFC();                         // constructor to initialize internal "key" with 0x00
    void begin();                  // initialize nfc module
    void run(void);                // idle loop processing - named "run" for consistency

    bool is_new_processing(void);  // check if this card we attempt to read it before or not
    bool save_new_key(const unsigned char[], size_t); // store received key from Blynk in internal class
                                   // attribute: "key"
    void set_key_to_update(byte);  // save which key is going to be updated on the insecure card
    void set_permite_update_cheie(byte);

    bool cheie_noua_primita(void);

    void activare_functionalitate(void); // cat timp functionarea nu-i activa, nu ne putem autentifica
    void configureaza_idle(void); // schimba starea la IDLE

  private:
    bool verifica_card_nou(void);
    bool autentificare(void);
    void update_key_on_card(void); // changed key on the Mifare card with "new key"
                                   // stored in "key" attribute
    void read_data(void);          // read data from card
    void write_data(void);         // write data to card
    void detach_current_card(void);
    void reinit(void);             // 
    void error(void);              // 

    bool authenticate_card(const enum MFRC522::PICC_Command, MFRC522::MIFARE_Key, byte);
    void config_intarziere_intoarcere_la_idle(const unsigned long);
    bool read_block(byte block_number, byte buffer[18]);
    bool write_block(byte block_number, byte buffer[16]);
    bool is_valid_card_type();
    bool change_uid(byte new_uid[4]);

    bool ascii_to_byte(const unsigned char *, byte, byte []);
    bool string_digit_to_byte(const char digit, byte *destination);
    unsigned char byte_to_HEX_string(byte byte_number);
    void hex_to_ascii(const unsigned char[], byte, char[]);

    MFRC522 mfrc522;
    StareNfc stare;

    MFRC522::MIFARE_Key key;
    bool received_new_key = false;

    bool update_key   = false;
    bool acces_permis = false;

    ListaStariAuth key_to_update;

    char uid[7];
    byte card_data_buffer[18]; // trebuie sa fie 18 ca sa poata citi libraria. Doar 16 sunt folositi
};

extern NFC nfc;

#endif  // NFC_CONFIG_FILE
