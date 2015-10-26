#ifndef __UCOS_API_H_
#define __UCOS_API_H_

#include "wlan_include.h"
#include "ucos_types.h"

#define WIFI_TASK_STK_POOL_SIZE			(256*8)
#define WIFI_TASK_MAX	4


uint8* ucos_zmalloc(uint32 sz);

uint8* ucos_malloc(uint32 sz);

void ucos_mfree(uint8 *pbuf, uint32 sz);

void ucos_msleep(int32 ms);

void ucos_spinlock_init(_lock *plock);

void ucos_spinlock_free(_lock *plock);

void ucos_spin_lock(_lock	*plock);

void ucos_spin_unlock(_lock *plock);

void ucos_init_timer(_timer *ptimer,void *padapter, TIMER_FUN pfunc,void* cntx, const uint8 *name);

void ucos_set_timer(_timer *ptimer,uint32 delay_time);

boolean ucos_timer_pending(_timer *ptimer);

void ucos_cancel_timer(_timer *ptimer,uint8 *bcancelled);

void ucos_del_timer_safe(_timer *ptimer);

int32 ucos_create_task(_os_task *task, const uint8 *name, uint32 stack_size, uint32 queue_size,
					uint32 priority, task_func_t func, void *ctx);

void ucos_delete_task(OS_TCB *task_tcb);

#endif

