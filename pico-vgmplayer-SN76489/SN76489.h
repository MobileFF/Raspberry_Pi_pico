#include	"Arduino.h"

class	SN76489_Class{
	public:
		SN76489_Class();
		void  begin();
//		uint8_t	read();
		void  write(uint8_t data);
    void  pinWrite(uint8_t value);
    void  set_volume(uint8_t channel,uint8_t volume);
    void  mute();
		
	private:
//    static  const uint8_t   SN_PIN_D0=9;
//    static  const uint8_t   SN_PIN_D1=8;
//    static  const uint8_t   SN_PIN_D2=7;
//    static  const uint8_t   SN_PIN_D3=6;
//    static  const uint8_t   SN_PIN_D4=5;
//    static  const uint8_t   SN_PIN_D5=4;
//    static  const uint8_t   SN_PIN_D6=3;
//    static  const uint8_t   SN_PIN_D7=2;
    static  const uint8_t   SN_PIN_D0=7;
    static  const uint8_t   SN_PIN_D1=6;
    static  const uint8_t   SN_PIN_D2=5;
    static  const uint8_t   SN_PIN_D3=4;
    static  const uint8_t   SN_PIN_D4=3;
    static  const uint8_t   SN_PIN_D5=2;
    static  const uint8_t   SN_PIN_D6=1;
    static  const uint8_t   SN_PIN_D7=0;
	
		static	const	uint8_t		SN_PIN_WE=8;
		
		void wait(uint8_t loop);
    void WE_HIGH();
    void WE_LOW();

};
extern SN76489_Class SN76489;
