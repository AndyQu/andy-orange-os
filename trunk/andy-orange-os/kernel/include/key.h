#ifndef	_ORANGES_KEY_H_
#define	_ORANGES_KEY_H_
PUBLIC	void	init_keyboard();
PUBLIC	int	keyboard_read(TTY *p);
PUBLIC	void	init_keyboard();
PUBLIC	void	printStr(int tty_id,char*s);
PUBLIC	void	printStrLen(int tty_id,char*s,int len);
PUBLIC	void	set_disp_addr(int addr);
PUBLIC	void	set_cursor(int pos);
PUBLIC	int	printChar(int tty,t_32 key);
#endif
