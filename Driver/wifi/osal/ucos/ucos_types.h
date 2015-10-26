#ifndef __UCOS_TYPE_H_
#define __UCOS_TYPE_H_

#include <includes.h>

#define _SUCCESS	1
#define _FAIL		0

#undef _TRUE
#define _TRUE		1

#undef _FALSE
#define _FALSE		0

#undef TRUE
#define TRUE		1

#undef FALSE
#define FALSE		0

#undef NULL
#define NULL		0

typedef  unsigned char      	boolean;     	/* Boolean value type. */

typedef  unsigned long		uint32;      	/* Unsigned 32 bit value */
typedef  unsigned short     	uint16;      	/* Unsigned 16 bit value */
typedef  unsigned char      	uint8;       	/* Unsigned 8  bit value */

typedef  signed long    		int32;       	/* Signed 32 bit value */
typedef  signed short       	int16;       	/* Signed 16 bit value */
typedef  signed char        	int8;     		/* Signed 8  bit value */

typedef unsigned long long	uint64;
typedef signed long long	int64;

typedef unsigned long SIZE_T;
typedef signed long SSIZE_T;

typedef enum _WLAN_STATUS{
	WLAN_STATUS_FAILURE,
	WLAN_STATUS_SUCCESS
}WLAN_STATUS,*PWLAN_STATUS;

typedef struct _ADAPTER ADAPTER, *PADAPTER;
typedef unsigned long NDIS_STATUS;
typedef struct recv_frame RECV_FRAME, *PRECV_FRAME;

typedef OS_SEM     _sema;
typedef OS_MUTEX	_lock, _mutex;
typedef OS_TCB	_os_task;


#include "wlan_list.h"

typedef struct	list_head	_list;
typedef unsigned long	_irqL;
//typedef BLOCK_ID _thread_hdl_;
typedef OS_PRIO  _thread_hdl_;


typedef long atomic_long_t;

#define IN
#define OUT
#define NDIS_OID uint32

typedef void (*TIMER_FUN)(uint32);

/* careful, max name is 64 */
typedef struct timer_list {
	OS_TMR os_timer;
	char timer_name[32];
//	uint32 expires;
	void (*function)(uint32);

	void *adapter;
	void *context;
} _timer;

#if 0
typedef struct mutex {
	SCI_MUTEX_PTR	sci_mutex;
	uint8 mutex_name[64];
} _lock, _mutex;

typedef struct semaphore {
	SCI_SEMAPHORE_PTR sci_sema;
	uint8 sema_name[64];
} _sema;
#endif

typedef struct	__queue	{
	struct	list_head	queue;
	_lock	lock;
} _queue;

typedef void (*task_func_t)(void* argv);

struct work_struct;
typedef void (*work_func_t)(void *context, uint8 *data);


struct workqueue_struct {
	char task_name[64];
	char queue_name[64];
	const uint8 *name;		/* I: workqueue thread name */
	OS_TCB *task_tcb;
	uint32 queue_num; /* total signal num */
	uint32 cur_queue_num; /* cur signal num should < queue_num */
	uint32 blocked;
	_lock wq_lock;
};

struct work_struct {
	void	 *data;
	work_func_t func;
	void *context;
	struct workqueue_struct *used_wq;
};

struct delayed_work {
	struct work_struct work;
	struct timer_list timer;
};

struct iw_point {
  void 	*pointer;	/* Pointer to the data */
  uint16	length;		/* number of fields or size in bytes */
  uint16	flags;		/* Optional params */
};

#define FIELD_OFFSET(s,field)	((SSIZE_T)&((s*)(0))->field)
#define SIZE_PTR SIZE_T

#define jiffies 	0
#define HZ			10000000	//?
#define PKT_BUF_RESERVE_HEAD	96
#define PKT_BUF_RESERVE_TAIL	64

#endif

