/*from "../lib/"*/
#ifndef _LIB_KLIBC_H_
#define _LIB_KLIBC_H_
#include"type.h"
#include"const.h"

PUBLIC	char*	itoa(char *str,unsigned int num);
PUBLIC	char*	strcpy(char*dest, char*src);
PUBLIC	int	strlen(char*s);
PUBLIC void port_read(t_port port,void*buf,int count);

#endif
