#ifndef  WPA_INCLUDE

#define  WPA_INCLUDE

//#include     "macro.h"
#include     <string.h>		//标准头文件
#include     "typedef.h"		//数据类型头文件
#include     "hw_nvic.h"
//#include     "SysConfig.h"
//#include     "global.h"
#include     <stdio.h>

//#include     "SysConfig.h"

#include     "Delay.h"

//#include     "sysmalloc.h"//dgl for malloc and free.

#include     <stdarg.h>
//#include     "FsInclude.h"
#include     "sockets.h"
#include     "arch.h"


#define      EOF   (-1)

typedef unsigned long  DWORD;
typedef char TCHAR;


#if 0//WIFI_DEBUG
#define lwip_log(format,...)printf("%s: , LINE: %d: "format" \r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define lwip_log(format,...)
#endif

#endif

