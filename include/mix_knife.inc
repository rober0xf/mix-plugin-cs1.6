#if defined _mix_knife_included
    #endinput
#endif
#define _mix_knife_included

#include <amxmodx>
#include <cstrike>
#include <fun>
#include <hamsandwich>

#include "mix_vars.inc"
#include "mix_draft.inc"

stock knife_init()
{
    register_clcmd("jointeam", "cmd_block_jointeam");
    register_clcmd("buy", "cmd_block_buy");
    register_clcmd("buyammo1", "cmd_block_buy");
    register_clcmd("buyammo2", "cmd_block_buy");

    RegisterHam(Ham_Spawn, "player", "fw_knife_spawn", 1);
}

// block team switching during knife / draft
public cmd_block_jointeam(id)
{
    if (g_knife_active || g_draft_active)
        return PLUGIN_HANDLED;

    return PLUGIN_CONTINUE;
}

public cmd_block_buy(id)
{
    if (g_knife_active)
        return PLUGIN_HANDLED;

    return PLUGIN_CONTINUE;
}

// keep captains knife-only
public fw_knife_spawn(id)
{
    if (!g_knife_active)
        return;

    if (id == g_captain_a || id == g_captain_b)
    {
        strip_user_weapons(id);
        if (is_user_alive(id))
            give_item(id, "weapon_knife");

        cs_set_user_money(id, 0);
    }
    else
    {
        // kill + force spec
        if (is_user_alive(id))
            user_kill(id);
        if (cs_get_user_team(id) != CS_TEAM_SPECTATOR)
            cs_set_user_team(id, CS_TEAM_SPECTATOR);
    }
}

public task_lock_spec()
{
    if (!g_knife_active && !g_draft_active)
    {
        remove_task(MIX_LOCK_TASK_ID);
        return;
    }

    lock_spectators();
}

stock bool:start_knife()
{
    if (g_mix_active || g_knife_active || g_draft_active)
        return false;

    new players[32], count;
    get_players(players, count, "h");
    if (count < 2)
        return false;

    // pick two random captains
    new a = players[random_num(0, count - 1)];
    new b = players[random_num(0, count - 1)];

    new guard;
    while (b == a && guard++ < 10)
        b = players[random_num(0, count - 1)];
    if (b == a)
        b = (a + 1) % count;

    g_captain_a = a;
    g_captain_b = b;
    g_knife_active = true;

    for (new i = 1; i <= MAX_PLAYERS; i++)
    {
        g_in_mix[i] = false;
    }

    g_in_mix[a] = true;
    g_in_mix[b] = true;

    cs_set_user_team(a, CS_TEAM_T);
    cs_set_user_team(b, CS_TEAM_CT);
    cs_set_user_money(a, 0);
    cs_set_user_money(b, 0);

    lock_spectators();
    set_task(1.0, "task_lock_spec", MIX_LOCK_TASK_ID, _, _, "b");

    client_print(0, print_chat, "[ROBER MIX] KNIFE 1v1: %n vs %n", a, b);
    server_cmd("sv_restart 1");

    return true;
}

public knife_end(winner)
{
    if (!g_knife_active)
        return;

    g_knife_active = false;

    new winner_id, loser_id;
    if (winner == 1)
    {
        winner_id = g_captain_a;
        loser_id = g_captain_b;
    }
    else
    {
        winner_id = g_captain_b;
        loser_id = g_captain_a;
    }

    g_captain_a = winner_id;
    g_captain_b = loser_id;

    client_print(0, print_chat, "[ROBER MIX] %n GANO", winner_id);

    new menu = menu_create("ELEGI TU LADO:", "side_pick_handler");
    menu_additem(menu, "TERRORIST", "1");
    menu_additem(menu, "CT", "2");
    menu_setprop(menu, MPROP_EXIT, MEXIT_NEVER);
    menu_display(winner_id, menu);
}

public side_pick_handler(id, menu, item)
{
    if (item == MENU_EXIT)
    {
        menu_destroy(menu);
        return PLUGIN_HANDLED;
    }

    new data[8], name[33], access, callback;
    menu_item_getinfo(menu, item, access, data, charsmax(data), name, charsmax(name), callback);
    new t = str_to_num(data);

    menu_destroy(menu);

    g_team_a_is_t = (t == 1);
    client_print(0, print_chat, "[ROBER MIX] %n ELIGE %s", g_captain_a, t == 1 ? "T" : "CT");

    start_draft();

    return PLUGIN_HANDLED;
}
