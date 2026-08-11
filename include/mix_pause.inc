#if defined _mix_pause_included
    #endinput
#endif
#define _mix_pause_included

#include <amxmodx>

new bool:g_is_paused = false;

public cmd_pause(id)
{
    if (!g_in_mix[id] && !(get_user_flags(id) & ADMIN_KICK))
    {
        client_print(id, print_chat, "[MIX] solo jugadores del mix o admins pueden pausar");
        return PLUGIN_HANDLED;
    }

    g_is_paused = !g_is_paused;

    // engine capability
    set_cvar_num("pausable", 1);

    if (g_is_paused)
    {
        server_cmd("pause");
        client_print(0, print_chat, "[MIX] fue pausado por: %n", id);
    }
    else
    {
        server_cmd("pause");
        client_print(0, print_chat, "[MIX] fue reanudado por: %n", id);
    }

    return PLUGIN_HANDLED;
}
