#if defined _mix_logic_included
    #endinput
#endif
#define _mix_logic_included

#include <amxmodx>
#include <cstrike>

#include "mix_vars.inc"
#include "mix_swap.inc"

stock bool:select_players()
{
    new t_players[32], ct_players[32];
    new t_count, ct_count;

    get_players(t_players, t_count, "he", "TERRORIST");
    get_players(ct_players, ct_count, "he", "CT");

    for (new i = 1; i <= MAX_PLAYERS; i++)
    {
        g_in_mix[i] = false;
    }

    new players_per_team = g_mix_players / 2;

    if (t_count < players_per_team || ct_count < players_per_team)
    {
        return false;
    }

    // select T players
    for (new i = 0; i < players_per_team; i++)
    {
        g_in_mix[t_players[i]] = true;
    }

    // select CT players
    for (new i = 0; i < players_per_team; i++)
    {
        g_in_mix[ct_players[i]] = true;
    }

    // move to spec
    new all_players[32];
    new all_count;

    get_players(all_players, all_count, "h");
    for (new i = 0; i < all_count; i++)
    {
        new id = all_players[i];
        if (!g_in_mix[id])
        {
            cs_set_user_team(id, CS_TEAM_SPECTATOR);
        }
    }

    return true;
}

// swap teams
stock start_mix()
{
    g_mix_active = true;

    g_mix_round = 0;
    g_mix_score_a = 0;
    g_mix_score_b = 0;

    // team A starts as tt
    g_team_a_is_t = true;
}

stock end_mix()
{
    g_mix_active = false;

    client_print(0, print_chat,"[MIX] EL MIX TERMINO: %d-%d", g_mix_score_a, g_mix_score_b);

    g_mix_round = 0;
    g_mix_score_a = 0;
    g_mix_score_b = 0;
    g_team_a_is_t = false;

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
