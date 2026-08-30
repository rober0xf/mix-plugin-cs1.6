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

// knife round state
new bool:g_knife_active = false;
new g_captain_a;
new g_captain_b;

// captain draft state
new bool:g_draft_active = false;
new bool:g_draft_turn_a = true;
new g_picks_a;
new g_picks_b;

#define MIX_LOCK_TASK_ID 5544
#define DRAFT_TIMEOUT_TASK_ID 6000
