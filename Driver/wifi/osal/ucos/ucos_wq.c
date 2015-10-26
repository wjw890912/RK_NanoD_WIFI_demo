#include "ucos_types.h"
#include "ucos_api.h"
#include "ucos_wq.h"

OS_TCB wifi_task_tcb[WIFI_TASK_MAX];
u16 wifi_task_tcb_offset = 0; /*point to the next available task tcb*/

/* implement queue work like linux style */

static void wq_worker_thread(void* argv)
{
	OS_ERR	  err;
	void *msg_ptr = NULL;
	OS_MSG_SIZE msg_size = 0;
	struct work_struct *work = NULL;
	struct workqueue_struct *wq = NULL;
	CPU_SR_ALLOC();

	for(;;) {
		msg_ptr = OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &msg_size, (CPU_TS *)0, &err);

		if (err != OS_ERR_NONE)
			DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("OSTaskQPend error!\n"));

		if (msg_ptr != NULL) {
			work = (struct work_struct *)msg_ptr;
			wq = work->used_wq;

			ucos_spin_lock(&wq->wq_lock);
			CPU_CRITICAL_ENTER();
			if (wq->cur_queue_num >= 1)
				wq->cur_queue_num --;

			//DBG_PRINT(WLAN_DBG_OSAL, _drv_info_, ("wq_worker_thread, current queue num --!\n"));
			//if (wq == padapter->recv_wq)
				//DBG_PRINT(WLAN_DBG_OSAL, _drv_info_, ("recv_work: current queue num --, = %d\n", wq->cur_queue_num));
			CPU_CRITICAL_EXIT();
			ucos_spin_unlock(&wq->wq_lock);

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

struct workqueue_struct *ucos_create_workqueue(const uint8 *name,
	uint32  stack_size, uint32  queue_size, uint32 wlan_priority)
{
	struct workqueue_struct *wq;
	uint32 priority;
	int32 result;

	wq = (struct workqueue_struct *)ucos_zmalloc(sizeof(*wq));
	if (!wq)
		goto err;

	wq->blocked = _FALSE;
	wq->queue_num = queue_size;
	wq->cur_queue_num = 0;
	wq->name = name;
	ucos_spinlock_init(&wq->wq_lock);

	strcat(wq->task_name, "T_WLAN_WIFI_");
	strcat(wq->task_name, (const char *)name);
	strcat(wq->task_name, "_WQ_TASK");

	strcat(wq->queue_name, "Q_WLAN_WIFI_");
	strcat(wq->queue_name, (const char *)name);
	strcat(wq->queue_name, "_WQ_TASK");

	DBG_PRINT(WLAN_DBG_OSAL, _drv_info_, ("ucos_create_workqueue(): task_name is %s, queue_name is %s\r\n",wq->task_name, wq->queue_name));

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

	result = ucos_create_task(&wifi_task_tcb[wifi_task_tcb_offset], (const uint8 *)wq->task_name, stack_size, queue_size,
                             		priority, wq_worker_thread, NULL);

	if (!result) {
		DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("create task fail: [%s].\n", wq->name));
	}

	wq->task_tcb = &wifi_task_tcb[wifi_task_tcb_offset];
	wifi_task_tcb_offset++;

	return wq;

	err:
		return NULL;
}

void ucos_destroy_workqueue(struct workqueue_struct *wq)
{
	uint32 wait_num = 5;

	if (!wq)
		return;

	DBG_PRINT(WLAN_DBG_OSAL, _drv_info_, ("ucos_destroy_workqueue: [%s]\r\n", wq->name));

	wq->blocked = _TRUE;

	/* flush queue work */
	while (wq->cur_queue_num > 0 && wait_num) {
		DBG_PRINT(WLAN_DBG_OSAL, _drv_warning_, ("ucos_destroy_workqueue flush queue work[%s] :%d\n",
			wq->name, wq->cur_queue_num));
		ucos_msleep(1);
		wait_num --;
	}

	if(wq->task_tcb != NULL)
		ucos_delete_task(wq->task_tcb);

	ucos_spinlock_free(&wq->wq_lock);
	ucos_mfree((uint8 *)wq, sizeof(*wq));
	wq = NULL;
}

static int32 queue_work(struct workqueue_struct *wq, struct work_struct *work)
{
	OS_ERR	  err;
	int32 ret = 0;
	void	*msg_ptr = NULL;
   	OS_MSG_SIZE	 msg_size = 0;
	OS_TCB	*receiver_tcb;
	CPU_SR_ALLOC();

	if (!wq)
		return -1;
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

	if (!padapter->interrupt_context) {
		ucos_spin_lock(&wq->wq_lock);
		CPU_CRITICAL_ENTER();
	}
	wq->cur_queue_num ++;
	//if (wq == padapter->recv_wq)
		//DBG_PRINT(WLAN_DBG_OSAL, _drv_info_, ("recv_work: current queue num++, = %d\n", wq->cur_queue_num));

	if (!padapter->interrupt_context) {
		CPU_CRITICAL_EXIT();
		ucos_spin_unlock(&wq->wq_lock);
	}

	msg_size = sizeof(*work);
	msg_ptr = (void*)work;
	OSTaskQPost(receiver_tcb, msg_ptr, msg_size, OS_OPT_POST_FIFO, &err);

	if (err == OS_ERR_NONE) {

	} else if (err == OS_ERR_MSG_POOL_EMPTY || err == OS_ERR_Q_MAX) {
		if (!padapter->interrupt_context) {
			ucos_spin_lock(&wq->wq_lock);
			CPU_CRITICAL_ENTER();
		}
		wq->cur_queue_num --;
		if (wq == padapter->recv_wq)
			DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("recv_work: current queue num--, = %d\n", wq->cur_queue_num));
		if (!padapter->interrupt_context) {
			CPU_CRITICAL_EXIT();
			ucos_spin_unlock(&wq->wq_lock);
		}

		if (err == OS_ERR_MSG_POOL_EMPTY) {
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

	if (delay == 0)
	{
		ret = queue_work(wq, &(dwork->work));
		if (ret == 1)	{// queue full or msg pool empty
			//ucos_set_timer(&dwork->timer, 50);
			DBG_PRINT(WLAN_DBG_OSAL, _drv_err_, ("queue_delayed_work fail!\n"));
		}

		return ret;
	}

	ucos_set_timer(&dwork->timer, delay);

	return _TRUE;
}

static void delayed_work_timer_handler(void *context)
{
	_timer *ptimer = (_timer *)context;
	struct delayed_work *dwork = (struct delayed_work *)ptimer->context;

	queue_delayed_work(dwork->work.used_wq, dwork, 0);
}

void ucos_init_delayed_work(struct delayed_work *dwork, work_func_t func, void * work_data, uint32 delay_time, const uint8 *timer_name)
{
	OS_INIT_DELAYED_WORK(dwork, func);
	dwork->work.data = work_data;

	if (delay_time != 0) {
		ucos_init_timer(&(dwork->timer), NULL, (TIMER_FUN)delayed_work_timer_handler, dwork, timer_name);
	}
}

int32 ucos_queue_delayed_work(struct workqueue_struct *wq,
			struct delayed_work *dwork, uint32 delay, void* context)
{
	dwork->work.context = context;
	dwork->work.used_wq = wq;
	return queue_delayed_work(wq, dwork, delay);
}

boolean ucos_cancel_delayed_work(struct delayed_work *work)
{
	ucos_del_timer_safe(&work->timer);

	//work_clear_pending(&work->work);

	return _TRUE;
}
