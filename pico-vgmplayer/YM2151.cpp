/*
	YM2151 library	v0.13
	author:ISH
  modified for Raspberry Pi Pico: mobileFF
*/
#include  "YM2151.h"

#define	DIRECT_IO

YM2151_Class	YM2151;

YM2151_Class::YM2151_Class()
{
}

/*! IOの初期設定を行いYM2151/YM3012をハードリセットする、必ず呼ぶ必要あり
 */
void	YM2151_Class::begin()
{
  pinMode(YM_PIN_D0, OUTPUT);
  pinMode(YM_PIN_D1, OUTPUT);
  pinMode(YM_PIN_D2, OUTPUT);
  pinMode(YM_PIN_D3, OUTPUT);
  pinMode(YM_PIN_D4, OUTPUT);
  pinMode(YM_PIN_D5, OUTPUT);
  pinMode(YM_PIN_D6, OUTPUT);
  pinMode(YM_PIN_D7, OUTPUT);

	pinMode(YM_PIN_A0, OUTPUT);
	pinMode(YM_PIN_WR, OUTPUT);
	pinMode(YM_PIN_RD, OUTPUT);
	pinMode(YM_PIN_IC, OUTPUT);

  digitalWrite(YM_PIN_IC, HIGH);
  digitalWrite(YM_PIN_A0, HIGH);
  digitalWrite(YM_PIN_WR, HIGH);
  digitalWrite(YM_PIN_RD, HIGH);
 
	digitalWrite(YM_PIN_IC, LOW);
	delay(200);
	digitalWrite(YM_PIN_IC, HIGH);
	delay(200);
	return;
}

#ifdef DIRECT_IO

static	uint8_t last_write_addr=0x00;

/*! 指定アドレスのレジスタに書き込みを行う
	\param addr		アドレス
	\param data		データ
 */
void	YM2151_Class::write(uint8_t addr,uint8_t data)
{
	uint8_t i,wi;

	if(last_write_addr != 0x20){
		// addr 0x20へのアクセスの後busyフラグが落ちなくなる病 '86の石だと発生
		// 他のレジスタに書くまで落ちないので、強引だが0x20アクセス後ならチェックしない
		wait(8);
		A0_LOW();
		wait(4);
		for(i=0;i<32;i++){
			RD_LOW();
			wait(4);
        if((pinRead() & 0x80) == 0) { 
				RD_HIGH();
				wait(4);
				break;
			}
			RD_HIGH();
			wait(8);
			if(i>16){
				delayMicroseconds(1);
			}
		}
	}
	wait(4);

	wait(8);
	A0_LOW();
  pinWrite(addr);
  	
	wait(4);
	WR_LOW();		// Write Address
	wait(4);
	WR_HIGH();
	wait(2);
	A0_HIGH();
	wait(2);
  pinWrite(data);

	wait(4);
	WR_LOW();		// Write Data
	wait(4);
	WR_HIGH();
	wait(2);
	last_write_addr = addr;
}

/*! ステータスを読み込む、bit0のみ有効
 */
uint8_t	YM2151_Class::read()
{
	uint8_t i,wi,data;
	A0_HIGH();
	wait(4);
	RD_LOW();		// Read Data
	wait(4);
  data = pinRead();
	RD_HIGH();
	wait(4);
  return data;
}


/*! 約300nSec x loop分だけ待つ、あまり正確でない。
	\param loop		ループ数
 */
void	YM2151_Class::wait(uint8_t loop)
{
	uint8_t wi;
	for(wi=0;wi<loop;wi++){
		// 16MHz  nop = @60nSec
		asm volatile("nop\n\t nop\n\t nop\n\t nop\n\t");
	}
}

void YM2151_Class::pinWrite(uint8_t value) {
  digitalWrite(YM_PIN_D0,(value & 0b00000001)==0x01?HIGH:LOW);
  digitalWrite(YM_PIN_D1,(value & 0b00000010)==0x02?HIGH:LOW);
  digitalWrite(YM_PIN_D2,(value & 0b00000100)==0x04?HIGH:LOW);
  digitalWrite(YM_PIN_D3,(value & 0b00001000)==0x08?HIGH:LOW);
  digitalWrite(YM_PIN_D4,(value & 0b00010000)==0x10?HIGH:LOW);
  digitalWrite(YM_PIN_D5,(value & 0b00100000)==0x20?HIGH:LOW);
  digitalWrite(YM_PIN_D6,(value & 0b01000000)==0x40?HIGH:LOW);
  digitalWrite(YM_PIN_D7,(value & 0b10000000)==0x80?HIGH:LOW);
}

uint8_t YM2151_Class::pinRead() {
  pinMode(YM_PIN_D0, INPUT);
  pinMode(YM_PIN_D1, INPUT);
  pinMode(YM_PIN_D2, INPUT);
  pinMode(YM_PIN_D3, INPUT);
  pinMode(YM_PIN_D4, INPUT);
  pinMode(YM_PIN_D5, INPUT);
  pinMode(YM_PIN_D6, INPUT);
  pinMode(YM_PIN_D7, INPUT);
  uint8_t result = 
    (digitalRead(YM_PIN_D0)==HIGH?1:0)+
    (digitalRead(YM_PIN_D1)==HIGH?1:0)<<1+
    (digitalRead(YM_PIN_D2)==HIGH?1:0)<<2+
    (digitalRead(YM_PIN_D3)==HIGH?1:0)<<3+
    (digitalRead(YM_PIN_D4)==HIGH?1:0)<<4+
    (digitalRead(YM_PIN_D5)==HIGH?1:0)<<5+
    (digitalRead(YM_PIN_D6)==HIGH?1:0)<<6+
    (digitalRead(YM_PIN_D7)==HIGH?1:0)<<7;
  pinMode(YM_PIN_D0, OUTPUT);
  pinMode(YM_PIN_D1, OUTPUT);
  pinMode(YM_PIN_D2, OUTPUT);
  pinMode(YM_PIN_D3, OUTPUT);
  pinMode(YM_PIN_D4, OUTPUT);
  pinMode(YM_PIN_D5, OUTPUT);
  pinMode(YM_PIN_D6, OUTPUT);
  pinMode(YM_PIN_D7, OUTPUT);
  return result;
}

void YM2151_Class::A0_HIGH() {
  digitalWrite(YM2151_Class::YM_PIN_A0,HIGH);
}
void YM2151_Class::A0_LOW() {
  digitalWrite(YM2151_Class::YM_PIN_A0,LOW);
}
void YM2151_Class::RD_HIGH() {
  digitalWrite(YM2151_Class::YM_PIN_RD,HIGH);
}
void YM2151_Class::RD_LOW() {
  digitalWrite(YM2151_Class::YM_PIN_RD,LOW);
}
void YM2151_Class::WR_HIGH() {
  digitalWrite(YM2151_Class::YM_PIN_WR,HIGH);
}
void YM2151_Class::WR_LOW() {
  digitalWrite(YM2151_Class::YM_PIN_WR,LOW);
}

#endif
