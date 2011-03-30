
#define	DA_32	0x4000
#define	DA_LIMIT_4K	0x8000	

#define	DA_DPL0	 0x00	
#define	DA_DPL1	0x20	
#define	DA_DPL2	0x40
#define	DA_DPL3	0x60


#define	DA_DR	0x90	
#define	DA_DRW	0x92	
#define	DA_DRWA	0x93
#define	DA_C	0x98
#define	DA_CR	0x9A
#define	DA_CCO	0x9C
#define	DA_CCOR	0x9E

#define	DA_LDT		0x82	
#define	DA_TaskGate  0x85	
#define	DA_386TSS  0x89	
#define	DA_386CGate  0x8C
#define	DA_386IGate  0x8E
#define	DA_386TGate  0x8F

#define	SA_RPL0			0	//; ┓
#define	SA_RPL1			1	//; ┣ RPL
#define	SA_RPL2			2	//; ┃
#define	SA_RPL3			3	//; ┛

#define	SA_TIG			0	//; ┓TI
#define	SA_TIL			4	//; ┛



#define	PG_P			1	//; 页存在属性位
#define	PG_RWR			0	//; R/W 属性位值, 读/执行
#define	PG_RWW			2	//; R/W 属性位值, 读/写/执行
#define	PG_USS			0	//; U/S 属性位值, 系统级
#define	PG_USU			4	//; U/S 属性位值, 用户级
