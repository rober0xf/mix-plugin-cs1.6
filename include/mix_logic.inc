#if defined _mix_logic_included
    #endinput
#endif
#define _mix_logic_included

#include <amxmodx>
#include <cstrike>

#include "mix_vars.inc"
#include "mix_swap.inc"

// move every non-mix player to spectator
stock lock_spectators()
{
    new players[32], count;
    get_players(players, count, "h");

    for (new i = 0; i < count; i++)
    {
        new id = players[i];
        if (!g_in_mix[id])
        {
            // kill alive players first so they cant stay in the map
            if (is_user_alive(id))
                user_kill(id);

            if (cs_get_user_team(id) != CS_TEAM_SPECTATOR)
                cs_set_user_team(id, CS_TEAM_SPECTATOR);
        }
    }
}

// swap teams
stock start_mix()
{
    g_mix_active = true;

    g_mix_round = 0;
    g_mix_score_a = 0;
    g_mix_score_b = 0;
}

stock end_mix()
{
    g_mix_active = false;

    client_print(0, print_chat,"[MIX] EL MIX TERMINO: %d-%d", g_mix_score_a, g_mix_score_b);

    g_mix_round = 0;
    g_mix_score_a = 0;
    g_mix_score_b = 0;
    g_team_a_is_t = false;

    // knife / draft state reset
    g_knife_active = false;
    g_draft_active = false;
    remove_task(MIX_LOCK_TASK_ID);

    // drop any pending draft timeout tasks
    for (new i = 1; i <= MAX_PLAYERS; i++)
    {
        remove_task(DRAFT_TIMEOUT_TASK_ID + i);
    }

    for (new i = 1; i <= MAX_PLAYERS; i++)
    {
        g_in_mix[i] = false;
    }

    server_cmd("exec addons/amxmodx/configs/public.cfg");
    server_exec();

    server_cmd("sv_restart 1");
}

stock handle_mix_round_end(winner)
{
    if (!g_mix_active)
        return;

    g_mix_round++;

    // register winner
    if (winner == 1)
    {
        // t won
        if (g_team_a_is_t)
            g_mix_score_a++;
        else
            g_mix_score_b++;
    }
    else if (winner == 2)
    {
        // ct won
        if (g_team_a_is_t)
            g_mix_score_b++;
        else
            g_mix_score_a++;
    }

    client_print(0, print_chat, "[MIX] ROUND %d/30 | TOM: %d - JERRY: %d", g_mix_round, g_mix_score_a, g_mix_score_b);

    if (g_mix_score_a >= 16)
    {
        client_print(0, print_chat, "[MIX] EQUIPO TOM GANA EL MIX %d-%d", g_mix_score_a, g_mix_score_b);
        end_mix();

        return;
    }

    if (g_mix_score_b >= 16)
    {
        client_print(0, print_chat, "[MIX] EQUIPO JERRY GANA EL MIX %d-%d", g_mix_score_b, g_mix_score_a);
        end_mix();

        return;
    }

    // match point
    if (g_mix_score_a == 15)
    {
        client_print(0, print_chat, "[MIX] MATCH POINT PARA EQUIPO TOM");
    }
    else if (g_mix_score_b == 15)
    {
        client_print(0, print_chat, "[MIX] MATCH POINT PARA EQUIPO JERRY");
    }

    // swap after 15
    if (g_mix_round == MIX_HALF_ROUNDS)
    {
        swap_teams();

        // team A now playing as B
        g_team_a_is_t = !g_team_a_is_t;

        return;
    }

    // limit
    if (g_mix_round >= MIX_HALF_ROUNDS * 2)
    {
        end_mix();
    }
}
