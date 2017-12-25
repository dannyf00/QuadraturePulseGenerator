//using timer interrupts to geneate quadrature waveforms
#include "config.h"								//configuration words
#include "gpio.h"                          	 	//we use gpio functions
//#include "delay.h"                          	//we use software delays
#include "tmr1.h"								//we use tmr1
//hardware configuration
#define QPOUT1_PORT		GPIO
#define QPOUT1_DDR		TRISIO
#define QPOUT1			(1<<5)					//GPOUT1 on GP5
#define QPOUT2_PORT		GPIO
#define QPOUT2_DDR		TRISIO
#define QPOUT2			(1<<4)					//GPOUT2 on GP4
#define QP_PR			(250*4)					//period for the quadrature waveform - better be multiples of 4

#define LED_PORT		GPIO
#define LED_DDR			TRISIO
#define LED				(1<<0)					//led on gp0
//end hardware configuration

//global defines

//global variables

//isr
void interrupt isr(void) {
	if (TMR1IF) tmr1_isr();
}
	
//QP isr handler
void QP_gen(void) {
	static char index=0;						//cycle index, increment every 90 degrees
	
	switch (index) {
		case 0: IO_SET(QPOUT1_PORT, QPOUT1); index=1; break;
		case 1: IO_SET(QPOUT2_PORT, QPOUT2); index=2; break;
		case 2: IO_CLR(QPOUT1_PORT, QPOUT1); index=3; break;
		case 3: IO_CLR(QPOUT2_PORT, QPOUT2); index=0; break;
	}	
}
	
void QPgen_init(void) {
	//initialize output pins
	IO_OUT(QPOUT1_DDR, QPOUT1);
	IO_OUT(QPOUT2_DDR, QPOUT2);
	
	//configure pwm generator
	//configure timebase - TIMER1
	tmr1_init(TMR1_PS1x, QP_PR/4);			//initialize timer
	tmr1_act(QP_gen);						//install user handler	
}
	
int main(void) {
	
	mcu_init();							    //initialize the mcu
	IO_OUT(LED_DDR, LED);					//led as output
	QPgen_init();							//initialize quadrature pulse geneartor
	ei();
	while (1) {
		IO_FLP(LED_PORT, LED);				//flip led - measure CPU usage
	}
}

