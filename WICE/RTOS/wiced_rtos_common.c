/*
 * Copyright 2015, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

/** @file
 *
 */

#include <string.h>
#include <stdlib.h>
#include "rtos.h"
#include "RTOS/wwd_rtos_interface.h"
#include "platform/wwd_platform_interface.h"
#include "wwd_assert.h"
#include "wwd_debug.h"
#include "wiced_rtos.h"
#include "wiced_defaults.h"
#include "wiced_result.h"
#include "internal/wiced_internal_api.h"
#include "platform_config.h"
#include "source_macro.h"
#include "ap6181wifi.h"
#include "TaskPlugin.h"
#include "wifithread.h"
#ifdef _DRIVER_AP6181_WIFI_C__
//#pragma arm section code = "ap6181wifiCode", rodata = "ap6181wifiCode", rwdata = "ap6181wifidata", zidata = "ap6181wifidata"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define CPU_CYCLES_PER_MICROSECOND    ( CPU_CLOCK_HZ / 1000000 )

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

typedef struct
{
    event_handler_t function;
    void* arg;
} wiced_event_message_t;

/******************************************************
 *               Static Function Declarations
 ******************************************************/

static void timed_event_handler( void* arg );

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/
_ap6181WICE_WIFI_INIT_
wiced_result_t wiced_rtos_init( void )
{
    wiced_result_t result = WICED_SUCCESS;

    WPRINT_RTOS_INFO( ("Started " RTOS_NAME " " RTOS_VERSION "\n") );
#if 1 //rk change 2015-7-27
    result = wiced_rtos_create_worker_thread( WICED_HARDWARE_IO_WORKER_THREAD, WICED_DEFAULT_WORKER_PRIORITY, HARDWARE_IO_WORKER_THREAD_STACK_SIZE, HARDWARE_IO_WORKER_THREAD_QUEUE_SIZE );
    if ( result != WICED_SUCCESS )
    {
        WPRINT_RTOS_ERROR( ("Failed to create WICED_HARDWARE_IO_WORKER_THREAD\n") );
        return result;
    }

    result = wiced_rtos_create_worker_thread( WICED_NETWORKING_WORKER_THREAD, WICED_NETWORK_WORKER_PRIORITY, NETWORKING_WORKER_THREAD_STACK_SIZE, NETWORKING_WORKER_THREAD_QUEUE_SIZE );
    if ( result != WICED_SUCCESS )
    {
        WPRINT_RTOS_ERROR( ("Failed to create WICED_NETWORKING_WORKER_THREAD\n") );
    }
#else
     result = RKTaskCreate(TASK_ID_HARDWARE_TO_WORK, 0, NULL);
    if(result != RK_SUCCESS)
        return WICED_ERROR;

    result = RKTaskCreate(TASK_ID_NETWORKING, 0, NULL);
    if(result != RK_SUCCESS)
        return WICED_ERROR;
#endif


    return result;

}

_ap6181WICE_WIFI_INIT_
wiced_result_t wiced_rtos_deinit( void )
{
#if 1//rk change 2015-7-27
    wiced_result_t result = wiced_rtos_delete_worker_thread( WICED_HARDWARE_IO_WORKER_THREAD );

    if ( result == WICED_SUCCESS )
    {
        result = wiced_rtos_delete_worker_thread( WICED_NETWORKING_WORKER_THREAD );
    }

    return result;
#else
    RKTaskDelete(TASK_ID_HARDWARE_TO_WORK, 0, SYNC_MODE);
    RKTaskDelete(TASK_ID_NETWORKING, 0, SYNC_MODE);
#endif
}

wiced_result_t wiced_rtos_thread_join( wiced_thread_t* thread )
{
    return host_rtos_join_thread( WICED_GET_THREAD_HANDLE( thread ) );
}

wiced_result_t wiced_rtos_delay_milliseconds( uint32_t milliseconds )
{
    return host_rtos_delay_milliseconds( milliseconds );
}

