#if defined _mix_logic_included
    #endinput
#endif
#define _mix_logic_included

stock select_players()
{
    new players[32];
    new count;

    // ch to exclude bots
    get_players(players, count, "h");

    for (new i = 1; i <= MAX_PLAYERS; i++)
    {
        g_in_mix[i] = false;
    }

    // limit to available players to avoid out of bound
    new total_to_select = (g_mix_players < count) ? g_mix_players : count;
    for (new i = 0; i < total_to_select; i++)
    {
        g_in_mix[players[i]] = true;
    }
}
