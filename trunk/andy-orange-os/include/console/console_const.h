#ifndef _TINIX_CONSOLE_H_
#define _TINIX_CONSOLE_H_
/*This file was added in chaper8.a*/
#define VIDEO_MEM_BASE  0xB8000/*the base address of video memory*/
#define VIDEO_MEM_SIZE  0x8000/*the base address of video memory*/
#define SCR_WIDTH	80

#define DEFAULT_CHAR_COLOR      (MAKE_COLOR(BLACK, WHITE))
#define GRAY_CHAR               (MAKE_COLOR(BLACK, BLACK) | BRIGHT)
#define RED_CHAR                (MAKE_COLOR(BLUE, RED) | BRIGHT)

#endif
