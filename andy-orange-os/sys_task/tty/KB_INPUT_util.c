#include"type.h"
#include"const.h"
#include"keyboard_type.h"
#include"assert.h"

/*
RING<1><0>
*/
PUBLIC	void	init_KB_INPUT(KB_INPUT* kb_buf)
{
	if(kb_buf==NULL)return;
	kb_buf->head=0;
	kb_buf->tail=1;
	kb_buf->count=0;
}

/*
RING<1>
*/
PUBLIC	char	get_scancode(KB_INPUT* kb_buf,int index)
{
	int a;
	assert(kb_buf);
	if(index>kb_buf->count||index<=0)
		return	-1;
	if(kb_buf->head<kb_buf->tail){
			return	kb_buf->buf[kb_buf->head+index];
	}
	else if(kb_buf->head>kb_buf->tail){
		if(kb_buf->head+index>KB_IN_BYTES-1)
			a=kb_buf->head+index-(KB_IN_BYTES);
		else 
			a=kb_buf->head+index;
		return	kb_buf->buf[a];

	}
	return -1;
}

/*
RING<1>
*/
PUBLIC	char	shift_out(KB_INPUT* p)
{
	assert(p);
	if(p->count<=0)
		return	-1;
	if(p->head < p->tail){
		p->head++;
		p->count--;
		return	p->buf[p->head];
	}
	else if(p->head > p->tail){
		if(p->head==KB_IN_BYTES-1){
			p->head=0;
			p->count--;
			return	p->buf[p->head];
		}else{
			p->head++;
			p->count--;
			return	p->buf[p->head];
		}
	}
}
/*
RING<1>
*/
PUBLIC	void	shift_outM(KB_INPUT*p,int c)
{
	int i=0;
	for(;i<c;i++){
		shift_out(p);
	}
}

/*
RING<1>
*/
PUBLIC	int	add_head(KB_INPUT*p,char code)
{
	assert(p);
	if(p->count>=KB_IN_BYTES-2)
		return	FALSE;
	if(p->head<p->tail){
		if(p->head==0){
			p->buf[0]=code;
			p->head=KB_IN_BYTES-1;
			p->count++;
		}
		else{
			p->buf[p->head]=code;
			p->head--;
			p->count++;
		}
	}
	else if(p->head>p->tail){
		p->buf[p->head]=code;
		p->head--;
		p->count++;
	}
	return TRUE;
}

/*
RING<0><1>
*/
PUBLIC	int	add_tail(KB_INPUT*p,char code)
{
	if(p==NULL)return FALSE;
	if(p->count>=KB_IN_BYTES-2)
		return	FALSE;
	if(p->head<p->tail){
		if(p->tail==KB_IN_BYTES-1){
			p->buf[p->tail]=code;
			p->tail=0;
			p->count++;
		}
		else{
			p->buf[p->tail]=code;
			p->tail++;
			p->count++;
		}
	}
	else if(p->head>p->tail){
		p->buf[p->tail]=code;
		p->tail++;
		p->count++;
	}
	return TRUE;
}
