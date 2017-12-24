//Using TIMER1 to generate two pulse trains 90 degrees apart
//"quadrature pulse"

//#include <avr/io.h>							//we use gcc-avr
#include "gpio.h"
//#include "delay.h"							//we use software delays

//hardware configuration
#define QPOUT1_PORT		PORTB
#define QPOUT1_DDR		DDRB
#define QPOUT1			(1<<1)					//OC1A on PB1
#define QPOUT2_PORT		PORTB
#define QPOUT2_DDR		DDRB
#define QPOUT2			(1<<4)					//OC1B on PB4

#define QP_PS			0x01					//prescaler for timer1
//end hardware configuration

//global defines

//global variables

//initialize pulse generation module
//TIMER1, normal mode (non-PWM),
//Output on OC1A and OC1B,
//Period is 0xff -> OCR1C / CTC1 not used
//PS: specified by QP_PS
void QPgen_init(void) {
	//initialize the output pins
	IO_OUT(QPOUT1_DDR, QPOUT1);
	IO_OUT(QPOUT2_DDR, QPOUT2);

	//configure timer1: output compare, toggle OC1A/B on output compare
	TCCR1 = (TCCR1 &~0x0f) | (0x0f & 0x00);		//stop the timer
	TCNT1 = 0;									//reset the clock
	OCR1A = 0;									//COM1A in phase
	OCR1B = 256 / 2 / 1;						//COM1B 90 degrees out
	//OCR1C = pr;								//sets output compare period - need to set CTC1 as well
	TCCR1 &=~(1<<PWM1A); TCCR1 = (TCCR1 &~0x30) | (0x30 & 0x10);			//0x10->COM1A toggle on compare, normal mode (pwm disabled)
	GTCCR &=~(1<<PWM1B); GTCCR = (GTCCR &~0x30) | (0x30 & 0x10);			//0x10->COM1B toggle on compare, normal mode (pwm disabled)

	//enable clock
	TCCR1 = (TCCR1 &~0x0f) | (0x0f & QP_PS);		//TIMER1 prescaler -> 1:1
}
int main(void) {

	mcu_init();								//reset the mcu
	QPgen_init();							//reset the quadrature pulse generator
	while(1) {
	}

	return 0;
}
