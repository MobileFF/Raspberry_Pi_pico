#include	"Arduino.h"

class	AY_3_8910_Class{
	public:
		AY_3_8910_Class();
		void  begin();
		void  write(uint8_t address,uint8_t data);
    void  pinWrite(uint8_t value);
    
//    uint8_t read();
//    void  set_volume(uint8_t channel,uint8_t volume);
//    void  mute();
		
	private:
    static  const uint8_t   AY_PIN_D0=0;
    static  const uint8_t   AY_PIN_D1=1;
    static  const uint8_t   AY_PIN_D2=2;
    static  const uint8_t   AY_PIN_D3=3;
    static  const uint8_t   AY_PIN_D4=4;
    static  const uint8_t   AY_PIN_D5=5;
    static  const uint8_t   AY_PIN_D6=6;
    static  const uint8_t   AY_PIN_D7=7;
	
    static  const uint8_t   AY_PIN_WR  = 9;
    static  const uint8_t   AY_PIN_RD  = 11;
    static  const uint8_t   AY_PIN_RES = 10;
    static  const uint8_t   AY_PIN_A0  = 8;
    static  const uint8_t   AY_PIN_LED = 25;
		
//		void wait(uint8_t loop);
    void mode_set(PinStatus iowr,PinStatus iord,PinStatus a0,PinStatus led,uint8_t slp);
    void mode_set(PinStatus iowr,PinStatus iord,PinStatus a0,PinStatus led);
    void mode_inactive();
    void mode_address();
    void mode_write();
    void mode_read();
    void write_address(byte addr);
    void write_data(byte data);

};
extern AY_3_8910_Class AY_3_8910;
