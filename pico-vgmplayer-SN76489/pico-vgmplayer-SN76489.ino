// https://github.com/zenmai/portableMDXPlayer/blob/master/SN76489.cpp
#include "SN76489.h"
#include <Arduino.h>

//   ** MISO - pin 16
//   ** MOSI - pin 19
//   ** CS   - pin 17
//   ** SCK  - pin 18
//#define PIN_SD_MOSI       PIN_SPI0_MOSI
//#define PIN_SD_MISO       PIN_SPI0_MISO
//#define PIN_SD_SCK        PIN_SPI0_SCK
//#define PIN_SD_SS         PIN_SPI0_SS
#define PIN_SD_MOSI       19
#define PIN_SD_MISO       16
#define PIN_SD_SCK        18
#define PIN_SD_SS         17
#include <RP2040_SD.h>

//bool pristine = true;

//uint8_t vgmdata[65535];
unsigned int vgmidx = 0x00;
unsigned int vgmpos = 0x00;
unsigned int vgm_offset = 0x00;
unsigned int eof_offset = 0x00;
unsigned int loop_offset = 0x00;
unsigned int loop_samples = 0x00;
float vgm_version = 0;
bool vgmend = false;

unsigned long startTime;
unsigned long duration;

//bool onload = false;

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
//uint8_t getByte() {
//    uint8_t ret = vgmdata[vgmpos++];
//    return ret;
//}

unsigned int read16() {
    return getByte() + (getByte() << 8);
}

unsigned int read32(int command) {
    return command + (getByte()<<8) + (getByte()<<16) + (getByte()<<24);
}

void pause(long samples){
    duration = ((1000.0 / (44100.0 / (float)samples)) * 1000);
    startTime = micros();
}


void vgm_header(byte command) {
//  Serial.print(vgmpos,HEX);
//  Serial.print(":");
//  Serial.println(command,HEX);
  
  byte value1;
  unsigned int value2;
  switch(vgmpos) {
    case 0x04+1:
      eof_offset = read32(command);
      //Serial.println();
      Serial.print("EOF OFFSET:");
      Serial.println(eof_offset,HEX);
      break;
    case 0x08+1:
      char hexString1[3];
      sprintf(hexString1, "%02X", command);
      value1 = getByte();
      char hexString2[3];
      sprintf(hexString2, "%02X", value1);
      strcat(hexString2,".");
      strcat(hexString2,hexString1);
      vgm_version = atof(hexString2);
      //Serial.println();
      Serial.print("VGM VERSION:");
      Serial.println(vgm_version);
      break;
    case 0x1c+1:
      loop_offset = read32(command);
      //Serial.println();
      Serial.print("LOOP OFFSET:");
      Serial.println(loop_offset,HEX);
      break;
    case 0x20+1:
      loop_samples = read32(command);
      //Serial.println();
      Serial.print("LOOP SAMPLES:");
      Serial.println(loop_samples,HEX);
      break;
    case 0x34+1:
      value2 = read32(command);
      if (vgm_version < 1.5) {
        vgm_offset = 0x40;
      } else if (value2==0x0c) {
        vgm_offset = 0x40;
      } else {
        vgm_offset = value2;
      }
      //Serial.println();
      Serial.print("VGM OFFSET:");
      Serial.println(vgm_offset,HEX);
      break;
  }    
}

