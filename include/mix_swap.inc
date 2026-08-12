#if defined _mix_swap_included
    #endinput
#endif
#define _mix_swap_included

#include <amxmodx>
#include <cstrike>
#include "mix_vars.inc"

public cmd_swap(id)
{
    if (!(get_user_flags(id) & ADMIN_KICK))
    {
        client_print(id, print_chat, "[MIX] no tenes permisos")
        return PLUGIN_HANDLED;
    }

    swap_teams();
    return PLUGIN_HANDLED;
}

stock swap_teams()
{
    new players[32], count;

    // explude c to enable bots
    get_players(players, count, "h");

    for (new i = 0; i < count; i++)
    {
        new id = players[i];

        // swap players in the mix
        if (!g_in_mix[id])
            continue;

        new CsTeams:current_team = cs_get_user_team(id);

        if (current_team == CS_TEAM_T)
        {
            cs_set_user_team(id, CS_TEAM_CT);
        }
        else if (current_team == CS_TEAM_CT)
        {
            cs_set_user_team(id, CS_TEAM_T);
        }
    }

    client_print(0, print_chat, "[MIX] cambio de lados");
}
