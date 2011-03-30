#Makefile for boot

#Entry Point for my os
#It must be the same as 'KernelEntryPointPhyAddr' in load.inc
ENTRY_POINT=0x030500

#Offset of entry point in kernel file
#It depends on Entry Point
ENTRY_OFFSET=0x400

#program,flags,etc.
ASM=nasm
DASM=ndisasm
CC=gcc
#CC=g++
LD=ld
ASM_BOOT_FLAGS=-I ./boot/include/
ASM_KERNEL_FLAGS=-I include/ -f elf32
CFLAGS=-I include -I kernel/include -c -fno-builtin -m32 -fno-stack-protector -Wno-deprecated
#-Wno-deprecated added in chapter8/a
#LDFLAGS=-Ttext $(ENTRY_POINT) --oformat elf32-i386 #-s
LDFLAGS=-Ttext $(ENTRY_POINT) -m elf_i386 #-s
DASMFLAGS=-u -o $(ENTRY_POINT) -e $(ENTRY_OFFSET)

#This program
TINIX_BOOT=boot/boot.img boot/loader.com boot/boot.com
TINIX_KERNEL=kernel/kernel.bin

#All Phony Targets
.PHONY:everything all clean disasm build_iso build_img

#Default starting position
everything: $(TINIX_KERNEL) $(TINIX_BOOT) build_img


###########################################################################
build_img:
	mount	-o loop	boot/boot.img	/mnt/floppy
	cp -f boot/loader.com	/mnt/floppy
	cp -f $(TINIX_KERNEL) /mnt/floppy
	umount	/mnt/floppy	
build_iso:
	mkisofs -o cd.iso boot/loader.com boot/boot.com $(TINIX_KERNEL)	
dasmInfo:
	mkdir -p kernel/info/
	ndisasm -u -o $(ENTRY_POINT) -e $(ENTRY_OFFSET) $(TINIX_KERNEL) >kernel/info/kernel.bin.asm
	readelf	-a -S $(TINIX_KERNEL) >kernel/info/kernel.elf
	ndisasm -u kernel/init.o >kernel/info/init.asm
#生成目标软盘
boot/boot.img: boot/boot.bin
	dd if=/dev/zero of=emptydisk.img bs=512 count=2880 #生成空白软盘镜像文件
	dd if=boot/boot.bin of=boot/boot.img bs=512 count=1 #用 bin file 生成对应的镜像文件
	dd if=emptydisk.img of=boot/boot.img skip=1 seek=1 bs=512 count=2879 #在 bin 生成的镜像文件后补上空白，最后成为合适大小的软盘镜像
	rm -rf emptydisk.img
	sudo chmod 666 boot/boot.img
boot/boot.bin:./boot/boot.asm ./boot/include/load.inc ./boot/include/fat12hdr.inc
	$(ASM) $(ASM_BOOT_FLAGS) -o $@ $<
boot/loader.com:boot/loader.asm boot/include/load.inc boot/include/fat12hdr.inc boot/include/pm.inc boot/include/lib.inc
	$(ASM) $(ASM_BOOT_FLAGS) -o $@ $<
boot/boot.com:./boot/boot.asm ./boot/include/load.inc ./boot/include/fat12hdr.inc
	$(ASM) $(ASM_BOOT_FLAGS) -D_BOOT_DEBUG_ -o $@ $<
###########################################################################


