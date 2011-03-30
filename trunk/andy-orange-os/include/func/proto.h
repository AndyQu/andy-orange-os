/*from "../lib/"*/
#ifndef _ASM_LIB_H_
#define _ASM_LIB_H_
#include"type.h"
#include"const.h"
PUBLIC void out_byte(t_port port,t_8 value);
PUBLIC t_8  in_byte(t_port port);
PUBLIC void disp_str(char *pszInfo);
PUBLIC void disp_color_str(char *pszInfo,int color);
PUBLIC void disp_int(unsigned int input);

PUBLIC void*memcpy(void* pDst, void* pSrc, int iSize);

PUBLIC	void	disable_int();
PUBLIC	void	enable_int();

PUBLIC	int	myprintf(const char*,...);

/**
 * `phys_copy' and `phys_set' are used only in the kernel, where segments
 * are all flat (based on 0). In the meanwhile, currently linear address
 * space is mapped to the identical physical address space. Therefore,
 * a `physical copy' will be as same as a common copy, so does `phys_set'.
 */
#define phys_copy memcpy
#endif
