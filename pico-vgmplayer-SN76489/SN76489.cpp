/*
	SN76489 library	v0.13
	author:ISH
  modified for Raspberry Pi Pico: mobileFF
*/
#include  "SN76489.h"

#define	DIRECT_IO

SN76489_Class	SN76489;

SN76489_Class::SN76489_Class()
{
}

/*! IOの初期設定を行いSN76489をハードリセットする、必ず呼ぶ必要あり
 */
void	SN76489_Class::begin()
{
  pinMode(SN_PIN_D0, OUTPUT);
  pinMode(SN_PIN_D1, OUTPUT);
  pinMode(SN_PIN_D2, OUTPUT);
  pinMode(SN_PIN_D3, OUTPUT);
  pinMode(SN_PIN_D4, OUTPUT);
  pinMode(SN_PIN_D5, OUTPUT);
  pinMode(SN_PIN_D6, OUTPUT);
  pinMode(SN_PIN_D7, OUTPUT);

	pinMode(SN_PIN_WE, OUTPUT);
  digitalWrite(SN_PIN_WE, HIGH);

  mute();
     
	return;
}

#ifdef DIRECT_IO

static	uint8_t last_write_addr=0x00;

/*! 指定アドレスのレジスタに書き込みを行う
	\param data		データ
 */
void	SN76489_Class::write(uint8_t data)
{
  pinWrite(data);
//  delayMicroseconds(1);
//  wait(8);
	WE_LOW();		// Write Address
  delayMicroseconds(10);
//	wait(8);
	WE_HIGH();
  delayMicroseconds(1);
//  wait(8);
  
}

///*! ステータスを読み込む、bit0のみ有効
// */
//uint8_t	SN76489_Class::read()
//{
//	uint8_t i,wi,data;
//	A0_HIGH();
//	wait(4);
//	RD_LOW();		// Read Data
//	wait(4);
//  data = pinRead();
//	RD_HIGH();
//	wait(4);
//  return data;
//}


/*! 約300nSec x loop分だけ待つ、あまり正確でない。
	\param loop		ループ数
 */
void	SN76489_Class::wait(uint8_t loop)
{
	uint8_t wi;
	for(wi=0;wi<loop;wi++){
		// 16MHz  nop = @60nSec
		asm volatile("nop\n\t nop\n\t nop\n\t nop\n\t");
	}
}

void SN76489_Class::pinWrite(uint8_t value) {
  digitalWrite(SN_PIN_D0,(value & 0b00000001)==0x01?HIGH:LOW);
  digitalWrite(SN_PIN_D1,(value & 0b00000010)==0x02?HIGH:LOW);
  digitalWrite(SN_PIN_D2,(value & 0b00000100)==0x04?HIGH:LOW);
  digitalWrite(SN_PIN_D3,(value & 0b00001000)==0x08?HIGH:LOW);
  digitalWrite(SN_PIN_D4,(value & 0b00010000)==0x10?HIGH:LOW);
  digitalWrite(SN_PIN_D5,(value & 0b00100000)==0x20?HIGH:LOW);
  digitalWrite(SN_PIN_D6,(value & 0b01000000)==0x40?HIGH:LOW);
  digitalWrite(SN_PIN_D7,(value & 0b10000000)==0x80?HIGH:LOW);
}

//uint8_t SN76489_Class::pinRead() {
//  pinMode(SN_PIN_D0, INPUT);
//  pinMode(SN_PIN_D1, INPUT);
//  pinMode(SN_PIN_D2, INPUT);
//  pinMode(SN_PIN_D3, INPUT);
//  pinMode(SN_PIN_D4, INPUT);
//  pinMode(SN_PIN_D5, INPUT);
//  pinMode(SN_PIN_D6, INPUT);
//  pinMode(SN_PIN_D7, INPUT);
//  uint8_t result = 
//    (digitalRead(SN_PIN_D0)==HIGH?1:0)+
//    (digitalRead(SN_PIN_D1)==HIGH?1:0)<<1+
//    (digitalRead(SN_PIN_D2)==HIGH?1:0)<<2+
//    (digitalRead(SN_PIN_D3)==HIGH?1:0)<<3+
//    (digitalRead(SN_PIN_D4)==HIGH?1:0)<<4+
//    (digitalRead(SN_PIN_D5)==HIGH?1:0)<<5+
//    (digitalRead(SN_PIN_D6)==HIGH?1:0)<<6+
//    (digitalRead(SN_PIN_D7)==HIGH?1:0)<<7;
//  pinMode(SN_PIN_D0, OUTPUT);
//  pinMode(SN_PIN_D1, OUTPUT);
//  pinMode(SN_PIN_D2, OUTPUT);
//  pinMode(SN_PIN_D3, OUTPUT);
//  pinMode(SN_PIN_D4, OUTPUT);
//  pinMode(SN_PIN_D5, OUTPUT);
//  pinMode(SN_PIN_D6, OUTPUT);
//  pinMode(SN_PIN_D7, OUTPUT);
//  return result;
//}

void SN76489_Class::WE_HIGH() {
  digitalWrite(SN76489_Class::SN_PIN_WE,HIGH);
}

void SN76489_Class::WE_LOW() {
  digitalWrite(SN76489_Class::SN_PIN_WE,LOW);
}

void SN76489_Class::set_volume(uint8_t channel,uint8_t volume) {
    uint8_t volume_command = 0x90 | (channel << 5) | (15 - volume); // 値を反転
    write(volume_command);
}

void SN76489_Class::mute() {
  set_volume(0,0);
  set_volume(1,0);
  set_volume(2,0);
  set_volume(3,0);
  
}
#endif
