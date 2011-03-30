
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                              keyboard.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                           Forrest Yu, December, 2003
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef	_KEYBOARD_TYPE_H_
#define	_KEYBOARD_TYPE_H_

#include"keyboard_const.h"
/************************************************************************/
/*                         Stucture Definition                          */
/************************************************************************/
/* Keyboard structure, 1 per console. */
typedef struct s_kb {
	//char*	p_head;			/* 指向缓冲区中下一个空闲位置 */
	//char*	p_tail;			/* 指向键盘任务应处理的字节 */
	t_32	head;
	t_32	tail;
	t_32	count;			/* 缓冲区中共有多少字节 */
	t_8	buf[KB_IN_BYTES];	/* 缓冲区 */
}KB_INPUT;

/*TTY struct*/
typedef	struct	s_tty{
	t_32	start_addr;
	t_32	end_addr;
	t_32	display_addr;
	t_32	cursor_addr;
}TTY;


#endif /* _TINIX_KEYBOARD_H_ */
