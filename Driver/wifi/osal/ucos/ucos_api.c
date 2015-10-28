#include "malloc.h"
#include "Delay.h"
#include "ucos_types.h"
#include "wlan_include.h"
#include "ucos_api.h"
#include "ucos_wq.h"

#if 0
#if defined   (__CC_ARM) /*!< ARM Compiler */
CPU_STK wifi_task_stk[WIFI_TASK_STK_POOL_SIZE] __attribute__((at(0X68000000+0XC0000)));
#elif defined ( __ICCARM__ ) /*!< IAR Compiler */
CPU_STK wifi_task_stk[WIFI_TASK_STK_POOL_SIZE] @ 0X68000000+0XC0000;
#endif
#endif

CPU_STK wifi_task_stk[WIFI_TASK_STK_POOL_SIZE];

uint16 wifi_task_stk_offset = 0; /*point32 to the next available task stack beginning*/

extern int32 malloc_count;
extern u16 Get_Adc(u8 ch);

#ifdef CONFIG_HEAP_USAGE_TRACKING
int32 used_heap_size;
int32 max_used_heap_size;
int32 min_free_heap_size;

void init_heap_usage(void)
{
	used_heap_size = 0;
	max_used_heap_size = 0;
	min_free_heap_size = get_free_heap_size();
}

void add_heap_usage(void *ptr, int32 size, uint8 memx)
{
	int32 free_heap_size = get_free_heap_size();
	uint16 block_num = 0;
	uint16 block_size;

	if(ptr == NULL) {
		DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("Alloc memory fail, size: %d, current heap free size: %d\n", size, free_heap_size));
		return;
	} else {
		DBG_PRINT(WLAN_DBG_OSAL, _drv_dump_, ("Alloc memory at %p with size of %d\n", ptr, size));
	}

	block_size = get_heap_block_size(memx);

	CPU_IntDis();

	if (size % block_size) {
		block_num = (size / block_size) + 1;
		used_heap_size += (block_num * block_size);
	} else {
		used_heap_size += size;
	}

	if (used_heap_size > max_used_heap_size)
		max_used_heap_size = used_heap_size;

	if (min_free_heap_size > free_heap_size)
		min_free_heap_size = free_heap_size;

	CPU_IntEn();
}

void del_heap_usage(void *ptr, int32 size, uint8 memx)
{
	uint16 block_num = 0;
	uint16 block_size;

	if (ptr == NULL)
		return;

	DBG_PRINT(WLAN_DBG_OSAL, _drv_dump_, ("Free memory at %p with size of %d\n", ptr, size));

	block_size = get_heap_block_size(memx);

	CPU_IntDis();

	if (size % block_size) {
		block_num = (size / block_size) + 1;
		used_heap_size -= (block_num * block_size);
	} else {
		used_heap_size -= size;
	}

	CPU_IntEn();
}

#endif

uint8* ucos_zmalloc(uint32 sz)
{
	void* ptr = 0;
	malloc_count++;

	ptr = mymalloc(SRAMEX, sz);
	if (ptr != 0) {
		mymemset(ptr, 0, sz);
	}

#ifdef CONFIG_HEAP_USAGE_TRACKING
	add_heap_usage(ptr, sz, SRAMEX);
#endif

	return ptr;
}


uint8* ucos_malloc(uint32 sz)
{
	void* ptr = 0;
	malloc_count++;

	ptr = mymalloc(SRAMEX, sz);
	if (ptr != 0) {
		mymemset(ptr, 0, sz);
	}

#ifdef CONFIG_HEAP_USAGE_TRACKING
	add_heap_usage(ptr, sz, SRAMEX);
#endif

	return ptr;
}

void ucos_mfree(uint8 *pbuf, uint32 sz)
{
	if(pbuf == NULL)
		return;
	mymemset(pbuf, 0, sz);
	myfree(SRAMEX, pbuf);
#ifdef CONFIG_HEAP_USAGE_TRACKING
	del_heap_usage(pbuf, sz, SRAMEX);
#endif
	pbuf = NULL;
	malloc_count--;
}

static void ucos_memcpy(void* dst, void* src, uint32 sz)
{
	mymemcpy(dst, src, sz);
}

static int32 ucos_memcmp(void *dst, void *src, uint32 sz)
{
	//under spread sdk, the return value of
	//memcmp for two same mem. chunk is 0
	if(!mymemcmp(dst, src, sz))
		return _TRUE;
	else
		return _FALSE;
}

