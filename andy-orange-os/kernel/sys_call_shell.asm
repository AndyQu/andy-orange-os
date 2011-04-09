
extern  ticks
extern  sys_call_table
extern	__processID


extern	save_sys_call_ret

global  sys_call
global	printx
global	sendrec

extern	Save
extern	restart

extern	in_sys_call
[section .text]
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;系统调用中断处理函数
;注：系统调用不会在内核层中被调用，因此不必进行判断。
__sys_call_local dd 0
sys_call:
	cli
	mov	dword[in_sys_call],1
	call    Save
	push	edx
	mov	edx,dword[__processID]
	mov dword[__sys_call_local],edx
	pop	edx
	sti

	push	ebp
	mov	ebp,esp

	push	edx
	push	ecx
	push	ebx
	push	dword[__sys_call_local];
	call    [sys_call_table+eax*4]
	add	esp,4*4

	mov	esp,ebp
	pop	ebp

	cli
	mov	dword[in_sys_call],0
	ret
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;get_ticks
;global	get_ticks
;get_ticks:
;	mov     eax,0
;	int     0x90
;	ret
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;public int printx(char*buf, int len)
printx:
;堆栈情况，从栈顶开始
;caller ip
;char *buf
;int len	
	push	ebp
	mov		ebp,esp

	mov		ebx,[ebp+4*2];char*buf
	mov		ecx,[ebp+4*3];int len
	mov		eax,1
	int		0x90
	
	mov		esp,ebp
	pop		ebp
	ret
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;public int sendrec(int function,int src_dest,MESSAGE*m)
;"m" belong to the memory space of the calling process
sendrec:
	push	ebp
	mov	ebp,esp
	
	mov	ebx,[ebp+4*2]; int function
	mov	ecx,[ebp+4*3]; int src_dest
	mov	edx,[ebp+4*4]; MESSAGE* m
	mov	eax,2
	int	0x90

	mov	esp,ebp
	pop	ebp
	ret

