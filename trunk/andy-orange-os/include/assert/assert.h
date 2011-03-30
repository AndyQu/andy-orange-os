/*
file name:assert.h

This file is added in chapter8-d.
Functions in this file are mainly for debugging.
*/
#ifndef _TINIX_ASSERT_H_
#define _TINIX_ASSERT_H_

/*
assert(exp) MACRO definition
Level: user level
*/
#define ASSERT
#ifdef  ASSERT
 void assertion_failure(char*exp, char*file, char*base_file, int line);
 #define assert(exp) if((exp)); \
 else assertion_failure(#exp,__FILE__,__BASE_FILE__,__LINE__);
#else
        #define assert(exp)
#endif

/*
assert_kernel(exp) MACRO definition
Level: kernel level
*/
#define ASSERT_KERNEL
#ifdef  ASSERT_KERNEL
 void assertion_failure_kernel(char*exp, char*file, char*base_file, int line);
 #define assert_kernel(exp) if((exp)); \
 else assertion_failure_kernel(#exp,__FILE__,__BASE_FILE__,__LINE__);
#else
        #define assert_kernel(exp)
#endif

#endif
