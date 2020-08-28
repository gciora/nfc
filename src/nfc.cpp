#include "debug.h"
#include "setari.h"
#include "stari-mgmt.h"
#include "dispozitiv.h"
#include "initializare.h"
#include "nfc.h"

NFC nfc;

NFC::NFC() 
{
  for(byte i = 0; i < 6; i++) 
  {
    key.keyByte[i] = 0;
  }
}

void NFC::begin() 
{
  SPI.begin();
  mfrc522.PCD_Init(SS_PIN, RST_PIN);
  mfrc522.PCD_SetAntennaGain(0x50);
  // Initializare machina de stari
  stare.set(VEGHE);
}

void NFC::run(void) 
{ 
  // VEGHE running on timer
  ListaStariNfc stare_curenta = stare.get();
  switch(stare_curenta) 
  {
    case VEGHE:
      if(apare_card()) stare.set(CARD_NOU);
      break;
    case CARD_NOU:
      if(autentificare()) 
        stare.set(AUTENTIFICAT);
      else 
        {
          config_intarziere_intoarcere_la_VEGHE(TIMEOUT_INTRE_AUTENTIFICAT_SI_VEGHE);
        }
        break;
    case AUTENTIFICAT:
      if(cheie_schimbata) stare.set(SCHIMBARE_CHEIE);
      else
      {
        if(acces_permis) stare.set(ZAVOR_DESCHIS);
        else config_intarziere_intoarcere_la_VEGHE(TIMEOUT_INTRE_AUTENTIFICAT_SI_VEGHE);
      }
      break;
    case SCHIMBARE_CHEIE:
      digitalWrite(PIN_LED_BLUE,LOW);
      schimbare_cheie();
      config_intarziere_intoarcere_la_VEGHE(TIMEOUT_INTRE_SCHIMBARE_CHEIE_SI_VEGHE);
      break;
    case ZAVOR_DESCHIS:
      digitalWrite(PIN_ZAVOR,HIGH);
      blynk_timer.setTimer(zavor_config_timeout, timeout_zavor, 1);
      acces_permis = false;
      config_intarziere_intoarcere_la_VEGHE(TIMEOUT_INTRE_AUTENTIFICAT_SI_VEGHE);
      break;
    case ASTEPTARE:
      DEBUG_PRINTLN("Asteptam sa ne intoarcem la VEGHE");
    case NFC_ERROR:
      reinit();
      break;
  }
}

bool NFC::autentificare(void) 
{
  bool autentificat = false;
  if(primire_cheie_noua) 
  { 
    // cheia noua, secretă, a fost receptionata
    // incercam autentificarea cu cheia noua (secreta)
    // primita de la aplicatie si salvata in membrul clasei
    // cu numele "key"
    if( autentificare_card(READ_KEYA, key, BLOC_AUTENTIFICARE) ) 
    {
      autentificat = true;
      acces_permis = true;
      DEBUG_PRINTLN("Ne-am autentificat cu cheia secretă");
      DEBUG_PRINTLN("Accesul este permis");
      Blynk.virtualWrite(CHN_AUTH,CHEIE_SECRETA); // informam serverul ca autentificarea a fost reusita
    } 
    else 
    {
      retragere_card(); // trebuie reinitializata comunicatia cu cardul pentru a incerca o cheie noua
      if(! apare_card()) 
      { // dupa deconectare cardul trebuie sa se re-connecteze (se comporta ca un card nou)
        return autentificat; // daca acest card nu s-a reconectat, nu mai putem face nimic
      }
    }
  }
  if(! autentificat) { // autentificarea cu cheia noua nu a fost posibila
    // incercam autentificarea cu cheia implicita din fabrica (nfc_default_key_a din setari.h)
    if( autentificare_card(READ_KEYA, nfc_default_key_a, BLOC_AUTENTIFICARE) ) 
    {
      autentificat = true;
      DEBUG_PRINTLN("Autentificat cu cheia din fabrica");
      Blynk.virtualWrite(CHN_AUTH,CHEIE_FABRICA);
    } 
    else 
    {
      DEBUG_PRINTLN("Nu ne-am putut autentifica la acest card");
      return autentificat;
    }
  }
  return autentificat;
}

