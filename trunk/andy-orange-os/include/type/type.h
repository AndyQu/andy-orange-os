#ifndef _TINIX_TYPE_H_
#define _TINIX_TYPE_H_

typedef unsigned int	t_32;
typedef	unsigned short	t_16;
typedef	unsigned char	t_8;

typedef	int		t_bool;
#define FALSE   0
#define TRUE    1

typedef	unsigned long long u64;

typedef	unsigned int	t_port;

typedef	void (*t_pf_int_handler)();

typedef	void*	t_sys_call;
typedef	char*	va_list;

#endif
