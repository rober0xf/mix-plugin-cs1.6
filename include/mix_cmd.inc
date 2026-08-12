#if defined _mix_cmd_included
    #endinput
#endif
#define _mix_cmd_included

#include "mix_swap.inc"
#include "mix_pause.inc"
#include "mix_logic.inc"

public cmd_say(id)
{
    new args[192];
    read_args(args, charsmax(args));
    remove_quotes(args);
    trim(args);


    // /mix
    if (equali(args, "/mix") == 0)
    {
        client_print(id, print_chat, "[MIX] uso: /mix <jugadores>");
        return PLUGIN_HANDLED;
    }

    // /mix x
    if (containi(args, "/mix ") == 0)
    {
        if (!(get_user_flags(id) & ADMIN_KICK))
        {
            client_print(id, print_chat, "[MIX] no tenes permisos");
            return PLUGIN_HANDLED;
        }

        new value[16]; 
        copy(value, charsmax(value), args[5]);
        trim(value);

        new players = str_to_num(value);

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

        // only real players
        // new connected = get_playersnum();

        // testing with bots
        new player_and_bots[32];
        new connected;
        get_players(player_and_bots, connected, "h");

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

    // .pause
    if (equali(args, ".pause") || equali(args, "/pause"))
    {
        return cmd_pause(id);
    }

    // .swap
    if (equali(args, ".swap") || equali(args, "/swap"))
    {
        return cmd_swap(id);
    }
    
    return PLUGIN_CONTINUE;
}
