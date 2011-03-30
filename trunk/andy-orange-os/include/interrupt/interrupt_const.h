#ifndef _TINIX_INTERRUPT_CONST_H_
#define	_TINIX_INTERRUPT_CONST_H_

/*8259A硬件中断向量，从0x20开始*/
#define INT_VECTOR_IRQ0	0x20/*时钟中断*/
#define INT_VECTOR_IRQ1	0x21/*键盘中断*/

#define INT_VECTOR_IRQ2	0x22/*从8259中断*/

#define INT_VECTOR_IRQ3	0x23/*串口2中断*/
#define INT_VECTOR_IRQ4	0x24/*串口1中断*/
#define INT_VECTOR_IRQ5	0x25/*LPT2中断*/
#define INT_VECTOR_IRQ6	0x26/*软盘中断*/
#define INT_VECTOR_IRQ7	0x27/*LPT1中断*/

#define INT_VECTOR_IRQ8	0x28/*实时时钟*/
#define INT_VECTOR_IRQ9	0x29/*重定向IRQ2*/
#define INT_VECTOR_IRQA	0x2A/*保留*/
#define INT_VECTOR_IRQB	0x2B/*保留*/
#define INT_VECTOR_IRQC	0x2C/*PS/2鼠标*/
#define INT_VECTOR_IRQD	0x2D/*FPU异常*/
#define INT_VECTOR_IRQE	0x2E/*AT温盘*/
#define INT_VECTOR_IRQF	0x2F/*保留*/

#define INT_VECTOR_DISK INT_VECTOR_IRQE
#define INT_VECTOR_SLAVE_8259	INT_VECTOR_IRQ2

/*中断门的属性定义*/
#define DA_386IGate             0x8E    

/*中断向量号的定义*/
#define INT_VECTOR_DIVIDE               0x0
#define INT_VECTOR_DEBUG                0x1
#define INT_VECTOR_NMI                  0x2
#define INT_VECTOR_BREAKPOINT           0x3
#define INT_VECTOR_OVERFLOW             0x4
#define INT_VECTOR_BOUNDS               0x5
#define INT_VECTOR_INVAL_OP             0x6
#define INT_VECTOR_COPROC_NOT           0x7
#define INT_VECTOR_DOUBLE_FAULT         0x8
#define INT_VECTOR_COPROC_SEG           0x9
#define INT_VECTOR_INVAL_TSS            0xA
#define INT_VECTOR_SEG_NOT              0xB
#define INT_VECTOR_STACK_FAULT          0xC
#define INT_VECTOR_PROTECTION           0xD
#define INT_VECTOR_PAGE_FAULT           0xE
/*0x0f号中断Intel保留，未使用*/
#define INT_VECTOR_COPROC_ERR		0x10
#define INT_VECTOR_ALIGN_ERR		0x11
#define INT_VECTOR_MACHINE_ERR		0x12
#define INT_VECTOR_SIMD_ERR		0x13
/*Mini OS的系统调用中断*/
#define	INT_VECTOR_SYS_CALL		0x90
/*Mini OS系统调用函数的数量*/
#define	NR_SYS_CALL			0x10
#define	__NR_get_ticks			0x00
#define	__NR_printx			0x01/*changed from write to printx*/

/*added chapter8/a*/
#define __NR_sendrec			0x02
/*added chapter8/a end*/

#endif
