#include <amxmodx>

#include "include/mix_cmd.inc"
#include "include/mix_logic.inc"
#include "include/mix_vars.inc"
#include "include/mix_swap.inc"
#include "include/mix_pause.inc"

public plugin_init()
{
    register_plugin("Rober Mix", "1.0", "rrober");

    // mix commands
    register_clcmd("say /mix", "cmd_mix");
    register_clcmd("say_team /mix", "cmd_mix");

    // pause commands
    register_clcmd("say .pause", "cmd_pause");
    register_clcmd("say /pause", "cmd_pause");
    register_clcmd("say_team .pause", "cmd_pause");
    register_clcmd("say_team /pause", "cmd_pause");

    // swap commands. admin only
    register_clcmd("say .swap", "cmd_swap");
    register_clcmd("say /swap", "cmd_swap");
}

public client_putinserver(id)
{
    g_in_mix[id] = false;
}

public client_disconnected(id)
{
    g_in_mix[id] = false;
}