OBJS=./kernel/kernel.o ./kernel/tss.o ./kernel/myprintf.o ./kernel/ipc.o ./kernel/proc_init.o ./kernel/schedule.o ./kernel/set_idt.o ./kernel/sys_call_shell.o ./kernel/exception.o ./kernel/global_const.o ./kernel/i8259.o ./kernel/proc_util.o ./kernel/printf.o ./kernel/kernel_printf.o ./kernel/init.o ./kernel/interrupt.o ./kernel/global.o ./kernel/sys_call_kernel.o ./lib/klib.o ./lib/misc.o ./lib/klibC.o ./sys_task/task.o ./sys_task/interface_get_ticks.o ./sys_task//disk/task_disk.o ./sys_task//tick/task_tick.o ./sys_task//tty/tty.o ./sys_task//tty/task_tty.o ./sys_task//tty/key.o ./sys_task//tty/keymap.o ./application//test/normal_procs.o 
CFLAGS=-c -fno-builtin -m32 -fno-stack-protector -Wno-deprecated  -I ./kernel/./include/ -I ./include/ -I ./include//assert/ -I ./include//func/ -I ./include//8259A/ -I ./include//8253/ -I ./include//keyboard/ -I ./include//ipc/ -I ./include//task/ -I ./include//protect/ -I ./include//hard_disk/ -I ./include//interrupt/ -I ./include//proc/ -I ./include//video/ -I ./include//type/ -I ./include//console/ -I ./include//const/


###########################################################################

renew:	clean everything
clean:
	rm -f $(OBJS) $(TINIX_BOOT) $(TINIX_KERNEL) $(DASM_OUTPUT) boot/boot.bin
###########################################################################


