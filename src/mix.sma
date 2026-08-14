#include <amxmodx>

#include "include/mix_cmd.inc"
#include "include/mix_logic.inc"
#include "include/mix_vars.inc"
#include "include/mix_swap.inc"
#include "include/mix_pause.inc"

public plugin_init()
{
    register_plugin("Rober Mix", "13.13", "rrober");

    // mix commands
    register_clcmd("say", "cmd_say");
    register_clcmd("say_team", "cmd_say");

    //  detect round winner via audio
    register_event("SendAudio", "event_t_win", "a", "2&%!MRAD_terwin");
    register_event("SendAudio", "event_ct_win", "a", "2&%!MRAD_ctwin");
}

public client_putinserver(id)
{
    g_in_mix[id] = false;
}

public client_disconnected(id)
{
    g_in_mix[id] = false;
}

public event_t_win()
{
    if (!g_mix_active)
        return;

    handle_mix_round_end(1);
}

public event_ct_win()
{
    if (!g_mix_active)
        return;

    handle_mix_round_end(2);
}
