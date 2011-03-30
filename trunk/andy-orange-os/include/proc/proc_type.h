#ifndef _PROC_TYPE_H_
#define _PROC_TYPE_H_

#include"type.h"
#include"proc_const.h"
#include"ipc.h"
/*进程的寄存器*/
typedef struct  s_regs
{
        t_32    p_gs;   //0
        t_32    p_fs;   //4
        t_32    p_es;   //8
        t_32    p_ds;   //12
        //pushad
        t_32    p_edi;  //16
        t_32    p_esi;  //20
        t_32    p_ebp;  //24
        t_32    p_fake_esp;//28 This is not the true esp.It's the interrupt esp.
        t_32    p_ebx;  //32
        t_32    p_edx;  //36
        t_32    p_ecx;  //40
        t_32    p_eax;  //44
        //interrupt stack
        t_32    p_ret;  //return address,stub
        t_32    p_eip;  //48
        t_32    p_cs;   //52
        t_32    p_eflags;//56
        t_32    p_esp;//66 This is the esp of the process being interrupted.
        t_32    p_ss;   //64
}REGS;

/*进程表结构*/
typedef struct s_process_table
{
        REGS    p_regs;
        t_32    ldt_selector;
        t_32    priority;
        t_32    ticks;
        DESCRIPTOR      ldt[LDT_SIZE];
        t_8     stack[STACK_SIZE];
        t_32    CPL;
        t_32    tty_id;
        /*whether this process table is valid.
        0:valid,
        !=0:not valid. 
        added in chapter8/a
        */
        t_32    invalid;
        /*state of this process
        SENDING         0x02
        RECEIVING       0x04
        RUNNABLE        0x00 
        added in chapter8/a
        */
        t_bool  p_flags;
        /*
        "msg" is the message that this process is sending to another process, if not null and if p_flags&SENDING!=0
        ***msg is just a pointer,should be assign valid value before used.***
        "nextSendingPro" is the next process that is sending message to the same process as this process.
        */
        t_32     p_sendto;
        MESSAGE*msg;
        struct  s_process_table*        nextSendingPro;
        /*
        "sendingQ" links to the processes that are sending messages to this process.
        */
        struct  s_process_table*        sendingQ;
        /*
        "p_recvfrom" is the PID of the process that this process is waiting message from.
        "ANY" means any process's message is OK.
        */
        t_32     p_recvfrom;
		/*
		has_int_msg: whether this process has a message from the kernel interrupt handler
		*/
		t_bool	has_int_msg;
}PROCESS_TABLE;

/*特权级定义*/
#define PRIVILEGE_KRNL  0
#define PRIVILEGE_TASK  1
#define PRIVILEGE_USER  3

#endif
