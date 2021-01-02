/*
 * Copyright (c) 2002-2011 - Aleksander Naszko, Julien Laurent
 *
 *
 *    By AMX Mod Team.
 *
 *    Some code was taken from AMX Mod X, credits for this code go to them.
 *    The authors of AMX Mod X are:
 *    David "BAILOPAN" Anderson, Pavol "PM OnoTo" Marko,
 *	  Felix "SniperBeamer" Geyer, Jonny "Got His Gun" Bergstrom,
 *		Lukasz "SidLuke" Wlasinski, Christian "Basic-Master" Hammacher,
 *	  Borja "faluco" Ferrer, Scott "Damaged Soul" Ehlert.
 *
 *
 *    This file is part of AMX Mod.
 *
 *
 *    AMX Mod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    AMX Mod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with AMX Mod; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 *
 */

#include <extdll.h>
#include <meta_api.h>
#include <time.h>
#include "amxmod.h"

#define ML() (g_plugins.findPluginFast(amx)->isML())

NATIVE(get_xvar_id) {
  int len;
  char* sName = get_amxstring(amx, params[1], 0, len);
  cell ptr;

  for(CPluginMngr::iterator a = g_plugins.begin(); a ; ++a) {
    if((*a).isValid() && amx_FindPubVar((*a).getAMX(), sName, &ptr) == AMX_ERR_NONE) {
      return g_xvars.put((*a).getAMX(), get_amxaddr((*a).getAMX(), ptr));
    }
  }
  return -1;
}

NATIVE(get_xvar_num) {
  return g_xvars.getValue(params[1]);
}

NATIVE(set_xvar_num) {
  if(g_xvars.setValue(params[1], params[2])) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid xvar id %d", (int)params[1]);
    return 0;
  }
  return 1;
}

NATIVE(xvar_exists) {
  return (get_xvar_id(amx, params) != -1) ? 1 : 0;
}

/* 7 param */
NATIVE(emit_sound) {
  int len;
  char* szSample = get_amxstring(amx, params[3], 0, len);
  float vol = CellToFloat(params[4]);
  float att = CellToFloat(params[5]);
  int channel = params[2];
  int pitch = params[7];
  int flags = params[6];

  if(params[1] == 0) {
    for(int i = 1; i <= gpGlobals->maxClients; ++i) {
      CPlayer* pPlayer = GET_PLAYER_POINTER_I(i);
      if(pPlayer->ingame) {
        EMIT_SOUND_DYN2(pPlayer->pEdict, channel, szSample, vol, att, flags, pitch);
      }
    }
  }
  else {
    edict_t* pEdict = INDEXENT(params[1]);
    if(!FNullEnt(pEdict)) {
      EMIT_SOUND_DYN2(pEdict, channel, szSample, vol, att, flags, pitch);
    }
  }
  return 1;
}

/* 1 param */
NATIVE(server_print) {
  int len;

  g_translator.setDest(0);
  char* message = format_amxstring(amx, params, 1, len);
  if(len > 254) len = 254;

  message[len++]='\n';
  message[len]=0;
  SERVER_PRINT(message);
  return len;
}

/* 3 param */
NATIVE(engclient_print) {
  int len;
  char* message;
  
  int channel = params[2];

  if(params[1] == 0) {
    for(int i = 1; i <= gpGlobals->maxClients; ++i) {
      CPlayer* pPlayer = GET_PLAYER_POINTER_I(i);
      if(pPlayer->ingame) {
        g_translator.setDest(i);
        message = format_amxstring(amx, params, 3, len);
        message[len++]='\n';
        message[len]=0;
        CLIENT_PRINT(pPlayer->pEdict, (PRINT_TYPE)channel, message);
      }
    }
  }
  else {
    int index = params[1];
    if(is_player(index, 1, amx)) {
      CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
      g_translator.setDest(index);
      message = format_amxstring(amx, params, 3, len);
      message[len++]='\n';
      message[len]=0;
      CLIENT_PRINT(pPlayer->pEdict, (PRINT_TYPE)channel, message);
    }
  }
  return len;
}

/* 2 param */
NATIVE(console_cmd) {
  int index = params[1];
  int len;
  char* cmd = format_amxstring(amx,params,2,len);
  
  cmd[len++]='\n';
  cmd[len]=0;

  if(index < 1 || index > gpGlobals->maxClients) {
    SERVER_COMMAND(cmd);
  }
  else {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if(!pPlayer->IsBot() && pPlayer->initialized) {
      CLIENT_COMMAND(pPlayer->pEdict, "%s", cmd);
    }
  }
  return len;
}

/* 2 param */
NATIVE(console_print) {
  int len;
  char* message;
  int index = params[1];

  if(index < 1 || index > gpGlobals->maxClients) {
    g_translator.setDest(0);
    message = format_amxstring(amx, params, 2, len);
    if(len > 254) len = 254;

    message[len++]='\n';
    message[len]=0;
    SERVER_PRINT(message);
  }
  else {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if(pPlayer->ingame) {
      g_translator.setDest(index);
      message = format_amxstring(amx, params, 2, len);
      if(len > 254) len = 254;

      message[len++]='\n';
      message[len]=0;
      UTIL_ClientPrint(pPlayer->pEdict, 2, message);
    }
  }
  return len;
}

/* 3 param */
NATIVE(client_print) {
  int len;
  char* message;
  int index;
  CPlayer *pPlayer;
  
  int channel = params[2];

  if(params[1] == 0) {
    //if(1 /* g_translator.isTranslation(amx,params[3]) */) {
      for(index = 1; index <= gpGlobals->maxClients; ++index) {
        pPlayer = GET_PLAYER_POINTER_I(index);
        if(pPlayer->ingame) {
          g_translator.setDest(index);
          message = format_amxstring(amx, params, 3, len);
          message[len++]='\n';
          message[len]=0;
          UTIL_ClientPrint(pPlayer->pEdict, channel, message);
        }
      }
    /*}
    else {
      message = format_amxstring(amx, params, 3, len);
      message[len++]='\n';
      message[len]=0;
      UTIL_ClientPrint(NULL, channel, message);
    }*/
  }
  else {
    index = params[1];
    if(is_player(index, 1, amx)) {
      pPlayer = GET_PLAYER_POINTER_I(index);
      g_translator.setDest(index);
      message = format_amxstring(amx, params, 3, len);
      message[len++]='\n';
      message[len]=0;
      UTIL_ClientPrint(pPlayer->pEdict, channel, message);
    }
  }
  return len;
}

/* 2 param */
NATIVE(show_motd) {
  int ilen;
  const char* szHead = get_amxstring(amx, params[3], 0, ilen);
  if(!ilen) szHead = hostname->string;

  char* szBody = get_amxstring(amx, params[2], 1, ilen);
  int iFile = 0;
  char* sToShow = NULL;// = szBody;
  if(ilen < 128) sToShow = (char*)LOAD_FILE_FOR_ME(szBody, &iFile);

  if(!iFile) sToShow = szBody;
  else ilen = iFile;

  if(params[1] == 0) {
    for(int i = 1; i <= gpGlobals->maxClients; ++i) {
      CPlayer* pPlayer = GET_PLAYER_POINTER_I(i);
      if(pPlayer->ingame) UTIL_ShowMOTD(pPlayer->pEdict, sToShow, ilen, szHead);
    }
  }
  else {
    int index = params[1];
    if(index < 1 || index > gpGlobals->maxClients) {
      amx_logerror(amx, AMX_ERR_NATIVE, "Invalid player id %d", index);
      if(iFile) FREE_FILE(sToShow);
      return 0;
    }
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if(pPlayer->ingame) UTIL_ShowMOTD(pPlayer->pEdict, sToShow, ilen, szHead);
  }
  if(iFile) FREE_FILE(sToShow);

  return 1;
}

/* 11 param */
NATIVE(set_hudmessage) {
  g_hudset.a1 = 0;
  g_hudset.a2 = 0;
  g_hudset.r2 = 255;
  g_hudset.g2 = 255;
  g_hudset.b2 = 250;
  g_hudset.r1 = params[1];
  g_hudset.g1 = params[2];
  g_hudset.b1 = params[3];
  g_hudset.x = CellToFloat(params[4]);
  g_hudset.y = CellToFloat(params[5]);
  g_hudset.effect = params[6];
  g_hudset.fxTime = CellToFloat(params[7]);
  g_hudset.holdTime = CellToFloat(params[8]);
  g_hudset.fadeinTime = CellToFloat(params[9]);
  g_hudset.fadeoutTime = CellToFloat(params[10]);
  g_hudset.channel = params[11];
  return 1;
}

/* 2 param */
NATIVE(show_hudmessage) {
  int len;
  char* message;
  CPlayer *pPlayer;
  int index;

  if(params[1] == 0) {
    //if(1 /* g_translator.isTranslation(amx,params[2]) */) {
      for(index = 1; index <= gpGlobals->maxClients; ++index) {
        pPlayer = GET_PLAYER_POINTER_I(index);
        if(pPlayer->ingame && !pPlayer->IsBot()) {
          g_translator.setDest(index);
          message = UTIL_SplitHudMessage(format_amxstring(amx, params, 2, len));
          UTIL_HudMessage(pPlayer->pEdict, g_hudset, message);
        }
      }
    /*}
    else {
      message = UTIL_SplitHudMessage(format_amxstring(amx, params, 2, len));
      UTIL_HudMessage(NULL, g_hudset, message);
    }*/
  }
  else {
    index = params[1];
    if(is_player(index, 1, amx)) {
      pPlayer = GET_PLAYER_POINTER_I(index);
      g_translator.setDest(index);
      message = UTIL_SplitHudMessage(format_amxstring(amx, params, 2, len));
      UTIL_HudMessage(pPlayer->pEdict, g_hudset, message);
    }
  }
  return len;
}

/* 3 param */
NATIVE(get_user_name) {
  int index = params[1];
  return set_amxstring(amx, params[2], (index < 1 || index > gpGlobals->maxClients) ?
    hostname->string : g_players[index].name.str(), params[3]);
}

/* 1 param */
NATIVE(get_user_index) {
  int i;
  char* sptemp = get_amxstring(amx,params[1], 0, i);
  for(i = 1; i <= gpGlobals->maxClients; ++i) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(i);
    if(strcmp(pPlayer->name.str(), sptemp) == 0) return i;
  }
  return 0;
}

NATIVE(is_dedicated_server) {
  return (IS_DEDICATED_SERVER() ? 1 : 0);
}

NATIVE(is_linux_server) {
#ifdef LINUX
  return 1;
#else
  return 0;
#endif
}

/* 1 param */
NATIVE(is_map_valid) {
  int ilen;
  return (IS_MAP_VALID(get_amxstring(amx, params[1], 0, ilen)) ? 1 : 0);
}

/* 1 param */
NATIVE(is_user_connected) {
  int index = params[1];
  if(index < 1 || index > gpGlobals->maxClients) {
    return 0;
  }

  CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
  return (pPlayer->ingame ? 1 : 0);
}

/* 1 param */
NATIVE(is_user_connecting) {
  int index = params[1];
  if(index < 1 || index > gpGlobals->maxClients) {
    return 0;
  }

  CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
  return (!pPlayer->ingame && pPlayer->initialized && (GETPLAYERUSERID(pPlayer->pEdict) > 0)) ? 1 : 0;
}

/* 1 param */
NATIVE(is_user_bot) {
  int index = params[1];
  if(index < 1 || index > gpGlobals->maxClients) {
    return 0;
  }

  return (GET_PLAYER_POINTER_I(index)->IsBot() ? 1 : 0);
}