void vgmplay() {
    if(isPause) {
        return;
    }
    if((micros() - startTime) <= duration) {
        return;
    }
    
    byte command = getByte();

    if ((vgm_offset == 0x00)&&(vgmpos <= 0xff)) {
      vgm_header(command);
      return;
    }

    if ((vgm_offset != 0x00)&&(vgmpos < vgm_offset)) {
      vgm_header(command);
      return;
    }
    
    byte data = 0;
    switch (command) {
        case 0x50:
            // SN76489
            data = getByte();
//            Serial.print("SN76489.write(");
//            Serial.print(data,HEX);
//            Serial.println(")");
            SN76489.write(data);
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
  // LED HIGH
  pinMode(25,OUTPUT);
  digitalWrite(25,HIGH);
  
  // log initialize
  Serial.begin(115200);
//  while (!Serial) {
//    delay(1);  // wait for serial port to connect. Needed for native USB port only
//  }
  
  Serial.println("VGMDUINO JUKEBOX for Raspberry Pi pico with microSD");

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
//  Serial1.setTX(0);
//  Serial1.setRX(1);
  
  pinMode(PIN_SD_SS,OUTPUT);
  while (!SD.begin(PIN_SD_SS)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    delay(3000);
  }
  Serial.println("card initialized.");
  numOfTracks = getFiles();

  // SN76489 initialize
  SN76489.begin();
  delay(400);
  
}

void checkUART() {
    if(Serial1.available()>0){
        digitalWrite(LED_BUILTIN,LOW);
        byte buf[10] = {0,0,0,0,0,0,0,0,0,0};;
//        int idx = 0;
//        while(Serial1.available()>0) {
//          uint8_t val = Serial1.read();       // 受信したデータを読み込む
//          if (idx<=9) {
//            buf[idx++] = val;               
//          }
//          if (val==0xEF) {
//            break;
//          }
//        }
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
    vgmpos = 0x00;
  
//    String fileName = getFileName(trackNo);
//    dataFile = SD.open(fileName);
    dataFile = SD.open(files[trackNo]);
    Serial.print(trackNo);
    Serial.print(":");
    Serial.println(files[trackNo]);
  
//    int idx=0;
//    if (dataFile) {
//      while (dataFile.available() && idx++ < vgm_offset) {
//        dataFile.read();
//      }
//    } else {
//      Serial.print("error opening ");
//      Serial.println(files[trackNo]);
//      vgmend=true;
//    }
//    int idx=0;
    if (!dataFile) {
      Serial.print("error opening ");
      Serial.println(files[trackNo]);
      vgmend=true;
    }
    
    while(!vgmend) {
        vgmplay();
//        checkUART();
        bootselBtn();
        digitalWrite(LED_BUILTIN,HIGH);

    }
    dataFile.close();

    SN76489.begin();
    delay(1000);

    trackNo++;
    if (trackNo>=numOfTracks) {
      trackNo=0;
    }
}

//void loop() {
//  if(BOOTSEL){
//    delay(10);
//    while(BOOTSEL);
//    delay(10);
//    rp2040.reboot();
//  }
//  if(onload) {
//    vgmplay();
//
//    if (vgmend) {
//      Serial.print("vgmpos:");
//      Serial.println(vgmpos);
//      Serial.println("VGM end.SN76489 reset.");
//      onload=false;
//      vgmend=false;
//      vgmpos = 0xb0;
//      vgmidx = 0;
//      pristine = true;    
//      SN76489.begin();
//      delay(3000);
//    }
//  }
//}
//
//void dataReceive(int number) {
//  if(Wire.available() && onload==false) {
//    if (pristine && number==4) {
//      
//      unsigned long f1 = Wire.read();
//      unsigned long f2 = Wire.read();
//      unsigned long f3 = Wire.read();
//      unsigned long f4 = Wire.read();
//      filesize = (f1<<24)+(f2<<16)+(f3<<8)+f4;
//      pristine = false;
//      Serial.print("filesize:");
//      Serial.println(filesize);
//    } else if (pristine == false ) {
//      for(int i=0;i<number;i++) {
//        uint8_t value = Wire.read();
//        if(!(vgmidx==0 && value==0xFF)) {
//          vgmdata[vgmidx++] = value;
//        }
//      }
//    }
//
//    if (vgmidx>=filesize) {
//      onload = true;
//      Serial.println("load complete. -> vgmplay");
//    }
//  }
//}
