#include"global.h"
#include"global_const.h"
PUBLIC void setTssEspReady(int pid)
{
	tss->tss_esp0=(int)(process_table+pid-1)+regs_bottom_offset;
}
int	ready_p=0;
PUBLIC void getProRegTop(int pid)
{
	ready_p=(int)(process_table+pid-1)+regs_top_offset;
}
