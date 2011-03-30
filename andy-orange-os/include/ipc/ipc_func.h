/*This file is added in chapter8/a */
#ifndef _IPC_FUNC_H_
#define	_IPC_FUNC_H_
#include"ipc_type.h"
PUBLIC  void    msg_send(int src, int dest, MESSAGE*m);
PUBLIC  void    msg_receive(int dest, int src, MESSAGE*m);
#endif
