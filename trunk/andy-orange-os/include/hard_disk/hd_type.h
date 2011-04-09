#ifndef __HD_TYPE_H__
#define __HD_TYPE_H__
#include"type.h"
#include"hd_const.h"
struct hd_cmd{
	t_8 data;
	t_8 features;
	t_8 sectorCount;
	t_8 lbaLow;
	t_8 lbaMid;
	t_8 lbaHigh;
	t_8 device;
	t_8 command;
};

struct part_info{
	t_bool bootable;
	t_16 type;
	t_32 base;
	t_32 size;
};

struct hd_info{
	struct part_info primary[NR_PRIM_PER_DRIVE];
	t_32 ext_base;
	t_32 ext_size;
	t_bool ext_valid;
	struct part_info logical[NR_SUB_PER_DRIVE];
};
#endif
