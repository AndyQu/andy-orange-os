
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
	//char*	p_head;			/* ָ�򻺳�������һ������λ�� */
	//char*	p_tail;			/* ָ���������Ӧ������ֽ� */
	t_32	head;
	t_32	tail;
	t_32	count;			/* �������й��ж����ֽ� */
	t_8	buf[KB_IN_BYTES];	/* ������ */
}KB_INPUT;

/*TTY struct*/
typedef	struct	s_tty{
	t_32	start_addr;
	t_32	end_addr;
	t_32	display_addr;
	t_32	cursor_addr;
}TTY;


#endif /* _TINIX_KEYBOARD_H_ */