void NFC::schimbare_cheie(void) 
{
  int i = 0;

  if(cheie_schimbata) 
  {
    DEBUG_PRINT("Schimbam cheia pe card cu: "); DEBUG_PRINTLN(key_to_update);

    // configuram bitii de acces la card
    //card_data_buffer[6] = 0x80;
    //card_data_buffer[7] = 0x07;
    //card_data_buffer[8] = 0xFF;
    
    mfrc522.MIFARE_SetAccessBits(
      &(card_data_buffer[6]),/*adresa biti configurare securitatepentru sectorul curent(autentificat)*/
      (byte)0b000,           /*biti acces bloc zero in ordinea C1 C2 C3 (C1 MSB)*/
      (byte)0b000,           /*biti acces bloc unu in ordinea C1 C2 C3 (C1 MSB)*/
      (byte)0b000,           /*biti acces bloc doi in ordinea C1 C2 C3 (C1 MSB)*/
      (byte)0b000            /*biti acces bloc trei (sector trailer block) in ordinea C1 C2 C3 (C1 MSB)*/
    );

    // configuram byte-ul utilizator - in general nefolosit
    card_data_buffer[9] = 0xFF;

    switch (key_to_update) 
    {
      case CHEIE_FABRICA:
        // punem noua cheie A
        for(i = 0; i < 6; i++) 
        {
        card_data_buffer[i] = nfc_default_key_a.keyByte[i];
        }
        // punem noua cheie B
        for(i = 10; i < 16; i++) 
        {
        card_data_buffer[i] = nfc_default_key_b.keyByte[i];
        }
        break;
      case CHEIE_SECRETA:
        // punem noua cheie A
        for(i = 0; i < 6; i++) 
        {
          card_data_buffer[i] = key.keyByte[i];
        }
        // punem noua cheie B
        for(i = 10; i < 16; i++) 
        {
          card_data_buffer[i] = key.keyByte[i];
        }
        break;
    }

    if( write_block(BLOC_AUTENTIFICARE, card_data_buffer) ) 
    {
      DEBUG_PRINTLN("Cheie schimbata corect");
    }
      //255:255:255:255:255:255:105:128:7:255:0:0:0:0:0:0:
      //55:100:145:219:254:202:105:128:7:255:0:0:0:0:0:0:
  }
}

bool NFC::apare_card(void) 
{
  bool result = false;
  if ( mfrc522.PICC_IsNewCardPresent()) 
  { 
    // Card nou detectat (sau redetectat in timpul unei autentificari)
    if ( mfrc522.PICC_ReadCardSerial()) 
    { 
      // Numar serial card citit cu succes
      //Blynk.virtualWrite(CHN_CARD_UID, blynk_buffer, mfrc522.uid.size >> 1);
      if( validare_card() ) 
      { 
        // verifica daca avem un card pe care stim sa-l procesam
        Blynk.virtualWrite(CHN_CARD_VALID,1); // inform server CARD is VALID
        result = true;
      } 
      else 
      {
        Blynk.virtualWrite(CHN_CARD_VALID,0); // inform server CARD is NON-VALID
        result = false;
      } 
    }
  }
  return result;
}

void NFC::set_key_to_update(byte auth) 
{
  key_to_update = (ListaStariAutentificare) auth;
  DEBUG_PRINT("Cheie pe care o schimbam pe card: "); DEBUG_PRINTLN(auth);
}

void NFC::set_permite_update_cheie(byte updatam) 
{
  cheie_schimbata = (1 == updatam);
  DEBUG_PRINT("Updatam cheie: "); DEBUG_PRINTLN(updatam);
}

