%ifdef _BOOT_DEBUG_
	org	0100h
%else	
	org	07c00h
%endif

jmp	short LABEL_START
nop
%include "fat12hdr.inc"
%include "load.inc"
%ifdef _BOOT_DEBUG_
	BaseOfStack equ	0100h
%else	
	BaseOfStack equ 07c00h
%endif

OffsetOfDir		equ	08000h;根目录扇区被加载的位置－－－偏移地址
;引导区代码起始处--------------------------------------------
LABEL_START:
	;此时CS的数值为0x0
	mov	ax,cs
	mov	ds,ax
	mov	es,ax
	mov	ss,ax
	mov	sp,BaseOfStack
	;清屏
	mov	ax,0600h
	mov	bx,0700h
	mov	cx,0
	mov	dx,0184fh
	int	10h

	mov	dh,0
	call	DispStr;显示"booting"字符串
	;软驱复位
	xor	ah,ah
	xor	dl,dl
	int	13h
;下面在A盘的根目录中寻找loader.com----------------------------
	;读根目录的第一个扇区，到bx指向的内存处
	;es=0，bx=OffsetOfDir=0x8000h，所以根目录被加载到内存0x0000:8000处即0x08000h处
	mov	bx,OffsetOfDir
	mov	ax,SectorNoOfRootDirectory
	mov	cl,2
	call	ReadSector
;在根目录中寻找loader.com文件
	mov	bx,OffsetOfDir
	sub	bx,32
	mov	dx,0
.NextItem:
	add	bx,32
	cmp	dx,32
	ja	.CantFindLoader
	inc	dx
	mov	si,bx
	mov	di,LoaderFileName
	mov	cx,11
	.loop:
	lodsb
	cmp	al,[di]
	jnz	.NextItem
	inc	di
	loop	.loop
	;如每个字符都匹配，则表明成功找到loader.com的目录项,显示"L"
	;mov     ax,0b800h
        ;mov     gs,ax
        ;mov     ah,0ch
        ;mov     al,'L'
        ;mov     [gs:((80*0+60)*2)],ax
;将loader.com复制到内存中去
.Loading:
	;bx此时保存了loader目录项在内存中的偏移
	mov	ax,BaseOfLoader
	mov	es,ax
	add	bx,0x1A
	mov	ax,[ds:bx];ax此时保存了loader的第一个扇区号码
	
	;es=BaseOfLoader=08000h,bx=OffsetOfLoader=0100h，所以loader.com文件被加载于0x8000:0100即0x80100处
	;实模式下，从0xA0000处开始的内存被系统使用
	mov	cl,1
	mov	bx,OffsetOfLoader
	.NextOne:
	;	push    ax
        ;        push    bx
        ;        mov     ah,0eh
        ;        add	al,'0'
	;	mov     bl,0fh
        ;        int     10h
        ;        pop     bx
        ;        pop     ax
	push	ax
	add	ax,RootDirSectors
	add	ax,17
	call	ReadSector
	;	push	ax
	;	push	bx
	;	mov	ah,0eh
	;	mov	al,'L'
	;	mov	bl,0fh
	;	int	10h
	;	pop	bx
	;	pop	ax
	pop	ax
	call	GetFATEntry
	add	bx,[BPB_BytsPerSec]
	cmp	ax,0xff8
	jb	.NextOne
.Loaded:
	mov	dh,1
	call	DispStr
	;跳转到loader.com处，使得cs的数值变为BaseOfLoader=0x8000
	jmp	BaseOfLoader:OffsetOfLoader
.CantFindLoader:
	mov	ax,4c00h
	int	21h
;-------------------------------------------------------------
;DispStr 
;输入:dh为message的序号  输出:在屏幕上显示字符串
DispStr:
	push	ax
	push	bp
	push	bx
	push	es
	push	cx
	push	dx

	mov	ax,MessageLength
	mul	dh
	add	ax,BootMessage
	mov	bp,ax
	mov	ax,ds
	mov	es,ax
	mov	cx,MessageLength
	mov	ax,01301h
	mov	bx,0007h
	mov	dl,0
;	add	dh,3
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
	mov	ax,BaseOfLoader
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
;BootMessage:	db "Booting  "
;Message1:	db "load   OK"
;Message2:	db "No LOADER"
MessageLength	equ 21
BootMessage:	db "Booting.............."
Message1:	db "Load Kernel Loader..."
Message2:	db "No LOADER............"
SecSize:	db	0
LoaderFileName:	db 'LOADER  COM',0 ;loader文件的名字
times	510-($-$$)	db 0
dw	0xaa55

