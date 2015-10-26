
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include <malloc.h>

#define TOTAL_BUF_SIZE 1024*1229
char WMA_BUF[TOTAL_BUF_SIZE];
int malloc_pos=0;
int mall_cnt = 0;
int free_cnt = 0;

void *WMV_malloc(int n)
{
#if 1
   malloc_pos +=n;
   printf(" %d \n",malloc_pos);
   if(malloc_pos >=TOTAL_BUF_SIZE)
   {
   	printf("Âë±íÄÚ´æ²»¹»\n");
   }
   return &WMA_BUF[malloc_pos-n];
#else
	void *tmp = malloc(n);
	mall_cnt++;
	malloc_pos +=n;
	printf(" %d  %d\n",n,mall_cnt);
	return tmp;
#endif
}


void WMV_free(void *p)
{
	free_cnt++;
	//printf("free  %d \n",free_cnt);
	//free(p);
	p = NULL;
}

