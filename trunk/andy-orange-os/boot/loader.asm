org	0100h
jmp	short LABEL_START
;下面是FAT12磁盘的头，之所以在loader中包含它是因为用到了磁盘的一些信息
%include "fat12hdr.inc"
%include "pm.inc"
%include "load.inc"

BaseOfStack equ	0100h
OffsetOfDir		equ	09000h;根目录扇区被加载的位置－－－偏移地址
BaseOfFatEntry		equ	07000h;fat表被加载的位置－－－－－段地址


;全局描述符表
LABEL_GDT: Descriptor 0,0,0 ;空描述符
;0～4G的可执行段
LABEL_DESC_FLAT_C:Descriptor	0,	0fffffh,	DA_CR|DA_32|DA_LIMIT_4K
;0～4G的可读写段
LABEL_DESC_FLAT_RW:Descriptor	0,	0fffffh,	DA_DRW|DA_32|DA_LIMIT_4K
;指向显存的段，优先级为3(DPL)
LABEL_DESC_VIDEO:Descriptor	0B8000h,0ffffh,		DA_DRW|DA_DPL3

GdtLen	equ	$-LABEL_GDT
GdtPtr	dw	GdtLen-1
	dd	BaseOfLoaderPhyAddr+LABEL_GDT
;选择子
SelectorFlatC	equ	LABEL_DESC_FLAT_C-LABEL_GDT
SelectorFlatRW	equ	LABEL_DESC_FLAT_RW-LABEL_GDT
SelectorSS	equ	LABEL_DESC_FLAT_RW-LABEL_GDT
SelectorES	equ	LABEL_DESC_FLAT_RW-LABEL_GDT
SelectorDS	equ	LABEL_DESC_FLAT_RW-LABEL_GDT
SelectorVideo	equ	LABEL_DESC_VIDEO-LABEL_GDT+SA_RPL3 ;这个选择子的RPL为3

;引导区代码起始处--------------------------------------------
LABEL_START:
	mov	ax,cs
	mov	ds,ax
	mov	es,ax
	mov	ss,ax
	mov	sp,BaseOfStack
	mov	dh,0
	call	DispStrRealMode;显示"Loading"字符串
	;软驱复位
	xor	ah,ah
	xor	dl,dl
	int	13h
;下面在A盘的根目录中寻找kernel.bin----------------------------
	;读根目录的第一二个扇区，到bx指向的内存处
	;es=cs=BaseOfLoader=0x8000，bx=OffsetOfDir=09000h。所以根目录被加载在0x8000:9000即0x89000处
	mov	bx,OffsetOfDir
	mov	ax,SectorNoOfRootDirectory
	mov	cl,2
	call	ReadSector

	mov	bx,OffsetOfDir
	sub	bx,32
	mov	dx,0
.NextItem:
	add	bx,32
	cmp	dx,32
	ja	.CantFindKernel
	inc	dx
	mov	si,bx
	mov	di,KernelFileName
	mov	cx,11
	.loop:
	lodsb
	cmp	al,[di]
	jnz	.NextItem
	inc	di
	loop	.loop
	;如每个字符都匹配，则表明成功找到kernel.bin的目录项,显示"K"
	;mov     ax,0b800h
        ;mov     gs,ax
        ;mov     ah,0ch
        ;mov     al,'K'
        ;mov     [gs:((80*1+60)*2)],ax
;将loader.com复制到内存中去
.Loading:
	;bx此时保存了loader目录项在内存中的偏移
	mov	ax,BaseOfKernelFile
	mov	es,ax
	add	bx,0x1A
	mov	ax,[ds:bx];ax此时保存了loader的第一个扇区号码
	
	;es=BaseOfKernelFile=0x9000，bx=OffsetOfKernelFile=0x0，所以kernel.bin文件被加载处0x9000:0000即0x90000处
	mov	cl,1
	mov	bx,OffsetOfKernelFile
	.NextOne:
		;每次读扇区之前，先打印出这个扇区在数据区中的簇号
	;	push    ax
        ;        push    bx
        ;        mov     ah,0eh
        ;       add	al,'0'
	;	mov     bl,0fh
        ;        int     10h
        ;        pop     bx
        ;        pop     ax
	push	ax
	add	ax,RootDirSectors
	add	ax,17
	call	ReadSector
		;每次读完扇区之后，打印出一个L字样
	;	push	ax
	;	push	bx
	;	mov	ah,0eh
	;	mov	al,'L'
	;	mov	bl,0fh
	;	int	10h
	;	pop	bx
	;	pop	ax
	;调用GetFATEntry函数得到下一个扇区号
	pop	ax
	call	GetFATEntry
	add	bx,[BPB_BytsPerSec]
	cmp	ax,0xff8
	jb	.NextOne
