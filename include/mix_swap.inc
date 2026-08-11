#if defined _mix_swap_included
    #endinput
#endif
#define _mix_swap_included

#include <amxmodx>
#include <cstrike>
#include "mix_vars.inc"

stock swap_teams()
{
    new players[32], count;
    get_players(players, count, "ch");

    for (new i = 0; i < count; i++)
    {
        new id = players[i];

        // swap players in the mix
        if (!g_in_mix[id])
            continue;

        new CsTeams:current_team = cs_get_user_team(id);

        if (current_team == CS_TEAM_T)
        {
            cs_set_user_team(id, CS_TEAM_CT)
        }
        else if (current_team == CS_TEAM_CT)
        {
            cs_set_user_team(id, CS_TEAM_T)
        }
    }

    client_print(0, print_chat, "[MIX] cambio de lados");
}
