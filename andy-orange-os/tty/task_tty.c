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
	//panic("TEST");
	while(1){
		new_tty=keyboard_read( id2tty(__currentTTY));
		if( new_tty!=__currentTTY && new_tty!=-1 &&new_tty>=0&& new_tty<NR_TTY){
		//	printf("switch to %d",new_tty);
			switch_tty( id2tty(new_tty), id2tty(__currentTTY) );
			 __currentTTY=new_tty;
		 }
	}
}
