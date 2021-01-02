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

#include "fun.h"

NATIVE(set_user_godmode) { /* 2 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    if(params[2]) {
      pPlayer->pEdict->v.takedamage = DAMAGE_NO;
    }
    else {
      pPlayer->pEdict->v.takedamage = DAMAGE_AIM;
    }
    return 1;
  }
  return 0;
}

NATIVE(set_user_frags) { /* 2 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    pPlayer->pEdict->v.frags = params[2];
    if(g_bmod_cstrike) {
      MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
      WRITE_BYTE( index );
      WRITE_SHORT( (int)params[2] );
      WRITE_SHORT( pPlayer->deaths );
      WRITE_SHORT( 0 );
      WRITE_SHORT( pPlayer->team );
      MESSAGE_END();
    }
    return 1;
  }
  return 0;
}

NATIVE(set_user_armor) { /* 2 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    int armorvalue = params[2];
    if (armorvalue < 0) armorvalue = 0;
    pPlayer->pEdict->v.armorvalue = armorvalue;
    return 1;
  }
  return 0;
}

NATIVE(set_user_health) { /* 2 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    if(params[2] > 0) {
      pPlayer->pEdict->v.health = float(params[2]);
    }
    else {
      MDLL_ClientKill(pPlayer->pEdict);
    }
    return 1;
  }
  return 0;
}

NATIVE(set_user_origin) { /* 2 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    Vector origin;
    cell *cpOrigin = MF_GetAmxAddr(amx,params[2]);
    origin.x = *cpOrigin;
    origin.y = *(cpOrigin+1);
    origin.z = *(cpOrigin+2);
    SET_ORIGIN(pPlayer->pEdict,origin);
    return 1;
  }
  return 0;
}

NATIVE(set_user_rendering) { /* 7 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    int temp = params[2];
    if (pPlayer->pEdict->v.renderfx!=temp){
      pPlayer->pEdict->v.renderfx=temp;
    }
    pPlayer->pEdict->v.rendercolor.x = (byte)params[3];
    pPlayer->pEdict->v.rendercolor.y = (byte)params[4];
    pPlayer->pEdict->v.rendercolor.z = (byte)params[5];
    temp = params[6];
    if(pPlayer->pEdict->v.rendermode!=temp) {
      pPlayer->pEdict->v.rendermode=temp;
    }
    pPlayer->pEdict->v.renderamt = (byte)params[7];
    return 1;
  }
  return 0;
}

NATIVE(set_user_maxspeed) { /* 2 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    float maxspeed = *(float *)((void *)&params[2]);
    SETCLIENTMAXSPEED(pPlayer->pEdict, maxspeed);
    pPlayer->pEdict->v.maxspeed = maxspeed;
    return 1;
  }
  return 0;
}

NATIVE(get_user_maxspeed) { /* 1 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    float pFloat = 0.0f;
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    pFloat = pPlayer->pEdict->v.maxspeed;
    return FloatToCell(pFloat);
  }
  return 0;
}

NATIVE(set_user_gravity) { /* 2 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    pPlayer->pEdict->v.gravity = *(float *)((void *)&params[2]);
    return 1;
  }
  return 0;
}

NATIVE(get_user_gravity) { /* 1 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    float pFloat = 0.0f;
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    pFloat = pPlayer->pEdict->v.gravity;
    return FloatToCell(pFloat);
  }
  return 0;
}

NATIVE(get_user_godmode) { /* 1 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    return(pPlayer->pEdict->v.takedamage == DAMAGE_NO) ? 1 : 0;
  }
  return 0;
}

NATIVE(set_user_noclip) { /* 2 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    if(pPlayer->IsAlive()) {
      if(params[2] == 1) {
        pPlayer->pEdict->v.movetype = MOVETYPE_NOCLIP;
      }
      else {
        pPlayer->pEdict->v.movetype = MOVETYPE_WALK;
      }
      return 1;
    }
  }
  return 0;
}

NATIVE(get_user_noclip) { /* 1 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    return(pPlayer->pEdict->v.movetype == MOVETYPE_NOCLIP) ? 1 : 0;
  }
  return 0;
}

NATIVE(get_user_hitzones) { /* 2 param */
  int index = params[1];
  if(index < 0 || index > gpGlobals->maxClients) {
    MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid player id %d", index);
    return 0;
  }
  int target = params[2];
  if(target < 0 || target > gpGlobals->maxClients) {
    MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid target id %d", index);
    return 0;
  }
  if(index) {
    int hitzones = (int)GET_PLAYER_POINTER_I(index)->bodyhits[target];
    return (hitzones == 1) ? -1 : hitzones;
  }
  else {
    bool same = true;
    int hitzones = -1;
    for(int i = 1; i <= gpGlobals->maxClients; ++i) {
      if(hitzones == -1) {
        hitzones = (int)GET_PLAYER_POINTER_I(i)->bodyhits[target];
      }
      else {
        if((int)GET_PLAYER_POINTER_I(i)->bodyhits[target] != hitzones) {
          same = false;
        }
      }
    }
    if(same) {
      return (hitzones == 1) ? -1 : hitzones;
    }
    else {
      return -1;
    }
  }
  return 0;
}

