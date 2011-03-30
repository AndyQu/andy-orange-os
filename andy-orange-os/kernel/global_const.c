#include"type.h"
#include"const.h"
#include"protect_type.h"
#include"global.h"
PUBLIC	const	t_32	ldt_selector_offset=(int)(&(process_table->ldt_selector))-(int)process_table;
PUBLIC	const	t_32	priority_offset=(int)(&(process_table->priority)) - (int)process_table;
PUBLIC	const	t_32	ticks_offset=	(int)(&(process_table->ticks))    - (int)process_table;
PUBLIC	const	t_32	regs_offset=	(int)(&(process_table->p_regs))	-(int)process_table;

//added chapter8/Macro_Kernel_ProcChanged
PUBLIC	const	t_32	regs_size=	sizeof(REGS);

PUBLIC	const	t_32	gs_offset=	(int)(&(process_table->p_regs.p_gs))-(int)process_table;
PUBLIC	const	t_32	fs_offset=	(int)(&(process_table->p_regs.p_fs))-(int)process_table;
PUBLIC	const	t_32	es_offset=	(int)(&(process_table->p_regs.p_es))-(int)process_table;
PUBLIC	const	t_32	ds_offset=	(int)(&(process_table->p_regs.p_ds))-(int)process_table;

PUBLIC	const	t_32	edi_offset=	(int)(&(process_table->p_regs.p_edi))-(int)process_table;
PUBLIC	const	t_32	esi_offset=	(int)(&(process_table->p_regs.p_esi))-(int)process_table;
PUBLIC	const	t_32	ebp_offset=	(int)(&(process_table->p_regs.p_ebp))-(int)process_table;

PUBLIC	const	t_32	ebx_offset=	(int)(&(process_table->p_regs.p_ebx))-(int)process_table;
PUBLIC	const	t_32	edx_offset=	(int)(&(process_table->p_regs.p_edx))-(int)process_table;
PUBLIC	const	t_32	ecx_offset=	(int)(&(process_table->p_regs.p_ecx))-(int)process_table;
PUBLIC	const	t_32	eax_offset=	(int)(&(process_table->p_regs.p_eax))-(int)process_table;

PUBLIC	const	t_32	eip_offset=	(int)(&(process_table->p_regs.p_eip))-(int)process_table;
PUBLIC	const	t_32	cs_offset=	(int)(&(process_table->p_regs.p_cs))-(int)process_table;
PUBLIC	const	t_32	eflags_offset=	(int)(&(process_table->p_regs.p_eflags))-(int)process_table;
PUBLIC	const	t_32	esp_offset=	(int)(&(process_table->p_regs.p_esp))-(int)process_table;
PUBLIC	const	t_32	ss_offset=	(int)(&(process_table->p_regs.p_ss))-(int)process_table;
//added chapter8/Macro_Kernel_ProcChanged

//added in chapter8.c
PUBLIC	const	t_32	regs_bottom_offset=	(int)(&(process_table->p_regs.p_ss))-(int)process_table+sizeof(t_32);
PUBLIC	const	t_32	regs_top_offset=	(int)(&(process_table->p_regs.p_gs))-(int)process_table;
//added in chapter8.c	end

PUBLIC	const	t_32	size_process_table= sizeof(PROCESS_TABLE);
