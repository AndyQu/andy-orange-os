#ifndef _8259A_CONST_H_
#define	_8259A_CONST_H_

/*8259a interrupt controller ports*/
/*I/O port for interrupt controller <Master>*/
#define INT_M_CTL 0x20
/*setting bits in this port disables interrupts<Master>*/
#define INT_M_CTLMASK 0x21
/*I/O port for interrupt controller <Slave>*/
#define INT_S_CTL 0xA0
/*setting bits in this port disables interrupts <Slave>*/
#define INT_S_CTLMASK 0xA1

#endif