/* 1 param */
NATIVE(is_user_hltv) {
  int index = params[1];
  if(index < 1 || index > gpGlobals->maxClients) {
    return 0;
  }

  CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
  if(!pPlayer->initialized) return 0;

  if(pPlayer->pEdict->v.flags & FL_PROXY) return 1;

  const char* authid = GETPLAYERAUTHID(pPlayer->pEdict);
  if(authid && !stricmp(authid, "HLTV")) return 1;

  return 0;
}

/* 1 param */
extern bool g_bmod_tfc;
NATIVE(is_user_alive) {
  int index = params[1];
  if(index < 1 || index > gpGlobals->maxClients) {
    return 0;
  }

  CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);

  // By AMXMODX Dev Team
  if(g_bmod_tfc) {
    edict_t *e = pPlayer->pEdict;
    if(e->v.flags & FL_SPECTATOR || (!e->v.team || !e->v.playerclass)) {
      return 0;
    }
  }

  return ((pPlayer->ingame && pPlayer->IsAlive()) ? 1 : 0);
}

/* 1 param */
NATIVE(is_user_authorized) {
  int index = params[1];
  if(index < 1 || index > gpGlobals->maxClients) {
    return 0;
  }

  return GET_PLAYER_POINTER_I(index)->authorized;
}

/* 1 param */
NATIVE(get_user_frags) {
  int index = params[1];
  if(index < 1 || index > gpGlobals->maxClients) {
    return 0;
  }

  CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
  return (cell)(pPlayer->ingame ? pPlayer->pEdict->v.frags : 0);
}

/* 1 param */
NATIVE(get_user_deaths) {
  int index = params[1];
  if(index < 1 || index > gpGlobals->maxClients) {
    return 0;
  }

  CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
  return (pPlayer->ingame ? pPlayer->deaths : 0);
}

/* 1 param */
NATIVE(get_user_armor) {
  int index = params[1];
  if(index < 1 || index > gpGlobals->maxClients) {
    return 0;
  }

  CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
  return (cell)(pPlayer->ingame ? pPlayer->pEdict->v.armorvalue : 0);
}

/* 1 param */
NATIVE(get_user_health) {
  int index = params[1];
  if(index < 1 || index > gpGlobals->maxClients) {
    return 0;
  }

  CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
  return (cell)(pPlayer->ingame ? pPlayer->pEdict->v.health : 0);
}

/* 1 param */
NATIVE(get_user_userid) {
  int index = params[1];
  if(index < 1 || index > gpGlobals->maxClients) {
    return 0;
  }

  CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
  return pPlayer->initialized ? GETPLAYERUSERID(pPlayer->pEdict) : -1;
}

/* 3 param */
NATIVE(get_user_authid) {
  int index = params[1];
  const char* authid = 0;
  if(index > 0 && index <= gpGlobals->maxClients) authid = GETPLAYERAUTHID(g_players[index].pEdict);

  return set_amxstring(amx, params[2], authid ? authid : "", params[3]);
}

/* 3 param */
NATIVE(get_weaponname) {
  int index = params[1];
  if(index < 1 || index >= MAX_WEAPONS) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid weapon id %d", index);
    return 0;
  }
  return set_amxstring(amx, params[2], g_weaponsData[index].fullName.str(), params[3]);
}

// By AMXMODX Dev Team
/* 1 param */
NATIVE(get_weaponid) {
  int ilen;
  const char *name = get_amxstring(amx, params[1], 0, ilen);

  for(int i = 1; i < MAX_WEAPONS; ++i) {
    if(!strcmp(g_weaponsData[i].fullName.str(), name))
      return g_weaponsData[i].iId;
  }

  return 0;
}

/* 3 param */
NATIVE(get_user_weapons) {
  int index = params[1];
  if(is_player(index, 1, amx)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    cell *cpNum = get_amxaddr(amx, params[3]);
    cell *cpIds = get_amxaddr(amx, params[2]);
    *cpIds = 0;
    int weapons = pPlayer->pEdict->v.weapons & ~(1<<31); // don't count last element
    for(int i = 1; i < MAX_WEAPONS; ++i){
      if(weapons & (1<<i)) {
        *(cpIds+(*cpNum)) = i;
        (*cpNum)++;
      }
    }
    return weapons;
  }
  return 0;
}

/* 3 param */
NATIVE(get_user_origin) {
  int index = params[1];
  if(is_player(index, 1, amx)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    int mode = params[3];
    cell *cpOrigin = get_amxaddr(amx, params[2]);
    if(mode == 4) {
      cpOrigin[0] = (long int)pPlayer->lastHit.x;
      cpOrigin[1] = (long int)pPlayer->lastHit.y;
      cpOrigin[2] = (long int)pPlayer->lastHit.z;
      return 1;
    }
    edict_t* edict = pPlayer->pEdict;
    Vector pos = edict->v.origin;
    if(mode && mode != 2) pos = pos + edict->v.view_ofs;

    if(mode > 1) {
      Vector v_forward;
      ANGLEVECTORS(edict->v.v_angle, v_forward, NULL, NULL);
      TraceResult trEnd;
      Vector v_dest = pos + v_forward * 9999;
      TRACE_LINE(pos, v_dest, 0, edict, &trEnd);
      pos = (trEnd.flFraction < 1.0) ? trEnd.vecEndPos : Vector(0,0,0);
    }
    cpOrigin[0] = (long int)pos.x;
    cpOrigin[1] = (long int)pos.y;
    cpOrigin[2] = (long int)pos.z;
  }
  return 1;
}

/* 3 param */
NATIVE(get_user_ip) {
  int index = params[1];
  char *ptr;
  char szIp[32];
  strcpy(szIp,(index < 1 || index > gpGlobals->maxClients) ?
    CVAR_GET_STRING("net_address") : g_players[index].ip.str());

  if(params[4] && (ptr = strstr(szIp, ":")) != 0) *ptr = '\0';

  return set_amxstring(amx, params[2], szIp, params[3]);
}

/* 2 param */
NATIVE(get_user_attacker) {
  int index = params[1];
  if(is_player(index, 1, amx)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    edict_t *enemy = NULL;
    enemy = pPlayer->pEdict->v.dmg_inflictor;
    int weapon = 0;
    if(!FNullEnt(enemy)) {
      if(enemy->v.flags & (FL_CLIENT | FL_FAKECLIENT)) {
        pPlayer = GET_PLAYER_POINTER(enemy);
        weapon = pPlayer->current;
      }
      else if(g_grenades.find(enemy, &pPlayer, weapon)) {
        enemy = pPlayer->pEdict;
      }
      else {
        enemy = enemy->v.owner;
        if (!FNullEnt(enemy) && (enemy->v.flags & (FL_CLIENT | FL_FAKECLIENT))) {
          pPlayer = GET_PLAYER_POINTER(enemy);
          weapon = pPlayer->current;
        }
        else {
          switch (*params / sizeof(cell)) {
            case 3: *get_amxaddr(amx, params[3]) = 0;
            case 2: *get_amxaddr(amx, params[2]) = 0;
          }
          return ENTINDEX(pPlayer->pEdict->v.dmg_inflictor);
        }
      }
    }
    else enemy = NULL;

    if(enemy) {
      switch(*params/sizeof(cell)) {
        case 3: *get_amxaddr(amx, params[3]) = pPlayer->aiming;
        case 2: *get_amxaddr(amx, params[2]) = weapon;
      }
    }
    return (enemy ? pPlayer->index : 0);
  }
  return 0;
}

/* 2 param */
NATIVE(has_user_weapon) {
  int index = params[1];
  if(is_player(index, 1, amx)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    edict_t *pEntity = pPlayer->pEdict;
    if((pEntity->v.weapons & (1<<params[2])) > 0) return 1;
  }
  return 0;
}

/* 3 param */
NATIVE(set_user_weapon) {
  int index = params[1];
  if(is_player(index, 1, amx)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    edict_t *pEntity = pPlayer->pEdict;
    if((pEntity->v.weapons & (1<<params[2])) > 0) {
      if(params[3] == 0) {
        pEntity->v.weapons &= ~(1<<params[2]);
        return 1;
      }
      return 0;
    }
    else {
      if(params[3] == 1) {
        pEntity->v.weapons |= (1<<params[2]);
        return 1;
      }
      return 0;
    }
  }
  return 0;
}

/* 3 param */
NATIVE(get_user_weapon) {
  int index = params[1];
  if(is_player(index, 1, amx)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    int wpn = pPlayer->current;
    int wpnflags = pPlayer->pEdict->v.weapons;
    if(!(wpnflags & (1<<wpn)) && (g_bmod_cstrike || g_bmod_dod)) {
      pPlayer->current = 0;
      wpn = 0;
    }
    cell *cpTemp = get_amxaddr(amx, params[2]);
    *cpTemp = (wpn == 0) ? 0 : pPlayer->weapons[wpn].clip;
    cpTemp = get_amxaddr(amx, params[3]);
    *cpTemp = (wpn == 0) ? 0 : pPlayer->weapons[wpn].ammo;
    return wpn;
  }
  return 0;
}

/* 4 param */
NATIVE(get_user_ammo) {
  int index = params[1];
  if(is_player(index, 1, amx)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    int wpn = params[2];
    if(wpn < 1 || wpn >= MAX_WEAPONS) {
      amx_logerror(amx, AMX_ERR_NATIVE, "Invalid weapon id %d", wpn);
      return 0;
    }
    cell *cpTemp = get_amxaddr(amx, params[3]);
    *cpTemp = pPlayer->weapons[wpn].clip;
    cpTemp = get_amxaddr(amx, params[4]);
    *cpTemp = pPlayer->weapons[wpn].ammo;
  }
  return 1;
}

/* 3 param */
NATIVE(get_user_team) {
  int index = params[1];
  if(index < 1 || index > gpGlobals->maxClients) return -1;

  CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
  if(pPlayer->ingame) {
    if(g_bmod_dod) {
      int iTeam = pPlayer->pEdict->v.team;
      if(params[3]) {
        char *szTeam = "";
        switch(iTeam) {
          case 1:
            szTeam = "Allies";
            break;
          case 2:
            szTeam = "Axis";
            break;
        }
        set_amxstring(amx, params[2], szTeam, params[3]);
      }
      return iTeam;
    }
    if(params[3]) set_amxstring(amx, params[2], pPlayer->team.str(), params[3]);

    return pPlayer->teamId;
  }
  return -1;
}

/* 3 param */
NATIVE(show_menu) {
  int ilen,tlen;
  char* sMenu = get_amxstring(amx, params[3], 0, ilen);
  int menuid;
  int keys = params[2];
  int time = params[4];
  int i;

  if(ML()) {
    char *s = get_amxstring(amx, params[5], 1, tlen);
    if(!*s) {
      amx_logerror(amx, AMX_ERR_NONE, "Translator: translated plugin didn't give Menu Title in show_menu()");
      menuid = g_menucmds.findMenuId(sMenu, amx);
    }
    else menuid = g_menucmds.findMenuId(s, amx);
  }
  else menuid = g_menucmds.findMenuId(sMenu, amx);

  if(params[1] == 0) {
    for(i = 1; i <= gpGlobals->maxClients; ++i) {
      CPlayer* pPlayer = GET_PLAYER_POINTER_I(i);
      if(pPlayer->ingame) {
        pPlayer->keys = keys;
        pPlayer->menu = menuid;
        if(time > 0.0f) {
          pPlayer->clearmenu = menuid;
          pPlayer->clearmenutime = gpGlobals->time + time;
        }
        else {
          pPlayer->clearmenu = 0;
          pPlayer->clearmenutime = 0.0f;
        }
        UTIL_ShowMenu(pPlayer->pEdict, keys, time, sMenu, ilen);
      }
    }
  }
  else {
    int index = params[1];
    if(is_player(index, 1, amx)) {
      CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
      pPlayer->keys = keys;
      pPlayer->menu = menuid;
      if(time > 0.0f) {
        pPlayer->clearmenu = menuid;
        pPlayer->clearmenutime = gpGlobals->time + time;
      }
      else {
        pPlayer->clearmenu = 0;
        pPlayer->clearmenutime = 0.0f;
      }
      UTIL_ShowMenu(pPlayer->pEdict, keys, time, sMenu, ilen);
    }
  }
  return 1;
}

