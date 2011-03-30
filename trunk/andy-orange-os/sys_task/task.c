#include"type.h"
#include"const.h"
#include"printf.h"


void task_disk(void);
void task_tick(void);
void task_tty(void);
PUBLIC void init_tasks(void)
{
	add_system_task((int)task_tty);
	add_system_task((int)task_tick);
	add_system_task((int)task_disk);
}
