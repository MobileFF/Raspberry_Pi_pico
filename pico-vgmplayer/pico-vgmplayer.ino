// https://github.com/zenmai/portableMDXPlayer/blob/master/YM2151.cpp
#include "YM2151.h"

//   ** MISO - pin 16
//   ** MOSI - pin 19
//   ** CS   - pin 17
//   ** SCK  - pin 18
#define PIN_SD_MOSI       PIN_SPI0_MOSI
#define PIN_SD_MISO       PIN_SPI0_MISO
#define PIN_SD_SCK        PIN_SPI0_SCK
#define PIN_SD_SS         PIN_SPI0_SS
#include <RP2040_SD.h>

unsigned int vgmidx=0;
unsigned int vgmpos = 0x00;
unsigned int vgm_offset = 0xb0;
bool vgmend = false;

unsigned long startTime;
unsigned long duration;

unsigned int trackNo = 0;
unsigned int numOfTracks = 0;
unsigned int maxTracks = 25;
bool isPause = false;

File dataFile;
String files[25];

Sd2Card card;
SdVolume volume;

#define BAUD_RATE 9600

uint8_t getByte() {
    uint8_t ret = 0x66;
    if (dataFile.available()) {
      ret = dataFile.read();
    }
    vgmpos++;
    return ret;
}

unsigned int read16() {
    return getByte() + (getByte() << 8);
}

void pause(long samples){
    duration = ((1000.0 / (44100.0 / (float)samples)) * 1000);
    startTime = micros();
}

void vgmplay() {
    if(isPause) {
        return;
    }
    if((micros() - startTime) <= duration) {
        return;
    }
    
    byte command = getByte();
    uint8_t reg;
    uint8_t dat;

    switch (command) {
        case 0x54:
            // YM2151
            reg = getByte();
            dat = getByte();
            YM2151.write(reg, dat);
            break;
        case 0x61:
            pause(read16());
            break;
        case 0x62:
            pause(735);
            break;
        case 0x63:
            pause(882);
            break;
        case 0x66:
            vgmend = true;
            break;
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x76:
        case 0x77:
        case 0x78:
        case 0x79:
        case 0x7A:
        case 0x7B:
        case 0x7C:
        case 0x7D:
        case 0x7E:
        case 0x7F:
            pause((command & 0x0f) + 1);
            break;
        default:
            break;
    }
}

int getFiles() {

  int count = 0;
  File dir = SD.open("/");

  while (count < maxTracks) {
    
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      //Serial.println("no more files");
      break;
    }
    if (entry.isDirectory() || !(String(entry.name()).endsWith(".VGM"))) {
      entry.close();
      continue;
    }
    Serial.print(count);
    Serial.print(":");
    Serial.println(entry.name());
    files[count]=entry.name();
    entry.close();
    count++;
  }

  dir.close();
  return count;
}

void setup() {
  // log initialize
  Serial.begin(115200);
  Serial.println("VGMDUINO JUKEBOX for Raspberry Pi pico with microSD");

  // LED HIGH
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);

  // microSD initialize

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, PIN_SD_SS))
  {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");

    while (1);
  }
  else
  {
    Serial.println("Wiring is correct and a card is present.");
  }

  // Software Serial
  Serial1.begin(BAUD_RATE);
  Serial1.setTX(0);
  Serial1.setRX(1);
  
  pinMode(PIN_SD_SS,OUTPUT);
  while (!SD.begin(PIN_SD_SS)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    delay(3000);
  }
  Serial.println("card initialized.");
  numOfTracks = getFiles();

  // YM2151 initialize
  YM2151.begin();
  delay(400);
  
}

void checkUART() {
    if(Serial1.available()>0){
        digitalWrite(LED_BUILTIN,LOW);
        byte buf[10] = {0,0,0,0,0,0,0,0,0,0};;
        Serial1.readBytes(buf,10);
        for(int i=0;i<10;i++) {
          Serial.print(buf[i],HEX);
          Serial.print(":");
        }
        Serial.println();
        if(buf[0]==0x7e && buf[1] == 0xFF && buf[2] == 0x06) {
          switch(buf[3]) {
            case 0x01:
              Serial.println("NEXT");
              vgmend = true;
              return;
            case 0x02:
              Serial.println("PREV");
              vgmend = true;
              trackNo -= 2;
              return;
            case 0x0D:
              Serial.println("PLAY");
              isPause = false;
              return;
            case 0x0E:
              Serial.println("PAUSE");
              isPause = true;
              return;                 
          }
        }
    }
  
}

void bootselBtn() {
    if(BOOTSEL){
      delay(10);
      while(BOOTSEL);
      delay(10);
      rp2040.reboot();
    }
}

void loop() {
    bootselBtn();
    
    vgmend = false;
    vgmpos = 0xb0;

    dataFile = SD.open(files[trackNo]);
    Serial.print(trackNo);
    Serial.print(":");
    Serial.println(files[trackNo]);
  
    int idx=0;
    if (dataFile) {
      while (dataFile.available() && idx++ < vgm_offset) {
        dataFile.read();
      }
    } else {
      Serial.print("error opening ");
      Serial.println(files[trackNo]);
      vgmend=true;
    }
    
    while(!vgmend) {
        vgmplay();
        checkUART();
        bootselBtn();
        digitalWrite(LED_BUILTIN,HIGH);

    }
    dataFile.close();

    YM2151.begin();
    delay(1000);

    trackNo++;
    if (trackNo>=numOfTracks) {
      trackNo=0;
    }
}