/* 3 param */
NATIVE(register_plugin) {
  CPluginMngr::CPlugin* a = g_plugins.findPluginFast( amx );
  int i;
  if(g_translator.isTranslation(amx, params[1])) {
    a->setTitle("RHAH");
    g_translator.setDest(100);
    a->setLocTitle(get_amxstring(amx, params[1], 0, i));
  }
  else a->setTitle(get_amxstring(amx, params[1], 0, i));

  a->setVersion(get_amxstring(amx, params[2], 0, i));
  a->setAuthor(get_amxstring(amx, params[3], 0, i));
  return a->getId();;
}

/* 3 param */
NATIVE(register_menucmd) {
  CPluginMngr::CPlugin* plugin = g_plugins.findPluginFast(amx);
  int ilen, idx;
  char* sptemp = get_amxstring(amx,params[3],0,ilen);

  if(amx_FindPublic(amx, sptemp ,&idx) != AMX_ERR_NONE) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Function is not present (function \"%s\")", sptemp);
    return 0;
  }
  g_menucmds.registerMenuCmd(plugin, params[1], params[2], idx);
  return 1;
}

/* 12 param */
NATIVE(get_plugin) {
  CPluginMngr::CPlugin* a;
  if(params[1] < 0) a = g_plugins.findPluginFast(amx);
  else a = g_plugins.findPlugin((int)params[1]);

  if(a) {
    set_amxstring(amx, params[2], a->getName(), params[3]);
    set_amxstring(amx, params[6], a->getVersion(), params[7]);
    set_amxstring(amx, params[8], a->getAuthor(), params[9]);
    set_amxstring(amx, params[10], a->getStatus(), params[11]);
    cell *addr = get_amxaddr(amx, params[12]);
#if defined JIT
    *addr = 1;
#elif defined JITORNOT
    *addr = a->getJIT() ? 1 : 0;
#else
    *addr = 0;
#endif
// This test was bad: a non-ML plugin could retrieve commands from ML ones
//  if(ML()) {
    if(params[13] == -1) g_translator.setDest(101);
    else g_translator.setDest(params[13]);
//  }
    set_amxstring(amx, params[4], a->getTitle(), params[5]);
    return a->getId();
  }
  return -1;
}

NATIVE(get_pluginsnum) {
  return g_plugins.getPluginsNum();
}

NATIVE(get_modulesnum) {
  int loaded = 0;
  CList<CModule>::iterator a = g_modules.begin();

  while(a) {
    if((*a).getStatusValue() == MODULE_LOADED) ++loaded;
    ++a;
  }
  return loaded;
}

/* 4 param */
NATIVE(register_concmd) {
  CPluginMngr::CPlugin* plugin = g_plugins.findPluginFast(amx);
  int i, idx = 0;
  char* temp = get_amxstring(amx, params[2], 0, i);
  if(amx_FindPublic(amx, temp, &idx) != AMX_ERR_NONE) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Function is not present (function \"%s\")", temp);
    return 0;
  }
  temp = get_amxstring(amx, params[1], 0, i);
  char* info = "";
  if(/*PARAMS()>3 &&*/ !g_translator.isTranslation(amx, params[4])) info = get_amxstring(amx, params[4], 1, i);

  CmdMngr::Command* cmd;
  int access = params[3];
  bool listable = true;
  if(access < 0) { // if access is -1 then hide from listing
    access = 0;
    listable = false;
  }
  if((cmd = g_commands.registerCommand(plugin, idx, temp, info, access, listable)) == NULL) return 0;

  cmd->setCmdType(CMD_ConsoleCommand);

  if(/*PARAMS()>3 &&*/ g_translator.isTranslation(amx, params[4])) {
    g_translator.setDest(100);
    cmd->setLocInfo(get_amxstring(amx, params[4], 0, i));
  }

  REG_SVR_COMMAND((char*)cmd->getCommand(), plugin_srvcmd);
  return 1;
}

/* 4 param */
NATIVE(register_clcmd) {
  CPluginMngr::CPlugin* plugin = g_plugins.findPluginFast(amx);
  int i, idx = 0;
  char* temp = get_amxstring(amx, params[2], 0, i);
  if(amx_FindPublic(amx, temp, &idx) != AMX_ERR_NONE) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Function is not present (function \"%s\")", temp);
    return 0;
  }
  temp = get_amxstring(amx, params[1], 0, i);
  char* info = "";
  if(/*PARAMS()>3 &&*/ !g_translator.isTranslation(amx, params[4])) info = get_amxstring(amx, params[4], 1, i);

  CmdMngr::Command* cmd;
  int access = params[3];
  bool listable = true;
  if(access < 0) { // if access is -1 then hide from listing
    access = 0;
    listable = false;
  }
  if((cmd = g_commands.registerCommand(plugin, idx, temp, info, access, listable)) == NULL) return 0;

  cmd->setCmdType(CMD_ClientCommand);

  if(/*PARAMS()>3 &&*/ g_translator.isTranslation(amx, params[4])) {
    g_translator.setDest(100);
    cmd->setLocInfo(get_amxstring(amx, params[4], 0, i));
  }
  return 1;
}

/* 2 param */
NATIVE(register_srvcmd) {
  CPluginMngr::CPlugin* plugin = g_plugins.findPluginFast(amx);
  int i, idx = 0;
//int p = PARAMS();
  char* temp = get_amxstring(amx, params[2], 0, i);
  if(amx_FindPublic(amx, temp, &idx) != AMX_ERR_NONE) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Function is not present (function \"%s\")", temp);
    return 0;
  }
  temp = get_amxstring(amx,params[1], 0, i);
  char * info = "";
  if(/*p>3 &&*/ !g_translator.isTranslation(amx, params[4])) info = get_amxstring(amx, params[4], 1, i);

  CmdMngr::Command* cmd;
  int access = params[3];
  bool listable = true;
  if(access < 0) { // if access is -1 then hide from listing
    access = 0;
    listable = false;
  }
  if((cmd = g_commands.registerCommand(plugin, idx, temp, info, access, listable)) == NULL) return 0;

  cmd->setCmdType(CMD_ServerCommand);

  if(/*p>3 &&*/ g_translator.isTranslation(amx, params[4])) {
    g_translator.setDest(100);
    cmd->setLocInfo(get_amxstring(amx, params[4], 0, i));
  }

  REG_SVR_COMMAND((char*)cmd->getCommand(), plugin_srvcmd);
  return 0;
}

/* 7 param */
NATIVE(get_concmd) {
  int who = params[8];
  int forwho;

// This test was bad: a non-ML plugin could retrieve commands from ML ones
//  if(ML())
//  {
  forwho = params[9];
  if(forwho == -1) forwho = who == -1 ? 102 : who;

  g_translator.setDest(forwho);
//  }
  // id of player - client command
  if(who > 0) who = CMD_ClientCommand;
  // server
  else if(who == 0) who = CMD_ServerCommand;
  // -1 parameter - all commands
  else who = CMD_ConsoleCommand;

  CmdMngr::Command* cmd = g_commands.getCmd(params[1], who, params[7]);

  if(cmd == 0) return 0;

  set_amxstring(amx, params[2], cmd->getCmdLine(), params[3]);
  set_amxstring(amx, params[5], cmd->getCmdInfo(), params[6]);
  cell *cpFlags = get_amxaddr(amx, params[4]);
  *cpFlags = cmd->getFlags();
  return 1;
}

/* 7 param */
NATIVE(get_clcmd) {
  int forwho;
  forwho = params[8];
  if(forwho == -1) forwho = 102;

  g_translator.setDest(forwho);
  CmdMngr::Command* cmd = g_commands.getCmd(params[1], CMD_ClientCommand, params[7]);
  if(cmd == 0) return 0;

  set_amxstring(amx, params[2], cmd->getCmdLine(), params[3]);
  set_amxstring(amx, params[5], cmd->getCmdInfo(), params[6]);
  cell *cpFlags = get_amxaddr(amx, params[4]);
  *cpFlags = cmd->getFlags();

  return 1;
}

NATIVE(get_srvcmd) {
  int forwho;
  forwho = params[8];
  if(forwho == -1) forwho = 102;

  g_translator.setDest(forwho);
  CmdMngr::Command* cmd = g_commands.getCmd(params[1], CMD_ServerCommand, params[7]);
  if(cmd == 0) return 0;

  set_amxstring(amx,params[2], cmd->getCmdLine() ,params[3]);
  set_amxstring(amx,params[5], cmd->getCmdInfo() ,params[6]);
  cell *cpFlags = get_amxaddr(amx,params[4]);
  *cpFlags = cmd->getFlags();
  return 1;
}

NATIVE(get_cmdaccess) {
  int len;
  char* cmd = get_amxstring(amx, params[1], 0, len);
  char access[32];
  CmdMngr::iterator a = g_commands.begin(CMD_ConsoleCommand);

  while(a) {
    if(!stricmp((*a).getCmdLine(), cmd)) {
      UTIL_GetFlags(access, (*a).getFlags());
      return set_amxstring(amx, params[2], access, params[3]);
    }
    ++a;
  }
  return 0;
}

NATIVE(get_plugincmd) {
  int len;
  char* plugin = get_amxstring(amx, params[1], 0, len);
  int id = params[2];
  int forwho;
  forwho = params[9];
  if(forwho == -1) forwho = 102;

  g_translator.setDest(forwho);
  int type = params[10];
  int amount = -1;
  char access[32];
  CmdMngr::iterator a = g_commands.begin(CMD_ConsoleCommand);

  while(a) {
    if((*a).getCmdTypeInt() & type) {
      if(!stricmp((*a).getPlugin()->getName(), plugin)) {
        ++amount;
        if(amount == id) {
          UTIL_GetFlags( access , (*a).getFlags() );
          set_amxstring(amx, params[3], (*a).getCmdLine(), params[4]);
          set_amxstring(amx, params[5], access, params[6]);
          set_amxstring(amx, params[7], (*a).getCmdInfo(), params[8]);
          return 1;
        }
      }
    }
    ++a;
  }
  return 0;
}

NATIVE(get_plugincmdsnum) {
  int len;
  char* plugin = get_amxstring(amx, params[1], 0, len);
  int type = params[2];
  int amount = 0;
  CmdMngr::iterator a = g_commands.begin(CMD_ConsoleCommand);

  while(a) {
    if((*a).getCmdTypeInt() & type) {
      if(!stricmp((*a).getPlugin()->getName(), plugin)) {
        ++amount;
      }
    }
    ++a;
  }
  return amount;
}