NATIVE(set_user_hitzones) { /* 3 param */
  int index = params[1];
  if(index < 0 || index > gpGlobals->maxClients) {
    MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid player id %d", index);
    return 0;
  }
  int target = params[2];
  if(target < 0 || target > gpGlobals->maxClients) {
    MF_AmxLogError(amx, AMX_ERR_NATIVE, "Invalid target id %d", index);
    return 0;
  }
  int hitzones = params[3];

  if(g_pengfuncsTable->pfnTraceLine == NULL) {
    g_pengfuncsTable->pfnTraceLine = TraceLine;
  }

  if(index) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    if(pPlayer->ingame) {
      if(target) {
        pPlayer->bodyhits[target] = hitzones;
        if((int)pPlayer->bodyhits[0] != hitzones) pPlayer->bodyhits[0] = 1;
      }
      else {
        for(int i = 0; i <= gpGlobals->maxClients; ++i) {
          pPlayer->bodyhits[i] = hitzones;
        }
      }
      if(disableTraceLine() == true) {
        g_pengfuncsTable->pfnTraceLine = NULL;
      }
      return 1;
    }
    if(disableTraceLine() == true) {
      g_pengfuncsTable->pfnTraceLine = NULL;
    }
    return 0;
  }
  else {
    player_t* pPlayer;
    for(int i = 1; i <= gpGlobals->maxClients; ++i) {
      pPlayer = GET_PLAYER_POINTER_I(i);
      if(target) {
        pPlayer->bodyhits[target] = hitzones;
        if((int)pPlayer->bodyhits[0] != hitzones) pPlayer->bodyhits[0] = 1;
      }
      else {
        for(int i = 0; i <= gpGlobals->maxClients; ++i) {
          pPlayer->bodyhits[i] = hitzones;
        }
      }
    }
  }
  if(disableTraceLine() == true) {
    g_pengfuncsTable->pfnTraceLine = NULL;
  }
  return 1;
}

NATIVE(user_spawn) { /* 1 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    pPlayer->pEdict->v.deadflag = DEAD_RESPAWNABLE;
    MDLL_Spawn(pPlayer->pEdict);
    pPlayer->pEdict->v.iuser1 = 0;
    return 1;
  }
  return 0;
}

Slist itemBase;

NATIVE(give_item) {
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    if(pPlayer->IsAlive()) {
      int iLen;
      char* szItem = MF_GetAmxString(amx,params[2],0,iLen);
      if(strncmp(szItem,"weapon_",7) && strncmp(szItem,"ammo_",5) && strncmp(szItem,"item_",5)) {
        return 0;
      }
      char* name = (char*)itemBase.set( szItem );
      edict_t *edict = CREATE_NAMED_ENTITY( MAKE_STRING( name ) );

      if( FNullEnt(edict)) {
        itemBase.erase( szItem );
        return 0;
      }

      edict->v.origin = pPlayer->pEdict->v.origin;
      edict->v.spawnflags |= ( 1 << 30 );
      MDLL_Spawn(edict);

      int before = edict->v.solid;
      MDLL_Touch(edict,pPlayer->pEdict);

      if( edict->v.solid == before ) {
        REMOVE_ENTITY( edict );
      }
      return ENTINDEX(edict);
    }
  }
  return 0;
}

NATIVE(get_user_money) {
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    void* ptr = GET_PRIVATE(pPlayer->pEdict);

    if(gmsgMoney && ptr) {
#ifdef LINUX
      int *a = (int*)ptr + 0x78;
#else
      int *a = (int*)ptr + 0x73;
#endif
      return *a;
    }
  }
  return 0;
}

NATIVE(set_user_money) {
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    void* ptr = GET_PRIVATE(pPlayer->pEdict);

    if(gmsgMoney && ptr) {
#ifdef LINUX
      int *a = (int*)ptr + 0x78;
#else
      int *a = (int*)ptr + 0x73;
#endif
      *a = params[2];

      MESSAGE_BEGIN( MSG_ONE, gmsgMoney, NULL, pPlayer->pEdict );
      WRITE_LONG( *a );
      WRITE_BYTE( params[3] ? 1 : 0 );
      MESSAGE_END();
      return 1;
    }
  }
  return 0;
}

NATIVE(set_user_footsteps) { /* 2 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    if(params[2]) {
      pPlayer->pEdict->v.flTimeStepSound = 999;
      pPlayer->nofootsteps = true;
      if(g_pFunctionTable->pfnPlayerPostThink == NULL) {
        g_pFunctionTable->pfnPlayerPostThink = PlayerPostThink;
      }
    }
    else {
      pPlayer->pEdict->v.flTimeStepSound = 400;
      pPlayer->nofootsteps = false;
      if(countFootSteps() == 0 && countModelled() == 0) {
        g_pFunctionTable->pfnPlayerPostThink = NULL;
      }
    }
    return 1;
  }
  return 0;
}

NATIVE(get_user_footsteps) { /* 1 param */
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    return(pPlayer->nofootsteps) ? 1 : 0;
  }
  return 0;
}

