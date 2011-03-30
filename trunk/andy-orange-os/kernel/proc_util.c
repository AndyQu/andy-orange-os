#include"const.h"
#include"type.h"
#include"protect_type.h"
#include"global.h"
#include"proto.h"
#include"assert.h"
#include"ipc.h"

static int __DEBUG_ProcUtil=0;
/*
<Ring 0>
*/

PUBLIC  int     proc2pid(PROCESS_TABLE*p)
{
        assert_kernel(p!=0);
		assert_kernel(p>process_table);
        return (int)(p-process_table+1);
}
/*
<Ring 0>
*/
PUBLIC  PROCESS_TABLE* pid2proc(int pid)
{
		if(pid<=0)
			return 0;
        assert_kernel(pid>0);
        return (PROCESS_TABLE*)(process_table+pid-1);
}
/*****************************************************************************
 *                                unblock
 *****************************************************************************/
/**
 * <Ring 0>     unblock a process
 * @param p     The process.
 *****************************************************************************/
PUBLIC  void unblock(PROCESS_TABLE*p)
{
	assert_kernel(p->p_flags==RUNNABLE);
	if(__DEBUG_ProcUtil==1){
		printf_kernel("(unblock %d)",proc2pid(p));
/*
		int i,j;
		for(i=0;i<10000;i++){
			for(j=0;j<10000;j++);
		}
*/
	}
}
/*****************************************************************************
 *                                block
 *****************************************************************************/
/**
 * <Ring 0>     block a process
 * @param p     The process.
 *****************************************************************************/
PUBLIC  void block(PROCESS_TABLE*p)
{
        int     text_color=0x74;/*灰底红字*/
        assert_kernel(p->p_flags!=RUNNABLE);
	if(__DEBUG_ProcUtil==1){
		printf_kernel("(block %d)",proc2pid(p));
/*
		int i,j;
		for(i=0;i<10000;i++){
			for(j=0;j<10000;j++);
		}
*/
	}
        schedule();
}
/*
<Ring 0>
Function usage:transfer virtual address of a process to physical address
*/
PUBLIC  void* va2la(int pid,void*a)
{
        PROCESS_TABLE*p=process_table+pid-1;
        int     ds=p->p_regs.p_ds;
        DESCRIPTOR dtor=p->ldt[(ds&0xfffffff0)/8];
        int     baseaddr=dtor.base_low+dtor.base_mid<<16+dtor.base_high<<24;
        return (void*)(baseaddr+(int)a);
}
/*****************************************************************************
 *                                printIPC
 *****************************************************************************/
/**
 * <Ring 0>     get out all processes that are sending messages
 * 
 * 
 *****************************************************************************/
PUBLIC	void* printIPC(void)
{
	int i;
	PROCESS_TABLE*p;
	for(i=0;i<NR_TASKS+NR_PROCS;i++){
		if(process_table[i].invalid==0){
			p=process_table[i].sendingQ;
			printf_kernel(" Sending to %d: ",i+1);
			while(p!=0){
				printf_kernel("%d ",proc2pid(p));
				p=p->nextSendingPro;
			}
		}
	}
	printf_kernel("\n");
}
/*
<Ring 1>
wait for an interrupt to happen
*/
PUBLIC void interrupt_wait()
{
	MESSAGE msg;
	sendrec(RECEIVE,INTERRUPT,&msg);
}
