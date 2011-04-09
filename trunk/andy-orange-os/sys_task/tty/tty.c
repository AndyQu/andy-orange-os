#include"const.h"
#include"type.h"
#include"keyboard_type.h"
#include"proto.h"
#include"key.h"
#include"assert.h"

extern int disp_pos;
extern PUBLIC  TTY tty_table[NR_TTY];
/*
RING<1>
*/
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

/*
RING<1>
*/
PUBLIC int tty2id(TTY*p)
{
	assert(p);
	return (p-tty_table);
}
/*
RING<1>
*/
PUBLIC TTY* id2tty(int id)
{
//	printf("ttyid:%d ",id);
	assert(id>=0);
	return tty_table+id;
}
