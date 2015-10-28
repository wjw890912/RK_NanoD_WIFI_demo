/*
 * wpa_supplicant/hostapd / Internal implementation of OS specific functions
 * Copyright (c) 2005-2006, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 *
 * This file is an example of operating system specific  wrapper functions.
 * This version implements many of the functions internally, so it can be used
 * to fill in missing functions from the target system C libraries.
 *
 * Some of the functions are using standard C library calls in order to keep
 * this file in working condition to allow the functions to be tested on a
 * Linux target. Please note that OS_NO_C_LIB_DEFINES needs to be defined for
 * this file to work correctly. Note that these implementations are only
 * examples and are not optimized for speed.
 */

#include "build_config.h"

#include "wpa_include.h"

#include <stdarg.h>
#include <FreeRTOS.h>
//#undef OS_REJECT_C_LIB_FUNCTIONS
#include "wpaos.h"
#include "ws_common.h"
#include "arch.h"
#ifdef WPA_AUTH

void * os_malloc(size_t size)
{
   #if 0
	if (size > OSBUFSIZ)
	{
		printf("err os_m %d\n", size);
		return NULL;
	}
	memset(os_buf, 0, OSBUFSIZ);
    return os_buf;
   #endif
   return pvPortMalloc(size);
}

void * os_memset(void *s, int c, size_t n)
{
    char *p = s;
    while (n--)
        *p++ = c;
    return s;
}


void * os_memcpy(void *dest, const void *src, size_t n)
{
    return memcpy(dest, src, n);
}

size_t os_strlen(const char *s)
{
    const char *p = s;
    while (*p)
        p++;
    return p - s;
}

void os_free(void *ptr)
{
    vPortFree(ptr);
	return;
}

int os_get_random(unsigned char *buf, size_t len)
{
    int i;

    srand(255);
    for(i=0; i<len; i++)
    {
        buf[i] = rand();
    }
    return 0;

}

void * os_memmove(void *dest, const void *src, size_t n)
{
	if (dest < src)
		os_memcpy(dest, src, n);
	else {
		/* overlapping areas */
		char *d = (char *) dest + n;
		const char *s = (const char *) src + n;
		while (n--)
			*--d = *--s;
	}
	return dest;
}

int os_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *p1 = s1, *p2 = s2;

	if (n == 0)
		return 0;

	while (*p1 == *p2) {
		p1++;
		p2++;
		n--;
		if (n == 0)
			return 0;
	}

	return *p1 - *p2;
}


int os_strcasecmp(const char *s1, const char *s2)
{
	/*
	 * Ignoring case is not required for main functionality, so just use
	 * the case sensitive version of the function.
	 */
	return os_strcmp(s1, s2);
}


char * os_strchr(const char *s, int c)
{
	while (*s) {
		if (*s == c)
			return (char *) s;
		s++;
	}
	return NULL;
}

char * os_strrchr(const char *s, int c)
{
	const char *p = s;
	while (*p && *p!='\n' && *p!='\r')
		p++;
	p--;
	while (p >= s) {
		if (*p == c)
			return (char *) p;
		p--;
	}
	return NULL;
}

int os_strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2)
	{
		if (*s1 == '\0')
			break;
		s1++;
		s2++;
	}

	return *s1 - *s2;
}


int os_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return 0;

	while (*s1 == *s2)
	{
		if (*s1 == '\0')
			break;
		s1++;
		s2++;
		n--;
		if (n == 0)
			return 0;
	}

	return *s1 - *s2;
}


char * os_strncpy(char *dest, const char *src, size_t n)
{
	char *d = dest;

	while (n--) {
		*d = *src;
		if (*src == '\0')
			break;
		d++;
		src++;
	}

	return dest;
}

char * os_strstr(const char *haystack, const char *needle)
{
	size_t len = os_strlen(needle);

	while (*haystack)
	{
		if (os_strncmp(haystack, needle, len) == 0)
			return (char *) haystack;
		haystack++;
	}

	return NULL;
}

#endif /*WPA_AUTH*/
