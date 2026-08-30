#include <amxmodx>
#include <cstrike>

#include "include/mix_vars.inc"
#include "include/mix_logic.inc"
#include "include/mix_swap.inc"
#include "include/mix_pause.inc"
#include "include/mix_draft.inc"
#include "include/mix_knife.inc"
#include "include/mix_cmd.inc"

public plugin_init()
{
    register_plugin("Rober Mix", "13.13", "rrober");

    // mix commands
    register_clcmd("say", "cmd_say");
    register_clcmd("say_team", "cmd_say");

    // knife round setup
    knife_init();

    //  detect round winner via audio
    register_event("SendAudio", "event_t_win", "a", "2&%!MRAD_terwin");
    register_event("SendAudio", "event_ct_win", "a", "2&%!MRAD_ctwin");
}

public client_putinserver(id)
{
    g_in_mix[id] = false;

    // force spectator
    if (g_knife_active || g_draft_active)
    {
        cs_set_user_team(id, CS_TEAM_SPECTATOR);
    }
}

public client_disconnected(id)
{
    g_in_mix[id] = false;

    if ((g_knife_active || g_draft_active) && (id == g_captain_a || id == g_captain_b))
    {
        client_print(0, print_chat, "[MIX] UN CAPITAN ABANDONO, SE CANCELA EL PROCESO");
        end_mix();
    }
}

public event_t_win()
{
    // knife round first
    if (g_knife_active)
    {
        knife_end(1);
        return;
    }

    if (!g_mix_active)
        return;

    handle_mix_round_end(1);
}

public event_ct_win()
{
    // knife round first
    if (g_knife_active)
    {
        knife_end(2);
        return;
    }

    if (!g_mix_active)
        return;

    handle_mix_round_end(2);
}
