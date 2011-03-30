/*************************************************************************//**
 *****************************************************************************
 * @file   misc.c
 * @brief
 * @author Andy Qu
 * @date   2010
 * This file is added in chapter8/a
 *****************************************************************************
 *****************************************************************************/

#include "type.h"
#include "const.h"
#include"printf.h"

/*
Function: spin
Description:
	spin is a simple function that makes a process hlt in an infinite loop.
Level:	user.
@param:
	char* func_name: function name that calls spin
*/
PUBLIC void spin(char * func_name)
{
	printf("spinning in %s ...\n", func_name);
	while (1) {}
}
/*
Function: assertion_failure
Description:
	print out error message that caused the failure, and spin.
Level:	user.
@param:
	char* exp: the expression that caused the assertion failure
	char* file:the file name, where the assert() failed
	char* base_file: the base file name, if exists
	char* line:the line number of "assert()" sentence.
Attention:
	__processID appears here just for debugging. Should not be accessed in User Level function!!!
*/
extern int __processID;
PUBLIC void assertion_failure(char*exp, char*file, char*base_file, int line)
{
	printf("%d %c Assertion Fail:%s in file:%s line number:%d base file:%s\n",
		__processID,MAG_CH_ASSERT, exp, file, line, base_file
		);
	spin("assertion_fail()");
	__asm__ __volatile__("ud2");
}
/*
Function: panic
Description:
	print out error message and hlt the whole Operating System.
Level:	user.
@param:
	undetermined parameter number.
	
*/
PUBLIC void panic(const char*fmt,...)
{
	int i;
        char buf[256];

        va_list arg = (va_list)((char*)&fmt + 4);
        i = vsprintf(buf, arg, fmt);
        printf("%c !!PANIC!! %s",MAG_CH_PANIC,buf);
	__asm__ __volatile__("ud2");

}


/*
Function: spin_kernel
Description:
	spin_kernel is a simple function that makes kernel hlt in an infinite loop.
Level:	kernel.
@param:
	char* func_name: function name that calls spin_kernel
*/
PUBLIC void spin_kernel(char * func_name)
{
	printf_kernel("spinning in %s ...\n", func_name);
	while (1) {}
}
/*
Function: assertion_failure_kernel
Description:
	print out error message that caused the failure, and spin_kernel.
Level:	kernel.
@param:
	char* exp: the expression that caused the assertion failure
	char* file:the file name, where the assert() failed
	char* base_file: the base file name, if exists
	char* line:the line number of "assert()" sentence.
*/
extern int __processID;
PUBLIC void assertion_failure_kernel(char*exp, char*file, char*base_file, int line)
{
	printf_kernel("%d %c Assertion Fail:%s in file:%s line number:%d base file:%s\n",
		__processID,MAG_CH_ASSERT, exp, file, line, base_file
		);
	spin_kernel("assertion_fail()");
	__asm__ __volatile__("ud2");
}