$(TINIX_KERNEL):$(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
	objdump -D $(TINIX_KERNEL) > dump
./kernel/tss.o: kernel/tss.c kernel/./include/global.h \
  include//protect/protect_type.h include//type/type.h \
  include//const/const.h include//protect/protect_const.h \
  include//protect/pm.h include//proc/proc_const.h \
  include//interrupt/interrupt_const.h include//proc/proc_type.h \
  include//type/type.h include//proc/proc_const.h include//ipc/ipc.h \
  include//ipc/ipc_const.h include//ipc/ipc_type.h \
  include//ipc/ipc_func.h kernel/./include/global_const.h
	$(CC) $(CFLAGS) -o $@ $<
./kernel/ipc.o: kernel/ipc.c include//const/const.h include//type/type.h \
  include//protect/protect_type.h include//type/type.h \
  include//const/const.h include//protect/protect_const.h \
  include//protect/pm.h include//proc/proc_const.h \
  kernel/./include/global.h include//protect/protect_type.h \
  include//interrupt/interrupt_const.h include//proc/proc_type.h \
  include//type/type.h include//proc/proc_const.h include//ipc/ipc.h \
  include//ipc/ipc_const.h include//ipc/ipc_type.h \
  include//ipc/ipc_func.h include//func/proto.h include//type/type.h \
  include//const/const.h include//assert/assert.h include//proc/proc.h \
  include//proc/proc_type.h include//proc/proc_func.h \
  include//const/const.h
	$(CC) $(CFLAGS) -o $@ $<
./kernel/proc_init.o: kernel/proc_init.c include//type/type.h \
  include//const/const.h include//protect/protect_type.h \
  include//type/type.h include//const/const.h \
  include//protect/protect_const.h include//protect/pm.h \
  include//proc/proc_const.h kernel/./include/global.h \
  include//protect/protect_type.h include//interrupt/interrupt_const.h \
  include//proc/proc_type.h include//type/type.h \
  include//proc/proc_const.h include//ipc/ipc.h include//ipc/ipc_const.h \
  include//ipc/ipc_type.h include//ipc/ipc_func.h \
  kernel/./include/printf.h include//const/const.h
	$(CC) $(CFLAGS) -o $@ $<
./kernel/schedule.o: kernel/schedule.c include//const/const.h include//type/type.h \
  kernel/./include/global.h include//protect/protect_type.h \
  include//type/type.h include//const/const.h \
  include//protect/protect_const.h include//protect/pm.h \
  include//proc/proc_const.h include//interrupt/interrupt_const.h \
  include//proc/proc_type.h include//type/type.h \
  include//proc/proc_const.h include//ipc/ipc.h include//ipc/ipc_const.h \
  include//ipc/ipc_type.h include//ipc/ipc_func.h
	$(CC) $(CFLAGS) -o $@ $<
./kernel/set_idt.o: kernel/set_idt.c include//type/type.h include//const/const.h \
  include//protect/protect_type.h include//type/type.h \
  include//const/const.h include//protect/protect_const.h \
  include//protect/pm.h include//proc/proc_const.h \
  kernel/./include/global.h include//protect/protect_type.h \
  include//interrupt/interrupt_const.h include//proc/proc_type.h \
  include//type/type.h include//proc/proc_const.h include//ipc/ipc.h \
  include//ipc/ipc_const.h include//ipc/ipc_type.h \
  include//ipc/ipc_func.h include//keyboard/keyboard_type.h \
  include//keyboard/keyboard_const.h include//func/proto.h \
  include//type/type.h include//const/const.h include//proc/proc.h \
  include//proc/proc_type.h include//proc/proc_func.h \
  include//const/const.h
	$(CC) $(CFLAGS) -o $@ $<
./kernel/global_const.o: kernel/global_const.c include//type/type.h \
  include//const/const.h include//protect/protect_type.h \
  include//type/type.h include//const/const.h \
  include//protect/protect_const.h include//protect/pm.h \
  include//proc/proc_const.h kernel/./include/global.h \
  include//protect/protect_type.h include//interrupt/interrupt_const.h \
  include//proc/proc_type.h include//type/type.h \
  include//proc/proc_const.h include//ipc/ipc.h include//ipc/ipc_const.h \
  include//ipc/ipc_type.h include//ipc/ipc_func.h
	$(CC) $(CFLAGS) -o $@ $<
./kernel/i8259.o: kernel/i8259.c include//type/type.h include//const/const.h \
  include//protect/protect_type.h include//type/type.h \
  include//const/const.h include//protect/protect_const.h \
  include//protect/pm.h include//proc/proc_const.h \
  include//interrupt/interrupt_const.h include//func/proto.h \
  include//type/type.h include//const/const.h \
  include//8259A/8259A_const.h
	$(CC) $(CFLAGS) -o $@ $<
./kernel/proc_util.o: kernel/proc_util.c include//const/const.h \
  include//type/type.h include//protect/protect_type.h \
  include//type/type.h include//const/const.h \
  include//protect/protect_const.h include//protect/pm.h \
  include//proc/proc_const.h kernel/./include/global.h \
  include//protect/protect_type.h include//interrupt/interrupt_const.h \
  include//proc/proc_type.h include//type/type.h \
  include//proc/proc_const.h include//ipc/ipc.h include//ipc/ipc_const.h \
  include//ipc/ipc_type.h include//ipc/ipc_func.h include//func/proto.h \
  include//type/type.h include//const/const.h include//assert/assert.h \
  include//ipc/ipc.h
	$(CC) $(CFLAGS) -o $@ $<
./kernel/printf.o: kernel/printf.c include//const/const.h include//type/type.h \
  include//func/proto.h include//type/type.h include//const/const.h \
  kernel/./include/sys_call_shell.h include//const/const.h \
  include//ipc/ipc.h include//ipc/ipc_const.h include//ipc/ipc_type.h \
  include//ipc/ipc_func.h
	$(CC) $(CFLAGS) -o $@ $<
./kernel/kernel_printf.o: kernel/kernel_printf.c include//const/const.h \
  include//type/type.h include//func/proto.h include//type/type.h \
  include//const/const.h kernel/./include/sys_call_shell.h \
  include//const/const.h include//ipc/ipc.h include//ipc/ipc_const.h \
  include//ipc/ipc_type.h include//ipc/ipc_func.h
	$(CC) $(CFLAGS) -o $@ $<
./kernel/init.o: kernel/init.c include//func/proto.h include//type/type.h \
  include//const/const.h kernel/./include/global.h \
  include//protect/protect_type.h include//type/type.h \
  include//const/const.h include//protect/protect_const.h \
  include//protect/pm.h include//proc/proc_const.h \
  include//interrupt/interrupt_const.h include//proc/proc_type.h \
  include//type/type.h include//proc/proc_const.h include//ipc/ipc.h \
  include//ipc/ipc_const.h include//ipc/ipc_type.h \
  include//ipc/ipc_func.h kernel/./include/global_const.h \
  include//protect/protect_const.h include//8253/8253_const.h
	$(CC) $(CFLAGS) -o $@ $<
./kernel/global.o: kernel/global.c include//type/type.h include//const/const.h \
  include//protect/protect_type.h include//type/type.h \
  include//const/const.h include//protect/protect_const.h \
  include//protect/pm.h include//proc/proc_const.h \
  kernel/./include/global.h include//protect/protect_type.h \
  include//interrupt/interrupt_const.h include//proc/proc_type.h \
  include//type/type.h include//proc/proc_const.h include//ipc/ipc.h \
  include//ipc/ipc_const.h include//ipc/ipc_type.h \
  include//ipc/ipc_func.h
	$(CC) $(CFLAGS) -o $@ $<
./kernel/sys_call_kernel.o: kernel/sys_call_kernel.c include//const/const.h \
  include//type/type.h include//protect/protect_type.h \
  include//type/type.h include//const/const.h \
  include//protect/protect_const.h include//protect/pm.h \
  include//proc/proc_const.h kernel/./include/global.h \
  include//protect/protect_type.h include//interrupt/interrupt_const.h \
  include//proc/proc_type.h include//type/type.h \
  include//proc/proc_const.h include//ipc/ipc.h include//ipc/ipc_const.h \
  include//ipc/ipc_type.h include//ipc/ipc_func.h \
  include//keyboard/keyboard_type.h include//keyboard/keyboard_const.h \
  include//ipc/ipc.h include//proc/proc.h include//proc/proc_type.h \
  include//proc/proc_func.h include//const/const.h \
  include//console/console_const.h kernel/./include/key.h \
  include//func/proto.h include//type/type.h include//const/const.h \
  include//video/video_const.h
	$(CC) $(CFLAGS) -o $@ $<
./lib/misc.o: lib/misc.c include//type/type.h include//const/const.h \
  kernel/./include/printf.h include//const/const.h
	$(CC) $(CFLAGS) -o $@ $<
./lib/klibC.o: lib/klibC.c include//type/type.h include//const/const.h
	$(CC) $(CFLAGS) -o $@ $<
./sys_task/task.o: sys_task/task.c include//type/type.h include//const/const.h \
  kernel/./include/printf.h include//const/const.h
	$(CC) $(CFLAGS) -o $@ $<
./sys_task/interface_get_ticks.o: sys_task/interface_get_ticks.c \
  include//type/type.h include//const/const.h include//ipc/ipc.h \
  include//ipc/ipc_const.h include//ipc/ipc_type.h \
  include//ipc/ipc_func.h
	$(CC) $(CFLAGS) -o $@ $<
./sys_task//disk/task_disk.o: sys_task//disk/task_disk.c include//type/type.h \
  include//const/const.h include//protect/protect_type.h \
  include//type/type.h include//const/const.h \
  include//protect/protect_const.h include//protect/pm.h \
  include//proc/proc_const.h kernel/./include/global.h \
  include//protect/protect_type.h include//interrupt/interrupt_const.h \
  include//proc/proc_type.h include//type/type.h \
  include//proc/proc_const.h include//ipc/ipc.h include//ipc/ipc_const.h \
  include//ipc/ipc_type.h include//ipc/ipc_func.h include//ipc/ipc.h \
  include//assert/assert.h include//8259A/8259A.h \
  include//8259A/8259A_const.h include//8259A/8259A_func.h \
  include//8253/8253_const.h include//hard_disk/hd_const.h \
  include//hard_disk/hd_type.h include//type/type.h include//proc/proc.h \
  include//proc/proc_type.h include//proc/proc_func.h \
  include//const/const.h include//task/task.h
	$(CC) $(CFLAGS) -o $@ $<
./sys_task//tick/task_tick.o: sys_task//tick/task_tick.c include//type/type.h \
  include//const/const.h include//protect/protect_type.h \
  include//type/type.h include//const/const.h \
  include//protect/protect_const.h include//protect/pm.h \
  include//proc/proc_const.h kernel/./include/global.h \
  include//protect/protect_type.h include//interrupt/interrupt_const.h \
  include//proc/proc_type.h include//type/type.h \
  include//proc/proc_const.h include//ipc/ipc.h include//ipc/ipc_const.h \
  include//ipc/ipc_type.h include//ipc/ipc_func.h include//ipc/ipc_func.h \
  include//ipc/ipc_const.h include//assert/assert.h
	$(CC) $(CFLAGS) -o $@ $<
./sys_task//tty/tty.o: sys_task//tty/tty.c include//const/const.h include//type/type.h \
  include//keyboard/keyboard_type.h include//keyboard/keyboard_const.h \
  include//func/proto.h include//type/type.h include//const/const.h \
  kernel/./include/key.h include//assert/assert.h
	$(CC) $(CFLAGS) -o $@ $<
./sys_task//tty/task_tty.o: sys_task//tty/task_tty.c include//type/type.h \
  include//const/const.h include//protect/protect_type.h \
  include//type/type.h include//const/const.h \
  include//protect/protect_const.h include//protect/pm.h \
  include//proc/proc_const.h kernel/./include/global.h \
  include//protect/protect_type.h include//interrupt/interrupt_const.h \
  include//proc/proc_type.h include//type/type.h \
  include//proc/proc_const.h include//ipc/ipc.h include//ipc/ipc_const.h \
  include//ipc/ipc_type.h include//ipc/ipc_func.h include//ipc/ipc_func.h \
  include//keyboard/keyboard_type.h include//keyboard/keyboard_const.h \
  kernel/./include/key.h
	$(CC) $(CFLAGS) -o $@ $<
./sys_task//tty/key.o: sys_task//tty/key.c include//type/type.h include//const/const.h \
  include//keyboard/keyboard_type.h include//keyboard/keyboard_const.h \
  include//keyboard/keymap.h include//func/proto.h include//type/type.h \
  include//const/const.h include//assert/assert.h \
  include//video/video_const.h
	$(CC) $(CFLAGS) -o $@ $<
./sys_task//tty/keymap.o: sys_task//tty/keymap.c include//const/const.h \
  include//type/type.h include//keyboard/keyboard_type.h \
  include//keyboard/keyboard_const.h
	$(CC) $(CFLAGS) -o $@ $<
./application//test/normal_procs.o: application//test/normal_procs.c include//type/type.h \
  include//const/const.h include//protect/protect_type.h \
  include//type/type.h include//const/const.h \
  include//protect/protect_const.h include//protect/pm.h \
  include//proc/proc_const.h kernel/./include/global.h \
  include//protect/protect_type.h include//interrupt/interrupt_const.h \
  include//proc/proc_type.h include//type/type.h \
  include//proc/proc_const.h include//ipc/ipc.h include//ipc/ipc_const.h \
  include//ipc/ipc_type.h include//ipc/ipc_func.h \
  kernel/./include/printf.h include//const/const.h
	$(CC) $(CFLAGS) -o $@ $<
./kernel/myprintf.o: ./kernel/myprintf.asm
	$(ASM) $(ASM_KERNEL_FLAGS) -o $@ $<
./kernel/sys_call_shell.o: ./kernel/sys_call_shell.asm
	$(ASM) $(ASM_KERNEL_FLAGS) -o $@ $<
./kernel/exception.o: ./kernel/exception.asm
	$(ASM) $(ASM_KERNEL_FLAGS) -o $@ $<
./kernel/kernel.o: ./kernel/kernel.asm include/pm.inc
	$(ASM) $(ASM_KERNEL_FLAGS) -o $@ $<
./kernel/interrupt.o: ./kernel/interrupt.asm
	$(ASM) $(ASM_KERNEL_FLAGS) -o $@ $<
./lib/klib.o: ./lib/klib.asm
	$(ASM) $(ASM_KERNEL_FLAGS) -o $@ $<
