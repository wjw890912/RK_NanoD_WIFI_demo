#ifndef __WLAN_DYN_CHK_H_
#define __WLAN_DYN_CHK_H_

#define BUSY_TRAFFIC_THREADHOLD					50			// Tx/Rx packets in a certain period

#ifdef CONFIG_SOFTAP_SUPPORT
void softap_auth_expire(ADAPTER *Adapter);
void softap_assoc_expire(ADAPTER *Adapter);
#endif

void wlan_dynamic_check_timer_handler (void *FunctionContext);
void wlan_link_status_check(ADAPTER *Adapter);
void wlan_dynamic_check(ADAPTER *Adapter);

#endif