.Loaded:
	call	KillMonitor
	mov	dh,1
	call	DispStrRealMode;显示"KERNELDON"字样
;跳入保护模式:
	lgdt	[GdtPtr] ;加载gdt寄存器
	cli
	in	al,92h
	or	al,00000010b
	out	92h,al
	;打开保护机制
	mov	eax,cr0
	or	eax,1
	mov	cr0,eax
	;跳入保护模式代码
	jmp	dword SelectorFlatC:(BaseOfLoaderPhyAddr+LABEL_PM_START)

.CantFindKernel:
	mov	dh,2
	call	DispStrRealMode
	mov	ax,4c00h
	int	21h
;-------------------------------------------------------------
;关闭软驱马达
KillMonitor:
	push	dx
	mov	dx,03f2h
	mov	al,0
	out	dx,al
	pop	dx
	ret
;-------------------------------------------------------------
;输入:dh为message的序号  输出:在屏幕上显示字符串
DispStrRealMode:
	push	ax
	push	bp
	push	bx
	push	es
	push	cx
	push	dx

	mov	ax,MessageLength
	mul	dh
	add	ax,LoadMessage
	mov	bp,ax
	mov	ax,ds
	mov	es,ax
	mov	cx,MessageLength
	mov	ax,01301h
	mov	bx,0007h
	mov	dl,0
	add	dh,4
	int	10h
	
	pop	dx	
	pop	cx	
	pop	es
	pop	bx
	pop	bp
	pop	ax

	ret
;--------------------------------------------------------------
;ReadSector----------------------------------------------------
;从ax个Sector开始，将cl个Sector读入es:bx中,bx的数值不能被改变
ReadSector:
	push	dx
	push	ax
	push	cx
	push	bx

	mov	byte[SecSize],cl;保存要读的扇区数
	push	bx

	mov	bl,[BPB_SecPerTrk]
	div	bl
	;取得起始扇区号,cl
	mov	cl,ah
	inc	cl	;ah中保存的是余数，加一得起始扇区数
	;取得磁头号,dh
	mov	dh,al	;al中保存商
	and	dh,0x01
	;取得柱面号,ch
	shr	al,1
	mov	ch,al
	;赋值驱动器号,dl
	mov	dl,0
	mov	ah,02h
	pop	bx
.GoOnReading:
	;取得要读的扇区数,al
	mov	al,[SecSize]
	;指定复制方式为第二种
	mov	ah,02h
	;调用中断
	int	13h
	jc	.GoOnReading;如果读取错误，cf会被置位，这时就继续读

	pop	bx
	pop	cx
	pop	ax
	pop	dx
	ret
;-------------------------------------------------------------------
;GetFATEntry--------------------------------------------------------
;输入:ax中存放文件的扇区号码
;输出:ax中存放起对应FAT项的数值，即下一个扇区号码
GetFATEntry:
	push	bx
	push	cx
	push	es

	;读第一个FAT表到内存中
	push	ax
	mov	ax,BaseOfFatEntry
	sub	ax,0100h	;mov	ax,ds
	mov	es,ax
	mov	ax,SectorNoOfFAT1
	mov	cl,1
	mov	bx,0
	call	ReadSector
	pop	ax
	
	;判断ax是偶数还是奇数
	push	ax

	mov	bl,2
	div	bl
	cmp	ah,1
	jz	LABEL_ODD
