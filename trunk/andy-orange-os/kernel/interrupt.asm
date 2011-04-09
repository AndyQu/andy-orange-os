extern	key_interrupt
extern	clock_interrupt
extern	process_table
extern	size_process_table
extern	ldt_selector_offset
extern	__processID
extern	clock_times
extern	MAX_CLOCK
extern	ticks
extern	schedule
global	key_int
global	clock_int
[section .text]
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
extern	in_sys_call
extern	clock_int_count
;时钟中断处理
clock_int:
extern	StackTop
extern	debug
	inc	dword[ticks]
	inc	byte[gs:538];每一次时钟中断，改变一次
	call	Save

	;发送EOI，使得i8259继续接收中断
	mov	al,20h
	out	20h,al
	sti

	inc	dword[clock_times]
	mov	eax,[clock_times]
	cmp	eax,dword[MAX_CLOCK]
	jbe	.noswitch
	mov	dword[clock_times],0
	call	schedule
.noswitch:
	cli
	ret
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;键盘中断处理
key_int:
extern	StackTop
	inc	byte[gs:542];每次键盘中断一次，改变一次
	call	Save
	mov	al,20h
	out	20h,al;发送EOI，使得i8259继续接收中断
	sti
;调用键盘函数
	call	key_interrupt
	cli
	ret
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;硬盘中断处理
extern hd_handler
global disk_int
disk_int:
	inc byte[gs:544];每次硬盘中断一次，改变一次
	call	Save
	mov	al,20h
	out	0A0h,al
	mov	al,20h
	out	020h,al
	sti

	call hd_handler
	cli
	ret
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;Save:
;Description:
;	This function is used for saving regs of the interrupted process.
;	Should be called at the beggining of an interrupt procedure.
;Attention: "Save" will be called in "kernel/sys_call_shell.asm:sys_call".
;	Because sys_call needs eax,ebx,ecx,edx as parameters, So "Save" should not modify any register of the 4.
global	Save
Save:
extern	k_reenter
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	;给ds赋值
	mov	di,ss
	mov	ds,di
	mov	es,di

	mov edi,esp

	inc	dword[k_reenter]
	cmp	dword[k_reenter],1
	ja	.reenter
	mov	esp,StackTop
	push restart;将restart函数地址压入堆栈
	jmp [edi+4*12]
.reenter:
	push restart_reenter;将restart_reenter函数地址压入堆栈
	jmp [edi+4*12]
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;restart:
;Description:
;	This function is used for loading regs and LDT of the target process.
;	Should be called at the very end of an interrupt procedure. It will change stack to target process regs structure,
;	restore regs and LDT reg, and use iretd to return to target process directly.
global	restart
restart:
extern	setTssEspReady
extern	getProRegTop
extern	getProPointer
extern	ldt_selector_offset
extern	ready_p
	push	dword[__processID]
	call	setTssEspReady;set TSS ESP ready for Interrupt of next time
	add	esp,4

	push	dword[__processID]
	call	getProRegTop;get target process regs address,put in "ready_p"
	add	esp,4

	push	dword[__processID]
	call	getProPointer;get target process pointer, put it at eax
	add	esp,4
	;load LDT selector
	mov     esi,eax
	add     esi,[ldt_selector_offset]
	mov     eax,[esi]
	lldt    ax
	mov	esp,dword[ready_p]

restart_reenter:
	dec	dword[k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp,4;skip over the ret stub
	
	iretd
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
