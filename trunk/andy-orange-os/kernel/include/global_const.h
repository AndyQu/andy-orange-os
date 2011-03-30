#ifndef	_GLOBAL_CONST_H_
#define	_GLOBAL_CONST_H_
//这个变量代表的是ldt选择子在进程表中的偏移
EXTERN	t_32	ldt_selector_offset;
EXTERN	t_32	size_process_table;
EXTERN	t_32	priority_offset;
EXTERN	t_32	regs_offset;
EXTERN	t_32	ticks_offset;
EXTERN	t_32	regs_bottom_offset;
EXTERN	t_32	regs_top_offset;
#endif
