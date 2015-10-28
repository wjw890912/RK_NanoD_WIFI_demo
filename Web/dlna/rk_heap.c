#include "source_macro.h"
#ifdef __WIFI_UPNP_C__
#include <rtos_config.h>
#include "FreeRTOS.h"
#include <stdlib.h>
#include <string.h>


#define MEM_ASSERT(x)                                \
{   unsigned int a = x;                                      \
    if(a == 0)                                       \
    {                                                \
        printf("value ==0\n");                       \
    }                                                \
    else if(a >= 0x03000000 && a <= 0x03050000 )     \
    {                                                \
                                                     \
    }                                                \
    else if(a >= 0x03050000 && a <= 0x03090000)      \
    {                                                \
                                                     \
    }                                                \
    else                                             \
    {                                                \
        printf("value INVALID =0x%04x\n",a);         \
    }                                                \
                                                     \
}

typedef struct A_BLOCK_LINK
{
    struct A_BLOCK_LINK *pxNextFreeBlock;    /*<< The next free block in the list. */
    size_t xBlockSize;                        /*<< The size of the free block. */
} xBlockLink;

//static const unsigned short heapSTRUCT_SIZE    = ( sizeof( xBlockLink ) + portBYTE_ALIGNMENT - ( sizeof( xBlockLink ) % portBYTE_ALIGNMENT ) );
static const unsigned short heapSTRUCT_SIZE    =  sizeof( xBlockLink );
extern size_t xPortGetFreeHeapSize( void );
void *malloc(size_t xWantedSize)
{
    void *p;
    //return pvPortMalloc(xWantedSize);
    p = pvPortMalloc(xWantedSize);
     //printf("remain memory = %d\n", xPortGetFreeHeapSize());
    if (p == NULL)
     {
        //printf("remain memory = %d\n", xPortGetFreeHeapSize());
        printf("malloc fail xwantedSize = %d\n", xWantedSize);
     }
    return p;
}


void free( void *pv )
{
    //MEM_ASSERT((unsigned long)pv);
    vPortFree(pv);
}

void *realloc(void *pv , size_t xWantedSize)
{
    void *new_point = NULL;
    xBlockLink *block_point = NULL;
    size_t copy_size = 0;

    if (!pv)
        return malloc(xWantedSize);

    new_point = malloc(xWantedSize);
    if (new_point == NULL)
    {
        free(pv);
        return NULL;
    }

    block_point = (void *)(( unsigned char * )pv - sizeof( xBlockLink));

    copy_size = block_point->xBlockSize < xWantedSize ? block_point->xBlockSize  : xWantedSize;

    memcpy(new_point, pv, copy_size);
    free(pv);

    return new_point;
}

void *rk_realloc(void *pv , size_t xmySize, size_t xWantedSize)
{
    void *new_point = NULL;
    xBlockLink *block_point = NULL;
    size_t copy_size = 0;

    if (!pv)
        return malloc(xWantedSize);

    new_point = malloc(xWantedSize);
    if (new_point == NULL)
    {
        free(pv);
        return NULL;
    }

    memcpy(new_point, pv, xmySize);
    free(pv);

    return new_point;
}

void *memmove( void* dest, const void* src, size_t count )
{
    void * new_buffer = NULL;
    new_buffer = malloc (count);
    memcpy(new_buffer, src, count);
    memcpy(dest, new_buffer, count);
    free(new_buffer);

    return dest;
}
#endif
