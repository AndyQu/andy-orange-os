#include"const.h"
#include"type.h"
#include"keyboard_type.h"
#include"proto.h"
#include"key.h"
#include"assert.h"

PRIVATE	TTY	tty_table[NR_TTY];
PUBLIC	int	__currentTTY;
PUBLIC	void init_tty(TTY*p,int startaddr,int endaddr)
{
	if(startaddr>=endaddr){
		disp_str("init_tty error:startaddr bigger than endaddr\n");
		return;
	}
	p->start_addr=startaddr;
	p->end_addr=endaddr;
	p->display_addr=p->start_addr;
	p->cursor_addr=p->start_addr;
}
extern	int	disp_pos;
void switch_tty(TTY*p,TTY*old);
PUBLIC int tty2id(TTY*p);
PUBLIC TTY* id2tty(int id);
PUBLIC	void	init_all_ttys()
{
	/*初始化键盘相关数据*/
	init_keyboard();
	/*初始化tty相关数据*/
	init_tty( id2tty(0),80*TTY0_START_LINE,80*(TTY0_START_LINE+TTY_LINES));
	tty_table[0].cursor_addr=disp_pos/2;	
	printStr(0,".............TTY0.............\n");

	init_tty( id2tty(1),80*TTY1_START_LINE,80*(TTY1_START_LINE+TTY_LINES));
	printStr(1,".............TTY1.............\n");

	init_tty( id2tty(2),80*TTY2_START_LINE,80*(TTY2_START_LINE+TTY_LINES));
	printStr(2,".............TTY2.............\n");

	__currentTTY=0;

}

PUBLIC	void switch_tty(TTY*p,TTY*old)
{
	/*save current tty state*/
//	old->cursor_addr=disp_pos/2;
	/*set display start address*/
	set_disp_addr(p->display_addr);
	/*set cursor address*/
	set_cursor(p->cursor_addr);
	/*load new tty state*/
	disp_pos=p->cursor_addr*2;
}

PUBLIC int tty2id(TTY*p)
{
	assert(p);
	return (p-tty_table);
}
PUBLIC TTY* id2tty(int id)
{
	assert(id>=0);
	return tty_table+id;
}