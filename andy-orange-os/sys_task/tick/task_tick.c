/*This file is added in chapter8/a*/
#include"type.h"
#include"const.h"
#include"protect_type.h"
#include"global.h"
#include"ipc_func.h"
#include"ipc_const.h"
#include"assert.h"



PUBLIC void task_tick()
{
	MESSAGE m;
	int request_pid;
	while(1){
		sendrec(RECEIVE,ANY,&m);
		request_pid=m.source;
		m.source=2;
		m.type=MESSAGE_M1;
		m.u.m1.m1i1=ticks;
		assert(request_pid!=0);
		sendrec(SEND,request_pid,&m);
	}
}
/*
#define LDT_SelectorP4_Code          0x0+SA_RPL1+SA_TIL      //;0x5
#define LDT_SelectorP4_DS            0x8+SA_RPL1+SA_TIL      //;0x0D
#define LDT_SelectorP4_ES            0x10+SA_RPL1+SA_TIL     //;0x15
#define LDT_SelectorP4_SS            0x18+SA_RPL1+SA_TIL     //;0x1D
#define P4_STACK_SIZE                1000
PUBLIC void init_task_tick()
{
	PROCESS_TABLE*task_table=process_table+1;
	int R1_eflags=0x00001202;
	set_process_regs(task_table,
                        (int)task_tick,
                        LDT_SelectorP4_Code,
                        R1_eflags,
                        (int)(task_table->stack)+STACK_SIZE-1,
                        LDT_SelectorP4_SS,
                        LDT_SelectorP4_DS,
                        LDT_SelectorP4_ES
                        );
	initProcess(task_table,30,30,1,0);
	
	//进程的LDT entry 放置于gdt的index 12处，同时设置进程表中的ldt_selector
        setGDTEntry(12,(int)(task_table->ldt),sizeof(DESCRIPTOR)*LDT_SIZE-1,DA_32+DA_DPL1+DA_LDT);
        task_table->ldt_selector=0x60+SA_RPL1;

	//进程的LDT表
        setLDTEntry(task_table->ldt, 0, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL1+DA_CR);//代码段放在地0个LDT entry处
        setLDTEntry(task_table->ldt, 1, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL1+DA_DRW);//这个描述符作为ds的数值，以便这个进程能够访问到所有的数据
        setLDTEntry(task_table->ldt, 2, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL1+DA_DRW);//这个描述符作为es的数值，以便这个进程能够访问到所有的数据
        setLDTEntry(task_table->ldt, 3, 0,(int)(task_table->stack)+ STACK_SIZE-1, DA_32+DA_DPL1+DA_DRW);//这个描述符作为ss的>数值，指向进程的堆栈
}
*/