static void ucos_memset(void *pbuf, int32 c, uint32 sz)
{
	mymemset(pbuf, c, sz);
}

static void ucos_init_sema(_sema *sema, int32 init_val)
{
	OS_ERR err;

	OSSemCreate(sema, "wifi", init_val, &err);

	if(err != OS_ERR_NONE ){
		DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("ucos_init_sema failed!\n"));
	}
}

static void ucos_free_sema(_sema *sema)
{
	OS_ERR     err;

	if (sema == NULL)
		return;

	OSSemDel(sema, OS_OPT_DEL_ALWAYS, &err );

	WLAN_ASSERT( err == OS_ERR_NONE, ("OSSemDel error!\n"));
}

static void ucos_up_sema(_sema	*sema)
{
	OS_ERR	  err;
	OSSemPost(sema, OS_OPT_POST_1, &err);
	WLAN_ASSERT(err == OS_ERR_NONE, ("OSSemPost error!\n"));
}

static void ucos_down_sema(_sema *sema)
{
#if 0 // have timeout argument
	OS_ERR	  err;
	OS_TICK  tick_timeout;
	OS_TICK  tick_start;
	OS_TICK  tick_stop;

	tick_start = OSTimeGet(&err);

	tick_timeout = (timeout * OS_CFG_TICK_RATE_HZ) / 1000;

	if((tick_timeout==0)&&(timeout!=0)){
		tick_timeout = 1;
	}

	OSSemPend (sema,tick_timeout,OS_OPT_PEND_BLOCKING,NULL,&err);


	if (OS_ERR_TIMEOUT==err){
		return SYS_ARCH_TIMEOUT;
	}


	if(OS_ERR_NONE == err){

		if(timeout==0){
			return 0;
		}

		tick_stop = OSTimeGet(&err);
		return ((tick_stop - tick_start)*1000) / OS_CFG_TICK_RATE_HZ;
	}

	return 0;
#endif

	OS_ERR	  err;
	OSSemPend(sema, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
	WLAN_ASSERT(err == OS_ERR_NONE, ("OSSemPend error!\n"));
}

static uint32	ucos_get_current_time(void)
{
	OS_ERR err;

	return (uint32)((OSTimeGet(&err)*1000)/OS_CFG_TICK_RATE_HZ);
}

void ucos_msleep(int32 ms)
{
	OS_ERR err;

	if ((ms*OS_CFG_TICK_RATE_HZ) >= 1000)
		OSTimeDly((ms*OS_CFG_TICK_RATE_HZ)/1000, OS_OPT_TIME_DLY, &err);
	else
		OSTimeDly(1, OS_OPT_TIME_DLY, &err); // sleep one tick
	WLAN_ASSERT(err==OS_ERR_NONE, ("OSTimeDly error!\n"));
}

static void ucos_usleep(int32 us)
{
	if (us >= 1000)
		os_msleep(us/1000);
	else
		os_msleep(1); //there is no usleep in ucos,  so we use sleep 1ms here.
}
static void ucos_mdelay(int32 ms)
{
	delay_ms(ms);
}

static void ucos_udelay(int32 us)
{
	delay_us(us);
}

static void ucos_mutex_init(_mutex *pmutex)
{
	OS_ERR err;

	OSMutexCreate(pmutex, "wifi", &err);

	if(err != OS_ERR_NONE ){
		DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("ucos_mutex_init failed!\n"));
	}
}

static void ucos_mutex_free(_mutex *pmutex)
{
	OS_ERR	  err;

	OSMutexDel(pmutex, OS_OPT_DEL_ALWAYS, &err);

	WLAN_ASSERT(err == OS_ERR_NONE, ("OSMutexDel error!\n"));
}

__inline static void _ucos_mutex_get(_mutex *pmutex)
{
	OS_ERR	  err;

	OSMutexPend(pmutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);

	WLAN_ASSERT(err == OS_ERR_NONE, ("OSMutexPend error!\n"));
}

__inline static void _ucos_mutex_put(_mutex *pmutex)
{
	OS_ERR	  err;

	OSMutexPost(pmutex, OS_OPT_POST_NONE, &err);

	WLAN_ASSERT(err == OS_ERR_NONE, ("OSMutexPost error!\n"));
}

static void ucos_mutex_get(_lock	*plock)
{
	_ucos_mutex_get((_mutex *)plock);
}

static void ucos_mutex_put(_lock *plock)
{
	_ucos_mutex_put((_mutex *)plock);
}

