/*
	AY_3_8910 library	v0.13
	author:ISH
  modified for Raspberry Pi Pico: mobileFF
*/
#include  "AY_3_8910.h"

#define	DIRECT_IO

AY_3_8910_Class	AY_3_8910;

AY_3_8910_Class::AY_3_8910_Class()
{
}

/*! IOの初期設定を行いAY_3_8910をハードリセットする、必ず呼ぶ必要あり
 */
void	AY_3_8910_Class::begin()
{
  pinMode(AY_PIN_D0, OUTPUT);
  pinMode(AY_PIN_D1, OUTPUT);
  pinMode(AY_PIN_D2, OUTPUT);
  pinMode(AY_PIN_D3, OUTPUT);
  pinMode(AY_PIN_D4, OUTPUT);
  pinMode(AY_PIN_D5, OUTPUT);
  pinMode(AY_PIN_D6, OUTPUT);
  pinMode(AY_PIN_D7, OUTPUT);

  pinMode(AY_PIN_A0,  OUTPUT);
  pinMode(AY_PIN_WR,  OUTPUT);
  pinMode(AY_PIN_RES, OUTPUT);
  pinMode(AY_PIN_A0,  OUTPUT);

  pinMode(AY_PIN_LED, OUTPUT);

  //digitalWrite(AY_PIN_WE, HIGH);

  mode_inactive();
  digitalWrite(AY_PIN_RES,HIGH);
  digitalWrite(AY_PIN_RES,LOW);

  //mute();
     
	return;
}

#ifdef DIRECT_IO

/*! 指定アドレスのレジスタに書き込みを行う
	\param data		データ
 */
void	AY_3_8910_Class::write(uint8_t address,uint8_t data)
{
  write_address(address);
//  delayMicroseconds(1);
  write_data(data);
  
}

///*! ステータスを読み込む、bit0のみ有効
// */
//uint8_t	AY_3_8910_Class::read()
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
//void	AY_3_8910_Class::wait(uint8_t loop)
//{
//	uint8_t wi;
//	for(wi=0;wi<loop;wi++){
//		// 16MHz  nop = @60nSec
//		asm volatile("nop\n\t nop\n\t nop\n\t nop\n\t");
//	}
//}

void AY_3_8910_Class::pinWrite(uint8_t value) {
  digitalWrite(AY_PIN_D0,(value & 0b00000001)==0x01?HIGH:LOW);
  digitalWrite(AY_PIN_D1,(value & 0b00000010)==0x02?HIGH:LOW);
  digitalWrite(AY_PIN_D2,(value & 0b00000100)==0x04?HIGH:LOW);
  digitalWrite(AY_PIN_D3,(value & 0b00001000)==0x08?HIGH:LOW);
  digitalWrite(AY_PIN_D4,(value & 0b00010000)==0x10?HIGH:LOW);
  digitalWrite(AY_PIN_D5,(value & 0b00100000)==0x20?HIGH:LOW);
  digitalWrite(AY_PIN_D6,(value & 0b01000000)==0x40?HIGH:LOW);
  digitalWrite(AY_PIN_D7,(value & 0b10000000)==0x80?HIGH:LOW);
}

//uint8_t AY_3_8910_Class::pinRead() {
//  pinMode(AY_PIN_D0, INPUT);
//  pinMode(AY_PIN_D1, INPUT);
//  pinMode(AY_PIN_D2, INPUT);
//  pinMode(AY_PIN_D3, INPUT);
//  pinMode(AY_PIN_D4, INPUT);
//  pinMode(AY_PIN_D5, INPUT);
//  pinMode(AY_PIN_D6, INPUT);
//  pinMode(AY_PIN_D7, INPUT);
//  uint8_t result = 
//    (digitalRead(AY_PIN_D0)==HIGH?1:0)+
//    (digitalRead(AY_PIN_D1)==HIGH?1:0)<<1+
//    (digitalRead(AY_PIN_D2)==HIGH?1:0)<<2+
//    (digitalRead(AY_PIN_D3)==HIGH?1:0)<<3+
//    (digitalRead(AY_PIN_D4)==HIGH?1:0)<<4+
//    (digitalRead(AY_PIN_D5)==HIGH?1:0)<<5+
//    (digitalRead(AY_PIN_D6)==HIGH?1:0)<<6+
//    (digitalRead(AY_PIN_D7)==HIGH?1:0)<<7;
//  pinMode(AY_PIN_D0, OUTPUT);
//  pinMode(AY_PIN_D1, OUTPUT);
//  pinMode(AY_PIN_D2, OUTPUT);
//  pinMode(AY_PIN_D3, OUTPUT);
//  pinMode(AY_PIN_D4, OUTPUT);
//  pinMode(AY_PIN_D5, OUTPUT);
//  pinMode(AY_PIN_D6, OUTPUT);
//  pinMode(AY_PIN_D7, OUTPUT);
//  return result;
//}

void AY_3_8910_Class::mode_set(PinStatus iowr,PinStatus iord,PinStatus a0,PinStatus led,uint8_t slp) {
  digitalWrite(AY_PIN_WR,iowr);
  digitalWrite(AY_PIN_RD,iord);
  digitalWrite(AY_PIN_A0,a0);
  digitalWrite(AY_PIN_LED,led);
  delayMicroseconds(slp);
}

void AY_3_8910_Class::mode_set(PinStatus iowr,PinStatus iord,PinStatus a0,PinStatus led) {
  mode_set(iowr,iord,a0,led,0);
}

void AY_3_8910_Class::mode_inactive() {
  mode_set(HIGH,HIGH,HIGH,LOW);
}

void AY_3_8910_Class::mode_address() {
  mode_set(LOW,HIGH,LOW,HIGH);
}

void AY_3_8910_Class::mode_write() {
  mode_set(LOW,HIGH,HIGH,HIGH);
}

void AY_3_8910_Class::mode_read() {
  mode_set(HIGH,LOW,LOW,HIGH);
}

void AY_3_8910_Class::write_address(byte addr) {
  mode_inactive();
  mode_address();
  pinWrite(addr);
  mode_inactive();
}

void AY_3_8910_Class::write_data(byte data) {
  mode_inactive();
  pinWrite(data);
  mode_write();
  mode_inactive();
}

//void AY_3_8910_Class::set_volume(uint8_t channel,uint8_t volume) {
//    uint8_t volume_command = 0x90 | (channel << 5) | (15 - volume); // 値を反転
//    write(volume_command);
//}

//void AY_3_8910_Class::mute() {
//  set_volume(0,0);
//  set_volume(1,0);
//  set_volume(2,0);
//  set_volume(3,0);
//  
//}
#endif
