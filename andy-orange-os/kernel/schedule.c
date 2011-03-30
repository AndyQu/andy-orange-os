/***********************************************************************
                        schedule.c
***********************************************************************/
#include"const.h"
#include"type.h"
#include"global.h"


/*=============================schedule==============================*
Usage: select a process to run
Ring<0>
======================================================================*/
/*what if no processes are available? For example, dead lock occurs, and all processes are blocked*/
PUBLIC  void    schedule()
{
	int     i;
	int     largest_ticks=0;
	int     index=-1;
	// int     text_color=0x74;/*灰底红字*/
	int     non_blocks=0;
	int	last_pid=__processID;
	if(process_table[0].p_flags!=RUNNABLE){
		printf_kernel(" !tty task is blocked! ");
		__asm__ __volatile__("cli");
		__asm__ __volatile__("hlt");
	}
	for(i=__processID;i<NR_TASKS+NR_PROCS;i++){
		if(process_table[i].invalid==0 && process_table[i].p_flags==RUNNABLE){
			index=i;
			break;
		}
	}
	if(index==-1){
		for(i=0;i<__processID-1;i++){
			if(process_table[i].invalid==0 && process_table[i].p_flags==RUNNABLE){
				index=i;
				break;
			}
		}
	}
/*
	int j;
	for(i=0;i<1000;i++)
		for(j=0;j<10;j++)
			;
*/
	if(index==-1){
		if(process_table[__processID-1].p_flags==RUNNABLE){
//			printf_kernel("schedule:%d %d",last_pid,__processID);
			return;
		}
		printf_kernel("schedule:All processes are blocked\n");
		__asm__ __volatile__("cli");
		__asm__ __volatile__("hlt");
	}
	else{
		__processID=index+1;
//		printf_kernel("schedule:%d %d",last_pid,__processID);
		return;
	}
}
