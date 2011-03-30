#ifndef _PROC_CONST_H_
#define	_PROC_CONST_H_

/*进程堆栈的大小*/
#define	STACK_SIZE (1024*10)
/*number of system tasks,
NR_TASKS+1 should be equal to P1_PID, which is defined in /kernel/kernel.asm */
#define NR_TASKS 20

/*number of user processes*/
#define NR_PROCS 20
/*p_sendto , p_recvfrom*/
#define	NO_TASK	(NR_TASKS+NR_PROCS+100)
#define	ANY	9999
#define INTERRUPT 0

/*
*State of a process
*/
#define	RUNNABLE	0x00
#define	SENDING 0x02
#define	RECEIVING	0x04

#endif
