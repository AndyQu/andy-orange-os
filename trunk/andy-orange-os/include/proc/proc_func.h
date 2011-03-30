#ifndef _PROC_FUNC_H_
#define _PROC_FUNC_H_
#include "const.h"
PUBLIC	void* va2la(int pid,void*a);
PUBLIC  int     proc2pid(PROCESS_TABLE*p);
PUBLIC  PROCESS_TABLE* pid2proc(int pid);
PUBLIC  void unblock(PROCESS_TABLE*p);
PUBLIC  void block(PROCESS_TABLE*p);
PUBLIC void interrupt_wait(void);

#endif
