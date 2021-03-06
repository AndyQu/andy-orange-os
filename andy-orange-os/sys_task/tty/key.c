#include"type.h"
#include"const.h"
#include"keyboard_type.h"
#include"keymap.h"
#include"proto.h"
#include"assert.h"
#include"video_const.h"

extern	int disp_pos;
extern	int	__currentTTY;
PRIVATE	KB_INPUT	kb_input[1];
PRIVATE	t_bool	l_shift;
PRIVATE	t_bool	r_shift;
PRIVATE	t_bool	l_ctrl;
PRIVATE	t_bool	r_ctrl;
PRIVATE	t_bool	l_alt;
PRIVATE	t_bool	r_alt;
void	init_KB_INPUT(KB_INPUT* kb_buf);
char	get_scancode(KB_INPUT* kb_buf,int index);
char	shift_out(KB_INPUT* p);
void	shift_outM(KB_INPUT*p,int c);
int	add_head(KB_INPUT*p,char code);
int	add_tail(KB_INPUT*p,char code);
int	printChar(int tty,t_32 key);
void set_cursor(int pos);
void	init_KB_INPUT(KB_INPUT* kb_buf);
/*
RING<0>
This is the keyboard interrupt handler
*/
PUBLIC	void key_interrupt()
{
	/*get scan code from 8042,and append it in the buffer.*/
	t_8	value;
	value=in_byte(0x60);
	add_tail(kb_input,value);
}
/*
RING<0>
*/
PUBLIC	void	init_keyboard()
{
	init_KB_INPUT(kb_input);
	l_shift=FALSE;
	r_shift=FALSE;
	l_ctrl=FALSE;
	r_ctrl=FALSE;
	l_alt=FALSE;
	r_alt=FALSE;
}
/*
RING<1>
*/
PUBLIC	int	keyboard_read(TTY *p)
{
	/*deal with the buffer*/
	t_32	key=0;
	t_bool	make=FALSE;//make=TRUE:key presses,make=FALSE:key released

	char code1,code2;

	code1=get_scancode(kb_input,1);
	if(code1==-1)
		return -1;	
	/*Pause Press*/
	if((t_8)code1==0xE1){
		if(kb_input->count<4){
			//可能这个时候时钟中断还没有把扫描码写入缓冲区，所以暂时先不做处理。
			return -1;
		}
		key=PAUSEBREAK;
		make=TRUE;
		shift_outM(kb_input,6);
	}
	/*Special Keys*/
	else if((t_8)code1==0xE0){
		if(kb_input->count<2){
			//可能这个时候时钟中断还没有把扫描码写入缓冲区，所以暂时先不做处理。
			return -1;
		}
		code2=get_scancode(kb_input,2);
		if(code2>=0 && (unsigned int)code2<=0x7f){
			key=keymap[MAP_COLS*code2+2];//get the special key from keymap
			make=TRUE;
		}else if( code2 & FLAG_BREAK !=0 ){//if code2 & 0x080 !=0
			make=FALSE;
		}
		switch((t_8)code2){
			case 0x1D://right ctrl press
				r_ctrl=TRUE;
				break;
			case 0x1D+FLAG_BREAK://right ctrl released
				r_ctrl=FALSE;
				break;
			case 0x38://right alt press
				r_alt=TRUE;
				break;
			case 0x38+FLAG_BREAK://right alt released
				r_alt=FALSE;
				break;
			case 0x2A://Print Screen press
				if(kb_input->count<4)
					//可能这个时候时钟中断还没有把扫描码写入缓冲区，所以暂时先不做处理。
					return -1;
				else{
					key=PRINTSCREEN;
					make=TRUE;
					shift_outM(kb_input,4);
					return -1;
				}
			case 0xB7://Print Screen released
				if(kb_input->count<4)
					return -1;
				else{
					key=PRINTSCREEN;
					make=FALSE;
					shift_outM(kb_input,4);
					return -1;
				}
			default:
				break;
		}
		shift_outM(kb_input,2);
	}
	/*Normal keys Press*/
	else if(code1>=0 && (unsigned int)code1<=0x7f){
		switch(code1){
			case	0x36://right shift press
				r_shift=TRUE;
				break;
			case	0x2A://left shift press
				l_shift=TRUE;
				break;
			case	0x1D://left ctrl press
				l_ctrl=TRUE;
				break;
			case	0x38://left alt press
				l_alt=TRUE;
				break;
			default:	//normal character key scan code
				break;
		}
		key=keymap[code1*MAP_COLS];
		keymap[code1*MAP_COLS+3]=PRESSED;
		if(l_shift==TRUE||r_shift==TRUE)
			key=keymap[code1*MAP_COLS+1];
		make=TRUE;
		shift_out(kb_input);
	}
	/*Normal keys released*/
	else{
		switch((t_8)code1){
			case	0x36+FLAG_BREAK://right shift released
				r_shift=FALSE;
				break;
			case	0x2A+FLAG_BREAK://left shift released
				l_shift=FALSE;
				break;
			case	0x1D+FLAG_BREAK://left ctrl released
				l_ctrl=FALSE;
				break;
			case	0x38+FLAG_BREAK://left alt released
				l_alt=FALSE;
				break;
			default:
				break;
		}
		keymap[MAP_COLS*(code1&0x7f)+3]=RELEASED;
		make=FALSE;
		shift_out(kb_input);
	}

	if(make==TRUE){
		if(l_alt==1||r_alt==1){
			switch(key){
				case	F1:
				//	printf("ALT-F1 ");
					return	0;
				case	F2:
				//	printf("ALT-F2 ");
					return	1;
				case	F3:
				//	printf("ALT-F3 ");
					return	2;
				default:
					break;
			}
		}
		if(keymap[0x1E*MAP_COLS+3]==PRESSED)/*'a' pressed*/{
			if(keymap[0x2c*MAP_COLS+3]==PRESSED)/*'z' pressed*/{
				return 0;
			}
			if(keymap[0x2d*MAP_COLS+3]==PRESSED)/*'x' pressed*/{
				return 1;
			}
			if(keymap[0x2e*MAP_COLS+3]==PRESSED)/*'c' pressed*/{
				return 2;
			}
		}

		key|=l_shift?FLAG_SHIFT_L:0;
		key|=r_shift?FLAG_SHIFT_R:0;
		key|=l_alt?FLAG_ALT_L:0;
		key|=r_alt?FLAG_ALT_R:0;
		key|=l_ctrl?FLAG_CTRL_L:0;
		key|=r_ctrl?FLAG_CTRL_R:0;

		printChar(tty2id(p),key);
	}
	return	-1;
}


