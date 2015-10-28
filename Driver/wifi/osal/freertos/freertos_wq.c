#include "freertos_types.h"
#include "freertos_api.h"
#include "freertos_wq.h"


#include "FreeRTOS.h"
#include "RKOS.h"

#ifdef __DRIVER_INNOPHACE_WIFI_C__
#pragma arm section code = "innowifiCode", rodata = "innowifiCode", rwdata = "innowifidata", zidata = "innowifidata"
typedef int OS_ERR;
typedef int OS_MSG_SIZE;

xTaskHandle wifi_task_tcb[WIFI_TASK_MAX];
uint16 wifi_task_tcb_offset = 0; /*point to the next available task tcb*/

/* implement queue work like linux style */
#define DEBUG(format,...)  //printf("FILE: %s, LINE: %d: \n"format, __FILE__, __LINE__, ##__VA_ARGS__)

static void wq_worker_thread(void* argv)
{
	OS_ERR	  err;
//	void *msg_ptr = NULL;
    struct work_struct work_temp;
	OS_MSG_SIZE msg_size = 0;
	struct work_struct *work = NULL;
	struct workqueue_struct *wq = argv;

    DEBUG();

//	CPU_SR_ALLOC();

	for(;;) {

        //printf("task %s wait\n", wq->task_name);
        err = xQueueReceive(wq->queue_hanle, &work_temp, portMAX_DELAY);
        //printf("task %s get msg\n", wq->task_name);
		//msg_ptr = OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &msg_size, (CPU_TS *)0, &err);
        DEBUG();
		if (err != pdTRUE)
			DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("OSTaskQPend error!\n"));
        DEBUG();
		if (err == pdTRUE) {
			work = (struct work_struct *)&work_temp;
			wq = work->used_wq;

			freertos_spin_lock(&wq->wq_lock);
			//CPU_CRITICAL_ENTER();
            portENTER_CRITICAL();
			if (wq->cur_queue_num >= 1)
				wq->cur_queue_num --;

			//DBG_PRINT(WLAN_DBG_OSAL, _drv_info_, ("wq_worker_thread, current queue num --!\n"));
			//if (wq == padapter->recv_wq)
				//DBG_PRINT(WLAN_DBG_OSAL, _drv_info_, ("recv_work: current queue num --, = %d\n", wq->cur_queue_num));
			//CPU_CRITICAL_EXIT();
            portEXIT_CRITICAL();
			freertos_spin_unlock(&wq->wq_lock);

			if (wq->blocked == _TRUE) {
				DBG_PRINT(WLAN_DBG_OSAL, _drv_warning_, ("[%s]:blocked cur_queue_num:%d\n",
						wq->name, wq->cur_queue_num));
				if (wq->cur_queue_num == 0)
					break;
			} else {
				work->func(work->context, (uint8 *)work->data);
			}
		}
	}

}

struct workqueue_struct *freertos_create_workqueue(const uint8 *name,
	uint32  stack_size, uint32  queue_size, uint32 wlan_priority)
{
	struct workqueue_struct *wq;
	uint32 priority;
	int32 result;
    DEBUG();
	wq = (struct workqueue_struct *)freertos_zmalloc(sizeof(*wq));
	if (!wq)
		goto err;
    DEBUG();
	wq->blocked = _FALSE;
	wq->queue_num = queue_size;
	wq->cur_queue_num = 0;
	wq->name = name;
	freertos_spinlock_init(&wq->wq_lock);

	strcat(wq->task_name, "T_WLAN_WIFI_");
	strcat(wq->task_name, (const char *)name);
	strcat(wq->task_name, "_WQ_TASK");

	strcat(wq->queue_name, "Q_WLAN_WIFI_");
	strcat(wq->queue_name, (const char *)name);
	strcat(wq->queue_name, "_WQ_TASK");

	DBG_PRINT(WLAN_DBG_OSAL, _drv_info_, ("freertos_create_workqueue(): task_name is %s, queue_name is %s\r\n",wq->task_name, wq->queue_name));

	switch(wlan_priority)
	{
	case 0:
	 	priority = 15;
	  	break;

	 case 1:
	 	priority = 17;
	  	break;

	 case 2:
	 	priority = 26;
	 	break;

	 case 3:
	    	priority = 27;
	 	break;

	 case 4:
	    	priority = 29;
	 	break;

	 default:
	   	priority = 29;
	    	break;
	}

	if(wifi_task_tcb_offset == WIFI_TASK_MAX){
		DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("wifi task tcb not enough\n"));
		return 0;
	}

    wq->queue_hanle = xQueueCreate(queue_size, sizeof(struct work_struct));
	DEBUG("queue_size = %d, wq->queue_hanle = 0x%04x\n",queue_size,(uint32)wq->queue_hanle);
	result = freertos_create_task(&wifi_task_tcb[wifi_task_tcb_offset], (const uint8 *)wq->task_name, stack_size, queue_size,
                             		priority, wq_worker_thread, wq);

	if (!result) {
		DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("create task fail: [%s].\n", wq->name));
	}

	wq->task_tcb = &wifi_task_tcb[wifi_task_tcb_offset];
	wifi_task_tcb_offset++;

    DEBUG("wq = 0x%04x\n",(uint32)wq);
	return wq;

	err:
		return NULL;
}

