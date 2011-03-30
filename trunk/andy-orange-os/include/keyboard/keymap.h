
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                              keymap.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                            Forrest Yu, January, 2004
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************/
/*    "scan code" <--> "key" map.                                     */
/*    It should be and can only be included by keyboard.c!          */
/*	needs	include/keyboard.h 
		include/type.h
		include/const.h*/
/********************************************************************/

#ifndef	_TINIX_KEYMAP_H_
#define	_TINIX_KEYMAP_H_

EXTERN t_32 keymap[NR_SCAN_CODES * MAP_COLS];

#endif /* _TINIX_KEYMAP_H_ */