TTY* id2tty(int);
/*
RING<0><1>
*/
PUBLIC int printChar(int tty_id,t_32 key)
{
	//disable_int();
	char *output="1";
	int a=key&FLAG_EXT;
	
	TTY*p_tty=id2tty(tty_id);
	
	/*if 超出显示范围，则从头开始显示*/
	if(p_tty->cursor_addr>=p_tty->end_addr){
		p_tty->cursor_addr=p_tty->start_addr;
	}
	int ret=-1;
	disp_pos=p_tty->cursor_addr*2;
	if(a==0){
		output[0]=(char)(key&0xff);
		disp_str(output);
		ret=0;
	}else {
		int raw_code=key & MASK_RAW;
		switch(raw_code){
			case ENTER:
				disp_str("\n");
				//set_cursor(disp_pos/2);
				ret=0;
				break;
			case BACKSPACE:
				disp_pos-=2;
				ret=0;
				break;
			case UP:
				if( (key& FLAG_SHIFT_L) || (key& FLAG_SHIFT_R)){
					/*shift+UP*/
				}
				break;
			case DOWN:
				if( (key& FLAG_SHIFT_L) || (key& FLAG_SHIFT_R)){
					/*shift+DOWN*/
				}
				break;
			default:
				break;
		}
	}
//	disp_str(".");
	p_tty->cursor_addr=disp_pos/2;
	if(tty2id(p_tty)==__currentTTY){
		set_cursor(disp_pos/2);
	}
	int current_line=disp_pos/(2*80);
	int start_line=p_tty->display_addr/80;
	int lines=current_line-start_line;
	int diff=lines-24;
	/*如果超出了屏幕显示界限，则将起始显示地址重置为下一行。*/
	if(diff>=1){
		p_tty->display_addr+=diff*80;
		if(p_tty->display_addr+24*80>=p_tty->end_addr){
			p_tty->display_addr=p_tty->start_addr;
			p_tty->cursor_addr =p_tty->start_addr;
		}
		if(tty2id(p_tty)==__currentTTY){
			set_disp_addr(p_tty->display_addr);
		}
	}
	//enable_int();
	return ret;
}
/*
RING<0><1>
*/
PUBLIC void	printStr(int tty_id,char*s)
{
	int i;
	for(i=0;s[i]!=0;i++){
		printChar(tty_id, (t_32)s[i]);
	}
}
/*
RING<0><1>
*/
PUBLIC void set_cursor(int pos)
{

	out_byte(CRTC_ADDR_REG,CRTC_DATA_IDX_CURSOR_H);
	out_byte(CRTC_DATA_REG,(pos>>8)& 0xff);
	out_byte(CRTC_ADDR_REG,CRTC_DATA_IDX_CURSOR_L);
	out_byte(CRTC_DATA_REG,(pos)& 0xff);
}
/*
RING<0><1>
*/
PUBLIC void	set_disp_addr(int addr)
{
	out_byte(CRTC_ADDR_REG,CRTC_DATA_IDX_START_ADDR_H);
	out_byte(CRTC_DATA_REG,(addr>>8)&0xff);
	out_byte(CRTC_ADDR_REG,CRTC_DATA_IDX_START_ADDR_L);
	out_byte(CRTC_DATA_REG,(addr)&0xff);
}
