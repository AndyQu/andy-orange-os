#ifndef __HD_TYPE_H__
#define __HD_TYPE_H__
#include"type.h"

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
#endif
