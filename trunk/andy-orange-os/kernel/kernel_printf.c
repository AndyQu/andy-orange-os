#include"const.h"
#include"type.h"
#include"proto.h"
#include"sys_call_shell.h"

PRIVATE	int vsprintf(char *buf, va_list args,const char *fmt);
PRIVATE  int     kernel_printx(char* buf, int len);

int printf_kernel(const char fmt[], ...)
{
        int i;
        char buf[256];

        va_list arg = (va_list)((char*)&fmt + 4);
        i = vsprintf(buf, arg, fmt);
	kernel_printx(buf, i);

        return i;
}

PRIVATE int vsprintf(char *buf, va_list args,const char *fmt)
{
        char*   p;
        char    tmp[256];
        va_list p_next_arg = args;
	char*des;
        for (p=buf;*fmt;fmt++) {
                if (*fmt != '%') {
                        *p = *fmt;
                        p++;
                        continue;
                }
                fmt++;
                switch (*fmt) {
                case 'd':
                        itoa(tmp, *((int*)p_next_arg));
                        strcpy(p, tmp);
                        p_next_arg += 4;
                        p += strlen(tmp);
                        break;
                case 'c':
			*p=*(char*)p_next_arg;
			p_next_arg+=4;
			p++;
                        break;
		case 's':
			des=(*(char**)p_next_arg);
			strcpy(p,des);
			p+=strlen(des);
			p_next_arg +=4;
			break;
                default:
                        break;
                }
        }

        return (p - buf);
}

PRIVATE  int     kernel_printx(char* buf, int len)
{
	int i=0;
	int tty_id=0;
	int count=0;
	for(i=0;i<len && buf[i]!=0;i++){
		if(buf[i]==MAG_CH_PANIC||buf[i]==MAG_CH_ASSERT){
			continue;
		}
		if(printChar(tty_id,(t_32)buf[i])==0)
			count++;
	}
	return i;
}

