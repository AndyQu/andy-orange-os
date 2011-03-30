#include"type.h"
#include"const.h"
#include"protect_type.h"
#include"interrupt_const.h"
#include"proto.h"
#include"8259A_const.h"

/*
Priviledge: Ring<0> and Ring<1>
Usage:
 Initialize 8259A
 Enable clock interrupt
 Enable keyboard interrupt
*/
PUBLIC void init_8259A()
{
	//Master 8259,ICW1
	out_byte(INT_M_CTL,0x11);
	//ICW2
	out_byte(INT_M_CTLMASK,INT_VECTOR_IRQ0);
	//ICW3
	out_byte(INT_M_CTLMASK,0x4);
	//ICW4
	out_byte(INT_M_CTLMASK,0x1);
	//OCW1，打开时钟中断、键盘中断，时钟中断为IRQ0，键盘中断为IRQ1，从8259片连在IRQ2上
	out_byte(INT_M_CTLMASK,0xfc);

	//Slave 8259,ICW1
	out_byte(INT_S_CTL,0x11);
	//ICW2
	out_byte(INT_S_CTLMASK,INT_VECTOR_IRQ8);
	//ICW3
	out_byte(INT_S_CTLMASK,0x2);
	//ICW4
	out_byte(INT_S_CTLMASK,0x1);
	//OCW1
	out_byte(INT_S_CTLMASK,0xff);
}


/*
Priviledge: Ring<0> and Ring<1>
Usage:
 Enable slave 8259A interrupt
*/
PUBLIC void enable_slave8259()
{
	int mask=in_byte(INT_M_CTLMASK);
	mask=mask&0x000000FB;
	out_byte(INT_M_CTLMASK,mask);
}
/*
Priviledge: Ring<0> and Ring<1>
Usage:
 Disable slave 8259A interrupt
*/
PUBLIC void disable_slave8259()
{
	int mask=in_byte(INT_M_CTLMASK);
	mask=mask|0x00000004;
	out_byte(INT_M_CTLMASK,mask);
}
/*
Priviledge: Ring<0> and Ring<1>
Usage:
 Enable disk interrupt
*/
PUBLIC void enable_disk()
{
	enable_slave8259();
	int mask=in_byte(INT_S_CTLMASK);
	mask=mask&0x000000BF;
	out_byte(INT_S_CTLMASK,mask);
}
/*
Priviledge: Ring<0> and Ring<1>
Usage:
 Disable disk interrupt
*/
PUBLIC void disable_disk()
{
	int mask=in_byte(INT_S_CTLMASK);
	mask=mask|0x00000040;
	out_byte(INT_S_CTLMASK,mask);
}
/*
Priviledge: Ring<0> and Ring<1>
Usage:
 Enable clock interrupt
*/
PUBLIC void enable_clock()
{
	int mask=in_byte(INT_M_CTLMASK);
	mask=mask&0x000000FE;
	out_byte(INT_M_CTLMASK,mask);
}
/*
Priviledge: Ring<0> and Ring<1>
Usage:
 Disable clock interrupt
*/
PUBLIC void disable_clock()
{
	int mask=in_byte(INT_M_CTLMASK);
	mask=mask|0x00000001;
	out_byte(INT_M_CTLMASK,mask);
}
/*
Priviledge: Ring<0> and Ring<1>
Usage:
 Enable keyboard interrupt
*/
PUBLIC void enable_keyboard()
{
	int mask=in_byte(INT_M_CTLMASK);
	mask=mask&0x000000FD;
	out_byte(INT_M_CTLMASK,mask);
}
/*
Priviledge: Ring<0> and Ring<1>
Usage:
 Disable keyboard interrupt
*/
PUBLIC void disable_keyboard()
{
	int mask=in_byte(INT_M_CTLMASK);
	mask=mask|0x00000002;
	out_byte(INT_M_CTLMASK,mask);
}

/*
Priviledge: Ring<0> triggerSlave8259
Usage:
 inform slave 8259A that interrupt handling has completed
*/
PUBLIC void triggerSlave8259(void)
{
	out_byte(INT_S_CTL,0x20);
}
/*
Priviledge: Ring<0> triggerMaster8259
Usage:
 inform master 8259A that interrupt handling has completed
*/
PUBLIC void triggerMaster8259(void)
{
	out_byte(INT_M_CTL,0x20);
}
