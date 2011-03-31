[section .data]
global	disp_pos
disp_pos	dd	0
[section .text]
;导出函数
global	disp_str
global	disp_color_str
global	out_byte
global	in_byte
global	memcpy
global	disable_int
global	enable_int
;---------------------------------------------
;void disable_int();
disable_int:
	cli
	ret
;---------------------------------------------
;void enable_int();
enable_int:
	sti
	ret
;---------------------------------------------
disp_str:
	push	ebp
	mov	ebp,esp
	
	push	esi
	push	edi
	push	eax
	push	ebx

	mov	esi,[ebp+8]	;字符串的指针
	mov	edi,[disp_pos]
	mov	ah,0fh
	cld
.1:
	lodsb
	test	al,al
	jz	.2


	cmp	al,0x7a
	jnz	.4
;	inc	dword[gs:20]
.4:
	cmp	al,0ah
	jnz	.3
	push	eax
	mov	eax,edi
	mov	bl,160
	div	bl
	and	eax,0ffh
	inc	eax
	mov	bl,160
	mul	bl
	mov	edi,eax
	pop	eax
	jmp	.1
.3:
	mov	[gs:edi],ax
	add	edi,2
	jmp	.1
.2:
	mov	[disp_pos],edi

	pop	ebx
	pop	eax
	pop	edi
	pop	esi

        mov     esp, ebp
	pop	ebp
	
	ret
;---------------------------------------------
;out_byte(t_port port,t_8 data)
out_byte:
	push	ebp
	mov	ebp,esp
	
	mov	edx,[ebp+8]
	mov	al,[ebp+8+4]
	out	dx,al
	nop
	nop
	mov	esp,ebp
	pop	ebp
	ret
;---------------------------------------------
;in_byte(t_port port)
in_byte:
	push	ebp
	mov	ebp,esp
	
	mov	edx,[ebp+8]
	xor	eax,eax	
	;mov	al,[ebp+8+4]
	in	al,dx
	nop
	nop
	mov	esp,ebp
	pop	ebp
	ret
;---------------------------------------------
;---------------port_read(t_port port, t_16*buf, int count)--------------
global port_read
port_read:
	mov edx,[esp+4]
	mov edi,[esp+4*2]
	mov ecx,[esp+4*3]
	shr ecx,1
	cld
	rep insw
	ret
; ------------------------------------------------------------------------

; void* memcpy(void* es:pDest, void* ds:pSrc, int iSize);
;参数都依次被压入在堆栈中
;使用ax作为返回数值：目标地址偏移
; ------------------------------------------------------------------------

memcpy:
        push    ebp
        mov     ebp, esp; 取得此时堆栈指针
        push    esi
        push    edi
        push    ecx
        push    eax

        mov     edi, [ebp + 8]  ; Destination
        mov     esi, [ebp + 12] ; Source
        mov     ecx, [ebp + 16] ; Counter
.loop:
        mov     al,[ds:esi]
        mov     [es:edi],al
        inc     esi
        inc     edi
        loop    .loop

        pop     eax
        pop     ecx
        pop     edi
        pop     esi
        mov     esp, ebp
        pop     ebp

        ret                     ; 函数结束，返回

; memcpy 结束-------------------------------------------------------------
disp_color_str:
	push	ebp
	mov	ebp,esp
	
	mov	esi,[ebp+8]	;字符串的指针
	mov	edi,[disp_pos]
	;mov	ah,0fh
	mov	ah,[ebp+12]
.1:
	lodsb
	test	al,al
	jz	.2
	cmp	al,0ah
	jnz	.3
	push	eax
	mov	eax,edi
	mov	bl,160
	div	bl
	and	eax,0ffh
	inc	eax
	mov	bl,160
	mul	bl
	mov	edi,eax
	pop	eax
	jmp	.1
.3:
	mov	[gs:edi],ax
	add	edi,2
	jmp	.1
.2:
	cmp	edi,4000
	jb	.4
	mov	edi,2
.4:
	mov	[disp_pos],edi
        mov     esp, ebp
	pop	ebp
	
	ret
;---------------------------------------------