NATIVE(get_user_deaths_cs) {
  if(!g_bmod_cstrike) {
    MF_AmxLogError(amx, AMX_ERR_NATIVE, "get_user_deaths_cs can be used only with Counter-Strike");
    return 0;
  }
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    void* ptr = GET_PRIVATE(pPlayer->pEdict);

    if(ptr) {
#ifdef LINUX
      int *a = (int*)ptr + 0x1C1;
#else
      int *a = (int*)ptr + 0x1BC;
#endif
      return *a;
    }
  }
  return 0;
}

NATIVE(set_user_deaths_cs) {
  if(!g_bmod_cstrike) {
    MF_AmxLogError(amx, AMX_ERR_NATIVE, "set_user_deaths_cs can be used only with Counter-Strike");
    return 0;
  }
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    void* ptr = GET_PRIVATE(pPlayer->pEdict);

    if(ptr) {
#ifdef LINUX
      int *a = (int*)ptr + 0x1C1;
#else
      int *a = (int*)ptr + 0x1BC;
#endif
      *a = params[2];
      MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
      WRITE_BYTE( index );
      WRITE_SHORT( (int)pPlayer->pEdict->v.frags );
      WRITE_SHORT( (int)params[2] );
      WRITE_SHORT( 0 );
      WRITE_SHORT( pPlayer->team );
      MESSAGE_END();
      return 1;
    }
  }
  return 0;
}

NATIVE(get_user_team_cs) {
  if(!g_bmod_cstrike) {
    MF_AmxLogError(amx, AMX_ERR_NATIVE, "get_user_team_cs can be used only with Counter-Strike");
    return 0;
  }
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    void* ptr = GET_PRIVATE(pPlayer->pEdict);

    if(ptr) {
#ifdef LINUX
      int *a = (int*)ptr + 0x77;
#else
      int *a = (int*)ptr + 0x72;
#endif
      return *a;
    }
  }
  return 0;
}

