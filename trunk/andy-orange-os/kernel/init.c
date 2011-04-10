#include"proto.h"
#include"global.h"
#include"global_const.h"
#include"protect_const.h"
#include"8253_const.h"

PUBLIC void init_Gdt_Idt()/*function name changed from 'cstart' to 'init_Gdt_Idt'. in chapter8.a*/
{
	int i;
	disp_str("\n\n\n\n\n\n");
	disp_str("Kernel:\n");
	disp_str("Initializing GDT...........\n");
	/*将新的GDT表清零*/
	for(i=0;i<GDT_SIZE;i++){
		gdt[i].limit_low=0;
		gdt[i].base_low=0;
		gdt[i].base_mid=0;
		gdt[i].attr1=0;
		gdt[i].limit_high_attr2=0;
		gdt[i].base_high=0;
	}
	/*将loader中的gdt表的内容复制到新的gdt处*/
	memcpy(&gdt, (void*)( *( (t_32*)(&gdt_ptr[2]) )), *((t_16*)(&gdt_ptr[0]))+1);
	/*初始化gdt pointer*/
	t_16* p_gdt_limit=(t_16*)(&gdt_ptr[0]);
	t_32* p_gdt_base=(t_32*)(&gdt_ptr[2]);
	*p_gdt_limit= GDT_SIZE*sizeof(DESCRIPTOR)-1;
	*p_gdt_base=(t_32)&gdt;


	disp_str("Initializing IDT Pointer...\n");
	/*初始化idt pointer*/
	t_16* p_idt_limit=(t_16*)(&idt_ptr[0]);
	t_32* p_idt_base=(t_32*)(&idt_ptr[2]);
	*p_idt_limit= IDT_SIZE*sizeof(GATE)-1;
	*p_idt_base=(t_32)&idt;

}
void init_process_table();
void init_tss();
void init_prot();
t_32	clock_int_count=0;
void	set_timer();
void	init_sys_call();
int	in_sys_call=0;
PUBLIC	void sys_init()
{
	/*初始化idt*/
	disp_str("Initializing IDT entries...\n");
	init_prot();
	/*初始化系统调用函数表*/
	disp_str("Initializing sys calls.....\n");
	init_sys_call();
	/*初始化ticks变量，记录时钟中断次数的变量*/
	ticks=0;
	/*设置8253时钟*/
	disp_str("Initializing 8253 timer....\n");
	set_timer();
	disp_str("Initializing ttys .........\n");
	init_all_ttys();
	/*added in chapter8.a*/
	/*initialize process tables of the 3 processes*/
	MAX_CLOCK=50;
	MAX_CLOCK=20;
	init_process_table();
	/*initialize tss segment*/
	init_tss();
	k_reenter=0;
}


PUBLIC void set_timer()
{
	out_byte(TIMER_MODE,RATE_GENERATOR);
	out_byte(TIMER0,(t_8)(TIMER_FREQ/HZ));
	out_byte(TIMER0,(t_8)((TIMER_FREQ/HZ)>>8) );
}


PUBLIC void setGDTEntry(t_32 index, t_32 base, t_32 limit, t_16 attr)
{
	gdt[index].limit_low=(t_16)(limit&0x0000ffff);
	gdt[index].base_low= (t_16)(base &0x0000ffff);
	gdt[index].base_mid= (t_8) ((base>>16)&0x0ff);
	gdt[index].attr1=(t_8) (attr&0x00ff);
	gdt[index].limit_high_attr2=(t_8)( 
		((attr>>8)&0x0f0) 
		|
		((limit>>16)&0x0f)
	);
	gdt[index].base_high=(t_8)(
		(base>>24) &0x0ff
	);
}

/*

*/
PUBLIC void init_tss()
{
	tss->tss_io_bitmap_end=0xff;
	//TSS段描述符 放置于gdt的index 4处
	setGDTEntry(4,(int)(tss),sizeof(TSS)-1,DA_32+DA_DPL0+DA_386TSS);
}

