#ifndef _PROTECT_TYPE_H_
#define	_PROTECT_TYPE_H_

#include"type.h"
#include"const.h"
#include"protect_const.h"
#include"proc_const.h"
/*存储段描述符、系统段描述符*/
typedef struct s_descriptor
{
	t_16	limit_low;
	t_16	base_low;
	t_8	base_mid;
	t_8	attr1;
	t_8	limit_high_attr2;
	t_8	base_high;
}DESCRIPTOR;

/*中断描述符定义*/
typedef struct s_gate
{
	t_16	offset_low;
	t_16	selector;
	t_8	dcount;
	t_8	attr;
	t_16	offset_high;
}GATE;

/*TSS段定义*/
typedef struct s_tss
{
t_32 	tss_task_link;//   dd
t_32	tss_esp0     ;//   dd
t_32	tss_ss0      ;//   dw
                //dw
t_32	tss_esp1     ;//   dd
t_32	tss_ss1      ;//   dw
                //dw
t_32	tss_esp2     ;//   dd
t_32	tss_ss2      ;//   dw
                //dw
t_32	tss_gr3      ;//   dd
t_32	tss_eip      ;//   dd
t_32	tss_eflags   ;//   dd
t_32	tss_eax      ;//   dd
t_32	tss_ecx      ;//   dd
t_32	tss_edx      ;//   dd
t_32	tss_ebx      ;//   dd
t_32	tss_esp      ;//   dd
t_32	tss_ebp      ;//   dd
t_32	tss_esi      ;//   dd
t_32	tss_edi      ;//   dd
t_32	tss_es       ;//   dw
                //dw
t_32	tss_cs       ;//   dw
                //dw
t_32	tss_ss       ;//   dw
                //dw
t_32	tss_ds       ;//   dw
                //dw
t_32	tss_fs       ;//   dw
                //dw
t_32	tss_gs       ;//   dw
                //dw
t_32	tss_ldt      ;//   dw
                //dw
 
t_16	tss_trap;	//dw      0    
t_16	tss_io_bitmap;	//dw      0    
t_16	tss_io_bitmap_end;//dw      0xff 

}TSS;

#endif
