#ifndef _PLAYSTATE_H_
#define _PLAYSTATE_H_

typedef enum __pstate
{
    PLAYER_STOPPED,
    PLAYER_PLAYING,
    PLAYER_PAUSED,
    PLAYER_TRANSITIONING,
    PLAYER_NO_MEDIA,
} pstate_t;

typedef void (* state_listener_t)(pstate_t state);

#endif
