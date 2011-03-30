/*proc.c:This file is added in chapter8.a*/
#include"type.h"
#include"const.h"
#include"protect_type.h"
#include"global.h"
#include"printf.h"

PUBLIC void setLDTEntry(DESCRIPTOR*ldt, t_32 index, t_32 base, t_32 limit, t_16 attr)
{
        ldt[index].limit_low=(t_16)(limit&0x0000ffff);
        ldt[index].base_low= (t_16)(base &0x0000ffff);
        ldt[index].base_mid= (t_8) ((base>>16)&0x0ff);
        ldt[index].attr1=(t_8) (attr&0x00ff);
        ldt[index].limit_high_attr2=(t_8)(
                ((attr>>8)&0x0f0)
                |
                ((limit>>16)&0x0f)
        );
        ldt[index].base_high=(t_8)(
                (base>>24) &0x0ff
        );
}

PUBLIC void setGDTEntry(t_32 index, t_32 base, t_32 limit, t_16 attr);
#define SelectorVideo   0x18+SA_RPL3 //;这个选择子的RPL为3
PUBLIC void set_process_regs(PROCESS_TABLE*p,int eip,int cs,int eflags, int esp,int ss, int ds, int es)
{
	if(p==0)
		return;
	p->p_regs.p_eip=eip;
	p->p_regs.p_cs=cs;
	p->p_regs.p_eax=0;
	p->p_regs.p_ebx=0;
	p->p_regs.p_ecx=0;
	p->p_regs.p_edx=0;
	p->p_regs.p_edi=0;
	p->p_regs.p_esi=0;
	p->p_regs.p_ebp=0;
	p->p_regs.p_esp=esp;
	p->p_regs.p_ss=ss;
	p->p_regs.p_ds=ds;
	p->p_regs.p_es=es;
	p->p_regs.p_fs=0;
	p->p_regs.p_gs=SelectorVideo;
	p->p_regs.p_eflags=eflags;
}
PUBLIC void initProcess(PROCESS_TABLE*p,int priority,int ticks,int CPL,int tty_id)
{
	p->priority=priority;
	p->ticks=ticks;
	p->CPL=CPL;
	p->tty_id=tty_id;
	p->invalid=0;
	p->p_flags=RUNNABLE;
	p->msg=0;
	p->nextSendingPro=0;
	p->sendingQ=0;
	p->p_recvfrom=NO_TASK;
	p->p_sendto=NO_TASK;
	p->has_int_msg=FALSE;
}
/*void add_system_task(int program_pointer);
void task_disk(void);
void task_tick(void);
void task_tty(void);
*/
void init_tasks(void);
int __task_inc=0;
int __userProcess_inc=0;
int __ldt_inc=0;
PUBLIC void init_process_table()
{
	int i=0;
	/*make process tables invalid*/
	for(i=0;i<NR_TASKS+NR_PROCS;i++){
		process_table[i].invalid=1;
	}
	init_normal_process();

	__task_inc=0;
	__ldt_inc=11;
	init_tasks();
/*	add_system_task((int)task_tty);
	add_system_task((int)task_tick);
	add_system_task((int)task_disk);
	__userProcess_inc=NR_TASKS+3;
	*/
}

PUBLIC int getProPointer(int pid)
{
	return (int)(process_table+pid-1);
}


#define LDT_SelectorP4_Code          0x0+SA_RPL1+SA_TIL      //;0x5
#define LDT_SelectorP4_DS            0x8+SA_RPL1+SA_TIL      //;0x0D
#define LDT_SelectorP4_ES            0x10+SA_RPL1+SA_TIL     //;0x15
#define LDT_SelectorP4_SS            0x18+SA_RPL1+SA_TIL     //;0x1D
#define P4_STACK_SIZE                2000
PUBLIC void add_system_task(int program_pointer)
{
        PROCESS_TABLE*task_table=process_table+__task_inc;/*当添加一个新的进程时，这里需要修改*/
	__task_inc++;
        int R1_eflags=0x00001202;
        set_process_regs(task_table,
                        (int)program_pointer,/*当添加一个新的进程时，这里需要修改*/
                        LDT_SelectorP4_Code,
                        R1_eflags,
                        (int)(task_table->stack)+STACK_SIZE-1,
                        LDT_SelectorP4_SS,
                        LDT_SelectorP4_DS,
                        LDT_SelectorP4_ES
                        );
        initProcess(task_table,30,30,1,0);

        //进程的LDT entry 放置于gdt的index 13处，同时设置进程表中的ldt_selector
        setGDTEntry(__ldt_inc,(int)(task_table->ldt),sizeof(DESCRIPTOR)*LDT_SIZE-1,DA_32+DA_DPL1+DA_LDT);/*当添加一个新的进程时，这>里需要修改*/
        task_table->ldt_selector=__ldt_inc*8+SA_RPL1;/*当添加一个新的进程时，这里需要修改*/
	__ldt_inc++;

        //进程的LDT表
        setLDTEntry(task_table->ldt, 0, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL1+DA_CR);//代码段放在地0个LDT entry处
        setLDTEntry(task_table->ldt, 1, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL1+DA_DRW);//这个描述符作为ds的数值，以便这个进程能够访问到所有的数据
        setLDTEntry(task_table->ldt, 2, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL1+DA_DRW);//这个描述符作为es的数值，以便这个进程能够访问到所有的数据
        setLDTEntry(task_table->ldt, 3, 0,(int)(task_table->stack)+ STACK_SIZE-1, DA_32+DA_DPL1+DA_DRW);//这个描述符作为ss的>数值，指向进程的堆栈
}
