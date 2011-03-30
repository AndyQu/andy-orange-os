#ifndef _ORANGE_TSS_H_
#define _ORANGE_TSS_H_
/*
setTssEspReady:
	set the TSS ESP0 field as the bottom address of the process table regs
	This function is used to get TSS ESP ready for next Interrupt
@pid: the process ID
*/
PUBLIC void setTssEspReady(int pid);
/*
getProRegTop:
	get the address of the process table regs
	This function is used for loading target process state.
#pid:the process ID
*/
PUBLIC void getProRegTop(int pid);
#endif
