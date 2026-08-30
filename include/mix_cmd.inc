#if defined _mix_cmd_included
    #endinput
#endif
#define _mix_cmd_included

#include "mix_swap.inc"
#include "mix_pause.inc"
#include "mix_logic.inc"
#include "mix_knife.inc"

public cmd_say(id)
{
    new args[192];
    read_args(args, charsmax(args));
    remove_quotes(args);
    trim(args);

    // split command and rest
    new cmd[33];
    new rest[16];
    if (strtok2(args, cmd, charsmax(cmd), rest, charsmax(rest), ' ', TRIM_FULL) == -1)
    {
        copy(cmd, charsmax(cmd), args);
        rest[0] = 0;
    }

    // /mix or .mix
    if (equali(cmd, "/mix") || equali(cmd, ".mix"))
    {
        if (rest[0] == 0)
        {
            client_print(id, print_chat, "[MIX] USO: /mix <jugadores>");
            return PLUGIN_HANDLED;
        }

        if (!(get_user_flags(id) & ADMIN_KICK))
        {
            client_print(id, print_chat, "[MIX] NO TENES PERMISOS");
            return PLUGIN_HANDLED;
        }

        new players = str_to_num(rest);

        if (players < 2 || players > MAX_PLAYERS)
        {
            client_print(id, print_chat, "[MIX] NUMERO ENTRE 2 Y %d.", MAX_PLAYERS);
            return PLUGIN_HANDLED;
        }

        if (players % 2 != 0)
        {
            client_print(id, print_chat, "[MIX] DEBE SER CANTIDAD PAR DE JUGADORES.");
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
            client_print(id, print_chat, "[MIX] NO HAY SUFICIENTES JUGADORES.");
            return PLUGIN_HANDLED;
        }

        g_mix_players = players;

        // already running a mix process
        if (g_mix_active || g_knife_active || g_draft_active)
        {
            client_print(id, print_chat, "[MIX] YA HAY UN MIX O PROCESO EN CURSO");
            return PLUGIN_HANDLED;
        }

        if (!start_knife())
        {
            client_print(id, print_chat, "[MIX] NO HAY SUFICIENTES JUGADORES PARA LA KNIFE 1v1.");
            return PLUGIN_HANDLED;
        }

        server_cmd("exec addons/amxmodx/configs/mix.cfg");
        server_exec();

        return PLUGIN_HANDLED;
    }

    // end mix
    if (equali(cmd, ".end") || equali(cmd, "/end"))
    {
        return cmd_finish(id);
    }

    // .pause
    if (equali(cmd, ".pause") || equali(cmd, "/pause"))
    {
        return cmd_pause(id);
    }

    // .swap
    if (equali(cmd, ".swap") || equali(cmd, "/swap"))
    {
        return cmd_swap(id);
    }
    
    return PLUGIN_CONTINUE;
}

public cmd_finish(id)
{
    if (!(get_user_flags(id) & ADMIN_KICK))
    {
        client_print(id, print_chat, "[MIX] NO TENES PERMISOS");
        return PLUGIN_HANDLED;
    }

    end_mix();
    return PLUGIN_HANDLED;
}
