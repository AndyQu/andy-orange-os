#ifndef _VIDEO_CONST_H_
#define	_VIDEO_CONST_H_

/*VGA macros*/
#define	CRTC_ADDR_REG	0x03D4
#define	CRTC_DATA_REG	0x03D5
#define	CRTC_DATA_IDX_START_ADDR_H	0x0C
#define	CRTC_DATA_IDX_START_ADDR_L	0x0D
#define	CRTC_DATA_IDX_CURSOR_H		0x0E
#define	CRTC_DATA_IDX_CURSOR_L		0x0F


/*added in chapter8.a*/
/* Color */
/*
 * e.g. MAKE_COLOR(BLUE, RED)
 *      MAKE_COLOR(BLACK, RED) | BRIGHT
 *      MAKE_COLOR(BLACK, RED) | BRIGHT | FLASH
 */
#define BLACK   0x0     /* 0000 */
#define WHITE   0x7     /* 0111 */
#define RED     0x4     /* 0100 */
#define GREEN   0x2     /* 0010 */
#define BLUE    0x1     /* 0001 */
#define FLASH   0x80    /* 1000 0000 */
#define BRIGHT  0x08    /* 0000 1000 */
#define MAKE_COLOR(x,y) ((x<<4) | y) /* MAKE_COLOR(Background,Foreground) */


#endif
