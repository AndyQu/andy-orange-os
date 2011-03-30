#include"const.h"
#include"type.h"
#include"protect_type.h"
#include"global.h"
#include"keyboard_type.h"
#include"ipc.h"
#include"proc.h"
#include"console_const.h"
#include"key.h"
#include"proto.h"
#include"video_const.h"

PUBLIC	void	save_sys_call_ret(int pid,int eax)
{
	process_table[pid-1].p_regs.p_eax=eax;
}

PUBLIC  int     sys_get_ticks()
{
        return  ticks;
}
PUBLIC	int	sys_printx(int pid, char* buf, int len)/*sys_write changed to "sys_printx"*/
{
	char*p=0;
	char reenter_error[]="? k_reenter is incorrect for unknown reason";
	reenter_error[0]=MAG_CH_PANIC;

	/**
         * @note Code in both Ring 0 and Ring 1~3 may invoke printx().
         * If this happens in Ring 0, no linear-physical address mapping
         * is needed.
         *
         * @attention The value of `k_reenter' is tricky here. When
         *   -# printx() is called in Ring 0
         *      - k_reenter > 0. When code in Ring 0 calls printx(),
         *        an `interrupt re-enter' will occur (printx() generates
         *        a software interrupt). Thus `k_reenter' will be increased
         *        by `kernel.asm::save' and be greater than 0.
         *   -# printx() is called in Ring 1~3
         *      - k_reenter == 0.
         */
	if(k_reenter==0){
		p=(char*)va2la(pid,(void*)buf);
	}else if(k_reenter>0){
		p=buf;
	}else{
		p=reenter_error;
	}
	if(pid==0){
		p=buf;
	}
	/*the first character MAG_CH_PANIC or MAG_ASSERT indicates that this function is called by assert() or panic()*/
	else if( *p==MAG_CH_PANIC || (*p==MAG_CH_ASSERT && pid<NR_TASKS) ){
		disable_int();
		char*v =(char*)VIDEO_MEM_BASE;
		const char*q=p+1;/*skip the magic char*/
		while(v<(char*)(VIDEO_MEM_BASE+VIDEO_MEM_SIZE)){
			*v=*q;
			v++;q++;
			*v=RED_CHAR;
			v++;
			if(*q==0){
				while( ((int)v - VIDEO_MEM_BASE) %(SCR_WIDTH*16) ){
					v++;
					*v=GRAY_CHAR;
					v++;
				}
				q=p+1;
			}
		}
		__asm__ __volatile__("hlt");
	}
	/*print out normally*/
        int i=0;
	int tty_id=process_table[pid-1].tty_id;
	int count=0;
        for(i=0;i<len && buf[i]!=0;i++){
		if(buf[i]==MAG_CH_PANIC||buf[i]==MAG_CH_ASSERT){
			continue;
		}
		if(printChar(tty_id,(t_32)buf[i])==0)
			count++;
        }
	return i;
}

/*added in chapter8/a*/
/*
"m" belong to the memory space of the calling process
*/
PUBLIC	int	sys_sendrec(int pid,int function,int src_dest,MESSAGE*m)
{
	if(function==SEND){
		m->source=pid;
		msg_send(pid,src_dest,m);
		if(pid==1||src_dest==1){
			printf_kernel(" #%d sending to %d# ",pid,src_dest);
		}
	}else if(function==RECEIVE){
		if(pid==1||src_dest==1){
			printf_kernel(" #%d receiving from %d %d# ",pid,src_dest,m->u.m1.m1i1);
		}
		msg_receive(pid,src_dest,m);
	}else{
		panic("Invalid function description in sys_sendrec");
	}
	return	1;
}
/*added in chapter8/a end*/

PUBLIC	void	init_sys_call()
{
/*
系统调用get_ticks()已经使用消息传递机制实现，所以这里去掉sys_get_ticks
*/
//	sys_call_table[__NR_get_ticks]=(void*)sys_get_ticks;
	sys_call_table[__NR_printx]=(void*)sys_printx;
	/*added in chapter8/a*/
	sys_call_table[__NR_sendrec]=(void*)sys_sendrec;
	/*added in chapter8/a end*/
}