NATIVE(get_plugincvar) {
  int len;
  char* plugin = get_amxstring(amx, params[1], 0, len);
  int id = params[2];
  int flags = params[7];

  if(flags <= 0) flags = 8;

  int amount = -1;

  for(CList<CCVar>::iterator a = g_cvars.begin(); a; ++a) {
    int cvarFlags = CVAR_GET_POINTER((*a).getName())->flags;
    if(!stricmp((*a).getPluginName(), plugin) && ((flags == cvarFlags) || (flags != 8 && (flags & cvarFlags) != 0))) {
      ++amount;
    }
    if(amount == id) {
      set_amxstring(amx, params[3], (*a).getName(), params[4]);
      set_amxstring(amx, params[5], CVAR_GET_STRING((*a).getName()), params[6]);
      return 1;
    }
  }
  return 0;
}

NATIVE(get_plugincvarsnum) {
  int len;
  char* plugin = get_amxstring(amx, params[1], 0, len);
  int flags = params[2];

  if(flags <= 0) flags = 8;

  int amount = 0;

  for(CList<CCVar>::iterator a = g_cvars.begin(); a; ++a) {
    int cvarFlags = CVAR_GET_POINTER((*a).getName())->flags;
    if(!stricmp((*a).getPluginName(), plugin) && ((flags == cvarFlags) || (flags != 8 && (flags & cvarFlags) != 0))) {
      ++amount;
    }
  }
  return amount;
}

/* 1 param */
NATIVE(get_srvcmdsnum) {
  return g_commands.getCmdNum(CMD_ServerCommand, params[1]);
}

/* 1 param */
NATIVE(get_clcmdsnum) {
  return g_commands.getCmdNum(CMD_ClientCommand, params[1]);
}

/* 1 param */
NATIVE(get_concmdsnum) {
  int who = params[2];
  if(who > 0) return g_commands.getCmdNum(CMD_ClientCommand, params[1]);
  if(who == 0) return g_commands.getCmdNum(CMD_ServerCommand, params[1]);

  return g_commands.getCmdNum(CMD_ConsoleCommand, params[1]);
}

/* 2 param */
NATIVE(register_event) {
  int len, pos, iFunction;
  char* sTemp = get_amxstring(amx,params[1],0,len);

  if((pos = g_events.getEventId(sTemp)) == 0) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid event (name \"%s\")", sTemp);
    return 0;
  }

  sTemp = get_amxstring(amx, params[2], 0, len);

  if(amx_FindPublic(amx, sTemp, &iFunction) != AMX_ERR_NONE) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Function is not present (function \"%s\")", sTemp);
    return 0;
  }

  int numparam = *params/sizeof(cell);
  int flags = 0;

  if(numparam > 2) flags = UTIL_ReadFlags(get_amxstring(amx, params[3], 0, len));

  CPluginMngr::CPlugin* plugin = g_plugins.findPluginFast(amx);
  EventsMngr::ClEvent* a = g_events.registerEvent(plugin, iFunction, flags, pos);

  if(a == 0) return 0;

  for(int i = 4; i <= numparam; ++i) {
    a->registerFilter(get_amxstring(amx, params[i], 0, len));
  }
  
  return 1;
}

/* 2 param */
NATIVE(user_kill) {
  int index = params[1];
  if(is_player(index, 1, amx)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if(pPlayer->IsAlive()) {
      float bef = pPlayer->pEdict->v.frags;
      MDLL_ClientKill(pPlayer->pEdict);
      if(params[2]) pPlayer->pEdict->v.frags = bef;
      return 1;
    }
  }
  return 0;
}

/* 2 param */
NATIVE(user_slap) {
  int index = params[1];
  if(is_player(index, 1, amx)) {
    int power = abs((int)params[2]);
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if(pPlayer->IsAlive()) {
      if(pPlayer->pEdict->v.health <= power) {
        float bef = pPlayer->pEdict->v.frags;
        MDLL_ClientKill(pPlayer->pEdict);
        pPlayer->pEdict->v.frags = bef;
      }
      else {
        edict_t *pEdict = pPlayer->pEdict;
        int numparam = *params/sizeof(cell);
        if(numparam<3 || params[3]) {
          pEdict->v.velocity.x += RANDOM_LONG(-600, 600);
          pEdict->v.velocity.y += RANDOM_LONG(-180, 180);
          pEdict->v.velocity.z += RANDOM_LONG(100, 200);
        }
        else {
          Vector v_forward, v_right;
          ANGLEVECTORS( pEdict->v.angles, v_forward, v_right, NULL );
          pEdict->v.velocity = pEdict->v.velocity + v_forward * 220 + Vector(0, 0, 200);
        }
        pEdict->v.punchangle.x = RANDOM_LONG(-10, 10);
        pEdict->v.punchangle.y = RANDOM_LONG(-10, 10);
        pEdict->v.health -= power;
        int armor = (int)pEdict->v.armorvalue;
        armor -= power;
        if(armor < 0) armor = 0;

        pEdict->v.armorvalue = armor;
        pEdict->v.dmg_inflictor = pEdict;
        if(g_bmod_cstrike) {
          static const char *cs_sound[4] = {
            "player/bhit_flesh-3.wav",
            "player/bhit_flesh-2.wav",
            "player/pl_die1.wav",
            "player/pl_pain6.wav"
          };
          EMIT_SOUND_DYN2(pEdict, CHAN_VOICE, cs_sound[RANDOM_LONG(0, 3)], 1.0, ATTN_NORM, 0, PITCH_NORM);
        }
        else {
          static const char *bit_sound[3] = {
            "weapons/cbar_hitbod1.wav",
            "weapons/cbar_hitbod2.wav",
            "weapons/cbar_hitbod3.wav"
          };
          EMIT_SOUND_DYN2(pEdict, CHAN_VOICE, bit_sound[RANDOM_LONG(0, 2)], 1.0, ATTN_NORM, 0, PITCH_NORM);
        }
      }
      return 1;
    }
  }
  return 0;
}

/* 1 param */
NATIVE(server_cmd) {
  int len;
  g_translator.setDest(0);
  char* cmd = format_amxstring(amx, params, 1, len);
  cmd[len++]='\n';
  cmd[len]=0;
  SERVER_COMMAND(cmd);
  return len;
}

/* 2 param */
NATIVE(client_cmd) {
  int len;
  char* cmd;

  if(params[1] == 0) {
    for(int i = 1; i <= gpGlobals->maxClients; ++i) {
      CPlayer* pPlayer = GET_PLAYER_POINTER_I(i);
      if(!pPlayer->IsBot() && pPlayer->initialized) {
        g_translator.setDest(i);
        cmd = format_amxstring(amx, params, 2, len);
        cmd[len++]='\n';
        cmd[len]=0;
        CLIENT_COMMAND(pPlayer->pEdict, " %s", cmd); // blank space added intentionally
      }
    }
  }
  else {
    int index = params[1];
    if(is_player(index, 0, amx)) {
      CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
      if(!pPlayer->IsBot() && pPlayer->initialized) {
        g_translator.setDest(index);
        cmd = format_amxstring(amx, params, 2, len);
        cmd[len++]='\n';
        cmd[len]=0;
        CLIENT_COMMAND(pPlayer->pEdict, " %s", cmd); // blank space added intentionally
      }
    }
  }
  return len;
}

/* 1 param */
NATIVE(get_cvar_pointer) {
	int ilen;
	char *sptemp = get_amxstring(amx, params[1], 0, ilen);

	cvar_t *ptr = CVAR_GET_POINTER(sptemp);

	return reinterpret_cast<cell>(ptr); // By AMXMODX Dev Team
}

/* 3 param */
NATIVE(get_cvar_string) {
  int ilen;
  char* sptemp = get_amxstring(amx, params[1], 0, ilen);
  return set_amxstring(amx, params[2], CVAR_GET_STRING(sptemp), params[3]);
}

/* 1 param */
NATIVE(get_cvar_float) {
  int ilen;
  float pFloat = CVAR_GET_FLOAT(get_amxstring(amx, params[1], 0, ilen));
  return FloatToCell(pFloat);
}

/* 1 param */
NATIVE(get_cvar_num) {
  int ilen;
  return (int)CVAR_GET_FLOAT(get_amxstring(amx, params[1], 0, ilen));
}

/* 3 param */
NATIVE(get_cvarptr_string) {
  cvar_t* ptr = reinterpret_cast<cvar_t *>(params[1]); // By AMXMODX Dev Team
  return set_amxstring(amx, params[2], ptr ? (ptr->string ? ptr->string : "") : "", params[3]);
}

/* 1 param */
NATIVE(get_cvarptr_float) {
  cvar_t* ptr = reinterpret_cast<cvar_t *>(params[1]); // By AMXMODX Dev Team
  if(!ptr) {
    return 0;
  }
  return FloatToCell((float)ptr->value);
}

/* 1 param */
NATIVE(get_cvarptr_num) {
  cvar_t* ptr = reinterpret_cast<cvar_t *>(params[1]); // By AMXMODX Dev Team
  if(!ptr) {
    return 0;
  }
  return (int)ptr->value;
}

/* 2 param */
NATIVE(set_cvar_string) {
  int ilen;
  char* sptemp = get_amxstring(amx, params[1], 0, ilen);
  char* szValue = get_amxstring(amx, params[2], 1, ilen);
  CVAR_SET_STRING(sptemp, szValue);
  return 1;
}

/* 2 param */
NATIVE(set_cvar_float) {
  int ilen;
  CVAR_SET_FLOAT(get_amxstring(amx, params[1], 0, ilen), CellToFloat(params[2]));
  return 1;
}

/* 2 param */
NATIVE(set_cvar_num) {
  int ilen;
  CVAR_SET_FLOAT(get_amxstring(amx, params[1], 0, ilen), (float)params[2]);
  return 1;
}

/* 2 param */
NATIVE(set_cvarptr_string) {
	cvar_t *ptr = reinterpret_cast<cvar_t *>(params[1]); // By AMXMODX Dev Team
	if(!ptr) {
    return 0;
  }
	int ilen;
	(*g_engfuncs.pfnCvar_DirectSet)(ptr, get_amxstring(amx, params[2], 0, ilen)); // By AMXMODX Dev Team
	return 1;
}

/* 2 param */
NATIVE(set_cvarptr_float) {
  cvar_t* ptr = reinterpret_cast<cvar_t *>(params[1]); // By AMXMODX Dev Team
  if(!ptr) {
    return 0;
  }
  ptr->value = CellToFloat(params[2]);
  return 1;
}

/* 2 param */
NATIVE(set_cvarptr_num) {
  cvar_t* ptr = reinterpret_cast<cvar_t *>(params[1]); // By AMXMODX Dev Team
  if(!ptr) {
    return 0;
  }
  ptr->value = float(params[2]);
  return 1;
}

/* 1 param */
NATIVE(log_message) {
  g_translator.setDest(0);
  int len;
  char* message =  format_amxstring(amx, params, 1, len);
  message[len++]='\n';
  message[len]=0;
  ALERT(at_logged, "%s", message);
  return len;
}

/* 1 param */
NATIVE(log_to_file) {
  g_translator.setDest(0);
  int ilen; int len;
  char* file = get_amxstring(amx, params[1], 0, ilen);
  char* msg = format_amxstring(amx, params, 2, len);
  msg[len++]='\n';
  msg[len]=0;
  amx_log(file, msg);
  return 1;
}

/* 3 param */
NATIVE(num_to_word) {
  char sptemp[512];
  UTIL_IntToString(params[1], sptemp);
  return set_amxstring(amx, params[2], sptemp, params[3]);
}