NATIVE(set_user_team_cs) {
  if(!g_bmod_cstrike) {
    MF_AmxLogError(amx, AMX_ERR_NATIVE, "set_user_team_cs can be used only with Counter-Strike");
    return 0;
  }
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    void* ptr = GET_PRIVATE(pPlayer->pEdict);

    if(ptr) {
      int team = params[2];
      if(team >= 0 && team <= 3) {

        #ifdef LINUX
        int *a = (int*)ptr + 0x77;
        #else
        int *a = (int*)ptr + 0x72;
        #endif
        *a = team;

        if(pPlayer->IsAlive()) {
          UTIL_FakeClientCommand(pPlayer->pEdict, "drop", "weapon_c4");

          if(team != 2) {
            #ifdef LINUX
            int *b = (int*)ptr + 0xC6;
            #else
            int *b = (int*)ptr + 0xC1;
            #endif
            if(*b & (1<<16)) {
              *b -= (1<<16);
            }

            MESSAGE_BEGIN( MSG_ONE, gmsgStatusIcon, NULL, pPlayer->pEdict );
            WRITE_BYTE( 0 );
            WRITE_STRING( "defuser" );
            WRITE_BYTE( 0 );
            WRITE_BYTE( 255 );
            WRITE_BYTE( 0 );
            MESSAGE_END();
          }

          int iLen;
          char* szModel = MF_GetAmxString(amx, params[3], 0, iLen);
          if(strcmp(szModel, "") != 0) {
            strcpy(pPlayer->modelname, szModel);
            pPlayer->modeled = true;
            if(g_pFunctionTable->pfnPlayerPostThink == NULL) {
              g_pFunctionTable->pfnPlayerPostThink = PlayerPostThink;
            }
            if(g_pFunctionTable->pfnClientUserInfoChanged == NULL) {
              g_pFunctionTable->pfnClientUserInfoChanged = ClientUserInfoChanged;
            }
            SET_CLIENT_KEYVALUE(pPlayer->index, GET_INFO_KEYBUFFER(pPlayer->pEdict), "model", pPlayer->modelname);
          }
        }

        if(params[4]) {
          float health = pPlayer->pEdict->v.health;
          float armorvalue = pPlayer->pEdict->v.armorvalue;
          pPlayer->pEdict->v.deadflag = DEAD_RESPAWNABLE;
          MDLL_Spawn(pPlayer->pEdict);
          pPlayer->pEdict->v.iuser1 = 0;
          pPlayer->pEdict->v.health = health;
          pPlayer->pEdict->v.armorvalue = armorvalue;
        }

        if(params[5]) {
          char szTeam[32];
          switch(team) {
            case 0:
              strcpy(szTeam, "UNASSIGNED");
              break;
            case 1:
              strcpy(szTeam, "TERRORIST");
              break;
            case 2:
              strcpy(szTeam, "CT");
              break;
            case 3:
              strcpy(szTeam, "SPECTATOR");
              break;
          }

          MESSAGE_BEGIN( MSG_ALL, gmsgTeamInfo );
          WRITE_BYTE( index );
          WRITE_STRING( szTeam );
          MESSAGE_END();
        }
        return 1;
      }
    }
  }
  return 0;
}

NATIVE(get_client_listen) {
  if(MF_IsPlayer(params[1], 1) && MF_IsPlayer(params[2], 1)) {
		return GETCLIENTLISTENING(params[1], params[2]);
  }
  return 0;
}

NATIVE(set_client_listen) {
  if(MF_IsPlayer(params[1], 1) && MF_IsPlayer(params[2], 1)) {
    return SETCLIENTLISTENING(params[1], params[2], params[3]);
	}
  return 0;
}

// By AMXMODX Dev Team
NATIVE(strip_user_weapons) {
  int index = params[1];
  if(MF_IsPlayer(index, 1)) {
    player_t* pPlayer = GET_PLAYER_POINTER_I(index);
    string_t item = MAKE_STRING("player_weaponstrip");
    edict_t *pEntity = CREATE_NAMED_ENTITY(item);
    if(!FNullEnt(pEntity)) {
      MDLL_Spawn(pEntity);
      MDLL_Use(pEntity, pPlayer->pEdict);
      REMOVE_ENTITY(pEntity);
      *reinterpret_cast<int *>(MF_PlayerPropAddr(params[1], Player_CurrentWeapon)) = 0;
      return 1;
    }
  }
  return 0;
}

AMX_NATIVE_INFO fun_natives[] = {
  {"get_user_deaths_cs",  get_user_deaths_cs},
  {"get_user_footsteps",  get_user_footsteps},
  {"get_user_godmode",    get_user_godmode},
  {"get_user_gravity",    get_user_gravity},
  {"get_user_hitzones",   get_user_hitzones},
  {"get_user_maxspeed",   get_user_maxspeed},
  {"get_user_money",      get_user_money},
  {"get_user_noclip",     get_user_noclip},
  {"get_user_team_cs",    get_user_team_cs},
	{"get_client_listen",   get_client_listen},
  {"give_item",           give_item},
  {"set_user_armor",      set_user_armor},
  {"set_user_deaths_cs",  set_user_deaths_cs},
  {"set_user_footsteps",  set_user_footsteps},
  {"set_user_frags",      set_user_frags},
  {"set_user_godmode",    set_user_godmode},
  {"set_user_gravity",    set_user_gravity},
  {"set_user_health",     set_user_health},
  {"set_user_hitzones",   set_user_hitzones},
  {"set_user_maxspeed",   set_user_maxspeed},
  {"set_user_money",      set_user_money},
  {"set_user_noclip",     set_user_noclip},
  {"set_user_origin",     set_user_origin},
  {"set_user_rendering",  set_user_rendering},
  {"set_user_team_cs",    set_user_team_cs},
	{"set_client_listen",   set_client_listen},
  {"strip_user_weapons",  strip_user_weapons},
  {"user_spawn",          user_spawn},
  {NULL,                  NULL}
};
