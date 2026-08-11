#if defined _mix_cmd_included
    #endinput
#endif
#define _mix_cmd_included

#include "mix_swap.inc"
#include "mix_pause.inc"
#include "mix_logic.inc"

public cmd_mix(id)
{
    if (!(get_user_flags(id) & ADMIN_KICK))
    {
        client_print(id, print_chat, "[MIX] no tenes permisos");
        return PLUGIN_HANDLED;
    }

    new args[32];
    read_args(args, charsmax(args));
    remove_quotes(args);
    trim(args);

    new players = str_to_num(args);

    if (players < 2 || players > MAX_PLAYERS)
    {
        client_print(id, print_chat, "[MIX] numero entre 2 y %d.", MAX_PLAYERS);
        return PLUGIN_HANDLED;
    }

    if (players % 2 != 0)
    {
        client_print(id, print_chat, "[MIX] debe ser cantidad par de jugadores.");
        return PLUGIN_HANDLED;
    }

    new connected = get_playersnum();

    if (connected < players)
    {
        client_print(id, print_chat, "[MIX] no hay suficientes jugadores.");
        return PLUGIN_HANDLED;
    }

    g_mix_players = players;

    select_players();
    client_print(0, print_chat, "[MIX] empieza el mix: %d jugadores.", g_mix_players);

    return PLUGIN_HANDLED;
}
