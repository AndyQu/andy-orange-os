/***********************************************************************
			task_disk.c
***********************************************************************/
#include"type.h"
#include"const.h"
#include"protect_type.h"
#include"global.h"
#include"ipc.h"
#include"assert.h"
#include"8259A.h"
#include"8253_const.h"
#include"hd_const.h"
#include"hd_type.h"
#include"proc.h"
#include"task.h"

/*=============================hd_handler==============================*
hard disk interrupt handler
Ring<0>
======================================================================*/
PUBLIC void hd_handler()
{
	printf_kernel("THis is the hard disk interrupt handler.\n");
	inform_int(TASK_HD);
}

void set_hd_handler(t_pf_int_handler handler);
void hd_cmd_out(struct hd_cmd*cmd);
void disk_int(void);
PRIVATE void print_identify_info(t_16* hdinfo);
PRIVATE void hd_identify(int drive);
/*=============================task_disk==============================*
hard disk driver process
Ring<1>
======================================================================*/
PRIVATE t_16 hdbuf[SECTOR_SIZE];
PUBLIC void task_disk()
{

	t_8* pNrDrives=(t_8*)(0x475);
	printf("NrDrives:%d.\n",*pNrDrives);
	assert(*pNrDrives);
	set_hd_handler(disk_int);
	enable_disk();
	printf("This is disk task\n");

	MESSAGE msg;
	int src;
	while(1){
		sendrec(RECEIVE,ANY,&msg);
		switch(msg.type){
			case DISK_IDENTIFY:
				hd_identify(0);
				break;
			default:
				continue;	
		}	
	}
}
PRIVATE void hd_identify(int drive)
{
	struct hd_cmd cmd;
	cmd.device=MAKE_HD_DEVICE_REG(0,drive,0);
	cmd.command=ATA_IDENTIFY;
	hd_cmd_out(&cmd);

	interrupt_wait();
	port_read(REG_HD_DATA,hdbuf,SECTOR_SIZE);
	print_identify_info(hdbuf);
}

PRIVATE int waitfor(int mask, int val, int timeout);
/*=============================hd_cmd_out==============================*
issue a query command to hard disk.
Ring<0> Ring<1>
@struct hd_cmd*cmd: the parameter struct
======================================================================*/
PUBLIC void hd_cmd_out(struct hd_cmd*cmd)
{
	if(!waitfor(STATUS_BUSY,0,HD_TIMEOUT*10))
		panic("hd error.");

	//Activate the Interrupt Enable bit
	out_byte(REG_HD_DEV_CTRL,0);
	
	out_byte(REG_HD_FEATURES,cmd->features);
	out_byte(REG_HD_NSECTOR,cmd->sectorCount);
	out_byte(REG_HD_LBA_LOW,cmd->lbaLow);
	out_byte(REG_HD_LBA_MID,cmd->lbaMid);
	out_byte(REG_HD_LBA_HIGH,cmd->lbaHigh);
	out_byte(REG_HD_DEVICE,cmd->device);//select device and mode

	//write command code into Command Register
	out_byte(REG_HD_COMMAND,cmd->command);
	
}


/*=============================waitfor==============================*
Ring<0> Ring<1> wait for a certain status of hard disk

@int mask: status mask
@int val:  Required status
@int timeout: Timeout in milliseconds

@return 1 if success, 0 if timeout
======================================================================*/
PRIVATE int waitfor(int mask, int val, int timeout)
{
	int t=get_ticks();
	while(((get_ticks()-t)*1000/HZ)<timeout){
		if( (in_byte(REG_HD_STATUS)&mask)==val )
			return 1;
	}
	return 0;
}

/*=============================print_identify_info==============================*
Ring<1> print out information of disk
@t_16*:disk information
======================================================================*/
PRIVATE void print_identify_info(t_16* hdinfo)
{
int i,k;
char s[64];

struct iden_info_ascii{
	int idx;int len;char*desc;
}iinfo[]={
	{10,20,"HD_SN"},
	{27,40,"HD_Model"}
	};
for(k=0;k<sizeof(iinfo)/sizeof(iinfo[0]);k++){
	int index=iinfo[k].idx;
	int length=iinfo[k].len;
	char*p=(char*)&hdinfo[index];
	for(i=0;i<length/2;i++){
		s[i*2+1]=*p;
		p++;
		s[i*2]=*p;
		p++;
	}
	s[2*i]=0;
	printf("%s: %s  ",iinfo[k].desc,s);
}
int capabilities=hdinfo[49];
printf("LBA supported: %s  ",(capabilities&0x0200)?"Yes":"No");
int cmd_set_supported=hdinfo[83];
printf("LBA48 supported: %s  ",(cmd_set_supported&0x0400)?"Yes":"No");
int sectors=((int)hdinfo[61]<<16)+hdinfo[60];
printf("Sector size: %d  ",sectors);
printf("HD size: %dMB  ",(sectors/1024*SECTOR_SIZE)/1024);
}