void ucos_spinlock_init(_lock *plock)
{
	ucos_mutex_init((_mutex *)plock);
}

void ucos_spinlock_free(_lock *plock)
{
	ucos_mutex_free((_mutex *)plock);
}

void ucos_spin_lock(_lock	*plock)
{
	_ucos_mutex_get((_mutex *)plock);
}

void ucos_spin_unlock(_lock *plock)
{
	_ucos_mutex_put((_mutex *)plock);
}

static void ucos_spin_lock_irqsave(_lock *plock, _irqL *pirqL)
{
	_ucos_mutex_get(plock);
}

static void ucos_spin_unlock_irqsave(_lock *plock, _irqL *pirqL)
{
	_ucos_mutex_put(plock);
}

static void ucos_enter_critical(_lock *plock, _irqL *pirqL)
{
	//CPU_SR_ALLOC();
	//CPU_CRITICAL_ENTER();

	CPU_IntDis();
}

static void ucos_exit_critical(_lock *plock, _irqL *pirqL)
{
	//CPU_CRITICAL_EXIT();
	CPU_IntEn();
}

static void	ucos_timer_handler(void *p_tmr, void *p_arg)
{
	_timer *ptimer = (_timer *)p_arg;

	DBG_PRINT(WLAN_DBG_OSAL, _drv_debug_, ("ucos_timer_handler!\n"));

	if (ptimer->function)
		ptimer->function((uint32)ptimer);
}

static void ucos_yield(void)
{
	OS_ERR	  err;

	OSSchedRoundRobinYield(&err);

	WLAN_ASSERT(err == OS_ERR_NONE, ("OSSchedRoundRobinYield error!\n"));
}

void ucos_init_timer(_timer *ptimer,void *padapter, TIMER_FUN pfunc,void* cntx, const uint8 *name)
{
	OS_ERR	  err;

	os_memset((void*)ptimer->timer_name, 0, 32);
	os_memcpy((void*)ptimer->timer_name, (void*)name, 32);

	// create the timer with initial delay 1s.
	OSTmrCreate(&ptimer->os_timer, ptimer->timer_name, OS_CFG_TICK_RATE_HZ, 0, OS_OPT_TMR_ONE_SHOT,
				(OS_TMR_CALLBACK_PTR)ucos_timer_handler, (void*)ptimer, &err);

	if(err != OS_ERR_NONE ){
		DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("ucos_init_timer failed!\n"));
	}

       ptimer->function = (TIMER_FUN)pfunc;

	ptimer->adapter = (void*)padapter;
	ptimer->context = (void*)cntx;

}

void ucos_set_timer(_timer *ptimer,uint32 delay_time)
{
	OS_ERR	  err;

	if(!ptimer->function)
		return;

	// change timer to the desired delay
	if ((delay_time*OS_CFG_TMR_TASK_RATE_HZ) >= 1000)
		ptimer->os_timer.Dly = (OS_TICK) (delay_time*OS_CFG_TMR_TASK_RATE_HZ/1000) ;
	else
		ptimer->os_timer.Dly = (OS_TICK)1;
	OSTmrStart(&ptimer->os_timer, &err);
	WLAN_ASSERT(err == OS_ERR_NONE, ("OSTmrStart error!\n") );
}

boolean ucos_timer_pending(_timer *ptimer)
{
	OS_ERR	  err;
	OS_STATE	timer_state;

	if(!ptimer->function)
		return _FALSE;
	//if (!ptimer->os_timer)
	//	return _FALSE;

	timer_state = OSTmrStateGet(&ptimer->os_timer, &err);

	if (OS_TMR_STATE_RUNNING == timer_state)
		return _TRUE;
	else
		return _FALSE;
}

void ucos_cancel_timer(_timer *ptimer,uint8 *bcancelled)
{
	OS_ERR	  err;
	CPU_BOOLEAN is_cancelled = DEF_FALSE;

	if (ptimer->function) {
		is_cancelled = OSTmrStop(&ptimer->os_timer, OS_OPT_TMR_NONE, NULL, &err);
		if(DEF_TRUE == is_cancelled)
			*bcancelled=  1;//TRUE ==1; FALSE==0
		else
			*bcancelled=  0;//TRUE ==1; FALSE==0
	}
}

