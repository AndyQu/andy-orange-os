#ifndef __HD_CONST_H__
#define __HD_CONST_H__

/*MAKE_HD_DEVICE_REG
make the device parameter for "Device" of Command Block Registers
*/
#define MAKE_HD_DEVICE_REG(lba,drive,lba_highest) (\
						  ((lba)<<6)|	\
						  ((drive)<<4)|	\
						  (lba_highest&0xf)| \
						  0xA0)

/*Following Macros are used for "Command" of Command Block Registers*/
#define ATA_IDENTIFY 0xEC

/*Following Macros are of hard disk I/O ports used to write into */
#define REG_HD_DEV_CTRL		0x03f6	//Control Block Register
#define REG_HD_DATA		0x01f0	//Data Block Register
#define REG_HD_FEATURES		0x01f1	//Feature Command Block Register
#define REG_HD_NSECTOR		0x01f2	//Sector Count Command Block Register
#define REG_HD_LBA_LOW		0x01f3	//LBA Low Command Block Register
#define REG_HD_LBA_MID		0x01f4	//LBA Mid Command Block Register
#define REG_HD_LBA_HIGH		0x01f5	//LBA High Command Block Register
#define REG_HD_DEVICE		0x01f6	//Device Command BLock Register
#define REG_HD_COMMAND		0x01f7	//Command Command Block Register
/*Following Macros are of hard disk I/O ports used to read*/
#define REG_HD_ERROR		0x01f1
#define	REG_HD_STATUS		0x01f7
#define REG_HD_ALT_STATUS	0x03f6

/*This Macro definition is used as a mask to get status of "status" command block register*/
#define STATUS_BUSY	0x80

#define HD_TIMEOUT	1000

#define SECTOR_SIZE 512
#endif
