#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#include <player/play-state.h>

typedef struct ctrl_s
{
    char name[128];
    int (*init)(void);
    int (*deinit)(void);
    int (*play)(char* url);
    int (*stop)(void);
    int (*resume)(void);
    int (*pause)(void);
    int (*seek)(int ms);
    int (*current_time)(char* buf, int len);
    int (*get_duration)(char* buf, int len);
    int (*get_state)(void);
    int (*set_state)(int state);
    int (*state_listener)(state_listener_t listener);
    int (*set_volume)(int volume);
} ctrl_t;

#endif