wiced_result_t wiced_rtos_delay_microseconds( uint32_t microseconds )
{
 //rk change
    #if 0
      uint32_t current_time;
    uint32_t duration;
    uint32_t elapsed_time = 0;

    current_time = host_platform_get_cycle_count( );
    duration     = ( microseconds * CPU_CYCLES_PER_MICROSECOND );
    while ( elapsed_time < duration )
    {
        elapsed_time = host_platform_get_cycle_count( ) - current_time;
    }
#endif
    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_init_semaphore( wiced_semaphore_t* semaphore )
{
    return host_rtos_init_semaphore( (host_semaphore_type_t*) semaphore );
}

wiced_result_t wiced_rtos_set_semaphore( wiced_semaphore_t* semaphore )
{
    return host_rtos_set_semaphore( (host_semaphore_type_t*) semaphore, host_platform_is_in_interrupt_context( ) );
}

wiced_result_t wiced_rtos_get_semaphore( wiced_semaphore_t* semaphore, uint32_t timeout_ms )
{
    return host_rtos_get_semaphore( (host_semaphore_type_t*) semaphore, timeout_ms, WICED_FALSE );
}
_ap6181WICE_WIFI_INIT_
wiced_result_t wiced_rtos_deinit_semaphore( wiced_semaphore_t* semaphore )
{
    return host_rtos_deinit_semaphore( (host_semaphore_type_t*) semaphore );
}

wiced_result_t wiced_rtos_push_to_queue( wiced_queue_t* queue, void* message, uint32_t timeout_ms )
{
    return host_rtos_push_to_queue( WICED_GET_QUEUE_HANDLE( queue ), message, timeout_ms );
}

wiced_result_t wiced_rtos_pop_from_queue( wiced_queue_t* queue, void* message, uint32_t timeout_ms )
{
    return host_rtos_pop_from_queue( WICED_GET_QUEUE_HANDLE( queue ), message, timeout_ms );
}

static void worker_thread_main( uint32_t arg )
{
    wiced_worker_thread_t* worker_thread = (wiced_worker_thread_t*) arg;

    while ( 1 )
    {
        wiced_event_message_t message;

        if ( wiced_rtos_pop_from_queue( &worker_thread->event_queue, &message, WICED_WAIT_FOREVER ) == WICED_SUCCESS )
        {
            message.function( message.arg );
        }
    }
}

wiced_result_t wiced_rtos_create_worker_thread( wiced_worker_thread_t* worker_thread, uint8_t priority, uint32_t stack_size, uint32_t event_queue_size )
{
    memset( worker_thread, 0, sizeof( *worker_thread ) );

    if ( wiced_rtos_init_queue( &worker_thread->event_queue, "worker queue", sizeof(wiced_event_message_t), event_queue_size ) != WICED_SUCCESS )
    {
        return WICED_ERROR;
    }

    if ( wiced_rtos_create_thread( &worker_thread->thread, WICED_PRIORITY_TO_NATIVE_PRIORITY( priority ), "worker thread", worker_thread_main, stack_size, (void*) worker_thread ) != WICED_SUCCESS )
    {
        wiced_rtos_deinit_queue( &worker_thread->event_queue );
        return WICED_ERROR;
    }

    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_delete_worker_thread( wiced_worker_thread_t* worker_thread )
{
    if ( wiced_rtos_delete_thread( &worker_thread->thread ) != WICED_SUCCESS )
    {
        rk_printf("iced_rtos_delete_thread delfail\n");
        return WICED_ERROR;
    }

    if ( wiced_rtos_deinit_queue( &worker_thread->event_queue ) != WICED_SUCCESS )
    {
        rk_printf("wiced_rtos_deinit_queue delfail\n");
        return WICED_ERROR;
    }

    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_register_timed_event( wiced_timed_event_t* event_object, wiced_worker_thread_t* worker_thread, event_handler_t function, uint32_t time_ms, void* arg )
{
    if ( wiced_rtos_init_timer( &event_object->timer, time_ms, timed_event_handler, (void*) event_object ) != WICED_SUCCESS )
    {
        return WICED_ERROR;
    }

    event_object->function = function;
    event_object->thread = worker_thread;
    event_object->arg = arg;

    if ( wiced_rtos_start_timer( &event_object->timer ) != WICED_SUCCESS )
    {
        wiced_rtos_deinit_timer( &event_object->timer );
        return WICED_ERROR;
    }

    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_deregister_timed_event( wiced_timed_event_t* event_object )
{
    if ( wiced_rtos_deinit_timer( &event_object->timer ) != WICED_SUCCESS )
    {
        return WICED_ERROR;
    }

    memset( event_object, 0, sizeof( *event_object ) );

    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_send_asynchronous_event( wiced_worker_thread_t* worker_thread, event_handler_t function, void* arg )
{
    wiced_event_message_t message;

    message.function = function;
    message.arg = arg;

    return wiced_rtos_push_to_queue( &worker_thread->event_queue, &message, WICED_NO_WAIT );
}

static void timed_event_handler( void* arg )
{
    wiced_timed_event_t* event_object = (wiced_timed_event_t*) arg;
    wiced_event_message_t message;

    message.function = event_object->function;
    message.arg = event_object->arg;

    wiced_rtos_push_to_queue( &event_object->thread->event_queue, &message, WICED_NO_WAIT );
}

//#pragma arm section code
#endif
