#include"const.h"
#include"type.h"
#include"ipc.h"
#include"task.h"
void task_fs(void)
{
printf("This is the file system task\n");
MESSAGE msg;
msg.type=DISK_IDENTIFY;
sendrec(SEND,TASK_HD,&msg);
spin("spin in FS...\n");
}