NATIVE(get_timeleft) {
  float flCvarTimeLimit = mp_timelimit->value;
  if(flCvarTimeLimit) {
    int iReturn = (int)((g_game_timeleft + flCvarTimeLimit * 60.0) - gpGlobals->time);
    return (iReturn < 0) ? 0 : iReturn;
  }
  return 0;
}

/* 3 param */
NATIVE(get_time) {
  int ilen;
  char* sptemp = get_amxstring(amx, params[1], 0, ilen);
  time_t td = time(NULL);
  tm* lt = localtime(&td);
  if(lt == 0) {
    return 0;
  }
  char szDate[512];
  strftime(szDate, 511, sptemp, lt);
  return set_amxstring(amx, params[2], szDate, params[3]);
}

/* 3 param */
NATIVE(format_time) {
  int ilen;
  char* sptemp = get_amxstring(amx, params[3], 0, ilen);
  time_t tim = params[4];
  time_t td = (tim != -1) ? tim : time(NULL);
  tm* lt = localtime(&td);
  if(lt == 0) {
    return 0;
  }
  char szDate[512];
  strftime(szDate, 511, sptemp, lt);
  return set_amxstring(amx, params[1], szDate, params[2]);
}

/* 3 param */
NATIVE(parse_time) {
  int ilen;
  char* sTime = get_amxstring(amx,params[1],1,ilen);
  char* sFormat = get_amxstring(amx,params[2],0,ilen);
  tm* mytime;
  time_t td;
  if(params[3] == -1) {
    td = time(NULL);
    mytime = localtime(&td);
    if(mytime == 0) {
      return 0;
    }
    strptime(sTime, sFormat, mytime, 0);
  }
  else {
    td = params[3];
    mytime = localtime(&td);
    if(mytime == 0){
      return 0;
    }
    strptime(sTime, sFormat, mytime, 1);
  }
  return mktime(mytime);
}

/* 3 param */
NATIVE(get_systime) {
  time_t td = time(NULL);
  td += params[1];
  return td;
}

/* 0 param */
NATIVE(read_datanum) {
  return g_events.getArgNum() + 1;
}

/* 3 param */
NATIVE(read_data) {
  switch(*params/sizeof(cell)) {
    case 1: return g_events.getArgInteger(params[1]);
    case 3: return set_amxstring(amx, params[2], g_events.getArgString(params[1]), *get_amxaddr(amx, params[3]));
    default:
      cell *fCell = get_amxaddr(amx, params[2]);
      float *pFloat = (float *)((void *)fCell);
      *pFloat = g_events.getArgFloat(params[1]);
      return (int)(*pFloat);
  }
}

NATIVE(get_playersnum) {
  if(!params[1]) return g_players_num;

  int a = 0;
  for(int i = 1; i <= gpGlobals->maxClients; ++i) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(i);
    if(pPlayer->initialized && (GETPLAYERUSERID(pPlayer->pEdict) > 0)) ++a;
  }
  return a;
}

/* 4 param */
NATIVE(get_players) {
  int iNum = 0;
  int ilen;
  char* sptemp = get_amxstring(amx, params[3], 0, ilen);
  int flags = UTIL_ReadFlags(sptemp);

  cell *aPlayers = get_amxaddr(amx, params[1]);
  cell *iMax = get_amxaddr(amx, params[2]);

  int team = 0;

  if(flags & 48) {
    sptemp = get_amxstring(amx, params[4], 0, ilen);

    if(flags & 16) {
      int c = 0;
      while(c < ilen && isdigit(sptemp[c])) ++c;

      if(c == ilen) team = atoi(sptemp);
      else if(flags & 64) {
        if(g_bmod_dod) {
          if(!strcmpi(sptemp, "Allies")) team = 1;
          else if(!strcmpi(sptemp, "Axis")) team = 2;
        }
        else team = g_teamsIds.findTeamId(sptemp);
      }
      else {
        if(g_bmod_dod) {
          if(!strcmp(sptemp, "Allies")) team = 1;
          else if(!strcmp(sptemp, "Axis")) team = 2;
        }
        else team = g_teamsIds.findTeamIdCase(sptemp);
      }
    }
  }

  for(int i = 1; i <= gpGlobals->maxClients; ++i) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(i);
    if(pPlayer->ingame) {
      if(pPlayer->IsAlive() ? (flags & 2) : (flags & 1)) continue;
      if(pPlayer->IsBot() ? (flags & 4) : (flags & 8)) continue;
      if((flags & 16) && ((g_bmod_dod && pPlayer->pEdict->v.team != team) || (!g_bmod_dod && pPlayer->teamId != team))) {
        continue;
      }
      if ((flags & 128) && (pPlayer->pEdict->v.flags & FL_PROXY)) continue;
      /*if(flags & 16) {
        if(flags & 64) {
          if(strcmpi(pPlayer->team.str(), sptemp)) continue;
        }
        else if(strcmp(pPlayer->team.str(), sptemp)) continue;
      }*/
      if(flags & 32) {
        if(flags & 64) {
          if(stristr(pPlayer->name.str(), sptemp) == NULL) continue;
        }
        else if(strstr(pPlayer->name.str(), sptemp) == NULL) continue;
      }
      aPlayers[iNum++] = i;
    }
  }

  *iMax = iNum;
  return 1;
}

/* 1 param */
NATIVE(find_player) {
  typedef int (*STRCOMPARE)(const char*, const char*);
  STRCOMPARE func; // By AMXMODX Dev Team

  int ilen, userid = 0;
  char* sptemp = get_amxstring(amx, params[1], 0, ilen);
  int flags = UTIL_ReadFlags(sptemp);
  if(flags & 31) sptemp = get_amxstring(amx, params[2], 0, ilen);
  else if(flags & 1024) userid = *get_amxaddr(amx, params[2]);

  // a b c d e f g h i j k l
  int result = 0;

  if(flags & 2048)
    func = strcmpi;
  else
    func = strcmp;

  for(int i = 1; i <= gpGlobals->maxClients; ++i) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(i);
    if(pPlayer->ingame) {
      if(pPlayer->IsAlive() ? (flags & 64) : (flags & 32)) continue;
      if(pPlayer->IsBot() ? (flags & 128) : (flags & 256)) continue;
      if(flags & 1) {
        if((func)(pPlayer->name.str(), sptemp)) continue;
      }
      if(flags & 2) {
        if(flags & 2048) {
          if(stristr(pPlayer->name.str(), sptemp) == NULL) continue;
        }
        else if(strstr(pPlayer->name.str(), sptemp) == NULL) continue;
      }
      if(flags & 4) {
        const char* authid = GETPLAYERAUTHID(pPlayer->pEdict);
        if(!authid || (func)(authid, sptemp)) continue;
      }
      if(flags & 1024) {
        if(userid != GETPLAYERUSERID(pPlayer->pEdict)) continue;
      }
      if(flags & 8) {
        if(strncmp(pPlayer->ip.str(), sptemp, ilen)) continue;
      }
      if(flags & 16) {
        if((func)(pPlayer->team.str(), sptemp)) continue;
      }
      result = i;
      if((flags & 512) == 0) break;
    }
  }
  return result;
}

NATIVE(get_maxplayers) {
  return gpGlobals->maxClients;
}

NATIVE(get_gametime) {
  float pFloat = gpGlobals->time;
  return FloatToCell(pFloat);
}

/* 2 param */
NATIVE(get_mapname) {
  return set_amxstring(amx, params[1], STRING(gpGlobals->mapname), params[2]);
}

/* 2 param */
NATIVE(get_modname) {
  return set_amxstring(amx, params[1], g_mod_name.str(), params[2]);
}

/* 3 param */
NATIVE(get_localinfo) {
  int ilen;
  char* sptemp = get_amxstring(amx, params[1], 0, ilen);
  return set_amxstring(amx, params[2], LOCALINFO(sptemp), params[3]);
}

/* 2 param */
NATIVE(set_localinfo) {
  int ilen;
  char* sptemp = get_amxstring(amx, params[1], 0, ilen);
  char* szValue = get_amxstring(amx, params[2], 1, ilen);
  SET_LOCALINFO(sptemp, szValue);
  return 1;
}

/* 4 param */
NATIVE(get_user_info) {
  int index = params[1];
  if(is_player(index, 0, amx)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if(!pPlayer->pEdict) {
      amx_logerror(amx, AMX_ERR_NATIVE, "Player %d is not connected", index);
      return 0;
    }
    int ilen;
    char* sptemp = get_amxstring(amx, params[2], 0, ilen);
    return set_amxstring(amx, params[3], ENTITY_KEYVALUE(pPlayer->pEdict, sptemp), params[4]);
  }
  return 1;
}

/* 3 param */
NATIVE(set_user_info) {
  int index = params[1];
  if(is_player(index, 0, amx)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if(!pPlayer->pEdict){
      amx_logerror(amx, AMX_ERR_NATIVE, "Player %d is not connected", index);
      return 0;
    }
    int ilen;
    char* sptemp = get_amxstring(amx, params[2], 0, ilen);
    char* szValue = get_amxstring(amx, params[3], 1, ilen);
    ENTITY_SET_KEYVALUE(pPlayer->pEdict, sptemp, szValue);
  }
  return 1;
}

NATIVE(read_argc) {
  return CMD_ARGC();
}

/* 3 param */
NATIVE(read_argv) {
  return set_amxstring(amx, params[2], CMD_ARGV(params[1]), params[3]);
}

/* 2 param */
NATIVE(read_args) {
  const char* sValue = CMD_ARGS();
  return set_amxstring(amx,params[1], sValue ? sValue : "", params[2]);
}

/* 1 param */
NATIVE(get_user_msgid) {
  int ilen;
  char* sptemp = get_amxstring(amx, params[1], 0, ilen);
  return GET_USER_MSG_ID(PLID, sptemp, NULL);
}

/* 3 params */
NATIVE(get_user_msgname) {
  const char* msgname = GET_USER_MSG_NAME(PLID, params[1], NULL);
  if(msgname) return set_amxstring(amx, params[2], msgname, params[3]);

  return 0;
}

/* 2 param */
NATIVE(set_task) {
  CPluginMngr::CPlugin *plugin = g_plugins.findPluginFast(amx);
  int a, iFunc;
  char* stemp = get_amxstring(amx,params[2],1, a );

  if(amx_FindPublic(amx, stemp , &iFunc) != AMX_ERR_NONE) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Function is not present (function \"%s\")", stemp);
    return 0;
  }

  float base = CellToFloat(params[1]);
  if(base < 0.1f) base = 0.1f;

  char* temp = get_amxstring(amx, params[6], 0, a);

  g_tasksMngr.registerTask(plugin, iFunc, UTIL_ReadFlags(temp), params[3], base,
    gpGlobals->time + base, params[5], get_amxaddr(amx,params[4]), params[7]);

  return 1;
}

/* 1 param */
NATIVE(remove_task) {
  return g_tasksMngr.removeTasks(params[1], params[2] ? 0 : amx);
}

/* 3 param */
NATIVE(change_tasktime) {
  float new_time = CellToFloat(params[2]);
  return g_tasksMngr.changeTasks(params[1], params[3] ? 0 : amx, new_time);
}

/* 1 param */
NATIVE(task_exists) {
  return g_tasksMngr.taskExists(params[1], params[2] ? 0 : amx);
}

/* 1 param */
NATIVE(cvar_exists) {
  int ilen;
  return (CVAR_GET_POINTER(get_amxstring(amx, params[1], 0, ilen)) ? 1 : 0);
}

