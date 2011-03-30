/*proc.c:This file is added in chapter8.a*/
#include"type.h"
#include"const.h"
#include"protect_type.h"
#include"global.h"
#include"printf.h"

int get_ticks();
/*下面是进程相关函数*/
PUBLIC void milli_delay(int milli_sec)
{
        t_32 t=get_ticks();
	//get_ticks();
	//return;
        t_32 a;
        while(1){
                a=get_ticks();
                if((a-t)*10>milli_sec){
                        return;
                }
        }
}
PUBLIC void p1()
{
        int i;
        char*tmp="Hello,Wold";
        char*s="<A:%d>";
//	assert(0);
        //char*s="<A>";
        //s[0]=MAG_CH_PANIC;
        while(1){
		i=get_ticks();
		printf(s,i);
		//printf(s);
		milli_delay(1000);
        }
}
PUBLIC void p2()
{
        int i=0;
//	assert(0);
        while(1){
                printf(" B.");
                milli_delay(1000);
        }
}


PUBLIC void p3()
{
        int i=0;
//	assert(0);
        while(1){
		printf(":P3:");
                milli_delay(1000);
        }
}

#define LDT_SelectorP1_Code          0x0+SA_RPL3+SA_TIL      //;0x7
#define LDT_SelectorP1_SS            0x8+SA_RPL3+SA_TIL      //;0x0F
#define LDT_SelectorP1_ES            0x10+SA_RPL3+SA_TIL     //;0x17
#define LDT_SelectorP1_DS            0x18+SA_RPL3+SA_TIL     //;0x1F
#define P1_STACK_SIZE                1000

#define LDT_SelectorP2_Code          0x0+SA_RPL3+SA_TIL      //;0x7
#define LDT_SelectorP2_DS            0x8+SA_RPL3+SA_TIL      //;0x0F
#define LDT_SelectorP2_ES            0x10+SA_RPL3+SA_TIL     //;0x17
#define LDT_SelectorP2_SS            0x18+SA_RPL3+SA_TIL     //;0x1F
#define P2_STACK_SIZE                1000

#define LDT_SelectorP3_Code          0x0+SA_RPL3+SA_TIL      //;0x7
#define LDT_SelectorP3_DS            0x8+SA_RPL3+SA_TIL      //;0x0F
#define LDT_SelectorP3_ES            0x10+SA_RPL3+SA_TIL     //;0x17
#define LDT_SelectorP3_SS            0x18+SA_RPL3+SA_TIL     //;0x1F
#define P3_STACK_SIZE                1000

PUBLIC void init_normal_process()
{
	/*added in chapter8.a*/
	int pid1=NR_TASKS+0;
	int pid2=NR_TASKS+1;
	int pid3=NR_TASKS+2;
	int pid_task=0;
	PROCESS_TABLE* p1_table=process_table+pid1;
	PROCESS_TABLE* p2_table=process_table+pid2;
	PROCESS_TABLE* p3_table=process_table+pid3;
	int R3_eflags=0x00000202;
	set_process_regs(p1_table,
			(int)p1,
			LDT_SelectorP1_Code,
			R3_eflags, 
			(int)((p1_table)->stack)+STACK_SIZE-1,
			LDT_SelectorP1_SS, 
			LDT_SelectorP1_DS, 
			LDT_SelectorP1_ES
			);
	set_process_regs(p2_table,
			(int)p2,
			LDT_SelectorP2_Code,
			R3_eflags, 
			(int)(p2_table->stack)+STACK_SIZE-1,
			LDT_SelectorP2_SS, 
			LDT_SelectorP2_DS, 
			LDT_SelectorP2_ES
			);
	set_process_regs(p3_table,
			(int)p3,
			LDT_SelectorP3_Code,
			R3_eflags, 
			(int)(p3_table->stack)+STACK_SIZE-1,
			LDT_SelectorP3_SS, 
			LDT_SelectorP3_DS, 
			LDT_SelectorP3_ES
			);
	initProcess(p1_table,
				30,30,3,0);
	initProcess(p2_table,
				30,30,3,1);
	initProcess(p3_table,
				30,30,3,2);

	/*added in chapter8.a end*/

	//第一个进程的LDT entry 放置于gdt的index 8处，同时设置进程表中的ldt_selector
	setGDTEntry(8,(int)(p1_table->ldt),sizeof(DESCRIPTOR)*LDT_SIZE-1,DA_32+DA_DPL3+DA_LDT);
	p1_table->ldt_selector=0x40+SA_RPL3;
	//第二个进程的LDT entry 放置于gdt的index 9处，同时设置进程表中的ldt_selector
	setGDTEntry(9,(int)(p2_table->ldt),sizeof(DESCRIPTOR)*LDT_SIZE-1,DA_32+DA_DPL3+DA_LDT);
	p2_table->ldt_selector=0x48+SA_RPL3;
	//第三个进程的LDT entry 放置于gdt的index 10处，同时设置进程表中的ldt_selector
	setGDTEntry(10,(int)(p3_table->ldt),sizeof(DESCRIPTOR)*LDT_SIZE-1,DA_32+DA_DPL3+DA_LDT);
	p3_table->ldt_selector=0x50+SA_RPL3;

	//填充第一个进程的LDT表
	setLDTEntry(p1_table->ldt, 0, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL3+DA_CR);//代码段放在第一个LDT entry处
	setLDTEntry(p1_table->ldt, 3, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL3+DA_DRW);//这个描述符作为ds的数值，以便这个进程能够访问到所有的数据
	setLDTEntry(p1_table->ldt, 2, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL3+DA_DRW);//这个描述符作为es的数值，以便这个进程能够访问到所有的数据
	setLDTEntry(p1_table->ldt, 1, 0,(int)(p1_table->stack)+STACK_SIZE-1, DA_32+DA_DPL3+DA_DRW);//这个描述符作为ss的数值，指向进程的堆栈


	//填充第二个进程的LDT表
	setLDTEntry(p2_table->ldt, 0, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL3+DA_CR);//代码段放在地0个LDT entry处
	setLDTEntry(p2_table->ldt, 1, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL3+DA_DRW);//这个描述符作为ds的数值，以便这个进程能够访问到所有的数据
	setLDTEntry(p2_table->ldt, 2, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL3+DA_DRW);//这个描述符作为es的数值，以便这个进程能够访问到所有的数据
	setLDTEntry(p2_table->ldt, 3, 0,(int)(p2_table->stack)+STACK_SIZE-1, DA_32+DA_DPL3+DA_DRW);//这个描述符作为ss的数值，指向进程的堆栈


	//填充第三个进程的LDT表
	setLDTEntry(p3_table->ldt, 0, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL3+DA_CR);//代码段放在地0个LDT entry处
	setLDTEntry(p3_table->ldt, 1, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL3+DA_DRW);//这个描述符作为ds的数值，以便这个进程能够访问到所有的数据
	setLDTEntry(p3_table->ldt, 2, 0, 0xffff, DA_LIMIT_4K+DA_32+DA_DPL3+DA_DRW);//这个描述符作为es的数值，以便这个进程能够访问到所有的数据
	setLDTEntry(p3_table->ldt, 3, 0,(int)(p3_table->stack)+ STACK_SIZE-1, DA_32+DA_DPL3+DA_DRW);//这个描述符作为ss的数值，指向进程的堆栈

}