void freertos_destroy_workqueue(struct workqueue_struct *wq)
{
	uint32 wait_num = 5;

	if (!wq)
		return;
    DEBUG();
	DBG_PRINT(WLAN_DBG_OSAL, _drv_info_, ("freertos_destroy_workqueue: [%s]\r\n", wq->name));

	wq->blocked = _TRUE;
    DEBUG();
	/* flush queue work */
	while (wq->cur_queue_num > 0 && wait_num) {
		DBG_PRINT(WLAN_DBG_OSAL, _drv_warning_, ("freertos_destroy_workqueue flush queue work[%s] :%d\n",
			wq->name, wq->cur_queue_num));
		freertos_msleep(1);
		wait_num --;
	}

	if(wq->task_tcb != NULL)
		freertos_delete_task(wq->task_tcb);

	freertos_spinlock_free(&wq->wq_lock);
	freertos_mfree((uint8 *)wq, sizeof(*wq));
	wq = NULL;
}

static int32 queue_work(struct workqueue_struct *wq, struct work_struct *work)
{
	OS_ERR	  err;
	int32 ret = 0;
	void	*msg_ptr = NULL;
   	OS_MSG_SIZE	 msg_size = 0;
	xTaskHandle	*receiver_tcb;
//	CPU_SR_ALLOC();
    DEBUG();
	if (!wq)
	{   DEBUG("wq is NULL\n");
		return -1;
	}
	receiver_tcb = wq->task_tcb;

	if (receiver_tcb == NULL) {
		DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("Oops: queue_work[%s]: thread is unavailable\n", wq->queue_name));
		return -1;
	}

	if (wq->blocked == _TRUE) {
		DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("Oops: queue_work[%s] is blocked\n", wq->queue_name));
		return -1;
	}

	if (wq->cur_queue_num >= wq->queue_num - 10) {
		DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("Oops: queue_work[%s] is full, cur_queue_num = %d\n", wq->queue_name, wq->cur_queue_num));
		ret = 1;
		goto queue_full;
	}
    DEBUG();
	if (!padapter->interrupt_context) {
		freertos_spin_lock(&wq->wq_lock);
		portENTER_CRITICAL();
	}
	wq->cur_queue_num ++;
	//if (wq == padapter->recv_wq)
		//DBG_PRINT(WLAN_DBG_OSAL, _drv_info_, ("recv_work: current queue num++, = %d\n", wq->cur_queue_num));

	if (!padapter->interrupt_context) {
		portEXIT_CRITICAL();
		freertos_spin_unlock(&wq->wq_lock);
	}

	msg_size = sizeof(*work);
	msg_ptr = (void*)work;
	//OSTaskQPost(receiver_tcb, msg_ptr, msg_size, OS_OPT_POST_FIFO, &err);

    err = xQueueSend(wq->queue_hanle, msg_ptr, 100);

	if (err == pdTRUE)
    {

	} else /*if (err == OS_ERR_MSG_POOL_EMPTY || err == OS_ERR_Q_MAX)*/ {
	    DEBUG();
		if (!padapter->interrupt_context) {
			freertos_spin_lock(&wq->wq_lock);
			portENTER_CRITICAL();
		}
		wq->cur_queue_num --;
		if (wq == padapter->recv_wq)
			DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("recv_work: current queue num--, = %d\n", wq->cur_queue_num));
		if (!padapter->interrupt_context) {
			portEXIT_CRITICAL();
			freertos_spin_unlock(&wq->wq_lock);
		}

		if (err == errQUEUE_FULL) {
			DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("os msg pool is empty, no msg got!\n"));
		} else {
			DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("target task msg queue is full: %s\n", wq->queue_name));
		}
		ret = 1;
	}

queue_full:
	return ret;
}

static int32 queue_delayed_work(struct workqueue_struct *wq,
			struct delayed_work *dwork, uint32 delay)
{
	int32 ret = _TRUE;
	DEBUG();
	if (delay == 0)
	{
		ret = queue_work(wq, &(dwork->work));
		if (ret == 1)	{// queue full or msg pool empty
			//freertos_set_timer(&dwork->timer, 50);
			DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("queue_delayed_work fail!\n"));
		}

		return ret;
	}
    DEBUG();
	freertos_set_timer(&dwork->timer, delay);

	return _TRUE;
}

static void delayed_work_timer_handler(void *context)
{
	_timer *ptimer = (_timer *)context;
	struct delayed_work *dwork = (struct delayed_work *)ptimer->context;
    DEBUG();
	queue_delayed_work(dwork->work.used_wq, dwork, 0);
}

void freertos_init_delayed_work(struct delayed_work *dwork, work_func_t func, void * work_data, uint32 delay_time, const uint8 *timer_name)
{
    DEBUG("delay_time = %d\n");
	OS_INIT_DELAYED_WORK(dwork, func);
	dwork->work.data = work_data;

	if (delay_time != 0) {
		freertos_init_timer(&(dwork->timer), NULL, (TIMER_FUN)delayed_work_timer_handler, dwork, timer_name);
	}
}

int32 freertos_queue_delayed_work(struct workqueue_struct *wq,
			struct delayed_work *dwork, uint32 delay, void* context)
{
    //printf("freertos_queue_delayed_work = %s,0x%04x\n",wq->name,(uint32)wq);
	dwork->work.context = context;
	dwork->work.used_wq = wq;
	return queue_delayed_work(wq, dwork, delay);
}

boolean freertos_cancel_delayed_work(struct delayed_work *work)
{
    DEBUG();
	freertos_del_timer_safe(&work->timer);

	//work_clear_pending(&work->work);

	return _TRUE;
}
#pragma arm section code
#endif /*__DRIVER_INNOPHACE_WIFI_C__*/