/* 3 param */
NATIVE(register_cvar) {
  int i;
  char* temp = get_amxstring(amx, params[1], 0, i);

  if(!g_cvars.find(temp)) {
    CPluginMngr::CPlugin *plugin = g_plugins.findPluginFast(amx);
    CCVar* cvar = new CCVar(temp, plugin->getName(), params[3], CellToFloat(params[4]));
    if(cvar == 0) return 0;

    g_cvars.put(cvar);

    if(CVAR_GET_POINTER(temp) == 0) {
      static cvar_t cvar_reg_helper;
      cvar_reg_helper = *(cvar->getCvar());
      CVAR_REGISTER(&cvar_reg_helper);
    }
    CVAR_SET_STRING(temp, get_amxstring(amx, params[2], 1, i));
    return reinterpret_cast<cell>(CVAR_GET_POINTER(temp)); // By AMXMODX Dev Team
  }
  return reinterpret_cast<cell>(CVAR_GET_POINTER(temp)); // By AMXMODX Dev Team
}

/* 3 param */
NATIVE(get_user_ping) {
  int index = params[1];
  if(is_player(index, 1, amx)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    cell *cpPing = get_amxaddr(amx, params[2]);
    cell *cpLoss = get_amxaddr(amx, params[3]);
    int ping, loss;
    PLAYER_CNX_STATS(pPlayer->pEdict, &ping, &loss);
    *cpPing = ping;
    *cpLoss = loss;
  }
  return 1;
}

/* 1 param */
NATIVE(get_user_time) {
  int index = params[1];
  if(is_player(index, 1, amx)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    int time = (int)(gpGlobals->time - (params[2] ? pPlayer->playtime : pPlayer->time));
    return time;
  }
  return 0;
}

NATIVE(server_exec) {
  SERVER_EXECUTE();
  return 1;
}

/* 4 param */
NATIVE(engclient_cmd) {
  int ilen;
  const char* szCmd = get_amxstring(amx, params[2], 0, ilen);
  const char* sArg1 = get_amxstring(amx, params[3], 1, ilen);
  if(ilen == 0) sArg1 = 0;

  const char* sArg2 = get_amxstring(amx, params[4], 2, ilen);
  if(ilen == 0) sArg2 = 0;

  if(params[1] == 0) {
    for(int i = 1; i <= gpGlobals->maxClients; ++i) {
      CPlayer* pPlayer = GET_PLAYER_POINTER_I(i);
      if(pPlayer->ingame) UTIL_FakeClientCommand(pPlayer->pEdict, szCmd, sArg1, sArg2);
    }
  }
  else {
    int index = params[1];
    if(is_player(index, 1, amx)) {
      CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
      UTIL_FakeClientCommand(pPlayer->pEdict, szCmd, sArg1, sArg2);
    }
  }
  return 1;
}

/* 3 param */
NATIVE(pause) {
  int ilen;
  char* temp = get_amxstring(amx, params[1], 0, ilen);
  int flags = UTIL_ReadFlags(temp);

  CPluginMngr::CPlugin *plugin = 0;

  if(flags & 2) { // pause function
    if(flags & 4) { // look outside the plugin
      temp = get_amxstring(amx, params[3], 0, ilen);
      plugin = g_plugins.findPlugin(temp);
    }
    else plugin = g_plugins.findPluginFast(amx);
    if(!plugin) return 0; // plugin not found

    temp = get_amxstring(amx, params[2], 0, ilen);
    int err, index;
    if((err = amx_FindPublic(plugin->getAMX(), temp, &index))!= AMX_ERR_NONE) {
      amx_logerror(amx, AMX_ERR_NONE, "Function is not present (function \"%s\") (plugin \"%s\")\n", temp, plugin->getName());
      return 0;
    }
    plugin->pauseFunction(index);
    return 1;
  }
  else if(flags & 4) {
    temp = get_amxstring(amx, params[2], 0, ilen);
    plugin = g_plugins.findPlugin(temp);
  }
  else plugin = g_plugins.findPluginFast(amx);

  if(plugin && plugin->isValid()) {
    if(flags & 8) plugin->setStatus(ps_stopped);
    else if(flags & 16) plugin->setStatus(ps_locked);
    else plugin->pausePlugin();
    return 1;
  }
  return 0;
}

/* 3 param */
NATIVE(unpause) {
  int ilen;
  char* sptemp = get_amxstring(amx, params[1], 0, ilen);
  int flags = UTIL_ReadFlags(sptemp);
  CPluginMngr::CPlugin *plugin = 0;
  if(flags & 2) {
    if(flags & 4) {
      sptemp = get_amxstring(amx, params[3], 0, ilen);
      plugin = g_plugins.findPlugin(sptemp);
    }
    else plugin = g_plugins.findPluginFast(amx);

    if(!plugin) return 0;

    sptemp = get_amxstring(amx, params[2], 0, ilen);
    int err, index;
    if((err = amx_FindPublic(plugin->getAMX(), sptemp, &index))!= AMX_ERR_NONE) {
      amx_logerror(amx, AMX_ERR_NONE, "Function is not present (function \"%s\") (plugin \"%s\")\n", sptemp, plugin->getName());
      return 0;
    }
    plugin->unpauseFunction(index);
    return 1;
  }
  else if(flags & 4) {
    sptemp = get_amxstring(amx, params[2], 0, ilen);
    plugin = g_plugins.findPlugin(sptemp);
  }
  else plugin = g_plugins.findPluginFast(amx);

  if(plugin && plugin->isValid()) {
    plugin->unpausePlugin();
    return 1;
  }
  return 0;
}

/* 1 param */
NATIVE(read_flags) {
  int ilen;
  char* sptemp = get_amxstring(amx, params[1], 0, ilen);
  return UTIL_ReadFlags(sptemp);
}

/* 1 param */
NATIVE(get_flags) {
  char flags[32];
  UTIL_GetFlags(flags, params[1]);
  return set_amxstring(amx, params[2], flags, params[3]);
}

/* 2 param */
NATIVE(get_user_flags) {
  int index = params[1];
  if(index < 0 || index > gpGlobals->maxClients) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid player id %d", index);
    return 0;
  }
  int id = params[2];
  if(id < 0) id = 0;
  if(id > 31) id = 31;

  return GET_PLAYER_POINTER_I(index)->flags[id];
}

/* 3 param */
NATIVE(set_user_flags) {
  int index = params[1];
  if(index < 0 || index > gpGlobals->maxClients) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid player id %d", index);
    return 0;
  }
  CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
  int flag = params[2];
  int id = params[3];
  if(id < 0) id = 0;
  if(id > 31) id = 31;

  pPlayer->flags[id] |= flag;
  return 1;
}

/* 3 param */
NATIVE(remove_user_flags) {
  int index = params[1];
  if(index < 0 || index > gpGlobals->maxClients) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid player id %d", index);
    return 0;
  }
  CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
  int flag = params[2];
  int id = params[3];
  if(id < 0) id = 0;
  if(id > 31) id = 31;

  pPlayer->flags[id] &= ~flag;
  return 1;
}

/* 1 param */
NATIVE(register_menuid) {
  int i;
  char* temp = get_amxstring(amx, params[1], 0, i);
  AMX* a = (*params/sizeof(cell) < 2 || params[2]) ? 0 : amx;
  return g_menucmds.registerMenuId(temp, a);
}

/* 3 param */
NATIVE(get_user_menu) {
  int index = params[1];
  if(is_player(index, 1, amx)) {
    cell *cpMenu = get_amxaddr(amx, params[2]);
    cell *cpKeys = get_amxaddr(amx, params[3]);
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    *cpMenu = pPlayer->menu;
    *cpKeys = pPlayer->keys;
  }
  return 1;
}

/* 1 param */
NATIVE(precache_sound) {
  if(g_dontprecache) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Cannot call this native outside of plugin_precache()");
    return 0;
  }
  int ilen;
  char* sptemp = get_amxstring(amx, params[1], 0, ilen);
  PRECACHE_SOUND((char*)STRING(ALLOC_STRING(sptemp)));
  return 1;
}

/* 1 param */
NATIVE(precache_model) {
  if(g_dontprecache) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Cannot call this native outside of plugin_precache()");
    return 0;
  }
  int ilen;
  char* sptemp = get_amxstring(amx, params[1], 0, ilen);
  return PRECACHE_MODEL((char*)STRING(ALLOC_STRING(sptemp)));
}

/* 1 param */
NATIVE(precache_generic) {
  if(g_dontprecache) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Cannot call this native outside of plugin_precache()");
    return 0;
  }
  int ilen;
  char* sptemp = get_amxstring(amx, params[1], 0, ilen);
  return PRECACHE_GENERIC((char*)STRING(ALLOC_STRING(sptemp)));
}

/* 2 param */
NATIVE(random_float) {
  float one = CellToFloat(params[1]);
  float two = CellToFloat(params[2]);
  float fRnd = RANDOM_FLOAT(one,two);
  return FloatToCell(fRnd);
}

/* 2 param */
NATIVE(random_num) {
  return RANDOM_LONG(params[1], params[2]);
}

/* 1 param */
NATIVE(remove_quotes) {
  cell *text = get_amxaddr(amx, params[1]);
  if(*text == '\"') {
    register cell *temp = text;
    int len = 0;
    while(*temp++) ++len; // get length

    cell *src = text;
    if(src[len - 1] == '\r') src[--len] = 0;

    if(src[--len] == '\"') {
      src[len] = 0;
      temp = src + 1;
      while((*src++ = *temp++));
      return 1;
    }
  }
  return 0;
}

/* 4 param */
NATIVE(get_user_aiming) {
  int index = params[1];
  cell *cpId = get_amxaddr(amx, params[2]);
  cell *cpBody = get_amxaddr(amx, params[3]);
  float pFloat = 0.0f; // By AMXMODX Dev Team
  if(is_player(index, 1, amx)) {
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    edict_t* edict = pPlayer->pEdict;
    Vector v_forward;
    Vector v_src = edict->v.origin + edict->v.view_ofs;
    ANGLEVECTORS(edict->v.v_angle, v_forward, NULL, NULL);
    TraceResult trEnd;
    Vector v_dest = v_src + v_forward * params[4];
    TRACE_LINE(v_src, v_dest, 0, edict, &trEnd);
    *cpId = FNullEnt(trEnd.pHit) ? 0 : ENTINDEX(trEnd.pHit);
    *cpBody = trEnd.iHitgroup;
    if(trEnd.flFraction < 1.0) {
      pFloat = (trEnd.vecEndPos - v_src).Length();
    }
    return FloatToCell(pFloat);
  }
  *cpId = 0;
  *cpBody = 0;
  return FloatToCell(pFloat); // By AMXMODX Dev Team
}

NATIVE(remove_cvar_flags) {
  int ilen;
  char* sCvar = get_amxstring(amx, params[1], 0, ilen);
  if(!strcmp(sCvar, "amx_version") || !strcmp(sCvar, "sv_cheats")) {
    return 0;
  }
  cvar_t* pCvar = CVAR_GET_POINTER(sCvar);
  if(pCvar) {
    pCvar->flags &= ~((int)(params[2]));
    return 1;
  }
  return 0;
}

NATIVE(get_cvar_flags) {
  int ilen;
  char* sCvar = get_amxstring(amx, params[1], 0, ilen);
  cvar_t* pCvar = CVAR_GET_POINTER(sCvar);
  return pCvar ? pCvar->flags : 0;
}

NATIVE(get_cvarptr_flags) {
  cvar_t* ptr = reinterpret_cast<cvar_t *>(params[1]); // By AMXMODX Dev Team
  if(!ptr) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid Cvar pointer");
    return 0;
  }
  return ptr->flags;
}

