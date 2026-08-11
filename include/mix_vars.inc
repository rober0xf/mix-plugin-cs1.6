#if defined _mix_vars_included
    #endinput
#endif
#define _mix_vars_included

#include <amxmodx>

#define MAX_PLAYERS 32

new g_mix_players;
new bool:g_in_mix[MAX_PLAYERS + 1];