void ucos_del_timer_safe(_timer *ptimer)
{
	OS_ERR	  err;
	uint8 bcancelled = 0;

	if (ptimer->function) {
		ucos_cancel_timer(ptimer, &bcancelled);
		OSTmrDel(&ptimer->os_timer, &err);
		if(err != OS_ERR_NONE ) {
			DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("ucos_del_timer_safe failed!\n"));
		}
	}

	ptimer->function = (TIMER_FUN)NULL;
}

static uint32 _adc_srand32(void)
{
	uint32 tmp = 0;
	uint32 mask = 0x80000000;

	do {
		tmp |= (Get_Adc(ADC_Channel_1) & 0x01)? mask : 0; //set tmp based on adc's lsb
		mask = mask >> 1;  //shift to the next bit
	} while (mask);

	return tmp;
}

static int32 _ucos_arc4random()
{
	uint32 res = ucos_get_current_time();
	uint32 seed = _adc_srand32();

	seed = ((seed & 0x007F00FF) << 7) ^
	    ((seed & 0x0F80FF00) >> 8) ^ // be sure to stir those low bits
	    (res << 13) ^ (res >> 9);    // using the clock too!

	return (int)seed;
}

static int32 ucos_get_random_bytes(void* dst, uint32 size)
{
	uint32 ranbuf;
	uint32 *lp;
	int32 i, count;
	count = size / sizeof(uint32);
	lp = (uint32 *) dst;

	for(i = 0; i < count; i ++) {
		lp[i] = _ucos_arc4random();
		size -= sizeof(uint32);
	}

	if(size > 0) {
		ranbuf = _ucos_arc4random();
		ucos_memcpy(&lp[i], &ranbuf, size);
	}

	return 0;
}

int32 ucos_create_task(_os_task *task, const uint8 *name, uint32 stack_size, uint32 queue_size,
					uint32 priority, task_func_t func, void *ctx)
{
	OS_ERR err;
	CPU_STK_SIZE stk_size = (stack_size>>2); 	//CPU_STK: CPU_INT32U

	DBG_PRINT(WLAN_DBG_OSAL, _drv_info_, ("&ucos_create_task(): priority is %d\r\n", priority));

	if((wifi_task_stk_offset+stk_size)>WIFI_TASK_STK_POOL_SIZE){
		DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("wifi task stack not enough\n"));
		return 0;
	}

    OSTaskCreate(task,
                 (CPU_CHAR  *)name,
                 (OS_TASK_PTR)func,
                 (void      *)ctx,
                 (OS_PRIO    )priority,
                 (CPU_STK   *)&wifi_task_stk[wifi_task_stk_offset],
                 (CPU_STK_SIZE)(stk_size/10),
                 (CPU_STK_SIZE)stk_size,
                 (OS_MSG_QTY )queue_size,
                 (OS_TICK    )0,
                 (void      *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    *)&err);

	WLAN_ASSERT(err==OS_ERR_NONE, ("OSTaskCreate error!\n"));

	wifi_task_stk_offset+=stk_size;

	return 1;

}

void ucos_delete_task(OS_TCB *task_tcb)
{
	OS_ERR err;

	if (task_tcb != NULL) {
		OSTaskDel(task_tcb, &err);
		WLAN_ASSERT(err==OS_ERR_NONE, ("OSTaskDel error!\n"));
	}
}

const struct os_api_ops os_api = {

	ucos_malloc,
	ucos_zmalloc,
	ucos_mfree,
	ucos_memcpy,
	ucos_memcmp,
	ucos_memset,

	ucos_init_sema,
	ucos_free_sema,
	ucos_up_sema,
	ucos_down_sema,
	ucos_mutex_init,
	ucos_mutex_free,
	ucos_mutex_get,
	ucos_mutex_put,
	ucos_spinlock_init,
	ucos_spinlock_free,
	ucos_spin_lock,
	ucos_spin_unlock,
	ucos_spin_lock_irqsave,
	ucos_spin_unlock_irqsave,
	ucos_enter_critical,
	ucos_exit_critical,

	ucos_get_current_time,
	ucos_msleep,
	ucos_usleep,
	ucos_mdelay,
	ucos_udelay,

	ucos_yield,

	ucos_init_timer,
	ucos_set_timer,
	ucos_timer_pending,
	ucos_cancel_timer,
	ucos_del_timer_safe,

	ucos_get_random_bytes,

	ucos_create_task,
	ucos_delete_task,

	ucos_create_workqueue,
	ucos_destroy_workqueue,
	ucos_init_delayed_work,
	ucos_queue_delayed_work
};