NATIVE(set_cvar_flags) {
  int ilen;
  char* sCvar = get_amxstring(amx, params[1], 0, ilen);
  if(!strcmp(sCvar, "amx_version") || !strcmp(sCvar, "sv_cheats")) {
    return 0;
  }
  cvar_t* pCvar = CVAR_GET_POINTER(sCvar);
  if(pCvar) {
    pCvar->flags |= (int)(params[2]);
    return 1;
  }
  return 0;
}

NATIVE(set_cvarptr_flags) {
  cvar_t* ptr = reinterpret_cast<cvar_t *>(params[1]); // By AMXMODX Dev Team
  if(!ptr) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Invalid Cvar pointer");
    return 0;
  }
  if(!strcmp(ptr->string, "amx_version") || !strcmp(ptr->string, "sv_cheats")) {
    return 0;
  }
  ptr->flags |= (int)(params[2]);
  return 1;
}

NATIVE(force_unmodified) {
  int a;
  cell *cpVec1 = get_amxaddr(amx, params[2]);
  cell *cpVec2 = get_amxaddr(amx, params[3]);
  Vector vec1 = Vector((float)cpVec1[0], (float)cpVec1[1], (float)cpVec1[2]);
  Vector vec2 = Vector((float)cpVec2[0], (float)cpVec2[1], (float)cpVec2[2]);
  char* filename = get_amxstring(amx, params[4], 0, a);

  ForceObject* aaa = new ForceObject(filename, (FORCE_TYPE)((int)(params[1])), vec1, vec2, amx);

  if(aaa) {
    if(stristr(filename, ".wav")) g_forcesounds.put(aaa);
    else if(stristr(filename, ".mdl")) g_forcemodels.put(aaa);
    else g_forcegeneric.put(aaa);
    return 1;
  }
  return 0;
}

NATIVE(read_logdata) {
  return set_amxstring(amx, params[1], g_logevents.getLogString(), params[2]);
}

NATIVE(read_logargc) {
  return g_logevents.getLogArgNum();
}

NATIVE(read_logargv) {
  return set_amxstring(amx, params[2], g_logevents.getLogArg(params[1]), params[3]);
}

NATIVE(parse_loguser) {
  int len;
  char *text = get_amxstring(amx, params[1], 0, len);
  if(len < 6) { // no user to parse!?
    amx_logerror(amx, AMX_ERR_NATIVE, "No user name specified");
    return 0;
  }
/******** GET TEAM **********/
  char* end = text + --len;
  *end = 0;
  while(*end != '<' && len--) --end;

  ++end;
  cell *cPtr = get_amxaddr(amx, params[7]);
  int max = params[8]; // get TEAM
//  print_srvconsole("Got team: %s (Len %d)\n",end,len);
  while(max-- && *end) *cPtr++ = *end++;

  *cPtr = 0;
/******** GET AUTHID **********/
  if(len <= 0) {
    amx_logerror(amx, AMX_ERR_NATIVE, "No authid found");
    return 0;
  }
  end = text + --len;
  *end = 0;
  while(*end != '<' && len--) --end;

  ++end;
  cPtr = get_amxaddr(amx, params[5]);
  max = params[6]; // get AUTHID
 // print_srvconsole("Got auth: %s (Len %d)\n",end,len);
  while(max-- && *end) *cPtr++ = *end++;

  *cPtr = 0;
/******** GET USERID **********/
  if(len <= 0) {
    amx_logerror(amx, AMX_ERR_NATIVE, "No userid found");
    return 0;
  }
  end = text + --len;
  *end = 0;
  while(*end != '<' && len--) --end;

//  print_srvconsole("Got userid: %s (Len %d)\n",end + 1,len);
  if(*(cPtr = get_amxaddr(amx, params[4])) != -2) *cPtr = atoi(end + 1);
/******** GET NAME **********/
  *end = 0;
  cPtr = get_amxaddr(amx, params[2]);
  max = params[3]; // get NAME
//  print_srvconsole("Got name: %s (Len %d)\n",text,len);
  while(max-- && *text) *cPtr++ = *text++;

  *cPtr = 0;
  return 1;
}

NATIVE(register_logevent) {
  CPluginMngr::CPlugin *plugin = g_plugins.findPluginFast(amx);
  int a, iFunc;
  char* temp = get_amxstring(amx, params[1], 0, a);

  if(amx_FindPublic(amx, temp, &iFunc) != AMX_ERR_NONE) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Function is not present (function \"%s\")", temp);
    return 0;
  }

  LogEventsMngr::CLogEvent* r = g_logevents.registerLogEvent(plugin, iFunc, params[2]);

  if(r == 0) return 0;

  int numparam = *params/sizeof(cell);
  for(int i = 3; i <= numparam; ++i) {
    r->registerFilter(get_amxstring(amx, params[i], 0, a));
  }
  
  if(g_pengfuncsTable_Post->pfnAlertMessage == NULL) {
    g_pengfuncsTable_Post->pfnAlertMessage = AlertMessage_Post;
  }
  
  return 1;
}

NATIVE(is_module_running) {
  int l;
  char* module = get_amxstring(amx, params[1], 0, l);
  CList<CModule>::iterator a = g_modules.begin();

  int id = 1;

  while(a) {
    if(!stricmp(module, (*a).getName())) {
      if((*a).getStatusValue() == MODULE_LOADED) return id;
      else return 0;
    }
    ++id;
    ++a;
  }
  return 0;
}

NATIVE(is_plugin_running) {
  g_translator.setDest(101);

  int l;
  char* plugin = get_amxstring(amx, params[1], 0, l);
  CPluginMngr::iterator a = g_plugins.begin();

  while(a) {
    if(!stricmp(plugin, (*a).getName()) || !stricmp(plugin, (*a).getTitle())) {
      if(!stricmp((*a).getStatus(), "running")) return (*a).getId() + 1;
      else return 0;
    }
    ++a;
  }
  return 0;
}

NATIVE(is_running_jit) {
#if defined JIT
  return 1;
#elif defined JITORNOT
  return (amx->usejit ? 1 : 0);
#else
  return 0;
#endif
}

NATIVE(is_translated) {
  int a;
  CTranslator::CDictionary *d = g_plugins.findPluginFast(amx)->getDictionary();
  return (d->getStringId(get_amxstring(amx, params[1], 0, a)) != -1) ? 1 : 0;
}

NATIVE(load_translations) {
  int i;
  CPluginMngr::CPlugin* a = g_plugins.findPluginFast(amx);
  if(a->getDictionary()) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Translator: only one translations file can be precached");
    return 0;
  }
  a->setDictionary(g_translator.loadDictionary(get_amxstring(amx, params[1], 0, i)));
  if(!a->getDictionary()->getTranslations()) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Translator: cannot load translations");
    return 0;
  }
  a->setML();
  return 1;
}

NATIVE(translate) {
  int a;
  int destid = params[2];
  
  int forcelang;
  if((*params / sizeof(cell)) == 2) {
    forcelang = -1;
  }
  else {
    forcelang = params[3];
  }
  
  if(forcelang < -1 || forcelang >= g_translator.getLangs()) {
    amx_logerror(amx, AMX_ERR_NATIVE, "Translator: forcelang parameter is incorrect");
    return false;
  }
  
  if(forcelang != -1) {
    int s = g_translator.getLangSupport(forcelang);
    if(!s) {
      amx_logerror(amx, AMX_ERR_NATIVE, "Translator: forcelang parameter matches a language that has no support");
      return false;
    }
  }
  
  CTranslator::CDictionary *d = g_plugins.findPluginFast(amx)->getDictionary();

  if(!d) {
    CPluginMngr::CPlugin* p = g_plugins.findPluginFast(amx);
    amx_logerror(amx, AMX_ERR_NATIVE, "Translator: no translations file loaded, deactivating plugin");
    p->setStatus(ps_locked);
    return false;
  }
  return d->getTranslation(get_amxstring(amx, params[1], 0, a), destid, forcelang);
}

NATIVE(get_languagesnum) {
  return g_translator.getLangs();
}

NATIVE(get_language) {
  return set_amxstring(amx, params[2], g_translator.getLang(params[1]), params[3]);
}

NATIVE(is_language_known) {
  int a;
  return g_translator.getLangId(get_amxstring(amx, params[1], 0, a)) != -1 ? 1 : 0;
}

NATIVE(get_user_language) {
  int index = params[1];
  const char *l;

  if(index) {
    if(index > gpGlobals->maxClients) {
      amx_logerror(amx, AMX_ERR_NATIVE, "Invalid user id %d", index);
      return 0;
    }
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if(!params[4]) l = g_translator.getLang(pPlayer->langid);
    else l = g_translator.getLang(pPlayer->langid2);
  }
  else {
    if(!params[4]) l = g_translator.getLang(g_translator.getServerLang());
    else l = g_translator.getLang(0);
  }
  set_amxstring(amx, params[2], l, params[3]);
  return 1;
}

NATIVE(get_user_languagenum) {
  int index = params[1];

  if(index) {
    if(index > gpGlobals->maxClients) {
      amx_logerror(amx, AMX_ERR_NATIVE, "Invalid user id %d", index);
      return 0;
    }
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if(!params[2]) return pPlayer->langid;
    else return pPlayer->langid2;
  }
  else {
    if(!params[2]) return g_translator.getServerLang();
    return 0;
  }
}

NATIVE(set_user_languagenum) {
  int index = params[1];
  int lang = params[2];
  if(lang < 0 || lang >= g_translator.getLangs()) return 0;

  if(index) {
    if(index > gpGlobals->maxClients) {
      amx_logerror(amx, AMX_ERR_NATIVE, "Invalid user id %d", index);
      return 0;
    }
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if(!params[3]) pPlayer->langid = lang;
    else pPlayer->langid2 = lang;
  }
  else {
    if(!params[3]) g_translator.setServerLang(lang);
    else return 0;
  }
  return 1;
}

NATIVE(set_user_language) {
  int a;
  char *l = get_amxstring(amx, params[2], 0, a);
  int id = g_translator.getLangId(l);
  int index = params[1];
  if(id == -1) return 0;

  if(index) {
    if(index > gpGlobals->maxClients) {
      amx_logerror(amx, AMX_ERR_NATIVE, "Invalid user id", index);
      return 0;
    }
    CPlayer* pPlayer = GET_PLAYER_POINTER_I(index);
    if(!params[3]) pPlayer->langid = id;
    else pPlayer->langid2 = id;
    return 1;
  }
  if(params[3]) return 0;

  return g_translator.setServerLang(id) ? 1 : 0;
}

NATIVE(get_languagenum_support) {
  int id = params[1];
  int s;
  float pFloat = 0.0f;

  if(id >= 0 && id < g_translator.getLangs()) {
    s = g_translator.getLangSupport(id);
    if(s) {
      pFloat = (float)g_translator.getLangSupport(id) / (float)g_translator.getLangSupport(0);
    }
  }
  return FloatToCell(pFloat);
}

NATIVE(get_language_support) {
  int s;
  char *l = get_amxstring(amx, params[1], 0, s);
  int id = g_translator.getLangId(l);
  float pFloat = 0.0f;

  if(id != -1) {
    s = g_translator.getLangSupport(id);
    if(s) {
      pFloat = (float)g_translator.getLangSupport(id) / (float)g_translator.getLangSupport(0);
    }
  }
  return FloatToCell(pFloat);
}