bool NFC::save_new_key(const unsigned char buffer[], size_t length) 
{
  unsigned int string_index = 0;
  DEBUG_PRINT("Salvam cheia noua: ")
  if(ascii_to_byte(buffer, length, key.keyByte)) 
  {
    DEBUG_PRINT("Cheie salvata corect: ");
    for( string_index = 0; string_index < length/2; string_index++ ) 
    {
      DEBUG_PRINT( key.keyByte[string_index] );
      DEBUG_PRINT(":");
    }
    DEBUG_PRINTLN("");
    primire_cheie_noua = true; // Cheie adaugat cu succes
    dispozitiv.configurareTerminata();
  } 
  else 
  {
    DEBUG_PRINTLN("Cheia nu este corecta");
    primire_cheie_noua = false;
  }
  return primire_cheie_noua;
}

bool NFC::autentificare_card(const enum MFRC522::PICC_Command key_type, MFRC522::MIFARE_Key key, byte block) { //
  MFRC522::StatusCode status;
  byte i;

  status = mfrc522.PCD_Authenticate(key_type, block, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status == MFRC522::STATUS_OK) 
  {
    DEBUG_PRINT("Card autentificat: ");
  } 
  else 
  {
    DEBUG_PRINT(("Autentificare esuata: "));
  }
  for(i = 0; i < mfrc522.uid.size; i++) 
  {
    DEBUG_PRINT( mfrc522.uid.uidByte[i] ); //sak
    DEBUG_PRINT( ":" );
  }
  DEBUG_PRINTLN("")

  if(status != MFRC522::STATUS_OK) 
  {
    DEBUG_PRINT("Eroare autentificare: ");
    DEBUG_PRINTLN(mfrc522.GetStatusCodeName(status));
  }

  return status == MFRC522::STATUS_OK;
}

bool NFC::read_block(byte block_number, byte buffer[18]) 
{
  MFRC522::StatusCode status;

  byte len = 18;

  status = mfrc522.MIFARE_Read(block_number, buffer, &len);
  if (status == MFRC522::STATUS_OK) 
  {
    return true;
  } 
  else 
  {
    DEBUG_PRINT("Citire esuata: ");
    DEBUG_PRINTLN(mfrc522.GetStatusCodeName(status));
  }
  return false;
}

bool NFC::write_block(byte block_number, byte buffer[16]) 
{
  MFRC522::StatusCode status;

  byte len = 16;

  status = mfrc522.MIFARE_Write(block_number, buffer, len);
  if (status == MFRC522::STATUS_OK) 
  {
    return true;
  } 
  else 
  {
    DEBUG_PRINT("Scriere Esuata: ");
    DEBUG_PRINTLN(mfrc522.GetStatusCodeName(status));
  }
  return false;
}

bool NFC::validare_card() 
{
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  DEBUG_PRINT("Tip PICC (Card): ");
  DEBUG_PRINT(mfrc522.PICC_GetTypeName(piccType));
  DEBUG_PRINT(" (SAK ");
  DEBUG_PRINT( mfrc522.uid.sak );
  DEBUG_PRINTLN(")");
  if (  piccType != MFRC522::PICC_TYPE_MIFARE_MINI 
    &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
    &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    DEBUG_PRINTLN("Numai carduri MIFARE Classic pot fi folosite pentru autentificare.");
    return false;
  }
  return true;
}

void NFC::retragere_card(void) 
{
  MFRC522::StatusCode result;
  result = mfrc522.PICC_HaltA();
  if(MFRC522::STATUS_OK != result) 
  {
    DEBUG_PRINTLN("Nu pot opri comunicatia cu cardul (HALT)");
  }
  mfrc522.PCD_StopCrypto1();
  mfrc522.PCD_Init(SS_PIN, RST_PIN);
}

void timeout_intoarcere_la_VEGHE(void) 
{ 
  // apelata de timer ca sa activeze functionalitatea
   nfc.intrare_stare_VEGHE();
}

void NFC::config_intarziere_intoarcere_la_VEGHE(const unsigned long mili_secunde) 
{
  acces_permis = false;
  retragere_card();
  if(SCHIMBARE_CHEIE == stare.get()) 
  {
    digitalWrite(PIN_LED_BLUE,HIGH);
  }
  stare.set(ASTEPTARE);
  blynk_timer.setTimer(mili_secunde, timeout_intoarcere_la_VEGHE, 1);
  DEBUG_PRINT("Asteapta pentru o noua operatie: "); DEBUG_PRINTLN(mili_secunde);
}

