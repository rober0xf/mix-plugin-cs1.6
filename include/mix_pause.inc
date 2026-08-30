#if defined _mix_pause_included
    #endinput
#endif
#define _mix_pause_included

#include <amxmodx>

#include "mix_vars.inc"

new bool:g_is_paused = false;

public cmd_pause(id)
{
    if (!g_in_mix[id] && !(get_user_flags(id) & ADMIN_KICK))
    {
        client_print(id, print_chat, "[MIX] SOLO JUGADORES DEL MIX O ADMINS PUEDEN PAUSAR");
        return PLUGIN_HANDLED;
    }

    g_is_paused = !g_is_paused;

    if (g_is_paused)
    {
        set_cvar_num("pausable", 1);
        server_cmd("pause");
        client_print(0, print_chat, "[MIX] FUE PAUSADO POR: %n", id);
    }
    else
    {
        set_cvar_num("pausable", 1);
        server_cmd("pause");
        client_print(0, print_chat, "[MIX] FUE REANUDADO POR: %n", id);
    }

    return PLUGIN_HANDLED;
}
