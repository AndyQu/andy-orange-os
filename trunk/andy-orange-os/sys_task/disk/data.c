#include"type.h"
#include"const.h"
#include"hd_type.h"

PUBLIC void disk2mem(int drive,int sect_nr,int cnt, t_16* mem)
{
	struct hd_cmd cmd;
	cmd.features=0;
	cmd.sectorCount=cnt;
	cmd.lbaLow=sect_nr&0xff;
	cmd.lbaMid=(sect_nr>>8)&0xff;
	cmd.lbaHigh=(sect_nr>>16)&0xff;
	cmd.device= MAKE_HD_DEVICE_REG(1,drive,(sect_nr>>24)&0xf);
	cmd.command=ATA_READ;

	hd_cmd_out(&cmd);
	interrupt_wait();
	port_read(REG_HD_DATA,mem,SECTOR_SIZE*cnt);

}