LABEL_EVEN:	;如果扇区号码为偶数
	pop	ax
	mov	bl,3
	mul	bl
	add	ax,2
	mov	bl,2
	div	bl;此时ax的值为 (3*n+2)/2
	mov	bx,ax
	xor	ax,ax
	mov	ah,[es:bx]
	and	ah,0Fh
	
	sub	bx,1
	mov	al,[es:bx];此时ax中保存了所求数值

	pop	es	
	pop	cx
	pop	bx
	ret
LABEL_ODD:	;如果扇区号码为奇数
	pop	ax
	mov	bl,3
	mul	bl
	sub	ax,1
	mov	bl,2
	div	bl;此时ax的值为(3*n-1)/2
	mov	bx,ax
	xor	ax,ax
	mov	al,[es:bx]

	add	bx,1
	mov	ah,[es:bx];此时ax中保存了所求数值
	
	shr	ax,4
	
	pop	es
	pop	cx
	pop	bx
	ret
;-------------------------------------------------------------------
;MessageLength	equ 9
;LoadMessage:	db "Loading  "
;Message1:	db "KERNELDON"
;Message2:	db "No KERNEL"
MessageLength	equ 19
LoadMessage:	db "Loading Kernel....."
Message1:	db "Load    Kernel done"
Message2:	db "No KERNEL          "
SecSize:	db	0
pos:		db	0
KernelFileName:	db 'KERNEL  BIN',0 ;loader文件的名字



;32位保护模式代码段
[SECTION .s32]
ALIGN 32
[bits 32]
LABEL_PM_START:
	mov	ax,SelectorVideo
	mov	gs,ax
	mov	ax,SelectorFlatRW
	mov	ds,ax
	mov	es,ax
	mov	fs,ax
	mov	ss,ax
	mov	esp,TopOfStack

	call	InitKernel
;	mov	ah,0fh
;	mov	al,'P'
;	mov	[gs:((80*0+39)*2)],ax
	jmp	SelectorFlatC:KernelEntryPointPhyAddr
;在下面的函数中将内存中的kernel.bin elf格式文件加载到指定位置
InitKernel:
	;esi<--program header在文件中的偏移，注意要加上kernel基地址才能取得在内存中的偏移
	mov	esi, dword[ds:BaseOfKernelPhyAddr+1ch]
	add	esi,BaseOfKernelPhyAddr
	;cx<--program header的数目	
	mov	cx,  word[ds:BaseOfKernelPhyAddr+2ch]
.CopyNextProgramSeg:
	mov	eax,[esi+0]
	cmp	eax,0
	jz	.NoAction
	;取得这个program seg的大小-->ax
	mov	eax,[ds:esi+10h]
	push	eax
	;取得在文件中的偏移-->ax，注意要加上kernel基地址才能取得在内存中的偏移
	mov	eax,[ds:esi+04h]
	add	eax,BaseOfKernelPhyAddr
	push	eax
	;取得复制的目标地址（内存虚拟地址）-->ax
	mov	eax,[ds:esi+08h]
	push	eax
	;进行复制
	call	MemCpy	
	add	esp,12
	;变量递增
.NoAction:	
	sub	cx,1
	cmp	cx,0
	jz	.CopyProgramDone
	;bx<--一个prgram header的大小
	xor	ebx,ebx
	mov	bx,word[ds:BaseOfKernelPhyAddr+2ah]
	cmp	ebx,32
	jnz	.Lock
	add	esi,ebx
;	add	esi,020h
	jmp	.CopyNextProgramSeg
.CopyProgramDone:
	;取得kernel程序入口地址
	;mov	ah,0fh
	;mov	al,'C'
	;mov	[gs:((80*1+39)*2)],ax
	ret
.Lock:	
	mov	ah,0fh
	mov	al,'O'
	mov	[gs:((80*2+39)*2)],ax
	jmp	$
%include"lib.inc"
dwDispPos	dd   	(80*6+0)*2
szReturn	db	0ah,0
;段结束

;堆栈段
[section .data1]
ALIGN 32
LABEL_DATA:
StackSpace: times 1024 db 0
TopOfStack	equ	BaseOfLoaderPhyAddr+$
