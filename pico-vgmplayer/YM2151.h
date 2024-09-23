#include	"Arduino.h"

class	YM2151_Class{
	public:
		YM2151_Class();
		void	begin();
		uint8_t	read();
		void	write(uint8_t addr,uint8_t data);
    void pinWrite(uint8_t value);
    uint8_t pinRead();
		
	private:
		static	const	uint8_t		YM_PIN_D0=2;
		static	const	uint8_t		YM_PIN_D1=3;
		static	const	uint8_t		YM_PIN_D2=4;
		static	const	uint8_t		YM_PIN_D3=5;
		static	const	uint8_t		YM_PIN_D4=6;
		static	const	uint8_t		YM_PIN_D5=7;
		static	const	uint8_t		YM_PIN_D6=8;
		static	const	uint8_t		YM_PIN_D7=9;
		
		static	const	uint8_t		YM_PIN_RD=10;
		static	const	uint8_t		YM_PIN_WR=11;
		static	const	uint8_t		YM_PIN_A0=12;
		static	const	uint8_t		YM_PIN_IC=13;
		
		void	wait(uint8_t loop);
    void A0_HIGH();
    void A0_LOW();
    void RD_HIGH();
    void RD_LOW();
    void WR_HIGH();
    void WR_LOW();

};
extern YM2151_Class YM2151;
