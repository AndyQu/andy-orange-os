#ifdef GLOBAL_VARIABLES_HERE
#undef EXTERN
#define EXTERN
#endif

#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include"protect_type.h"
#include"interrupt_const.h"
#include"proc_type.h"
/*GDT指针和描述符表*/
EXTERN PUBLIC  t_8     gdt_ptr[6];
EXTERN PUBLIC  DESCRIPTOR      gdt[GDT_SIZE];

/*IDT指针和描述符表*/
EXTERN PUBLIC  t_8     idt_ptr[6];
EXTERN PUBLIC  GATE    idt[IDT_SIZE];

/*进程表*/
EXTERN PUBLIC PROCESS_TABLE	process_table[NR_TASKS+NR_PROCS];/*进程表*/

/*当前正在运行的进程的id*/
EXTERN	PUBLIC 	t_32	__processID;
/*当前进程运行的clock数*/
EXTERN	PUBLIC	t_32	clock_times;
/*一个进程可运行的最大时钟数*/
EXTERN	PUBLIC	t_32	MAX_CLOCK;

/*TSS段*/
EXTERN	PUBLIC	TSS	tss[1];


/*记录时钟中断次数的变量*/
EXTERN	PUBLIC	int	ticks;
/*Mini OS的系统调用函数表*/
EXTERN	PUBLIC	t_sys_call	sys_call_table[NR_SYS_CALL];

/*
added in chapter8.a.
to identify whether it's in kernel or process.
0: process; 
>0:kernel
*/
EXTERN	PUBLIC	int	k_reenter;

#define SELECTOR_KERNEL_CS 8
#endif
