/*This file is added in chapter8/a*/
#include"type.h"
#include"const.h"
#include"protect_type.h"
#include"global.h"
#include"ipc_func.h"
#include"keyboard_type.h"
#include"key.h"


extern PUBLIC  int     __currentTTY;

TTY*id2tty(int);
PUBLIC  void task_tty()
{
	int new_tty;
	while(1){
		new_tty=keyboard_read( id2tty(__currentTTY));
		if( new_tty!=__currentTTY && new_tty!=-1 &&new_tty>=0&& new_tty<NR_TTY){
			switch_tty( id2tty(new_tty), id2tty(__currentTTY) );
			 __currentTTY=new_tty;
		 }
	}
}

/*
RING<1>
*/
extern  int disp_pos;
PUBLIC  void switch_tty(TTY*p,TTY*old)
{
  /*set display start address*/
  set_disp_addr(p->display_addr);
  /*set cursor address*/
  set_cursor(p->cursor_addr);
  /*load new tty state*/
  disp_pos=p->cursor_addr*2;
}