void NFC::intrare_stare_VEGHE(void) 
{
  stare.set(VEGHE);
  DEBUG_PRINTLN("Inapoi la VEGHE");
}

void NFC::read_data(void) 
{
  stare.set(VEGHE);
  retragere_card();
}
void NFC::write_data(void) 
{
  stare.set(VEGHE);
}
void NFC::reinit(void) 
{
  //TODO: reseteaza sistemul
}
void NFC::error(void) 
{

}

bool NFC::change_uid(byte new_uid[4]) 
{
  if ( mfrc522.MIFARE_SetUid(new_uid, (byte)4, true) ) 
  {
    DEBUG_PRINTLN("Wrote new UID to card.");
    return true;
  }
  return false;
}

bool NFC::ascii_to_byte(const unsigned char *ascii_string, byte size, byte result[]) 
{
  bool conversion_result = true;
  byte string_index = 0, result_index = 0;
  byte tmp;
  for(string_index = 0; string_index < size; string_index++) 
  {
    conversion_result = string_digit_to_byte(ascii_string[string_index++], &tmp);
    if(conversion_result) result[result_index] = tmp << 4;
    else 
    {
      conversion_result = false;
      break;
    }
    conversion_result = string_digit_to_byte(ascii_string[string_index], &tmp);
    if(conversion_result) result[result_index] |= tmp;
    else 
    {
      conversion_result = false;
      break;
    }
    // DEBUG_PRINT("Byte convertit (DEC): "); DEBUG_PRINTLN( String(result[result_index]).c_str() );
    result_index++;
  }
  return conversion_result;
}
bool NFC::string_digit_to_byte(const char digit, byte *destination) 
{
  // DEBUG_PRINT("Convertim Byte (HEX): "); DEBUG_PRINTLN( String(digit).c_str() );
  if(digit >= 'A' && digit <= 'Z') 
  {
    *destination = digit - 'A' + 10;
    return true;
  }
  if(digit >= 'a' && digit <= 'z') 
  {
    *destination = digit - 'a' + 10;
    return true;
  }
  if(digit >= '0' && digit <= '9') 
  {
    *destination = digit -'0';
    return true;
  }
  return false;
}

void NFC::hex_to_ascii(const unsigned char *hex_array, byte size, char *result) 
{
  byte src_index = 0, dst_index = 0;
  for(src_index = 0; src_index < size; src_index ++ ) 
  {
    result[dst_index++] = byte_to_HEX_string(hex_array[src_index] >> 4);
    result[dst_index++] = byte_to_HEX_string(hex_array[src_index]);
  }
  result[dst_index] = 0; // Terminator sir caractere
  DEBUG_PRINT("HEX_STRING: "); DEBUG_PRINTLN(dst_index);
}

unsigned char NFC::byte_to_HEX_string(byte byte_number) 
{
  if((byte_number & 0x0F) < 10) return '0' + (byte_number & 0x0F);
  return 'A' + (byte_number & 0x0F) - 10;
}

bool NFC::cheie_noua_primita(void) 
{
  return primire_cheie_noua;
}

//byte = PCD_GetAntennaGain();
//void PCD_SetAntennaGain(byte mask);
//RxGain_18dB    RxGain_23dB    RxGain_18dB_2    RxGain_23dB_2    RxGain_33dB    RxGain_38dB    RxGain_43dB    RxGain_48dB    RxGain_min    RxGain_avg    RxGain_max
//void PCD_AntennaOn();  void PCD_AntennaOff(); bool PCD_PerformSelfTest();void PCD_SoftPowerDown();  void PCD_SoftPowerUp();
// void MIFARE_SetAccessBits(byte *accessBitBuffer, byte g0, byte g1, byte g2, byte g3);  bool MIFARE_OpenUidBackdoor(bool logErrors);
// bool MIFARE_SetUid(byte *newUid, byte uidSize, bool logErrors);  bool MIFARE_UnbrickUidSector(bool logErrors);
