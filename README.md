# CS 1.6 Mix / Pug Plugin

My **AMX Mod X** mix plugin for my local server ReHLDS. It allows 2v2, 3v3, ... any even number of players.

## Features

- Unlimited respawns during warmup. Type `.ready` to start.
- Quick knife round to decide starting sides.
- Handles match starts, half-time team swaps, and score tracking.
- Simple `.pause` command and round restoration.

## Main Commands

- `.ready` / `.unready` — Toggle ready state in warmup.
- `.pause` — Pause during mix.
- `amx_startmix` — Admin command to force-start the match.

## Install

1. Copy the `.amxx` file to `cstrike/addons/amxmodx/plugins/`
2. Add the plugin name to `cstrike/addons/amxmodx/configs/plugins.ini`
3. Restart the server
