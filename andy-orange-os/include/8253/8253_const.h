#ifndef _8253_CONST_H_
#define	_8253_CONST_H_
/*8253 PIT: programmable interval timer*/
/*I/O port for Counter 0*/
#define	TIMER0	0x40
#define	TIMER_MODE	0x43
#define	RATE_GENERATOR	0x34	//write into TIMER_MODE
#define	TIMER_FREQ	1193182L//write into TIMER0
#define	HZ		50

#endif
