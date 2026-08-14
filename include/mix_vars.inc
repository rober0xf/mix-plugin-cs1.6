#if defined _mix_vars_included
    #endinput
#endif
#define _mix_vars_included

#include <amxmodx>

#define MAX_PLAYERS 32
#define MIX_HALF_ROUNDS 15

new g_mix_players;
new bool:g_in_mix[MAX_PLAYERS + 1];

// mix state
new bool:g_mix_active = false;
new g_mix_round = 0;

// store teams score
new g_mix_score_a = 0;
new g_mix_score_b = 0;

// which team is playing as a t
new bool:g_team_a_is_t = false;
