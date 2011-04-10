%include"pm.inc"
SELECTOR_KERNEL_CS	equ	8
SelectorTss	equ	20h		;20h
SelectorP1_LDT	equ	0x40+SA_RPL3	;41h
LDT_SelectorP1_Code	equ	0x0+SA_RPL3+SA_TIL	;0x7
LDT_SelectorP1_DS	equ	0x18+SA_RPL3+SA_TIL	;0x1F
LDT_SelectorP1_ES	equ	0x10+SA_RPL3+SA_TIL	;0x17
LDT_SelectorP1_SS	equ	0x8+SA_RPL3+SA_TIL	;0x0F
P1_STACK_SIZE		equ	1000

P1_PID	equ	21;P1_PID should be equal to NR_TASKS+1, which is defined in /include/proc/proc_const.h
;导入函数和全局变量
extern init_Gdt_Idt;modified in chapter8.a
extern sys_init
extern gdt_ptr
extern idt_ptr
extern tss
extern p1
extern __processID
extern clock_times

[section .bss]
global	StackTop
StackSpace	resb 8*1024
StackTop:	;栈顶
StackSpaceFill	resb 512
[section .text]	;代码在此
global	_start	;导出_start
_start:
	;把esp从loader挪到Kernel
	mov	esp,	StackTop
	;加载gdt
	sgdt	[gdt_ptr]
	call	init_Gdt_Idt
	lgdt	[gdt_ptr]
	;加载idt
	lidt	[idt_ptr]
	;这个跳转用到了新的GDT
	jmp	SELECTOR_KERNEL_CS:csinit
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
csinit:
	call	sys_init
;加载task register 寄存器
	mov	ax,SelectorTss
	ltr	ax
;将当前代码段的es和esp填入tss段
	xor	eax,eax
	mov	ax,ss
	mov	[tss+8],eax;填充ss
	mov	dword[__processID],P1_PID;P1_PID must be the same as the index+1 of p1 in process_table
extern	setTssEspReady
	push	dword[__processID]
	call	setTssEspReady
	add	esp,4
	
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;加载第一个进程的LDT选择子到LDT寄存器
	mov	ax,SelectorP1_LDT
	lldt	ax
;将第一个C进程的es，esp，cs，eip压入堆栈
	mov	dword[clock_times],0
	
        mov     dh,0fh
        mov     dl,'A'
	mov     [gs:538],dx
	mov     [gs:540],dx
	mov     [gs:542],dx
	mov     [gs:544],dx
	mov     [gs:550],dx

	mov	eax,LDT_SelectorP1_SS
	push	eax
	mov	eax,P1_STACK_SIZE-1
	push	eax
	mov	eax,LDT_SelectorP1_Code
	push	eax
	mov	eax,p1
	push	eax
	mov	ax,LDT_SelectorP1_DS;在这里更改了ds和es的数值，使得指向LDT
	mov	ds,ax
	mov	ax,LDT_SelectorP1_ES
	mov	es,ax
	sti;打开中断
	retf
	jmp	$
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
