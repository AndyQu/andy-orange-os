;address 80100h,80000h=8*2^16=8*64K=512K
BaseOfLoader            equ     08000h;loader.com被加载的位置－－－段地址
OffsetOfLoader          equ     0100h;loader.com被加载的位置－－－偏移地址

BaseOfLoaderPhyAddr	equ	BaseOfLoader*10h ;loader.com文件被加载的段的基地址

;address 90000h,90000h=9*64K=576K
BaseOfKernelFile        equ     09000h;kernel.bin被加载的位置－－－段地址
OffsetOfKernelFile      equ     0h;kernel.bin被加载的位置－－－偏移地址

BaseOfKernelPhyAddr	equ	BaseOfKernelFile*10h ;kernel.bin文件被加载的段的基地址，由于偏移地址是0，所以这也是kernel.bin文件的物理基地址

KernelEntryPointPhyAddr	equ	030500h

