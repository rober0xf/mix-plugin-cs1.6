#if defined _mix_draft_included
    #endinput
#endif
#define _mix_draft_included

#include <amxmodx>
#include <cstrike>

#include "mix_vars.inc"
#include "mix_logic.inc"

stock start_draft()
{
    new needed = g_mix_players / 2 - 1;
    if (needed < 0)
        needed = 0;

    // captains alone are enough
    if (needed == 0)
    {
        finish_draft();
        return;
    }

    g_draft_active = true;
    g_picks_a = 0;
    g_picks_b = 0;
    g_draft_turn_a = true;

    client_print(0, print_chat, "[ROBER MIX] DRAFT: %n y %n | %d eligen c/u", g_captain_a, g_captain_b, needed);

    show_draft_menu(g_captain_a);
}

stock show_draft_menu(picker)
{
    new title[64];
    if (g_draft_turn_a)
        format(title, charsmax(title), "CAPITAN A (%n) ELIGE:", g_captain_a);
    else
        format(title, charsmax(title), "CAPITAN B (%n) ELIGE:", g_captain_b);

    new menu = menu_create(title, "draft_handler");

    new players[32], count;
    get_players(players, count, "h");

    for (new i = 0; i < count; i++)
    {
        new pid = players[i];
        if (g_in_mix[pid])
            continue;

        new name[33];
        get_user_name(pid, name, charsmax(name));

        new data[8];
        num_to_str(pid, data, charsmax(data));

        menu_additem(menu, name, data);
    }

    menu_setprop(menu, MPROP_EXIT, MEXIT_NEVER);
    menu_display(picker, menu);

    // auto-pick after 10s if the captain doesnt choose
    remove_task(DRAFT_TIMEOUT_TASK_ID + picker);
    set_task(10.0, "task_draft_timeout", DRAFT_TIMEOUT_TASK_ID + picker);
}

public task_draft_timeout(taskid)
{
    if (!g_draft_active)
        return;

    new picker = taskid - DRAFT_TIMEOUT_TASK_ID;

    // only valid if it is still this picker's turn
    if (g_draft_turn_a != (picker == g_captain_a))
        return;

    new players[32], count;
    get_players(players, count, "h");

    if (count == 0)
        return;

    new pid = players[random_num(0, count - 1)];
    new guard;
    while (g_in_mix[pid] && guard++ < 10)
        pid = players[random_num(0, count - 1)];
    if (g_in_mix[pid])
        return;

    client_print(0, print_chat, "[ROBER MIX] %n SE QUEDO SIN TIEMPO.", picker);
    apply_draft_pick(pid);
}

stock get_draft_teams(&CsTeams:team_a, &CsTeams:team_b)
{
    if (g_team_a_is_t)
    {
        team_a = CS_TEAM_T;
        team_b = CS_TEAM_CT;
    }
    else
    {
        team_a = CS_TEAM_CT;
        team_b = CS_TEAM_T;
    }
}

stock apply_draft_pick(pid)
{
    new CsTeams:team_a, CsTeams:team_b;
    get_draft_teams(team_a, team_b);

    if (g_draft_turn_a)
    {
        cs_set_user_team(pid, team_a);
        g_picks_a++;
    }
    else
    {
        cs_set_user_team(pid, team_b);
        g_picks_b++;
    }

    g_in_mix[pid] = true;

    client_print(0, print_chat, "[ROBER MIX] %n ELIGE A %n", g_draft_turn_a ? g_captain_a : g_captain_b, pid);

    new needed = g_mix_players / 2 - 1;
    if (needed < 0)
        needed = 0;

    if (g_picks_a >= needed && g_picks_b >= needed)
    {
        finish_draft();
        return;
    }

    // next picker, skipping captains already full
    new next;
    if (g_picks_a >= needed)
    {
        g_draft_turn_a = false;
        next = g_captain_b;
    }
    else if (g_picks_b >= needed)
    {
        g_draft_turn_a = true;
        next = g_captain_a;
    }
    else
    {
        g_draft_turn_a = !g_draft_turn_a;
        next = g_draft_turn_a ? g_captain_a : g_captain_b;
    }

    show_draft_menu(next);
}

public draft_handler(id, menu, item)
{
    if (item == MENU_EXIT)
    {
        menu_destroy(menu);
        show_draft_menu(id);
        return PLUGIN_HANDLED;
    }

    new data[8], name[33], access, callback;
    menu_item_getinfo(menu, item, access, data, charsmax(data), name, charsmax(name), callback);
    new pid = str_to_num(data);

    menu_destroy(menu);

    if (!g_draft_active)
        return PLUGIN_HANDLED;

    if (!is_user_connected(pid))
    {
        show_draft_menu(id);
        return PLUGIN_HANDLED;
    }

    remove_task(DRAFT_TIMEOUT_TASK_ID + id);
    apply_draft_pick(pid);

    return PLUGIN_HANDLED;
}

stock finish_draft()
{
    g_draft_active = false;

    lock_spectators();
    remove_task(MIX_LOCK_TASK_ID);
    remove_task(DRAFT_TIMEOUT_TASK_ID + g_captain_a);
    remove_task(DRAFT_TIMEOUT_TASK_ID + g_captain_b);

    // move captains to the side the knife winner
    new CsTeams:team_a, CsTeams:team_b;
    get_draft_teams(team_a, team_b);
    cs_set_user_team(g_captain_a, team_a);
    cs_set_user_team(g_captain_b, team_b);

    start_mix();
    server_cmd("sv_restart 1");

    client_print(0, print_chat, "[ROBER MIX] %dv%d | MIX INICIADO", g_mix_players / 2, g_mix_players / 2);
}