NATIVE(rpt_error) {
  g_translator.setDest(0);
  int len;
  char *error = format_amxstring(amx, params, 1, len);
  amx_logerror(amx, AMX_ERR_NATIVE, error);
  return 1;
}

NATIVE(query_client_cvar) {
  if(g_engfuncs.pfnQueryClientCvarValue2) {
    int index = params[1];
    if(is_player(index, 0, amx)) {
      CPlayer *pPlayer = GET_PLAYER_POINTER_I(index);
      if(!pPlayer->IsBot()) {
        int len, iFunc;
        const char *cvar = get_amxstring(amx, params[2], 0, len);
        const char *function = get_amxstring(amx, params[3], 1, len);

        if(amx_FindPublic(amx, function , &iFunc) != AMX_ERR_NONE) {
          amx_logerror(amx, AMX_ERR_NATIVE, "Function is not present (function \"%s\")", function);
          return 0;
        }

        cvarQuery* query = new cvarQuery;
        query->amx = amx;
        query->functionToExec = iFunc;
        query->requestId = MAKE_REQUESTID(PLID);

        pPlayer->cvar_queries.put(query);

        QUERY_CLIENT_CVAR_VALUE2(pPlayer->pEdict, cvar, query->requestId);
      }
    }
  }
  return 1;
}

NATIVE(arrayset) {
  cell value = params[2];
  if(value == 0) {
    memset(get_amxaddr(amx, params[1]), 0, params[3] * sizeof(cell)); // By AMXMODX Dev Team
  }
  else {
    cell* array = get_amxaddr(amx, params[1]);
    int size = params[3];
    while(size-- > 0) {
      *array++ = value; 
    }      
  }
  return 1;
}

/* 1 param */
NATIVE(load_module) {
  if(amx->flags & AMX_FLAG_INNATCHK) {
    int len;
    const char* module = get_amxstring(amx, params[1], 0, len);
    if(loadModule(module, g_pluginsloaded ? PT_STARTUP : PT_ANYTIME, true) > 0) {
      return 1;
    }
  }
  return 0;
}

/* 2 param */
NATIVE(print_to_admins) {
  int len;
  char* message;
  int index;
  CPlayer *pPlayer;
  
  char* sptemp = get_amxstring(amx, params[1], 0, len);
  int iFlags = UTIL_ReadFlags(sptemp);
  int channel = params[2];
  
  for(index = 1; index <= gpGlobals->maxClients; ++index) {
    pPlayer = GET_PLAYER_POINTER_I(index);
    if(pPlayer->ingame && (pPlayer->flags[0] & iFlags)) {
      g_translator.setDest(index);
      message = format_amxstring(amx, params, 3, len);
      message[len++]='\n';
      message[len]=0;
      UTIL_ClientPrint(pPlayer->pEdict, channel, message);
    }
  }
  return len;
}

/* 2 param */
NATIVE(change_cmdaccess) {
  int len;
  char* cmd = get_amxstring(amx, params[1], 0, len);

  CmdMngr::iterator a = g_commands.begin(CMD_ConsoleCommand);

  while(a) {
    if(!stricmp((*a).getCmdLine(), cmd)) {
      int access = params[2];
      bool listable = true;
      if(access < 0) { // if access is -1 then hide from listing
        access = 0;
        listable = false;
      }
      (*a).setCmdAccess(access, listable);
      g_commands.clearBufforedInfo();
      return 1;
    }
    ++a;
  }
  return 0;
}

/* 1 param */
NATIVE(log_amx) {
  g_translator.setDest(0);
  int len;
  char* msg = format_amxstring(amx, params, 1, len);
  msg[len++]='\n';
  msg[len]=0;

	char date[32];
  time_t td; time(&td);
  strftime(date, 31, "admin%Y%m%d.log", localtime(&td));

  amx_log(date, msg);
  return 1;
}

AMX_NATIVE_INFO amxmod_Natives[] = {
  { "change_tasktime",        change_tasktime },
  { "client_cmd",             client_cmd },
  { "client_print",           client_print },
  { "console_cmd",            console_cmd },
  { "console_print",          console_print },
  { "cvar_exists",            cvar_exists },
  { "emit_sound",             emit_sound },
  { "engclient_cmd",          engclient_cmd },
  { "engclient_print",        engclient_print },
  { "find_player",            find_player },
  { "force_unmodified",       force_unmodified },
  { "format_time",            format_time },
  { "get_clcmd",              get_clcmd },
  { "get_clcmdsnum",          get_clcmdsnum },
  { "get_cmdaccess",          get_cmdaccess },
  { "get_concmd",             get_concmd },
  { "get_concmdsnum",         get_concmdsnum },
  { "get_cvar_flags",         get_cvar_flags },
  { "get_cvarptr_flags",      get_cvarptr_flags },
  { "get_cvar_float",         get_cvar_float },
  { "get_cvarptr_float",      get_cvarptr_float },
  { "get_cvar_num",           get_cvar_num },
  { "get_cvarptr_num",        get_cvarptr_num },
  { "get_cvar_string",        get_cvar_string },
  { "get_cvarptr_string",     get_cvarptr_string },
  { "get_cvar_pointer",       get_cvar_pointer },
  { "get_flags",              get_flags },
  { "get_gametime",           get_gametime },
  { "get_localinfo",          get_localinfo },
  { "get_mapname",            get_mapname },
  { "get_maxplayers",         get_maxplayers },
  { "get_modname",            get_modname },
  { "get_modulesnum",         get_modulesnum },
  { "get_players",            get_players },
  { "get_playersnum",         get_playersnum },
  { "get_plugin",             get_plugin },
  { "get_plugincmd",          get_plugincmd },
  { "get_plugincmdsnum",      get_plugincmdsnum },
  { "get_plugincvar",         get_plugincvar },
  { "get_plugincvarsnum",     get_plugincvarsnum },
  { "get_pluginsnum",         get_pluginsnum },
  { "get_srvcmd",             get_srvcmd },
  { "get_srvcmdsnum",         get_srvcmdsnum },
  { "get_systime",            get_systime},
  { "get_time",               get_time },
  { "get_timeleft",           get_timeleft },
  { "get_user_aiming",        get_user_aiming },
  { "get_user_ammo",          get_user_ammo },
  { "get_user_armor",         get_user_armor },
  { "get_user_attacker",      get_user_attacker },
  { "get_user_authid",        get_user_authid },
  { "get_user_deaths",        get_user_deaths },
  { "get_user_flags",         get_user_flags },
  { "get_user_frags",         get_user_frags },
  { "get_user_health",        get_user_health },
  { "get_user_index",         get_user_index },
  { "get_user_info",          get_user_info },
  { "get_user_ip",            get_user_ip },
  { "get_user_menu",          get_user_menu },
  { "get_user_msgid",         get_user_msgid },
  { "get_user_msgname",       get_user_msgname },
  { "get_user_name",          get_user_name },
  { "get_user_origin",        get_user_origin },
  { "get_user_ping",          get_user_ping },
  { "get_user_team",          get_user_team },
  { "get_user_time",          get_user_time },
  { "get_user_userid",        get_user_userid },
  { "get_user_weapon",        get_user_weapon },
  { "get_user_weapons",       get_user_weapons},
  { "get_weaponid",           get_weaponid },
  { "get_weaponname",         get_weaponname },
  { "get_xvar_float",         get_xvar_num },
  { "get_xvar_id",            get_xvar_id },
  { "get_xvar_num",           get_xvar_num },
  { "has_user_weapon",        has_user_weapon },
  { "is_dedicated_server",    is_dedicated_server },
  { "is_linux_server",        is_linux_server },
  { "is_map_valid",           is_map_valid },
  { "is_module_running",      is_module_running },
  { "is_plugin_running",      is_plugin_running },
  { "is_running_jit",         is_running_jit },
  { "is_translated",          is_translated },
  { "is_user_alive",          is_user_alive },
  { "is_user_authorized",     is_user_authorized },
  { "is_user_bot",            is_user_bot },
  { "is_user_connected",      is_user_connected },
  { "is_user_connecting",     is_user_connecting },
  { "is_user_hltv",           is_user_hltv },
  { "log_message",            log_message },
  { "log_to_file",            log_to_file },
  { "num_to_word",            num_to_word },
  { "parse_loguser",          parse_loguser },
  { "parse_time",             parse_time },
  { "pause",                  pause },
  { "precache_model",         precache_model },
  { "precache_sound",         precache_sound },
  { "precache_generic",       precache_generic },
  { "random_float",           random_float },
  { "random_num",             random_num },
  { "read_argc",              read_argc },
  { "read_args",              read_args },
  { "read_argv",              read_argv },
  { "read_data",              read_data },
  { "read_datanum",           read_datanum },
  { "read_flags",             read_flags },
  { "read_logargc",           read_logargc },
  { "read_logargv",           read_logargv },
  { "read_logdata",           read_logdata },
  { "register_clcmd",         register_clcmd },
  { "register_concmd",        register_concmd },
  { "register_cvar",          register_cvar },
  { "register_event",         register_event },
  { "register_logevent",      register_logevent },
  { "register_menucmd",       register_menucmd },
  { "register_menuid",        register_menuid },
  { "register_plugin",        register_plugin },
  { "register_srvcmd",        register_srvcmd },
  { "remove_cvar_flags",      remove_cvar_flags },
  { "remove_quotes",          remove_quotes },
  { "remove_task",            remove_task },
  { "remove_user_flags",      remove_user_flags },
  { "server_cmd",             server_cmd },
  { "server_exec",            server_exec },
  { "server_print",           server_print },
  { "set_cvar_flags",         set_cvar_flags },
  { "set_cvarptr_flags",      set_cvarptr_flags },
  { "set_cvar_float",         set_cvar_float },
  { "set_cvarptr_float",      set_cvarptr_float },
  { "set_cvar_num",           set_cvar_num },
  { "set_cvarptr_num",        set_cvarptr_num },
  { "set_cvar_string",        set_cvar_string },
  { "set_cvarptr_string",     set_cvarptr_string },
  { "set_hudmessage",         set_hudmessage },
  { "set_localinfo",          set_localinfo },
  { "set_task",               set_task },
  { "set_user_flags",         set_user_flags },
  { "set_user_info",          set_user_info },
  { "set_user_weapon",        set_user_weapon },
  { "set_xvar_float",         set_xvar_num },
  { "set_xvar_num",           set_xvar_num },
  { "show_hudmessage",        show_hudmessage },
  { "show_menu",              show_menu },
  { "show_motd",              show_motd },
  { "task_exists",            task_exists },
  { "unpause",                unpause },
  { "user_kill",              user_kill },
  { "user_slap",              user_slap },
  { "xvar_exists",            xvar_exists },
  { "translate",              translate },
  { "set_user_language",      set_user_language },
  { "get_user_language",      get_user_language },
  { "set_user_languagenum",   set_user_languagenum },
  { "get_user_languagenum",   get_user_languagenum },
  { "is_language_known",      is_language_known },
  { "get_languagesnum",       get_languagesnum },
  { "get_language",           get_language },
  { "load_translations",      load_translations },
  { "get_language_support",   get_language_support },
  { "get_languagenum_support",get_languagenum_support },
  { "report_error",           rpt_error },
  { "query_client_cvar",      query_client_cvar },
  { "arrayset",               arrayset },
  { "arraysetfloat",          arrayset },
  { "load_module",            load_module },
  { "print_to_admins",        print_to_admins },
  { "change_cmdaccess",       change_cmdaccess },
	{ "log_amx",                log_amx },
  { NULL,                     NULL }
};
