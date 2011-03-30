[section .text]
extern	disp_int
extern	vsprintf
extern	printx

global	myprintf
;printf(const char*fmt, ...)
myprintf:
	push	ebp
	mov		ebp,esp
	
	sub		esp,64


	;fmt入栈
	mov		eax,[ebp+4*2]
	push	eax		
	;不定参数的地址入栈
	mov		eax,ebp
	add		eax,3*4
	push	eax		
	;buf数值入栈
	mov		eax,ebp
	sub		eax,64
	push	eax
	;vsprintf(buf,fmt,...)
	call	vsprintf ;此时返回值保存在eax中
	add		esp,4*3



	;i入栈
	push	eax
	;buf数值入栈
	mov		eax,ebp
	sub		eax,64
	push	eax
	;write(buf,i)
	call	printx;此时返回值保存在eax中
	add		esp,4*2



	;恢复堆栈	
	add		esp,64

	mov		esp,ebp
	pop		ebp

	ret

