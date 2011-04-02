#include"type.h"
#include"const.h"
#include"ipc.h"
//int get_ticksA()
int get_ticks()
{
	MESSAGE m;
	sendrec(SEND,2,&m);
	sendrec(RECEIVE,2,&m);
	return m.u.m1.m1i1;
}
