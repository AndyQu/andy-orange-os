#include"type.h"
#include"const.h"
#include"protect_type.h"
#include"global.h"
#include"keyboard_type.h"
#include"proto.h"
#include"proc.h"

void divide_error();
void single_step_exception();
void nmi();
void breakpoint_exception();
void overflow();
void bounds_check();
void inval_opcode();
void copr_not_available();
void double_fault();
void copr_seg_overrun();
void inval_tss();
void segment_not_present();
void stack_exception();
void general_protection();
void page_fault();
void copr_error();
void align_error();
void machine_error();
void simd_error();

void clock_int();
void key_int();

void init_8259A();

void sys_call();
PUBLIC void init_idt_desc(unsigned char vector, t_8 desc_type, t_pf_int_handler handler, unsigned char privilege)
{
	GATE*	p_gate=	&idt[vector];
	t_32	base=	(t_32)handler;
	p_gate->offset_low=	base& 0xffff;
	p_gate->offset_high=	(base>>16) &0xffff;
	p_gate->selector=	SELECTOR_KERNEL_CS;
	p_gate->attr=		desc_type|(privilege<<5);
	p_gate->dcount=		0;
}

PUBLIC void init_prot()
{
	init_8259A();
	//全部初始化为中断门（没有陷阱门）
	init_idt_desc(	INT_VECTOR_DIVIDE,	DA_386IGate,	divide_error,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_DEBUG,	DA_386IGate,	single_step_exception,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_NMI,		DA_386IGate,	nmi,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_BREAKPOINT,	DA_386IGate,	breakpoint_exception,	PRIVILEGE_USER);

	init_idt_desc(	INT_VECTOR_OVERFLOW,	DA_386IGate,	overflow,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_BOUNDS,	DA_386IGate,	bounds_check,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_INVAL_OP,	DA_386IGate,	inval_opcode,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_COPROC_NOT,	DA_386IGate,	copr_not_available,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_DOUBLE_FAULT,	DA_386IGate,	double_fault,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_COPROC_SEG,	DA_386IGate,	copr_seg_overrun,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_INVAL_TSS,	DA_386IGate,	inval_tss,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_SEG_NOT,	DA_386IGate,	segment_not_present,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_STACK_FAULT,	DA_386IGate,	stack_exception,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_PROTECTION,	DA_386IGate,	general_protection,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_PAGE_FAULT,	DA_386IGate,	page_fault,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_COPROC_ERR,	DA_386IGate,	copr_error,	PRIVILEGE_KRNL);

	init_idt_desc(	INT_VECTOR_ALIGN_ERR,	DA_386IGate,	align_error,	PRIVILEGE_KRNL);
	init_idt_desc(	INT_VECTOR_MACHINE_ERR,	DA_386IGate,	machine_error,	PRIVILEGE_KRNL);
	init_idt_desc(	INT_VECTOR_SIMD_ERR,	DA_386IGate,	simd_error,	PRIVILEGE_KRNL);

	/*时钟中断处理向量*/
	init_idt_desc(	INT_VECTOR_IRQ0,	DA_386IGate,	clock_int,	PRIVILEGE_KRNL);
	/*键盘中断处理向量*/
	init_idt_desc(	INT_VECTOR_IRQ0+1,	DA_386IGate,	key_int,	PRIVILEGE_KRNL);

	/*Mini OS 系统调用中断*/
	init_idt_desc(	INT_VECTOR_SYS_CALL,	DA_386IGate,	sys_call,	PRIVILEGE_USER);
}

/*=============================set_hd_handler==============================*
set hard disk interrupt handler
Ring<0> & Ring<1>
======================================================================*/
PUBLIC void set_hd_handler(t_pf_int_handler handler)
{
	init_idt_desc(INT_VECTOR_DISK, DA_386IGate, handler, PRIVILEGE_KRNL);
}

/*下面这个函数作为前16个中断向量对应的中断处理函数:被上层的汇编中断函数调用*/
extern int flag;
extern int clock_int_count;
extern int k_reenter;
PUBLIC void exception_handler(
	int	vec_no,
	int	err_code,
	int	eip,
	int	cs,
	int	eflags
){
	int	text_color=0x74;/*灰底红字*/
	PROCESS_TABLE*p=pid2proc(__processID);
	printf_kernel("*:This is the exception_handler:%d",vec_no);
	printf_kernel(":Error Code:%d",err_code);
	printf_kernel(":Process ID:%d",__processID);
	printf_kernel(":CS Register:%d",cs);
	printf_kernel(":EIP Register:%d",eip);
	printf_kernel(":nested clock number:%d",clock_int_count);
	printf_kernel(":k_reenter:%d",k_reenter);
	printf_kernel(":P CS Register:%d",p->p_regs.p_cs);
	printf_kernel(":P EIP Register:%d",p->p_regs.p_eip);
	printIPC();
}

/*
debug()
Description:
	This function is just for debugging purpose. It prints out some information about process tables.
@param:
	stub
@return
*/
PUBLIC void debug(int stub)
{
	char*arg=(char*)&stub-4;
//	printf_kernel("#jumpInRet:%d ",*(int*)arg);
	arg+=8;
	printf_kernel("#jumpIn clockRetEip:%d#",*(int*)arg);
//	arg+=4;
//	printf_kernel("clockRetCS:%d ",*(int*)arg);
//	printf_kernel("NestedClock#");
}

/*
jumpout()
Description:
	This function is just for debugging purpose.
	it is called when os jumps out the nested clock interrupt.
@param:
	int stub
@return

--------------
int stub
return address
EIP
CS
--------------
*/
PUBLIC void jumpout(int stub)
{
	char*arg=(char*)&stub-4;
	arg+=8;
	printf_kernel("#jumpOut clockRetEip:%d#",*(int*)arg);
//	arg+=4;
//	printf_kernel("clockRetCS:%d ",*(int*)arg);
//	printf_kernel("JumpOout#");
}
/*
printInterrupted()
Description:
	This function is called by clock interrupt handler to print out information about interrupted process
@param:
@return
*/
void printInterrupted()
{
	PROCESS_TABLE*p=pid2proc(__processID);
	printf_kernel("*EnterCLock");
	printf_kernel("IntEip:%d*",p->p_regs.p_eip);
}
